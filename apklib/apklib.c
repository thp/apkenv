
/**
 * apkenv
 * Copyright (c) 2012, Thomas Perl <m@thp.io>
 * Copyright (c) 2013, Gražvydas "notaz" Ignotas <notasas@gmail.com>
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
#include <unistd.h>

#include "unzip.h"
#include "ioapi_mem.h"
#include "resource_types.h"

#include "apklib.h"


#define DBG(msg) fprintf(stderr,"%s(%d): %s\n",__FILE__,__LINE__,#msg)

AndroidApk *
apk_open(const char *filename)
{
    AndroidApk *apk = calloc(1, sizeof(AndroidApk));
    apk->zip = unzOpen(filename);
    return apk;
}

struct SharedLibrary*
apk_get_shared_libraries(AndroidApk *apk, const char* libdir)
{
    assert(apk != NULL);

    char filename[PATH_MAX];
    FILE *result = NULL;
    char buf[64*1024];
    int read;
    struct SharedLibrary *libs = 0;
    struct SharedLibrary *head = 0;
    int libdirlen = strlen(libdir);

    if (unzGoToFirstFile(apk->zip) != UNZ_OK) {
        return NULL;
    }

    do {
        if (unzGetCurrentFileInfo(apk->zip, NULL, filename, sizeof(filename), NULL, 0, NULL, 0) == UNZ_OK) {
            if (memcmp(filename, libdir, libdirlen) == 0) {
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
                    if (libs==0) {
                        libs = malloc(sizeof(struct SharedLibrary));
                        head = libs;
                        libs->next = 0;
                    } else {
                        libs->next = malloc(sizeof(struct SharedLibrary));
                        libs->next->next = 0;
                        libs = libs->next;
                    }
                    libs->filename = strdup(filename);
                }
            }
        }
    } while (unzGoToNextFile(apk->zip) == UNZ_OK);

    return head;
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

static char *
apk_get_res_string(const void *str, int is_utf8)
{
    const uint16_t *str16;
    uint16_t len;
    uint32_t i;
    char *out;

    memcpy(&len, str, sizeof(len));
    out = malloc(len + 1);
    if (out == NULL)
        return NULL;

    if (is_utf8) {
        memcpy(out, (const char *)str + 2, len);
    } else {
        str16 = (const uint16_t *)str + 1;
        for (i = 0; i < len; i++)
            out[i] = str16[i];
    }
    out[len] = 0;
    return out;
}

enum ApkResult
apk_read_resources(AndroidApk *apk, struct ResourceStrings *rstrings)
{
    char *buf, *p, *p2, *p_end;
    char *values, *keys;
    struct ResTable_header *res_table_h;
    struct ResTable_package *res_table_package;
    struct ResChunk_header *chunk;
    struct ResStringPool_header *value_pool, *key_pool;
    struct ResTable_type *res_table;
    struct ResTable_entry *entry;
    struct Res_value *value;
    int *value_index, *key_index;
    size_t rstr_i, rstr_alloc;
    enum ApkResult ret;
    size_t size;

    assert(apk != NULL && rstrings != NULL);

    ret = apk_read_file(apk, "resources.arsc", &buf, &size);
    if (ret != APK_OK) {
        fprintf(stderr, "missing/bad resources.arsc\n");
        return ret;
    }

    rstrings->count = rstr_i = 0;
    rstrings->app_name = NULL;
    rstr_alloc = 32;
    rstrings->entries = malloc(rstr_alloc * sizeof(rstrings->entries[0]));
    if (rstrings->entries == NULL) {
        ret = APK_ERROR;
        goto out;
    }

    p = buf;
    res_table_h = (void *)p;
    assert(res_table_h->header.type == RES_TABLE_TYPE);
    p += res_table_h->header.headerSize;

    value_pool = (void *)p;
    assert(value_pool->header.type == RES_STRING_POOL_TYPE);
    value_index = (int *)(p + value_pool->header.headerSize);
    values = p + value_pool->stringsStart;
    p += value_pool->header.size;

    res_table_package = (void *)p;
    assert(res_table_package->header.type == RES_TABLE_PACKAGE_TYPE);
    key_pool = (void *)(p + res_table_package->keyStrings);

    p = (void *)key_pool;
    assert(key_pool->header.type == RES_STRING_POOL_TYPE);
    key_index = (int *)(p + key_pool->header.headerSize);
    keys = p + key_pool->stringsStart;
    p += key_pool->header.size;

    /* handle all ResTable chunks */
    for (; p < buf + size; p += chunk->size) {
        chunk = (void *)p;
        if (chunk->type != RES_TABLE_TYPE_TYPE)
            continue;

        res_table = (void *)p;
        p2 = p + res_table->entriesStart;
        /* res_table->entryCount is unreliable? */
        p_end = p + chunk->size;
        for (; p2 < p_end; p2 += entry->size + value->size) {
            entry = (void *)p2;
            value = (void *)(p2 + entry->size);
            if (entry->flags & ~FLAG_PUBLIC) {
                // printf("flags %x\n", entry->flags);
                break;
            }
            if (value->dataType != TYPE_STRING) {
                //printf("  type %x\n", value->dataType);
                // TODO: at least TYPE_INT_BOOLEAN might be useful
                continue;
            }
            if (entry->key.index >= key_pool->stringCount) {
                fprintf(stderr, "key index out of range: %u >= %u\n",
                    entry->key.index, key_pool->stringCount);
                break;
            }
            if (value->data >= value_pool->stringCount) {
                fprintf(stderr, "value index out of range: %u >= %u\n",
                    value->data, value_pool->stringCount);
                break;
            }
            rstrings->entries[rstr_i].key = apk_get_res_string(
                keys + key_index[entry->key.index], key_pool->flags & UTF8_FLAG);
            rstrings->entries[rstr_i].value = apk_get_res_string(
                values + value_index[value->data], value_pool->flags & UTF8_FLAG);
            if (strcmp(rstrings->entries[rstr_i].key, "app_name") == 0)
                rstrings->app_name = rstrings->entries[rstr_i].value;

            rstr_i++;
            if (rstr_i >= rstr_alloc) {
                rstr_alloc *= 2;
                rstrings->entries = realloc(rstrings->entries,
                    rstr_alloc * sizeof(rstrings->entries[0]));
                if (rstrings->entries == NULL)
                    goto out;
            }
        }
    }
    rstrings->count = rstr_i;

out:
    free(buf);
    return ret;
}

void
apk_close(AndroidApk *apk)
{
    assert(apk != NULL);
    unzClose(apk->zip);
}

