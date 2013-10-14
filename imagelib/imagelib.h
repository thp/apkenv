#ifndef APKENV_IMAGELIB_H
#define APKENV_IMAGELIB_H

/// image lib, (c) crow_riot 2013

#include <stddef.h>

typedef struct {
    unsigned char *data;
    int width;
    int height;
    int bpp;
} image_t;

typedef struct {
    /// flip horizontally
    int swapy;
    /// convert image data to rgba
    int forcergba;
    /// swap red and blue channels (bgr<>rgb)
    int swaprb;
    /// the alpha value to be used when image conversion takes place
    /// and the source file does not provide an alpha
    unsigned char alpha;
} imageloadersettings_t;

typedef image_t *(*image_loader_t)(char *buf, size_t size, const imageloadersettings_t settings);

#endif /* APKENV_IMAGELIB_H */
