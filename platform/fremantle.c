/**
 * apkenv
 * Copyright (c) 2012, 2014, Thomas Perl <m@thp.io>
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
#include "../accelerometer/accelerometer.h"

#include <SDL.h>
#include <SDL_gles.h>
#include <stdio.h>

#include "common/sdl_audio_impl.h"
#include "common/sdl_mixer_impl.h"

struct PlatformPriv {
    SDL_Surface *screen;
};

static struct PlatformPriv priv;


struct N900Accelerometer {
    // Must have the same structure as struct Accelerometer from accelerometer.h
    int (*init)(struct N900Accelerometer *accelerometer);
    int (*get)(struct N900Accelerometer *accelerometer, float *x, float *y, float *z);
};

static int
n900_accelerometer_init(struct N900Accelerometer *accelerometer)
{
    return 1;
}

static int
n900_accelerometer_get(struct N900Accelerometer *accelerometer, float *x, float *y, float *z)
{
    FILE *fp = fopen("/sys/class/i2c-adapter/i2c-3/3-001d/coord", "r");
    int xx = 0, yy = 0, zz = 0;
    int result = 0;

    if (!fp) {
        return 0;
    }

    if (fscanf(fp, "%i %i %i", &xx, &yy, &zz) == 3) {
        // X and Y are swapped, and X is negative; compare:
        // http://wiki.maemo.org/N900_accelerometer
        // http://developer.android.com/reference/android/hardware/SensorEvent.html

        if (x) {
            *x = 2 * 0.001 * yy;
        }

        if (y) {
            *y = 2 * 0.001 * -xx;
        }

        if (z) {
            *z = 2 * 0.001 * zz;
        }

        result = 1;
    }

    fclose(fp);
    return result;
}

static struct N900Accelerometer
g_n900_accelerometer = {
    n900_accelerometer_init,
    n900_accelerometer_get,
};

static struct Accelerometer *
n900_accelerometer = (struct Accelerometer *)(&g_n900_accelerometer);


static int
fremantle_init(int gles_version)
{
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_JOYSTICK) < 0) {
        return 0;
    }

    SDL_GLES_Init(gles_version == 2 ? SDL_GLES_VERSION_2_0 : SDL_GLES_VERSION_1_1);
    SDL_GLES_SetAttribute(SDL_GLES_DEPTH_SIZE, 16);
    priv.screen = SDL_SetVideoMode(0, 0, 0, SDL_FULLSCREEN);
    SDL_GLES_MakeCurrent(SDL_GLES_CreateContext());

    if (priv.screen == NULL) {
        return 0;
    }

    SDL_ShowCursor(0);

    apkenv_accelerometer_register(n900_accelerometer);
    apkenv_audio_register(sdl_audio);
    apkenv_mixer_register(sdl_mixer);

    return 1;
}

static const char *
fremantle_get_path(enum PlatformPath which)
{
    switch (which) {
        case PLATFORM_PATH_INSTALL_DIRECTORY:
            return  "/home/user/.local/share/applications/";
        case PLATFORM_PATH_DATA_DIRECTORY:
            return "/home/user/.apkenv/";
        case PLATFORM_PATH_MODULE_DIRECTORY:
            return "/opt/apkenv/modules/";
        default:
            return NULL;
    }
}

static void
fremantle_get_size(int *width, int *height)
{
    if (width) {
        *width = priv.screen->w;
    }

    if (height) {
        *height = priv.screen->h;
    }
}

static int
fremantle_input_update(struct SupportModule *module)
{
    int width = priv.screen->w;
    int height = priv.screen->h;

    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_MOUSEBUTTONDOWN) {
            module->input(module, ACTION_DOWN, e.button.x, e.button.y, e.button.which);
        } else if (e.type == SDL_MOUSEBUTTONUP) {
            module->input(module, ACTION_UP, e.button.x, e.button.y, e.button.which);
        } else if (e.type == SDL_MOUSEMOTION) {
            module->input(module, ACTION_MOVE, e.motion.x, e.motion.y, e.motion.which);
        } else if (e.type == SDL_QUIT) {
            return 1;
        } else if (e.type == SDL_ACTIVEEVENT) {
            if (e.active.state == SDL_APPACTIVE && e.active.gain == 0) {
                module->pause(module);
                while (1) {
                    SDL_WaitEvent(&e);
                    if (e.type == SDL_ACTIVEEVENT) {
                        if (e.active.state == SDL_APPACTIVE &&
                                e.active.gain == 1) {
                            break;
                        }
                    } else if (e.type == SDL_QUIT) {
                        return 1;
                    }
                }
                module->resume(module);
            }
        }
    }

    return 0;
}

static void
fremantle_request_text_input(int is_password, const char *text,
        text_callback_t callback, void *user_data)
{
    fprintf(stderr, "XXX: Request text input not implemented!\n");

    // Admit failure right away
    callback(NULL, user_data);
}

static void
fremantle_update()
{
    SDL_GLES_SwapBuffers();
}

static void
fremantle_exit()
{
}

struct PlatformSupport platform_support = {
    fremantle_init,
    fremantle_get_path,
    fremantle_get_size,
    fremantle_input_update,
    fremantle_request_text_input,
    fremantle_update,
    fremantle_exit,
};
