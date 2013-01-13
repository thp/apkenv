
#include "glesinit.h"

#ifdef APKENV_GLES2
#include <GLES2/gl2.h>
#include <EGL/egl.h>
#else
#include <GLES/gl.h>
#include <EGL/egl.h>
#endif

#include <SDL/SDL_syswm.h>

#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/fb.h>

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
        GLES_Exit(ptr); \
        return ret; \
    }

int GLES_TestError(const char* msg)
{
    EGLint err = eglGetError();
    if (err!=EGL_SUCCESS) {
        fprintf(stderr,"EGL ERROR: %x near %s\n",err,msg);
        return 1;
    }

    err = glGetError();
    if(err!=GL_NO_ERROR) {
        fprintf(stderr,"GL ERROR: %x near %s\n",err,msg);
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
} GLES_Data;

GLES_Data* G_Data = NULL;

void* GLES_Init( int version )
{
    if ( version!=1 && version!=2 )
        return 0;

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

    SDL_SysWMinfo  sysWmInfo;
    SDL_VERSION(&sysWmInfo.version);
    SDL_GetWMInfo(&sysWmInfo);

    data->eglDisplay = eglGetDisplay((EGLNativeDisplayType)sysWmInfo.info.x11.display);
    CHK_FREE_RET(data->eglDisplay==EGL_NO_DISPLAY,data,NULL);
    CHK_FREE_RET(GLES_TestError("eglGetDisplay"),data,NULL);

    EGLBoolean r = eglInitialize(data->eglDisplay,0,0);
    CHK_FREE_RET(!r,data,NULL);
    CHK_FREE_RET(GLES_TestError("eglInitialize"),data,NULL);

    int iConfigs;
    r = eglChooseConfig(data->eglDisplay, egl_config, &data->eglConfig, 1, &iConfigs);
    CHK_FREE_RET(!r||iConfigs!=1,data,NULL);
    CHK_FREE_RET(GLES_TestError("eglChooseConfig"),data,NULL);

    data->eglSurface = eglCreateWindowSurface(data->eglDisplay, data->eglConfig, (NativeWindowType)sysWmInfo.info.x11.window, NULL);
    CHK_FREE_RET(data->eglSurface==EGL_NO_SURFACE,data,NULL);
    CHK_FREE_RET(GLES_TestError("eglCreateWindowSurface"),data,NULL);

	EGLint contextAttribs[] =
	{
        EGL_CONTEXT_CLIENT_VERSION, version,
        EGL_NONE
	};

    data->eglContext = eglCreateContext(data->eglDisplay,data->eglConfig,NULL,contextAttribs);
    CHK_FREE_RET(data->eglContext==EGL_NO_CONTEXT,data,NULL);
    CHK_FREE_RET(GLES_TestError("eglCreateContext"),data,NULL);

    eglMakeCurrent(data->eglDisplay,data->eglSurface,data->eglSurface,data->eglContext);
    GLES_TestError("eglMakeCurrent");

    data->fbdev = open(FRAMEBUFFERDEVICE,O_RDONLY);

    G_Data = data;

    return data;
}

int GLES_SwapBuffers( void* rawptr )
{
    if (rawptr!=NULL)
    {
        GLES_Data* data = (GLES_Data*)rawptr;

        if (data->eglDisplay!=EGL_NO_DISPLAY && data->eglSurface!=EGL_NO_SURFACE)
        {
            if ( data->fbdev>=0 )
            {
                int arg = 0;
                ioctl( data->fbdev, FBIO_WAITFORVSYNC, &arg );
            }
            eglSwapBuffers(data->eglDisplay,data->eglSurface);

            GLES_TestError("eglSwapBuffers");
#ifdef APKENV_DEBUG
            // printf("%d swap\n",pthread_self());
#endif
            return 1;
        }
    }

    return 0;
}


int GLES_Exit( void* rawptr )
{
    if (rawptr!=0)
    {
        GLES_Data* data = (GLES_Data*)rawptr;

        if (data->eglDisplay!=EGL_NO_DISPLAY) eglMakeCurrent(data->eglDisplay, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT );
        if (data->eglContext!=EGL_NO_CONTEXT) eglDestroyContext ( data->eglDisplay, data->eglContext );
        if (data->eglSurface!=EGL_NO_SURFACE) eglDestroySurface ( data->eglDisplay, data->eglSurface );
        if (data->eglDisplay!=EGL_NO_DISPLAY) eglTerminate ( data->eglDisplay );
        if (data->fbdev>=0) close(data->fbdev);

        free(data);

        return 1;
    }
    return 0;
}

void GLES_SetCurrentContext()
{
    if (G_Data)
        eglMakeCurrent(G_Data->eglDisplay,G_Data->eglSurface,G_Data->eglSurface,G_Data->eglContext);

}
