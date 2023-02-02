#pragma once

/**
 * apkenv
 * Copyright (c) 2023 Thomas Perl <m@thp.io>
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


#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include "../apkenv.h"
#include "../jni/jnienv.h"

/**
 * NDK Asset Manager API, see:
 * https://developer.android.com/ndk/reference/group/asset
 **/

struct AAsset;
struct AAssetManager;

struct AAssetManager *
AAssetManager_fromJava(JNIEnv *env, jobject assetManager) SOFTFP;

struct AAsset *
AAssetManager_open(struct AAssetManager *manager, const char *filename, uint32_t mode) SOFTFP;

uint64_t
AAsset_getLength64(struct AAsset *asset) SOFTFP;

int
AAsset_read(struct AAsset *asset, void *buf, size_t count) SOFTFP;

void
AAsset_close(struct AAsset *asset) SOFTFP;

struct AConfiguration;

struct AConfiguration *
AConfiguration_new(void) SOFTFP;

void
AConfiguration_fromAssetManager(struct AConfiguration *out, struct AAssetManager *am) SOFTFP;

void
AConfiguration_getLanguage(struct AConfiguration *config, char *outLanguage) SOFTFP;

void
AConfiguration_getCountry(struct AConfiguration *config, char *outCountry) SOFTFP;

void
AConfiguration_delete(struct AConfiguration *config) SOFTFP;


/* NDK Looper and Input Events (NativeActivity) */

struct ALooper;

typedef int (*ALooper_callbackFunc)(int fd, int events, void *data) SOFTFP;

struct android_poll_source {
    int32_t id;
    struct android_app* app;
    void (*process)(struct android_app *app, struct android_poll_source *source);
};

int
ALooper_pollAll(int timeoutMillis, int* outFd, int* outEvents, void** outData) SOFTFP;

struct ALooper *
ALooper_prepare(int opts) SOFTFP;

int
ALooper_addFd(struct ALooper *looper, int fd, int ident, int events,
        ALooper_callbackFunc callback, void *data) SOFTFP;

struct AInputEvent {
    int type;
    int action;
    float x;
    float y;
};

#define AINPUT_EVENT_TYPE_MOTION (2)

int32_t
AInputEvent_getType(const struct AInputEvent *event) SOFTFP;

int32_t
AMotionEvent_getAction(const struct AInputEvent *motion_event) SOFTFP;

float
AMotionEvent_getX(const struct AInputEvent *motion_event, size_t pointer_index) SOFTFP;

float
AMotionEvent_getY(const struct AInputEvent *motion_event, size_t pointer_index) SOFTFP;

struct AInputQueue;

void
AInputQueue_detachLooper(struct AInputQueue *queue) SOFTFP;

void
AInputQueue_attachLooper(struct AInputQueue *queue, struct ALooper *looper,
        int ident, ALooper_callbackFunc callback, void *data) SOFTFP;

int32_t
AInputQueue_getEvent(struct AInputQueue *queue, struct AInputEvent **outEvent) SOFTFP;

int32_t
AInputQueue_preDispatchEvent(struct AInputQueue *queue, struct AInputEvent *event) SOFTFP;

void
AInputQueue_finishEvent(struct AInputQueue *queue, struct AInputEvent *event, int handled) SOFTFP;

struct ARect {
    int32_t left;
    int32_t top;
    int32_t right;
    int32_t bottom;
};

/* native activity glue */

struct ANativeActivityCallbacks;

struct ANativeActivity {
    struct ANativeActivityCallbacks *callbacks;
    JavaVM* vm;
    JNIEnv* env;
    jobject clazz;

    const char *internalDataPath;
    const char *externalDataPath;
    int32_t sdkVersion;
    void *instance;
    struct AAssetManager *assetManager;
    const char *obbPath;
};

struct android_app {
    void *userData;
    void (*onAppCmd)(struct android_app *app, int32_t cmd);
    int32_t (*onInputEvent)(struct android_app *app, struct AInputEvent *event);

    struct ANativeActivity *activity;
    struct AConfiguration *config;

    void *savedState;
    size_t savedStateSize;

    struct ALooper *looper;
    struct AInputQueue *inputQueue;
    struct ANativeWindow *window;
    struct ARect contentRect;
    int activityState;
    int destroyRequested;

    // + private implementation details follow (different from stock android):

    bool window_was_inited;
    struct SupportModule *apkenv_module;
};

void
apkenv_android_wrappers_set_android_app(struct android_app *app);
