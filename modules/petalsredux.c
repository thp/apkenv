
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
#include "../audio/audio.h"

#include <stdio.h>
#include <stdarg.h>

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

static struct GlobalState *global;

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

    char *filename;
};
static struct SupportModulePriv petalsredux_priv;

jobject
CallStaticObjectMethodV(JNIEnv *env, jclass klass, jmethodID method, va_list args)
{
    jstring path = va_arg(args, jstring);
    const char *filename = (*env)->GetStringUTFChars(env, path, 0);
    return global->read_file_to_jni_array(petalsredux_priv.filename);
}

void
check_petalsredux(const char *filename, char *buffer, size_t size)
{
    free(petalsredux_priv.filename);
    petalsredux_priv.filename = strdup(filename);
}

static int
petalsredux_try_init(struct SupportModule *self)
{
    global = GLOBAL_M;

    self->priv->JNI_OnLoad = (jni_onload_t)LOOKUP_M("JNI_OnLoad");

    GLOBAL_M->foreach_file("assets/petals3d.tnx", check_petalsredux);
    GLOBAL_M->foreach_file("assets/petals3d.wad", check_petalsredux);
    self->override_env.CallStaticObjectMethodV = CallStaticObjectMethodV;

    return (self->priv->JNI_OnLoad != NULL && self->priv->filename != NULL);
}

static void mix_audio(void *user_data, void *stream, int len)
{
    struct SupportModule *self = (struct SupportModule *)user_data;

    JNIEnv *thread_env;
    JNIEnv ref_env;

    jarray *array;
    (*VM(global))->AttachCurrentThread(VM(global), &thread_env, NULL);

    /* here we need the original NewGlobalRef */
    if(global->use_dvm)
    {
        ref_env = &(global->dalvik_copy_env);
    }
    else ref_env = *thread_env;

    array = (*thread_env)->NewShortArray(thread_env, len / 2);

    jobject *ref = ref_env->NewGlobalRef(thread_env, array);
    petalsredux_priv.native_mix(ENV_M, GLOBAL_M, ref);
    ref_env->DeleteGlobalRef(thread_env, ref);

    jshort *elements = (*thread_env)->GetShortArrayElements(thread_env, array, 0);
    memcpy(stream, elements, len);
    (*thread_env)->ReleaseShortArrayElements(thread_env, array, elements, JNI_ABORT);

    (*thread_env)->DeleteLocalRef(thread_env, array);

    (*VM(global))->DetachCurrentThread(VM(global));
}

static void
petalsredux_init(struct SupportModule *self, int width, int height, const char *home)
{
    self->priv->JNI_OnLoad(VM_M, NULL);

    GLOBAL_M->module_hacks->current_orientation = ORIENTATION_PORTRAIT;

    self->priv->native_start = jnienv_find_native_method(NULL, "nativeStart");
    self->priv->native_pause = jnienv_find_native_method(NULL, "nativePause");
    self->priv->native_resume = jnienv_find_native_method(NULL, "nativeResume");
    self->priv->native_back = jnienv_find_native_method(NULL, "nativeBack");
    self->priv->native_init = jnienv_find_native_method(NULL, "nativeInit");
    self->priv->native_resize = jnienv_find_native_method(NULL, "nativeResize");
    self->priv->native_render = jnienv_find_native_method(NULL, "nativeRender");
    self->priv->native_touch = jnienv_find_native_method(NULL, "nativeTouch");
    self->priv->native_mix = jnienv_find_native_method(NULL, "nativeMix");

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

    int freq = 44100;
    enum AudioFormat format = AudioFormat_S16SYS;
    apkenv_audio_open(freq, 1, format, freq / 8, mix_audio, self);
    apkenv_audio_play();
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
    apkenv_audio_close();
}

static void
petalsredux_pause(struct SupportModule *self)
{
    self->priv->native_pause(ENV_M, GLOBAL_M);
    apkenv_audio_pause();
}

static void
petalsredux_resume(struct SupportModule *self)
{
    self->priv->native_resume(ENV_M, GLOBAL_M);
    apkenv_audio_play();
}

static int
petalsredux_requests_exit(struct SupportModule *self)
{
    return 0;
}

APKENV_MODULE(petalsredux, MODULE_PRIORITY_GAME)
