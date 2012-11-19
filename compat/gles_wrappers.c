#ifdef APKENV_GLES
#include "gles_wrappers.h"
#include <assert.h>
#ifdef APKENV_DEBUG
#  define WRAPPERS_DEBUG_PRINTF(...) printf(__VA_ARGS__)
#else
#  define WRAPPERS_DEBUG_PRINTF(...)
#endif
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
    WRAPPERS_DEBUG_PRINTF("glDrawElements()\n", mode, count, type, indices);
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
    WRAPPERS_DEBUG_PRINTF("glEnableClientState()\n", array);
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
    WRAPPERS_DEBUG_PRINTF("glReadPixels()\n", x, y, width, height, format, type, pixels);
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
    WRAPPERS_DEBUG_PRINTF("glTexImage2D()\n", target, level, internalformat, width, height, border, format, type, pixels);
    glTexImage2D(target, level, internalformat, width, height, border, format, type, pixels);
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
    WRAPPERS_DEBUG_PRINTF("glViewport()\n", x, y, width, height);
    glViewport(x, y, width, height);
}
void
my_glPointSizePointerOES(GLenum type, GLsizei stride, const GLvoid *pointer)
{
    WRAPPERS_DEBUG_PRINTF("glPointSizePointerOES()\n", type, stride, pointer);
    /* No CALL */ assert(0/*glPointSizePointerOES*/);
}
void
my_glBlendEquationSeparateOES(GLenum modeRGB, GLenum modeAlpha)
{
    WRAPPERS_DEBUG_PRINTF("glBlendEquationSeparateOES()\n", modeRGB, modeAlpha);
    /* No CALL */ assert(0/*glBlendEquationSeparateOES*/);
}
void
my_glBlendFuncSeparateOES(GLenum srcRGB, GLenum dstRGB, GLenum srcAlpha, GLenum dstAlpha)
{
    WRAPPERS_DEBUG_PRINTF("glBlendFuncSeparateOES()\n", srcRGB, dstRGB, srcAlpha, dstAlpha);
    /* No CALL */ assert(0/*glBlendFuncSeparateOES*/);
}
void
my_glBlendEquationOES(GLenum mode)
{
    WRAPPERS_DEBUG_PRINTF("glBlendEquationOES()\n", mode);
    /* No CALL */ assert(0/*glBlendEquationOES*/);
}
void
my_glDrawTexsOES(GLshort x, GLshort y, GLshort z, GLshort width, GLshort height)
{
    WRAPPERS_DEBUG_PRINTF("glDrawTexsOES()\n", x, y, z, width, height);
    /* No CALL */ assert(0/*glDrawTexsOES*/);
}
void
my_glDrawTexiOES(GLint x, GLint y, GLint z, GLint width, GLint height)
{
    WRAPPERS_DEBUG_PRINTF("glDrawTexiOES()\n", x, y, z, width, height);
    /* No CALL */ assert(0/*glDrawTexiOES*/);
}
void
my_glDrawTexxOES(GLfixed x, GLfixed y, GLfixed z, GLfixed width, GLfixed height)
{
    WRAPPERS_DEBUG_PRINTF("glDrawTexxOES()\n", x, y, z, width, height);
    /* No CALL */ assert(0/*glDrawTexxOES*/);
}
void
my_glDrawTexsvOES(const GLshort *coords)
{
    WRAPPERS_DEBUG_PRINTF("glDrawTexsvOES()\n", coords);
    /* No CALL */ assert(0/*glDrawTexsvOES*/);
}
void
my_glDrawTexivOES(const GLint *coords)
{
    WRAPPERS_DEBUG_PRINTF("glDrawTexivOES()\n", coords);
    /* No CALL */ assert(0/*glDrawTexivOES*/);
}
void
my_glDrawTexxvOES(const GLfixed *coords)
{
    WRAPPERS_DEBUG_PRINTF("glDrawTexxvOES()\n", coords);
    /* No CALL */ assert(0/*glDrawTexxvOES*/);
}
void
my_glDrawTexfOES(GLfloat x, GLfloat y, GLfloat z, GLfloat width, GLfloat height)
{
    WRAPPERS_DEBUG_PRINTF("glDrawTexfOES()\n", x, y, z, width, height);
    /* No CALL */ assert(0/*glDrawTexfOES*/);
}
void
my_glDrawTexfvOES(const GLfloat *coords)
{
    WRAPPERS_DEBUG_PRINTF("glDrawTexfvOES()\n", coords);
    /* No CALL */ assert(0/*glDrawTexfvOES*/);
}
void
my_glAlphaFuncxOES(GLenum func, GLclampx ref)
{
    WRAPPERS_DEBUG_PRINTF("glAlphaFuncxOES()\n", func, ref);
    /* No CALL */ assert(0/*glAlphaFuncxOES*/);
}
void
my_glClearColorxOES(GLclampx red, GLclampx green, GLclampx blue, GLclampx alpha)
{
    WRAPPERS_DEBUG_PRINTF("glClearColorxOES()\n", red, green, blue, alpha);
    /* No CALL */ assert(0/*glClearColorxOES*/);
}
void
my_glClearDepthxOES(GLclampx depth)
{
    WRAPPERS_DEBUG_PRINTF("glClearDepthxOES()\n", depth);
    /* No CALL */ assert(0/*glClearDepthxOES*/);
}
void
my_glClipPlanexOES(GLenum plane, const GLfixed *equation)
{
    WRAPPERS_DEBUG_PRINTF("glClipPlanexOES()\n", plane, equation);
    /* No CALL */ assert(0/*glClipPlanexOES*/);
}
void
my_glColor4xOES(GLfixed red, GLfixed green, GLfixed blue, GLfixed alpha)
{
    WRAPPERS_DEBUG_PRINTF("glColor4xOES()\n", red, green, blue, alpha);
    /* No CALL */ assert(0/*glColor4xOES*/);
}
void
my_glDepthRangexOES(GLclampx zNear, GLclampx zFar)
{
    WRAPPERS_DEBUG_PRINTF("glDepthRangexOES()\n", zNear, zFar);
    /* No CALL */ assert(0/*glDepthRangexOES*/);
}
void
my_glFogxOES(GLenum pname, GLfixed param)
{
    WRAPPERS_DEBUG_PRINTF("glFogxOES()\n", pname, param);
    /* No CALL */ assert(0/*glFogxOES*/);
}
void
my_glFogxvOES(GLenum pname, const GLfixed *params)
{
    WRAPPERS_DEBUG_PRINTF("glFogxvOES()\n", pname, params);
    /* No CALL */ assert(0/*glFogxvOES*/);
}
void
my_glFrustumxOES(GLfixed left, GLfixed right, GLfixed bottom, GLfixed top, GLfixed zNear, GLfixed zFar)
{
    WRAPPERS_DEBUG_PRINTF("glFrustumxOES()\n", left, right, bottom, top, zNear, zFar);
    /* No CALL */ assert(0/*glFrustumxOES*/);
}
void
my_glGetClipPlanexOES(GLenum pname, GLfixed eqn[4])
{
    WRAPPERS_DEBUG_PRINTF("glGetClipPlanexOES()\n", pname, eqn);
    /* No CALL */ assert(0/*glGetClipPlanexOES*/);
}
void
my_glGetFixedvOES(GLenum pname, GLfixed *params)
{
    WRAPPERS_DEBUG_PRINTF("glGetFixedvOES()\n", pname, params);
    /* No CALL */ assert(0/*glGetFixedvOES*/);
}
void
my_glGetLightxvOES(GLenum light, GLenum pname, GLfixed *params)
{
    WRAPPERS_DEBUG_PRINTF("glGetLightxvOES()\n", light, pname, params);
    /* No CALL */ assert(0/*glGetLightxvOES*/);
}
void
my_glGetMaterialxvOES(GLenum face, GLenum pname, GLfixed *params)
{
    WRAPPERS_DEBUG_PRINTF("glGetMaterialxvOES()\n", face, pname, params);
    /* No CALL */ assert(0/*glGetMaterialxvOES*/);
}
void
my_glGetTexEnvxvOES(GLenum env, GLenum pname, GLfixed *params)
{
    WRAPPERS_DEBUG_PRINTF("glGetTexEnvxvOES()\n", env, pname, params);
    /* No CALL */ assert(0/*glGetTexEnvxvOES*/);
}
void
my_glGetTexParameterxvOES(GLenum target, GLenum pname, GLfixed *params)
{
    WRAPPERS_DEBUG_PRINTF("glGetTexParameterxvOES()\n", target, pname, params);
    /* No CALL */ assert(0/*glGetTexParameterxvOES*/);
}
void
my_glLightModelxOES(GLenum pname, GLfixed param)
{
    WRAPPERS_DEBUG_PRINTF("glLightModelxOES()\n", pname, param);
    /* No CALL */ assert(0/*glLightModelxOES*/);
}
void
my_glLightModelxvOES(GLenum pname, const GLfixed *params)
{
    WRAPPERS_DEBUG_PRINTF("glLightModelxvOES()\n", pname, params);
    /* No CALL */ assert(0/*glLightModelxvOES*/);
}
void
my_glLightxOES(GLenum light, GLenum pname, GLfixed param)
{
    WRAPPERS_DEBUG_PRINTF("glLightxOES()\n", light, pname, param);
    /* No CALL */ assert(0/*glLightxOES*/);
}
void
my_glLightxvOES(GLenum light, GLenum pname, const GLfixed *params)
{
    WRAPPERS_DEBUG_PRINTF("glLightxvOES()\n", light, pname, params);
    /* No CALL */ assert(0/*glLightxvOES*/);
}
void
my_glLineWidthxOES(GLfixed width)
{
    WRAPPERS_DEBUG_PRINTF("glLineWidthxOES()\n", width);
    /* No CALL */ assert(0/*glLineWidthxOES*/);
}
void
my_glLoadMatrixxOES(const GLfixed *m)
{
    WRAPPERS_DEBUG_PRINTF("glLoadMatrixxOES()\n", m);
    /* No CALL */ assert(0/*glLoadMatrixxOES*/);
}
void
my_glMaterialxOES(GLenum face, GLenum pname, GLfixed param)
{
    WRAPPERS_DEBUG_PRINTF("glMaterialxOES()\n", face, pname, param);
    /* No CALL */ assert(0/*glMaterialxOES*/);
}
void
my_glMaterialxvOES(GLenum face, GLenum pname, const GLfixed *params)
{
    WRAPPERS_DEBUG_PRINTF("glMaterialxvOES()\n", face, pname, params);
    /* No CALL */ assert(0/*glMaterialxvOES*/);
}
void
my_glMultMatrixxOES(const GLfixed *m)
{
    WRAPPERS_DEBUG_PRINTF("glMultMatrixxOES()\n", m);
    /* No CALL */ assert(0/*glMultMatrixxOES*/);
}
void
my_glMultiTexCoord4xOES(GLenum target, GLfixed s, GLfixed t, GLfixed r, GLfixed q)
{
    WRAPPERS_DEBUG_PRINTF("glMultiTexCoord4xOES()\n", target, s, t, r, q);
    /* No CALL */ assert(0/*glMultiTexCoord4xOES*/);
}
void
my_glNormal3xOES(GLfixed nx, GLfixed ny, GLfixed nz)
{
    WRAPPERS_DEBUG_PRINTF("glNormal3xOES()\n", nx, ny, nz);
    /* No CALL */ assert(0/*glNormal3xOES*/);
}
void
my_glOrthoxOES(GLfixed left, GLfixed right, GLfixed bottom, GLfixed top, GLfixed zNear, GLfixed zFar)
{
    WRAPPERS_DEBUG_PRINTF("glOrthoxOES()\n", left, right, bottom, top, zNear, zFar);
    /* No CALL */ assert(0/*glOrthoxOES*/);
}
void
my_glPointParameterxOES(GLenum pname, GLfixed param)
{
    WRAPPERS_DEBUG_PRINTF("glPointParameterxOES()\n", pname, param);
    /* No CALL */ assert(0/*glPointParameterxOES*/);
}
void
my_glPointParameterxvOES(GLenum pname, const GLfixed *params)
{
    WRAPPERS_DEBUG_PRINTF("glPointParameterxvOES()\n", pname, params);
    /* No CALL */ assert(0/*glPointParameterxvOES*/);
}
void
my_glPointSizexOES(GLfixed size)
{
    WRAPPERS_DEBUG_PRINTF("glPointSizexOES()\n", size);
    /* No CALL */ assert(0/*glPointSizexOES*/);
}
void
my_glPolygonOffsetxOES(GLfixed factor, GLfixed units)
{
    WRAPPERS_DEBUG_PRINTF("glPolygonOffsetxOES()\n", factor, units);
    /* No CALL */ assert(0/*glPolygonOffsetxOES*/);
}
void
my_glRotatexOES(GLfixed angle, GLfixed x, GLfixed y, GLfixed z)
{
    WRAPPERS_DEBUG_PRINTF("glRotatexOES()\n", angle, x, y, z);
    /* No CALL */ assert(0/*glRotatexOES*/);
}
void
my_glSampleCoveragexOES(GLclampx value, GLboolean invert)
{
    WRAPPERS_DEBUG_PRINTF("glSampleCoveragexOES()\n", value, invert);
    /* No CALL */ assert(0/*glSampleCoveragexOES*/);
}
void
my_glScalexOES(GLfixed x, GLfixed y, GLfixed z)
{
    WRAPPERS_DEBUG_PRINTF("glScalexOES()\n", x, y, z);
    /* No CALL */ assert(0/*glScalexOES*/);
}
void
my_glTexEnvxOES(GLenum target, GLenum pname, GLfixed param)
{
    WRAPPERS_DEBUG_PRINTF("glTexEnvxOES()\n", target, pname, param);
    /* No CALL */ assert(0/*glTexEnvxOES*/);
}
void
my_glTexEnvxvOES(GLenum target, GLenum pname, const GLfixed *params)
{
    WRAPPERS_DEBUG_PRINTF("glTexEnvxvOES()\n", target, pname, params);
    /* No CALL */ assert(0/*glTexEnvxvOES*/);
}
void
my_glTexParameterxOES(GLenum target, GLenum pname, GLfixed param)
{
    WRAPPERS_DEBUG_PRINTF("glTexParameterxOES()\n", target, pname, param);
    /* No CALL */ assert(0/*glTexParameterxOES*/);
}
void
my_glTexParameterxvOES(GLenum target, GLenum pname, const GLfixed *params)
{
    WRAPPERS_DEBUG_PRINTF("glTexParameterxvOES()\n", target, pname, params);
    /* No CALL */ assert(0/*glTexParameterxvOES*/);
}
void
my_glTranslatexOES(GLfixed x, GLfixed y, GLfixed z)
{
    WRAPPERS_DEBUG_PRINTF("glTranslatexOES()\n", x, y, z);
    /* No CALL */ assert(0/*glTranslatexOES*/);
}
GLboolean
my_glIsRenderbufferOES(GLuint renderbuffer)
{
    WRAPPERS_DEBUG_PRINTF("glIsRenderbufferOES()\n", renderbuffer);
    /* No CALL */ assert(0/*glIsRenderbufferOES*/);
}
void
my_glBindRenderbufferOES(GLenum target, GLuint renderbuffer)
{
    WRAPPERS_DEBUG_PRINTF("glBindRenderbufferOES()\n", target, renderbuffer);
    /* No CALL */ assert(0/*glBindRenderbufferOES*/);
}
void
my_glDeleteRenderbuffersOES(GLsizei n, const GLuint *renderbuffers)
{
    WRAPPERS_DEBUG_PRINTF("glDeleteRenderbuffersOES()\n", n, renderbuffers);
    /* No CALL */ assert(0/*glDeleteRenderbuffersOES*/);
}
void
my_glGenRenderbuffersOES(GLsizei n, GLuint *renderbuffers)
{
    WRAPPERS_DEBUG_PRINTF("glGenRenderbuffersOES()\n", n, renderbuffers);
    /* No CALL */ assert(0/*glGenRenderbuffersOES*/);
}
void
my_glRenderbufferStorageOES(GLenum target, GLenum internalformat, GLsizei width, GLsizei height)
{
    WRAPPERS_DEBUG_PRINTF("glRenderbufferStorageOES()\n", target, internalformat, width, height);
    /* No CALL */ assert(0/*glRenderbufferStorageOES*/);
}
void
my_glGetRenderbufferParameterivOES(GLenum target, GLenum pname, GLint *params)
{
    WRAPPERS_DEBUG_PRINTF("glGetRenderbufferParameterivOES()\n", target, pname, params);
    /* No CALL */ assert(0/*glGetRenderbufferParameterivOES*/);
}
GLboolean
my_glIsFramebufferOES(GLuint framebuffer)
{
    WRAPPERS_DEBUG_PRINTF("glIsFramebufferOES()\n", framebuffer);
    /* No CALL */ assert(0/*glIsFramebufferOES*/);
}
void
my_glBindFramebufferOES(GLenum target, GLuint framebuffer)
{
    WRAPPERS_DEBUG_PRINTF("glBindFramebufferOES()\n", target, framebuffer);
    /* No CALL */ assert(0/*glBindFramebufferOES*/);
}
void
my_glDeleteFramebuffersOES(GLsizei n, const GLuint *framebuffers)
{
    WRAPPERS_DEBUG_PRINTF("glDeleteFramebuffersOES()\n", n, framebuffers);
    /* No CALL */ assert(0/*glDeleteFramebuffersOES*/);
}
void
my_glGenFramebuffersOES(GLsizei n, GLuint *framebuffers)
{
    WRAPPERS_DEBUG_PRINTF("glGenFramebuffersOES()\n", n, framebuffers);
    /* No CALL */ assert(0/*glGenFramebuffersOES*/);
}
GLenum
my_glCheckFramebufferStatusOES(GLenum target)
{
    WRAPPERS_DEBUG_PRINTF("glCheckFramebufferStatusOES()\n", target);
    /* No CALL */ assert(0/*glCheckFramebufferStatusOES*/);
}
void
my_glFramebufferRenderbufferOES(GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer)
{
    WRAPPERS_DEBUG_PRINTF("glFramebufferRenderbufferOES()\n", target, attachment, renderbuffertarget, renderbuffer);
    /* No CALL */ assert(0/*glFramebufferRenderbufferOES*/);
}
void
my_glFramebufferTexture2DOES(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level)
{
    WRAPPERS_DEBUG_PRINTF("glFramebufferTexture2DOES()\n", target, attachment, textarget, texture, level);
    /* No CALL */ assert(0/*glFramebufferTexture2DOES*/);
}
void
my_glGetFramebufferAttachmentParameterivOES(GLenum target, GLenum attachment, GLenum pname, GLint *params)
{
    WRAPPERS_DEBUG_PRINTF("glGetFramebufferAttachmentParameterivOES()\n", target, attachment, pname, params);
    /* No CALL */ assert(0/*glGetFramebufferAttachmentParameterivOES*/);
}
void
my_glGenerateMipmapOES(GLenum target)
{
    WRAPPERS_DEBUG_PRINTF("glGenerateMipmapOES()\n", target);
    /* No CALL */ assert(0/*glGenerateMipmapOES*/);
}
void
my_glCurrentPaletteMatrixOES(GLuint matrixpaletteindex)
{
    WRAPPERS_DEBUG_PRINTF("glCurrentPaletteMatrixOES()\n", matrixpaletteindex);
    /* No CALL */ assert(0/*glCurrentPaletteMatrixOES*/);
}
void
my_glLoadPaletteFromModelViewMatrixOES()
{
    WRAPPERS_DEBUG_PRINTF("glLoadPaletteFromModelViewMatrixOES()\n");
    /* No CALL */ assert(0/*glLoadPaletteFromModelViewMatrixOES*/);
}
void
my_glMatrixIndexPointerOES(GLint size, GLenum type, GLsizei stride, const GLvoid *pointer)
{
    WRAPPERS_DEBUG_PRINTF("glMatrixIndexPointerOES()\n", size, type, stride, pointer);
    /* No CALL */ assert(0/*glMatrixIndexPointerOES*/);
}
void
my_glWeightPointerOES(GLint size, GLenum type, GLsizei stride, const GLvoid *pointer)
{
    WRAPPERS_DEBUG_PRINTF("glWeightPointerOES()\n", size, type, stride, pointer);
    /* No CALL */ assert(0/*glWeightPointerOES*/);
}
GLbitfield
my_glQueryMatrixxOES(GLfixed mantissa[16], GLint exponent[16])
{
    WRAPPERS_DEBUG_PRINTF("glQueryMatrixxOES()\n", mantissa, exponent);
    /* No CALL */ assert(0/*glQueryMatrixxOES*/);
}
void
my_glDepthRangefOES(GLclampf zNear, GLclampf zFar)
{
    WRAPPERS_DEBUG_PRINTF("glDepthRangefOES()\n", zNear, zFar);
    /* No CALL */ assert(0/*glDepthRangefOES*/);
}
void
my_glFrustumfOES(GLfloat left, GLfloat right, GLfloat bottom, GLfloat top, GLfloat zNear, GLfloat zFar)
{
    WRAPPERS_DEBUG_PRINTF("glFrustumfOES()\n", left, right, bottom, top, zNear, zFar);
    /* No CALL */ assert(0/*glFrustumfOES*/);
}
void
my_glOrthofOES(GLfloat left, GLfloat right, GLfloat bottom, GLfloat top, GLfloat zNear, GLfloat zFar)
{
    WRAPPERS_DEBUG_PRINTF("glOrthofOES()\n", left, right, bottom, top, zNear, zFar);
    /* No CALL */ assert(0/*glOrthofOES*/);
}
void
my_glClipPlanefOES(GLenum plane, const GLfloat *equation)
{
    WRAPPERS_DEBUG_PRINTF("glClipPlanefOES()\n", plane, equation);
    /* No CALL */ assert(0/*glClipPlanefOES*/);
}
void
my_glGetClipPlanefOES(GLenum pname, GLfloat eqn[4])
{
    WRAPPERS_DEBUG_PRINTF("glGetClipPlanefOES()\n", pname, eqn);
    /* No CALL */ assert(0/*glGetClipPlanefOES*/);
}
void
my_glClearDepthfOES(GLclampf depth)
{
    WRAPPERS_DEBUG_PRINTF("glClearDepthfOES()\n", depth);
    /* No CALL */ assert(0/*glClearDepthfOES*/);
}
void
my_glTexGenfOES(GLenum coord, GLenum pname, GLfloat param)
{
    WRAPPERS_DEBUG_PRINTF("glTexGenfOES()\n", coord, pname, param);
    /* No CALL */ assert(0/*glTexGenfOES*/);
}
void
my_glTexGenfvOES(GLenum coord, GLenum pname, const GLfloat *params)
{
    WRAPPERS_DEBUG_PRINTF("glTexGenfvOES()\n", coord, pname, params);
    /* No CALL */ assert(0/*glTexGenfvOES*/);
}
void
my_glTexGeniOES(GLenum coord, GLenum pname, GLint param)
{
    WRAPPERS_DEBUG_PRINTF("glTexGeniOES()\n", coord, pname, param);
    /* No CALL */ assert(0/*glTexGeniOES*/);
}
void
my_glTexGenivOES(GLenum coord, GLenum pname, const GLint *params)
{
    WRAPPERS_DEBUG_PRINTF("glTexGenivOES()\n", coord, pname, params);
    /* No CALL */ assert(0/*glTexGenivOES*/);
}
void
my_glTexGenxOES(GLenum coord, GLenum pname, GLfixed param)
{
    WRAPPERS_DEBUG_PRINTF("glTexGenxOES()\n", coord, pname, param);
    /* No CALL */ assert(0/*glTexGenxOES*/);
}
void
my_glTexGenxvOES(GLenum coord, GLenum pname, const GLfixed *params)
{
    WRAPPERS_DEBUG_PRINTF("glTexGenxvOES()\n", coord, pname, params);
    /* No CALL */ assert(0/*glTexGenxvOES*/);
}
void
my_glGetTexGenfvOES(GLenum coord, GLenum pname, GLfloat *params)
{
    WRAPPERS_DEBUG_PRINTF("glGetTexGenfvOES()\n", coord, pname, params);
    /* No CALL */ assert(0/*glGetTexGenfvOES*/);
}
void
my_glGetTexGenivOES(GLenum coord, GLenum pname, GLint *params)
{
    WRAPPERS_DEBUG_PRINTF("glGetTexGenivOES()\n", coord, pname, params);
    /* No CALL */ assert(0/*glGetTexGenivOES*/);
}
void
my_glGetTexGenxvOES(GLenum coord, GLenum pname, GLfixed *params)
{
    WRAPPERS_DEBUG_PRINTF("glGetTexGenxvOES()\n", coord, pname, params);
    /* No CALL */ assert(0/*glGetTexGenxvOES*/);
}
void
my_glEGLImageTargetTexture2DOES(GLenum target, GLeglImageOES image)
{
    WRAPPERS_DEBUG_PRINTF("glEGLImageTargetTexture2DOES()\n", target, image);
    /* No CALL */ assert(0/*glEGLImageTargetTexture2DOES*/);
}
void
my_glEGLImageTargetRenderbufferStorageOES(GLenum target, GLeglImageOES image)
{
    WRAPPERS_DEBUG_PRINTF("glEGLImageTargetRenderbufferStorageOES()\n", target, image);
    /* No CALL */ assert(0/*glEGLImageTargetRenderbufferStorageOES*/);
}
void *
my_glMapBufferOES(GLenum target, GLenum access)
{
    WRAPPERS_DEBUG_PRINTF("glMapBufferOES()\n", target, access);
    /* No CALL */ assert(0/*glMapBufferOES*/);
}
GLboolean
my_glUnmapBufferOES(GLenum target)
{
    WRAPPERS_DEBUG_PRINTF("glUnmapBufferOES()\n", target);
    /* No CALL */ assert(0/*glUnmapBufferOES*/);
}
void
my_glGetBufferPointervOES(GLenum target, GLenum pname, void **params)
{
    WRAPPERS_DEBUG_PRINTF("glGetBufferPointervOES()\n", target, pname, params);
    /* No CALL */ assert(0/*glGetBufferPointervOES*/);
}
void
my_glTexBindStreamIMG(GLint device, GLint deviceoffset)
{
    WRAPPERS_DEBUG_PRINTF("glTexBindStreamIMG()\n", device, deviceoffset);
    /* No CALL */ assert(0/*glTexBindStreamIMG*/);
}
void
my_glGetTexStreamDeviceAttributeivIMG(GLint device, GLenum pname, GLint *params)
{
    WRAPPERS_DEBUG_PRINTF("glGetTexStreamDeviceAttributeivIMG()\n", device, pname, params);
    /* No CALL */ assert(0/*glGetTexStreamDeviceAttributeivIMG*/);
}
const GLubyte *
my_glGetTexStreamDeviceNameIMG(GLint device)
{
    WRAPPERS_DEBUG_PRINTF("glGetTexStreamDeviceNameIMG()\n", device);
    /* No CALL */ assert(0/*glGetTexStreamDeviceNameIMG*/);
}
void
my_glVertexAttrib4fARB(GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w)
{
    WRAPPERS_DEBUG_PRINTF("glVertexAttrib4fARB()\n", index, x, y, z, w);
    /* No CALL */ assert(0/*glVertexAttrib4fARB*/);
}
void
my_glProgramEnvParameter4fARB(GLenum target, GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w)
{
    WRAPPERS_DEBUG_PRINTF("glProgramEnvParameter4fARB()\n", target, index, x, y, z, w);
    /* No CALL */ assert(0/*glProgramEnvParameter4fARB*/);
}
void
my_glProgramEnvParameter4fvARB(GLenum target, GLuint index, const GLfloat *params)
{
    WRAPPERS_DEBUG_PRINTF("glProgramEnvParameter4fvARB()\n", target, index, params);
    /* No CALL */ assert(0/*glProgramEnvParameter4fvARB*/);
}
void
my_glProgramLocalParameter4fARB(GLenum target, GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w)
{
    WRAPPERS_DEBUG_PRINTF("glProgramLocalParameter4fARB()\n", target, index, x, y, z, w);
    /* No CALL */ assert(0/*glProgramLocalParameter4fARB*/);
}
void
my_glProgramLocalParameter4fvARB(GLenum target, GLuint index, const GLfloat *params)
{
    WRAPPERS_DEBUG_PRINTF("glProgramLocalParameter4fvARB()\n", target, index, params);
    /* No CALL */ assert(0/*glProgramLocalParameter4fvARB*/);
}
void
my_glVertexAttrib4xIMG(GLuint index, GLfixed x, GLfixed y, GLfixed z, GLfixed w)
{
    WRAPPERS_DEBUG_PRINTF("glVertexAttrib4xIMG()\n", index, x, y, z, w);
    /* No CALL */ assert(0/*glVertexAttrib4xIMG*/);
}
void
my_glProgramLocalParameter4xIMG(GLenum target, GLuint index, GLfixed x, GLfixed y, GLfixed z, GLfixed w)
{
    WRAPPERS_DEBUG_PRINTF("glProgramLocalParameter4xIMG()\n", target, index, x, y, z, w);
    /* No CALL */ assert(0/*glProgramLocalParameter4xIMG*/);
}
void
my_glProgramLocalParameter4xvIMG(GLenum target, GLuint index, const GLfixed *params)
{
    WRAPPERS_DEBUG_PRINTF("glProgramLocalParameter4xvIMG()\n", target, index, params);
    /* No CALL */ assert(0/*glProgramLocalParameter4xvIMG*/);
}
void
my_glProgramEnvParameter4xIMG(GLenum target, GLuint index, GLfixed x, GLfixed y, GLfixed z, GLfixed w)
{
    WRAPPERS_DEBUG_PRINTF("glProgramEnvParameter4xIMG()\n", target, index, x, y, z, w);
    /* No CALL */ assert(0/*glProgramEnvParameter4xIMG*/);
}
void
my_glProgramEnvParameter4xvIMG(GLenum target, GLuint index, const GLfixed *params)
{
    WRAPPERS_DEBUG_PRINTF("glProgramEnvParameter4xvIMG()\n", target, index, params);
    /* No CALL */ assert(0/*glProgramEnvParameter4xvIMG*/);
}
void
my_glVertexAttribPointerARB(GLuint index, GLsizei size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid *pointer)
{
    WRAPPERS_DEBUG_PRINTF("glVertexAttribPointerARB()\n", index, size, type, normalized, stride, pointer);
    /* No CALL */ assert(0/*glVertexAttribPointerARB*/);
}
void
my_glEnableVertexAttribArrayARB(GLuint index)
{
    WRAPPERS_DEBUG_PRINTF("glEnableVertexAttribArrayARB()\n", index);
    /* No CALL */ assert(0/*glEnableVertexAttribArrayARB*/);
}
void
my_glDisableVertexAttribArrayARB(GLuint index)
{
    WRAPPERS_DEBUG_PRINTF("glDisableVertexAttribArrayARB()\n", index);
    /* No CALL */ assert(0/*glDisableVertexAttribArrayARB*/);
}
void
my_glProgramStringARB(GLenum target, GLenum format, GLsizei len, const void *string)
{
    WRAPPERS_DEBUG_PRINTF("glProgramStringARB()\n", target, format, len, string);
    /* No CALL */ assert(0/*glProgramStringARB*/);
}
void
my_glBindProgramARB(GLenum target, GLuint program)
{
    WRAPPERS_DEBUG_PRINTF("glBindProgramARB()\n", target, program);
    /* No CALL */ assert(0/*glBindProgramARB*/);
}
void
my_glDeleteProgramsARB(GLsizei n, const GLuint *programs)
{
    WRAPPERS_DEBUG_PRINTF("glDeleteProgramsARB()\n", n, programs);
    /* No CALL */ assert(0/*glDeleteProgramsARB*/);
}
void
my_glGenProgramsARB(GLsizei n, GLuint *programs)
{
    WRAPPERS_DEBUG_PRINTF("glGenProgramsARB()\n", n, programs);
    /* No CALL */ assert(0/*glGenProgramsARB*/);
}
void
my_glMultiDrawArraysEXT(GLenum mode, GLint *first, GLsizei *count, GLsizei primcount)
{
    WRAPPERS_DEBUG_PRINTF("glMultiDrawArraysEXT()\n", mode, first, count, primcount);
    /* No CALL */ assert(0/*glMultiDrawArraysEXT*/);
}
void
my_glMultiDrawElementsEXT(GLenum mode, const GLsizei *count, GLenum type, const GLvoid **indices, GLsizei primcount)
{
    WRAPPERS_DEBUG_PRINTF("glMultiDrawElementsEXT()\n", mode, count, type, indices, primcount);
    /* No CALL */ assert(0/*glMultiDrawElementsEXT*/);
}
#endif /* APKENV_GLES */
