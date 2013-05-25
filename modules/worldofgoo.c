
/**
 * apkenv
 * Copyright (c) 2012, Thomas Perl <m@thp.io>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
 * IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **/

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <zlib.h>

#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>
#include <EGL/egl.h>

#include "common.h"

typedef void (*worldofgoo_init_t)(JNIEnv *env, jobject obj) SOFTFP;
typedef void (*worldofgoo_oncreate_t)(JNIEnv *env, jobject obj, jboolean a, jboolean is_demo, jboolean c) SOFTFP;
typedef void (*worldofgoo_resize_t)(void *env, jobject obj, jint width, jint height) SOFTFP;
typedef void (*worldofgoo_render_t)(void *env, jobject obj, jboolean menu_pressed, jboolean back_pressed) SOFTFP;
typedef void (*worldofgoo_input_t)(void *env, jobject obj, jint event, jfloat x, jfloat y, jint index) SOFTFP;
typedef void (*worldofgoo_ondestroy_t)(void *env, jobject obj, jboolean config_change) SOFTFP;

struct SupportModulePriv {
    worldofgoo_oncreate_t nativeOnCreate;
    worldofgoo_init_t nativeOnSurfaceCreated;
    worldofgoo_resize_t nativeResize;
    worldofgoo_render_t nativeRender;
    worldofgoo_input_t nativeTouchEvent;
    worldofgoo_ondestroy_t nativeOnDestroy;

    const char *home_directory;
    const void *apk_in_mem;
};
static struct SupportModulePriv worldofgoo_priv;


struct ZipFileIndex {
    char filename[1024];
    int offset;
    int length;
};

static struct ZipFileIndex apk_index[4096];
static int apk_index_pos;

void
build_apk_index(const char *filename)
{
    size_t offset = 0;
    FILE *fp = fopen(filename, "rb");

    int compressed_size;
    int uncompressed_size;
    uint16_t filename_length;
    uint16_t extrafield_length;

    apk_index_pos = 0;
    while (!feof(fp)) {
        fseek(fp, offset, SEEK_SET);
        if (fgetc(fp) != 'P') break;
        if (fgetc(fp) != 'K') break;
        if (fgetc(fp) != 3) break;
        if (fgetc(fp) != 4) break;

        fseek(fp, offset+18, SEEK_SET);
        fread(&compressed_size, sizeof(int), 1, fp);
        fread(&uncompressed_size, sizeof(int), 1, fp);
        fread(&filename_length, sizeof(uint16_t), 1, fp);
        fread(&extrafield_length, sizeof(uint16_t), 1, fp);

        if (compressed_size == uncompressed_size) {
            if (apk_index_pos >= sizeof(apk_index) / sizeof(apk_index[0])) {
                fprintf(stderr, "apk_index too small, increase and recompile\n");
                exit(1);
            }
            fseek(fp, offset+30, SEEK_SET);
            fread(apk_index[apk_index_pos].filename, sizeof(char), filename_length, fp);
            apk_index[apk_index_pos].filename[filename_length] = '\0';

            apk_index[apk_index_pos].offset = offset+30+filename_length+extrafield_length;
            apk_index[apk_index_pos].length = compressed_size;
            apk_index_pos++;
        }

        offset += 30+filename_length+extrafield_length+compressed_size;
    }
    fclose(fp);
    printf("Built APK index: %d entries\n", apk_index_pos);
}

static void lame_resample_44100_32000(Mix_Chunk *chunk)
{
    short *smp = (short *)chunk->abuf;
    int d, s, counter = 0;

    for (s = d = 0; s < chunk->alen / sizeof(*smp); d++) {
        smp[d] = smp[s];

        counter += 44100;
        while (counter >= 32000) {
            counter -= 32000;
            s++;
        }
    }
    chunk->alen = d * sizeof(*smp);
}

struct player_sound {
    char *filename;
    SDL_RWops *rw;
    Mix_Music *music;
    Mix_Chunk *chunk;
    int chunk_channel;
    struct player_sound *next;
};

static struct player_sound *sounds[512];

/* as SDL_mixer can only play 1 music at a time, and WOG does crossfades,
 * we must track currectly active music.. */
static Mix_Music *active_music;

static void
load_sound(const char *filename)
{
    const struct ZipFileIndex *zip_index = NULL;
    struct player_sound *sound;
    unsigned long hash;
    const char *mem;
    int loaded = 0;
    int i;

    hash = crc32(0, (void *)filename, strlen(filename));
    hash %= sizeof(sounds) / sizeof(sounds[0]);
    for (sound = sounds[hash]; sound != NULL; sound = sound->next) {
        if (strcmp(filename, sound->filename) == 0)
            break;
    }

    if (sound != NULL) {
        fprintf(stderr, "load_sound: %s already loaded?\n", filename);
        return;
    }

    for (i = 0; i < apk_index_pos; i++) {
        if (strstr(apk_index[i].filename, filename) != NULL) {
            zip_index = &apk_index[i];
            break;
        }
    }

    if (zip_index == NULL) {
        fprintf(stderr, "load_sound: %s not in apk?\n", filename);
        return;
    }

    sound = calloc(1, sizeof(*sound));
    if (sound == NULL)
        return;

    sound->filename = strdup(filename);
    sound->chunk_channel = -1;
    mem = (const char *)worldofgoo_priv.apk_in_mem + zip_index->offset;
    sound->rw = SDL_RWFromMem((void *)mem, zip_index->length);
    if (strstr(filename, "music/") != NULL) {
        sound->music = Mix_LoadMUS_RW(sound->rw);
        loaded = (sound->music != NULL);
    } else {
        sound->chunk = Mix_LoadWAV_RW(sound->rw, 0);
        if (sound->chunk != NULL) {
            loaded = 1;
            /* SDL_mixer can't resample 44100 to 32000 Hz :( */
            if (strncmp(mem, "OggS", 4) == 0 && *(unsigned short *)(mem + 0x28) == 44100)
                lame_resample_44100_32000(sound->chunk);
        }
    }
    if (loaded) {
        MODULE_DEBUG_PRINTF("loaded %s\n", filename);

        sound->next = sounds[hash];
        sounds[hash] = sound;
    } else {
        fprintf(stderr, "failed to load %s\n", filename);
        free(sound);
    }
}

static void *
play_sound(const char *filename, int loop, double volume)
{
    struct player_sound *sound;
    unsigned long hash;
    int sdl_volume = (int)(volume * MIX_MAX_VOLUME);

    hash = crc32(0, (void *)filename, strlen(filename));
    hash %= sizeof(sounds) / sizeof(sounds[0]);
    for (sound = sounds[hash]; sound != NULL; sound = sound->next) {
        if (strcmp(filename, sound->filename) == 0)
            break;
    }

    if (sound == NULL) {
        fprintf(stderr, "play_sound: %s not loaded?\n", filename);
        return NULL;
    }

    if (sound->chunk != NULL) {
        Mix_VolumeChunk(sound->chunk, sdl_volume);
        sound->chunk_channel = Mix_PlayChannel(-1, sound->chunk, loop ? -1 : 0);
    } else if (sound->music != NULL) {
        Mix_HaltMusic();
        active_music = sound->music;
        Mix_VolumeMusic(sdl_volume);
        Mix_PlayMusic(sound->music, loop ? -1 : 1);
    }
    return sound;
}

jobject
worldofgoo_CallObjectMethodV(JNIEnv *env, jobject p1, jmethodID p2, va_list p3) SOFTFP;

static void
worldofgoo_CallVoidMethodV(JNIEnv *env, jobject p1, jmethodID p2, va_list p3) SOFTFP;

jlong
worldofgoo_CallLongMethodV(JNIEnv *p0, jobject p1, jmethodID p2, va_list p3) SOFTFP;

jboolean
worldofgoo_CallBooleanMethodV(JNIEnv* p0, jobject p1, jmethodID p2, va_list p3) SOFTFP;

jint
worldofgoo_AttachCurrentThread(JavaVM *, JNIEnv **, void *) SOFTFP;

jint
worldofgoo_DetachCurrentThread(JavaVM *vm) SOFTFP;


jobject
worldofgoo_CallObjectMethodV(JNIEnv *env, jobject p1, jmethodID p2, va_list p3)
{
    MODULE_DEBUG_PRINTF("worldofgoo_CallObjectMethodV %x %s\n", p1, p2->name);
    if (strcmp(p2->name, "getApkPath") == 0) {
        return (*env)->NewStringUTF(env, GLOBAL_J(env)->apk_filename);
    } else if (strcmp(p2->name, "getExternalStoragePath") == 0) {
        return (*env)->NewStringUTF(env, worldofgoo_priv.home_directory);
    } else if (strcmp(p2->name, "getInternalStoragePath") == 0) {
        return (*env)->NewStringUTF(env, worldofgoo_priv.home_directory);
    } else if (strcmp(p2->name, "getLanguage") == 0) {
        return (*env)->NewStringUTF(env, "");
    } else if (strcmp(p2->name, "playSound") == 0) {
        struct dummy_jstring *name = va_arg(p3, struct dummy_jstring *);
        int loop = va_arg(p3, int);
        double volume = va_arg(p3, double);
        void *ret;
        MODULE_DEBUG_PRINTF("playSound '%s', %d, %f\n", name->data, loop, volume);
        ret = play_sound(name->data, loop, volume);
        MODULE_DEBUG_PRINTF(" = %p\n", ret);
        return ret;
    } else {
        fprintf(stderr, "Do not know what to do: %s\n", p2->name);
    }
    return GLOBAL_J(env);
}

static void
worldofgoo_CallVoidMethodV(JNIEnv *env, jobject p1, jmethodID p2, va_list p3)
{
    MODULE_DEBUG_PRINTF("worldofgoo_CallVoidMethodV(%x, %s, %s)\n", p1, p2->name, p2->sig);

    if (strcmp(p2->name, "loadSound") == 0) {
        struct dummy_jstring *arg = va_arg(p3, struct dummy_jstring *);
        int boo = va_arg(p3, int);
        MODULE_DEBUG_PRINTF("loadSound %s, %d\n", arg->data, boo);
        load_sound(arg->data);
    } else if (strcmp(p2->name, "setSoundVolume") == 0) {
        struct player_sound *sound = va_arg(p3, struct player_sound *);
        double volume = va_arg(p3, double);
        int sdl_volume = (int)(volume * MIX_MAX_VOLUME);
        MODULE_DEBUG_PRINTF("setSoundVolume %p %.3f\n", sound, volume);
        if (sound->chunk != NULL)
            Mix_VolumeChunk(sound->chunk, sdl_volume);
        else if (sound->music != NULL && sound->music == active_music)
            Mix_VolumeMusic(sdl_volume);
    } else if (strcmp(p2->name, "stopSound") == 0) {
        struct player_sound *sound = va_arg(p3, struct player_sound *);
        MODULE_DEBUG_PRINTF("stopSound %p\n", sound);
        if (sound->chunk != NULL) {
            if (sound->chunk_channel >= 0) {
                Mix_HaltChannel(sound->chunk_channel);
                sound->chunk_channel = -1;
            }
        } else if (sound->music != NULL && sound->music == active_music) {
            Mix_HaltMusic();
            active_music = NULL;
        }
    } else {
        fprintf(stderr, "Do not know what to do: %s\n", p2->name);
    }
}

jlong
worldofgoo_CallLongMethodV(JNIEnv *p0, jobject p1, jmethodID p2, va_list p3)
{
    MODULE_DEBUG_PRINTF("worldofgoo_CallLongMethodV %s\n", p2->name);
    struct dummy_jstring *str = va_arg(p3, struct dummy_jstring*);
    int i;
    if (strcmp(p2->name, "getAssetFileOffset") == 0) {
        for (i = 0; i < apk_index_pos; i++) {
            if (strstr(apk_index[i].filename, str->data) != NULL) {
                return apk_index[i].offset;
            }
        }
        fprintf(stderr, "not found: %s\n", str->data);
        return -1;
    } else if (strcmp(p2->name, "getAssetFileLength") == 0) {
        for (i = 0; i < apk_index_pos; i++) {
            if (strstr(apk_index[i].filename, str->data) != NULL) {
                return apk_index[i].length;
            }
        }
        fprintf(stderr, "not found: %s\n", str->data);
        return -1;
    } else {
        fprintf(stderr, "Do not know what to do: %s\n", p2->name);
    }
    return 0;
}

jboolean
worldofgoo_CallBooleanMethodV(JNIEnv* p0, jobject p1, jmethodID p2, va_list p3)
{
    MODULE_DEBUG_PRINTF("worldofgoo_CallBooleanMethodV %s\n", p2->name);
    if (strcmp(p2->name, "isGlThread") == 0) {
        MODULE_DEBUG_PRINTF("isGlThread: %x\n", eglGetCurrentContext());
        return eglGetCurrentContext() != 0;
    } else if (strcmp(p2->name, "isLargeScreen") == 0) {
        return 0;
    } else {
        fprintf(stderr, "Do not know what to do: %s\n", p2->name);
    }
    return 0;
}

jint
worldofgoo_AttachCurrentThread(JavaVM *vm, JNIEnv **env, void *args)
{
    MODULE_DEBUG_PRINTF("worldofgoo_AttachCurrentThread()\n");
    struct GlobalState *global = (*vm)->reserved0;
    *env = ENV(global);
    return 0;
}

jint
worldofgoo_DetachCurrentThread(JavaVM *vm)
{
    MODULE_DEBUG_PRINTF("worldofgoo_DetachCurrentThread()\n");
    return 0;
}


static int
worldofgoo_try_init(struct SupportModule *self)
{
    self->priv->nativeOnCreate = (worldofgoo_oncreate_t)LOOKUP_M("nativeOnCreate");
    self->priv->nativeOnSurfaceCreated = (worldofgoo_init_t)LOOKUP_M("nativeOnSurfaceCreated");
    self->priv->nativeResize = (worldofgoo_resize_t)LOOKUP_M("nativeResize");
    self->priv->nativeRender = (worldofgoo_render_t)LOOKUP_M("nativeRender");
    self->priv->nativeTouchEvent = (worldofgoo_input_t)LOOKUP_M("nativeTouchEvent");
    self->priv->nativeOnDestroy = (worldofgoo_ondestroy_t)LOOKUP_M("nativeOnDestroy");

    self->override_env.CallObjectMethodV = worldofgoo_CallObjectMethodV;
    self->override_env.CallVoidMethodV = worldofgoo_CallVoidMethodV;
    self->override_env.CallLongMethodV = worldofgoo_CallLongMethodV;
    self->override_env.CallBooleanMethodV = worldofgoo_CallBooleanMethodV;
    self->override_vm.AttachCurrentThread = worldofgoo_AttachCurrentThread;
    self->override_vm.DetachCurrentThread = worldofgoo_DetachCurrentThread;

    return (self->priv->nativeOnCreate != NULL &&
            self->priv->nativeOnSurfaceCreated != NULL &&
            self->priv->nativeResize != NULL &&
            self->priv->nativeRender != NULL &&
            self->priv->nativeTouchEvent != NULL &&
            self->priv->nativeOnDestroy != NULL);
}

static void
worldofgoo_init(struct SupportModule *self, int width, int height, const char *home)
{
    self->priv->home_directory = home;
    self->priv->apk_in_mem = GLOBAL_M->apk_in_mem;
    build_apk_index(GLOBAL_M->apk_filename);

    Mix_Init(MIX_INIT_OGG);

    /* init sound, must use 32000Hz because music is at that rate,
     * and SDL_mixer doesn't resample to/from 32000 */
    if (Mix_OpenAudio(32000, AUDIO_S16SYS, 2, 1024) < 0)
    {
        fprintf(stderr, "Mix_OpenAudio failed: %s.\n", Mix_GetError());
        exit(1);
    }

    self->priv->nativeOnCreate(ENV_M, GLOBAL_M, JNI_FALSE, JNI_TRUE, JNI_FALSE);
    self->priv->nativeOnSurfaceCreated(ENV_M, GLOBAL_M);
    self->priv->nativeResize(ENV_M, GLOBAL_M, width, height);
}

static void
worldofgoo_input(struct SupportModule *self, int event, int x, int y, int finger)
{
    self->priv->nativeTouchEvent(ENV_M, GLOBAL_M, event, x, y, finger);
}

static void
worldofgoo_key_input(struct SupportModule *self, int event, int keycode, int unicode)
{
}

static void
worldofgoo_update(struct SupportModule *self)
{
    self->priv->nativeRender(ENV_M, GLOBAL_M, JNI_FALSE, JNI_FALSE);
}

static void
worldofgoo_deinit(struct SupportModule *self)
{
    self->priv->nativeOnDestroy(ENV_M, GLOBAL_M, JNI_FALSE);
}

static void
worldofgoo_pause(struct SupportModule *self)
{
}

static void
worldofgoo_resume(struct SupportModule *self)
{
}

static int
worldofgoo_requests_exit(struct SupportModule *self)
{
    return 0;
}

APKENV_MODULE(worldofgoo, MODULE_PRIORITY_GAME)

