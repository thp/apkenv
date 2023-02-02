#include <EGL/egl.h>
#include "../apkenv.h"

void *
my_eglGetProcAddress(const char *procname) SOFTFP;

EGLDisplay
my_eglGetDisplay(EGLNativeDisplayType display_id) SOFTFP;

EGLBoolean
my_eglSwapBuffers(EGLDisplay dpy, EGLSurface surface) SOFTFP;

EGLBoolean
my_eglInitialize(EGLDisplay dpy, EGLint *major, EGLint *minor) SOFTFP;

EGLBoolean
my_eglChooseConfig(EGLDisplay dpy, const EGLint *attrib_list, EGLConfig *configs, EGLint config_size, EGLint *num_config) SOFTFP;

EGLBoolean
my_eglGetConfigAttrib(EGLDisplay dpy, EGLConfig config, EGLint attribute, EGLint *value) SOFTFP;

EGLSurface
my_eglCreateWindowSurface(EGLDisplay dpy, EGLConfig config, EGLNativeWindowType win, const EGLint *attrib_list) SOFTFP;

EGLContext
my_eglCreateContext(EGLDisplay dpy, EGLConfig config, EGLContext share_context, const EGLint *attrib_list) SOFTFP;

EGLBoolean
my_eglMakeCurrent(EGLDisplay dpy, EGLSurface draw, EGLSurface read, EGLContext ctx) SOFTFP;

EGLBoolean
my_eglQuerySurface(EGLDisplay dpy, EGLSurface surface, EGLint attribute, EGLint *value) SOFTFP;

EGLBoolean
my_eglDestroyContext(EGLDisplay dpy, EGLContext ctx) SOFTFP;

EGLBoolean
my_eglDestroySurface(EGLDisplay dpy, EGLSurface surface) SOFTFP;

EGLBoolean
my_eglTerminate(EGLDisplay dpy) SOFTFP;
