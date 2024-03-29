
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

#include <stdio.h>
#include <assert.h>
#include <elf.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "../apkenv.h"

#ifdef APKENV_DEBUG
#  define JNISHLIB_DEBUG_PRINTF(...) printf(__VA_ARGS__)
#else
#  define JNISHLIB_DEBUG_PRINTF(...)
#endif


char **
jni_shlib_find_methods(const char *filename)
{
    int method_table_size = 32;
    int method_table_offset = 0;
    char **method_table = calloc(method_table_size, sizeof(char*));

    FILE *fp = fopen(filename, "rb");

    char tmp[1024];
    int remaining_bytes, i;
    Elf32_Ehdr header;
    Elf32_Shdr section;

    fseek(fp, 0, SEEK_SET);
    if (fread(&header, sizeof(Elf32_Ehdr), 1, fp) != 1) {
        fprintf(stderr, "Could not read ELF header from '%s': %s\n", filename, strerror(errno));
        exit(1);
    }
    fseek(fp, header.e_shoff, SEEK_SET);

    for (i=0; i<header.e_shnum; i++) {
        if (fread(&section, sizeof(Elf32_Shdr), 1, fp) != 1) {
            fprintf(stderr, "Could not read ELF header from '%s': %s\n", filename, strerror(errno));
            exit(1);
        }
        if (section.sh_type == SHT_STRTAB) {
            remaining_bytes = section.sh_size;
            fseek(fp, section.sh_offset, SEEK_SET);
            i = 0;
            while (remaining_bytes) {
                if (i >= sizeof(tmp)) {
                    break;
                }
                tmp[i] = fgetc(fp);
                remaining_bytes--;
                if (tmp[i] == '\0') {
                    //JNISHLIB_DEBUG_PRINTF("[SHLIB] %s\n",tmp);

                    if ((memcmp(tmp, "Java_", 5) == 0) ||
                            (memcmp(tmp, "JNI_", 4) == 0) ||
                            (strcmp(tmp, "android_main") == 0)) {
                        method_table[method_table_offset++] = strdup(tmp);
                        if (method_table_offset == method_table_size) {
                            method_table_size *= 2;
                            method_table = realloc(method_table,
                                    method_table_size*sizeof(char*));
                        }
                    }
                    i = 0;
                    continue;
                }
                i++;
            }
            break;
        }
    }

    fclose(fp);
    method_table[method_table_offset] = NULL;
    return method_table;
}

void *
jni_shlib_resolve(struct GlobalState *global, const char *method)
{
    struct JniLibrary *lib = global->libraries;
    while (lib!=0) {
        char **method_table = lib->method_table;

        while (*method_table) {
            JNISHLIB_DEBUG_PRINTF("[shlib] Available symbol: %s\n",*method_table);
            if (strstr(*method_table, method) != NULL) {
                JNISHLIB_DEBUG_PRINTF("[shlib] Found symbol: %s\n",
                        *method_table);
                return apkenv_android_dlsym(lib->lib, *method_table);
            }
            method_table++;
        }
        lib = lib->next;
    }
    JNISHLIB_DEBUG_PRINTF("[shlib] Symbol not found: %s\n",method);

    return NULL;
}


void *
jni_shlib_lib_resolve(struct GlobalState *global, const char* libname, const char *method)
{
    struct JniLibrary *lib = global->libraries;
    while (lib!=0) {
        char **method_table = lib->method_table;
        if (strstr(lib->name,libname)!=0) {
            while (*method_table) {
                JNISHLIB_DEBUG_PRINTF("[shlib] Available symbol: %s in %s\n",*method_table,lib->name);
                if (strstr(*method_table, method) != NULL) {
                    JNISHLIB_DEBUG_PRINTF("[shlib] Found symbol: %s in %s\n",
                            *method_table,lib->name);
                    return apkenv_android_dlsym(lib->lib, *method_table);
                }
                method_table++;
            }
        }
        lib = lib->next;
    }

    return NULL;
}
