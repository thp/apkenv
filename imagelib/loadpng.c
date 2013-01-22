/// png loader, (c) crow_riot 2013

#include <stdio.h>
#include <libpng12/png.h>
#include <stdlib.h>
#include "imagelib.h"


image_t* loadpng( const char *filepath, const imageloadersettings_t settings)
{
    FILE *fp = fopen(filepath,"rb");
    if (!fp)
        return NULL;

    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING,NULL,NULL,NULL);
    if ( !png_ptr ) {
        fclose(fp);
        return NULL;
    }

    png_infop info = png_create_info_struct(png_ptr);
    if ( !info ) {
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        fclose(fp);
        return NULL;
    }

    if ( setjmp(png_jmpbuf(png_ptr)) ) {
        png_destroy_read_struct(&png_ptr, &info, NULL);
        fclose(fp);
        return NULL;
    }

    png_init_io(png_ptr,fp);
    png_set_sig_bytes(png_ptr,0);


    int png_transforms = PNG_TRANSFORM_STRIP_16|PNG_TRANSFORM_SWAP_ENDIAN|PNG_TRANSFORM_PACKING|PNG_TRANSFORM_EXPAND|(settings.swaprb?PNG_TRANSFORM_BGR:0);
    png_read_png(png_ptr, info, png_transforms, NULL);


    int rowbytes = png_get_rowbytes(png_ptr, info);
    int width = png_get_image_width(png_ptr, info);
    int height = png_get_image_height(png_ptr, info);
    int bit_depth = png_get_bit_depth(png_ptr, info);
    int color_type = png_get_color_type(png_ptr, info);

    // convert bit_depth + color_type to image format

    int bytepp = 0;
    if ( bit_depth==8 )
    {
        switch( color_type )
        {
            case PNG_COLOR_TYPE_GRAY: bytepp = 1; break;
            case PNG_COLOR_TYPE_GRAY_ALPHA: bytepp = 2; break;
            case PNG_COLOR_TYPE_RGB: bytepp = 3; break;
            case PNG_COLOR_TYPE_RGBA: bytepp = 4; break;
        }
    }

    if ( bytepp==0 ) {
        fclose(fp);
        png_destroy_read_struct(&png_ptr, &info, NULL);
        return NULL;
    }

    int forcergba = settings.forcergba && color_type!=PNG_COLOR_TYPE_RGBA;


    image_t* image = malloc(sizeof(image_t));
    image->data = malloc(sizeof(unsigned char)*width*height*(forcergba?4:bytepp));
    image->width = width;
    image->height = height;
    image->bpp = bytepp;

    int stride = width*(forcergba?4:bytepp);


    png_bytepp row_pointers = png_get_rows(png_ptr, info);

    int x,y;

    if (!forcergba) {
        if (settings.swapy) {
            for ( y=0; y<image->height; y++ )
                memcpy(image->data+stride*(image->height-1-y), row_pointers[y], rowbytes);
        }
        else {
            for ( y=0; y<image->height; y++ )
                memcpy(image->data+stride*y, row_pointers[y], rowbytes);
        }
    } else {

        if (color_type==PNG_COLOR_TYPE_RGB) {
            for ( y=0;y<image->height;y++ ) {
                unsigned char* src = row_pointers[y];
                unsigned char* trg = image->data+stride*y;
                for ( x=0; x<image->width; x++ ) {
                    int tx = x*4;
                    int sx = x*bytepp;
                    trg[tx+0] = src[sx+0];
                    trg[tx+1] = src[sx+1];
                    trg[tx+2] = src[sx+2];
                    trg[tx+3] = settings.alpha;
                }
            }
        }
        else
        if (color_type==PNG_COLOR_TYPE_GRAY) {
            for ( y=0;y<image->height;y++ ) {
                unsigned char* src = row_pointers[y];
                unsigned char* trg = image->data+stride*y;
                for ( x=0; x<image->width; x++ ) {
                    int tx = x*4;
                    int sx = x*bytepp;
                    trg[tx+0] = src[sx];
                    trg[tx+1] = src[sx];
                    trg[tx+2] = src[sx];
                    trg[tx+3] = settings.alpha;
                }
            }
        }
        else
        if (color_type==PNG_COLOR_TYPE_GRAY_ALPHA) {
            for ( y=0;y<image->height;y++ ) {
                unsigned char* src = row_pointers[y];
                unsigned char* trg = image->data+stride*y;
                for ( x=0; x<image->width; x++ ) {
                    int tx = x*4;
                    int sx = x*bytepp;
                    trg[tx+0] = src[sx];
                    trg[tx+1] = src[sx];
                    trg[tx+2] = src[sx];
                    trg[tx+3] = src[sx+1];
                }
            }
        }
    }

    png_destroy_read_struct(&png_ptr, &info, NULL);
    fclose(fp);

    return image;
}

