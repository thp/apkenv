
#ifdef DEBUG_TRACE_METHODS

#include <stdlib.h>
#include <string.h>

#include <sys/mman.h>

#include "wrappers.h"

struct _wrapper {
    const char *name;
    uint32_t *ptr;
    uint32_t size;
    uint8_t type;
    struct _wrapper *next;
};

struct _wrapper *wrappers = NULL;

int wrapper_cmp(void *a, void *b) {
    struct _wrapper *key = (struct _wrapper*)a;
    struct _wrapper *elem = (struct _wrapper*)b;
    int c = strcmp(key->name, elem->name);
    if(0 == c)
    {
        return (key->type - elem->type);
    }
    return c;
}

/*
int have_wrapper(const char *name, const uint8_t type) {
    struct _wrapper *it = wrappers;
    while(NULL != it)
    {
        if(0 == strcmp(name, it->name))
        {
            if(type == it->type)
            {
                return 1;
            }
        }
        it = it->next;
    }
    return 0;
}
*/

void register_wrapper(uint32_t *ptr, uint32_t size, const char *name, uint8_t type) {
    struct _wrapper *it = wrappers;
    struct _wrapper *last = NULL;
    while(NULL != it)
    {
        last = it;
        it = it->next;
    }
    struct _wrapper *w = (struct _wrapper*)malloc(sizeof(struct _wrapper));
    w->name = name;
    w->ptr = ptr;
    w->type = type;
    w->size = size;
    w->next = NULL;
    if(NULL == last)
    {
        wrappers = w;
    }
    else
    {
        last->next = w;
    }
}

void deinit_wrappers() {
    struct _wrapper *it = wrappers;
    while(NULL != it)
    {
        struct _wrapper *next = it->next;
        munmap(it->ptr,it->size);
        free(it);
        it = next;
    }
}

#endif /* DEBUG_TRACE_METHODS */

