
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

#include <SDL.h>
#include <SDL_opengl.h>

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

int
main(int argc, char *argv[])
{
    SDL_Init(SDL_INIT_EVERYTHING);

    int width = 640;
    int height = 480;

    SDL_Window *win = SDL_CreateWindow("apkenv", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_OPENGL);

    SDL_GetWindowSize(win, &width, &height);

    SDL_GLContext ctx = SDL_GL_CreateContext(win);

    bool running = true;
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
            uint32_t *pixels = malloc(len);
            size_t pos = 0;
            while (pos < len) {
                ssize_t res = TEMP_FAILURE_RETRY(read(STDIN_FILENO, ((uint8_t *)pixels) + pos, len - pos));
                if (res < 0) {
                    fprintf(stderr, "Could not read pixels from stdin: %s\n", strerror(errno));
                    exit(1);
                }

                pos += res;
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
        } else {
            fprintf(stderr, "Invalid command: >%s<\n", cmd);
            exit(1);
        }
    }

    SDL_GL_DeleteContext(ctx);

    SDL_DestroyWindow(win);

    SDL_Quit();

    return 0;
}
