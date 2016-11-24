/**
 * apkenv
 * Copyright (c) 2012, 2014, 2016, Thomas Perl <m@thp.io>
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

#include "SDL.h"
#include "common/sdl_audio_impl.h"
#include "common/sdl_mixer_impl.h"

#include "eglo.h"

struct PlatformPriv {
    int width;
    int height;
};

static struct PlatformPriv priv;


static int
pocketchip_init(int gles_version)
{
    if (SDL_Init(SDL_INIT_AUDIO | SDL_INIT_JOYSTICK) < 0) {
        return 0;
    }

    eglo_init(&priv.width, &priv.height, gles_version);

    apkenv_audio_register(sdl_audio);
    apkenv_mixer_register(sdl_mixer);

    return 1;
}

static const char *
pocketchip_get_path(enum PlatformPath which)
{
    switch (which) {
        case PLATFORM_PATH_INSTALL_DIRECTORY:
            return  "/home/chip/.local/share/applications/";
        case PLATFORM_PATH_DATA_DIRECTORY:
            return "/home/chip/.apkenv/";
        case PLATFORM_PATH_MODULE_DIRECTORY:
            return "/usr/lib/apkenv/modules/";
        default:
            return NULL;
    }
}

static void
pocketchip_get_size(int *width, int *height)
{
    if (width) {
        *width = priv.width;
    }
    if (height) {
        *height = priv.height;
    }
}

static int
pocketchip_input_update(struct SupportModule *module)
{
    EgloEvent e;
    while (eglo_next_event(&e)) {
        if (e.type == EGLO_MOUSE_DOWN) {
            module->input(module, ACTION_DOWN, e.mouse.x, e.mouse.y, 0);
        } else if (e.type == EGLO_MOUSE_UP) {
            module->input(module, ACTION_UP, e.mouse.x, e.mouse.y, 0);
        } else if (e.type == EGLO_MOUSE_MOTION) {
            module->input(module, ACTION_MOVE, e.mouse.x, e.mouse.y, 0);
        } else if (e.type == EGLO_QUIT) {
            return 1;
        }
    }

    return 0;
}

static int
pocketchip_get_orientation(void)
{
    return ORIENTATION_LANDSCAPE;
}

static void
pocketchip_request_text_input(int is_password, const char *text,
        text_callback_t callback, void *user_data)
{
    fprintf(stderr, "XXX: Request text input not implemented!\n");

    // Admit failure right away
    callback(NULL, user_data);
}

static void
pocketchip_update()
{
    eglo_swap_buffers();
}

static void
pocketchip_exit()
{
    eglo_quit();
}

struct PlatformSupport platform_support = {
    pocketchip_init,
    pocketchip_get_path,
    pocketchip_get_size,
    pocketchip_input_update,
    pocketchip_get_orientation,
    pocketchip_request_text_input,
    pocketchip_update,
    pocketchip_exit,
};
