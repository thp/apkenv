
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
 * Generic support module - can be used as a template for new modules
 * This module does not do anything useful, except for calling JNI_OnLoad
 **/

#include "common.h"

struct SupportModulePriv {
    jni_onload_t JNI_OnLoad;
};
static struct SupportModulePriv generic_priv;

static int
generic_try_init(struct SupportModule *self)
{
    self->priv->JNI_OnLoad = (jni_onload_t)LOOKUP_M("JNI_OnLoad");

    return (self->priv->JNI_OnLoad != NULL);
}

static void
generic_init(struct SupportModule *self, int width, int height, const char *home)
{
    self->priv->JNI_OnLoad(VM_M, NULL);
}

static void
generic_input(struct SupportModule *self, int event, int x, int y, int finger)
{
}

static void
generic_key_input(struct SupportModule *self, int event, int keycode, int unicode)
{
}

static void
generic_update(struct SupportModule *self)
{
}

static void
generic_deinit(struct SupportModule *self)
{
}

static void
generic_pause(struct SupportModule *self)
{
}

static void
generic_resume(struct SupportModule *self)
{
}
static int
generic_requests_exit(struct SupportModule *self)
{
    return 0;
}

APKENV_MODULE(generic, MODULE_PRIORITY_GENERIC)

