
/**
 * apkenv
 * Copyright (c) 2012, Thomas Perl <m@thp.io>
 * Based on code from libhybris: Copyright (c) 2012 Carsten Munk
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

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hooks.h"

#include "libc_wrappers.h"
#include "liblog_wrappers.h"
#include "gles_wrappers.h"
#include "gles2_wrappers.h"
#include "pthread_wrappers.h"


char my___sF[SIZEOF_SF * 3];

static struct _hook hooks[] = {
#include "libc_mapping.h"
#include "liblog_mapping.h"
#include "gles_mapping.h"
#include "gles2_mapping.h"
#include "pthread_mapping.h"

  {"__sF", my___sF},

  {NULL, NULL},
};

void *get_hooked_symbol(char *sym)
{
    struct _hook *ptr = &hooks[0];

    while (ptr->name != NULL) {
        if (strcmp(sym, ptr->name) == 0) {
            return ptr->func;
        }
        ptr++;
    }

    if (strstr(sym, "pthread") != NULL) {
        printf("Unimplemented: %s\n", sym);
        exit(0);
    }

    return NULL;
}

void hooks_init(void)
{
    libc_wrappers_init();
}
