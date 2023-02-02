
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

/* from android/asset_manager.h */
#define AASSET_MODE_STREAMING (2)

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
