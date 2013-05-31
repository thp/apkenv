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


#ifdef APKENV_GLES2
#include <GLES2/gl2.h>
#include <EGL/egl.h>
#else
#include <GLES/gl.h>
#include <EGL/egl.h>
#endif

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

const char* platform_getdatadirectory()
{
    return "./data/";
}


const char* platform_getmoduledirectory()
{
    return "./modules";
}

const char* platform_getinstalldirectory()
{
    return 0;
}


int platform_init()
{
    int version = 1;
#if APKENV_GLES2
    version = 2;
#endif

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
        EGL_RENDERABLE_TYPE, version==1 ? EGL_OPENGL_ES_BIT : EGL_OPENGL_ES2_BIT,
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
        EGL_CONTEXT_CLIENT_VERSION, version,
        EGL_NONE
	};

    data->eglContext = eglCreateContext(data->eglDisplay,data->eglConfig,NULL,contextAttribs);
    CHK_FREE_RET(data->eglContext==EGL_NO_CONTEXT,data,0);
    CHK_FREE_RET(GLES_TestError("eglCreateContext"),data,0);

    eglMakeCurrent(data->eglDisplay,data->eglSurface,data->eglSurface,data->eglContext);
    GLES_TestError("eglMakeCurrent");

    data->fbdev = open(FRAMEBUFFERDEVICE,O_RDONLY);

    G_Data = data;

    return 1;
}

int platform_getscreenwidth()
{
    return G_Data->screen->w;
}

int platform_getscreenheight()
{
    return G_Data->screen->h;
}

static unsigned int get_time_ms(void)
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec * 1000 + tv.tv_usec / 1000;
}

int platform_update()
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
        return 1;
    }

    return 0;
}


int platform_exit( )
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
    return 0;
}


