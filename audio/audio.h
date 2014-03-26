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

#ifndef APKENV_AUDIO_H
#define APKENV_AUDIO_H

typedef void (*audio_callback_t)(void *user_data, void *stream, int len);

enum AudioFormat {
    AudioFormat_Undefined = 0,
    AudioFormat_S16SYS = 1,
    // TODO: Add more audio formats as needed
};

struct AudioConfig {
    int frequency;
    enum AudioFormat format;
    int channels;
    int buffer;
    audio_callback_t callback;
    void *user_data;
};

struct Audio {
    // Open the audio device
    int (*open)(struct Audio *audio);

    // Close the audio device
    void (*close)(struct Audio *audio);

    // Start playback
    void (*play)(struct Audio *audio);

    // Stop playback
    void (*pause)(struct Audio *audio);

    // Internal structure to store the config from init
    struct AudioConfig config;
};


void apkenv_audio_register(struct Audio *audio);

int apkenv_audio_open(int frequency, enum AudioFormat format, int channels,
        int buffer, audio_callback_t callback, void *user_data);
void apkenv_audio_close();
void apkenv_audio_play();
void apkenv_audio_pause();

#endif /* APKENV_AUDIO_H */
