#ifndef APKENV_MODULES_COMMON_H
#define APKENV_MODULES_COMMON_H

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

#include "../apkenv.h"
#include "../jni/jnienv.h"

/* Macros to be used inside extension modules */
#define GLOBAL_M (self->global)
#define ENV_M ENV(GLOBAL_M)
#define VM_M VM(GLOBAL_M)
#define LOOKUP_M(method) (GLOBAL_M->lookup_symbol(method))
#define LOOKUP_LIBM(lib,method) (GLOBAL_M->lookup_lib_symbol(lib,method))

/* Version check (for compatibility) */
#define APKENV_MODULE_CHECK_VERSION(version) \
    if ((version) != APKENV_MODULE_VERSION) { \
        return APKENV_MODULE_VERSION; \
    }

/* Add init function for module export */
#define APKENV_MODULE(modulename, modulepriority) \
    int \
    apkenv_module_init(int version, struct SupportModule *module) { \
        APKENV_MODULE_CHECK_VERSION(version); \
        module->priv = &modulename ## _priv; \
        module->priority = modulepriority; \
        module->try_init = modulename ## _try_init; \
        module->init = modulename ## _init; \
        module->input = modulename ## _input; \
        module->update = modulename ## _update; \
        module->deinit = modulename ## _deinit; \
        module->pause = modulename ## _pause; \
        module->resume = modulename ## _resume; \
        module->requests_exit = modulename ## _requests_exit; \
        return APKENV_MODULE_VERSION; \
    }


/* Module priorities */
enum {
    /* For really generic modules - lowest priority */
    MODULE_PRIORITY_GENERIC = 1 << 1,

    /* For modules targetting a specific engine */
    MODULE_PRIORITY_ENGINE = 1 << 3,

    /* For modules targetting a specific game */
    MODULE_PRIORITY_GAME = 1 << 6,

    /* For modules targetting a specific version of a game */
    MODULE_PRIORITY_GAME_VERSION = 1 << 9,
};

/* JNI functions for JNI_OnLoad() and JNI_OnUnLoad() */
typedef void (*jni_onload_t)(JavaVM *vm, void *reserved) SOFTFP;
typedef void (*jni_onunload_t)(JavaVM *vm, void *reserved) SOFTFP;

#endif /* APKENV_MODULES_COMMON_H */
