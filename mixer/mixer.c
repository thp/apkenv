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

#include "mixer.h"


struct Mixer *
g_mixer = 0;

void apkenv_mixer_register(struct Mixer *mixer)
{
    g_mixer = mixer;
}

int
apkenv_mixer_open(int frequency, enum AudioFormat format, int channels, int buffer)
{
    if (g_mixer) {
        g_mixer->config.frequency = frequency;
        g_mixer->config.format = format;
        g_mixer->config.channels = channels;
        g_mixer->config.buffer = buffer;
        g_mixer->open(g_mixer);
    }
}

void
apkenv_mixer_close()
{
    if (g_mixer) {
        g_mixer->close(g_mixer);
    }
}

struct MixerMusic *
apkenv_mixer_load_music(const char *filename)
{
    if (g_mixer) {
        return g_mixer->load_music(g_mixer, filename);
    }

    return 0;
}

struct MixerMusic *
apkenv_mixer_load_music_buffer(const char *buffer, size_t size)
{
    if (g_mixer) {
        return g_mixer->load_music_buffer(g_mixer, buffer, size);
    }

    return 0;
}

struct MixerSound *
apkenv_mixer_load_sound(const char *filename)
{
    if (g_mixer) {
        return g_mixer->load_sound(g_mixer, filename);
    }

    return 0;
}

struct MixerSound *
apkenv_mixer_load_sound_buffer(const char *buffer, size_t size)
{
    if (g_mixer) {
        return g_mixer->load_sound_buffer(g_mixer, buffer, size);
    }

    return 0;
}

void
apkenv_mixer_free_music(struct MixerMusic *music)
{
    if (g_mixer) {
        g_mixer->free_music(g_mixer, music);
    }
}

void
apkenv_mixer_free_sound(struct MixerSound *sound)
{
    if (g_mixer) {
        g_mixer->free_sound(g_mixer, sound);
    }
}

void
apkenv_mixer_play_music(struct MixerMusic *music, int do_loop)
{
    if (g_mixer) {
        g_mixer->play_music(g_mixer, music, do_loop);
    }
}

void
apkenv_mixer_play_sound(struct MixerSound *sound, int do_loop)
{
    if (g_mixer) {
        g_mixer->play_sound(g_mixer, sound, do_loop);
    }
}

void
apkenv_mixer_stop_music(struct MixerMusic *music)
{
    if (g_mixer) {
        g_mixer->stop_music(g_mixer, music);
    }
}

void
apkenv_mixer_stop_sound(struct MixerSound *sound)
{
    if (g_mixer) {
        g_mixer->stop_sound(g_mixer, sound);
    }
}

int
apkenv_mixer_get_sound_channel(struct MixerSound *sound)
{
    if (g_mixer) {
        return g_mixer->get_sound_channel(g_mixer, sound);
    }
}

void
apkenv_mixer_set_sound_channel(struct MixerSound *sound, int channel)
{
    if (g_mixer) {
        g_mixer->set_sound_channel(g_mixer, sound, channel);
    }
}

void
apkenv_mixer_volume_music(struct MixerMusic *music, float volume)
{
    if (g_mixer) {
        g_mixer->volume_music(g_mixer, music, volume);
    }
}

void
apkenv_mixer_volume_sound(struct MixerSound *sound, float volume)
{
    if (g_mixer) {
        g_mixer->volume_sound(g_mixer, sound, volume);
    }
}

void apkenv_mixer_sound_lame_resample_44100_32000(struct MixerSound *sound) {
    if (g_mixer) {
        g_mixer->lame_resample_44100_32000(g_mixer, sound);
    }
}

