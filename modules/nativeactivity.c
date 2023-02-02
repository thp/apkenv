
/**
 * apkenv
 * Copyright (c) 2023 Thomas Perl <m@thp.io>
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
 * Support module for ANativeActivity-based apps
 **/

#include "common.h"

#include "../compat/android_wrappers.h"


struct SupportModulePriv {
    void (*android_main)(struct android_app *state) SOFTFP;

    struct ANativeActivity activity;
    struct android_app app;
};

static struct SupportModulePriv
nativeactivity_priv;

static int
nativeactivity_try_init(struct SupportModule *self)
{
    self->priv->android_main = LOOKUP_M("android_main");

    return (self->priv->android_main != NULL);
}

static void
nativeactivity_init(struct SupportModule *self, int width, int height, const char *home)
{
    GLOBAL_M->module_hacks->current_orientation = ORIENTATION_LANDSCAPE;

    self->priv->activity.vm = VM_M;
    self->priv->activity.env = ENV_M;
    self->priv->activity.internalDataPath = home;
    self->priv->activity.externalDataPath = home;

    self->priv->app.activity = &self->priv->activity;
    self->priv->app.window = (void *)0xDEADBEEF;

    self->priv->app.apkenv_module = self;

    apkenv_android_wrappers_set_android_app(&self->priv->app);

    fprintf(stderr, "calling android_main()\n");
    self->priv->android_main(&self->priv->app);
    fprintf(stderr, "android_main() returned\n");
}

static void
nativeactivity_input(struct SupportModule *self, int event, int x, int y, int finger)
{
    struct AInputEvent evt = {
        .type = AINPUT_EVENT_TYPE_MOTION,
        .action = event,
        .x = x,
        .y = y,
    };

    if (self->priv->app.onInputEvent) {
        self->priv->app.onInputEvent(&self->priv->app, &evt);
    }
}

static void
nativeactivity_key_input(struct SupportModule *self, int event, int keycode, int unicode)
{
    // TODO
}

static void
nativeactivity_update(struct SupportModule *self)
{
    // TODO
}

static void
nativeactivity_deinit(struct SupportModule *self)
{
    // TODO
}

static void
nativeactivity_pause(struct SupportModule *self)
{
    // TODO
}

static void
nativeactivity_resume(struct SupportModule *self)
{
    // TODO
}

static int
nativeactivity_requests_exit()
{
    /* Request exit from apkenv main loop, as we looped in
     * android_main() in nativeactivity_init() already */
    return 1;
}

APKENV_MODULE(nativeactivity, MODULE_PRIORITY_GENERIC)
