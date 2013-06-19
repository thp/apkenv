
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

#include <dlfcn.h>
#include <assert.h>
#include <stdio.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>

#include <SDL/SDL.h>

#include "jni/jnienv.h"
#include "jni/shlib.h"
#include "apklib/apklib.h"
#include "apklib/keycodes.h"
#include "debug/debug.h"
#include "compat/gles_wrappers.h"
#include "compat/gles2_wrappers.h"
#include "linker/linker.h"
#include "compat/hooks.h"

#include "apkenv.h"
#include "platform.h"

/* Global application state */
struct GlobalState global;
struct ModuleHacks global_module_hacks;

static unsigned char keymap[] = {
    /* 0-9, A-Z filled in later */
    [SDLK_UNKNOWN]  = AKEYCODE_UNKNOWN,
    [SDLK_UP]       = AKEYCODE_DPAD_UP,
    [SDLK_DOWN]     = AKEYCODE_DPAD_DOWN,
    [SDLK_RIGHT]    = AKEYCODE_DPAD_RIGHT,
    [SDLK_LEFT]     = AKEYCODE_DPAD_LEFT,
    [SDLK_COMMA]    = AKEYCODE_COMMA,
    [SDLK_PERIOD]   = AKEYCODE_PERIOD,
    [SDLK_RALT]     = AKEYCODE_ALT_RIGHT,
    [SDLK_LSHIFT]   = AKEYCODE_SHIFT_LEFT,
    [SDLK_TAB]      = AKEYCODE_TAB,
    [SDLK_SPACE]    = AKEYCODE_SPACE,
    [SDLK_RETURN]   = AKEYCODE_ENTER,
    [SDLK_DELETE]   = AKEYCODE_DEL,
    [SDLK_MINUS]    = AKEYCODE_MINUS,
    [SDLK_EQUALS]   = AKEYCODE_EQUALS,
    [SDLK_LEFTBRACKET]  = AKEYCODE_LEFT_BRACKET,
    [SDLK_RIGHTBRACKET] = AKEYCODE_RIGHT_BRACKET,
    [SDLK_BACKSLASH]    = AKEYCODE_BACKSLASH,
    [SDLK_SEMICOLON]    = AKEYCODE_SEMICOLON,
    [SDLK_SLASH]    = AKEYCODE_SLASH,
    [SDLK_AT]       = AKEYCODE_AT,
    [SDLK_PLUS]     = AKEYCODE_PLUS,
    [SDLK_BACKSPACE]= AKEYCODE_BACK,
    [SDLK_F1]       = AKEYCODE_MENU,
    [SDLK_F2]       = AKEYCODE_HOME,
    [SDLK_F3]       = AKEYCODE_SOFT_LEFT,
    [SDLK_F4]       = AKEYCODE_SOFT_RIGHT,
#ifndef PANDORA
    [SDLK_LALT]     = AKEYCODE_ALT_LEFT,
    [SDLK_RSHIFT]   = AKEYCODE_SHIFT_RIGHT,
    [SDLK_PAGEUP]   = AKEYCODE_PAGE_UP,
    [SDLK_PAGEDOWN] = AKEYCODE_PAGE_DOWN,
#else
    [SDLK_LALT]     = AKEYCODE_BUTTON_START,
    [SDLK_LCTRL]    = AKEYCODE_BUTTON_SELECT,
    [SDLK_PAGEUP]   = AKEYCODE_BUTTON_Y,
    [SDLK_PAGEDOWN] = AKEYCODE_BUTTON_X,
    [SDLK_HOME]     = AKEYCODE_BUTTON_A,
    [SDLK_END]      = AKEYCODE_BUTTON_B,
    [SDLK_RSHIFT]   = AKEYCODE_BUTTON_L1,
    [SDLK_RCTRL]    = AKEYCODE_BUTTON_R1,
#endif
    [SDLK_LAST]     = AKEYCODE_UNKNOWN,
};

static void *
lookup_symbol_impl(const char *method)
{
    return jni_shlib_resolve(&global, method);
}

static void *
lookup_lib_symbol_impl(const char *lib, const char *method)
{
    return jni_shlib_lib_resolve(&global, lib, method);
}

static void
foreach_file_impl(const char *prefix, apk_for_each_file_callback callback)
{
    apk_for_each_file(global.apklib_handle, prefix, callback);
}

static int
read_file_impl(const char *filename, char **buffer, size_t *size)
{
    return (apk_read_file(global.apklib_handle,
                filename, buffer, size) == APK_OK);
}

static void *
read_file_to_jni_array_impl(const char *filename)
{
    struct dummy_array *array = NULL;
    char *buffer = NULL;
    size_t size = 0;

    if (apk_read_file(global.apklib_handle, filename, &buffer, &size) == APK_OK) {
        array = malloc(sizeof(*array));
        if (array == NULL)
            return NULL;
        array->data = buffer;
        array->length = size;
        array->element_size = 1;
    }

    return array;
}

static const char *
lookup_resource_impl(const char *key)
{
    size_t i;

    if (strcmp(key, "app_name") == 0)
        return global.resource_strings.app_name;

    for (i = 0; i < global.resource_strings.count; i++)
        if (strcmp(key, global.resource_strings.entries[i].key) == 0)
            return global.resource_strings.entries[i].value;

    return NULL;
}

static void
register_module(struct SupportModule *module)
{
    struct SupportModule **current = &(global.support_modules);

    while ((*current) != NULL) {
        if ((*current)->priority < module->priority) {
            break;
        }
        current = &((*current)->next);
    }

    module->next = *current;
    *current = module;
}

static void
load_module(const char *filename)
{
    void *dl = dlopen(filename, RTLD_LAZY);
    if (dl == NULL) {
        printf("Cannot load module: %s\n", filename);
        return;
    }

    struct SupportModule *module = calloc(1, sizeof(struct SupportModule));
    module->global = &global;
    module->filename = strdup(filename);

    apkenv_module_init_t module_init = (apkenv_module_init_t)dlsym(dl,
            APKENV_MODULE_INIT);

    if (module_init != NULL) {
        int version = module_init(APKENV_MODULE_VERSION, module);
        switch (version) {
            case 0:
                printf("Cannot init module: %s\n", filename);
                break;
            case APKENV_MODULE_VERSION:
                printf("Loaded module: %s (%d)\n", filename, module->priority);
                register_module(module);
                return;
            default:
                printf("Wrong module ABI version: %x (need %x)\n",
                        version, APKENV_MODULE_VERSION);
                break;
        }
    } else {
        printf("Cannot find %s(): %s\n", APKENV_MODULE_INIT, filename);
    }

    free(module->filename);
    free(module);
    dlclose(dl);
}

static void
load_modules(const char *dirname)
{
    DIR *dir = opendir(dirname);
    if (dir == NULL) {
        return;
    }

    char tmp[PATH_MAX];
    strcpy(tmp, dirname);
    strcat(tmp, "/");
    char *filename = tmp + strlen(dirname) + 1;

    struct dirent *ent;
    while ((ent = readdir(dir)) != NULL) {
        int len = strlen(ent->d_name) - strlen(APKENV_MODULE_SUFFIX);
        if (len > 0) {
            char *suffix = ent->d_name + len;
            if (strcmp(suffix, APKENV_MODULE_SUFFIX) == 0) {
                strcpy(filename, ent->d_name);
                load_module(tmp);
            }
        }
    }
    closedir(dir);
}

static void
install_overrides(struct SupportModule *module)
{
    int i;
    void **override, **fake;

    /* Install overrides for JNIEnv */
    override = (void**)(&(module->override_env));
    fake = (void**)(&(global.fake_env));
    for (i=0; i<sizeof(module->override_env)/sizeof(void*); i++) {
        if (override[i]) {
            fake[i] = override[i];
        }
    }

    /* Install overrides for JavaVM */
    override = (void**)(&(module->override_vm));
    fake = (void**)(&(global.fake_vm));
    for (i=0; i<sizeof(module->override_vm)/sizeof(void*); i++) {
        if (override[i]) {
            fake[i] = override[i];
        }
    }
}

static void
usage()
{
    if (platform_getinstalldirectory()==0) {
        printf("Usage: %s <file.apk>\n",global.apkenv_executable);
    } else {
        printf("Usage: %s [--install] <file.apk>\n",global.apkenv_executable);
    }
    exit(1);
}

static const char *
apk_basename(const char *filename)
{
    char *fn = strrchr(filename, '/');
    if (fn != NULL) {
        return fn + 1;
    }
    return filename;
}

static void
operation(const char *operation, const char *filename)
{
    if (strcmp(operation, "--install") == 0 && platform_getinstalldirectory()!=0) {
        char apkenv_absolute[PATH_MAX];
        char apk_absolute[PATH_MAX];

        readlink("/proc/self/exe", apkenv_absolute, sizeof(apkenv_absolute));
        /* On Linux, realpath(3) always returns an absolute path */
        assert(realpath(filename, apk_absolute) != NULL);

        const char *app_name = apk_basename(filename);

        char icon_filename[PATH_MAX];
        sprintf(icon_filename, "%s%s.png", platform_getdatadirectory(), app_name);

        struct stat st;
        if (stat(icon_filename, &st) != 0) {
            // Extract icon from .apk here
            AndroidApk *apk = apk_open(filename);
            assert(apk != NULL);

            char *icon_buffer;
            size_t icon_size;
            /**
             * FIXME: The icon isn't always called like that, we should look
             * into the Android manifest and get the icon filename and also
             * the "nice" application name (for the .desktop file below) from
             * there (see also: http://stackoverflow.com/questions/2097813/)
             **/
            enum ApkResult result = apk_read_file(apk,
                    "res/drawable-hdpi/icon.png", &icon_buffer, &icon_size);
            apk_close(apk);

            if (result == APK_OK) {
                FILE *fp = fopen(icon_filename, "wb");
                assert(fp != NULL);
                fwrite(icon_buffer, icon_size, 1, fp);
                fclose(fp);
                printf("Extracted icon: %s\n", icon_filename);
                free(icon_buffer);
            } else {
                printf("Cannot extract icon from %s\n", filename);
                exit(2);
            }
        }

        char desktop_filename[PATH_MAX];
        recursive_mkdir(platform_getinstalldirectory());
        sprintf(desktop_filename, "%s/%s.desktop", platform_getinstalldirectory(),
                app_name);

        FILE *desktop = fopen(desktop_filename, "w");
        assert(desktop != NULL);
        fprintf(desktop,
                "[Desktop Entry]\n"
                "Name=%s\n"
                "Exec=invoker --single-instance --type=e %s %s\n"
                "Icon=%s\n"
                "Terminal=false\n"
                "Type=Application\n"
                "Categories=Game;\n",
                app_name, apkenv_absolute, apk_absolute, icon_filename);
        fclose(desktop);
        printf("Installed: %s\n", desktop_filename);
        exit(0);
    }

    usage();
}


static int
system_init(int gles_version)
{
    int i, j;

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_JOYSTICK) < 0) {
        printf("SDL Init failed.\n");
        return 0;
    }

    if ( !platform_init(gles_version) ) {
        printf("platform_init failed.\n");
        return 0;
    }

    gles_extensions_init();
    gles2_init();

    SDL_ShowCursor(0);

    for (i = SDLK_0, j = AKEYCODE_0; i <= SDLK_9; i++, j++)
        keymap[i] = j;
    for (i = SDLK_a, j = AKEYCODE_A; i <= SDLK_z; i++, j++)
        keymap[i] = j;

    /* SDL loads some libs.. */
    notify_gdb_of_libraries();

    return 1;
}

static int
system_update(void)
{
    return platform_update();
}

static void
system_exit()
{
    platform_exit();
}

/* returns 1 on quit key/event, 0 otherwise */
static int
input_update(struct SupportModule *module)
{
    static int emulate_multitouch = 0;
    const int emulate_finger_id = 2;

    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_KEYDOWN) {
#ifdef PANDORA
            if (e.key.keysym.sym==SDLK_ESCAPE) {
                return 1;
            }
            else if (e.key.keysym.sym==SDLK_RSHIFT) {
                //emulate_multitouch = 1;
                module->input(module,ACTION_DOWN, platform_getscreenwidth() / 2 - 1, platform_getscreenheight() / 2, emulate_finger_id);
            }
            else
#endif
            module->key_input(module, ACTION_DOWN, keymap[e.key.keysym.sym], e.key.keysym.unicode);
        }
        else if (e.type == SDL_KEYUP) {
#ifdef PANDORA
            if (e.key.keysym.sym==SDLK_RSHIFT) {
                //emulate_multitouch = 0;
                module->input(module,ACTION_UP, platform_getscreenwidth() / 2 - 1, platform_getscreenheight() / 2, emulate_finger_id);
            }
            else
#endif
            module->key_input(module, ACTION_UP, keymap[e.key.keysym.sym], e.key.keysym.unicode);
        } else if (e.type == SDL_MOUSEBUTTONDOWN) {
            module->input(module, ACTION_DOWN, e.button.x, e.button.y, e.button.which);
            if (emulate_multitouch) {
                module->input(module,ACTION_DOWN, platform_getscreenwidth()-e.button.x, platform_getscreenheight()-e.button.y,emulate_finger_id);
            }
        } else if (e.type == SDL_MOUSEBUTTONUP) {
            module->input(module, ACTION_UP, e.button.x, e.button.y, e.button.which);
            if (emulate_multitouch) {
                module->input(module,ACTION_UP, platform_getscreenwidth()-e.button.x, platform_getscreenheight()-e.button.y,emulate_finger_id);
            }
        } else if (e.type == SDL_MOUSEMOTION) {
            module->input(module, ACTION_MOVE, e.motion.x, e.motion.y, e.motion.which);
            if (emulate_multitouch) {
                module->input(module,ACTION_MOVE, platform_getscreenwidth()-e.button.x, platform_getscreenheight()-e.button.y,emulate_finger_id);
            }
        } else if (e.type == SDL_QUIT) {
            return 1;
        } else if (e.type == SDL_ACTIVEEVENT) {
            if (e.active.state == SDL_APPACTIVE && e.active.gain == 0) {
                module->pause(module);
                while (1) {
                    SDL_WaitEvent(&e);
                    if (e.type == SDL_ACTIVEEVENT) {
                        if (e.active.state == SDL_APPACTIVE &&
                                e.active.gain == 1) {
                            break;
                        }
                    } else if (e.type == SDL_QUIT) {
                        return 1;
                    }
                }
                module->resume(module);
            }
        }
    }

    return 0;
}

int main(int argc, char **argv)
{
    debug_init();

    char **tmp;

    const char *main_data_dir = platform_getdatadirectory();
    recursive_mkdir(main_data_dir);

    global.apkenv_executable = argv[0];
    global.apkenv_headline = APKENV_HEADLINE;
    global.apkenv_copyright = APKENV_COPYRIGHT;

    printf("%s\n%s\n\n", global.apkenv_headline, global.apkenv_copyright);

    switch (argc) {
        case 2:
            /* One argument - the .apk (continue below) */
            break;
        case 3:
            /* Two arguments - operation + the apk */
            operation(argv[1], argv[2]);
            break;
        default:
            /* Wrong number of arguments */
            usage();
    }

    memset(&global_module_hacks,0,sizeof(global_module_hacks));

    global.lookup_symbol = lookup_symbol_impl;
    global.lookup_lib_symbol = lookup_lib_symbol_impl;
    global.foreach_file = foreach_file_impl;
    global.read_file = read_file_impl;
    global.read_file_to_jni_array = read_file_to_jni_array_impl;
    global.recursive_mkdir = recursive_mkdir;
    global.lookup_resource = lookup_resource_impl;
    global.module_hacks = &global_module_hacks;

    global.module_hacks->system_update = system_update;
    global.module_hacks->input_update = input_update;

    hooks_init();
    jnienv_init(&global);
    javavm_init(&global);
    global.apk_filename = strdup(argv[argc-1]);
    global.apklib_handle = apk_open(global.apk_filename);
    global.support_modules = NULL;

    static const char* libdir[] = {
        "assets/libs/armeabi-v7a",
        "assets/libs/armeabi",
        "lib/armeabi-v7a",
        "lib/armeabi",
        0
    };
    static const char* libblacklist[] = {
        "libs3eflurry_ext.so",
        0
    };
    int ilib = 0;
    struct SharedLibrary *shlibs = 0;
    while (libdir[ilib]!=0) {
        shlibs = apk_get_shared_libraries(global.apklib_handle,libdir[ilib],main_data_dir);
        if (shlibs!=0)
            break;
        ilib ++;
    }
    if (shlibs==0) {
        printf("Not a native APK.\n");
        return 0;
    }

    struct JniLibrary *lib = malloc(sizeof(struct JniLibrary));
    struct JniLibrary *head = lib;
    lib->next = 0;

    struct SharedLibrary *shlib = shlibs;
    while (shlib!=0) {
        const char *basename = strrchr(shlib->filename, '/');
        if (basename++ == NULL)
            basename = shlib->filename;
        for (ilib = 0; libblacklist[ilib] != NULL; ilib++)
            if (strcmp(basename, libblacklist[ilib]) == 0)
                break;
        if (libblacklist[ilib] != NULL) {
            shlib = shlib->next;
            continue;
        }
        add_sopath(shlib->dirname);
        lib->lib = android_dlopen(shlib->filename,RTLD_LAZY);
        if (!(lib->lib)) {
            printf("Missing library dependencies.\n");
            return 0;
        }
        lib->method_table = jni_shlib_find_methods(shlib->filename);
        lib->name = strdup(shlib->filename);
    // unlink(shlib->filename);
        shlib = shlib->next;
        if (shlib!=0) {
            lib->next = malloc(sizeof(struct JniLibrary));
            lib = lib->next;
            lib->next = 0;
        }
    }

    global.libraries = head;

    load_modules(".");
    load_modules(platform_getmoduledirectory());
    notify_gdb_of_libraries();

    if (global.support_modules == NULL) {
        printf("No support modules found.\n");
    }

    /* mmap apk for direct asset access (avoids tempfile extraction) */
    struct stat st;
    global.apk_fd = open(global.apk_filename, O_RDONLY);
    if (global.apk_fd == -1) {
        perror("open");
        exit(1);
    }
    if (fstat(global.apk_fd, &st)) {
        perror("fstat");
        exit(1);
    }
    global.apk_size = st.st_size;
    global.apk_in_mem = mmap(NULL, global.apk_size, PROT_READ,
        MAP_SHARED, global.apk_fd, 0);
    if (global.apk_in_mem == MAP_FAILED) {
        perror("warning: mmap'ing apk failed");
        global.apk_in_mem = NULL;
    }

    /* Search for a suitable module to handle the library */
    struct SupportModule *module = global.support_modules;
    while (module != NULL) {
        if (module->try_init(module)) {
            break;
        }
        module = module->next;
    }

    if (module == NULL) {
        printf("Not supported yet, but found JNI methods:\n");

        struct JniLibrary *lib = global.libraries;
        while (lib!=0) {
            tmp = lib->method_table;
            while (*tmp) {
                printf("    %s\n", *tmp);
                tmp++;
            }
            lib = lib->next;
        }
        goto finish;
    }

    /* figure out GLES version to use */
    int gles_version = 1;

    if (global.loader_seen_glesv1 && global.loader_seen_glesv2) {
        printf("Warning: app uses both GLESv1 and GLESv2 libs\n");
        /* TODO: perhaps consult the module what to use here? */
    } else if (global.loader_seen_glesv2) {
        gles_version = 2;
    }
#if !defined(APKENV_GLES)
    gles_version = 2;
#endif
    printf("Using GLES version %d\n", gles_version);

    if (!system_init(gles_version)) {
        return 0;
    }

    global.active_module = module;
    printf("Using module: %s\n", module->filename);
    install_overrides(module);

    char data_directory[PATH_MAX];
    strcpy(data_directory, main_data_dir);
    strcat(data_directory, apk_basename(global.apk_filename));
    strcat(data_directory, "/");
    recursive_mkdir(data_directory);

    apk_read_resources(global.apklib_handle, &global.resource_strings);

    module->init(module, platform_getscreenwidth(), platform_getscreenheight(), data_directory);

    notify_gdb_of_libraries();

    if(global.module_hacks->handle_update) goto finish;

    while (1) {

        if (module->requests_exit(module)) {
            break;
        }

        if (input_update(module)) {
            break;
        }

        module->update(module);
        system_update();
    }

finish:
    module->deinit(module);

    lib = global.libraries;
    while (lib!=0) {
        tmp = lib->method_table;
        while (*tmp) {
            free(*tmp++);
        }
        free(lib->method_table);
        struct JniLibrary *next = lib->next;
        free(lib);
        lib = next;
    }

    if (global.apk_in_mem)
        munmap((void *)global.apk_in_mem, global.apk_size);
    close(global.apk_fd);
    apk_close(global.apklib_handle);
    system_exit();

    return 0;
}

