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


/**
 * segfault debug module
 * see // http://stackoverflow.com/questions/77005/how-to-generate-a-stacktrace-when-my-gcc-c-app-crashes
 *
 */


#ifndef __USE_GNU
#define __USE_GNU
#endif

#include <execinfo.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ucontext.h>
#include <unistd.h>
#include <dlfcn.h>
#include <stdint.h>

#include "../apkenv.h"


/* This structure mirrors the one found in /usr/include/asm/ucontext.h */
typedef struct _sig_ucontext {
 unsigned long     uc_flags;
 struct ucontext   *uc_link;
 stack_t           uc_stack;
 struct sigcontext uc_mcontext;
 sigset_t          uc_sigmask;
} sig_ucontext_t;

void
apkenv_debug_dump_stack()
{
    intptr_t tmp = 0xcafebabe;
    void **here = (void **)&tmp;

    uint32_t stack_top = (intptr_t)here;
    uint32_t stack_bot = (intptr_t)apkenv_base_of_stack;
    while ((void *)here <= apkenv_base_of_stack) {
        Dl_info out = {0};
        dladdr(*here, &out);
        uint32_t value = (uint32_t)(intptr_t)*here;
        fprintf(stderr, "on stack 0x%08x: %08x", (uint32_t)(intptr_t)here, value);
        if (value <= stack_bot && value >= stack_top) {
            fprintf(stderr, " -> pointer to stack (offset = %d)", value - (uint32_t)(intptr_t)here);
        }
        if (out.dli_fname != NULL) {
            fprintf(stderr, " (fname=%s +%x)", out.dli_fname, *here - out.dli_fbase);
        }

        if (out.dli_sname != NULL) {
            fprintf(stderr, " (sname=%s +%x)", out.dli_sname, *here - out.dli_saddr);
        }

        fprintf(stderr, "\n");
        ++here;
    }
}


void crit_err_hdlr(int sig_num, siginfo_t * info, void * ucontext)
{
    void *             array[50];
    void *             caller_address;
    char **            messages;
    int                size, i;
    sig_ucontext_t *   uc;

    uc = (sig_ucontext_t *)ucontext;

    caller_address = (void *) uc->uc_mcontext.arm_pc;

    apkenv_debug_dump_stack();

    Dl_info out = {0};
    dladdr(info->si_addr, &out);
    Dl_info outa = {0};
    apkenv_android_dladdr(info->si_addr, &outa);

    if (out.dli_fname == NULL) {
        out.dli_fname = outa.dli_fname;
    }
    if (out.dli_sname == NULL) {
        out.dli_sname = outa.dli_sname;
    }

    Dl_info out_caller = {0};
    dladdr(caller_address, &out_caller);
    Dl_info out_callera = {0};
    apkenv_android_dladdr(caller_address, &out_callera);

    if (out_caller.dli_fname == NULL) {
        out_caller.dli_fname = out_callera.dli_fname;
    }
    if (out_caller.dli_sname == NULL) {
        out_caller.dli_sname = out_callera.dli_sname;
    }

    fprintf(stderr, "signal %d (%s), address is %p (%s / %s) from %p (%s / %s)\n",
            sig_num, strsignal(sig_num),

            info->si_addr,
            out.dli_fname ?: "?",
            out.dli_sname ?: "?",

            (void *)caller_address,
            out_caller.dli_fname ?: "?",
            out_caller.dli_sname ?: "?");

    // Output memory map if possible
    FILE *fp = fopen("/proc/self/maps", "r");
    if (fp) {
        char line[512];
        while (fgets(line, 512, fp) != NULL) {
            fprintf(stderr, "maps: %s", line);
        }
        fclose(fp);
    }

    size = backtrace(array, 50);

    /* overwrite sigaction with caller's address */
    array[1] = caller_address;

    messages = backtrace_symbols(array, size);

    /* skip first stack frame (points here) */
    for (i = 1; i < size && messages != NULL; ++i)
    {
        fprintf(stderr, "[bt]: (%d) %s\n", i, messages[i]);
    }

    free(messages);

    exit(EXIT_FAILURE);
}


void debug_init()
{
    struct sigaction sigact;

    sigact.sa_sigaction = crit_err_hdlr;
    sigact.sa_flags = SA_RESTART | SA_SIGINFO;

    if (sigaction(SIGSEGV, &sigact, (struct sigaction *)NULL) != 0)
    {
        fprintf(stderr, "error setting signal handler for %d (%s)\n",SIGSEGV, strsignal(SIGSEGV));
    }
}



