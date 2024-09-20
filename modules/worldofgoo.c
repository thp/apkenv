
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

#include <pthread.h>

#include "common.h"
#include "../mixer/mixer.h"

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

struct GlobalState *global;

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
        if (fread(&compressed_size, sizeof(int), 1, fp) != 1) break;
        if (fread(&uncompressed_size, sizeof(int), 1, fp) != 1) break;
        if (fread(&filename_length, sizeof(uint16_t), 1, fp) != 1) break;
        if (fread(&extrafield_length, sizeof(uint16_t), 1, fp) != 1) break;

        if (compressed_size == uncompressed_size) {
            if (apk_index_pos >= sizeof(apk_index) / sizeof(apk_index[0])) {
                fprintf(stderr, "apk_index too small, increase and recompile\n");
                exit(1);
            }
            fseek(fp, offset+30, SEEK_SET);
            if (fread(apk_index[apk_index_pos].filename, filename_length, 1, fp) != 1) break;
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

struct player_sound {
    char *filename;
    struct MixerMusic *music;
    struct MixerSound *chunk;
    struct player_sound *next;
};

static struct player_sound *sounds[512];

/* as SDL_mixer can only play 1 music at a time, and WOG does crossfades,
 * we must track currectly active music.. */
static struct MixerMusic *active_music;

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
    mem = (const char *)worldofgoo_priv.apk_in_mem + zip_index->offset;
    if (strstr(filename, "music/") != NULL) {
        sound->music = apkenv_mixer_load_music_buffer((void*)mem, zip_index->length);
        loaded = (sound->music != NULL);
    } else {
        sound->chunk = apkenv_mixer_load_sound_buffer((void*)mem, zip_index->length);
        apkenv_mixer_set_sound_channel(sound->chunk, -1);
        if (sound->chunk != NULL) {
            loaded = 1;
            /* SDL_mixer can't resample 44100 to 32000 Hz :( */
            if (strncmp(mem, "OggS", 4) == 0 && *(unsigned short *)(mem + 0x28) == 44100)
                apkenv_mixer_sound_lame_resample_44100_32000(sound->chunk);
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
        apkenv_mixer_volume_sound(sound->chunk, volume);
        apkenv_mixer_play_sound(sound->chunk, loop);
    } else if (sound->music != NULL) {
        apkenv_mixer_stop_music(sound->music);
        active_music = sound->music;
        apkenv_mixer_volume_music(sound->music, volume);
        /* workaround, until we fix it */
#ifndef PLATFORM_SAILFISH
        apkenv_mixer_play_music(sound->music, loop);
#endif
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
        return (*env)->NewStringUTF(env, global->apk_filename);
    } else if (strcmp(p2->name, "getExternalStoragePath") == 0) {
        return (*env)->NewStringUTF(env, worldofgoo_priv.home_directory);
    } else if (strcmp(p2->name, "getInternalStoragePath") == 0) {
        return (*env)->NewStringUTF(env, worldofgoo_priv.home_directory);
    } else if (strcmp(p2->name, "getLanguage") == 0) {
        return (*env)->NewStringUTF(env, "");
    } else if (strcmp(p2->name, "playSound") == 0) {
        char *name_data = dup_jstring(global, va_arg(p3, jstring*));
        int loop = va_arg(p3, int);
        double volume = va_arg(p3, double);
        void *ret;
        MODULE_DEBUG_PRINTF("playSound '%s', %d, %f\n", name_data, loop, volume);
        ret = play_sound(name_data, loop, volume);
        MODULE_DEBUG_PRINTF(" = %p\n", ret);
        free(name_data);
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
        char *arg_data = dup_jstring(global, va_arg(p3, jstring*));
        int boo = va_arg(p3, int);
        MODULE_DEBUG_PRINTF("loadSound %s, %d\n", arg_data, boo);
        load_sound(arg_data);
        free(arg_data);
    } else if (strcmp(p2->name, "setSoundVolume") == 0) {
        struct player_sound *sound = va_arg(p3, struct player_sound *);
        double volume = va_arg(p3, double);
        MODULE_DEBUG_PRINTF("setSoundVolume %p %.3f\n", sound, volume);
        if (sound->chunk != NULL)
            apkenv_mixer_volume_sound(sound->chunk, volume);
        else if (sound->music != NULL && sound->music == active_music)
            apkenv_mixer_volume_music(sound->music, volume);
    } else if (strcmp(p2->name, "stopSound") == 0) {
        struct player_sound *sound = va_arg(p3, struct player_sound *);
        MODULE_DEBUG_PRINTF("stopSound %p\n", sound);
        if (sound->chunk != NULL) {
            if (apkenv_mixer_get_sound_channel(sound->chunk) >= 0) {
                apkenv_mixer_stop_sound(sound->chunk);
            }
        } else if (sound->music != NULL && sound->music == active_music) {
            apkenv_mixer_stop_music(sound->music);
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
    char *str_data = dup_jstring(global, va_arg(p3, jstring*));
    int i;
    if (strcmp(p2->name, "getAssetFileOffset") == 0) {
        for (i = 0; i < apk_index_pos; i++) {
            if (strstr(apk_index[i].filename, str_data) != NULL) {
                free(str_data);
                return apk_index[i].offset;
            }
        }
        fprintf(stderr, "not found: %s\n", str_data);
        free(str_data);
        return -1;
    } else if (strcmp(p2->name, "getAssetFileLength") == 0) {
        for (i = 0; i < apk_index_pos; i++) {
            if (strstr(apk_index[i].filename, str_data) != NULL) {
                free(str_data);
                return apk_index[i].length;
            }
        }
        fprintf(stderr, "not found: %s\n", str_data);
        free(str_data);
        return -1;
    } else {
        fprintf(stderr, "Do not know what to do: %s\n", p2->name);
        free(str_data);
    }
    free(str_data);
    return 0;
}

jboolean
worldofgoo_CallBooleanMethodV(JNIEnv* p0, jobject p1, jmethodID p2, va_list p3)
{
    MODULE_DEBUG_PRINTF("worldofgoo_CallBooleanMethodV %s\n", p2->name);
    if (strcmp(p2->name, "isGlThread") == 0) {
        return pthread_equal(pthread_self(), global->gl_thread_id);
    } else if (strcmp(p2->name, "isLargeScreen") == 0) {
        return 0;
    } else {
        fprintf(stderr, "Do not know what to do: %s\n", p2->name);
    }
    return 0;
}

jstring ref = NULL;
jstring str = NULL;

jstring
worldofgoo_NewStringUTF(JNIEnv* p0, const char* p1)
{
    MODULE_DEBUG_PRINTF("worldofgoo_NewStringUTF(%x)\n", p1);
    jstring ref_tmp;
    jstring str_tmp;

    str_tmp = global->dalvik_copy_env.NewStringUTF(p0, p1);
    ref_tmp = global->dalvik_copy_env.NewGlobalRef(p0, str_tmp);

    if(!ref_tmp)
    {
        return str_tmp;
    }

    jboolean iscopy;
    const char *thestring = global->dalvik_copy_env.GetStringUTFChars(p0, ref_tmp, &iscopy);

    if(thestring && strncmp(thestring, "res/", 4) == 0)
    {
        global->dalvik_copy_env.ReleaseStringUTFChars(p0, ref_tmp, thestring);
        if(str != NULL)
        {
            fprintf(stderr, "worldofgoo NewStringUTF/NewGlobalRef hack is bad\n");
        }

        str = str_tmp;
        ref = ref_tmp;
        return ref;
    }
    else
    {
        // global ref not needed
        global->dalvik_copy_env.ReleaseStringUTFChars(p0, ref_tmp, thestring);
        global->dalvik_copy_env.DeleteGlobalRef(p0, ref_tmp);
        return str_tmp;
    }

    return NULL;
}

void
worldofgoo_DeleteLocalRef(JNIEnv* p0, jobject p1)
{
    MODULE_DEBUG_PRINTF("worldofgoo_DeleteLocalRef(%x)\n", p1);
    if(p1 == ref)
    {
        global->dalvik_copy_env.DeleteGlobalRef(p0, ref);
        ref = NULL;
    }
    global->dalvik_copy_env.DeleteLocalRef(p0, str);
    str = NULL;
}

jobject
worldofgoo_NewGlobalRef(JNIEnv* p0, jobject p1)
{
    MODULE_DEBUG_PRINTF("worldofgoo_NewGlobalRef(%x)\n", p1);
    if(p1 == ref)
    {
        return global->dalvik_copy_env.NewGlobalRef(p0, str);
    }
    return p1;
}

void
worldofgoo_DeleteGlobalRef(JNIEnv* p0, jobject p1)
{
    MODULE_DEBUG_PRINTF("worldofgoo_DeleteGlobalRef()\n");
    global->dalvik_copy_env.DeleteGlobalRef(p0, p1);
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

    if(global->use_dvm)
    {
        self->override_env.NewGlobalRef = worldofgoo_NewGlobalRef;
        self->override_env.DeleteGlobalRef = worldofgoo_DeleteGlobalRef;
        self->override_env.NewStringUTF = worldofgoo_NewStringUTF;
        self->override_env.DeleteLocalRef = worldofgoo_DeleteLocalRef;
    }

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
    global = self->global;

    GLOBAL_M->module_hacks->current_orientation = ORIENTATION_LANDSCAPE;
    GLOBAL_M->module_hacks->glOrthof_rotation_hack = 1;
    GLOBAL_M->module_hacks->gles_viewport_hack = 1;

    self->priv->home_directory = home;
    self->priv->apk_in_mem = GLOBAL_M->apk_in_mem;
    build_apk_index(GLOBAL_M->apk_filename);

    /* init sound, must use 32000Hz because music is at that rate,
     * and SDL_mixer doesn't resample to/from 32000 */
    if (apkenv_mixer_open(32000, AudioFormat_S16SYS, 2, 1024) < 0)
    {
        fprintf(stderr, "Mix_OpenAudio failed.\n");
        exit(1);
    }

    self->priv->nativeOnCreate(ENV_M, GLOBAL_M, JNI_FALSE, JNI_TRUE, JNI_FALSE);
    self->priv->nativeOnSurfaceCreated(ENV_M, GLOBAL_M);
    if(GLOBAL_M->platform->get_orientation() == ORIENTATION_LANDSCAPE) {
        self->priv->nativeResize(ENV_M, GLOBAL_M, width, height);
    }
    else {
        self->priv->nativeResize(ENV_M, GLOBAL_M, height, width);
    }
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

