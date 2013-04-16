
/**
 * apkenv
 * Copyright (c) 2012, Thomas Perl <m@thp.io>
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


/**
 * Support module for That Rabbit Game 2
 * Download free .apk from: http://thp.io/2011/trg2/
 **/

#include "common.h"

#include <SDL/SDL.h>

typedef void (*trg2_init_t)(JNIEnv *env, jobject obj) SOFTFP;
typedef void (*trg2_resize_t)(JNIEnv *env, jobject obj, jint width, jint height) SOFTFP;
typedef void (*trg2_touch_t)(JNIEnv *env, jobject obj, jint action, jfloat x, jfloat y, jint finger) SOFTFP;
typedef jboolean (*trg2_render_t)(JNIEnv *env, jobject obj) SOFTFP;
typedef jboolean (*trg2_accelerometer_t)(JNIEnv *env, jobject obj, jfloat x, jfloat y, jfloat z) SOFTFP;

struct SupportModulePriv {
    jni_onload_t JNI_OnLoad;
    trg2_init_t native_init;
    trg2_resize_t native_resize;
    trg2_touch_t native_touch;
    trg2_render_t native_render;
    trg2_accelerometer_t native_accelerometer;
    SDL_Joystick *joystick;
};
static struct SupportModulePriv trg2_priv;

static int
trg2_try_init(struct SupportModule *self)
{
    self->priv->JNI_OnLoad = (jni_onload_t)LOOKUP_M("JNI_OnLoad");
    self->priv->native_init = (trg2_init_t)LOOKUP_M("Java_io_thp_trg2_android_GameRenderer_nativeInit");
    self->priv->native_resize = (trg2_resize_t)LOOKUP_M("nativeResize");
    self->priv->native_touch = (trg2_touch_t)LOOKUP_M("nativeTouch");
    self->priv->native_render = (trg2_render_t)LOOKUP_M("nativeRender");
    self->priv->native_accelerometer = (trg2_accelerometer_t)LOOKUP_M("nativeAccelerometer");

    return (self->priv->JNI_OnLoad != NULL &&
            self->priv->native_init != NULL &&
            self->priv->native_resize != NULL &&
            self->priv->native_touch != NULL &&
            self->priv->native_render != NULL);
}

static void
trg2_init(struct SupportModule *self, int width, int height, const char *home)
{
    self->priv->joystick = SDL_JoystickOpen(0);
    self->priv->JNI_OnLoad(VM_M, NULL);
    self->priv->native_init(ENV_M, GLOBAL_M);
    self->priv->native_resize(ENV_M, GLOBAL_M, width, height);
}

static void
trg2_input(struct SupportModule *self, int event, int x, int y, int finger)
{
    self->priv->native_touch(ENV_M, GLOBAL_M, event, x, y, finger);
}

static void
trg2_key_input(struct SupportModule *self, int event, int keycode, int unicode)
{
}

#define CONVERT_ACCELEROMETER(x) ((float)(x) / 32768. * 9.81 * 2)
#define GET_AXIS(j, x) CONVERT_ACCELEROMETER(SDL_JoystickGetAxis(j, x))

static void
trg2_update(struct SupportModule *self)
{
    if (self->priv->native_accelerometer != NULL) {
        SDL_JoystickUpdate();
        self->priv->native_accelerometer(ENV_M, GLOBAL_M,
                GET_AXIS(self->priv->joystick, 0),
                GET_AXIS(self->priv->joystick, 1),
                GET_AXIS(self->priv->joystick, 2));
    }

    self->priv->native_render(ENV_M, GLOBAL_M);
}

static void
trg2_deinit(struct SupportModule *self)
{
}

static void
trg2_pause(struct SupportModule *self)
{
}

static void
trg2_resume(struct SupportModule *self)
{
}

static int
trg2_requests_exit()
{
    return 0;
}

APKENV_MODULE(trg2, MODULE_PRIORITY_GAME)

