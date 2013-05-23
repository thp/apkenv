
#ifndef __WRAPPERS_H__
#define __WRAPPERS_H__

#ifdef DEBUG_TRACE_METHODS

#include <stdint.h>

#define WRAPPER_LATEHOOK            0
#define WRAPPER_UNHOOKED            1
#define WRAPPER_DYNHOOK             2
#define WRAPPER_ARM_INJECTION       3
#define WRAPPER_THUMB_INJECTION     4

//int have_wrapper(const char *name, uint8_t type);
void register_wrapper(uint32_t *ptr, uint32_t size, const char *name, uint8_t type);
void deinit_wrappers();

#endif /* DEBUG_TRACE_METHODS */

#endif /* include guard */

