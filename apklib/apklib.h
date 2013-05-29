#ifndef APKLIB_H
#define APKLIB_H

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

#include "unzip.h"

struct AndroidApk {
    unzFile *zip;
};
typedef struct AndroidApk AndroidApk;

enum ApkResult {
    APK_OK,
    APK_ERROR,
};

struct SharedLibrary {
    struct SharedLibrary *next;
    const char *filename;
    const char *dirname;
};

struct ResourceStrings {
    struct {
        char *key;
        char *value;
    } *entries;
    size_t count;
    /* convenience pointer to appropriate entries[x].value */
    const char *app_name;
};

typedef void (*apk_for_each_file_callback)(const char *filename, char *buffer, size_t size);

/* apklib.c */
AndroidApk *apk_open(const char *filename);
struct SharedLibrary *apk_get_shared_libraries(AndroidApk *apk, const char *libdir, const char *datadir);
enum ApkResult apk_read_file(AndroidApk *apk, const char *filename, char **buffer, size_t *size);
void apk_for_each_file(AndroidApk *apk, const char *prefix, apk_for_each_file_callback callback);
enum ApkResult apk_read_resources(AndroidApk *apk, struct ResourceStrings *rstrings);
void apk_close(AndroidApk *apk);

#endif /* APKLIB_H */
