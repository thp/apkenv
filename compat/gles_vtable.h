#pragma once

#define GL_GLEXT_PROTOTYPES
#include <GLES/gl.h>
#include <GLES/glext.h>
#include "gl_types.h"

#include <EGL/egl.h>

struct gles1_functions {
    void (*glAlphaFunc)(GLenum func, GLclampf ref);
    void (*glClearColor)(GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha);
    void (*glClearDepthf)(GLclampf depth);
    void (*glClipPlanef)(GLenum plane, const GLfloat *equation);
    void (*glColor4f)(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
    void (*glDepthRangef)(GLclampf zNear, GLclampf zFar);
    void (*glFogf)(GLenum pname, GLfloat param);
    void (*glFogfv)(GLenum pname, const GLfloat *params);
    void (*glFrustumf)(GLfloat left, GLfloat right, GLfloat bottom, GLfloat top, GLfloat zNear, GLfloat zFar);
    void (*glGetClipPlanef)(GLenum pname, GLfloat eqn[4]);
    void (*glGetFloatv)(GLenum pname, GLfloat *params);
    void (*glGetLightfv)(GLenum light, GLenum pname, GLfloat *params);
    void (*glGetMaterialfv)(GLenum face, GLenum pname, GLfloat *params);
    void (*glGetTexEnvfv)(GLenum env, GLenum pname, GLfloat *params);
    void (*glGetTexParameterfv)(GLenum target, GLenum pname, GLfloat *params);
    void (*glLightModelf)(GLenum pname, GLfloat param);
    void (*glLightModelfv)(GLenum pname, const GLfloat *params);
    void (*glLightf)(GLenum light, GLenum pname, GLfloat param);
    void (*glLightfv)(GLenum light, GLenum pname, const GLfloat *params);
    void (*glLineWidth)(GLfloat width);
    void (*glLoadMatrixf)(const GLfloat *m);
    void (*glMaterialf)(GLenum face, GLenum pname, GLfloat param);
    void (*glMaterialfv)(GLenum face, GLenum pname, const GLfloat *params);
    void (*glMultMatrixf)(const GLfloat *m);
    void (*glMultiTexCoord4f)(GLenum target, GLfloat s, GLfloat t, GLfloat r, GLfloat q);
    void (*glNormal3f)(GLfloat nx, GLfloat ny, GLfloat nz);
    void (*glOrthof)(GLfloat left, GLfloat right, GLfloat bottom, GLfloat top, GLfloat zNear, GLfloat zFar);
    void (*glPointParameterf)(GLenum pname, GLfloat param);
    void (*glPointParameterfv)(GLenum pname, const GLfloat *params);
    void (*glPointSize)(GLfloat size);
    void (*glPolygonOffset)(GLfloat factor, GLfloat units);
    void (*glRotatef)(GLfloat angle, GLfloat x, GLfloat y, GLfloat z);
    void (*glScalef)(GLfloat x, GLfloat y, GLfloat z);
    void (*glTexEnvf)(GLenum target, GLenum pname, GLfloat param);
    void (*glTexEnvfv)(GLenum target, GLenum pname, const GLfloat *params);
    void (*glTexParameterf)(GLenum target, GLenum pname, GLfloat param);
    void (*glTexParameterfv)(GLenum target, GLenum pname, const GLfloat *params);
    void (*glTranslatef)(GLfloat x, GLfloat y, GLfloat z);
    void (*glActiveTexture)(GLenum texture);
    void (*glAlphaFuncx)(GLenum func, GLclampx ref);
    void (*glBindBuffer)(GLenum target, GLuint buffer);
    void (*glBindTexture)(GLenum target, GLuint texture);
    void (*glBlendFunc)(GLenum sfactor, GLenum dfactor);
    void (*glBufferData)(GLenum target, GLsizeiptr size, const GLvoid *data, GLenum usage);
    void (*glBufferSubData)(GLenum target, GLintptr offset, GLsizeiptr size, const GLvoid *data);
    void (*glClear)(GLbitfield mask);
    void (*glClearColorx)(GLclampx red, GLclampx green, GLclampx blue, GLclampx alpha);
    void (*glClearDepthx)(GLclampx depth);
    void (*glClearStencil)(GLint s);
    void (*glClientActiveTexture)(GLenum texture);
    void (*glClipPlanex)(GLenum plane, const GLfixed *equation);
    void (*glColor4ub)(GLubyte red, GLubyte green, GLubyte blue, GLubyte alpha);
    void (*glColor4x)(GLfixed red, GLfixed green, GLfixed blue, GLfixed alpha);
    void (*glColorMask)(GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha);
    void (*glColorPointer)(GLint size, GLenum type, GLsizei stride, const GLvoid *pointer);
    void (*glCompressedTexImage2D)(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLsizei imageSize, const GLvoid *data);
    void (*glCompressedTexSubImage2D)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLsizei imageSize, const GLvoid *data);
    void (*glCopyTexImage2D)(GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLsizei height, GLint border);
    void (*glCopyTexSubImage2D)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height);
    void (*glCullFace)(GLenum mode);
    void (*glDeleteBuffers)(GLsizei n, const GLuint *buffers);
    void (*glDeleteTextures)(GLsizei n, const GLuint *textures);
    void (*glDepthFunc)(GLenum func);
    void (*glDepthMask)(GLboolean flag);
    void (*glDepthRangex)(GLclampx zNear, GLclampx zFar);
    void (*glDisable)(GLenum cap);
    void (*glDisableClientState)(GLenum array);
    void (*glDrawArrays)(GLenum mode, GLint first, GLsizei count);
    void (*glDrawElements)(GLenum mode, GLsizei count, GLenum type, const GLvoid *indices);
    void (*glEnable)(GLenum cap);
    void (*glEnableClientState)(GLenum array);
    void (*glFinish)(void);
    void (*glFlush)(void);
    void (*glFogx)(GLenum pname, GLfixed param);
    void (*glFogxv)(GLenum pname, const GLfixed *params);
    void (*glFrontFace)(GLenum mode);
    void (*glFrustumx)(GLfixed left, GLfixed right, GLfixed bottom, GLfixed top, GLfixed zNear, GLfixed zFar);
    void (*glGetBooleanv)(GLenum pname, GLboolean *params);
    void (*glGetBufferParameteriv)(GLenum target, GLenum pname, GLint *params);
    void (*glGetClipPlanex)(GLenum pname, GLfixed eqn[4]);
    void (*glGenBuffers)(GLsizei n, GLuint *buffers);
    void (*glGenTextures)(GLsizei n, GLuint *textures);
    GLenum (*glGetError)(void);
    void (*glGetFixedv)(GLenum pname, GLfixed *params);
    void (*glGetIntegerv)(GLenum pname, GLint *params);
    void (*glGetLightxv)(GLenum light, GLenum pname, GLfixed *params);
    void (*glGetMaterialxv)(GLenum face, GLenum pname, GLfixed *params);
    void (*glGetPointerv)(GLenum pname, GLvoid **params);
    const GLubyte * (*glGetString)(GLenum name);
    void (*glGetTexEnviv)(GLenum env, GLenum pname, GLint *params);
    void (*glGetTexEnvxv)(GLenum env, GLenum pname, GLfixed *params);
    void (*glGetTexParameteriv)(GLenum target, GLenum pname, GLint *params);
    void (*glGetTexParameterxv)(GLenum target, GLenum pname, GLfixed *params);
    void (*glHint)(GLenum target, GLenum mode);
    GLboolean (*glIsBuffer)(GLuint buffer);
    GLboolean (*glIsEnabled)(GLenum cap);
    GLboolean (*glIsTexture)(GLuint texture);
    void (*glLightModelx)(GLenum pname, GLfixed param);
    void (*glLightModelxv)(GLenum pname, const GLfixed *params);
    void (*glLightx)(GLenum light, GLenum pname, GLfixed param);
    void (*glLightxv)(GLenum light, GLenum pname, const GLfixed *params);
    void (*glLineWidthx)(GLfixed width);
    void (*glLoadIdentity)(void);
    void (*glLoadMatrixx)(const GLfixed *m);
    void (*glLogicOp)(GLenum opcode);
    void (*glMaterialx)(GLenum face, GLenum pname, GLfixed param);
    void (*glMaterialxv)(GLenum face, GLenum pname, const GLfixed *params);
    void (*glMatrixMode)(GLenum mode);
    void (*glMultMatrixx)(const GLfixed *m);
    void (*glMultiTexCoord4x)(GLenum target, GLfixed s, GLfixed t, GLfixed r, GLfixed q);
    void (*glNormal3x)(GLfixed nx, GLfixed ny, GLfixed nz);
    void (*glNormalPointer)(GLenum type, GLsizei stride, const GLvoid *pointer);
    void (*glOrthox)(GLfixed left, GLfixed right, GLfixed bottom, GLfixed top, GLfixed zNear, GLfixed zFar);
    void (*glPixelStorei)(GLenum pname, GLint param);
    void (*glPointParameterx)(GLenum pname, GLfixed param);
    void (*glPointParameterxv)(GLenum pname, const GLfixed *params);
    void (*glPointSizex)(GLfixed size);
    void (*glPolygonOffsetx)(GLfixed factor, GLfixed units);
    void (*glPopMatrix)(void);
    void (*glPushMatrix)(void);
    void (*glReadPixels)(GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, GLvoid *pixels);
    void (*glRotatex)(GLfixed angle, GLfixed x, GLfixed y, GLfixed z);
    void (*glSampleCoverage)(GLclampf value, GLboolean invert);
    void (*glSampleCoveragex)(GLclampx value, GLboolean invert);
    void (*glScalex)(GLfixed x, GLfixed y, GLfixed z);
    void (*glScissor)(GLint x, GLint y, GLsizei width, GLsizei height);
    void (*glShadeModel)(GLenum mode);
    void (*glStencilFunc)(GLenum func, GLint ref, GLuint mask);
    void (*glStencilMask)(GLuint mask);
    void (*glStencilOp)(GLenum fail, GLenum zfail, GLenum zpass);
    void (*glTexCoordPointer)(GLint size, GLenum type, GLsizei stride, const GLvoid *pointer);
    void (*glTexEnvi)(GLenum target, GLenum pname, GLint param);
    void (*glTexEnvx)(GLenum target, GLenum pname, GLfixed param);
    void (*glTexEnviv)(GLenum target, GLenum pname, const GLint *params);
    void (*glTexEnvxv)(GLenum target, GLenum pname, const GLfixed *params);
    void (*glTexImage2D)(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid *pixels);
    void (*glTexParameteri)(GLenum target, GLenum pname, GLint param);
    void (*glTexParameterx)(GLenum target, GLenum pname, GLfixed param);
    void (*glTexParameteriv)(GLenum target, GLenum pname, const GLint *params);
    void (*glTexParameterxv)(GLenum target, GLenum pname, const GLfixed *params);
    void (*glTexSubImage2D)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid *pixels);
    void (*glTranslatex)(GLfixed x, GLfixed y, GLfixed z);
    void (*glVertexPointer)(GLint size, GLenum type, GLsizei stride, const GLvoid *pointer);
    void (*glViewport)(GLint x, GLint y, GLsizei width, GLsizei height);
    void (*glPointSizePointerOES)(GLenum type, GLsizei stride, const GLvoid *pointer);

    /* GL_OES_framebuffer_object */
    GLboolean (*glIsRenderbufferOES)(GLuint renderbuffer);
    void (*glBindRenderbufferOES)(GLenum target, GLuint renderbuffer);
    void (*glDeleteRenderbuffersOES)(GLsizei n, const GLuint *renderbuffers);
    void (*glGenRenderbuffersOES)(GLsizei n, GLuint *renderbuffers);
    void (*glRenderbufferStorageOES)(GLenum target, GLenum internalformat, GLsizei width, GLsizei height);
    void (*glGetRenderbufferParameterivOES)(GLenum target, GLenum pname, GLint *params);
    GLboolean (*glIsFramebufferOES)(GLuint framebuffer);
    void (*glBindFramebufferOES)(GLenum target, GLuint framebuffer);
    void (*glDeleteFramebuffersOES)(GLsizei n, const GLuint *framebuffers);
    void (*glGenFramebuffersOES)(GLsizei n, GLuint *framebuffers);
    GLenum (*glCheckFramebufferStatusOES)(GLenum target);
    void (*glFramebufferRenderbufferOES)(GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer);
    void (*glFramebufferTexture2DOES)(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level);
    void (*glGetFramebufferAttachmentParameterivOES)(GLenum target, GLenum attachment, GLenum pname, GLint *params);
    void (*glGenerateMipmapOES)(GLenum target);

    /* GL_OES_matrix_palette */
    void (*glCurrentPaletteMatrixOES)(GLuint matrixpaletteindex);
    void (*glLoadPaletteFromModelViewMatrixOES)(void);
    void (*glMatrixIndexPointerOES)(GLint size, GLenum type, GLsizei stride, const void *pointer);
    void (*glWeightPointerOES)(GLint size, GLenum type, GLsizei stride, const void *pointer);

    /* GL_OES_query_matrix */
    GLbitfield (*glQueryMatrixxOES)(GLfixed *mantissa, GLint *exponent);

    /* GL_OES_mapbuffer */
    void * (*glMapBufferOES)(GLenum target, GLenum access);
    GLboolean (*glUnmapBufferOES)(GLenum target);
    void (*glGetBufferPointervOES)(GLenum target, GLenum pname, void **params);
};
