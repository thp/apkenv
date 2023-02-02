
/**
 * hostui: Host User Interface Process for apkenv-in-qemu
 * Copyright (c) 2023, Thomas Perl <m@thp.io>
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


#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/errno.h>
#include <pthread.h>

#include <SDL.h>
#include <SDL_opengl.h>
#include <ao/ao.h>

#include "platform/common/sdl_mixer_impl.h"

#include "compat/gen/gles_serialize.c"

#define ACTION_DOWN 0
#define ACTION_UP 1
#define ACTION_MOVE 2


static void
apkenv_reply(const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    vprintf(fmt, args);
    va_end(args);
    printf("\n");
    fflush(stdout);
}

static void *
alloc_and_read_from_stdin(size_t len)
{
    uint8_t *data = malloc(len);
    size_t pos = 0;
    while (pos < len) {
        ssize_t res = TEMP_FAILURE_RETRY(read(STDIN_FILENO, data + pos, len - pos));
        if (res == 0) {
            free(data);
            return NULL;
        }

        if (res < 0) {
            fprintf(stderr, "Could not read data from stdin: %s\n", strerror(errno));
            exit(1);
        }

        pos += res;
    }

    return data;
}

static void (*glOrtho_ptr)(GLdouble, GLdouble, GLdouble, GLdouble, GLdouble, GLdouble) = NULL;

static void
my_glOrthof(GLfloat left, GLfloat right, GLfloat bottom, GLfloat top, GLfloat nearVal, GLfloat farVal)
{
    glOrtho_ptr(left, right, bottom, top, nearVal, farVal);
}

static void *
get_proc_address(const char *name)
{
    if (strcmp(name, "glOrthof") == 0) {
        glOrtho_ptr = SDL_GL_GetProcAddress("glOrtho");
        return my_glOrthof;
    }

    void *result = SDL_GL_GetProcAddress(name);
    if (!result) {
        fprintf(stderr, "Missing GL function in host GL: %s\n", name);
        exit(1);
    }
    return result;
}

static void *
audio_thread_proc(void *user_data)
{
    bool *running = user_data;

    const int audio_fd = 3;
    const int audio_ack_fd = 4;

    char cmd[1024];

    memset(cmd, 0, sizeof(cmd));
    int pos = 0;

    while (pos < sizeof(cmd)) {
        ssize_t res = TEMP_FAILURE_RETRY(read(audio_fd, cmd + pos, 1));
        if (res == 0) {
            // EOF
            cmd[0] = '\0';
            break;
        } else if (res < 0) {
            fprintf(stderr, "Could not read from audio fd: %s\n", strerror(errno));
            exit(1);
        }

        if (cmd[pos] == '\n') {
            cmd[pos] = '\0';
            break;
        }

        ++pos;

        if (pos == sizeof(cmd)) {
            fprintf(stderr, "Command too long\n");
            exit(1);
        }
    }

    if (strlen(cmd) == 0) {
        return NULL;
    }

    int frequency, format, channels, buffer_size;
    int res = sscanf(cmd, "configure %d %d %d %d", &frequency, &format, &channels, &buffer_size);
    if (res != 4) {
        fprintf(stderr, "audio thread command fail: >%s<\n", cmd);
        exit(1);
    }

    ao_initialize();
    ao_sample_format fmt;
    memset(&fmt, 0, sizeof(fmt));

    if (format == AudioFormat_S16SYS) {
        fmt.bits = 16;
        fmt.byte_format = AO_FMT_NATIVE;
    } else {
        fprintf(stderr, "Unhandled format: %d\n", format);
        exit(1);
    }

    fmt.channels = channels;
    fmt.rate = frequency;

    ao_device *aodev = ao_open_live(ao_default_driver_id(), &fmt, NULL);
    if (!aodev) {
        fprintf(stderr, "Could not open libao device\n");
        exit(1);
    }

    size_t buffer_len = sizeof(int16_t) * channels * buffer_size;
    char *buffer = malloc(buffer_len);

    while (*running) {
        size_t buffer_pos = 0;
        while (buffer_pos < buffer_len) {
            ssize_t res = TEMP_FAILURE_RETRY(read(audio_fd, buffer + buffer_pos, buffer_len - buffer_pos));

            if (res == 0) {
                // EOF
                break;
            } else if (res < 0) {
                fprintf(stderr, "Could not read from audio fd: %s\n", strerror(errno));
                exit(1);
            }

            buffer_pos += res;
        }

        if (buffer_pos == buffer_len) {
            ao_play(aodev, buffer, buffer_pos);

            char ack = 42;
            ssize_t res = TEMP_FAILURE_RETRY(write(audio_ack_fd, &ack, 1));
            if (res != 1) {
                fprintf(stderr, "Could not ACK audio playback.\n");
                exit(1);
            }
        }
    }

    ao_close(aodev);
    ao_shutdown();

    free(buffer);

    return NULL;
}

int
main(int argc, char *argv[])
{
    SDL_Init(SDL_INIT_EVERYTHING);

    int width = 800;
    int height = 480;

    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);

    SDL_Window *win = SDL_CreateWindow("apkenv", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_OPENGL);

    SDL_GetWindowSize(win, &width, &height);

    SDL_GLContext ctx = SDL_GL_CreateContext(win);

    gles_serialize_install_decoder(get_proc_address);

    void *pointer_mailbox = NULL;

    bool running = true;

    pthread_t audio_thread;
    int res = pthread_create(&audio_thread, NULL, audio_thread_proc, &running);

    while (running) {
        char cmd[1024];

        memset(cmd, 0, sizeof(cmd));
        int pos = 0;

        while (pos < sizeof(cmd)) {
            ssize_t res = TEMP_FAILURE_RETRY(read(STDIN_FILENO, cmd + pos, 1));
            if (res < 0) {
                fprintf(stderr, "Could not read from stdin: %s\n", strerror(errno));
                exit(1);
            }

            if (cmd[pos] == '\n') {
                cmd[pos] = '\0';
                break;
            }

            ++pos;

            if (pos == sizeof(cmd)) {
                fprintf(stderr, "Command too long\n");
                exit(1);
            }
        }

        if (strcmp(cmd, "size") == 0) {
            apkenv_reply("%d %d", width, height);
        } else if (strcmp(cmd, "exit") == 0) {
            running = false;

            apkenv_reply("ok");
        } else if (strcmp(cmd, "blit") == 0) {
            apkenv_reply("ok");

            size_t len = sizeof(uint32_t) * width * height;
            uint32_t *pixels = alloc_and_read_from_stdin(len);
            if (pixels == NULL) {
                continue;
            }

            GLuint tex;
            glGenTextures(1, &tex);

            glBindTexture(GL_TEXTURE_2D, tex);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

            glEnable(GL_TEXTURE_2D);
            glBegin(GL_TRIANGLE_STRIP);
            glTexCoord2f(0.f, 0.f); glVertex2f(-1.f, -1.f);
            glTexCoord2f(0.f, 1.f); glVertex2f(-1.f, +1.f);
            glTexCoord2f(1.f, 0.f); glVertex2f(+1.f, -1.f);
            glTexCoord2f(1.f, 1.f); glVertex2f(+1.f, +1.f);
            glEnd();

            glFinish();

            glDeleteTextures(1, &tex);

            free(pixels);
        } else if (strcmp(cmd, "swap") == 0) {
            apkenv_reply("ok");

            SDL_GL_SwapWindow(win);
        } else if (strcmp(cmd, "poll") == 0) {
            SDL_Event e;
            bool handled = false;
            while (!handled) {
                if (SDL_PollEvent(&e)) {
                    switch (e.type) {
                        case SDL_QUIT:
                            apkenv_reply("quit");
                            handled = true;
                            break;
                        case SDL_MOUSEBUTTONDOWN:
                            apkenv_reply("mouse %d %d %d", ACTION_DOWN, e.button.x, e.button.y);
                            handled = true;
                            break;
                        case SDL_MOUSEBUTTONUP:
                            apkenv_reply("mouse %d %d %d", ACTION_UP, e.button.x, e.button.y);
                            handled = true;
                            break;
                        case SDL_MOUSEMOTION:
                            if (e.motion.state) {
                                apkenv_reply("mouse %d %d %d", ACTION_MOVE, e.motion.x, e.motion.y);
                                handled = true;
                            }
                            break;
                        default:
                            break;
                    }
                } else {
                    apkenv_reply("none");
                    handled = true;
                }
            }
        } else if (strncmp(cmd, "gles1 ", 6) == 0) {
            apkenv_reply("ok");

            size_t len;
            if (sscanf(cmd, "gles1 %zu", &len) != 1) {
                fprintf(stderr, "Could not read cmd size\n");
                exit(1);
            }

            uint32_t *data = alloc_and_read_from_stdin(len);
            if (data == NULL) {
                continue;
            }

            gles_serialize_decode(data, len);

            free(data);
        } else if (strncmp(cmd, "mixer ", 6) == 0) {
            const char *subcmd = cmd + 6;

            int frequency, format, channels, buffer;
            if (sscanf(subcmd, "open %d %d %d %d", &frequency, &format, &channels, &buffer) == 4) {
                memset(&sdl_mixer->config, 0, sizeof(sdl_mixer->config));
                sdl_mixer->config.frequency = frequency;
                sdl_mixer->config.format = format;
                sdl_mixer->config.channels = channels;
                sdl_mixer->config.buffer = buffer;

                int res = sdl_mixer->open(sdl_mixer);
                apkenv_reply(res ? "ok" : "error");
            } else if (strcmp(subcmd, "close") == 0) {
                sdl_mixer->close(sdl_mixer);

                apkenv_reply("ok");
            } else if (strncmp(subcmd, "load-music ", 11) == 0) {
                const char *filename = subcmd + 11;

                void *result = sdl_mixer->load_music(sdl_mixer, filename);

                apkenv_reply("%p", result);
            } else if (strncmp(subcmd, "free-music ", 11) == 0) {
                const char *ptr = subcmd + 11;
                uint64_t value = strtoull(ptr, NULL, 16);
                void *music = (void *)(intptr_t)value;

                sdl_mixer->free_music(sdl_mixer, music);

                apkenv_reply("ok");
            } else if (strncmp(subcmd, "free-sound ", 11) == 0) {
                const char *ptr = subcmd + 11;
                uint64_t value = strtoull(ptr, NULL, 16);
                void *sound = (void *)(intptr_t)value;

                sdl_mixer->free_sound(sdl_mixer, sound);

                apkenv_reply("ok");
            } else if (strncmp(subcmd, "play-music ", 11) == 0) {
                const char *ptr = subcmd + 11;

                uint64_t value;
                int do_loop;
                if (sscanf(ptr, "%" SCNx64 " %d", &value, &do_loop) != 2) {
                    fprintf(stderr, "Invalid format in play-music: >%s<\n", ptr);
                    exit(1);
                }

                void *music = (void *)(intptr_t)value;

                sdl_mixer->play_music(sdl_mixer, music, do_loop);

                apkenv_reply("ok");
            } else if (strncmp(subcmd, "stop-music ", 11) == 0) {
                const char *ptr = subcmd + 11;

                uint64_t value;
                if (sscanf(ptr, "%" SCNx64, &value) != 1) {
                    fprintf(stderr, "Invalid format in stop-music: >%s<\n", ptr);
                    exit(1);
                }

                void *music = (void *)(intptr_t)value;

                sdl_mixer->stop_music(sdl_mixer, music);

                apkenv_reply("ok");
            } else if (strncmp(subcmd, "stop-sound ", 11) == 0) {
                const char *ptr = subcmd + 11;

                uint64_t value;
                if (sscanf(ptr, "%" SCNx64, &value) != 1) {
                    fprintf(stderr, "Invalid format in stop-sound: >%s<\n", ptr);
                    exit(1);
                }

                void *sound = (void *)(intptr_t)value;

                sdl_mixer->stop_sound(sdl_mixer, sound);

                apkenv_reply("ok");
            } else if (strncmp(subcmd, "play-sound ", 11) == 0) {
                const char *ptr = subcmd + 11;

                uint64_t value;
                int do_loop;
                if (sscanf(ptr, "%" SCNx64 " %d", &value, &do_loop) != 2) {
                    fprintf(stderr, "Invalid format in play-sound: >%s<\n", ptr);
                    exit(1);
                }

                void *sound = (void *)(intptr_t)value;

                sdl_mixer->play_sound(sdl_mixer, sound, do_loop);

                apkenv_reply("ok");
            } else if (strncmp(subcmd, "volume-music ", 13) == 0) {
                const char *ptr = subcmd + 13;

                uint64_t value;
                int volume;
                if (sscanf(ptr, "%" SCNx64 " %d", &value, &volume) != 2) {
                    fprintf(stderr, "Invalid format in volume-music: >%s<\n", ptr);
                    exit(1);
                }

                void *music = (void *)(intptr_t)value;

                sdl_mixer->volume_music(sdl_mixer, music, 0.01f * volume);

                apkenv_reply("ok");
            } else if (strncmp(subcmd, "volume-sound ", 13) == 0) {
                const char *ptr = subcmd + 13;

                uint64_t value;
                int volume;
                if (sscanf(ptr, "%" SCNx64 " %d", &value, &volume) != 2) {
                    fprintf(stderr, "Invalid format in volume-sound: >%s<\n", ptr);
                    exit(1);
                }

                void *sound = (void *)(intptr_t)value;

                sdl_mixer->volume_sound(sdl_mixer, sound, 0.01f * volume);

                apkenv_reply("ok");
            } else if (strncmp(subcmd, "load-sound-buffer ", 18) == 0) {
                const char *str = subcmd + 18;

                int len;
                if (sscanf(str, "%d", &len) != 1) {
                    fprintf(stderr, "Could not parse sound buffer length\n");
                    exit(1);
                }

                apkenv_reply("ok");

                void *buf = alloc_and_read_from_stdin(len);
                if (buf == NULL) {
                    continue;
                }

                pointer_mailbox = sdl_mixer->load_sound_buffer(sdl_mixer, buf, len);

                free(buf);
            } else {
                fprintf(stderr, "Invalid mixer command: >%s<\n", subcmd);
                exit(1);
            }
        } else if (strcmp(cmd, "retrieve-pointer") == 0) {
            if (pointer_mailbox == NULL) {
                fprintf(stderr, "Could not retrieve pointer from pointer mailbox\n");
                exit(1);
            }

            apkenv_reply("%p", pointer_mailbox);
            pointer_mailbox = NULL;
        } else {
            fprintf(stderr, "Invalid command: >%s<\n", cmd);
            exit(1);
        }
    }

    pthread_join(audio_thread, NULL);

    SDL_GL_DeleteContext(ctx);

    SDL_DestroyWindow(win);

    SDL_Quit();

    return 0;
}
