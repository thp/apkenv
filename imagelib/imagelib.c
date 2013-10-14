
/**
 * apkenv
 * Copyright (c) 2012, 2013, Thomas Perl <m@thp.io>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
 * IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **/


#include "imagelib.h"
#include "imagelib_priv.h"

#include <stdio.h>
#include <string.h>


image_t *
imagelib_load_from_disk(const char *filename, const imageloadersettings_t settings)
{
    FILE *fp = fopen(filename,"rb");
    if (!fp) {
        return NULL;
    }

    fseek(fp,0,SEEK_END);
    size_t size = ftell(fp);
    fseek(fp,0,SEEK_SET);

    char* buf = malloc(sizeof(char)*size);
    fread(buf,size,1,fp);
    fclose(fp);

    return imagelib_load_from_mem(buf, size, settings);
}

image_t *
imagelib_load_from_mem(char *buf, size_t size, const imageloadersettings_t settings)
{
    const unsigned char png_magic[] = { 0x89, 0x50, 0x4E, 0x47, 0x0D, 0x0A, 0x1A, 0x0A };

    if (memcmp(buf, png_magic, sizeof(png_magic)) == 0) {
        return imagelib_load_png_from_mem(buf, size, settings);
    } else {
        return imagelib_load_jpeg_from_mem(buf, size, settings);
    }
}