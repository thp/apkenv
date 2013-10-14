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
#include "../compat/android_keycodes.h"

#include <GLES/gl.h>
#include <EGL/egl.h>

#include <SDL/SDL_syswm.h>
#include <SDL/SDL_video.h>

#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/fb.h>
#include <sys/time.h>
#include <unistd.h>

#ifndef FBIO_WAITFORVSYNC
#define FBIO_WAITFORVSYNC _IOW('F', 0x20, __u32)
#endif

#ifndef NULL
#define NULL 0
#endif

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
    [SDLK_LALT]     = AKEYCODE_BUTTON_START,
    [SDLK_LCTRL]    = AKEYCODE_BUTTON_SELECT,
    [SDLK_PAGEUP]   = AKEYCODE_BUTTON_Y,
    [SDLK_PAGEDOWN] = AKEYCODE_BUTTON_X,
    [SDLK_HOME]     = AKEYCODE_BUTTON_A,
    [SDLK_END]      = AKEYCODE_BUTTON_B,
    [SDLK_RSHIFT]   = AKEYCODE_BUTTON_L1,
    [SDLK_RCTRL]    = AKEYCODE_BUTTON_R1,
    [SDLK_LAST]     = AKEYCODE_UNKNOWN,
};


#define FRAMEBUFFERDEVICE "/dev/fb0"

//
#define CHK_FREE_RET( chk, ptr, ret ) \
    if ( chk ) { \
        fprintf(stderr,"ERROR: %s at %s(%d)\n",#chk,__FILE__,__LINE__);\
        platform_exit(); \
        return ret; \
    }

int GLES_TestError(const char* msg)
{
    EGLint err = eglGetError();
    if (err!=EGL_SUCCESS) {
        fprintf(stderr,"EGL ERROR: 0x%x near %s\n",err,msg);
        return 1;
    }

    err = glGetError();
    if(err!=GL_NO_ERROR) {
        fprintf(stderr,"GL ERROR: 0x%x near %s\n",err,msg);
        return 1;
    }

    return 0;
}


typedef struct
{
    EGLDisplay eglDisplay;
    EGLConfig  eglConfig;
    EGLSurface eglSurface;
    EGLContext eglContext;
    int fbdev;
    SDL_Surface* screen;
} GLES_Data;

GLES_Data* G_Data = NULL;

/* -------- */


static const char *
pandora_get_path(enum PlatformPath which)
{
    switch (which) {
        case PLATFORM_PATH_INSTALL_DIRECTORY:
            return NULL; /* install not supported on pandora yet */
        case PLATFORM_PATH_DATA_DIRECTORY:
            return "./data/";
        case PLATFORM_PATH_MODULE_DIRECTORY:
            return "./modules";
        default:
            return NULL;
    }
}

static int
pandora_init(int gles_version)
{
    int i, j;

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_JOYSTICK) < 0) {
        return 0;
    }

    for (i = SDLK_0, j = AKEYCODE_0; i <= SDLK_9; i++, j++) {
        keymap[i] = j;
    }

    for (i = SDLK_a, j = AKEYCODE_A; i <= SDLK_z; i++, j++) {
        keymap[i] = j;
    }

    EGLint egl_config[] =
    {
        EGL_BUFFER_SIZE, 16,
        EGL_RED_SIZE, 5,
        EGL_GREEN_SIZE, 6,
        EGL_BLUE_SIZE, 5,
        EGL_ALPHA_SIZE, 0,
        EGL_DEPTH_SIZE, 24,
        EGL_STENCIL_SIZE, EGL_DONT_CARE,
        EGL_CONFIG_CAVEAT, EGL_NONE,
        EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
        EGL_RENDERABLE_TYPE, gles_version == 2 ? EGL_OPENGL_ES2_BIT : EGL_OPENGL_ES_BIT,
        EGL_NONE
    };


    GLES_Data* data = (GLES_Data*)malloc(sizeof(GLES_Data));
    memset(data,0,sizeof(GLES_Data));

    data->screen = SDL_SetVideoMode(0,0,0,SDL_FULLSCREEN);
    CHK_FREE_RET(data->screen==NULL,data,0);

    SDL_SysWMinfo  sysWmInfo;
    SDL_VERSION(&sysWmInfo.version);
    SDL_GetWMInfo(&sysWmInfo);

    data->eglDisplay = eglGetDisplay(0); // (EGLNativeDisplayType)sysWmInfo.info.x11.display);
    CHK_FREE_RET(data->eglDisplay==EGL_NO_DISPLAY,data,0);
    CHK_FREE_RET(GLES_TestError("eglGetDisplay"),data,0);

    EGLBoolean r = eglInitialize(data->eglDisplay,0,0);
    CHK_FREE_RET(!r,data,0);
    CHK_FREE_RET(GLES_TestError("eglInitialize"),data,0);

    int iConfigs;
    r = eglChooseConfig(data->eglDisplay, egl_config, &data->eglConfig, 1, &iConfigs);
    CHK_FREE_RET(!r||iConfigs!=1,data,0);
    CHK_FREE_RET(GLES_TestError("eglChooseConfig"),data,0);

    data->eglSurface = eglCreateWindowSurface(data->eglDisplay, data->eglConfig, NULL, NULL); //(NativeWindowType)sysWmInfo.info.x11.window, NULL);
    CHK_FREE_RET(data->eglSurface==EGL_NO_SURFACE,data,0);
    CHK_FREE_RET(GLES_TestError("eglCreateWindowSurface"),data,0);

	EGLint contextAttribs[] =
	{
        EGL_CONTEXT_CLIENT_VERSION, gles_version,
        EGL_NONE
	};

    data->eglContext = eglCreateContext(data->eglDisplay,data->eglConfig,NULL,contextAttribs);
    CHK_FREE_RET(data->eglContext==EGL_NO_CONTEXT,data,0);
    CHK_FREE_RET(GLES_TestError("eglCreateContext"),data,0);

    eglMakeCurrent(data->eglDisplay,data->eglSurface,data->eglSurface,data->eglContext);
    GLES_TestError("eglMakeCurrent");

    data->fbdev = open(FRAMEBUFFERDEVICE,O_RDONLY);

    G_Data = data;

    SDL_ShowCursor(0);
    return 1;
}

static void
pandora_get_size(int *width, int *height)
{
    if (width) {
        *width = G_Data->screen->w;
    }

    if (height) {
        *height = G_Data->screen->h;
    }
}

static unsigned int get_time_ms(void)
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec * 1000 + tv.tv_usec / 1000;
}

static int
pandora_input_update(struct SupportModule *module)
{
    static int emulate_multitouch = 0;
    const int emulate_finger_id = 2;
    int width = G_Data->screen->w;
    int height = G_Data->screen->h;

    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_KEYDOWN) {
            if (e.key.keysym.sym==SDLK_ESCAPE) {
                return 1;
            }
            else if (e.key.keysym.sym==SDLK_RSHIFT) {
                //emulate_multitouch = 1;
                module->input(module,ACTION_DOWN, width / 2 - 1, height / 2, emulate_finger_id);
            } else {
                module->key_input(module, ACTION_DOWN, keymap[e.key.keysym.sym], e.key.keysym.unicode);
            }
        }
        else if (e.type == SDL_KEYUP) {
            if (e.key.keysym.sym==SDLK_RSHIFT) {
                //emulate_multitouch = 0;
                module->input(module,ACTION_UP, width / 2 - 1, height / 2, emulate_finger_id);
            } else {
                module->key_input(module, ACTION_UP, keymap[e.key.keysym.sym], e.key.keysym.unicode);
            }
        } else if (e.type == SDL_MOUSEBUTTONDOWN) {
            module->input(module, ACTION_DOWN, e.button.x, e.button.y, e.button.which);
            if (emulate_multitouch) {
                module->input(module,ACTION_DOWN, width-e.button.x, height-e.button.y,emulate_finger_id);
            }
        } else if (e.type == SDL_MOUSEBUTTONUP) {
            module->input(module, ACTION_UP, e.button.x, e.button.y, e.button.which);
            if (emulate_multitouch) {
                module->input(module,ACTION_UP, width-e.button.x, height-e.button.y,emulate_finger_id);
            }
        } else if (e.type == SDL_MOUSEMOTION) {
            module->input(module, ACTION_MOVE, e.motion.x, e.motion.y, e.motion.which);
            if (emulate_multitouch) {
                module->input(module,ACTION_MOVE, width-e.button.x, height-e.button.y,emulate_finger_id);
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

static void
pandora_update()
{
    static unsigned int last_time;
    unsigned int now;
    GLES_Data* data = G_Data;

    if (data->eglDisplay!=EGL_NO_DISPLAY && data->eglSurface!=EGL_NO_SURFACE)
    {
        eglSwapBuffers(data->eglDisplay, data->eglSurface);
        now = get_time_ms();

        if (data->fbdev >= 0 && now - last_time < 16)
        {
            int arg = 0;
            ioctl( data->fbdev, FBIO_WAITFORVSYNC, &arg );
        }

        last_time = get_time_ms();

       // GLES_TestError("eglSwapBuffers");
#ifdef APKENV_DEBUG
        // printf("%d swap\n",pthread_self());
#endif
    }
}

static void
pandora_exit()
{
    GLES_Data* data = G_Data;
    if (data) {
        if (data->eglDisplay!=EGL_NO_DISPLAY) eglMakeCurrent(data->eglDisplay, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT );
        if (data->eglContext!=EGL_NO_CONTEXT) eglDestroyContext ( data->eglDisplay, data->eglContext );
        if (data->eglSurface!=EGL_NO_SURFACE) eglDestroySurface ( data->eglDisplay, data->eglSurface );
        if (data->eglDisplay!=EGL_NO_DISPLAY) eglTerminate ( data->eglDisplay );
        if (data->fbdev>=0) close(data->fbdev);
        free(data);
    }
    G_Data = NULL;
}

struct PlatformSupport platform_support = {
    pandora_init,
    pandora_get_path,
    pandora_get_size,
    pandora_input_update,
    pandora_update,
    pandora_exit,
};
