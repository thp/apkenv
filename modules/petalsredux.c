
/**
 * apkenv
 * Copyright (c) 2012, 2014 Thomas Perl <m@thp.io>
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


/**
 * Support module for Petals Redux
 * http://thp.io/2014/petals/
 **/

#include "common.h"

#include <SDL/SDL.h>

#include <stdio.h>
#include <stdarg.h>

#define ASSET_FILE_PETALS3D_TNX "assets/petals3d.tnx"

jint RegisterNatives(JNIEnv *env, jclass klass, const JNINativeMethod *method, jint count) SOFTFP;
jobject CallStaticObjectMethodV(JNIEnv *env, jclass klass, jmethodID method, va_list args) SOFTFP;

typedef void (*petalsredux_init_t)(JNIEnv *env, jobject obj) SOFTFP;
typedef void (*petalsredux_start_t)(JNIEnv *env, jobject obj, jstring home, jstring locale) SOFTFP;
typedef void (*petalsredux_resize_t)(JNIEnv *env, jobject obj, jint width, jint height) SOFTFP;
typedef void (*petalsredux_touch_t)(JNIEnv *env, jobject obj, jint action, jfloat x, jfloat y, jint finger) SOFTFP;
typedef void (*petalsredux_render_t)(JNIEnv *env, jobject obj) SOFTFP;
typedef void (*petalsredux_pause_t)(JNIEnv *env, jobject obj) SOFTFP;
typedef void (*petalsredux_resume_t)(JNIEnv *env, jobject obj) SOFTFP;
typedef jboolean (*petalsredux_back_t)(JNIEnv *env, jobject obj) SOFTFP;
typedef void (*petalsredux_mix_t)(JNIEnv *env, jobject obj, jbyteArray buffer) SOFTFP;

struct GlobalState *global;

struct SupportModulePriv {
    jni_onload_t JNI_OnLoad;

    petalsredux_start_t native_start;
    petalsredux_pause_t native_pause;
    petalsredux_resume_t native_resume;
    petalsredux_back_t native_back;

    petalsredux_init_t native_init;
    petalsredux_resize_t native_resize;
    petalsredux_render_t native_render;

    petalsredux_touch_t native_touch;

    petalsredux_mix_t native_mix;

    int is_petalsredux;
};
static struct SupportModulePriv petalsredux_priv;

#define REGISTER_METHOD(x, y) \
    if (strcmp(method[i].name, x) == 0) { \
        petalsredux_priv.y = method[i].fnPtr; \
    }

jint
RegisterNatives(JNIEnv *env, jclass klass, const JNINativeMethod *method, jint count)
{
    int i;

    for (i = 0; i < count; i++) {
        REGISTER_METHOD("nativeStart", native_start);
        REGISTER_METHOD("nativePause", native_pause);
        REGISTER_METHOD("nativeResume", native_resume);
        REGISTER_METHOD("nativeBack", native_back);
        REGISTER_METHOD("nativeInit", native_init);
        REGISTER_METHOD("nativeResize", native_resize);
        REGISTER_METHOD("nativeRender", native_render);
        REGISTER_METHOD("nativeTouch", native_touch);
        REGISTER_METHOD("nativeMix", native_mix);
    }

    return 0;
}

jobject
CallStaticObjectMethodV(JNIEnv *env, jclass klass, jmethodID method, va_list args)
{
    jstring path = va_arg(args, jstring);
    const char *filename = (*env)->GetStringUTFChars(env, path, 0);
    return global->read_file_to_jni_array(ASSET_FILE_PETALS3D_TNX);
}

void
check_petalsredux(const char *filename, char *buffer, size_t size)
{
    petalsredux_priv.is_petalsredux = 1;
}

static int
petalsredux_try_init(struct SupportModule *self)
{
    global = GLOBAL_M;

    self->priv->JNI_OnLoad = (jni_onload_t)LOOKUP_M("JNI_OnLoad");

    GLOBAL_M->foreach_file(ASSET_FILE_PETALS3D_TNX, check_petalsredux);
    self->override_env.RegisterNatives = RegisterNatives;
    self->override_env.CallStaticObjectMethodV = CallStaticObjectMethodV;

    return (self->priv->JNI_OnLoad != NULL && self->priv->is_petalsredux);
}

static void mix_audio(void *user_data, Uint8 *stream, int len)
{
    struct SupportModule *self = (struct SupportModule *)user_data;

    struct dummy_array array;
    array.data = stream;
    array.length = len / 2;
    array.element_size = 1;

    petalsredux_priv.native_mix(ENV_M, GLOBAL_M, &array);
}

static void
petalsredux_init(struct SupportModule *self, int width, int height, const char *home)
{
    self->priv->JNI_OnLoad(VM_M, NULL);

    const char *locale = getenv("LANG");
    if (!locale) {
        locale = getenv("LC_MESSAGES");
    }
    if (!locale) {
        locale = getenv("LC_ALL");
    }

    jstring jhome = (*ENV_M)->NewStringUTF(ENV_M, home);
    jstring jlocale = (*ENV_M)->NewStringUTF(ENV_M, locale);

    self->priv->native_start(ENV_M, GLOBAL_M, jhome, jlocale);
    self->priv->native_init(ENV_M, GLOBAL_M);
    self->priv->native_resize(ENV_M, GLOBAL_M, width, height);

    SDL_AudioSpec desired, obtained;

    desired.freq = 44100;
    desired.channels = 1;
    desired.format = AUDIO_S16SYS;
    desired.samples = desired.freq / 8;
    desired.callback = mix_audio;
    desired.userdata = self;

    SDL_OpenAudio(&desired, &obtained);
    SDL_PauseAudio(0);
}

static void
petalsredux_input(struct SupportModule *self, int event, int x, int y, int finger)
{
    self->priv->native_touch(ENV_M, GLOBAL_M, event, x, y, finger);
}

static void
petalsredux_key_input(struct SupportModule *self, int event, int keycode, int unicode)
{
}

static void
petalsredux_update(struct SupportModule *self)
{
    self->priv->native_render(ENV_M, GLOBAL_M);
}

static void
petalsredux_deinit(struct SupportModule *self)
{
}

static void
petalsredux_pause(struct SupportModule *self)
{
    self->priv->native_pause(ENV_M, GLOBAL_M);
}

static void
petalsredux_resume(struct SupportModule *self)
{
    self->priv->native_resume(ENV_M, GLOBAL_M);
}

static int
petalsredux_requests_exit()
{
    return 0;
}

APKENV_MODULE(petalsredux, MODULE_PRIORITY_GAME)
