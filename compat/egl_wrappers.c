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
    void *sym = apkenv_get_hooked_symbol(procname, 1);
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

extern struct GlobalState global;

EGLBoolean
my_eglSwapBuffers(EGLDisplay dpy, EGLSurface surface)
{
    fprintf(stderr, "STUB eglSwapBuffers(dpy=%p, surface=%p\n", dpy, surface);

    global.platform->update();

    return EGL_FALSE;
}

EGLBoolean
my_eglInitialize(EGLDisplay dpy, EGLint *major, EGLint *minor)
{
    fprintf(stderr, "STUB eglInitialize(dpy=%p, major=%p, minor=%p)\n", dpy, major, minor);

    return EGL_TRUE;
}

EGLBoolean
my_eglChooseConfig(EGLDisplay dpy, const EGLint *attrib_list, EGLConfig *configs, EGLint config_size, EGLint *num_config)
{
    fprintf(stderr, "STUB eglChooseConfig(dpy=%p, attrib_list=%p, configs=%p, config_size=%d, num_config=%p)\n",
            dpy, attrib_list, configs, config_size, num_config);

    *num_config = 1;

    return EGL_TRUE;
}

EGLBoolean
my_eglGetConfigAttrib(EGLDisplay dpy, EGLConfig config, EGLint attribute, EGLint *value)
{
    fprintf(stderr, "STUB eglGetConfigAttrib(dpy=%p, config=%p, attribute=%x, value=%p)\n",
            dpy, config, attribute, value);

    switch (attribute) {
        case EGL_RED_SIZE:
        case EGL_GREEN_SIZE:
        case EGL_BLUE_SIZE:
            *value = 8;
            return EGL_TRUE;
        case EGL_DEPTH_SIZE:
            *value = 16;
            return EGL_TRUE;
        case 0x302e /* EGL_NATIVE_VISUAL_ID */:
            *value = 0xF00D;
            return EGL_TRUE;
        default:
            break;
    }

    fprintf(stderr, "Unhandled EGL attribute: %x\n", attribute);
    return EGL_FALSE;
}

EGLSurface
my_eglCreateWindowSurface(EGLDisplay dpy, EGLConfig config, EGLNativeWindowType win, const EGLint *attrib_list)
{
    fprintf(stderr, "STUB eglCreateWindowSurface(dpy=%p, config=%p, win=%ld, attrib_list=%p)\n",
            dpy, config, win, attrib_list);

    return (EGLSurface)0xCAFEBABE;
}

EGLContext
my_eglCreateContext(EGLDisplay dpy, EGLConfig config, EGLContext share_context, const EGLint *attrib_list)
{
    fprintf(stderr, "STUB eglCreateContext(dpy=%p, config=%p, share_context=%p, attrib_list=%p)\n",
            dpy, config, share_context, attrib_list);

    return (EGLContext)0xF00DFACE;
}

EGLBoolean
my_eglMakeCurrent(EGLDisplay dpy, EGLSurface draw, EGLSurface read, EGLContext ctx)
{
    fprintf(stderr, "STUB eglMakeCurrent(dpy=%p, draw=%p, read=%p, ctx=%p)\n", dpy, draw, read, ctx);

    return EGL_TRUE;
}

EGLBoolean
my_eglQuerySurface(EGLDisplay dpy, EGLSurface surface, EGLint attribute, EGLint *value)
{
    fprintf(stderr, "STUB eglQuerySurface(dpy=%p, surface=%p, attribute=%x, value=%p\n", dpy, surface, attribute, value);

    int width, height;
    global.platform->get_size(&width, &height);

    switch (attribute) {
        case EGL_WIDTH:
            *value = width;
            return EGL_TRUE;
        case EGL_HEIGHT:
            *value = height;
            return EGL_TRUE;
        default:
            break;
    }

    fprintf(stderr, "Unhandled attribute: %x\n", attribute);
    return EGL_FALSE;
}

EGLBoolean
my_eglDestroyContext(EGLDisplay dpy, EGLContext ctx)
{
    fprintf(stderr, "STUB eglDestroyContext(dpy=%p, ctx=%p)\n", dpy, ctx);

    return EGL_TRUE;
}

EGLBoolean
my_eglDestroySurface(EGLDisplay dpy, EGLSurface surface)
{
    fprintf(stderr, "STUB eglDestroySurface(dpy=%p, surface=%p)\n", dpy, surface);

    return EGL_TRUE;
}

EGLBoolean
my_eglTerminate(EGLDisplay dpy)
{
    fprintf(stderr, "STUB eglTerminate(dpy=%p)\n", dpy);

    return EGL_TRUE;
}
