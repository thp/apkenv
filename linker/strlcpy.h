#ifndef STRLCPY_H
#define STRLCPY_H

#include <sys/types.h>
#include <string.h>

size_t
apkenv_strlcpy(char *dst, const char *src, size_t siz);

#endif
