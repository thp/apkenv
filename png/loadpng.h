#ifndef LOADPNG_H
#define LOADPNG_h

/// png loader, (c) crow_riot 2013


typedef struct {
    unsigned char* data;
    int width;
    int height;
    int bpp;
} image_t;



/// load png from disk
image_t* loadpng(const char* filename, int swapy, int force_rgba, int swap_rb);




#endif
