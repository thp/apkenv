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


#include "eglo.h"

#include <cstdio>
#include <cstdlib>

#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <X11/Xutil.h>

#include <EGL/egl.h>
#define GL_GLEXT_PROTOTYPES
#include <GLES/gl.h>
#include <GLES/glext.h>
#include <GLES2/gl2.h>

#define FAIL(...) do { fprintf(stderr, __VA_ARGS__); exit(1); } while (0)

namespace {

class X11Window {
public:
    X11Window(int width, int height, int scale);
    ~X11Window();

    int process_event(EgloEvent *event);
    void swap();

    int get_width() { return width * scale; }
    int get_height() { return height * scale; }

protected:
    int width;
    int height;
    int scale;
    Display *dpy;
    Window win;
};

X11Window::X11Window(int width, int height, int scale)
    : width(width)
    , height(height)
    , scale(scale)
    , dpy()
    , win()
{
   dpy = XOpenDisplay(":0");
   if (dpy == nullptr) {
       FAIL("cannot connect to X server");
   }

   Window root = DefaultRootWindow(dpy);

   XSetWindowAttributes swa;
   swa.event_mask = (ExposureMask | PointerMotionMask | KeyPressMask | KeyReleaseMask | ButtonPressMask |
           ButtonReleaseMask | Button1MotionMask);

   win = XCreateWindow(dpy, root, 0, 0, width, height, 0, CopyFromParent,
           InputOutput, CopyFromParent, CWEventMask, &swa);

   XSetWindowAttributes xattr;
   xattr.override_redirect = False;
   XChangeWindowAttributes(dpy, win, CWOverrideRedirect, &xattr);

   Atom atom = XInternAtom(dpy, "_NET_WM_STATE_FULLSCREEN", True);
   XChangeProperty(dpy, win, XInternAtom(dpy, "_NET_WM_STATE", True),
           XA_ATOM, 32, PropModeReplace, (unsigned char *)&atom, 1);

   XWMHints hints;
   hints.input = True;
   hints.flags = InputHint;
   XSetWMHints(dpy, win, &hints);

   XMapWindow(dpy, win);
   XStoreName(dpy, win, "EGLO");
   XMoveResizeWindow(dpy, win, 0, 0, width, height);
}

X11Window::~X11Window()
{
   XDestroyWindow(dpy, win);
   //XCloseDisplay(dpy);
}

int
X11Window::process_event(EgloEvent *event)
{
    if (XPending(dpy)) {
        XEvent xev;
        XNextEvent(dpy, &xev);

        if (xev.type == MotionNotify) {
            event->type = EGLO_MOUSE_MOTION;
            event->mouse.x = xev.xmotion.x * scale;
            event->mouse.y = xev.xmotion.y * scale;
            return 1;
        } else if (xev.type == ButtonPress) {
            event->type = EGLO_MOUSE_DOWN;
            event->mouse.x = xev.xbutton.x * scale;
            event->mouse.y = xev.xbutton.y * scale;
            return 1;
        } else if (xev.type == ButtonRelease) {
            event->type = EGLO_MOUSE_UP;
            event->mouse.x = xev.xbutton.x * scale;
            event->mouse.y = xev.xbutton.y * scale;
            return 1;
        } else if (xev.type == KeyPress) {
            event->type = EGLO_KEY_DOWN;
            event->key.key = XLookupKeysym(&xev.xkey, 0);
            return 1;
        } else if (xev.type == KeyRelease) {
            event->type = EGLO_KEY_UP;
            event->key.key = XLookupKeysym(&xev.xkey, 0);
            return 1;
        } else {
            printf("Unhandled: %d\n", xev.type);
        }
    }

    return 0;
}

void
X11Window::swap()
{
    XForceScreenSaver(dpy, ScreenSaverReset);
}

class EGLWindow : public X11Window {
public:
    EGLWindow(int width, int height, int gles_version, const char *version, int scale);
    ~EGLWindow();

    void swap();

protected:
    EGLDisplay egl_display;
    EGLContext egl_context;
    EGLSurface egl_surface;
    int gles_version;
    const char *version;
    GLuint fbo;
    GLuint tex;
    GLuint prg;
    GLint idx;
};

EGLWindow::EGLWindow(int width, int height, int gles_version, const char *version, int scale)
    : X11Window(width, height, scale)
    , egl_display()
    , egl_context()
    , egl_surface()
    , gles_version(gles_version)
    , version(version)
{
    egl_display = eglGetDisplay((EGLNativeDisplayType)dpy);
    if (egl_display == EGL_NO_DISPLAY) {
        FAIL("Got no EGL display.");
    }

    if (!eglInitialize(egl_display, nullptr, nullptr)) {
        FAIL("Unable to initialize EGL: 0x%08x", eglGetError());
    }

    EGLint attr[] = {
        EGL_RENDERABLE_TYPE, (gles_version == 2) ? EGL_OPENGL_ES2_BIT : EGL_OPENGL_ES_BIT,
        EGL_NONE
    };

    EGLConfig ecfg;
    EGLint num_config;

    if (!eglChooseConfig(egl_display, attr, &ecfg, 1, &num_config)) {
        FAIL("Failed to choose config (eglError: 0x%08x)", eglGetError());
    }

    if (num_config != 1) {
        FAIL("Didn't get exactly one config, but %d", num_config);
    }

    egl_surface = eglCreateWindowSurface(egl_display, ecfg, win, nullptr);
    if (egl_surface == EGL_NO_SURFACE) {
        FAIL("Unable to create EGL surface (eglError: 0x%08x)", eglGetError());
    }

    EGLint ctxattr[] = {
        EGL_CONTEXT_CLIENT_VERSION, gles_version,
        EGL_NONE
    };

    egl_context = eglCreateContext(egl_display, ecfg, EGL_NO_CONTEXT, ctxattr);
    if (egl_context == EGL_NO_CONTEXT) {
        FAIL("Unable to create EGL context (eglError: 0x%08x)", eglGetError());
    }

    eglMakeCurrent(egl_display, egl_surface, egl_surface, egl_context);

    if (scale != 1) {
        if (gles_version == 2) {
            prg = glCreateProgram();
            GLuint vtx = glCreateShader(GL_VERTEX_SHADER);
            const char *vtx_src =
            "attribute vec4 pos;"
            "varying mediump vec2 tex;"
            "void main() {"
            "    gl_Position = pos;"
            "    tex = vec2(0.5 + 0.5 * pos.x, 0.5 + 0.5 * pos.y);"
            "}"
            ;
            glShaderSource(vtx, 1, &vtx_src, nullptr);
            glCompileShader(vtx);
            glAttachShader(prg, vtx);
            glDeleteShader(vtx);

            GLuint frg = glCreateShader(GL_FRAGMENT_SHADER);
            const char *frg_src =
            "varying mediump vec2 tex;"
            "uniform sampler2D sampler;"
            "void main() {"
            "    gl_FragColor = texture2D(sampler, tex);"
            "}"
            ;
            glShaderSource(frg, 1, &frg_src, nullptr);
            glCompileShader(frg);
            glAttachShader(prg, frg);
            glDeleteShader(frg);

            glLinkProgram(prg);

            idx = glGetAttribLocation(prg, "pos");
        }

        glGenTextures(1, &tex);
        glBindTexture(GL_TEXTURE_2D, tex);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width * scale, height * scale, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glBindTexture(GL_TEXTURE_2D, 0);

        if (gles_version == 2) {
            glGenFramebuffers(1, &fbo);
            glBindFramebuffer(GL_FRAMEBUFFER, fbo);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex, 0);
            if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
                printf("Framebuffer not complete\n");
            }
        } else {
            glGenFramebuffersOES(1, &fbo);
            glBindFramebufferOES(GL_FRAMEBUFFER, fbo);
            glFramebufferTexture2DOES(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex, 0);
            if (glCheckFramebufferStatusOES(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
                printf("Framebuffer not complete\n");
            }
        }

        glViewport(0, 0, width * scale, height * scale);
    }
}

EGLWindow::~EGLWindow()
{
    glDeleteTextures(1, &tex);
    if (gles_version == 2) {
        glDeleteFramebuffers(1, &fbo);
        glDeleteProgram(prg);
    } else {
        glDeleteFramebuffersOES(1, &fbo);
    }

    eglDestroyContext(egl_display, egl_context);
    eglDestroySurface(egl_display, egl_surface);
    eglTerminate(egl_display);
}

void
EGLWindow::swap()
{
    GLint old_viewport[4];
    GLint old_prg;
    GLint old_tex;
    GLint old_old_buffer_binding;
    GLint old_enabled = 0;
    void *old_ptr;
    GLint old_size;
    GLint old_type;
    GLint old_normalized;
    GLint old_stride;
    GLint old_buffer_binding;
    GLint old_texture2d_enabled;
    GLint old_vertex_array_enabled;
    GLint old_texture_coord_array_enabled;
    void *old_vertex_array_ptr;
    void *old_texture_coord_array_ptr;
    GLint old_vertex_array_buffer;
    GLint old_vertex_array_size;
    GLint old_vertex_array_type;
    GLint old_vertex_array_stride;
    GLint old_texture_coord_array_buffer;
    GLint old_texture_coord_array_size;
    GLint old_texture_coord_array_type;
    GLint old_texture_coord_array_stride;

    if (scale != 1) {
        glGetIntegerv(GL_VIEWPORT, old_viewport);
        glGetIntegerv(GL_TEXTURE_BINDING_2D, &old_tex);
        glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &old_old_buffer_binding);

        if (gles_version == 2) {
            glGetIntegerv(GL_CURRENT_PROGRAM, &old_prg);

            glGetVertexAttribiv(idx, GL_VERTEX_ATTRIB_ARRAY_ENABLED, &old_enabled);
            glGetVertexAttribPointerv(idx, GL_VERTEX_ATTRIB_ARRAY_POINTER, &old_ptr);
            glGetVertexAttribiv(idx, GL_VERTEX_ATTRIB_ARRAY_SIZE, &old_size);
            glGetVertexAttribiv(idx, GL_VERTEX_ATTRIB_ARRAY_TYPE, &old_type);
            glGetVertexAttribiv(idx, GL_VERTEX_ATTRIB_ARRAY_NORMALIZED, &old_normalized);
            glGetVertexAttribiv(idx, GL_VERTEX_ATTRIB_ARRAY_STRIDE, &old_stride);
            glGetVertexAttribiv(idx, GL_VERTEX_ATTRIB_ARRAY_BUFFER_BINDING, &old_buffer_binding);

            glBindFramebuffer(GL_FRAMEBUFFER, 0);
        } else {
            old_texture2d_enabled = glIsEnabled(GL_TEXTURE_2D);
            old_vertex_array_enabled = glIsEnabled(GL_VERTEX_ARRAY);
            old_texture_coord_array_enabled = glIsEnabled(GL_TEXTURE_COORD_ARRAY);
            glGetPointerv(GL_VERTEX_ARRAY_POINTER, &old_vertex_array_ptr);
            glGetPointerv(GL_TEXTURE_COORD_ARRAY_POINTER, &old_texture_coord_array_ptr);
            glGetIntegerv(GL_VERTEX_ARRAY_BUFFER_BINDING, &old_vertex_array_buffer);
            glGetIntegerv(GL_VERTEX_ARRAY_SIZE, &old_vertex_array_size);
            glGetIntegerv(GL_VERTEX_ARRAY_TYPE, &old_vertex_array_type);
            glGetIntegerv(GL_VERTEX_ARRAY_STRIDE, &old_vertex_array_stride);
            glGetIntegerv(GL_TEXTURE_COORD_ARRAY_BUFFER_BINDING, &old_texture_coord_array_buffer);
            glGetIntegerv(GL_TEXTURE_COORD_ARRAY_SIZE, &old_texture_coord_array_size);
            glGetIntegerv(GL_TEXTURE_COORD_ARRAY_TYPE, &old_texture_coord_array_type);
            glGetIntegerv(GL_TEXTURE_COORD_ARRAY_STRIDE, &old_texture_coord_array_stride);

            glBindFramebufferOES(GL_FRAMEBUFFER, 0);
        }

        glViewport(0, 0, width, height);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        float vtxcoord[] = {
            -1.f, -1.f,
            -1.f, 1.f,
            1.f, -1.f,
            1.f, 1.f,
        };

        float texcoord[] = {
            0.f, 0.f,
            0.f, 1.f,
            1.f, 0.f,
            1.f, 1.f,
        };

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        if (gles_version == 2) {
            glVertexAttribPointer(idx, 2, GL_FLOAT, GL_FALSE, 0, vtxcoord);
            glEnableVertexAttribArray(idx);
            glUseProgram(prg);
        } else {
            glEnable(GL_TEXTURE_2D);
            glEnableClientState(GL_VERTEX_ARRAY);
            glEnableClientState(GL_TEXTURE_COORD_ARRAY);
            glVertexPointer(2, GL_FLOAT, 0, vtxcoord);
            glTexCoordPointer(2, GL_FLOAT, 0, texcoord);
        }

        glBindTexture(GL_TEXTURE_2D, tex);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    }

    eglSwapBuffers(egl_display, egl_surface);
    X11Window::swap();

    if (scale != 1) {
        glBindTexture(GL_TEXTURE_2D, old_tex);

        if (gles_version == 2) {
            glUseProgram(old_prg);

            glBindBuffer(GL_ARRAY_BUFFER, old_buffer_binding);
            glVertexAttribPointer(idx, old_size, old_type, old_normalized, old_stride, old_ptr);

            if (!old_enabled) {
                glDisableVertexAttribArray(idx);
            }

            glBindFramebuffer(GL_FRAMEBUFFER, fbo);
        } else {
            if (!old_texture2d_enabled) {
                glDisable(GL_TEXTURE_2D);
            }
            if (!old_vertex_array_enabled) {
                glDisableClientState(GL_VERTEX_ARRAY);
            }
            if (!old_texture_coord_array_enabled) {
                glDisableClientState(GL_TEXTURE_COORD_ARRAY);
            }

            glBindBuffer(GL_ARRAY_BUFFER, old_vertex_array_buffer);
            glVertexPointer(old_vertex_array_size, old_vertex_array_type, old_vertex_array_stride, old_vertex_array_ptr);

            glBindBuffer(GL_ARRAY_BUFFER, old_texture_coord_array_buffer);
            glTexCoordPointer(old_texture_coord_array_size, old_texture_coord_array_type, old_texture_coord_array_stride,
                    old_texture_coord_array_ptr);

            glBindFramebufferOES(GL_FRAMEBUFFER, fbo);
        }

        glBindBuffer(GL_ARRAY_BUFFER, old_old_buffer_binding);
        glViewport(old_viewport[0], old_viewport[1], old_viewport[2], old_viewport[3]);
    }
}


const char *eglo_version_string =
"@(#) eglo 0.0.3 (2018-02-21) -- Copyright (c) 2016, 2017, 2018 Thomas Perl";

constexpr int DEFAULT_WIDTH = 480 + 1;
constexpr int DEFAULT_HEIGHT = 272;
constexpr int DEFAULT_SCALE = 1;

EGLWindow *g_eglo_window = nullptr;

};

#ifdef __cplusplus
extern "C" {
#endif

int eglo_init(int *width, int *height, int gles_version)
{
    eglo_quit();

    setenv("DISPLAY", ":0", 0);

    const char *eglo_scale_env = getenv("EGLO_SCALE");
    int scale = DEFAULT_SCALE;
    if (eglo_scale_env != nullptr) {
        scale = atoi(eglo_scale_env);
        if (scale == 0) {
            scale = 1;
        } else if (scale > 4) {
            scale = 4;
        }
    }

    g_eglo_window = new EGLWindow(DEFAULT_WIDTH, DEFAULT_HEIGHT, gles_version, eglo_version_string, scale);

    if (width != nullptr) {
        *width = g_eglo_window->get_width();
    }

    if (height != nullptr) {
        *height = g_eglo_window->get_height();
    }

    return 0;
}

int eglo_next_event(EgloEvent *event)
{
    if (g_eglo_window != nullptr) {
        return g_eglo_window->process_event(event);
    }

    return 0;
}

void eglo_swap_buffers()
{
    if (g_eglo_window != nullptr) {
        g_eglo_window->swap();
    }
}

void eglo_quit()
{
    if (g_eglo_window != nullptr) {
        delete g_eglo_window, g_eglo_window = nullptr;
    }
}

#ifdef __cplusplus
};
#endif
