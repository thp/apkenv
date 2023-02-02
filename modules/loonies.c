
/**
 * apkenv
 * Copyright (c) 2012, 2023 Thomas Perl <m@thp.io>
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
 * Support module for Loonies
 * https://thp.io/2021/loonies/
 **/

#include "common.h"

#include <stdbool.h>

#include "../audio/audio.h"


struct SupportModulePriv {
    void (*native_init)(JNIEnv *env, jobject obj, jboolean has_touch, jstring save_dir, jobject assets) SOFTFP;
    void (*native_resize)(JNIEnv *env, jobject obj, jint width, jint height) SOFTFP;
    void (*native_touch)(JNIEnv *env, jobject obj, jint action, jfloat x, jfloat y) SOFTFP;
    void (*native_key)(JNIEnv *env, jobject obj, jboolean down, jint key) SOFTFP;
    void (*native_pause)(JNIEnv *env, jobject obj) SOFTFP;
    void (*native_resume)(JNIEnv *env, jobject obj) SOFTFP;
    jboolean (*native_render)(JNIEnv *env, jobject obj) SOFTFP;
    void (*native_mix)(JNIEnv *env, jobject obj, jshortArray samples) SOFTFP;
    void (*native_quit)(JNIEnv *env, jobject obj) SOFTFP;

    bool requests_exit;
};

static struct SupportModulePriv
loonies_priv;

static int
loonies_try_init(struct SupportModule *self)
{
    self->priv->native_init = LOOKUP_M("Java_io_thp_loonies_android_MainActivity_begin");
    self->priv->native_resize = LOOKUP_M("Java_io_thp_loonies_android_MainActivity_setSize");
    self->priv->native_touch = LOOKUP_M("Java_io_thp_loonies_android_MainActivity_addTouchEvent");
    self->priv->native_key = LOOKUP_M("Java_io_thp_loonies_android_MainActivity_addKeyEvent");
    self->priv->native_pause = LOOKUP_M("Java_io_thp_loonies_android_MainActivity_paused");
    self->priv->native_resume = LOOKUP_M("Java_io_thp_loonies_android_MainActivity_resumed");
    self->priv->native_render = LOOKUP_M("Java_io_thp_loonies_android_MainActivity_render");
    self->priv->native_mix = LOOKUP_M("Java_io_thp_loonies_android_AudioOutput_nativeMix");
    self->priv->native_quit = LOOKUP_M("Java_io_thp_loonies_android_MainActivity_end");

    return (self->priv->native_init != NULL &&
            self->priv->native_resize != NULL &&
            self->priv->native_pause != NULL &&
            self->priv->native_resume != NULL &&
            self->priv->native_touch != NULL &&
            self->priv->native_key != NULL &&
            self->priv->native_render != NULL &&
            self->priv->native_mix != NULL &&
            self->priv->native_quit != NULL);
}

static void
loonies_audio_callback(void *user_data, void *stream, int len)
{
    struct SupportModule *self = user_data;

    JNIEnv *thread_env;
    (*VM_M)->AttachCurrentThread(VM_M, &thread_env, NULL);

    /* here we need the original NewGlobalRef */
    JNIEnv ref_env = GLOBAL_M->use_dvm ? &(GLOBAL_M->dalvik_copy_env) : *thread_env;

    jshortArray *array = (*thread_env)->NewShortArray(thread_env, len / sizeof(jshort));

    jobject *ref = ref_env->NewGlobalRef(thread_env, array);
    loonies_priv.native_mix(ENV_M, GLOBAL_M, ref);
    ref_env->DeleteGlobalRef(thread_env, ref);

    jshort *elements = (*thread_env)->GetShortArrayElements(thread_env, array, 0);
    memcpy(stream, elements, len);
    (*thread_env)->ReleaseShortArrayElements(thread_env, array, elements, JNI_ABORT);

    (*thread_env)->DeleteLocalRef(thread_env, array);

    (*VM_M)->DetachCurrentThread(VM_M);
}

static void
loonies_init(struct SupportModule *self, int width, int height, const char *home)
{
    GLOBAL_M->module_hacks->current_orientation = ORIENTATION_LANDSCAPE;

    self->priv->native_init(ENV_M, GLOBAL_M, /*has_touch=*/1, JNIEnv_NewString(ENV_M, (const jchar *)home, strlen(home)), (void *)0xDEADBEEF/* assets */);
    self->priv->native_resize(ENV_M, GLOBAL_M, width, height);

    apkenv_audio_open(44100, AudioFormat_S16SYS, 2, 512, loonies_audio_callback, self);
    apkenv_audio_play();
}

static void
loonies_input(struct SupportModule *self, int event, int x, int y, int finger)
{
    self->priv->native_touch(ENV_M, GLOBAL_M, event, x, y);
}

static void
loonies_key_input(struct SupportModule *self, int event, int keycode, int unicode)
{
    if (event == ACTION_DOWN || event == ACTION_UP) {
        self->priv->native_key(ENV_M, GLOBAL_M, (event == ACTION_DOWN), keycode);
    }
}

static void
loonies_update(struct SupportModule *self)
{
    if (!self->priv->native_render(ENV_M, GLOBAL_M)) {
        self->priv->requests_exit = true;
    }
}

static void
loonies_deinit(struct SupportModule *self)
{
    apkenv_audio_close();
    self->priv->native_quit(ENV_M, GLOBAL_M);
}

static void
loonies_pause(struct SupportModule *self)
{
    apkenv_audio_pause();
    self->priv->native_resume(ENV_M, GLOBAL_M);
}

static void
loonies_resume(struct SupportModule *self)
{
    self->priv->native_resume(ENV_M, GLOBAL_M);
    apkenv_audio_play();
}

static int
loonies_requests_exit()
{
    return loonies_priv.requests_exit;
}

APKENV_MODULE(loonies, MODULE_PRIORITY_GAME)
