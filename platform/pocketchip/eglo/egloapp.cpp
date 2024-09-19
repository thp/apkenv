/**
 * eglo -- Minimalistic X11/EGL abstraction for Pocket C.H.I.P
 * Copyright (c) 2016, 2017, 2018 Thomas Perl <m@thp.io>.
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH
 * REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND
 * FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT,
 * INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM
 * LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR
 * OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
 * PERFORMANCE OF THIS SOFTWARE.
 **/

#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <ctime>

#include <unistd.h>

#include <vector>
#include <chrono>

#include <EGL/egl.h>
#include <GLES2/gl2.h>
#include <X11/keysym.h>

#include "eglo.h"

#define FAIL(...) do { fprintf(stderr, __VA_ARGS__); exit(1); } while (0)

namespace {

const char *vertex_src =
"attribute vec4 position;"
"attribute vec4 color;"
"varying mediump vec4 col;"
"void main() {"
"  gl_Position = position;"
"  col = color;"
"}";

const char *fragment_src =
"varying mediump vec4 col;"
"void main() {"
"  gl_FragColor = col;"
"}";

GLuint
load_shader(const char *shader_source, GLenum type)
{
   GLuint shader = glCreateShader(type);

   glShaderSource(shader, 1, &shader_source, nullptr);
   glCompileShader(shader);

   GLint length = 0;
   glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
   if (length > 0) {
       std::vector<char> buffer(length);
       glGetShaderInfoLog(shader, length, nullptr, buffer.data());
       printf("shader info: %s\n", buffer.data());
   }

   GLint success = GL_FALSE;
   glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
   if (success != GL_TRUE) {
       FAIL("Could not compile shader");
   }

   return shader;
}


typedef float f32;

struct v3 {
    v3(f32 x=0.f, f32 y=0.f, f32 z=0.f) : x(x), y(y), z(z) {}
    v3 operator*(float f) const { return v3{x*f, y*f, z*f}; }
    v3 operator*(const v3 &o) const { return v3{x*o.x, y*o.y, z*o.z}; }
    v3 operator+(const v3 &o) const { return v3{x+o.x, y+o.y, z+o.z}; }
    v3 &operator+=(const v3 &o) { x+=o.x; y+=o.y; z+=o.z; return *this; }

    f32 x; f32 y; f32 z;
};

struct v4 {
    v4(f32 r=0.f, f32 g=0.f, f32 b=0.f, f32 a=0.f) : r(r), g(g), b(b), a(a) {}
    v4 operator*(float f) const { return v4{r*f, g*f, b*f, a*f}; }
    v4 operator*(const v4 &o) const { return v4{r*o.r, g*o.g, b*o.b, a*o.a}; }
    v4 operator+(const v4 &o) const { return v4{r+o.r, g+o.g, b+o.b, a+o.a}; }
    v4 &operator*=(float f) { r*=f; g*=f; b*=f; a*=f; return *this; }
    v4 &operator+=(const v4 &o) { r+=o.r; g+=o.g; b+=o.b; a+=o.a; return *this; }
    float intensity() { return r + g + b; }

    f32 r; f32 g; f32 b; f32 a;
};

struct vtx {
    vtx(const v3 &pos=v3(), const v4 &col=v4()) : position(pos), color(col) {}

    v3 position; v4 color;
};

GLint position_loc, color_loc;
std::vector<vtx> vertices;
std::vector<vtx> vertices_tmp;
int vertices_offset = 0;

f32 rnd() { return f32(random() % 1000) / 1000.f; }

v4 target {rnd(), rnd(), rnd(), rnd()};
v4 base{ 0.6f, 0.f, 0.f, 0.f };

void egloapp_init(int width, int height)
{
    const char *egl_extensions = eglQueryString(eglGetDisplay(EGL_DEFAULT_DISPLAY), EGL_EXTENSIONS);
    const GLubyte *gl_extensions = glGetString(GL_EXTENSIONS);

    printf("= EGL extensions==\n%s\n\n= GL extensions =\n%s\n\n",
            egl_extensions, gl_extensions);

    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, load_shader(vertex_src, GL_VERTEX_SHADER));
    glAttachShader(shaderProgram, load_shader(fragment_src, GL_FRAGMENT_SHADER));
    glLinkProgram(shaderProgram);
    glUseProgram(shaderProgram);

    position_loc = glGetAttribLocation(shaderProgram, "position");
    color_loc = glGetAttribLocation(shaderProgram, "color");

    if (position_loc < 0 || color_loc < 0) {
        FAIL("Unable to get uniform location");
    }

    glEnableVertexAttribArray(position_loc);
    glEnableVertexAttribArray(color_loc);
    glViewport(0, 0, width, height);
    glClearColor(0.f, 0.f, 0.f, 0.f);
}

void egloapp_render()
{
    glClear(GL_COLOR_BUFFER_BIT);
    int off = vertices_offset;
    int max = 5000;
    if (vertices.size() > max && off < vertices.size() - max) {
        off = vertices.size() - max;
    }
    char *ptr = (char *)(vertices.data() + off);
    glVertexAttribPointer(position_loc, 3, GL_FLOAT, false, sizeof(vtx), ptr);
    glVertexAttribPointer(color_loc, 4, GL_FLOAT, false, sizeof(vtx), ptr + sizeof(vtx::position));
    glDrawArrays(GL_TRIANGLE_STRIP, 0, vertices.size() - off);
}

void egloapp_update()
{
    static int i = 0;
    if (i++ % 100 == 0) {
        target = { rnd(), rnd(), rnd(), rnd() };
    }

    for (auto it = vertices.begin() + vertices_offset; it != vertices.end(); ++it) {
        it->color *= 0.99f;
        it->position += v3{0.5f-rnd(), 0.5f-rnd(), 0.5f-rnd()} * 0.05f;
    }

    auto it = vertices.begin() + vertices_offset;
    while (it != vertices.end() && it->color.intensity() < 0.03f) {
        vertices_offset++;
        ++it;
    }

    if (vertices_offset > 1000) {
        vertices_tmp.clear();
        vertices_tmp.reserve(vertices.size() - vertices_offset);
        vertices_tmp.insert(vertices_tmp.begin(), vertices.begin() + vertices_offset, vertices.end());
        vertices.swap(vertices_tmp);
        vertices_offset = 0;
    }

    float alpha = 0.9f;
    base *= alpha;
    base += target * (1.f - alpha);
}

};

int main(int argc, char *argv[])
{
    int width = 0, height = 0;

    if (eglo_init(&width, &height, 2) != 0) {
        FAIL("Could not create window");
    }

    egloapp_init(width, height);

    bool quit = false;
    while (!quit) {
        EgloEvent e;
        bool any_event = false;
        while (eglo_next_event(&e)) {
            any_event = true;
            switch (e.type) {
                case EGLO_QUIT:
                    quit = true;
                    break;
                case EGLO_KEY_DOWN:
                    if (e.key.key == XK_Escape) {
                        quit = true;
                    }
                    break;
                case EGLO_KEY_UP:
                    break;
                case EGLO_MOUSE_DOWN:
                    break;
                case EGLO_MOUSE_MOTION:
                    for (int x=0; x<40; x++) {
                        vertices.emplace_back(v3{
                            0.05f * (rnd() - 0.5f) + 2.f * (float(e.mouse.x) / float(width)) - 1.f,
                            0.05f * (rnd() - 0.5f) + 1.f - 2.f * float(e.mouse.y) / float(height),
                            0.f,
                        }, base + v4{rnd(), rnd(), rnd(), 1.f} * 0.2f);
                    }
                    break;
                case EGLO_MOUSE_UP:
                    break;
            }
        }

        if (!any_event) {
            static auto started = std::chrono::steady_clock::now();
            float now = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - started).count();
            float x = fabsf(sinf(now*0.0007f));
            float y = fabsf(cosf(now*0.001f));
            vertices.emplace_back(v3{
                    0.05f * (rnd() - 0.5f) + 2.f * x - 1.f,
                    0.05f * (rnd() - 0.5f) + 1.f - 2.f * y,
                    0.f,
            }, base + v4{rnd(), rnd(), rnd(), 1.f} * 0.2f);
        }

        egloapp_render();
        egloapp_update();

        eglo_swap_buffers();
    }

    eglo_quit();

    return 0;
}
