/**
 * apkenv
 * Copyright (c) 2012, 2014, Thomas Perl <m@thp.io>
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

#include <linux/limits.h>

#include <SDL2/SDL.h>

#include "common/sdl_accelerometer_impl.h"
#include "common/sdl_audio_impl.h"
#include "common/sdl_mixer_impl.h"

#include "common/input_transform.h"

struct PlatformPriv {
    SDL_Window *window;
    SDL_GLContext glcontext;
};

static struct PlatformPriv priv;


static int
rpi2_init(int gles_version)
{
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_JOYSTICK) < 0) {
        return 0;
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, gles_version);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);

    priv.window = SDL_CreateWindow(
                                    "apkenv",
                                    SDL_WINDOWPOS_UNDEFINED,
                                    SDL_WINDOWPOS_UNDEFINED,
                                    0, 0,
                                    SDL_WINDOW_OPENGL|SDL_WINDOW_FULLSCREEN
                                  );

    priv.glcontext = SDL_GL_CreateContext(priv.window);

    /* TODO: swipe lock? */

    SDL_ShowCursor(0);

    apkenv_accelerometer_register(sdl_accelerometer);

    apkenv_audio_register(sdl_audio);
    apkenv_mixer_register(sdl_mixer);

    return 1;
}

static const char *
rpi2_get_path(enum PlatformPath which)
{
    switch (which) {
        case PLATFORM_PATH_INSTALL_DIRECTORY:
            return "/home/debian/.local/share/applications/";
        case PLATFORM_PATH_DATA_DIRECTORY:
            return "/home/debian/.apkenv/";
        case PLATFORM_PATH_MODULE_DIRECTORY:
            return "/usr/lib/apkenv/modules/";
        default:
            return NULL;
    }
}

static void
rpi2_get_size(int *width, int *height)
{
    SDL_GetWindowSize(priv.window, width, height);
}

static int
rpi2_input_update(struct SupportModule *module)
{
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_FINGERDOWN) {
            module->input(module, ACTION_DOWN, input_transform_x(e.tfinger.x, e.tfinger.y), input_transform_y(e.tfinger.x, e.tfinger.y), e.tfinger.fingerId);
        } else if (e.type == SDL_FINGERUP) {
            module->input(module, ACTION_UP, input_transform_x(e.tfinger.x, e.tfinger.y), input_transform_y(e.tfinger.x, e.tfinger.y), e.tfinger.fingerId);
        } else if (e.type == SDL_FINGERMOTION) {
            module->input(module, ACTION_MOVE, input_transform_x(e.tfinger.x, e.tfinger.y), input_transform_y(e.tfinger.x, e.tfinger.y), e.tfinger.fingerId);
        } else if (e.type == SDL_QUIT) {
            return 1;
        } else if (e.type == SDL_WINDOWEVENT) {
            switch(e.window.event) {
                case SDL_WINDOWEVENT_FOCUS_LOST:
                    module->pause(module);
                    while (1) {
                        SDL_WaitEvent(&e);
                        if (e.type == SDL_WINDOWEVENT) {
                            if(e.window.event == SDL_WINDOWEVENT_FOCUS_GAINED)
                                break;
                        } else if (e.type == SDL_QUIT) {
                            return 1;
                        }
                    }
                    module->resume(module);
                    break;
            }
        }
    }

    return 0;
}

static int
rpi2_get_orientation(void)
{
    return ORIENTATION_LANDSCAPE;
}

static void
rpi2_request_text_input(int is_password, const char *text,
        text_callback_t callback, void *user_data)
{
    fprintf(stderr, "XXX: Request text input not implemented!\n");

    // Admit failure right away
    callback(NULL, user_data);
}

static void
rpi2_update()
{
    SDL_GL_SwapWindow(priv.window);
}

static void
rpi2_exit()
{
    SDL_GL_DeleteContext(priv.glcontext);
    SDL_DestroyWindow(priv.window);
    SDL_Quit();
}

struct PlatformSupport platform_support = {
    rpi2_init,
    rpi2_get_path,
    rpi2_get_size,
    rpi2_input_update,
    rpi2_get_orientation,
    rpi2_request_text_input,
    rpi2_update,
    rpi2_exit,
};

