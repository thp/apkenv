
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

#include "wrappers.h"
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

static int
hook_cmp(const void *p1, const void *p2)
{
    const struct _hook *h1 = (const struct _hook *)p1;
    const struct _hook *h2 = (const struct _hook *)p2;
    return strcmp(h1->name, h2->name);
}

#define HOOK_SIZE (sizeof(struct _hook))
#define HOOKS_COUNT (sizeof(hooks) / (HOOK_SIZE))

#ifdef DEBUG_TRACE_METHODS

static int
blacklist_cmp(const void *p1, const void *p2)
{
    const char *str1 = *(const char**)p1;
    const char *str2 = *(const char**)p2;
    return strcmp(str1,str2);
}

char *blacklisted_methods[] = {"dlsym", "dlopen", "dlclose", "dlerror", "dladdr", "android_dl_unwind_find_exidx", "dl_iterate_phdr", "__div0", "__divdf3", "__divdi3", "__divsi3", "__udivdi3", "__udivsi3", "__muldi3", "__muldf3", "__mulsf3", "__gnu_ldivmod_helper", "__gnu_uldivmod_helper"};

int is_blacklisted(const char *sym_name)
{
    if(strncmp("__aeabi", sym_name, 7) == 0) return 1;
    char *result = bsearch(&sym_name, &(blacklisted_methods[0]), sizeof(blacklisted_methods)/sizeof(char*), sizeof(char*), blacklist_cmp);
    return (result != NULL);
}

void print_fun_name(void *function, char *name, char *msg)
{
    // here we can simply use the stack and everything
    // because the compiler cares about pushing to the
    // stack and poping everything back afterwards
    printf("%s: %s@%x\n",msg,name,function);
    // we cannot simply call the function here
    // because the function expects the stack and registers
    // to be in a state as if this function and the generated
    // wrapper code never existed
    // but we can print debug information
}

const char *latehook = "calling late hooked method";
const char *stdhook = "calling unhooked method";
const char *dynhook = "calling dynamically loaded method";

void *assemble_wrapper(const char *symbol, void *addr, uint8_t type)
{
    if(is_blacklisted(symbol)) return addr;

    printf("assembling wrapper for: %s@0x%x\n", symbol, addr);

    void *wrapper_addr = 0;
    wrapper_addr = mmap(NULL,
                        64, // instructions, be sure your allocating enough memory!
                        PROT_READ | PROT_WRITE | PROT_EXEC,
                        MAP_ANONYMOUS | MAP_PRIVATE,
                        0,
                        0);
    if(MAP_FAILED == wrapper_addr)
    {
       printf("mmap failed, cannot create wrapper for %s",symbol);
       return addr;
    }
    int helper = 0;
    #include "wrapper.instructions"
    // symbols
    ((int32_t*)wrapper_addr)[helper++] = (uint32_t)symbol;
    ((int32_t*)wrapper_addr)[helper++] = (uint32_t)addr;
    ((int32_t*)wrapper_addr)[helper++] = (uint32_t)print_fun_name;

    if(WRAPPER_LATEHOOK == type)
    {
        ((int32_t*)wrapper_addr)[helper++] = (uint32_t)latehook;
    }
    else if(WRAPPER_DYNHOOK == type)
    {
        ((int32_t*)wrapper_addr)[helper++] = (uint32_t)dynhook;
    }
    else
    {
        ((int32_t*)wrapper_addr)[helper++] = (uint32_t)stdhook;
    }
    
    register_wrapper(wrapper_addr,64,symbol,WRAPPER_LATEHOOK);
    
    return wrapper_addr;
}
#endif

void *get_hooked_symbol(const char *sym, int dieifpthread)
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
        if(dieifpthread) exit(0);
    }

    return NULL;
}

void hooks_init(void)
{
    /* Sort hooks so we can use binary search in get_hooked_symbol() */
    qsort(&(hooks[0]), HOOKS_COUNT, HOOK_SIZE, hook_cmp);
#ifdef DEBUG_TRACE_METHODS
    // sort blacklists
    qsort(&(blacklisted_methods[0]), sizeof(blacklisted_methods)/sizeof(char*), sizeof(char*), blacklist_cmp);
#endif
    
    libc_wrappers_init();
}

void hooks_deinit(void)
{
#ifdef DEBUG_TRACE_METHODS
    deinit_wrappers();
#endif
}
