
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

#include <zlib.h>
#include <limits.h>
#include <assert.h>
#include <string.h>

#include "unzip.h"
#include "ioapi_mem.h"

#include "apklib.h"


#define DBG(msg) fprintf(stderr,"%s(%d): %s\n",__FILE__,__LINE__,#msg)

AndroidApk *
apk_open(const char *filename)
{
    AndroidApk *apk = calloc(1, sizeof(AndroidApk));
    apk->zip = unzOpen(filename);
    return apk;
}

char *
apk_get_shared_library(AndroidApk *apk)
{
    assert(apk != NULL);

    char filename[PATH_MAX], tmpname[PATH_MAX];
    FILE *result = NULL;
    char buf[64*1024];
    int read;

    if (unzGoToFirstFile(apk->zip) != UNZ_OK) {
        return NULL;
    }

    do {
        if (unzGetCurrentFileInfo(apk->zip, NULL, filename, sizeof(filename), NULL, 0, NULL, 0) == UNZ_OK) {
            if (memcmp(filename, "lib/armeabi", 11) == 0) {
                if (unzOpenCurrentFile(apk->zip) == UNZ_OK) {



#if !defined(PANDORA)
                    strcpy(filename, "/home/user/.apkenv-XXXXXX");
                    int fd = mkstemp(filename);
                    result = fdopen(fd, "w+b");
#else
                    sprintf(filename,"./%s",strdup(filename));
                    recursive_mkdir(filename);
                    result = fopen(filename, "w+b");
#endif
                    while (!unzeof(apk->zip)) {
                        read = unzReadCurrentFile(apk->zip, buf, sizeof(buf));
                        if (read) {
                            assert(fwrite(buf, read, 1, result) == 1);
                        }
                    }
                    fclose(result);
                    unzCloseCurrentFile(apk->zip);
#if defined(PANDORA)
                    sync();
#endif
                    return strdup(filename);
                }
                break;
            }
        }
    } while (unzGoToNextFile(apk->zip) == UNZ_OK);

    return NULL;
}

enum ApkResult
apk_uncompress_internal(AndroidApk *apk, char **buffer, size_t *size)
{
    zlib_filefunc_def filefunc;
    fill_memory_filefunc(&filefunc);
    char *old_buffer = *buffer;
    char path[1024];
    sprintf(path, "%x+%x", *buffer, *size);

    /* Decompress a single file in a .zip from memory */
    unz_file_info info;
    unzFile *unz = unzOpen2(path, &filefunc);
    unzGoToFirstFile(unz);
    unzGetCurrentFileInfo(unz, &info, NULL, 0, NULL, 0, NULL, 0);
    *size = info.uncompressed_size;
    *buffer = malloc(*size);
    unzOpenCurrentFile(unz);
    unzReadCurrentFile(unz, *buffer, *size);
    unzCloseCurrentFile(unz);
    unzClose(unz);
    free(old_buffer);

    return APK_OK;
}

enum ApkResult
apk_read_file(AndroidApk *apk, const char *filename, char **buffer, size_t *size)
{
    assert(apk != NULL);

    unz_file_info info;
    int read;
    char *write_pos;
    int remaining;
    int is_zipfile = 0;

    if (unzLocateFile(apk->zip, filename, 2) != UNZ_OK) {
        char *filename2 = malloc(strlen(filename) + strlen(".zip") + 1);
        sprintf(filename2, "%s.zip", filename);
        if (unzLocateFile(apk->zip, filename2, 2) == UNZ_OK) {
            is_zipfile = 1;
        } else {
            return APK_ERROR;
        }
    }

    if (unzGetCurrentFileInfo(apk->zip, &info, NULL, 0,
                NULL, 0, NULL, 0) == UNZ_OK) {
        if (unzOpenCurrentFile(apk->zip) == UNZ_OK) {
            *size = info.uncompressed_size;
            *buffer = malloc(*size);

            write_pos = *buffer;
            remaining = *size;
            while (!unzeof(apk->zip)) {
                read = unzReadCurrentFile(apk->zip, write_pos, remaining);
                write_pos += read;
                remaining -= read;
            }
            unzCloseCurrentFile(apk->zip);

            if (is_zipfile) {
                return apk_uncompress_internal(apk, buffer, size);
            }
            return APK_OK;
        }
    }

    return APK_ERROR;
}

void
apk_for_each_file(AndroidApk *apk, const char *prefix,
        apk_for_each_file_callback callback)
{
    assert(apk != NULL);

    unz_file_info info;
    char filename[PATH_MAX];
    char *buf;
    size_t size;

    if (unzGoToFirstFile(apk->zip) != UNZ_OK) {
        return;
    }

    do {
        if (unzGetCurrentFileInfo(apk->zip, &info, filename, sizeof(filename),
                    NULL, 0, NULL, 0) == UNZ_OK) {
            if (memcmp(filename, prefix, strlen(prefix)) == 0) {
                if (unzOpenCurrentFile(apk->zip) == UNZ_OK) {
                    size = info.uncompressed_size;
                    buf = malloc(info.uncompressed_size);
                    size = unzReadCurrentFile(apk->zip, buf,
                            info.uncompressed_size);
                    callback(filename, buf, size);
                    free(buf);
                    unzCloseCurrentFile(apk->zip);
                }
            }
        }
    } while (unzGoToNextFile(apk->zip) == UNZ_OK);
}

void
apk_close(AndroidApk *apk)
{
    assert(apk != NULL);
    unzClose(apk->zip);
}

