/// jpeg loader, (C) crow_riot 2013



#include "imagelib.h"
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <jpeglib.h>
#include <jerror.h>


/// stackoverflow <3
/// http://stackoverflow.com/questions/5280756/libjpeg-ver-6b-jpeg-stdio-src-vs-jpeg-mem-src

/* Read JPEG image from a memory segment */
static void init_source (j_decompress_ptr cinfo) {}
static boolean fill_input_buffer (j_decompress_ptr cinfo)
{
    ERREXIT(cinfo, JERR_INPUT_EMPTY);
    return TRUE;
}
static void skip_input_data (j_decompress_ptr cinfo, long num_bytes)
{
    struct jpeg_source_mgr* src = (struct jpeg_source_mgr*) cinfo->src;

    if (num_bytes > 0) {
        src->next_input_byte += (size_t) num_bytes;
        src->bytes_in_buffer -= (size_t) num_bytes;
    }
}
static void term_source (j_decompress_ptr cinfo) {}
static void jpeg_mem_src (j_decompress_ptr cinfo, void* buffer, long nbytes)
{
    struct jpeg_source_mgr* src;

    if (cinfo->src == NULL) {   /* first time for this JPEG object? */
        cinfo->src = (struct jpeg_source_mgr *)
            (*cinfo->mem->alloc_small) ((j_common_ptr) cinfo, JPOOL_PERMANENT,
            sizeof(struct jpeg_source_mgr));
    }

    src = (struct jpeg_source_mgr*) cinfo->src;
    src->init_source = init_source;
    src->fill_input_buffer = fill_input_buffer;
    src->skip_input_data = skip_input_data;
    src->resync_to_restart = jpeg_resync_to_restart; /* use default method */
    src->term_source = term_source;
    src->bytes_in_buffer = nbytes;
    src->next_input_byte = (JOCTET*)buffer;
}

image_t* loadjpeg_mem(char* buf, size_t size, const imageloadersettings_t settings)
{
    struct jpeg_decompress_struct cinfo;
    struct jpeg_error_mgr jerr;
    JSAMPLE *buffer;
    int x;

    cinfo.err = jpeg_std_error(&jerr);

    jpeg_create_decompress(&cinfo);

    //jpeg_stdio_src(&cinfo,fp);
    jpeg_mem_src(&cinfo,buf,size);

    if (jpeg_read_header(&cinfo,1)!=JPEG_HEADER_OK) {
        return NULL;
    }
    if(!jpeg_start_decompress(&cinfo)) {
        return NULL;
    }
    if (cinfo.output_components!=3) {
        jpeg_finish_decompress(&cinfo);
        jpeg_destroy_decompress(&cinfo);
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


image_t* loadjpeg_disk(const char *filename, const imageloadersettings_t settings)
{
    FILE *fp = fopen(filename,"rb");
    if (!fp)
        return NULL;

    fseek(fp,0,SEEK_END);
    size_t size = ftell(fp);
    fseek(fp,0,SEEK_SET);

    char* buf = malloc(sizeof(char)*size);
    fread(buf,size,1,fp);
    fclose(fp);

    return loadjpeg_mem(buf,size,settings);
}
