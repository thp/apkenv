
#ifdef APKENV_EGL

#include <EGL/egl.h>
#include <stdio.h>

#ifdef APKENV_DEBUG
#  define WRAPPERS_DEBUG_PRINTF(...) fprintf(stderr,__VA_ARGS__)
#else
#  define WRAPPERS_DEBUG_PRINTF(...)
#endif

#include "../apkenv.h"
EGLint my_eglGetError(void) SOFTFP;

EGLDisplay my_eglGetDisplay(EGLNativeDisplayType display_id) SOFTFP;
EGLBoolean my_eglInitialize(EGLDisplay dpy, EGLint *major, EGLint *minor) SOFTFP;
EGLBoolean my_eglTerminate(EGLDisplay dpy) SOFTFP;

const char * my_eglQueryString(EGLDisplay dpy, EGLint name) SOFTFP;

EGLBoolean my_eglGetConfigs(EGLDisplay dpy, EGLConfig *configs,
			 EGLint config_size, EGLint *num_config) SOFTFP;
EGLBoolean my_eglChooseConfig(EGLDisplay dpy, const EGLint *attrib_list,
			   EGLConfig *configs, EGLint config_size,
			   EGLint *num_config) SOFTFP;
EGLBoolean my_eglGetConfigAttrib(EGLDisplay dpy, EGLConfig config,
			      EGLint attribute, EGLint *value) SOFTFP;

EGLSurface my_eglCreateWindowSurface(EGLDisplay dpy, EGLConfig config,
				  EGLNativeWindowType win,
				  const EGLint *attrib_list) SOFTFP;
EGLSurface my_eglCreatePbufferSurface(EGLDisplay dpy, EGLConfig config,
				   const EGLint *attrib_list) SOFTFP;
EGLSurface my_eglCreatePixmapSurface(EGLDisplay dpy, EGLConfig config,
				  EGLNativePixmapType pixmap,
				  const EGLint *attrib_list) SOFTFP;
EGLBoolean my_eglDestroySurface(EGLDisplay dpy, EGLSurface surface) SOFTFP;
EGLBoolean my_eglQuerySurface(EGLDisplay dpy, EGLSurface surface,
			   EGLint attribute, EGLint *value) SOFTFP;

EGLBoolean my_eglBindAPI(EGLenum api) SOFTFP;
EGLenum my_eglQueryAPI(void) SOFTFP;

EGLBoolean my_eglWaitClient(void) SOFTFP;

EGLBoolean my_eglReleaseThread(void) SOFTFP;

EGLSurface my_eglCreatePbufferFromClientBuffer(
	      EGLDisplay dpy, EGLenum buftype, EGLClientBuffer buffer,
	      EGLConfig config, const EGLint *attrib_list) SOFTFP;

EGLBoolean my_eglSurfaceAttrib(EGLDisplay dpy, EGLSurface surface,
			    EGLint attribute, EGLint value) SOFTFP;
EGLBoolean my_eglBindTexImage(EGLDisplay dpy, EGLSurface surface, EGLint buffer) SOFTFP;
EGLBoolean my_eglReleaseTexImage(EGLDisplay dpy, EGLSurface surface, EGLint buffer) SOFTFP;


EGLBoolean my_eglSwapInterval(EGLDisplay dpy, EGLint interval) SOFTFP;


EGLContext my_eglCreateContext(EGLDisplay dpy, EGLConfig config,
			    EGLContext share_context,
			    const EGLint *attrib_list) SOFTFP;
EGLBoolean my_eglDestroyContext(EGLDisplay dpy, EGLContext ctx) SOFTFP;
EGLBoolean my_eglMakeCurrent(EGLDisplay dpy, EGLSurface draw,
			  EGLSurface read, EGLContext ctx) SOFTFP;

EGLContext my_eglGetCurrentContext(void) SOFTFP;
EGLSurface my_eglGetCurrentSurface(EGLint readdraw) SOFTFP;
EGLDisplay my_eglGetCurrentDisplay(void) SOFTFP;
EGLBoolean my_eglQueryContext(EGLDisplay dpy, EGLContext ctx,
			   EGLint attribute, EGLint *value) SOFTFP;

EGLBoolean my_eglWaitGL(void) SOFTFP;
EGLBoolean my_eglWaitNative(EGLint engine) SOFTFP;
EGLBoolean my_eglSwapBuffers(EGLDisplay dpy, EGLSurface surface) SOFTFP;
EGLBoolean my_eglCopyBuffers(EGLDisplay dpy, EGLSurface surface,
			  EGLNativePixmapType target) SOFTFP;

#endif

