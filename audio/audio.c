/**
 * apkenv Audio
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

#include "audio.h"

static struct Audio *
g_audio = 0;

void
apkenv_audio_register(struct Audio *audio)
{
    g_audio = audio;
}

int
apkenv_audio_open(int frequency, enum AudioFormat format, int channels,
        int buffer, audio_callback_t callback, void *user_data)
{
    if (g_audio) {
        g_audio->config.frequency = frequency;
        g_audio->config.format = format;
        g_audio->config.channels = channels;
        g_audio->config.buffer = buffer;
        g_audio->config.callback = (void (*)(void *, void *, int))callback;
        g_audio->config.user_data = user_data;
        return g_audio->open(g_audio);
    }

    return 0;
}

void
apkenv_audio_close()
{
    if (g_audio) {
        g_audio->close(g_audio);
    }
}

void
apkenv_audio_play()
{
    if (g_audio) {
        g_audio->play(g_audio);
    }
}

void
apkenv_audio_pause()
{
    if (g_audio) {
        g_audio->pause(g_audio);
    }
}
