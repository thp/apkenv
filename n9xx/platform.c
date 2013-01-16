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


#include "../platform.h"
#include <SDL/SDL.h>
#ifdef FREMANTLE
#    include <SDL/SDL_gles.h>
#endif

#include <SDL/SDL_syswm.h>
#include <X11/Xlib.h>
#include <X11/Xatom.h>

#define MEEGOTOUCH_BORDER 16

static SDL_Surface* screen = 0;


const char* platform_getdatadirectory()
{
    return "/home/user/.apkenv/";
}

const char* platform_getmoduledirectory()
{
    return "/opt/apkenv/modules/";
}

const char* platform_getinstalldirectory()
{
    return  "/home/user/.local/share/applications/";
}


int platform_init()
{
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

    if ( screen==NULL ) {
        return 0;
    }

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
    return 1;
}

int platform_getscreenheight()
{
    return screen->w;
}

int platform_getscreenwidth()
{
    return screen->h;
}

int platform_update()
{
#ifdef FREMANTLE
    SDL_GLES_SwapBuffers();
#else
    SDL_GL_SwapBuffers();
#endif
return 1;
}


int platform_exit()
{
    return 1;
}


