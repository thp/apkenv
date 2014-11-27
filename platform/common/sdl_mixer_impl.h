/**
 * apkenv Mixer
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

#include "../../mixer/mixer.h"

#include <SDL.h>
#include <SDL_mixer.h>
#include <assert.h>

/* older SDL_mixer compatibility */
#ifdef MIX_MAJOR_VERSION
#if ((MIX_MAJOR_VERSION << 16) | (MIX_MINOR_VERSION << 8) | MIX_PATCHLEVEL) < 0x1020a
#define Mix_Init(x)
#define Mix_Quit()
#endif
#endif

struct MixerMusic {
    Mix_Music *music;
};

struct MixerSound {
    Mix_Chunk *chunk;
    int channel;
};


int
sdl_mixer_open(struct Mixer *mixer)
{
    Mix_Init(MIX_INIT_OGG | MIX_INIT_MP3);

    int fmt;
    switch (mixer->config.format) {
        case AudioFormat_S16SYS:
            fmt = AUDIO_S16SYS;
            break;
        default:
            assert(0);
            break;
    }

    int result = (Mix_OpenAudio(mixer->config.frequency, fmt, mixer->config.channels, mixer->config.buffer) == 0);
    Mix_AllocateChannels(16);

    return result;
}

void
sdl_mixer_close(struct Mixer *mixer)
{
    Mix_CloseAudio();
}

struct MixerMusic *
sdl_mixer_load_music(struct Mixer *mixer, const char *filename)
{
    struct MixerMusic *music = calloc(1, sizeof(struct MixerMusic));
    music->music = Mix_LoadMUS(filename);
    return music;
}

struct MixerMusic *
sdl_mixer_load_music_buffer(struct Mixer *mixer, const char *buffer, size_t size)
{
    struct MixerMusic *music = calloc(1, sizeof(struct MixerMusic));
    SDL_RWops *rw = SDL_RWFromConstMem(buffer, size);
#if SDL_VERSION_ATLEAST(2,0,0)
    music->music = Mix_LoadMUS_RW(rw, 0);
#else
    music->music = Mix_LoadMUS_RW(rw);
#endif
    SDL_RWclose(rw);
    return music;

}

struct MixerSound *
sdl_mixer_load_sound(struct Mixer *mixer, const char *filename)
{
    struct MixerSound *sound = calloc(1, sizeof(struct MixerSound));
    sound->chunk = Mix_LoadWAV(filename);
    return sound;
}

struct MixerSound *
sdl_mixer_load_sound_buffer(struct Mixer *mixer, const char *buffer, size_t size)
{
    struct MixerSound *sound = calloc(1, sizeof(struct MixerSound));
    SDL_RWops *rw = SDL_RWFromConstMem(buffer, size);
    sound->chunk = Mix_LoadWAV_RW(rw, 0);
    SDL_RWclose(rw);
    return sound;
}

void
sdl_mixer_free_music(struct Mixer *mixer, struct MixerMusic *music)
{
    Mix_FreeMusic(music->music);
    free(music);
}

void
sdl_mixer_free_sound(struct Mixer *mixer, struct MixerSound *sound)
{
    Mix_FreeChunk(sound->chunk);
    free(sound);
}

void
sdl_mixer_play_music(struct Mixer *mixer, struct MixerMusic *music, int do_loop)
{
    Mix_PlayMusic(music->music, do_loop ? -1 : 0);
}

void
sdl_mixer_play_sound(struct Mixer *mixer, struct MixerSound *sound, int do_loop)
{
    sound->channel = Mix_PlayChannel(-1, sound->chunk, do_loop ? -1 : 0);
}

void
sdl_mixer_stop_music(struct Mixer *mixer, struct MixerMusic *music)
{
    Mix_HaltMusic();
}

void
sdl_mixer_stop_sound(struct Mixer *mixer, struct MixerSound *sound)
{
    Mix_HaltChannel(sound->channel);
}

void
sdl_mixer_volume_music(struct Mixer *mixer, struct MixerMusic *music, float volume)
{
    Mix_VolumeMusic(volume * MIX_MAX_VOLUME);
}

void
sdl_mixer_volume_sound(struct Mixer *mixer, struct MixerSound *sound, float volume)
{
    Mix_VolumeChunk(sound->chunk, volume * MIX_MAX_VOLUME);
}


static struct Mixer
g_sdl_mixer = {
    sdl_mixer_open,
    sdl_mixer_close,
    sdl_mixer_load_music,
    sdl_mixer_load_music_buffer,
    sdl_mixer_load_sound,
    sdl_mixer_load_sound_buffer,
    sdl_mixer_free_music,
    sdl_mixer_free_sound,
    sdl_mixer_play_music,
    sdl_mixer_play_sound,
    sdl_mixer_stop_music,
    sdl_mixer_stop_sound,
    sdl_mixer_volume_music,
    sdl_mixer_volume_sound,
    { 0 },
};

static struct Mixer *
sdl_mixer = &g_sdl_mixer;
