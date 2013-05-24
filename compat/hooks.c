
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
};

/* fully wrapped or harmful libs that should not be loaded
 * even if provided by user (like 3D driver libs) */
static const char *builtin_libs[] = {
    "libEGL.so",
    "libGLESv1_CM.so",
    "libGLESv2.so",
};

/* this is just to not log errors if those libs are missing */
static const char *optional_libs[] = {
    // "libc.so", // not yet
    // "libm.so", // not yet
    // "libstdc++.so", // not yet
    "liblog.so",
    "libz.so",
};

static int
hook_cmp(const void *p1, const void *p2)
{
    const struct _hook *h1 = (const struct _hook *)p1;
    const struct _hook *h2 = (const struct _hook *)p2;
    return strcmp(h1->name, h2->name);
}

#define HOOK_SIZE (sizeof(struct _hook))
#define HOOKS_COUNT (sizeof(hooks) / (HOOK_SIZE))

void *get_hooked_symbol(const char *sym)
{
    struct _hook target;
    target.name = sym;

    struct _hook *result = bsearch(&target, &(hooks[0]),
            HOOKS_COUNT, HOOK_SIZE, hook_cmp);

    if (result != NULL) {
        return result->func;
    }

    if (strstr(sym, "pthread") != NULL) {
        printf("Unimplemented: %s\n", sym);
        exit(0);
    }

    return NULL;
}

int is_lib_builtin(const char *name)
{
    size_t i;

    if (name == NULL)
        return 0;

    for (i = 0; i < sizeof(builtin_libs) / sizeof(builtin_libs[0]); i++)
        if (strcmp(name, builtin_libs[i]) == 0)
            return i + 1;

    return 0;
}

int is_lib_optional(const char *name)
{
    size_t i;

    for (i = 0; i < sizeof(optional_libs) / sizeof(optional_libs[0]); i++)
        if (strcmp(name, optional_libs[i]) == 0)
            return i + 1;

    return 0;
}

void hooks_init(void)
{
    /* Sort hooks so we can use binary search in get_hooked_symbol() */
    qsort(&(hooks[0]), HOOKS_COUNT, HOOK_SIZE, hook_cmp);

    libc_wrappers_init();
}
