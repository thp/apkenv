#ifndef IMAGELIB_H
#define IMAGELIB_h

/// image lib, (c) crow_riot 2013


typedef struct {
    unsigned char* data;
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


/// load png from disk
image_t* loadpng(const char *filename, const imageloadersettings_t settings);

/// load jpeg from disk
/// please not that only rgb jpeg images are suported by the loader, no color indexed ones!
image_t* loadjpeg(const char *filename, const imageloadersettings_t settings);


#endif
