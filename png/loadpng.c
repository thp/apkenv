/// png loader, (c) crow_riot 2013

#include <stdio.h>
#include <libpng12/png.h>
#include "loadpng.h"


image_t* loadpng( const char *filepath, int swapy, int force_rgba, int swap_rb)
{
    FILE *fp = fopen(filepath,"rb");
    if (!fp)
        return NULL;

    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING,NULL,NULL,NULL);
    if ( !png_ptr ) {
        return NULL;
    }

    png_infop info = png_create_info_struct(png_ptr);
    if ( !info ) {
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        return NULL;
    }

    if ( setjmp(png_jmpbuf(png_ptr)) ) {
        png_destroy_read_struct(&png_ptr, &info, NULL);
        return NULL;
    }

    png_init_io(png_ptr,fp);
    png_set_sig_bytes(png_ptr,0);


    int png_transforms = PNG_TRANSFORM_STRIP_16|PNG_TRANSFORM_SWAP_ENDIAN|PNG_TRANSFORM_PACKING|PNG_TRANSFORM_EXPAND|(swap_rb?PNG_TRANSFORM_BGR:0);
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
        png_destroy_read_struct(&png_ptr, &info, NULL);
        return NULL;
    }

    if (bytepp==4)
        force_rgba = 0;


    image_t* image = malloc(sizeof(image_t));
    image->data = malloc(sizeof(unsigned char)*width*height*(force_rgba?4:bytepp));
    image->width = width;
    image->height = height;
    image->bpp = bytepp;

    int stride = width*(force_rgba?4:bytepp);


    png_bytepp row_pointers = png_get_rows(png_ptr, info);

    int x,y;

    if (!force_rgba) {
        if (swapy) {
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
                    trg[x*4+0] = src[x*bytepp+0];
                    trg[x*4+1] = src[x*bytepp+1];
                    trg[x*4+2] = src[x*bytepp+2];
                    trg[x*4+3] = 255;
                }
            }
        }
    }

    png_destroy_read_struct(&png_ptr, &info, NULL);

    return image;
}

