/**
 * apkenv
 * Copyright (c) 2012, 2014, 2016, 2023 Thomas Perl <m@thp.io>
 * Copyright (c) 2014 Franz-Josef Anton Friedrich Haider <f_haider@gmx.at>
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


#include "../apkenv.h"

#include <stdio.h>

#include <GL/osmesa.h>
#include <EGL/egl.h>


struct PlatformPriv {
    OSMesaContext ctx;

    uint32_t *pixels;
    int width;
    int height;
};

static struct PlatformPriv priv;


static int
osmesa_init(int gles_version)
{
    priv.ctx = OSMesaCreateContextExt(OSMESA_RGBA, 16, 0, 0, NULL);

    priv.width = 960;
    priv.height = 540;

    priv.pixels = malloc(sizeof(uint32_t) * priv.width * priv.height);

    OSMesaMakeCurrent(priv.ctx, priv.pixels, GL_UNSIGNED_BYTE, priv.width, priv.height);

    // TODO: apkenv_audio_register(), apkenv_mixer_register()

    return 1;
}

static const char *
osmesa_get_path(enum PlatformPath which)
{
    switch (which) {
        case PLATFORM_PATH_INSTALL_DIRECTORY:
            return NULL; /* install not supported */
        case PLATFORM_PATH_DATA_DIRECTORY:
            return "./data/";
        case PLATFORM_PATH_MODULE_DIRECTORY:
            return "./staging";
        default:
            return NULL;
    }
}

static void
osmesa_get_size(int *width, int *height)
{
    if (width) {
        *width = priv.width;
    }
    if (height) {
        *height = priv.height;
    }
}

static int
osmesa_input_update(struct SupportModule *module)
{
    // TODO: Call module->input(module, ...);
    fprintf(stderr, "TODO: Input update\n");

    return 0;
}

static int
osmesa_get_orientation(void)
{
    return ORIENTATION_LANDSCAPE;
}

static void
osmesa_request_text_input(int is_password, const char *text,
        text_callback_t callback, void *user_data)
{
    fprintf(stderr, "XXX: Request text input not implemented!\n");

    // Admit failure right away
    callback(NULL, user_data);
}

static void
osmesa_update()
{
    printf("- SWAP -\n");
    glFinish();
    glFlush();

    FILE *fp = fopen("screen.ppm", "wb");
    if (fp != NULL) {
        fprintf(fp, "P6\n%d %d\n255\n", priv.width, priv.height);

        const uint8_t *src = (const uint8_t *)priv.pixels;
        for (int y=0; y<priv.height; ++y) {
            int base = priv.width * (priv.height - 1 - y);
            for (int x=0; x<priv.width; ++x) {
                fputc(src[(base+x)*4+0], fp);
                fputc(src[(base+x)*4+1], fp);
                fputc(src[(base+x)*4+2], fp);
            }
        }
        fclose(fp);
    }
}

static void
osmesa_exit()
{
    free(priv.pixels);

    OSMesaDestroyContext(priv.ctx);
}

typedef void (*eglGetProcAddressReturnType)(void);

eglGetProcAddressReturnType
eglGetProcAddress(const char *procname)
{
    OSMESAproc result = OSMesaGetProcAddress(procname);

    fprintf(stderr, "eglGetProcAddress(procname=\"%s\") -> %p\n", procname, result);

    return result;
}

const char *
eglQueryString(EGLDisplay dpy, EGLint name)
{
    fprintf(stderr, "eglQueryString(dpy=%p, name=0x%08x)\n", dpy, name);

    switch (name) {
        case EGL_CLIENT_APIS:
            return "OpenGL_ES ";
        case EGL_VENDOR:
            return "apkenv";
        case EGL_VERSION:
            return "1.5 apkenv-osmesa";
        case EGL_EXTENSIONS:
            return "";
        default:
            break;
    }

    return NULL;
}

struct PlatformSupport platform_support = {
    osmesa_init,
    osmesa_get_path,
    osmesa_get_size,
    osmesa_input_update,
    osmesa_get_orientation,
    osmesa_request_text_input,
    osmesa_update,
    osmesa_exit,
};
