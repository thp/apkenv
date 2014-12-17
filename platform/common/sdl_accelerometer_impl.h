/**
 * apkenv Accelerometer
 * Copyright (c) 2014, Thomas Perl <m@thp.io>
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

#include <SDL.h>

#include "../../accelerometer/accelerometer.h"

struct SDLAccelerometer {
    // Must have the same structure as struct Accelerometer from accelerometer.h
    int (*init)(struct SDLAccelerometer *accelerometer);
    int (*get)(struct SDLAccelerometer *Accelerometer, float *x, float *y, float *z);

    // Additional fields can be added after struct Accelerometer here
    SDL_Joystick *joystick;
};

static int
sdl_accelerometer_init(struct SDLAccelerometer *accelerometer)
{
    accelerometer->joystick = SDL_JoystickOpen(0);

    return (accelerometer->joystick != 0);
}

#define CONVERT_ACCELEROMETER(x) ((float)(x) / 32768. * 9.81 * 2)
#define GET_AXIS(j, x) CONVERT_ACCELEROMETER(SDL_JoystickGetAxis(j, x))

static int
sdl_accelerometer_get(struct SDLAccelerometer *accelerometer, float *x, float *y, float *z)
{
    if (!accelerometer->joystick) {
        return 0;
    }

    SDL_JoystickUpdate();

    if (x) {
        *x = GET_AXIS(accelerometer->joystick, 0);
    }

    if (y) {
        *y = GET_AXIS(accelerometer->joystick, 1);
    }

    if (z) {
        *z = GET_AXIS(accelerometer->joystick, 2);
    }

    return 1;
}

static struct SDLAccelerometer
g_sdl_accelerometer = {
    sdl_accelerometer_init,
    sdl_accelerometer_get,
    NULL,
};

static struct Accelerometer *
sdl_accelerometer = (struct Accelerometer *)(&g_sdl_accelerometer);
