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

#ifndef APKENV_MIXER_H
#define APKENV_MIXER_H

#include "../audio/audio.h"

#include <stdlib.h>

struct MixerMusic;
struct MixerSound;

struct Mixer {
    // Initialize the mixer
    int (*open)(struct Mixer *mixer);

    // Close the mixer
    void (*close)(struct Mixer *mixer);

    // Loading of data
    struct MixerMusic *(*load_music)(struct Mixer *mixer,
            const char *filename);
    struct MixerMusic *(*load_music_buffer)(struct Mixer *mixer,
            const char *buffer, size_t size);
    struct MixerSound *(*load_sound)(struct Mixer *mixer,
            const char *filename);
    struct MixerSound *(*load_sound_buffer)(struct Mixer *mixer,
            const char *buffer, size_t size);

    // Freeing of data
    void (*free_music)(struct Mixer *mixer, struct MixerMusic *music);
    void (*free_sound)(struct Mixer *mixer, struct MixerSound *sound);

    // Playback
    void (*play_music)(struct Mixer *mixer, struct MixerMusic *music,
            int do_loop);
    void (*play_sound)(struct Mixer *mixer, struct MixerSound *sound,
            int do_loop);
    void (*stop_music)(struct Mixer *mixer, struct MixerMusic *music);
    void (*stop_sound)(struct Mixer *mixer, struct MixerSound *sound);
    int (*get_sound_channel)(struct Mixer *mixer, struct MixerSound *sound);
    void (*set_sound_channel)(struct Mixer *mixer, struct MixerSound *sound, int channel);

    // Volume control
    void (*volume_music)(struct Mixer *mixer, struct MixerMusic *music,
            float volume);
    void (*volume_sound)(struct Mixer *mixer, struct MixerSound *sound,
            float volume);

    void (*lame_resample_44100_32000)(struct Mixer *mixer, struct MixerSound *sound);

    // Internal structure to store the config from init
    struct AudioConfig config;
};


void apkenv_mixer_register(struct Mixer *mixer);

int apkenv_mixer_open(int frequency, enum AudioFormat format, int channels, int buffer);
void apkenv_mixer_close();
struct MixerMusic *apkenv_mixer_load_music(const char *filename);
struct MixerMusic *apkenv_mixer_load_music_buffer(const char *buffer, size_t size);
struct MixerSound *apkenv_mixer_load_sound(const char *filename);
struct MixerSound *apkenv_mixer_load_sound_buffer(const char *buffer, size_t size);
void apkenv_mixer_free_music(struct MixerMusic *music);
void apkenv_mixer_free_sound(struct MixerSound *sound);
void apkenv_mixer_play_music(struct MixerMusic *music, int do_loop);
void apkenv_mixer_play_sound(struct MixerSound *sound, int do_loop);
void apkenv_mixer_stop_music(struct MixerMusic *music);
void apkenv_mixer_stop_sound(struct MixerSound *sound);
int apkenv_mixer_get_sound_channel(struct MixerSound *sound);
void apkenv_mixer_set_sound_channel(struct MixerSound *sound, int channel);
void apkenv_mixer_volume_music(struct MixerMusic *music, float volume);
void apkenv_mixer_volume_sound(struct MixerSound *sound, float volume);
void apkenv_mixer_sound_lame_resample_44100_3200(struct MixerSound *sound);

#endif /* APKENV_MIXER_H */
