
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

#include <SDL/SDL.h>
#ifdef FREMANTLE
#    include <SDL/SDL_gles.h>
#endif

#include <SDL/SDL_syswm.h>
#include <X11/Xlib.h>
#include <X11/Xatom.h>

#include "jni/jnienv.h"
#include "jni/shlib.h"
#include "apklib/apklib.h"

#include "apkenv.h"

/* Global application state */
struct GlobalState global;

void *
lookup_symbol_impl(const char *method)
{
    return jni_shlib_resolve(&global, method);
}

void
foreach_file_impl(const char *prefix, apk_for_each_file_callback callback)
{
    apk_for_each_file(global.apklib_handle, prefix, callback);
}

int
read_file_impl(const char *filename, char **buffer, size_t *size)
{
    return (apk_read_file(global.apklib_handle,
                filename, buffer, size) == APK_OK);
}

void register_module(struct SupportModule *module)
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

void load_module(const char *filename)
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

void load_modules(const char *dirname)
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

void install_overrides(struct SupportModule *module)
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

void
usage()
{
    printf("Usage: %s [--install] <file.apk>\n",
            global.apkenv_executable);
    exit(1);
}

char *
apk_basename(const char *filename)
{
    char *fn = strrchr(filename, '/');
    if (fn != NULL) {
        return fn + 1;
    }
    return filename;
}

void
recursive_mkdir(const char *directory)
{
    char *tmp = strdup(directory);
    struct stat st;
    int len = strlen(directory);
    int i;

    /* Dirty? Works for me... */
    for (i=1; i<len; i++) {
        if (tmp[i] == '/') {
            tmp[i] = '\0';
            if (stat(tmp, &st) != 0) {
                mkdir(tmp, 0700);
            }
            tmp[i] = '/';
        }
    }

    free(tmp);
}

#define LOCAL_SHARE_APPLICATIONS "/home/user/.local/share/applications/"

void
operation(const char *operation, const char *filename)
{
    if (strcmp(operation, "--install") == 0) {
        char apkenv_absolute[PATH_MAX];
        char apk_absolute[PATH_MAX];

        readlink("/proc/self/exe", apkenv_absolute, sizeof(apkenv_absolute));
        /* On Linux, realpath(3) always returns an absolute path */
        assert(realpath(filename, apk_absolute) != NULL);

        char *app_name = apk_basename(filename);

        char icon_filename[PATH_MAX];
        sprintf(icon_filename, "%s%s.png", DATA_DIRECTORY_BASE, app_name);

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
        recursive_mkdir(LOCAL_SHARE_APPLICATIONS);
        sprintf(desktop_filename, "%s/%s.desktop", LOCAL_SHARE_APPLICATIONS,
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

#define MEEGOTOUCH_BORDER 16

int main(int argc, char **argv)
{
    char **tmp;

    recursive_mkdir(DATA_DIRECTORY_BASE);
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

    global.lookup_symbol = lookup_symbol_impl;
    global.foreach_file = foreach_file_impl;
    global.read_file = read_file_impl;
    jnienv_init(&global);
    javavm_init(&global);
    global.apk_filename = strdup(argv[argc-1]);
    global.apklib_handle = apk_open(global.apk_filename);
    global.support_modules = NULL;

    const char *shlib = apk_get_shared_library(global.apklib_handle);
    if (shlib == NULL) {
        printf("Not a native APK.\n");
        return 0;
    }

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_JOYSTICK) < 0) {
        printf("SDL Init failed.\n");
        return 0;
    }

    SDL_Surface *screen;

    /**
     * The following block looks scary, but it just creates an SDL surface
     * with the right OpenGL ES context version. The block looks so scary,
     * because on Fremantle, javispedro's SDL_gles is used and on Harmattan
     * (where the GLES features are integrated directly into libSDL), only
     * SDL is used.
     **/
#ifdef FREMANTLE
#ifdef APKENV_GLES2
    SDL_GLES_Init(SDL_GLES_VERSION_2_0);
#else /* APKENV_GLES2 */
    SDL_GLES_Init(SDL_GLES_VERSION_1_1);
#endif /* APKENV_GLES2 */
    screen = SDL_SetVideoMode(0, 0, 0, SDL_FULLSCREEN);
    SDL_GLES_MakeCurrent(SDL_GLES_CreateContext());
#else /* FREMANTLE */
#ifdef APKENV_GLES2
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
#else /* APKENV_GLES2 */
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 1);
#endif /* APKENV_GLES2 */
    screen = SDL_SetVideoMode(0, 0, 0, SDL_OPENGLES | SDL_FULLSCREEN);
#endif /* FREMANTLE */

    SDL_ShowCursor(0);

#ifndef FREMANTLE
    /* Set up swipe lock (left and right) */
    SDL_SysWMinfo wm;
    SDL_VERSION(&wm.version);
    SDL_GetWMInfo(&wm);
    Display *dpy = wm.info.x11.display;
    Atom atom = XInternAtom(dpy, "_MEEGOTOUCH_CUSTOM_REGION", False);
    unsigned int region[] = {
        0,
        MEEGOTOUCH_BORDER,
        screen->w,
        screen->h - 2*MEEGOTOUCH_BORDER,
    };
    XChangeProperty(dpy, wm.info.x11.wmwindow, atom, XA_CARDINAL, 32,
            PropModeReplace, (unsigned char*)region, 4);
#endif

    global.method_table = jni_shlib_find_methods(shlib);
    global.jni_library = android_dlopen(shlib, RTLD_LAZY);
    unlink(shlib);
    if (!(global.jni_library)) {
        printf("Missing library dependencies.\n");
        return 0;
    }

    load_modules(".");
    load_modules("/opt/apkenv/modules");
    if (global.support_modules == NULL) {
        printf("No support modules found.\n");
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
        tmp = global.method_table;
        while (*tmp) {
            printf("    %s\n", *tmp);
            tmp++;
        }
        goto finish;
    }

    global.active_module = module;
    printf("Using module: %s\n", module->filename);
    install_overrides(module);

    char data_directory[PATH_MAX];
    strcpy(data_directory, DATA_DIRECTORY_BASE);
    strcat(data_directory, apk_basename(global.apk_filename));
    strcat(data_directory, "/");
    recursive_mkdir(data_directory);

    module->init(module, screen->w, screen->h, data_directory);

    while (1) {
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_MOUSEBUTTONDOWN) {
                module->input(module, ACTION_DOWN, e.button.x, e.button.y, e.button.which);
            } else if (e.type == SDL_MOUSEBUTTONUP) {
                module->input(module, ACTION_UP, e.button.x, e.button.y, e.button.which);
            } else if (e.type == SDL_MOUSEMOTION) {
                module->input(module, ACTION_MOVE, e.motion.x, e.motion.y, e.motion.which);
            } else if (e.type == SDL_QUIT) {
                module->deinit(module);
                goto finish;
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
                            goto finish;
                        }
                    }
                    module->resume(module);
                }
            }
        }
        module->update(module);
#ifdef FREMANTLE
        SDL_GLES_SwapBuffers();
#else
        SDL_GL_SwapBuffers();
#endif
    }

finish:
    tmp = global.method_table;
    while (*tmp) {
        free(*tmp++);
    }
    free(global.method_table);
    apk_close(global.apklib_handle);

    return 0;
}

