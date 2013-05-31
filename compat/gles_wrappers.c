#ifdef APKENV_GLES
#include "gles_wrappers.h"
#include <assert.h>
#include <EGL/egl.h>
#ifdef APKENV_DEBUG
#  define WRAPPERS_DEBUG_PRINTF(...) printf(__VA_ARGS__)
#  define GL_TEST_ERROR if (glGetError()!=GL_NO_ERROR) { printf("GL ERROR near %s\n", __FUNCTION__); }
#else
#  define WRAPPERS_DEBUG_PRINTF(...)
#  define GL_TEST_ERROR
#endif


extern struct ModuleHacks global_module_hacks;

struct gles_extensions {
    /* GL_OES_framebuffer_object */
    PFNGLISRENDERBUFFEROESPROC                      glIsRenderbufferOES;
    PFNGLBINDRENDERBUFFEROESPROC                    glBindRenderbufferOES;
    PFNGLDELETERENDERBUFFERSOESPROC                 glDeleteRenderbuffersOES;
    PFNGLGENRENDERBUFFERSOESPROC                    glGenRenderbuffersOES;
    PFNGLRENDERBUFFERSTORAGEOESPROC                 glRenderbufferStorageOES;
    PFNGLGETRENDERBUFFERPARAMETERIVOESPROC          glGetRenderbufferParameterivOES;
    PFNGLISFRAMEBUFFEROESPROC                       glIsFramebufferOES;
    PFNGLBINDFRAMEBUFFEROESPROC                     glBindFramebufferOES;
    PFNGLDELETEFRAMEBUFFERSOESPROC                  glDeleteFramebuffersOES;
    PFNGLGENFRAMEBUFFERSOESPROC                     glGenFramebuffersOES;
    PFNGLCHECKFRAMEBUFFERSTATUSOESPROC              glCheckFramebufferStatusOES;
    PFNGLFRAMEBUFFERRENDERBUFFEROESPROC             glFramebufferRenderbufferOES;
    PFNGLFRAMEBUFFERTEXTURE2DOESPROC                glFramebufferTexture2DOES;
    PFNGLGETFRAMEBUFFERATTACHMENTPARAMETERIVOESPROC glGetFramebufferAttachmentParameterivOES;
    PFNGLGENERATEMIPMAPOESPROC                      glGenerateMipmapOES;
    /* GL_OES_matrix_palette */
    PFNGLCURRENTPALETTEMATRIXOESPROC                glCurrentPaletteMatrixOES;
    PFNGLLOADPALETTEFROMMODELVIEWMATRIXOESPROC      glLoadPaletteFromModelViewMatrixOES;
    PFNGLMATRIXINDEXPOINTEROESPROC                  glMatrixIndexPointerOES;
    PFNGLWEIGHTPOINTEROESPROC                       glWeightPointerOES;
    /* GL_OES_query_matrix */
    PFNGLQUERYMATRIXXOESPROC                        glQueryMatrixxOES;
    /* GL_OES_mapbuffer */
    PFNGLMAPBUFFEROESPROC                           glMapBufferOES;
    PFNGLUNMAPBUFFEROESPROC                         glUnmapBufferOES;
    PFNGLGETBUFFERPOINTERVOESPROC                   glGetBufferPointervOES;
};
static struct gles_extensions extensions;

#define init_extension(ext) \
    extensions . ext = (typeof(extensions . ext))eglGetProcAddress(#ext); \
    WRAPPERS_DEBUG_PRINTF("%s is at 0x%x\n", #ext, extensions . ext)

void gles_extensions_init()
{
    init_extension(glIsRenderbufferOES);
    init_extension(glBindRenderbufferOES);
    init_extension(glDeleteRenderbuffersOES);
    init_extension(glGenRenderbuffersOES);
    init_extension(glRenderbufferStorageOES);
    init_extension(glGetRenderbufferParameterivOES);
    init_extension(glIsFramebufferOES);
    init_extension(glBindFramebufferOES);
    init_extension(glDeleteFramebuffersOES);
    init_extension(glGenFramebuffersOES);
    init_extension(glCheckFramebufferStatusOES);
    init_extension(glFramebufferRenderbufferOES);
    init_extension(glFramebufferTexture2DOES);
    init_extension(glGetFramebufferAttachmentParameterivOES);
    init_extension(glGenerateMipmapOES);
    init_extension(glCurrentPaletteMatrixOES);
    init_extension(glLoadPaletteFromModelViewMatrixOES);
    init_extension(glMatrixIndexPointerOES);
    init_extension(glWeightPointerOES);
    init_extension(glQueryMatrixxOES);
    init_extension(glMapBufferOES);
    init_extension(glUnmapBufferOES);
    init_extension(glGetBufferPointervOES);
}

void
my_glAlphaFunc(GLenum func, GLclampf ref)
{
    WRAPPERS_DEBUG_PRINTF("glAlphaFunc()\n", func, ref);
    glAlphaFunc(func, ref);
}
void
my_glClearColor(GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha)
{
    WRAPPERS_DEBUG_PRINTF("glClearColor()\n", red, green, blue, alpha);
    glClearColor(red, green, blue, alpha);
}
void
my_glClearDepthf(GLclampf depth)
{
    WRAPPERS_DEBUG_PRINTF("glClearDepthf()\n", depth);
    glClearDepthf(depth);
}
void
my_glClipPlanef(GLenum plane, const GLfloat *equation)
{
    WRAPPERS_DEBUG_PRINTF("glClipPlanef()\n", plane, equation);
    glClipPlanef(plane, equation);
}
void
my_glColor4f(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha)
{
    WRAPPERS_DEBUG_PRINTF("glColor4f()\n", red, green, blue, alpha);
    glColor4f(red, green, blue, alpha);
}
void
my_glDepthRangef(GLclampf zNear, GLclampf zFar)
{
    WRAPPERS_DEBUG_PRINTF("glDepthRangef()\n", zNear, zFar);
    glDepthRangef(zNear, zFar);
}
void
my_glFogf(GLenum pname, GLfloat param)
{
    WRAPPERS_DEBUG_PRINTF("glFogf()\n", pname, param);
    glFogf(pname, param);
}
void
my_glFogfv(GLenum pname, const GLfloat *params)
{
    WRAPPERS_DEBUG_PRINTF("glFogfv()\n", pname, params);
    glFogfv(pname, params);
}
void
my_glFrustumf(GLfloat left, GLfloat right, GLfloat bottom, GLfloat top, GLfloat zNear, GLfloat zFar)
{
    WRAPPERS_DEBUG_PRINTF("glFrustumf()\n", left, right, bottom, top, zNear, zFar);
    glFrustumf(left, right, bottom, top, zNear, zFar);
}
void
my_glGetClipPlanef(GLenum pname, GLfloat eqn[4])
{
    WRAPPERS_DEBUG_PRINTF("glGetClipPlanef()\n", pname, eqn);
    glGetClipPlanef(pname, eqn);
}
void
my_glGetFloatv(GLenum pname, GLfloat *params)
{
    WRAPPERS_DEBUG_PRINTF("glGetFloatv()\n", pname, params);
    glGetFloatv(pname, params);
}
void
my_glGetLightfv(GLenum light, GLenum pname, GLfloat *params)
{
    WRAPPERS_DEBUG_PRINTF("glGetLightfv()\n", light, pname, params);
    glGetLightfv(light, pname, params);
}
void
my_glGetMaterialfv(GLenum face, GLenum pname, GLfloat *params)
{
    WRAPPERS_DEBUG_PRINTF("glGetMaterialfv()\n", face, pname, params);
    glGetMaterialfv(face, pname, params);
}
void
my_glGetTexEnvfv(GLenum env, GLenum pname, GLfloat *params)
{
    WRAPPERS_DEBUG_PRINTF("glGetTexEnvfv()\n", env, pname, params);
    glGetTexEnvfv(env, pname, params);
}
void
my_glGetTexParameterfv(GLenum target, GLenum pname, GLfloat *params)
{
    WRAPPERS_DEBUG_PRINTF("glGetTexParameterfv()\n", target, pname, params);
    glGetTexParameterfv(target, pname, params);
}
void
my_glLightModelf(GLenum pname, GLfloat param)
{
    WRAPPERS_DEBUG_PRINTF("glLightModelf()\n", pname, param);
    glLightModelf(pname, param);
}
void
my_glLightModelfv(GLenum pname, const GLfloat *params)
{
    WRAPPERS_DEBUG_PRINTF("glLightModelfv()\n", pname, params);
    glLightModelfv(pname, params);
}
void
my_glLightf(GLenum light, GLenum pname, GLfloat param)
{
    WRAPPERS_DEBUG_PRINTF("glLightf()\n", light, pname, param);
    glLightf(light, pname, param);
}
void
my_glLightfv(GLenum light, GLenum pname, const GLfloat *params)
{
    WRAPPERS_DEBUG_PRINTF("glLightfv()\n", light, pname, params);
    glLightfv(light, pname, params);
}
void
my_glLineWidth(GLfloat width)
{
    WRAPPERS_DEBUG_PRINTF("glLineWidth()\n", width);
    glLineWidth(width);
}
void
my_glLoadMatrixf(const GLfloat *m)
{
    WRAPPERS_DEBUG_PRINTF("glLoadMatrixf()\n", m);
    glLoadMatrixf(m);
}
void
my_glMaterialf(GLenum face, GLenum pname, GLfloat param)
{
    WRAPPERS_DEBUG_PRINTF("glMaterialf()\n", face, pname, param);
    glMaterialf(face, pname, param);
}
void
my_glMaterialfv(GLenum face, GLenum pname, const GLfloat *params)
{
    WRAPPERS_DEBUG_PRINTF("glMaterialfv()\n", face, pname, params);
    glMaterialfv(face, pname, params);
}
void
my_glMultMatrixf(const GLfloat *m)
{
    WRAPPERS_DEBUG_PRINTF("glMultMatrixf()\n", m);
    glMultMatrixf(m);
}
void
my_glMultiTexCoord4f(GLenum target, GLfloat s, GLfloat t, GLfloat r, GLfloat q)
{
    WRAPPERS_DEBUG_PRINTF("glMultiTexCoord4f()\n", target, s, t, r, q);
    glMultiTexCoord4f(target, s, t, r, q);
}
void
my_glNormal3f(GLfloat nx, GLfloat ny, GLfloat nz)
{
    WRAPPERS_DEBUG_PRINTF("glNormal3f()\n", nx, ny, nz);
    glNormal3f(nx, ny, nz);
}
void
my_glOrthof(GLfloat left, GLfloat right, GLfloat bottom, GLfloat top, GLfloat zNear, GLfloat zFar)
{
    WRAPPERS_DEBUG_PRINTF("glOrthof()\n", left, right, bottom, top, zNear, zFar);
    if (global_module_hacks.gles_landscape_to_portrait!=0) {
        glRotatef(90, 0, 0, 1);
    }
    glOrthof(left, right, bottom, top, zNear, zFar);
}
void
my_glPointParameterf(GLenum pname, GLfloat param)
{
    WRAPPERS_DEBUG_PRINTF("glPointParameterf()\n", pname, param);
    glPointParameterf(pname, param);
}
void
my_glPointParameterfv(GLenum pname, const GLfloat *params)
{
    WRAPPERS_DEBUG_PRINTF("glPointParameterfv()\n", pname, params);
    glPointParameterfv(pname, params);
}
void
my_glPointSize(GLfloat size)
{
    WRAPPERS_DEBUG_PRINTF("glPointSize()\n", size);
    glPointSize(size);
}
void
my_glPolygonOffset(GLfloat factor, GLfloat units)
{
    WRAPPERS_DEBUG_PRINTF("glPolygonOffset()\n", factor, units);
    glPolygonOffset(factor, units);
}
void
my_glRotatef(GLfloat angle, GLfloat x, GLfloat y, GLfloat z)
{
    WRAPPERS_DEBUG_PRINTF("glRotatef()\n", angle, x, y, z);
    glRotatef(angle, x, y, z);
}
void
my_glScalef(GLfloat x, GLfloat y, GLfloat z)
{
    WRAPPERS_DEBUG_PRINTF("glScalef()\n", x, y, z);
    glScalef(x, y, z);
}
void
my_glTexEnvf(GLenum target, GLenum pname, GLfloat param)
{
    WRAPPERS_DEBUG_PRINTF("glTexEnvf()\n", target, pname, param);
    glTexEnvf(target, pname, param);
}
void
my_glTexEnvfv(GLenum target, GLenum pname, const GLfloat *params)
{
    WRAPPERS_DEBUG_PRINTF("glTexEnvfv()\n", target, pname, params);
    glTexEnvfv(target, pname, params);
}
void
my_glTexParameterf(GLenum target, GLenum pname, GLfloat param)
{
    WRAPPERS_DEBUG_PRINTF("glTexParameterf()\n", target, pname, param);
    glTexParameterf(target, pname, param);
}
void
my_glTexParameterfv(GLenum target, GLenum pname, const GLfloat *params)
{
    WRAPPERS_DEBUG_PRINTF("glTexParameterfv()\n", target, pname, params);
    glTexParameterfv(target, pname, params);
}
void
my_glTranslatef(GLfloat x, GLfloat y, GLfloat z)
{
    WRAPPERS_DEBUG_PRINTF("glTranslatef()\n", x, y, z);
    glTranslatef(x, y, z);
}
void
my_glActiveTexture(GLenum texture)
{
    WRAPPERS_DEBUG_PRINTF("glActiveTexture()\n", texture);
    glActiveTexture(texture);
}
void
my_glAlphaFuncx(GLenum func, GLclampx ref)
{
    WRAPPERS_DEBUG_PRINTF("glAlphaFuncx()\n", func, ref);
    glAlphaFuncx(func, ref);
}
void
my_glBindBuffer(GLenum target, GLuint buffer)
{
    WRAPPERS_DEBUG_PRINTF("glBindBuffer()\n", target, buffer);
    glBindBuffer(target, buffer);
}
void
my_glBindTexture(GLenum target, GLuint texture)
{
    WRAPPERS_DEBUG_PRINTF("glBindTexture()\n", target, texture);
    glBindTexture(target, texture);
}
void
my_glBlendFunc(GLenum sfactor, GLenum dfactor)
{
    WRAPPERS_DEBUG_PRINTF("glBlendFunc()\n", sfactor, dfactor);
    glBlendFunc(sfactor, dfactor);
}
void
my_glBufferData(GLenum target, GLsizeiptr size, const GLvoid *data, GLenum usage)
{
    WRAPPERS_DEBUG_PRINTF("glBufferData()\n", target, size, data, usage);
    glBufferData(target, size, data, usage);
}
void
my_glBufferSubData(GLenum target, GLintptr offset, GLsizeiptr size, const GLvoid *data)
{
    WRAPPERS_DEBUG_PRINTF("glBufferSubData()\n", target, offset, size, data);
    glBufferSubData(target, offset, size, data);
}
void
my_glClear(GLbitfield mask)
{
    WRAPPERS_DEBUG_PRINTF("glClear()\n", mask);
    glClear(mask);
}
void
my_glClearColorx(GLclampx red, GLclampx green, GLclampx blue, GLclampx alpha)
{
    WRAPPERS_DEBUG_PRINTF("glClearColorx()\n", red, green, blue, alpha);
    glClearColorx(red, green, blue, alpha);
}
void
my_glClearDepthx(GLclampx depth)
{
    WRAPPERS_DEBUG_PRINTF("glClearDepthx()\n", depth);
    glClearDepthx(depth);
}
void
my_glClearStencil(GLint s)
{
    WRAPPERS_DEBUG_PRINTF("glClearStencil()\n", s);
    glClearStencil(s);
}
void
my_glClientActiveTexture(GLenum texture)
{
    WRAPPERS_DEBUG_PRINTF("glClientActiveTexture()\n", texture);
    glClientActiveTexture(texture);
}
void
my_glClipPlanex(GLenum plane, const GLfixed *equation)
{
    WRAPPERS_DEBUG_PRINTF("glClipPlanex()\n", plane, equation);
    glClipPlanex(plane, equation);
}
void
my_glColor4ub(GLubyte red, GLubyte green, GLubyte blue, GLubyte alpha)
{
    WRAPPERS_DEBUG_PRINTF("glColor4ub()\n", red, green, blue, alpha);
    glColor4ub(red, green, blue, alpha);
}
void
my_glColor4x(GLfixed red, GLfixed green, GLfixed blue, GLfixed alpha)
{
    WRAPPERS_DEBUG_PRINTF("glColor4x()\n", red, green, blue, alpha);
    glColor4x(red, green, blue, alpha);
}
void
my_glColorMask(GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha)
{
    WRAPPERS_DEBUG_PRINTF("glColorMask()\n", red, green, blue, alpha);
    glColorMask(red, green, blue, alpha);
}
void
my_glColorPointer(GLint size, GLenum type, GLsizei stride, const GLvoid *pointer)
{
    WRAPPERS_DEBUG_PRINTF("glColorPointer()\n", size, type, stride, pointer);
    glColorPointer(size, type, stride, pointer);
}
void
my_glCompressedTexImage2D(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLsizei imageSize, const GLvoid *data)
{
    WRAPPERS_DEBUG_PRINTF("glCompressedTexImage2D()\n", target, level, internalformat, width, height, border, imageSize, data);
    glCompressedTexImage2D(target, level, internalformat, width, height, border, imageSize, data);
}
void
my_glCompressedTexSubImage2D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLsizei imageSize, const GLvoid *data)
{
    WRAPPERS_DEBUG_PRINTF("glCompressedTexSubImage2D()\n", target, level, xoffset, yoffset, width, height, format, imageSize, data);
    glCompressedTexSubImage2D(target, level, xoffset, yoffset, width, height, format, imageSize, data);
}
void
my_glCopyTexImage2D(GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLsizei height, GLint border)
{
    WRAPPERS_DEBUG_PRINTF("glCopyTexImage2D()\n", target, level, internalformat, x, y, width, height, border);
    glCopyTexImage2D(target, level, internalformat, x, y, width, height, border);
}
void
my_glCopyTexSubImage2D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height)
{
    WRAPPERS_DEBUG_PRINTF("glCopyTexSubImage2D()\n", target, level, xoffset, yoffset, x, y, width, height);
    glCopyTexSubImage2D(target, level, xoffset, yoffset, x, y, width, height);
}
void
my_glCullFace(GLenum mode)
{
    WRAPPERS_DEBUG_PRINTF("glCullFace()\n", mode);
    glCullFace(mode);
}
void
my_glDeleteBuffers(GLsizei n, const GLuint *buffers)
{
    WRAPPERS_DEBUG_PRINTF("glDeleteBuffers()\n", n, buffers);
    glDeleteBuffers(n, buffers);
}
void
my_glDeleteTextures(GLsizei n, const GLuint *textures)
{
    WRAPPERS_DEBUG_PRINTF("glDeleteTextures()\n", n, textures);
    glDeleteTextures(n, textures);
}
void
my_glDepthFunc(GLenum func)
{
    WRAPPERS_DEBUG_PRINTF("glDepthFunc()\n", func);
    glDepthFunc(func);
}
void
my_glDepthMask(GLboolean flag)
{
    WRAPPERS_DEBUG_PRINTF("glDepthMask()\n", flag);
    glDepthMask(flag);
}
void
my_glDepthRangex(GLclampx zNear, GLclampx zFar)
{
    WRAPPERS_DEBUG_PRINTF("glDepthRangex()\n", zNear, zFar);
    glDepthRangex(zNear, zFar);
}
void
my_glDisable(GLenum cap)
{
    WRAPPERS_DEBUG_PRINTF("glDisable()\n", cap);
    glDisable(cap);
}
void
my_glDisableClientState(GLenum array)
{
    WRAPPERS_DEBUG_PRINTF("glDisableClientState()\n", array);
    glDisableClientState(array);
}
void
my_glDrawArrays(GLenum mode, GLint first, GLsizei count)
{
    WRAPPERS_DEBUG_PRINTF("glDrawArrays()\n", mode, first, count);
    glDrawArrays(mode, first, count);
}
void
my_glDrawElements(GLenum mode, GLsizei count, GLenum type, const GLvoid *indices)
{
    WRAPPERS_DEBUG_PRINTF("glDrawElements(%d, %d, %d, %p)\n", mode, count, type, indices);
    glDrawElements(mode, count, type, indices);
}
void
my_glEnable(GLenum cap)
{
    WRAPPERS_DEBUG_PRINTF("glEnable()\n", cap);
    glEnable(cap);
}
void
my_glEnableClientState(GLenum array)
{
    WRAPPERS_DEBUG_PRINTF("glEnableClientState(0x%x)\n", array);
    glEnableClientState(array);
}
void
my_glFinish()
{
    WRAPPERS_DEBUG_PRINTF("glFinish()\n");
    glFinish();
}
void
my_glFlush()
{
    WRAPPERS_DEBUG_PRINTF("glFlush()\n");
    glFlush();
}
void
my_glFogx(GLenum pname, GLfixed param)
{
    WRAPPERS_DEBUG_PRINTF("glFogx()\n", pname, param);
    glFogx(pname, param);
}
void
my_glFogxv(GLenum pname, const GLfixed *params)
{
    WRAPPERS_DEBUG_PRINTF("glFogxv()\n", pname, params);
    glFogxv(pname, params);
}
void
my_glFrontFace(GLenum mode)
{
    WRAPPERS_DEBUG_PRINTF("glFrontFace()\n", mode);
    glFrontFace(mode);
}
void
my_glFrustumx(GLfixed left, GLfixed right, GLfixed bottom, GLfixed top, GLfixed zNear, GLfixed zFar)
{
    WRAPPERS_DEBUG_PRINTF("glFrustumx()\n", left, right, bottom, top, zNear, zFar);
    glFrustumx(left, right, bottom, top, zNear, zFar);
}
void
my_glGetBooleanv(GLenum pname, GLboolean *params)
{
    WRAPPERS_DEBUG_PRINTF("glGetBooleanv()\n", pname, params);
    glGetBooleanv(pname, params);
}
void
my_glGetBufferParameteriv(GLenum target, GLenum pname, GLint *params)
{
    WRAPPERS_DEBUG_PRINTF("glGetBufferParameteriv()\n", target, pname, params);
    glGetBufferParameteriv(target, pname, params);
}
void
my_glGetClipPlanex(GLenum pname, GLfixed eqn[4])
{
    WRAPPERS_DEBUG_PRINTF("glGetClipPlanex()\n", pname, eqn);
    glGetClipPlanex(pname, eqn);
}
void
my_glGenBuffers(GLsizei n, GLuint *buffers)
{
    WRAPPERS_DEBUG_PRINTF("glGenBuffers()\n", n, buffers);
    glGenBuffers(n, buffers);
}
void
my_glGenTextures(GLsizei n, GLuint *textures)
{
    WRAPPERS_DEBUG_PRINTF("glGenTextures()\n", n, textures);
    glGenTextures(n, textures);
}
GLenum
my_glGetError()
{
    WRAPPERS_DEBUG_PRINTF("glGetError()\n");
    return glGetError();
}
void
my_glGetFixedv(GLenum pname, GLfixed *params)
{
    WRAPPERS_DEBUG_PRINTF("glGetFixedv()\n", pname, params);
    glGetFixedv(pname, params);
}
void
my_glGetIntegerv(GLenum pname, GLint *params)
{
    WRAPPERS_DEBUG_PRINTF("glGetIntegerv()\n", pname, params);
    glGetIntegerv(pname, params);
}
void
my_glGetLightxv(GLenum light, GLenum pname, GLfixed *params)
{
    WRAPPERS_DEBUG_PRINTF("glGetLightxv()\n", light, pname, params);
    glGetLightxv(light, pname, params);
}
void
my_glGetMaterialxv(GLenum face, GLenum pname, GLfixed *params)
{
    WRAPPERS_DEBUG_PRINTF("glGetMaterialxv()\n", face, pname, params);
    glGetMaterialxv(face, pname, params);
}
void
my_glGetPointerv(GLenum pname, void **params)
{
    WRAPPERS_DEBUG_PRINTF("glGetPointerv()\n", pname, params);
    glGetPointerv(pname, params);
}
const GLubyte *
my_glGetString(GLenum name)
{
    WRAPPERS_DEBUG_PRINTF("glGetString()\n", name);
    return glGetString(name);
}
void
my_glGetTexEnviv(GLenum env, GLenum pname, GLint *params)
{
    WRAPPERS_DEBUG_PRINTF("glGetTexEnviv()\n", env, pname, params);
    glGetTexEnviv(env, pname, params);
}
void
my_glGetTexEnvxv(GLenum env, GLenum pname, GLfixed *params)
{
    WRAPPERS_DEBUG_PRINTF("glGetTexEnvxv()\n", env, pname, params);
    glGetTexEnvxv(env, pname, params);
}
void
my_glGetTexParameteriv(GLenum target, GLenum pname, GLint *params)
{
    WRAPPERS_DEBUG_PRINTF("glGetTexParameteriv()\n", target, pname, params);
    glGetTexParameteriv(target, pname, params);
}
void
my_glGetTexParameterxv(GLenum target, GLenum pname, GLfixed *params)
{
    WRAPPERS_DEBUG_PRINTF("glGetTexParameterxv()\n", target, pname, params);
    glGetTexParameterxv(target, pname, params);
}
void
my_glHint(GLenum target, GLenum mode)
{
    WRAPPERS_DEBUG_PRINTF("glHint()\n", target, mode);
    glHint(target, mode);
}
GLboolean
my_glIsBuffer(GLuint buffer)
{
    WRAPPERS_DEBUG_PRINTF("glIsBuffer()\n", buffer);
    return glIsBuffer(buffer);
}
GLboolean
my_glIsEnabled(GLenum cap)
{
    WRAPPERS_DEBUG_PRINTF("glIsEnabled()\n", cap);
    return glIsEnabled(cap);
}
GLboolean
my_glIsTexture(GLuint texture)
{
    WRAPPERS_DEBUG_PRINTF("glIsTexture()\n", texture);
    return glIsTexture(texture);
}
void
my_glLightModelx(GLenum pname, GLfixed param)
{
    WRAPPERS_DEBUG_PRINTF("glLightModelx()\n", pname, param);
    glLightModelx(pname, param);
}
void
my_glLightModelxv(GLenum pname, const GLfixed *params)
{
    WRAPPERS_DEBUG_PRINTF("glLightModelxv()\n", pname, params);
    glLightModelxv(pname, params);
}
void
my_glLightx(GLenum light, GLenum pname, GLfixed param)
{
    WRAPPERS_DEBUG_PRINTF("glLightx()\n", light, pname, param);
    glLightx(light, pname, param);
}
void
my_glLightxv(GLenum light, GLenum pname, const GLfixed *params)
{
    WRAPPERS_DEBUG_PRINTF("glLightxv()\n", light, pname, params);
    glLightxv(light, pname, params);
}
void
my_glLineWidthx(GLfixed width)
{
    WRAPPERS_DEBUG_PRINTF("glLineWidthx()\n", width);
    glLineWidthx(width);
}
void
my_glLoadIdentity()
{
    WRAPPERS_DEBUG_PRINTF("glLoadIdentity()\n");
    glLoadIdentity();
}
void
my_glLoadMatrixx(const GLfixed *m)
{
    WRAPPERS_DEBUG_PRINTF("glLoadMatrixx()\n", m);
    glLoadMatrixx(m);
}
void
my_glLogicOp(GLenum opcode)
{
    WRAPPERS_DEBUG_PRINTF("glLogicOp()\n", opcode);
    glLogicOp(opcode);
}
void
my_glMaterialx(GLenum face, GLenum pname, GLfixed param)
{
    WRAPPERS_DEBUG_PRINTF("glMaterialx()\n", face, pname, param);
    glMaterialx(face, pname, param);
}
void
my_glMaterialxv(GLenum face, GLenum pname, const GLfixed *params)
{
    WRAPPERS_DEBUG_PRINTF("glMaterialxv()\n", face, pname, params);
    glMaterialxv(face, pname, params);
}
void
my_glMatrixMode(GLenum mode)
{
    WRAPPERS_DEBUG_PRINTF("glMatrixMode()\n", mode);
    glMatrixMode(mode);
}
void
my_glMultMatrixx(const GLfixed *m)
{
    WRAPPERS_DEBUG_PRINTF("glMultMatrixx()\n", m);
    glMultMatrixx(m);
}
void
my_glMultiTexCoord4x(GLenum target, GLfixed s, GLfixed t, GLfixed r, GLfixed q)
{
    WRAPPERS_DEBUG_PRINTF("glMultiTexCoord4x()\n", target, s, t, r, q);
    glMultiTexCoord4x(target, s, t, r, q);
}
void
my_glNormal3x(GLfixed nx, GLfixed ny, GLfixed nz)
{
    WRAPPERS_DEBUG_PRINTF("glNormal3x()\n", nx, ny, nz);
    glNormal3x(nx, ny, nz);
}
void
my_glNormalPointer(GLenum type, GLsizei stride, const GLvoid *pointer)
{
    WRAPPERS_DEBUG_PRINTF("glNormalPointer()\n", type, stride, pointer);
    glNormalPointer(type, stride, pointer);
}
void
my_glOrthox(GLfixed left, GLfixed right, GLfixed bottom, GLfixed top, GLfixed zNear, GLfixed zFar)
{
    WRAPPERS_DEBUG_PRINTF("glOrthox()\n", left, right, bottom, top, zNear, zFar);
    glOrthox(left, right, bottom, top, zNear, zFar);
}
void
my_glPixelStorei(GLenum pname, GLint param)
{
    WRAPPERS_DEBUG_PRINTF("glPixelStorei()\n", pname, param);
    glPixelStorei(pname, param);
}
void
my_glPointParameterx(GLenum pname, GLfixed param)
{
    WRAPPERS_DEBUG_PRINTF("glPointParameterx()\n", pname, param);
    glPointParameterx(pname, param);
}
void
my_glPointParameterxv(GLenum pname, const GLfixed *params)
{
    WRAPPERS_DEBUG_PRINTF("glPointParameterxv()\n", pname, params);
    glPointParameterxv(pname, params);
}
void
my_glPointSizex(GLfixed size)
{
    WRAPPERS_DEBUG_PRINTF("glPointSizex()\n", size);
    glPointSizex(size);
}
void
my_glPolygonOffsetx(GLfixed factor, GLfixed units)
{
    WRAPPERS_DEBUG_PRINTF("glPolygonOffsetx()\n", factor, units);
    glPolygonOffsetx(factor, units);
}
void
my_glPopMatrix()
{
    WRAPPERS_DEBUG_PRINTF("glPopMatrix()\n");
    glPopMatrix();
}
void
my_glPushMatrix()
{
    WRAPPERS_DEBUG_PRINTF("glPushMatrix()\n");
    glPushMatrix();
}
void
my_glReadPixels(GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, GLvoid *pixels)
{
    WRAPPERS_DEBUG_PRINTF("glReadPixels(%d,%d,%d,%d,0x%x,0x%x,0x%x)\n", x, y, width, height, format, type, pixels);
    if (global_module_hacks.gles_no_readpixels==0)
        glReadPixels(x, y, width, height, format, type, pixels);
}
void
my_glRotatex(GLfixed angle, GLfixed x, GLfixed y, GLfixed z)
{
    WRAPPERS_DEBUG_PRINTF("glRotatex()\n", angle, x, y, z);
    glRotatex(angle, x, y, z);
}
void
my_glSampleCoverage(GLclampf value, GLboolean invert)
{
    WRAPPERS_DEBUG_PRINTF("glSampleCoverage()\n", value, invert);
    glSampleCoverage(value, invert);
}
void
my_glSampleCoveragex(GLclampx value, GLboolean invert)
{
    WRAPPERS_DEBUG_PRINTF("glSampleCoveragex()\n", value, invert);
    glSampleCoveragex(value, invert);
}
void
my_glScalex(GLfixed x, GLfixed y, GLfixed z)
{
    WRAPPERS_DEBUG_PRINTF("glScalex()\n", x, y, z);
    glScalex(x, y, z);
}
void
my_glScissor(GLint x, GLint y, GLsizei width, GLsizei height)
{
    WRAPPERS_DEBUG_PRINTF("glScissor()\n", x, y, width, height);
    glScissor(x, y, width, height);
}
void
my_glShadeModel(GLenum mode)
{
    WRAPPERS_DEBUG_PRINTF("glShadeModel()\n", mode);
    glShadeModel(mode);
}
void
my_glStencilFunc(GLenum func, GLint ref, GLuint mask)
{
    WRAPPERS_DEBUG_PRINTF("glStencilFunc()\n", func, ref, mask);
    glStencilFunc(func, ref, mask);
}
void
my_glStencilMask(GLuint mask)
{
    WRAPPERS_DEBUG_PRINTF("glStencilMask()\n", mask);
    glStencilMask(mask);
}
void
my_glStencilOp(GLenum fail, GLenum zfail, GLenum zpass)
{
    WRAPPERS_DEBUG_PRINTF("glStencilOp()\n", fail, zfail, zpass);
    glStencilOp(fail, zfail, zpass);
}
void
my_glTexCoordPointer(GLint size, GLenum type, GLsizei stride, const GLvoid *pointer)
{
    WRAPPERS_DEBUG_PRINTF("glTexCoordPointer()\n", size, type, stride, pointer);
    glTexCoordPointer(size, type, stride, pointer);
}
void
my_glTexEnvi(GLenum target, GLenum pname, GLint param)
{
    WRAPPERS_DEBUG_PRINTF("glTexEnvi()\n", target, pname, param);
    glTexEnvi(target, pname, param);
}
void
my_glTexEnvx(GLenum target, GLenum pname, GLfixed param)
{
    WRAPPERS_DEBUG_PRINTF("glTexEnvx()\n", target, pname, param);
    glTexEnvx(target, pname, param);
}
void
my_glTexEnviv(GLenum target, GLenum pname, const GLint *params)
{
    WRAPPERS_DEBUG_PRINTF("glTexEnviv()\n", target, pname, params);
    glTexEnviv(target, pname, params);
}
void
my_glTexEnvxv(GLenum target, GLenum pname, const GLfixed *params)
{
    WRAPPERS_DEBUG_PRINTF("glTexEnvxv()\n", target, pname, params);
    glTexEnvxv(target, pname, params);
}
void
my_glTexImage2D(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid *pixels)
{
    int maxsize = 0;
    glGetIntegerv(GL_MAX_TEXTURE_SIZE,&maxsize);

    int downscale = global_module_hacks.gles_downscale_images && (width>=maxsize || height>=maxsize);

    if ( downscale && format==GL_RGBA && (type==GL_UNSIGNED_SHORT_4_4_4_4 || type==GL_UNSIGNED_SHORT_5_6_5) )
    {
        int x,y;
        GLushort* src = (GLushort*)pixels;
        GLushort* downsized = malloc(width*height/2);
        GLushort* ptr = downsized;
        for (y=0; y<height;y+=2)
        {
            //GLbyte* row = src + y*width*4;
            for (x=0;x<width;x+=2)
            {
                int off = (y*width+x);
                *ptr ++ = src[off];
            }
        }

        WRAPPERS_DEBUG_PRINTF("downscale/glTexImage2D(0x%x,%d,%d,%d,%d,%d,0x%x,0x%x,0x%x)\n", target, level, internalformat, width, height, border, format, type, pixels);
        glTexImage2D(target, level, internalformat, width>>1, height>>1, border, format, type, downsized);
        free(downsized);
    }
    else
    if ( downscale && format==GL_RGBA && type==GL_UNSIGNED_BYTE )
    {
        int x,y;
        GLbyte* src = (GLbyte*)pixels;
        GLbyte* downsized = malloc(width*height*4/2);
        GLbyte* ptr = downsized;
        for (y=0; y<height;y+=2)
        {
            //GLbyte* row = src + y*width*4;
            for (x=0;x<width;x+=2)
            {
                int off = (y*width+x)*4;
                *ptr ++ = src[off+0];
                *ptr ++ = src[off+1];
                *ptr ++ = src[off+2];
                *ptr ++ = src[off+3];
            }
        }
        WRAPPERS_DEBUG_PRINTF("downscale/glTexImage2D(0x%x,%d,%d,%d,%d,%d,0x%x,0x%x,0x%x)\n", target, level, internalformat, width, height, border, format, type, pixels);
        glTexImage2D(target, level, internalformat, width>>1, height>>1, border, format, type, downsized);
        free(downsized);
    }
    else
    {
        if (downscale)
            WRAPPERS_DEBUG_PRINTF("downscale unsupported/glTexImage2D(0x%x,%d,%d,%d,%d,%d,0x%x,0x%x,0x%x)\n", target, level, internalformat, width, height, border, format, type, pixels);
        else
            WRAPPERS_DEBUG_PRINTF("glTexImage2D(0x%x,%d,%d,%d,%d,%d,0x%x,0x%x,0x%x)\n", target, level, internalformat, width, height, border, format, type, pixels);

        glTexImage2D(target, level, internalformat, width, height, border, format, type, pixels);
    }
}
void
my_glTexParameteri(GLenum target, GLenum pname, GLint param)
{
    WRAPPERS_DEBUG_PRINTF("glTexParameteri()\n", target, pname, param);
    glTexParameteri(target, pname, param);
}
void
my_glTexParameterx(GLenum target, GLenum pname, GLfixed param)
{
    WRAPPERS_DEBUG_PRINTF("glTexParameterx()\n", target, pname, param);
    glTexParameterx(target, pname, param);
}
void
my_glTexParameteriv(GLenum target, GLenum pname, const GLint *params)
{
    WRAPPERS_DEBUG_PRINTF("glTexParameteriv()\n", target, pname, params);
    glTexParameteriv(target, pname, params);
}
void
my_glTexParameterxv(GLenum target, GLenum pname, const GLfixed *params)
{
    WRAPPERS_DEBUG_PRINTF("glTexParameterxv()\n", target, pname, params);
    glTexParameterxv(target, pname, params);
}
void
my_glTexSubImage2D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid *pixels)
{
    WRAPPERS_DEBUG_PRINTF("glTexSubImage2D()\n", target, level, xoffset, yoffset, width, height, format, type, pixels);
    glTexSubImage2D(target, level, xoffset, yoffset, width, height, format, type, pixels);
}
void
my_glTranslatex(GLfixed x, GLfixed y, GLfixed z)
{
    WRAPPERS_DEBUG_PRINTF("glTranslatex()\n", x, y, z);
    glTranslatex(x, y, z);
}
void
my_glVertexPointer(GLint size, GLenum type, GLsizei stride, const GLvoid *pointer)
{
    WRAPPERS_DEBUG_PRINTF("glVertexPointer()\n", size, type, stride, pointer);
    glVertexPointer(size, type, stride, pointer);
}
void
my_glViewport(GLint x, GLint y, GLsizei width, GLsizei height)
{
    WRAPPERS_DEBUG_PRINTF("glViewport(%d, %d, %d, %d)\n", x, y, width, height);
    if (global_module_hacks.gles_landscape_to_portrait!=0) {
        glViewport(y, x, height, width);
    } else {
        glViewport(x, y, width, height);
    }
}
void
my_glPointSizePointerOES(GLenum type, GLsizei stride, const GLvoid *pointer)
{
    WRAPPERS_DEBUG_PRINTF("glPointSizePointerOES()\n", type, stride, pointer);
    /* No CALL */ printf("UNIMPLEMENTED: glPointSizePointerOES\n");
}
void
my_glBlendEquationSeparateOES(GLenum modeRGB, GLenum modeAlpha)
{
    WRAPPERS_DEBUG_PRINTF("glBlendEquationSeparateOES()\n", modeRGB, modeAlpha);
    /* No CALL */ printf("UNIMPLEMENTED: glBlendEquationSeparateOES\n");
}
void
my_glBlendFuncSeparateOES(GLenum srcRGB, GLenum dstRGB, GLenum srcAlpha, GLenum dstAlpha)
{
    WRAPPERS_DEBUG_PRINTF("glBlendFuncSeparateOES()\n", srcRGB, dstRGB, srcAlpha, dstAlpha);
    /* No CALL */ printf("UNIMPLEMENTED: glBlendFuncSeparateOES\n");
}
void
my_glBlendEquationOES(GLenum mode)
{
    WRAPPERS_DEBUG_PRINTF("glBlendEquationOES()\n", mode);
    /* No CALL */ printf("UNIMPLEMENTED: glBlendEquationOES\n");
}
void
my_glDrawTexsOES(GLshort x, GLshort y, GLshort z, GLshort width, GLshort height)
{
    WRAPPERS_DEBUG_PRINTF("glDrawTexsOES()\n", x, y, z, width, height);
    /* No CALL */ printf("UNIMPLEMENTED: glDrawTexsOES\n");
}
void
my_glDrawTexiOES(GLint x, GLint y, GLint z, GLint width, GLint height)
{
    WRAPPERS_DEBUG_PRINTF("glDrawTexiOES()\n", x, y, z, width, height);
    /* No CALL */ printf("UNIMPLEMENTED: glDrawTexiOES\n");
}
void
my_glDrawTexxOES(GLfixed x, GLfixed y, GLfixed z, GLfixed width, GLfixed height)
{
    WRAPPERS_DEBUG_PRINTF("glDrawTexxOES()\n", x, y, z, width, height);
    /* No CALL */ printf("UNIMPLEMENTED: glDrawTexxOES\n");
}
void
my_glDrawTexsvOES(const GLshort *coords)
{
    WRAPPERS_DEBUG_PRINTF("glDrawTexsvOES()\n", coords);
    /* No CALL */ printf("UNIMPLEMENTED: glDrawTexsvOES\n");
}
void
my_glDrawTexivOES(const GLint *coords)
{
    WRAPPERS_DEBUG_PRINTF("glDrawTexivOES()\n", coords);
    /* No CALL */ printf("UNIMPLEMENTED: glDrawTexivOES\n");
}
void
my_glDrawTexxvOES(const GLfixed *coords)
{
    WRAPPERS_DEBUG_PRINTF("glDrawTexxvOES()\n", coords);
    /* No CALL */ printf("UNIMPLEMENTED: glDrawTexxvOES\n");
}
void
my_glDrawTexfOES(GLfloat x, GLfloat y, GLfloat z, GLfloat width, GLfloat height)
{
    WRAPPERS_DEBUG_PRINTF("glDrawTexfOES()\n", x, y, z, width, height);
    /* No CALL */ printf("UNIMPLEMENTED: glDrawTexfOES\n");
}
void
my_glDrawTexfvOES(const GLfloat *coords)
{
    WRAPPERS_DEBUG_PRINTF("glDrawTexfvOES()\n", coords);
    /* No CALL */ printf("UNIMPLEMENTED: glDrawTexfvOES\n");
}
void
my_glAlphaFuncxOES(GLenum func, GLclampx ref)
{
    WRAPPERS_DEBUG_PRINTF("glAlphaFuncxOES()\n", func, ref);
    /* No CALL */ printf("UNIMPLEMENTED: glAlphaFuncxOES\n");
}
void
my_glClearColorxOES(GLclampx red, GLclampx green, GLclampx blue, GLclampx alpha)
{
    WRAPPERS_DEBUG_PRINTF("glClearColorxOES()\n", red, green, blue, alpha);
    /* No CALL */ printf("UNIMPLEMENTED: glClearColorxOES\n");
}
void
my_glClearDepthxOES(GLclampx depth)
{
    WRAPPERS_DEBUG_PRINTF("glClearDepthxOES()\n", depth);
    /* No CALL */ printf("UNIMPLEMENTED: glClearDepthxOES\n");
}
void
my_glClipPlanexOES(GLenum plane, const GLfixed *equation)
{
    WRAPPERS_DEBUG_PRINTF("glClipPlanexOES()\n", plane, equation);
    /* No CALL */ printf("UNIMPLEMENTED: glClipPlanexOES\n");
}
void
my_glColor4xOES(GLfixed red, GLfixed green, GLfixed blue, GLfixed alpha)
{
    WRAPPERS_DEBUG_PRINTF("glColor4xOES()\n", red, green, blue, alpha);
    /* No CALL */ printf("UNIMPLEMENTED: glColor4xOES\n");
}
void
my_glDepthRangexOES(GLclampx zNear, GLclampx zFar)
{
    WRAPPERS_DEBUG_PRINTF("glDepthRangexOES()\n", zNear, zFar);
    /* No CALL */ printf("UNIMPLEMENTED: glDepthRangexOES\n");
}
void
my_glFogxOES(GLenum pname, GLfixed param)
{
    WRAPPERS_DEBUG_PRINTF("glFogxOES()\n", pname, param);
    /* No CALL */ printf("UNIMPLEMENTED: glFogxOES\n");
}
void
my_glFogxvOES(GLenum pname, const GLfixed *params)
{
    WRAPPERS_DEBUG_PRINTF("glFogxvOES()\n", pname, params);
    /* No CALL */ printf("UNIMPLEMENTED: glFogxvOES\n");
}
void
my_glFrustumxOES(GLfixed left, GLfixed right, GLfixed bottom, GLfixed top, GLfixed zNear, GLfixed zFar)
{
    WRAPPERS_DEBUG_PRINTF("glFrustumxOES()\n", left, right, bottom, top, zNear, zFar);
    /* No CALL */ printf("UNIMPLEMENTED: glFrustumxOES\n");
}
void
my_glGetClipPlanexOES(GLenum pname, GLfixed eqn[4])
{
    WRAPPERS_DEBUG_PRINTF("glGetClipPlanexOES()\n", pname, eqn);
    /* No CALL */ printf("UNIMPLEMENTED: glGetClipPlanexOES\n");
}
void
my_glGetFixedvOES(GLenum pname, GLfixed *params)
{
    WRAPPERS_DEBUG_PRINTF("glGetFixedvOES()\n", pname, params);
    /* No CALL */ printf("UNIMPLEMENTED: glGetFixedvOES\n");
}
void
my_glGetLightxvOES(GLenum light, GLenum pname, GLfixed *params)
{
    WRAPPERS_DEBUG_PRINTF("glGetLightxvOES()\n", light, pname, params);
    /* No CALL */ printf("UNIMPLEMENTED: glGetLightxvOES\n");
}
void
my_glGetMaterialxvOES(GLenum face, GLenum pname, GLfixed *params)
{
    WRAPPERS_DEBUG_PRINTF("glGetMaterialxvOES()\n", face, pname, params);
    /* No CALL */ printf("UNIMPLEMENTED: glGetMaterialxvOES\n");
}
void
my_glGetTexEnvxvOES(GLenum env, GLenum pname, GLfixed *params)
{
    WRAPPERS_DEBUG_PRINTF("glGetTexEnvxvOES()\n", env, pname, params);
    /* No CALL */ printf("UNIMPLEMENTED: glGetTexEnvxvOES\n");
}
void
my_glGetTexParameterxvOES(GLenum target, GLenum pname, GLfixed *params)
{
    WRAPPERS_DEBUG_PRINTF("glGetTexParameterxvOES()\n", target, pname, params);
    /* No CALL */ printf("UNIMPLEMENTED: glGetTexParameterxvOES\n");
}
void
my_glLightModelxOES(GLenum pname, GLfixed param)
{
    WRAPPERS_DEBUG_PRINTF("glLightModelxOES()\n", pname, param);
    /* No CALL */ printf("UNIMPLEMENTED: glLightModelxOES\n");
}
void
my_glLightModelxvOES(GLenum pname, const GLfixed *params)
{
    WRAPPERS_DEBUG_PRINTF("glLightModelxvOES()\n", pname, params);
    /* No CALL */ printf("UNIMPLEMENTED: glLightModelxvOES\n");
}
void
my_glLightxOES(GLenum light, GLenum pname, GLfixed param)
{
    WRAPPERS_DEBUG_PRINTF("glLightxOES()\n", light, pname, param);
    /* No CALL */ printf("UNIMPLEMENTED: glLightxOES\n");
}
void
my_glLightxvOES(GLenum light, GLenum pname, const GLfixed *params)
{
    WRAPPERS_DEBUG_PRINTF("glLightxvOES()\n", light, pname, params);
    /* No CALL */ printf("UNIMPLEMENTED: glLightxvOES\n");
}
void
my_glLineWidthxOES(GLfixed width)
{
    WRAPPERS_DEBUG_PRINTF("glLineWidthxOES()\n", width);
    /* No CALL */ printf("UNIMPLEMENTED: glLineWidthxOES\n");
}
void
my_glLoadMatrixxOES(const GLfixed *m)
{
    WRAPPERS_DEBUG_PRINTF("glLoadMatrixxOES()\n", m);
    /* No CALL */ printf("UNIMPLEMENTED: glLoadMatrixxOES\n");
}
void
my_glMaterialxOES(GLenum face, GLenum pname, GLfixed param)
{
    WRAPPERS_DEBUG_PRINTF("glMaterialxOES()\n", face, pname, param);
    /* No CALL */ printf("UNIMPLEMENTED: glMaterialxOES\n");
}
void
my_glMaterialxvOES(GLenum face, GLenum pname, const GLfixed *params)
{
    WRAPPERS_DEBUG_PRINTF("glMaterialxvOES()\n", face, pname, params);
    /* No CALL */ printf("UNIMPLEMENTED: glMaterialxvOES\n");
}
void
my_glMultMatrixxOES(const GLfixed *m)
{
    WRAPPERS_DEBUG_PRINTF("glMultMatrixxOES()\n", m);
    /* No CALL */ printf("UNIMPLEMENTED: glMultMatrixxOES\n");
}
void
my_glMultiTexCoord4xOES(GLenum target, GLfixed s, GLfixed t, GLfixed r, GLfixed q)
{
    WRAPPERS_DEBUG_PRINTF("glMultiTexCoord4xOES()\n", target, s, t, r, q);
    /* No CALL */ printf("UNIMPLEMENTED: glMultiTexCoord4xOES\n");
}
void
my_glNormal3xOES(GLfixed nx, GLfixed ny, GLfixed nz)
{
    WRAPPERS_DEBUG_PRINTF("glNormal3xOES()\n", nx, ny, nz);
    /* No CALL */ printf("UNIMPLEMENTED: glNormal3xOES\n");
}
void
my_glOrthoxOES(GLfixed left, GLfixed right, GLfixed bottom, GLfixed top, GLfixed zNear, GLfixed zFar)
{
    WRAPPERS_DEBUG_PRINTF("glOrthoxOES()\n", left, right, bottom, top, zNear, zFar);
    /* No CALL */ printf("UNIMPLEMENTED: glOrthoxOES\n");
}
void
my_glPointParameterxOES(GLenum pname, GLfixed param)
{
    WRAPPERS_DEBUG_PRINTF("glPointParameterxOES()\n", pname, param);
    /* No CALL */ printf("UNIMPLEMENTED: glPointParameterxOES\n");
}
void
my_glPointParameterxvOES(GLenum pname, const GLfixed *params)
{
    WRAPPERS_DEBUG_PRINTF("glPointParameterxvOES()\n", pname, params);
    /* No CALL */ printf("UNIMPLEMENTED: glPointParameterxvOES\n");
}
void
my_glPointSizexOES(GLfixed size)
{
    WRAPPERS_DEBUG_PRINTF("glPointSizexOES()\n", size);
    /* No CALL */ printf("UNIMPLEMENTED: glPointSizexOES\n");
}
void
my_glPolygonOffsetxOES(GLfixed factor, GLfixed units)
{
    WRAPPERS_DEBUG_PRINTF("glPolygonOffsetxOES()\n", factor, units);
    /* No CALL */ printf("UNIMPLEMENTED: glPolygonOffsetxOES\n");
}
void
my_glRotatexOES(GLfixed angle, GLfixed x, GLfixed y, GLfixed z)
{
    WRAPPERS_DEBUG_PRINTF("glRotatexOES()\n", angle, x, y, z);
    /* No CALL */ printf("UNIMPLEMENTED: glRotatexOES\n");
}
void
my_glSampleCoveragexOES(GLclampx value, GLboolean invert)
{
    WRAPPERS_DEBUG_PRINTF("glSampleCoveragexOES()\n", value, invert);
    /* No CALL */ printf("UNIMPLEMENTED: glSampleCoveragexOES\n");
}
void
my_glScalexOES(GLfixed x, GLfixed y, GLfixed z)
{
    WRAPPERS_DEBUG_PRINTF("glScalexOES()\n", x, y, z);
    /* No CALL */ printf("UNIMPLEMENTED: glScalexOES\n");
}
void
my_glTexEnvxOES(GLenum target, GLenum pname, GLfixed param)
{
    WRAPPERS_DEBUG_PRINTF("glTexEnvxOES()\n", target, pname, param);
    /* No CALL */ printf("UNIMPLEMENTED: glTexEnvxOES\n");
}
void
my_glTexEnvxvOES(GLenum target, GLenum pname, const GLfixed *params)
{
    WRAPPERS_DEBUG_PRINTF("glTexEnvxvOES()\n", target, pname, params);
    /* No CALL */ printf("UNIMPLEMENTED: glTexEnvxvOES\n");
}
void
my_glTexParameterxOES(GLenum target, GLenum pname, GLfixed param)
{
    WRAPPERS_DEBUG_PRINTF("glTexParameterxOES()\n", target, pname, param);
    /* No CALL */ printf("UNIMPLEMENTED: glTexParameterxOES\n");
}
void
my_glTexParameterxvOES(GLenum target, GLenum pname, const GLfixed *params)
{
    WRAPPERS_DEBUG_PRINTF("glTexParameterxvOES()\n", target, pname, params);
    /* No CALL */ printf("UNIMPLEMENTED: glTexParameterxvOES\n");
}
void
my_glTranslatexOES(GLfixed x, GLfixed y, GLfixed z)
{
    WRAPPERS_DEBUG_PRINTF("glTranslatexOES()\n", x, y, z);
    /* No CALL */ printf("UNIMPLEMENTED: glTranslatexOES\n");
}
GLboolean
my_glIsRenderbufferOES(GLuint renderbuffer)
{
    WRAPPERS_DEBUG_PRINTF("glIsRenderbufferOES()\n", renderbuffer);
    return extensions.glIsRenderbufferOES(renderbuffer);
}
void
my_glBindRenderbufferOES(GLenum target, GLuint renderbuffer)
{
    WRAPPERS_DEBUG_PRINTF("glBindRenderbufferOES()\n", target, renderbuffer);
    extensions.glBindRenderbufferOES(target, renderbuffer);
}
void
my_glDeleteRenderbuffersOES(GLsizei n, const GLuint *renderbuffers)
{
    WRAPPERS_DEBUG_PRINTF("glDeleteRenderbuffersOES()\n", n, renderbuffers);
    extensions.glDeleteRenderbuffersOES(n, renderbuffers);
}
void
my_glGenRenderbuffersOES(GLsizei n, GLuint *renderbuffers)
{
    WRAPPERS_DEBUG_PRINTF("my_glGenRenderbuffersOES()\n", n, renderbuffers);
    extensions.glGenRenderbuffersOES(n,renderbuffers);
}
void
my_glRenderbufferStorageOES(GLenum target, GLenum internalformat, GLsizei width, GLsizei height)
{
    WRAPPERS_DEBUG_PRINTF("glRenderbufferStorageOES()\n", target, internalformat, width, height);
    extensions.glRenderbufferStorageOES(target, internalformat, width, height);
}
void
my_glGetRenderbufferParameterivOES(GLenum target, GLenum pname, GLint *params)
{
    WRAPPERS_DEBUG_PRINTF("glGetRenderbufferParameterivOES()\n", target, pname, params);
    extensions.glGetRenderbufferParameterivOES(target,pname,params);
}
GLboolean
my_glIsFramebufferOES(GLuint framebuffer)
{
    WRAPPERS_DEBUG_PRINTF("glIsFramebufferOES()\n", framebuffer);
    return extensions.glIsFramebufferOES(framebuffer);
}
void
my_glBindFramebufferOES(GLenum target, GLuint framebuffer)
{
    WRAPPERS_DEBUG_PRINTF("glBindFramebufferOES()\n", target, framebuffer);
    extensions.glBindFramebufferOES(target,framebuffer);
}
void
my_glDeleteFramebuffersOES(GLsizei n, const GLuint *framebuffers)
{
    WRAPPERS_DEBUG_PRINTF("glDeleteFramebuffersOES()\n", n, framebuffers);
    extensions.glDeleteFramebuffersOES(n, framebuffers);
}
void
my_glGenFramebuffersOES(GLsizei n, GLuint *framebuffers)
{
    WRAPPERS_DEBUG_PRINTF("glGenFramebuffersOES()\n", n, framebuffers);
    extensions.glGenFramebuffersOES(n, framebuffers);
}
GLenum
my_glCheckFramebufferStatusOES(GLenum target)
{
    WRAPPERS_DEBUG_PRINTF("glCheckFramebufferStatusOES()\n", target);
    return extensions.glCheckFramebufferStatusOES(target);
}
void
my_glFramebufferRenderbufferOES(GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer)
{
    WRAPPERS_DEBUG_PRINTF("glFramebufferRenderbufferOES()\n", target, attachment, renderbuffertarget, renderbuffer);
    extensions.glFramebufferRenderbufferOES(target, attachment, renderbuffertarget, renderbuffer);
}
void
my_glFramebufferTexture2DOES(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level)
{
    WRAPPERS_DEBUG_PRINTF("glFramebufferTexture2DOES()\n", target, attachment, textarget, texture, level);
    extensions.glFramebufferTexture2DOES(target,attachment,textarget,texture,level);
}
void
my_glGetFramebufferAttachmentParameterivOES(GLenum target, GLenum attachment, GLenum pname, GLint *params)
{
    WRAPPERS_DEBUG_PRINTF("glGetFramebufferAttachmentParameterivOES()\n", target, attachment, pname, params);
    /* No CALL */ printf("UNIMPLEMENTED: glGetFramebufferAttachmentParameterivOES\n");
}
void
my_glGenerateMipmapOES(GLenum target)
{
    WRAPPERS_DEBUG_PRINTF("glGenerateMipmapOES()\n", target);
    extensions.glGenerateMipmapOES(target);
}
void
my_glCurrentPaletteMatrixOES(GLuint matrixpaletteindex)
{
    WRAPPERS_DEBUG_PRINTF("glCurrentPaletteMatrixOES(%u)\n", matrixpaletteindex);
    extensions.glCurrentPaletteMatrixOES(matrixpaletteindex);
}
void
my_glLoadPaletteFromModelViewMatrixOES()
{
    WRAPPERS_DEBUG_PRINTF("glLoadPaletteFromModelViewMatrixOES()\n");
    extensions.glLoadPaletteFromModelViewMatrixOES();
}
void
my_glMatrixIndexPointerOES(GLint size, GLenum type, GLsizei stride, const GLvoid *pointer)
{
    WRAPPERS_DEBUG_PRINTF("glMatrixIndexPointerOES(%d, %d, %d, %p)\n", size, type, stride, pointer);
    extensions.glMatrixIndexPointerOES(size, type, stride, pointer);
}
void
my_glWeightPointerOES(GLint size, GLenum type, GLsizei stride, const GLvoid *pointer)
{
    WRAPPERS_DEBUG_PRINTF("glWeightPointerOES(%d, %d, %d, %p)\n", size, type, stride, pointer);
    extensions.glWeightPointerOES(size, type, stride, pointer);
}
GLbitfield
my_glQueryMatrixxOES(GLfixed mantissa[16], GLint exponent[16])
{
    WRAPPERS_DEBUG_PRINTF("glQueryMatrixxOES(%p, %p)\n", mantissa, exponent);
    return extensions.glQueryMatrixxOES(mantissa, exponent);
}
void
my_glDepthRangefOES(GLclampf zNear, GLclampf zFar)
{
    WRAPPERS_DEBUG_PRINTF("glDepthRangefOES()\n", zNear, zFar);
    /* No CALL */ printf("UNIMPLEMENTED: glDepthRangefOES\n");
}
void
my_glFrustumfOES(GLfloat left, GLfloat right, GLfloat bottom, GLfloat top, GLfloat zNear, GLfloat zFar)
{
    WRAPPERS_DEBUG_PRINTF("glFrustumfOES()\n", left, right, bottom, top, zNear, zFar);
    /* No CALL */ printf("UNIMPLEMENTED: glFrustumfOES\n");
}
void
my_glOrthofOES(GLfloat left, GLfloat right, GLfloat bottom, GLfloat top, GLfloat zNear, GLfloat zFar)
{
    WRAPPERS_DEBUG_PRINTF("glOrthofOES()\n", left, right, bottom, top, zNear, zFar);
    /* No CALL */ printf("UNIMPLEMENTED: glOrthofOES\n");
}
void
my_glClipPlanefOES(GLenum plane, const GLfloat *equation)
{
    WRAPPERS_DEBUG_PRINTF("glClipPlanefOES()\n", plane, equation);
    /* No CALL */ printf("UNIMPLEMENTED: glClipPlanefOES\n");
}
void
my_glGetClipPlanefOES(GLenum pname, GLfloat eqn[4])
{
    WRAPPERS_DEBUG_PRINTF("glGetClipPlanefOES()\n", pname, eqn);
    /* No CALL */ printf("UNIMPLEMENTED: glGetClipPlanefOES\n");
}
void
my_glClearDepthfOES(GLclampf depth)
{
    WRAPPERS_DEBUG_PRINTF("glClearDepthfOES()\n", depth);
    /* No CALL */ printf("UNIMPLEMENTED: glClearDepthfOES\n");
}
void
my_glTexGenfOES(GLenum coord, GLenum pname, GLfloat param)
{
    WRAPPERS_DEBUG_PRINTF("glTexGenfOES()\n", coord, pname, param);
    /* No CALL */ printf("UNIMPLEMENTED: glTexGenfOES\n");
}
void
my_glTexGenfvOES(GLenum coord, GLenum pname, const GLfloat *params)
{
    WRAPPERS_DEBUG_PRINTF("glTexGenfvOES()\n", coord, pname, params);
    /* No CALL */ printf("UNIMPLEMENTED: glTexGenfvOES\n");
}
void
my_glTexGeniOES(GLenum coord, GLenum pname, GLint param)
{
    WRAPPERS_DEBUG_PRINTF("glTexGeniOES()\n", coord, pname, param);
    /* No CALL */ printf("UNIMPLEMENTED: glTexGeniOES\n");
}
void
my_glTexGenivOES(GLenum coord, GLenum pname, const GLint *params)
{
    WRAPPERS_DEBUG_PRINTF("glTexGenivOES()\n", coord, pname, params);
    /* No CALL */ printf("UNIMPLEMENTED: glTexGenivOES\n");
}
void
my_glTexGenxOES(GLenum coord, GLenum pname, GLfixed param)
{
    WRAPPERS_DEBUG_PRINTF("glTexGenxOES()\n", coord, pname, param);
    /* No CALL */ printf("UNIMPLEMENTED: glTexGenxOES\n");
}
void
my_glTexGenxvOES(GLenum coord, GLenum pname, const GLfixed *params)
{
    WRAPPERS_DEBUG_PRINTF("glTexGenxvOES()\n", coord, pname, params);
    /* No CALL */ printf("UNIMPLEMENTED: glTexGenxvOES\n");
}
void
my_glGetTexGenfvOES(GLenum coord, GLenum pname, GLfloat *params)
{
    WRAPPERS_DEBUG_PRINTF("glGetTexGenfvOES()\n", coord, pname, params);
    /* No CALL */ printf("UNIMPLEMENTED: glGetTexGenfvOES\n");
}
void
my_glGetTexGenivOES(GLenum coord, GLenum pname, GLint *params)
{
    WRAPPERS_DEBUG_PRINTF("glGetTexGenivOES()\n", coord, pname, params);
    /* No CALL */ printf("UNIMPLEMENTED: glGetTexGenivOES\n");
}
void
my_glGetTexGenxvOES(GLenum coord, GLenum pname, GLfixed *params)
{
    WRAPPERS_DEBUG_PRINTF("glGetTexGenxvOES()\n", coord, pname, params);
    /* No CALL */ printf("UNIMPLEMENTED: glGetTexGenxvOES\n");
}
void
my_glEGLImageTargetTexture2DOES(GLenum target, GLeglImageOES image)
{
    WRAPPERS_DEBUG_PRINTF("glEGLImageTargetTexture2DOES()\n", target, image);
    /* No CALL */ printf("UNIMPLEMENTED: glEGLImageTargetTexture2DOES\n");
}
void
my_glEGLImageTargetRenderbufferStorageOES(GLenum target, GLeglImageOES image)
{
    WRAPPERS_DEBUG_PRINTF("glEGLImageTargetRenderbufferStorageOES()\n", target, image);
    /* No CALL */ printf("UNIMPLEMENTED: glEGLImageTargetRenderbufferStorageOES\n");
}
void *
my_glMapBufferOES(GLenum target, GLenum access)
{
    WRAPPERS_DEBUG_PRINTF("glMapBufferOES(%d, %d)\n", target, access);
    return extensions.glMapBufferOES(target, access);
}
GLboolean
my_glUnmapBufferOES(GLenum target)
{
    WRAPPERS_DEBUG_PRINTF("glUnmapBufferOES(%d)\n", target);
    return extensions.glUnmapBufferOES(target);
}
void
my_glGetBufferPointervOES(GLenum target, GLenum pname, void **params)
{
    WRAPPERS_DEBUG_PRINTF("glGetBufferPointervOES(%d, %d, %p)\n", target, pname, params);
    extensions.glGetBufferPointervOES(target, pname, params);
}
void
my_glTexBindStreamIMG(GLint device, GLint deviceoffset)
{
    WRAPPERS_DEBUG_PRINTF("glTexBindStreamIMG()\n", device, deviceoffset);
    /* No CALL */ printf("UNIMPLEMENTED: glTexBindStreamIMG\n");
}
void
my_glGetTexStreamDeviceAttributeivIMG(GLint device, GLenum pname, GLint *params)
{
    WRAPPERS_DEBUG_PRINTF("glGetTexStreamDeviceAttributeivIMG()\n", device, pname, params);
    /* No CALL */ printf("UNIMPLEMENTED: glGetTexStreamDeviceAttributeivIMG\n");
}
const GLubyte *
my_glGetTexStreamDeviceNameIMG(GLint device)
{
    WRAPPERS_DEBUG_PRINTF("glGetTexStreamDeviceNameIMG()\n", device);
    /* No CALL */ printf("UNIMPLEMENTED: glGetTexStreamDeviceNameIMG\n");
    return NULL;
}
void
my_glVertexAttrib4fARB(GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w)
{
    WRAPPERS_DEBUG_PRINTF("glVertexAttrib4fARB()\n", index, x, y, z, w);
    /* No CALL */ printf("UNIMPLEMENTED: glVertexAttrib4fARB\n");
}
void
my_glProgramEnvParameter4fARB(GLenum target, GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w)
{
    WRAPPERS_DEBUG_PRINTF("glProgramEnvParameter4fARB()\n", target, index, x, y, z, w);
    /* No CALL */ printf("UNIMPLEMENTED: glProgramEnvParameter4fARB\n");
}
void
my_glProgramEnvParameter4fvARB(GLenum target, GLuint index, const GLfloat *params)
{
    WRAPPERS_DEBUG_PRINTF("glProgramEnvParameter4fvARB()\n", target, index, params);
    /* No CALL */ printf("UNIMPLEMENTED: glProgramEnvParameter4fvARB\n");
}
void
my_glProgramLocalParameter4fARB(GLenum target, GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w)
{
    WRAPPERS_DEBUG_PRINTF("glProgramLocalParameter4fARB()\n", target, index, x, y, z, w);
    /* No CALL */ printf("UNIMPLEMENTED: glProgramLocalParameter4fARB\n");
}
void
my_glProgramLocalParameter4fvARB(GLenum target, GLuint index, const GLfloat *params)
{
    WRAPPERS_DEBUG_PRINTF("glProgramLocalParameter4fvARB()\n", target, index, params);
    /* No CALL */ printf("UNIMPLEMENTED: glProgramLocalParameter4fvARB\n");
}
void
my_glVertexAttrib4xIMG(GLuint index, GLfixed x, GLfixed y, GLfixed z, GLfixed w)
{
    WRAPPERS_DEBUG_PRINTF("glVertexAttrib4xIMG()\n", index, x, y, z, w);
    /* No CALL */ printf("UNIMPLEMENTED: glVertexAttrib4xIMG\n");
}
void
my_glProgramLocalParameter4xIMG(GLenum target, GLuint index, GLfixed x, GLfixed y, GLfixed z, GLfixed w)
{
    WRAPPERS_DEBUG_PRINTF("glProgramLocalParameter4xIMG()\n", target, index, x, y, z, w);
    /* No CALL */ printf("UNIMPLEMENTED: glProgramLocalParameter4xIMG\n");
}
void
my_glProgramLocalParameter4xvIMG(GLenum target, GLuint index, const GLfixed *params)
{
    WRAPPERS_DEBUG_PRINTF("glProgramLocalParameter4xvIMG()\n", target, index, params);
    /* No CALL */ printf("UNIMPLEMENTED: glProgramLocalParameter4xvIMG\n");
}
void
my_glProgramEnvParameter4xIMG(GLenum target, GLuint index, GLfixed x, GLfixed y, GLfixed z, GLfixed w)
{
    WRAPPERS_DEBUG_PRINTF("glProgramEnvParameter4xIMG()\n", target, index, x, y, z, w);
    /* No CALL */ printf("UNIMPLEMENTED: glProgramEnvParameter4xIMG\n");
}
void
my_glProgramEnvParameter4xvIMG(GLenum target, GLuint index, const GLfixed *params)
{
    WRAPPERS_DEBUG_PRINTF("glProgramEnvParameter4xvIMG()\n", target, index, params);
    /* No CALL */ printf("UNIMPLEMENTED: glProgramEnvParameter4xvIMG\n");
}
void
my_glVertexAttribPointerARB(GLuint index, GLsizei size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid *pointer)
{
    WRAPPERS_DEBUG_PRINTF("glVertexAttribPointerARB()\n", index, size, type, normalized, stride, pointer);
    /* No CALL */ printf("UNIMPLEMENTED: glVertexAttribPointerARB\n");
}
void
my_glEnableVertexAttribArrayARB(GLuint index)
{
    WRAPPERS_DEBUG_PRINTF("glEnableVertexAttribArrayARB()\n", index);
    /* No CALL */ printf("UNIMPLEMENTED: glEnableVertexAttribArrayARB\n");
}
void
my_glDisableVertexAttribArrayARB(GLuint index)
{
    WRAPPERS_DEBUG_PRINTF("glDisableVertexAttribArrayARB()\n", index);
    /* No CALL */ printf("UNIMPLEMENTED: glDisableVertexAttribArrayARB\n");
}
void
my_glProgramStringARB(GLenum target, GLenum format, GLsizei len, const void *string)
{
    WRAPPERS_DEBUG_PRINTF("glProgramStringARB()\n", target, format, len, string);
    /* No CALL */ printf("UNIMPLEMENTED: glProgramStringARB\n");
}
void
my_glBindProgramARB(GLenum target, GLuint program)
{
    WRAPPERS_DEBUG_PRINTF("glBindProgramARB()\n", target, program);
    /* No CALL */ printf("UNIMPLEMENTED: glBindProgramARB\n");
}
void
my_glDeleteProgramsARB(GLsizei n, const GLuint *programs)
{
    WRAPPERS_DEBUG_PRINTF("glDeleteProgramsARB()\n", n, programs);
    /* No CALL */ printf("UNIMPLEMENTED: glDeleteProgramsARB\n");
}
void
my_glGenProgramsARB(GLsizei n, GLuint *programs)
{
    WRAPPERS_DEBUG_PRINTF("glGenProgramsARB()\n", n, programs);
    /* No CALL */ printf("UNIMPLEMENTED: glGenProgramsARB\n");
}
void
my_glMultiDrawArraysEXT(GLenum mode, GLint *first, GLsizei *count, GLsizei primcount)
{
    WRAPPERS_DEBUG_PRINTF("glMultiDrawArraysEXT()\n", mode, first, count, primcount);
    /* No CALL */ printf("UNIMPLEMENTED: glMultiDrawArraysEXT\n");
}
void
my_glMultiDrawElementsEXT(GLenum mode, const GLsizei *count, GLenum type, const GLvoid **indices, GLsizei primcount)
{
    WRAPPERS_DEBUG_PRINTF("glMultiDrawElementsEXT()\n", mode, count, type, indices, primcount);
    /* No CALL */ printf("UNIMPLEMENTED: glMultiDrawElementsEXT\n");
}
#endif /* APKENV_GLES */
