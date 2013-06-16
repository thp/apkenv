#include "egl_wrappers.h"
#include "hooks.h"

#ifdef APKENV_DEBUG
#  define WRAPPERS_DEBUG_PRINTF(...) printf(__VA_ARGS__)
#else
#  define WRAPPERS_DEBUG_PRINTF(...)
#endif

void *
my_eglGetProcAddress(const char *procname)
{
    WRAPPERS_DEBUG_PRINTF("eglGetProcAddress(%s)\n", procname);
    void *sym = get_hooked_symbol(procname);
    if (sym == NULL)
        printf("eglGetProcAddress: unimplemented: %s\n", procname);
    return sym;
}

EGLDisplay
my_eglGetDisplay(EGLNativeDisplayType display_id)
{
    WRAPPERS_DEBUG_PRINTF("eglGetDisplay(%x)\n", (int)display_id);
    return (void *)0xc00fa15e;
}

#if 0
EGLDisplay
my_eglGetDisplay(EGLNativeDisplayType display_id)
{
    WRAPPERS_DEBUG_PRINTF("eglGetDisplay\n");
    return eglGetDisplay(display_id);
}
#endif

#ifdef APKENV_EGL

EGLint
my_eglGetError(void)
{
    WRAPPERS_DEBUG_PRINTF("eglGetError\n");
    return eglGetError();
}

EGLBoolean
my_eglInitialize(EGLDisplay dpy, EGLint *major, EGLint *minor)
{
    WRAPPERS_DEBUG_PRINTF("eglInitialize\n");
    return eglInitialize(dpy,major,minor);
}

EGLBoolean
my_eglTerminate(EGLDisplay dpy)
{
    WRAPPERS_DEBUG_PRINTF("eglTerminate\n");
    return eglTerminate(dpy);
}

const char *
my_eglQueryString(EGLDisplay dpy, EGLint name)
{
    WRAPPERS_DEBUG_PRINTF("eglQueryString\n");
    return eglQueryString(dpy,name);
}

EGLBoolean
my_eglGetConfigs(EGLDisplay dpy, EGLConfig *configs,
			 EGLint config_size, EGLint *num_config)
{
    WRAPPERS_DEBUG_PRINTF("eglGetConfigs\n");
    return eglGetConfigs(dpy,configs,config_size,num_config);
}

EGLBoolean
my_eglChooseConfig(EGLDisplay dpy, const EGLint *attrib_list,
			   EGLConfig *configs, EGLint config_size,
			   EGLint *num_config)
{
    WRAPPERS_DEBUG_PRINTF("eglChooseConfig\n");
    return eglChooseConfig(dpy,attrib_list,configs,config_size,num_config);
}

EGLBoolean
my_eglGetConfigAttrib(EGLDisplay dpy, EGLConfig config,
			      EGLint attribute, EGLint *value)
{
    WRAPPERS_DEBUG_PRINTF("eglGetConfigAttrib\n");
    return eglGetConfigAttrib(dpy,config,attribute,value);
}

EGLSurface
my_eglCreateWindowSurface(EGLDisplay dpy, EGLConfig config,
				  EGLNativeWindowType win,
				  const EGLint *attrib_list)
{
    WRAPPERS_DEBUG_PRINTF("eglCreateWindowSurface\n");
    return eglCreateWindowSurface(dpy,config,win,attrib_list);
}

EGLSurface
my_eglCreatePbufferSurface(EGLDisplay dpy, EGLConfig config,
				   const EGLint *attrib_list)
{
    WRAPPERS_DEBUG_PRINTF("eglCreatePbufferSurface\n");
    return eglCreatePbufferSurface(dpy,config,attrib_list);
}

EGLSurface
my_eglCreatePixmapSurface(EGLDisplay dpy, EGLConfig config,
				  EGLNativePixmapType pixmap,
				  const EGLint *attrib_list)
{
    WRAPPERS_DEBUG_PRINTF("eglCreatePixmapSurface\n");
    return eglCreatePixmapSurface(dpy,config,pixmap,attrib_list);
}

EGLBoolean
my_eglDestroySurface(EGLDisplay dpy, EGLSurface surface)
{
    WRAPPERS_DEBUG_PRINTF("eglDestroySurface\n");
    return eglDestroySurface(dpy,surface);
}
EGLBoolean
my_eglQuerySurface(EGLDisplay dpy, EGLSurface surface,
			   EGLint attribute, EGLint *value)
{
    WRAPPERS_DEBUG_PRINTF("eglQuerySurface\n");
    return eglQuerySurface(dpy,surface,attribute,value);
}

EGLBoolean
my_eglBindAPI(EGLenum api)
{
    WRAPPERS_DEBUG_PRINTF("eglBindAPI\n");
    return eglBindAPI(api);
}

EGLenum
my_eglQueryAPI(void)
{
    WRAPPERS_DEBUG_PRINTF("eglQueryAPI\n");
    return eglQueryAPI();
}

EGLBoolean
my_eglWaitClient(void)
{
    WRAPPERS_DEBUG_PRINTF("eglWaitClient\n");
    return eglWaitClient();
}

EGLBoolean
my_eglReleaseThread(void)
{
    WRAPPERS_DEBUG_PRINTF("eglReleaseThread\n");
    return eglReleaseThread();
}

EGLSurface
my_eglCreatePbufferFromClientBuffer(
	      EGLDisplay dpy, EGLenum buftype, EGLClientBuffer buffer,
	      EGLConfig config, const EGLint *attrib_list)
{
    WRAPPERS_DEBUG_PRINTF("eglCreatePbufferFromClientBuffer\n");
    return eglCreatePbufferFromClientBuffer(dpy,buftype,buffer,config,attrib_list);
}

EGLBoolean
my_eglSurfaceAttrib(EGLDisplay dpy, EGLSurface surface,
			    EGLint attribute, EGLint value)
{
    WRAPPERS_DEBUG_PRINTF("eglSurfaceAttrib\n");
    return eglSurfaceAttrib(dpy,surface,attribute,value);
}

EGLBoolean
my_eglBindTexImage(EGLDisplay dpy, EGLSurface surface, EGLint buffer)
{
    WRAPPERS_DEBUG_PRINTF("eglBindTexImage\n");
    return eglBindTexImage(dpy,surface,buffer);
}

EGLBoolean
my_eglReleaseTexImage(EGLDisplay dpy, EGLSurface surface, EGLint buffer)
{
    WRAPPERS_DEBUG_PRINTF("eglReleaseTexImage\n");
    return eglReleaseTexImage(dpy,surface,buffer);
}


EGLBoolean
my_eglSwapInterval(EGLDisplay dpy, EGLint interval)
{
    WRAPPERS_DEBUG_PRINTF("eglSwapInterval\n");
    return eglSwapInterval(dpy,interval);
}


EGLContext
my_eglCreateContext(EGLDisplay dpy, EGLConfig config,
			    EGLContext share_context,
			    const EGLint *attrib_list)
{
    WRAPPERS_DEBUG_PRINTF("eglCreateContext\n");
    return eglCreateContext(dpy,config,share_context,attrib_list);
}

EGLBoolean
my_eglDestroyContext(EGLDisplay dpy, EGLContext ctx)
{
    WRAPPERS_DEBUG_PRINTF("eglDestroyContext\n");
    return eglDestroyContext(dpy,ctx);
}

EGLBoolean
my_eglMakeCurrent(EGLDisplay dpy, EGLSurface draw,
			  EGLSurface read, EGLContext ctx)
{
    WRAPPERS_DEBUG_PRINTF("eglMakeCurrent\n");
    EGLBoolean b = eglMakeCurrent(dpy,draw,read,ctx);
    WRAPPERS_DEBUG_PRINTF("eglMakeCurrent done\n");
    return b;
}

EGLContext
my_eglGetCurrentContext(void)
{
    WRAPPERS_DEBUG_PRINTF("eglGetCurrentContext\n");
    return eglGetCurrentContext();
}

EGLSurface
my_eglGetCurrentSurface(EGLint readdraw)
{
    WRAPPERS_DEBUG_PRINTF("eglGetCurrentSurface\n");
    return eglGetCurrentSurface(readdraw);
}

EGLDisplay
my_eglGetCurrentDisplay(void)
{
    WRAPPERS_DEBUG_PRINTF("eglGetCurrentDisplay\n");
    return eglGetCurrentDisplay();
}

EGLBoolean
my_eglQueryContext(EGLDisplay dpy, EGLContext ctx,
			   EGLint attribute, EGLint *value)
{
    WRAPPERS_DEBUG_PRINTF("eglQueryContext\n");
    return eglQueryContext(dpy,ctx,attribute,value);
}

EGLBoolean
my_eglWaitGL(void)
{
    WRAPPERS_DEBUG_PRINTF("eglWaitGL\n");
    return eglWaitGL();
}

EGLBoolean
my_eglWaitNative(EGLint engine)
{
    WRAPPERS_DEBUG_PRINTF("eglWaitNative\n");
    return eglWaitNative(engine);
}

EGLBoolean
my_eglSwapBuffers(EGLDisplay dpy, EGLSurface surface)
{
    WRAPPERS_DEBUG_PRINTF("eglSwapBuffers\n");
    return eglSwapBuffers(dpy,surface);
}

EGLBoolean
my_eglCopyBuffers(EGLDisplay dpy, EGLSurface surface,
			  EGLNativePixmapType target)
{
    WRAPPERS_DEBUG_PRINTF("eglCopyBuffers\n");
    return eglCopyBuffers(dpy,surface,target);
}

#endif

