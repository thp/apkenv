/**
 * apkenv
 * Copyright (c) 2012, 2014, 2023 Thomas Perl <m@thp.io>
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


/* Raspberry Pi 3 port, tested with Raspbian GNU/Linux 11 */


#include "../apkenv.h"

#include <SDL.h>

#include "common/sdl_accelerometer_impl.h"
#include "common/sdl_audio_impl.h"
#include "common/sdl_mixer_impl.h"

struct PlatformPriv {
    SDL_Surface *screen;
    char *install_directory;
    char *data_directory;
};

static struct PlatformPriv priv;


static int
rpi3_init(int gles_version)
{
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_JOYSTICK) < 0) {
        return 0;
    }

    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);

    priv.screen = SDL_SetVideoMode(800, 480, 0, SDL_OPENGL);
    SDL_WM_SetCaption("apkenv", "apkenv");

    if (priv.screen == NULL) {
        return 0;
    }

    apkenv_accelerometer_register(sdl_accelerometer);
    apkenv_audio_register(sdl_audio);
    apkenv_mixer_register(sdl_mixer);

    return 1;
}

static const char *
rpi3_get_path(enum PlatformPath which)
{
    switch (which) {
        case PLATFORM_PATH_INSTALL_DIRECTORY:
            if (!priv.install_directory) {
                if (asprintf(&priv.install_directory, "%s/.local/share/applications/", getenv("HOME") ?: ".") == -1) {
                    fprintf(stderr, "Could not format install directory.\n");
                    exit(1);
                }
            }
            return priv.install_directory;
        case PLATFORM_PATH_DATA_DIRECTORY:
            if (!priv.data_directory) {
                if (asprintf(&priv.data_directory, "%s/.apkenv/", getenv("HOME") ?: ".") == -1) {
                    fprintf(stderr, "Coudl not format data directory.\n");
                    exit(1);
                }
            }
            return priv.data_directory;
        case PLATFORM_PATH_MODULE_DIRECTORY:
            return "/usr/lib/apkenv/modules/";
        default:
            return NULL;
    }
}

static void
rpi3_get_size(int *width, int *height)
{
    if (width) {
        *width = priv.screen->w;
    }

    if (height) {
        *height = priv.screen->h;
    }
}

static int
rpi3_input_update(struct SupportModule *module)
{
    int width = priv.screen->w;
    int height = priv.screen->h;

    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_MOUSEBUTTONDOWN) {
            module->input(module, ACTION_DOWN, e.button.x, e.button.y, 0);
        } else if (e.type == SDL_MOUSEBUTTONUP) {
            module->input(module, ACTION_UP, e.button.x, e.button.y, 0);
        } else if (e.type == SDL_MOUSEMOTION && e.motion.state) {
            module->input(module, ACTION_MOVE, e.motion.x, e.motion.y, 0);
        } else if (e.type == SDL_QUIT) {
            return 1;
        }
    }

    return 0;
}

static int
rpi3_get_orientation(void)
{
    return ORIENTATION_LANDSCAPE;
}

static void
rpi3_request_text_input(int is_password, const char *text,
        text_callback_t callback, void *user_data)
{
    fprintf(stderr, "XXX: Request text input not implemented!\n");

    // Admit failure right away
    callback(NULL, user_data);
}

static void
rpi3_update()
{
    SDL_GL_SwapBuffers();
}

static void
rpi3_exit()
{
}

struct PlatformSupport platform_support = {
    rpi3_init,
    rpi3_get_path,
    rpi3_get_size,
    rpi3_input_update,
    rpi3_get_orientation,
    rpi3_request_text_input,
    rpi3_update,
    rpi3_exit,
};
