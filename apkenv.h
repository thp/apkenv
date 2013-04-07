#ifndef APKENV_H
#define APKENV_H

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

#include "apklib/apklib.h"
#include "jni/jni.h"

#define APKENV_NAME "apkenv"
#define APKENV_VERSION "42.3.15"
#define APKENV_WEBSITE "http://thp.io/2012/apkenv/"
#define APKENV_HEADLINE APKENV_NAME " " APKENV_VERSION " - " APKENV_WEBSITE
#define APKENV_COPYRIGHT "Copyright (c) 2012 Thomas Perl <m@thp.io>"

struct GlobalState;
struct SupportModulePriv;

struct ModuleHacks {
    int gles_landscape_to_portrait;
    int gles_downscale_images;
    int gles_no_readpixels;

    int handle_update;
    int (*system_update)(void);
};

struct SupportModule {
    struct GlobalState *global;
    struct SupportModulePriv *priv;
    char *filename;
    int priority;

    struct JNINativeInterface override_env;
    struct JNIInvokeInterface override_vm;

    int (*try_init)(struct SupportModule *self);
    void (*init)(struct SupportModule *self, int width, int height, const char *home);
    void (*input)(struct SupportModule *self, int event, int x, int y, int finger);
    void (*update)(struct SupportModule *self);
    void (*deinit)(struct SupportModule *self);
    void (*pause)(struct SupportModule *self);
    void (*resume)(struct SupportModule *self);
    int (*requests_exit)(struct SupportModule *self);

    struct SupportModule *next;
};

typedef void *(*lookup_symbol_t)(const char *method);
typedef void *(*lookup_lib_symbol_t)(const char *lib, const char *method);
typedef void (*foreach_file_t)(const char *prefix, apk_for_each_file_callback callback);
typedef int (*read_file_t)(const char *filename, char **buffer, size_t *size);
typedef void (*recursive_mkdir_t)(const char *path);
typedef void (*patch_symbol_t)(const char *symbol, void *function);

struct JniLibrary {
    struct JniLibrary *next;
    char *name;
    void *lib;
    char **method_table;
};


struct GlobalState {
    const char *apkenv_executable;
    const char *apkenv_headline;
    const char *apkenv_copyright;

    struct JNINativeInterface fake_env;
    JNIEnv env;
    struct JNIInvokeInterface fake_vm;
    JavaVM vm;

    const char *apk_filename;
    AndroidApk *apklib_handle;

    struct JniLibrary *libraries;

    struct SupportModule *support_modules;
    struct SupportModule *active_module;
    struct ModuleHacks *module_hacks;

    lookup_symbol_t lookup_symbol;
    lookup_lib_symbol_t lookup_lib_symbol;
    foreach_file_t foreach_file;
    read_file_t read_file;
    recursive_mkdir_t recursive_mkdir;
};

#define VM(global_ptr) (&((global_ptr)->vm))
#define ENV(global_ptr) (&((global_ptr)->env))


/* Android MotionEvent */
#define ACTION_DOWN 0
#define ACTION_UP 1
#define ACTION_MOVE 2

/**
 * Attribute for softfp-calling-style functions
 * (only on Harmattan - Fremantle *is* softfp)
 **/
#if defined(FREMANTLE) || defined(PANDORA)
#    define SOFTFP
#else
#    define SOFTFP __attribute__((pcs("aapcs")))
#endif

/* Forward-declarations for the Bionic linker */
void *android_dlopen(const char *filename, int flag);
void *android_dlsym(void *handle, const char *symbol);

/* Module support */
typedef int (*apkenv_module_init_t)(int version, struct SupportModule *module);
#define APKENV_MODULE_INIT "apkenv_module_init"
#define APKENV_MODULE_SUFFIX ".apkenv.so"
#define APKENV_MODULE_VERSION 0x010001

#endif /* APKENV_H */
