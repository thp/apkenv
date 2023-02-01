/**
 * apkenv
 * Copyright (c) 2012, 2014, 2016, 2023 Thomas Perl <m@thp.io>
 * Copyright (c) 2014 Franz-Josef Anton Friedrich Haider <f_haider@gmx.at>
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


#include "../apkenv.h"
#include "../compat/gles_wrappers.h"
#include "../mixer/mixer.h"
#include "../audio/audio.h"

#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <stdbool.h>
#include <inttypes.h>
#include <sys/wait.h>
#include <sys/errno.h>
#include <pthread.h>

#include <GL/osmesa.h>
#include <EGL/egl.h>

extern struct GlobalState global;

struct PlatformPriv {
    OSMesaContext ctx;

    uint32_t *pixels;
    int width;
    int height;

    struct {
        int pid;
        int write_fd;
        int read_fd;
        int audio_fd;
        int audio_ack_fd;
    } hostui;
};

static struct PlatformPriv priv;

static const char *
hostui_rpc(const char *cmd)
{
    size_t len = strlen(cmd);
    size_t pos = 0;
    while (pos < len) {
        ssize_t res = TEMP_FAILURE_RETRY(write(priv.hostui.write_fd, cmd + pos, len - pos));
        if (res < 0) {
            fprintf(stderr, "Could not write to host UI: %s\n", strerror(errno));
            exit(1);
            return NULL;
        }

        pos += res;
    }

    char nl = '\n';
    if (TEMP_FAILURE_RETRY(write(priv.hostui.write_fd, &nl, 1)) != 1) {
        fprintf(stderr, "Could not write to host UI: %s\n", strerror(errno));
        exit(1);
        return NULL;
    }

    static char tmp[1024];

    memset(tmp, 0, sizeof(tmp));
    pos = 0;

    while (pos < sizeof(tmp) - 1) {
        ssize_t res = TEMP_FAILURE_RETRY(read(priv.hostui.read_fd, tmp + pos, 1));
        if (res < 0) {
            fprintf(stderr, "Could not read from host UI: %s\n", strerror(errno));
            exit(1);
            return NULL;
        }

        if (tmp[pos] == '\n') {
            break;
        }

        ++pos;
    }

    tmp[pos] = '\0';

    return tmp;
}

static const char *
hostui_rpc_printf(const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    char *tmp;
    vasprintf(&tmp, fmt, args);
    const char *result = hostui_rpc(tmp);
    free(tmp);
    va_end(args);
    return result;
}

static void
hostui_tx_bytes(const char *data, size_t len)
{
    size_t pos = 0;
    while (pos < len) {
        ssize_t res = TEMP_FAILURE_RETRY(write(priv.hostui.write_fd, data + pos, len - pos));
        if (res < 0) {
            fprintf(stderr, "Could not write to host UI: %s\n", strerror(errno));
            exit(1);
        }

        pos += res;
    }
}

static int
hostui_rpc_scanf(const char *cmd, const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);

    const char *reply = hostui_rpc(cmd);
    int res = vsscanf(reply, fmt, args);

    va_end(args);

    return res;
}

static void
osmesa_data_sink_gles1(const void *data, size_t len, void *user_data)
{
    hostui_rpc_printf("gles1 %zu", len);
    hostui_tx_bytes(data, len);
}

struct MixerSound {
    uint64_t handle;
};

struct MixerMusic {
    uint64_t handle;
};

static int
hostui_mixer_open(struct Mixer *mixer)
{
    const char *result = hostui_rpc_printf("mixer open %d %d %d %d",
            mixer->config.frequency, mixer->config.format,
            mixer->config.channels, mixer->config.buffer);

    return strcmp(result, "ok") == 0;
}

static void
hostui_mixer_close(struct Mixer *mixer)
{
    hostui_rpc("mixer close");
}

static struct MixerMusic *
hostui_mixer_load_music(struct Mixer *mixer, const char *filename)
{
    const char *ptr = hostui_rpc_printf("mixer load-music %s", filename);

    struct MixerMusic *music = malloc(sizeof(struct MixerMusic));
    music->handle = strtoull(ptr, NULL, 16);
    return music;
}

static struct MixerMusic *
hostui_mixer_load_music_buffer(struct Mixer *mixer,
        const char *buffer, size_t size)
{
    fprintf(stderr, "stub load music buffer %p %zu\n", buffer, size);
    return NULL;
}

static struct MixerSound *
hostui_mixer_load_sound(struct Mixer *mixer, const char *filename)
{
    fprintf(stderr, "stub load sound %s\n", filename);
    return NULL;
}

static struct MixerSound *
hostui_mixer_load_sound_buffer(struct Mixer *mixer,
            const char *buffer, size_t size)
{
    hostui_rpc_printf("mixer load-sound-buffer %d", (int)size);
    hostui_tx_bytes(buffer, size);

    const char *ptr = hostui_rpc("retrieve-pointer");

    struct MixerSound *sound = malloc(sizeof(struct MixerSound));

    sound->handle = strtoull(ptr, NULL, 16);
    return sound;
}

static void
hostui_mixer_free_music(struct Mixer *mixer, struct MixerMusic *music)
{
    hostui_rpc_printf("mixer free-music %" PRIx64, music->handle);
    free(music);
}

static void
hostui_mixer_free_sound(struct Mixer *mixer, struct MixerSound *sound)
{
    hostui_rpc_printf("mixer free-sound %" PRIx64, sound->handle);
    free(sound);
}

static void
hostui_mixer_play_music(struct Mixer *mixer, struct MixerMusic *music, int do_loop)
{
    hostui_rpc_printf("mixer play-music %" PRIx64 " %d", music->handle, do_loop);
}

static void
hostui_mixer_play_sound(struct Mixer *mixer, struct MixerSound *sound, int do_loop)
{
    hostui_rpc_printf("mixer play-sound %" PRIx64 " %d", sound->handle, do_loop);
}

static void
hostui_mixer_stop_music(struct Mixer *mixer, struct MixerMusic *music)
{
    hostui_rpc_printf("mixer stop-music %" PRIx64, music->handle);
}

static void
hostui_mixer_stop_sound(struct Mixer *mixer, struct MixerSound *sound)
{
    hostui_rpc_printf("mixer stop-sound %" PRIx64, sound->handle);
}

#define HOSTUI_MIXER_NOT_IMPLEMENTED() fprintf(stderr, "%s: Not implemented\n", __func__); exit(1)

static int
hostui_mixer_get_sound_channel(struct Mixer *mixer, struct MixerSound *sound)
{
    HOSTUI_MIXER_NOT_IMPLEMENTED();
}

static void
hostui_mixer_set_sound_channel(struct Mixer *mixer, struct MixerSound *sound, int channel)
{
    HOSTUI_MIXER_NOT_IMPLEMENTED();
}

static void
hostui_mixer_volume_music(struct Mixer *mixer, struct MixerMusic *music, float volume)
{
    hostui_rpc_printf("mixer volume-music %" PRIx64 " %d", music->handle, (int)(100.f * volume));
}

static void
hostui_mixer_volume_sound(struct Mixer *mixer, struct MixerSound *sound, float volume)
{
    hostui_rpc_printf("mixer volume-sound %" PRIx64 " %d", sound->handle, (int)(100.f * volume));
}

static void
hostui_mixer_lame_resample_44100_32000(struct Mixer *mixer, struct MixerSound *sound)
{
    HOSTUI_MIXER_NOT_IMPLEMENTED();
}

static struct Mixer
hostui_mixer = {
    hostui_mixer_open,
    hostui_mixer_close,
    hostui_mixer_load_music,
    hostui_mixer_load_music_buffer,
    hostui_mixer_load_sound,
    hostui_mixer_load_sound_buffer,
    hostui_mixer_free_music,
    hostui_mixer_free_sound,
    hostui_mixer_play_music,
    hostui_mixer_play_sound,
    hostui_mixer_stop_music,
    hostui_mixer_stop_sound,
    hostui_mixer_get_sound_channel,
    hostui_mixer_set_sound_channel,
    hostui_mixer_volume_music,
    hostui_mixer_volume_sound,
    hostui_mixer_lame_resample_44100_32000,
    { 0 },
};

static pthread_t
audio_playback_thread;

static struct AudioConfig
audio_playback_config;

static bool
audio_playback_thread_running = false;

static bool
audio_playback_paused = false;

static void *
audio_playback_thread_proc(void *)
{
    char *cmd;
    asprintf(&cmd, "configure %d %d %d %d\n",
            audio_playback_config.frequency, audio_playback_config.format,
            audio_playback_config.channels, audio_playback_config.buffer);

    size_t len = strlen(cmd);
    size_t pos = 0;
    while (pos < len) {
        ssize_t res = TEMP_FAILURE_RETRY(write(priv.hostui.audio_fd, cmd + pos, len - pos));
        if (res < 0) {
            fprintf(stderr, "Could not write to host UI: %s\n", strerror(errno));
            exit(1);
            return NULL;
        }

        pos += res;
    }

    free(cmd);

    size_t buffer_len = sizeof(uint16_t) * audio_playback_config.buffer * audio_playback_config.channels;
    char *buffer = malloc(buffer_len);

    while (audio_playback_thread_running) {
        if (!audio_playback_paused) {
            audio_playback_config.callback(audio_playback_config.user_data, buffer, buffer_len);

            size_t len = buffer_len;
            size_t pos = 0;
            while (pos < len) {
                ssize_t res = TEMP_FAILURE_RETRY(write(priv.hostui.audio_fd, buffer + pos, len - pos));
                if (res == 0) {
                    // EOF
                    break;
                } else if (res < 0) {
                    fprintf(stderr, "Could not write to host UI: %s\n", strerror(errno));
                    exit(1);
                    return NULL;
                }

                pos += res;
            }

            char tmp;
            ssize_t res = TEMP_FAILURE_RETRY(read(priv.hostui.audio_ack_fd, &tmp, 1));
            if (res != 1) {
                fprintf(stderr, "Could not wait for audio ack\n");
                exit(1);
            }
        } else {
            // TODO: Block and wait for condition variable
            usleep(100000);
        }
    }

    free(buffer);

    return NULL;
}


static int
hostui_audio_open(struct Audio *audio)
{
    audio_playback_config = audio->config;
    audio_playback_thread_running = true;

    return (pthread_create(&audio_playback_thread, NULL, audio_playback_thread_proc, NULL) == 0);
}

static void
hostui_audio_close(struct Audio *audio)
{
    if (audio_playback_thread_running) {
        audio_playback_thread_running = false;

        void *retval = NULL;
        int res = pthread_join(audio_playback_thread, &retval);
        if (res != 0) {
            fprintf(stderr, "pthread_join() -> %d\n", res);
            exit(1);
        }
    }
}

static void
hostui_audio_play(struct Audio *audio)
{
    audio_playback_paused = false;
}

static void
hostui_audio_pause(struct Audio *audio)
{
    audio_playback_paused = true;
}

static struct Audio
hostui_audio = {
    hostui_audio_open,
    hostui_audio_close,
    hostui_audio_play,
    hostui_audio_pause,
    { 0 },
};


static int
osmesa_init(int gles_version)
{
    // Create Host UI connection

    int to_host_fds[2];

    int res = pipe(to_host_fds);
    if (res != 0) {
        fprintf(stderr, "Could not create pipe\n");
        exit(1);
    }

    int from_host_fds[2];
    res = pipe(from_host_fds);
    if (res != 0) {
        fprintf(stderr, "Could not create pipe\n");
        exit(1);
    }

    int audio_fds[2];
    res = pipe(audio_fds);
    if (res != 0) {
        fprintf(stderr, "Could not create pipe\n");
        exit(1);
    }

    int audio_ack_fds[2];
    res = pipe(audio_ack_fds);
    if (res != 0) {
        fprintf(stderr, "Could not create pipe\n");
        exit(1);
    }

    int pid = fork();
    if (pid == 0) {
        // in child
        close(to_host_fds[1]);
        close(from_host_fds[0]);

        close(audio_fds[1]);
        close(audio_ack_fds[0]);

        int res = dup2(to_host_fds[0], STDIN_FILENO);
        if (res == -1) {
            printf("child could not dup2() -> stdin (%s)\n", strerror(errno));
            exit(1);
        }

        res = dup2(from_host_fds[1], STDOUT_FILENO);
        if (res == -1) {
            printf("child could not dup2() -> stdout (%s)\n", strerror(errno));
            exit(1);
        }

        res = dup2(audio_fds[0], 3);
        if (res == -1) {
            printf("child could not dup2() -> audio (%s)\n", strerror(errno));
            exit(1);
        }

        res = dup2(audio_ack_fds[1], 4);
        if (res == -1) {
            printf("child could not dup2() -> audio (%s)\n", strerror(errno));
            exit(1);
        }

        res = execl("./hostui", "./hostui", NULL);
        printf("exec failed\n");
        exit(1);
    } else {
        // in parent
        close(to_host_fds[0]);
        close(from_host_fds[1]);

        close(audio_fds[0]);
        close(audio_ack_fds[1]);

        priv.hostui.pid = pid;
        priv.hostui.write_fd = to_host_fds[1];
        priv.hostui.read_fd = from_host_fds[0];
        priv.hostui.audio_fd = audio_fds[1];
        priv.hostui.audio_ack_fd = audio_ack_fds[0];
    }

    if (hostui_rpc_scanf("size", "%d %d", &priv.width, &priv.height) != 2) {
        fprintf(stderr, "Could not get window size\n");
        exit(1);
    }

    priv.ctx = OSMesaCreateContextExt(OSMESA_RGBA, 16, 0, 0, NULL);

    priv.pixels = malloc(sizeof(uint32_t) * priv.width * priv.height);

    OSMesaMakeCurrent(priv.ctx, priv.pixels, GL_UNSIGNED_BYTE, priv.width, priv.height);

    if (global.use_gles_serialize) {
        gles_serialize_set_sink(osmesa_data_sink_gles1, NULL);
    }

    apkenv_audio_register(&hostui_audio);
    apkenv_mixer_register(&hostui_mixer);

    return 1;
}

static const char *
osmesa_get_path(enum PlatformPath which)
{
    switch (which) {
        case PLATFORM_PATH_INSTALL_DIRECTORY:
            return NULL; /* install not supported */
        case PLATFORM_PATH_DATA_DIRECTORY:
            return "./data/";
        case PLATFORM_PATH_MODULE_DIRECTORY:
            return "./staging";
        default:
            return NULL;
    }
}

static void
osmesa_get_size(int *width, int *height)
{
    if (width) {
        *width = priv.width;
    }
    if (height) {
        *height = priv.height;
    }
}

static bool
starts_with(const char *haystack, const char *prefix)
{
    return strncmp(haystack, prefix, strlen(prefix)) == 0;
}

static int
osmesa_input_update(struct SupportModule *module)
{
    while (true) {
        const char *reply = hostui_rpc("poll");

        int action, x, y;
        if (strcmp(reply, "none") == 0) {
            break;
        } else if (strcmp(reply, "quit") == 0) {
            return 1;
        } else if (sscanf(reply, "mouse %d %d %d", &action, &x, &y) == 3) {
            module->input(module, action, x, y, 0);
        } else {
            printf("reply: >%s<\n", reply);
            fprintf(stderr, "unhandled: >%s<\n", reply);
            exit(1);
        }
    }

    return 0;
}

static int
osmesa_get_orientation(void)
{
    return (priv.width > priv.height) ? ORIENTATION_LANDSCAPE : ORIENTATION_PORTRAIT;
}

static void
osmesa_request_text_input(int is_password, const char *text,
        text_callback_t callback, void *user_data)
{
    fprintf(stderr, "XXX: Request text input not implemented!\n");

    // Admit failure right away
    callback(NULL, user_data);
}

static void
osmesa_update()
{
    if (!global.use_gles_serialize) {
        glFinish();
        glFlush();

        hostui_rpc("blit");
        hostui_tx_bytes(priv.pixels, sizeof(uint32_t) * priv.width * priv.height);
    }

    hostui_rpc("swap");
}

static void
osmesa_exit()
{
    hostui_rpc("exit");

    close(priv.hostui.write_fd);
    close(priv.hostui.read_fd);
    close(priv.hostui.audio_fd);
    close(priv.hostui.audio_ack_fd);

    printf("Waiting for Host UI to close...\n");
    int status;
    if (waitpid(priv.hostui.pid, &status, 0) != -1 && WIFEXITED(status)) {
        printf("Host UI exit status: %d\n", WEXITSTATUS(status));
    } else {
        printf("waitpid() on hostui failed: %s\n", strerror(errno));
        exit(1);
    }

    free(priv.pixels);

    OSMesaDestroyContext(priv.ctx);
}

typedef void (*eglGetProcAddressReturnType)(void);

eglGetProcAddressReturnType
eglGetProcAddress(const char *procname)
{
    OSMESAproc result = OSMesaGetProcAddress(procname);

    fprintf(stderr, "eglGetProcAddress(procname=\"%s\") -> %p\n", procname, result);

    return result;
}

const char *
eglQueryString(EGLDisplay dpy, EGLint name)
{
    fprintf(stderr, "eglQueryString(dpy=%p, name=0x%08x)\n", dpy, name);

    switch (name) {
        case EGL_CLIENT_APIS:
            return "OpenGL_ES ";
        case EGL_VENDOR:
            return "apkenv";
        case EGL_VERSION:
            return "1.5 apkenv-osmesa";
        case EGL_EXTENSIONS:
            return "";
        default:
            break;
    }

    return NULL;
}

struct PlatformSupport platform_support = {
    osmesa_init,
    osmesa_get_path,
    osmesa_get_size,
    osmesa_input_update,
    osmesa_get_orientation,
    osmesa_request_text_input,
    osmesa_update,
    osmesa_exit,
};
