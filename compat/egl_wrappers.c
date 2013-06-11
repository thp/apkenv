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
