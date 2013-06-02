#include "egl_wrappers.h"
#include "hooks.h"

void *my_eglGetProcAddress(const char *procname)
{
    void *sym = get_hooked_symbol(procname);
    if (sym == NULL)
        printf("eglGetProcAddress: unimplemented: %s\n", procname);
    return sym;
}
