/**
 * apkenv
 * Copyright (c) 2012, 2014, Thomas Perl <m@thp.io>
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

#include <SDL/SDL.h>

#include <SDL/SDL_syswm.h>
#include <X11/Xlib.h>
#include <X11/Xatom.h>

#include "common/sdl_accelerometer_impl.h"

struct PlatformPriv {
    SDL_Surface *screen;
};

static struct PlatformPriv priv;


static int
harmattan_init(int gles_version)
{
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_JOYSTICK) < 0) {
        return 0;
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, gles_version);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
    priv.screen = SDL_SetVideoMode(0, 0, 0, SDL_OPENGLES | SDL_FULLSCREEN);

    if (priv.screen == NULL) {
        return 0;
    }

    /* Set up swipe lock (left and right) */
    SDL_SysWMinfo wm;
    SDL_VERSION(&wm.version);
    SDL_GetWMInfo(&wm);
    Display *dpy = wm.info.x11.display;
    Atom atom = XInternAtom(dpy, "_MEEGOTOUCH_CUSTOM_REGION", False);
    const int MEEGOTOUCH_BORDER = 16;
    unsigned int region[] = {
        0,
        MEEGOTOUCH_BORDER,
        priv.screen->w,
        priv.screen->h - 2*MEEGOTOUCH_BORDER,
    };
    XChangeProperty(dpy, wm.info.x11.wmwindow, atom, XA_CARDINAL, 32,
            PropModeReplace, (unsigned char*)region, 4);

    SDL_ShowCursor(0);

    apkenv_accelerometer_register(sdl_accelerometer);

    return 1;
}

static const char *
harmattan_get_path(enum PlatformPath which)
{
    switch (which) {
        case PLATFORM_PATH_INSTALL_DIRECTORY:
            return  "/home/user/.local/share/applications/";
        case PLATFORM_PATH_DATA_DIRECTORY:
            return "/home/user/.apkenv/";
        case PLATFORM_PATH_MODULE_DIRECTORY:
            return "/opt/apkenv/modules/";
        default:
            return NULL;
    }
}

static void
harmattan_get_size(int *width, int *height)
{
    if (width) {
        *width = priv.screen->w;
    }

    if (height) {
        *height = priv.screen->h;
    }
}

static int
harmattan_input_update(struct SupportModule *module)
{
    int width = priv.screen->w;
    int height = priv.screen->h;

    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_MOUSEBUTTONDOWN) {
            module->input(module, ACTION_DOWN, e.button.x, e.button.y, e.button.which);
        } else if (e.type == SDL_MOUSEBUTTONUP) {
            module->input(module, ACTION_UP, e.button.x, e.button.y, e.button.which);
        } else if (e.type == SDL_MOUSEMOTION) {
            module->input(module, ACTION_MOVE, e.motion.x, e.motion.y, e.motion.which);
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

static void
harmattan_update()
{
    SDL_GL_SwapBuffers();
}

static void
harmattan_exit()
{
}

struct PlatformSupport platform_support = {
    harmattan_init,
    harmattan_get_path,
    harmattan_get_size,
    harmattan_input_update,
    harmattan_update,
    harmattan_exit,
};
