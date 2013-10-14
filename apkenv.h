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
#define APKENV_VERSION "42.4.0"
#define APKENV_WEBSITE "http://thp.io/2012/apkenv/"
#define APKENV_HEADLINE APKENV_NAME " " APKENV_VERSION " - " APKENV_WEBSITE
#define APKENV_COPYRIGHT "Copyright (c) 2012, 2013 Thomas Perl <m@thp.io>"

struct GlobalState;
struct SupportModule;
struct SupportModulePriv;

struct ModuleHacks {
    int gles_landscape_to_portrait;
    int gles_downscale_images;
    int gles_no_readpixels;
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
    void (*key_input)(struct SupportModule *self, int event, int keycode, int unicode);
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
typedef void *(*read_file_to_jni_array_t)(const char *filename);
typedef void (*recursive_mkdir_t)(const char *path);
typedef const char *(*lookup_resource_t)(const char *key);
typedef void (*patch_symbol_t)(const char *symbol, void *function);

struct JniLibrary {
    struct JniLibrary *next;
    char *name;
    void *lib;
    char **method_table;
};

enum PlatformPath {
    /* Path to where .apk files should be installed, or NULL if unsupported */
    PLATFORM_PATH_INSTALL_DIRECTORY = 1,
    /* Path to where application data should be stored */
    PLATFORM_PATH_DATA_DIRECTORY = 2,
    /* Path from where support modules should be loaded */
    PLATFORM_PATH_MODULE_DIRECTORY = 3,
};

struct PlatformSupport {
    /* Initialize the video mode, return nonzero on success, zero on error */
    int (*init)(int gles_version);

    /* Get a device path (see enum PlatformPath), don't free() the result */
    const char *(*get_path)(enum PlatformPath which);

    /* Get the size of the device screen */
    void (*get_size)(int *width, int *height);

    /* Process input events and forward to "module", return nonzero to exit */
    int (*input_update)(struct SupportModule *module);

    /* Called once per rendering frame to swap buffers, etc... */
    void (*update)();

    /* Called when apkenv shuts down (for platform-specific cleanups) */
    void (*exit)();
};

struct GlobalState {
    const char *apkenv_executable;
    const char *apkenv_headline;
    const char *apkenv_copyright;

    struct PlatformSupport *platform;

    struct JNINativeInterface fake_env;
    JNIEnv env;
    struct JNIInvokeInterface fake_vm;
    JavaVM vm;

    const char *apk_filename;
    AndroidApk *apklib_handle;
    const void *apk_in_mem; /* can be NULL if APK is too large */
    size_t apk_size;
    int apk_fd;

    struct JniLibrary *libraries;

    struct SupportModule *support_modules;
    struct SupportModule *active_module;
    struct ModuleHacks *module_hacks;

    struct ResourceStrings resource_strings;

    int loader_seen_glesv1;
    int loader_seen_glesv2;

    lookup_symbol_t lookup_symbol;
    lookup_lib_symbol_t lookup_lib_symbol;
    foreach_file_t foreach_file;
    read_file_t read_file;
    read_file_to_jni_array_t read_file_to_jni_array;
    recursive_mkdir_t recursive_mkdir;
    lookup_resource_t lookup_resource;
};

#define VM(global_ptr) (&((global_ptr)->vm))
#define ENV(global_ptr) (&((global_ptr)->env))


/* Android MotionEvent/KeyEvent */
#define ACTION_DOWN 0
#define ACTION_UP 1
#define ACTION_MOVE 2
#define ACTION_MULTIPLE 2

/**
 * Attribute for softfp-calling-style functions
 * (only on Harmattan - Fremantle *is* softfp)
 **/
#if defined(FREMANTLE) || defined(PANDORA)
#    define SOFTFP
#else
#    define SOFTFP __attribute__((pcs("aapcs")))
#endif

/* multitouch support definitions */
#if defined(FREMANTLE) || defined(PANDORA)
#   define HAVE_MULTITOUCH 0
#else
#   define HAVE_MULTITOUCH 1
#endif

/* Forward-declarations for the Bionic linker */
void *android_dlopen(const char *filename, int flag);
void *android_dlsym(void *handle, const char *symbol);

/* Module support */
typedef int (*apkenv_module_init_t)(int version, struct SupportModule *module);
#define APKENV_MODULE_INIT "apkenv_module_init"
#define APKENV_MODULE_SUFFIX ".apkenv.so"
#define APKENV_MODULE_VERSION 0x010002

#endif /* APKENV_H */
