/// jpeg loader, (C) crow_riot 2013



#include "imagelib.h"
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <jpeglib.h>



image_t *loadjpeg(const char *filename, const imageloadersettings_t settings)
{
    FILE *fp = fopen(filename,"rb");
    if (!fp)
        return NULL;

    struct jpeg_decompress_struct cinfo;
    struct jpeg_error_mgr jerr;
    JSAMPLE *buffer;
    int x;

    cinfo.err = jpeg_std_error(&jerr);

    jpeg_create_decompress(&cinfo);

    jpeg_stdio_src(&cinfo,fp);

    if (jpeg_read_header(&cinfo,1)!=JPEG_HEADER_OK) {
        fclose(fp);
        return NULL;
    }
    if(!jpeg_start_decompress(&cinfo)) {
        fclose(fp);
        return NULL;
    }
    if (cinfo.output_components!=3) {
        jpeg_finish_decompress(&cinfo);
        jpeg_destroy_decompress(&cinfo);
        fclose(fp);
        return NULL;
    }
    int stride = cinfo.output_width * cinfo.output_components;

    image_t *image = malloc(sizeof(image_t));

    image->width = cinfo.output_width;
    image->height = cinfo.output_height;

    if (settings.forcergba) {
        image->bpp = 4;
    } else {
        image->bpp = cinfo.output_components;
    }

    buffer = malloc(stride * sizeof(JSAMPLE));
    image->data = malloc(sizeof(unsigned char)*image->width*image->height*image->bpp);
    int ro = settings.swaprb ? 2 : 0;
    int bo = settings.swaprb ? 0 : 2;

    while (cinfo.output_scanline<cinfo.output_height){
        jpeg_read_scanlines(&cinfo,&buffer,1);

        unsigned char* trg = image->data + image->width*image->bpp*cinfo.output_scanline;
        if (settings.forcergba) {
            for (x=0; x<cinfo.output_width;x++) {
                int tx = x<<2;
                int sx = x*cinfo.output_components;

                trg[tx+0] = buffer[sx+ro];
                trg[tx+1] = buffer[sx+1];
                trg[tx+2] = buffer[sx+bo];
                trg[tx+3] = settings.alpha;
            }
        }
        else {
            for (x=0; x<cinfo.output_width;x++) {
                int tx = x<<2;
                int sx = x*cinfo.output_components;

                trg[tx+0] = buffer[sx+ro];
                trg[tx+1] = buffer[sx+1];
                trg[tx+2] = buffer[sx+bo];
            }
        }
    }


    jpeg_finish_decompress(&cinfo);

    jpeg_destroy_decompress(&cinfo);

    return image;
}
