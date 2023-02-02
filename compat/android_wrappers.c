
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


#include "android_wrappers.h"

#include <stdio.h>
#include <string.h>

#include "../apklib/apklib.h"

extern struct GlobalState global;

static struct android_app *
g_android_app = NULL;

/* from android/asset_manager.h */
#define AASSET_MODE_STREAMING (2)

/* from android_native_app_glue.h */
#define APP_CMD_INIT_WINDOW (1)

struct AAssetManager {
    AndroidApk *apk;
};

struct AAsset {
    struct AAssetManager *manager;
    char *filename;

    char *buf;
    size_t len;
    size_t pos;
};

struct AAssetManager *
AAssetManager_fromJava(JNIEnv *env, jobject assetManager)
{
    fprintf(stderr, "AAssetManager_fromJava(env=%p, assetManager=%p)\n", env, assetManager);

    struct AAssetManager *manager = calloc(sizeof(struct AAssetManager), 1);
    manager->apk = apk_open(global.apk_filename);
    return manager;
}

struct AAsset *
AAssetManager_open(struct AAssetManager *manager, const char *filename, uint32_t mode)
{
    fprintf(stderr, "AAssetManager_open(manager=%p, filename=%s, mode=%x)\n", manager, filename, mode);

    if (mode != AASSET_MODE_STREAMING) {
        fprintf(stderr, "Unsupported mode: %x\n", mode);
        exit(1);
    }

    char *buf;
    size_t len;

    char *tmp;
    if (asprintf(&tmp, "assets/%s", filename) == -1) {
        fprintf(stderr, "Could not format asset path\n");
        exit(1);
    }
    enum ApkResult res = apk_read_file(manager->apk, tmp, &buf, &len);
    free(tmp);

    if (res != APK_OK) {
        fprintf(stderr, "File not found: '%s'\n", filename);
        return NULL;
    }

    struct AAsset *asset = calloc(sizeof(struct AAsset), 1);
    asset->manager = manager;
    asset->filename = strdup(filename);
    asset->buf = buf;
    asset->len = len;
    asset->pos = 0;

    return asset;
}

uint64_t
AAsset_getLength64(struct AAsset *asset)
{
    fprintf(stderr, "AAsset_getLength64(asset=%p [%s])\n", asset, asset->filename);

    return asset->len;
}

int
AAsset_read(struct AAsset *asset, void *buf, size_t count)
{
    fprintf(stderr, "AAsset_read(asset=%p [%s], buf=%p, count=%zu)\n", asset, asset->filename, buf, count);

    int remaining = asset->len - asset->pos;
    if (count < remaining) {
        remaining = count;
    }

    memcpy(buf, asset->buf + asset->pos, remaining);
    asset->pos += remaining;

    return remaining;
}

void
AAsset_close(struct AAsset *asset)
{
    fprintf(stderr, "AAsset_close(asset=%p [%s])\n", asset, asset->filename);

    free(asset->buf);
    free(asset->filename);
    free(asset);
}

struct AConfiguration *
AConfiguration_new(void)
{
    fprintf(stderr, "AConfiguration_new()\n");
    exit(1);
}

void
AConfiguration_fromAssetManager(struct AConfiguration *out, struct AAssetManager *am)
{
    fprintf(stderr, "AConfiguration_fromAssetManager(out=%p, am=%p)\n", out, am);
    exit(1);
}

void
AConfiguration_getLanguage(struct AConfiguration *config, char *outLanguage)
{
    fprintf(stderr, "AConfiguration_getLanguage(config=%p, outLanguage=%p)\n", config, outLanguage);
    exit(1);
}

void
AConfiguration_getCountry(struct AConfiguration *config, char *outCountry)
{
    fprintf(stderr, "AConfiguration_getCountry(config=%p, outCountry=%p)\n", config, outCountry);
    exit(1);
}

void
AConfiguration_delete(struct AConfiguration *config)
{
    fprintf(stderr, "AConfiguration_delete(config=%p)\n", config);
    exit(1);
}

static void
do_poll_input_events(struct android_app *app, struct android_poll_source *source)
{
    if (!app->window_was_inited) {
        fprintf(stderr, "sending init window...\n");
        app->onAppCmd(app, APP_CMD_INIT_WINDOW);
        fprintf(stderr, "init window finished...\n");
        app->window_was_inited = true;
    }

    // Process input events
    if (app->apkenv_module->global->platform->input_update(app->apkenv_module->global->active_module)) {
        app->destroyRequested = 1;
    }
}

static struct android_poll_source
input_event_poll_source = {
    .id = 1,
    .app = NULL,
    .process = do_poll_input_events,
};

int
ALooper_pollAll(int timeoutMillis, int* outFd, int* outEvents, void** outData)
{
    fprintf(stderr, "ALooper_pollAll(timeoutMillis=%d, outFd=%p, outEvents=%p, outData=%p)\n",
            timeoutMillis, outFd, outEvents, outData);

    if (g_android_app) {
        static int i = 0;
        if (i++ % 2 == 0) {
            input_event_poll_source.app = g_android_app;
            *outEvents = 1;
            outData[0] = (void *)&input_event_poll_source;
            return 1;
        }
    }

    return -1;
}

struct ALooper *
ALooper_prepare(int opts)
{
    fprintf(stderr, "ALooper_prepare(opts=%d)\n", opts);
    exit(1);

    return NULL;
}

int
ALooper_addFd(struct ALooper *looper, int fd, int ident, int events,
        ALooper_callbackFunc callback, void *data)
{
    fprintf(stderr, "ALooper_addFd(looper=%p, fd=%d, ident=%d, events=%d, callback=%p, data=%p)\n",
            looper, fd, ident, events, callback, data);
    exit(1);

    return -1;
}


int32_t
AInputEvent_getType(const struct AInputEvent *event)
{
    fprintf(stderr, "AInputEvent_getType(event=%p)\n", event);

    return event->type;
}

int32_t
AMotionEvent_getAction(const struct AInputEvent *motion_event)
{
    fprintf(stderr, "AMotionEvent_getAction(motion_event=%p)\n", motion_event);

    return motion_event->action;
}

float
AMotionEvent_getX(const struct AInputEvent *motion_event, size_t pointer_index)
{
    fprintf(stderr, "AMotionEvent_getX(motion_event=%p, pointer_index=%zu)\n", motion_event, pointer_index);

    return motion_event->x;
}

float
AMotionEvent_getY(const struct AInputEvent *motion_event, size_t pointer_index)
{
    fprintf(stderr, "AMotionEvent_getY(motion_event=%p, pointer_index=%zu)\n", motion_event, pointer_index);

    return motion_event->y;
}

void
AInputQueue_detachLooper(struct AInputQueue *queue)
{
    fprintf(stderr, "AInputQueue_detachLooper(queue=%p)\n", queue);
    exit(1);
}

void
AInputQueue_attachLooper(struct AInputQueue *queue, struct ALooper *looper,
        int ident, ALooper_callbackFunc callback, void *data)
{
    fprintf(stderr, "AInputQueue_attachLooper(queue=%p, looper=%p, ident=%d, callback=%p, data=%p)\n",
            queue, looper, ident, callback, data);
    exit(1);
}

int32_t
AInputQueue_getEvent(struct AInputQueue *queue, struct AInputEvent **outEvent)
{
    fprintf(stderr, "AInputQueue_getEvent(queue=%p, outEvent=%p)\n", queue, outEvent);
    exit(1);

    return 0;
}

int32_t
AInputQueue_preDispatchEvent(struct AInputQueue *queue, struct AInputEvent *event)
{
    fprintf(stderr, "AInputQueue_preDispatchEvent(queue=%p, event=%p)\n", queue, event);
    exit(1);

    return 0;
}

void
AInputQueue_finishEvent(struct AInputQueue *queue, struct AInputEvent *event, int handled)
{
    fprintf(stderr, "AInputQueue_finishEvent(queue=%p, event=%p, handled=%d)\n", queue, event, handled);
    exit(1);
}

void
apkenv_android_wrappers_set_android_app(struct android_app *app)
{
    g_android_app = app;
}
