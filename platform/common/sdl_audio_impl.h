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


#include "../../audio/audio.h"

#include <assert.h>

static int
sdl_audio_open(struct Audio *audio)
{
    SDL_AudioSpec desired, obtained;

    desired.freq = audio->config.frequency;
    desired.channels = audio->config.channels;
    switch (audio->config.format) {
        case AudioFormat_S16SYS:
            desired.format = AUDIO_S16SYS;
            break;
        default:
            // FIXME: Unknown or new format
            assert(0);
            break;
    }
    desired.samples = audio->config.buffer;
    desired.callback = audio->config.callback;
    desired.userdata = audio->config.user_data;

    return (SDL_OpenAudio(&desired, &obtained) == 0);
}

static void
sdl_audio_close(struct Audio *audio)
{
    SDL_CloseAudio();
}

static void
sdl_audio_play(struct Audio *audio)
{
    SDL_PauseAudio(0);
}

static void
sdl_audio_pause(struct Audio *audio)
{
    SDL_PauseAudio(1);
}

static struct Audio
g_sdl_audio = {
    sdl_audio_open,
    sdl_audio_close,
    sdl_audio_play,
    sdl_audio_pause,
    { 0 },
};

static struct Audio *
sdl_audio = &g_sdl_audio;
