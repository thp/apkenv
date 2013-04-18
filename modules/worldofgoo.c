
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

#include <SDL/SDL.h>
#include <EGL/egl.h>

#include "common.h"

typedef void (*worldofgoo_init_t)(JNIEnv *env, jobject obj) SOFTFP;
typedef void (*worldofgoo_oncreate_t)(JNIEnv *env, jobject obj, jboolean a, jboolean is_demo, jboolean c) SOFTFP;
typedef void (*worldofgoo_resize_t)(void *env, jobject obj, jint width, jint height) SOFTFP;
typedef void (*worldofgoo_render_t)(void *env, jobject obj, jboolean menu_pressed, jboolean back_pressed) SOFTFP;
typedef void (*worldofgoo_input_t)(void *env, jobject obj, jint event, jfloat x, jfloat y, jint index) SOFTFP;

struct SupportModulePriv {
    worldofgoo_oncreate_t nativeOnCreate;
    worldofgoo_init_t nativeOnSurfaceCreated;
    worldofgoo_resize_t nativeResize;
    worldofgoo_render_t nativeRender;
    worldofgoo_input_t nativeTouchEvent;

    const char *home_directory;
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

jobject
worldofgoo_CallObjectMethodV(JNIEnv *env, jobject p1, jmethodID p2, va_list p3) SOFTFP;

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
    if (strcmp(p2->name, "getApkPath") == 0) {
        return (*env)->NewStringUTF(env, GLOBAL_J(env)->apk_filename);
    } else if (strcmp(p2->name, "getExternalStoragePath") == 0) {
        return (*env)->NewStringUTF(env, worldofgoo_priv.home_directory);
    } else if (strcmp(p2->name, "getInternalStoragePath") == 0) {
        return (*env)->NewStringUTF(env, worldofgoo_priv.home_directory);
    } else if (strcmp(p2->name, "getLanguage") == 0) {
        return (*env)->NewStringUTF(env, "");
    } else if (strcmp(p2->name, "playSound") == 0) {
        // TODO: Play sound (doh!)
    } else {
        printf("Do not know what to do: %s\n", p2->name);
        exit(1);
    }
    return GLOBAL_J(env);
}

jlong
worldofgoo_CallLongMethodV(JNIEnv *p0, jobject p1, jmethodID p2, va_list p3)
{
    //printf("call long methodV: %s %s\n", p2->name, p2->sig);
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
    }
    return 0;
}

jboolean
worldofgoo_CallBooleanMethodV(JNIEnv* p0, jobject p1, jmethodID p2, va_list p3)
{
    printf("worldofgoo_CallBooleanMethodV(%s)\n", p2->name);
    if (strcmp(p2->name, "isGlThread") == 0) {
        printf("isGlThread: %x\n", eglGetCurrentContext());
        return eglGetCurrentContext() != 0;
    }
    return 0;
}

jint
worldofgoo_AttachCurrentThread(JavaVM *vm, JNIEnv **env, void *args)
{
    printf("worldofgoo_AttachCurrentThread()\n");
    struct GlobalState *global = (*vm)->reserved0;
    *env = ENV(global);
    return 0;
}

jint
worldofgoo_DetachCurrentThread(JavaVM *vm)
{
    printf("worldofgoo_DetachCurrentThread()\n");
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

    self->override_env.CallObjectMethodV = worldofgoo_CallObjectMethodV;
    self->override_env.CallLongMethodV = worldofgoo_CallLongMethodV;
    self->override_env.CallBooleanMethodV = worldofgoo_CallBooleanMethodV;
    self->override_vm.AttachCurrentThread = worldofgoo_AttachCurrentThread;
    self->override_vm.DetachCurrentThread = worldofgoo_DetachCurrentThread;

    return (self->priv->nativeOnCreate != NULL &&
            self->priv->nativeOnSurfaceCreated != NULL &&
            self->priv->nativeResize != NULL &&
            self->priv->nativeRender != NULL &&
            self->priv->nativeTouchEvent != NULL);
}

static void
worldofgoo_init(struct SupportModule *self, int width, int height, const char *home)
{
    self->priv->home_directory = home;
    build_apk_index(GLOBAL_M->apk_filename);
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

