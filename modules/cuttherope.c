
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

/**
 * Cut the Rope Lite - incomplete (but loading already works)
 *
 * Needs more love. Looks like they are rendering some things in Java,
 * so in that case, we can't really do much apart from re-implementing
 * everything from scratch - not something I'd do.
 **/

#include "common.h"

#include <SDL/SDL.h>

typedef void (*cuttherope_init_t)(JNIEnv *env, jobject obj, jobject resourceLoader, jobject soundManager, jobject preferences,
            jobject saveManager, jobject flurry, jobject videoPlayer, jobject scorer,
            jobject billingInterface, jobject remoteDataManager, jint x) SOFTFP;
typedef void (*cuttherope_resize_t)(JNIEnv *env, jobject object, jint width, jint height, jboolean flag) SOFTFP;
typedef void (*cuttherope_tick_t)(JNIEnv *env, jobject object, jlong tick) SOFTFP;
typedef void (*cuttherope_render_t)(JNIEnv *env, jobject object) SOFTFP;
typedef void (*cuttherope_videobannerfinished_t)(JNIEnv *env, jobject object) SOFTFP;

struct SupportModulePriv {
    jni_onload_t JNI_OnLoad;
    cuttherope_init_t nativeInit;
    cuttherope_resize_t nativeResize;
    cuttherope_tick_t nativeTick;
    cuttherope_render_t nativeRender;
    cuttherope_videobannerfinished_t videoBannerFinished;

    struct GlobalState *global;
};
static struct SupportModulePriv cuttherope_priv;

static void
cuttherope_video_banner_finished()
{
    cuttherope_priv.videoBannerFinished(ENV(cuttherope_priv.global),
            cuttherope_priv.global);
}


static jobject
cuttherope_CallObjectMethodV(JNIEnv *, jobject, jmethodID, va_list) SOFTFP;

static void
cuttherope_CallVoidMethodV(JNIEnv *, jobject, jmethodID, va_list) SOFTFP;

static jobject
cuttherope_CallStaticObjectMethodV(JNIEnv *, jclass, jmethodID, va_list) SOFTFP;


static jobject
cuttherope_CallObjectMethodV(JNIEnv *env, jobject p1, jmethodID p2, va_list p3)
{
    if (strcmp(p2->name, "loadData") == 0) {
        struct dummy_jstring *arg;
        arg = va_arg(p3, struct dummy_jstring*);
        jboolean flag = va_arg(p3, jint);
        (void)flag;

        char *filename = malloc(strlen(arg->data) + strlen("assets/") + 1);
        sprintf(filename, "assets/%s", arg->data);

        char *buf;
        size_t buf_size;
        if (GLOBAL_J(env)->read_file(filename, &buf, &buf_size)) {
            struct dummy_byte_array *array;
            array = malloc(sizeof(struct dummy_byte_array));
            array->data = buf;
            array->size = buf_size;

	    // Poll for events here to avoid "not responding" messages
	    SDL_Event e;
	    while (SDL_PollEvent(&e));

            return array;
        } else {
            return GLOBAL_J(env);
        }
    } else if (strcmp(p2->name, "getBytesOfBitmap") == 0) {
        char *buf;
        size_t buf_size;
        if (GLOBAL_J(env)->read_file("assets/zeptolab.png", &buf, &buf_size)) {
            struct dummy_byte_array *array;
            array = malloc(sizeof(struct dummy_byte_array));
            array->data = buf;
            array->size = buf_size;
            return array;
        }
    }
    return GLOBAL_J(env);
}

static void
cuttherope_CallVoidMethodV(JNIEnv* p0, jobject p1, jmethodID p2, va_list p3)
{
    if (p2 && strcmp(p2->name, "showVideoBanner") == 0) {
        cuttherope_video_banner_finished();
    }
}

static jobject
cuttherope_CallStaticObjectMethodV(JNIEnv *env, jclass p1, jmethodID p2, va_list p3)
{
    if (strcmp(p2->name, "getAppVersion") == 0) {
        return (*env)->NewStringUTF(env, "1.0.0");
    }
    return NULL;
}



static int
cuttherope_try_init(struct SupportModule *self)
{
    self->priv->JNI_OnLoad = (jni_onload_t)LOOKUP_M("JNI_OnLoad");
    self->priv->nativeInit = (cuttherope_init_t)LOOKUP_M("nativeInit");
    self->priv->nativeResize = (cuttherope_resize_t)LOOKUP_M("nativeResize");
    self->priv->nativeTick = (cuttherope_tick_t)LOOKUP_M("nativeTick");
    self->priv->nativeRender = (cuttherope_render_t)LOOKUP_M("nativeRender");
    self->priv->videoBannerFinished = (cuttherope_videobannerfinished_t)LOOKUP_M("videoBannerFinished");

    self->priv->global = GLOBAL_M;

    self->override_env.CallObjectMethodV = cuttherope_CallObjectMethodV;
    self->override_env.CallVoidMethodV = cuttherope_CallVoidMethodV;
    self->override_env.CallStaticObjectMethodV = cuttherope_CallStaticObjectMethodV;

    return (self->priv->JNI_OnLoad != NULL &&
            self->priv->nativeInit != NULL &&
            self->priv->nativeResize != NULL &&
            self->priv->nativeTick != NULL &&
            self->priv->videoBannerFinished != NULL);
}

static void
cuttherope_init(struct SupportModule *self, int width, int height, const char *home)
{
    void *resourceLoader = (void*)0xF00;
    void *soundManager = (void*)0xF01;
    void *preferences = (void*)0xF02;
    void *saveManager = (void*)0xF03;
    void *flurry = (void*)0xF04;
    void *videoPlayer = (void*)0xF05;
    void *scorer = (void*)0xF06;
    void *billingInterface = (void*)0xF07;
    void *remoteDataManager = (void*)0xF08;

    self->priv->JNI_OnLoad(VM_M, NULL);

    self->priv->nativeInit(ENV_M, GLOBAL_M, resourceLoader, soundManager, preferences,
            saveManager, flurry, videoPlayer, scorer,
            billingInterface, remoteDataManager, 0);
    self->priv->nativeResize(ENV_M, GLOBAL_M, width, height, 1);
}

static void
cuttherope_input(struct SupportModule *self, int event, int x, int y, int finger)
{
}

static void
cuttherope_update(struct SupportModule *self)
{
    self->priv->nativeTick(ENV_M, GLOBAL_M, SDL_GetTicks());
    self->priv->nativeRender(ENV_M, GLOBAL_M);
}

static void
cuttherope_deinit(struct SupportModule *self)
{
}

static void
cuttherope_pause(struct SupportModule *self)
{
}

static void
cuttherope_resume(struct SupportModule *self)
{
}

APKENV_MODULE(cuttherope, MODULE_PRIORITY_GAME)

