#ifdef APKENV_GLES2
#include "gles2_wrappers.h"
#include <assert.h>
#ifdef APKENV_DEBUG
#  define WRAPPERS_DEBUG_PRINTF(...) printf(__VA_ARGS__)
#else
#  define WRAPPERS_DEBUG_PRINTF(...)
#endif
void
my_glActiveTexture(GLenum texture)
{
    WRAPPERS_DEBUG_PRINTF("glActiveTexture()\n", texture);
    glActiveTexture(texture);
}
void
my_glAttachShader(GLuint program, GLuint shader)
{
    WRAPPERS_DEBUG_PRINTF("glAttachShader()\n", program, shader);
    glAttachShader(program, shader);
}
void
my_glBindAttribLocation(GLuint program, GLuint index, const char *name)
{
    WRAPPERS_DEBUG_PRINTF("glBindAttribLocation()\n", program, index, name);
    glBindAttribLocation(program, index, name);
}
void
my_glBindBuffer(GLenum target, GLuint buffer)
{
    WRAPPERS_DEBUG_PRINTF("glBindBuffer()\n", target, buffer);
    glBindBuffer(target, buffer);
}
void
my_glBindFramebuffer(GLenum target, GLuint framebuffer)
{
    WRAPPERS_DEBUG_PRINTF("glBindFramebuffer()\n", target, framebuffer);
    glBindFramebuffer(target, framebuffer);
}
void
my_glBindRenderbuffer(GLenum target, GLuint renderbuffer)
{
    WRAPPERS_DEBUG_PRINTF("glBindRenderbuffer()\n", target, renderbuffer);
    glBindRenderbuffer(target, renderbuffer);
}
void
my_glBindTexture(GLenum target, GLuint texture)
{
    WRAPPERS_DEBUG_PRINTF("glBindTexture()\n", target, texture);
    glBindTexture(target, texture);
}
void
my_glBlendColor(GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha)
{
    WRAPPERS_DEBUG_PRINTF("glBlendColor()\n", red, green, blue, alpha);
    glBlendColor(red, green, blue, alpha);
}
void
my_glBlendEquation(GLenum mode)
{
    WRAPPERS_DEBUG_PRINTF("glBlendEquation()\n", mode);
    glBlendEquation(mode);
}
void
my_glBlendEquationSeparate(GLenum modeRGB, GLenum modeAlpha)
{
    WRAPPERS_DEBUG_PRINTF("glBlendEquationSeparate()\n", modeRGB, modeAlpha);
    glBlendEquationSeparate(modeRGB, modeAlpha);
}
void
my_glBlendFunc(GLenum sfactor, GLenum dfactor)
{
    WRAPPERS_DEBUG_PRINTF("glBlendFunc()\n", sfactor, dfactor);
    glBlendFunc(sfactor, dfactor);
}
void
my_glBlendFuncSeparate(GLenum srcRGB, GLenum dstRGB, GLenum srcAlpha, GLenum dstAlpha)
{
    WRAPPERS_DEBUG_PRINTF("glBlendFuncSeparate()\n", srcRGB, dstRGB, srcAlpha, dstAlpha);
    glBlendFuncSeparate(srcRGB, dstRGB, srcAlpha, dstAlpha);
}
void
my_glBufferData(GLenum target, GLsizeiptr size, const void *data, GLenum usage)
{
    WRAPPERS_DEBUG_PRINTF("glBufferData()\n", target, size, data, usage);
    glBufferData(target, size, data, usage);
}
void
my_glBufferSubData(GLenum target, GLintptr offset, GLsizeiptr size, const void *data)
{
    WRAPPERS_DEBUG_PRINTF("glBufferSubData()\n", target, offset, size, data);
    glBufferSubData(target, offset, size, data);
}
GLenum
my_glCheckFramebufferStatus(GLenum target)
{
    WRAPPERS_DEBUG_PRINTF("glCheckFramebufferStatus()\n", target);
    return glCheckFramebufferStatus(target);
}
void
my_glClear(GLbitfield mask)
{
    WRAPPERS_DEBUG_PRINTF("glClear()\n", mask);
    glClear(mask);
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
my_glClearStencil(GLint s)
{
    WRAPPERS_DEBUG_PRINTF("glClearStencil()\n", s);
    glClearStencil(s);
}
void
my_glColorMask(GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha)
{
    WRAPPERS_DEBUG_PRINTF("glColorMask()\n", red, green, blue, alpha);
    glColorMask(red, green, blue, alpha);
}
void
my_glCompileShader(GLuint shader)
{
    WRAPPERS_DEBUG_PRINTF("glCompileShader()\n", shader);
    glCompileShader(shader);
}
void
my_glCompressedTexImage2D(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLsizei imageSize, const void *data)
{
    WRAPPERS_DEBUG_PRINTF("glCompressedTexImage2D()\n", target, level, internalformat, width, height, border, imageSize, data);
    glCompressedTexImage2D(target, level, internalformat, width, height, border, imageSize, data);
}
void
my_glCompressedTexSubImage2D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLsizei imageSize, const void *data)
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
GLuint
my_glCreateProgram()
{
    WRAPPERS_DEBUG_PRINTF("glCreateProgram()\n");
    return glCreateProgram();
}
GLuint
my_glCreateShader(GLenum type)
{
    WRAPPERS_DEBUG_PRINTF("glCreateShader()\n", type);
    return glCreateShader(type);
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
my_glDeleteFramebuffers(GLsizei n, const GLuint *framebuffers)
{
    WRAPPERS_DEBUG_PRINTF("glDeleteFramebuffers()\n", n, framebuffers);
    glDeleteFramebuffers(n, framebuffers);
}
void
my_glDeleteProgram(GLuint program)
{
    WRAPPERS_DEBUG_PRINTF("glDeleteProgram()\n", program);
    glDeleteProgram(program);
}
void
my_glDeleteRenderbuffers(GLsizei n, const GLuint *renderbuffers)
{
    WRAPPERS_DEBUG_PRINTF("glDeleteRenderbuffers()\n", n, renderbuffers);
    glDeleteRenderbuffers(n, renderbuffers);
}
void
my_glDeleteShader(GLuint shader)
{
    WRAPPERS_DEBUG_PRINTF("glDeleteShader()\n", shader);
    glDeleteShader(shader);
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
my_glDepthRangef(GLclampf zNear, GLclampf zFar)
{
    WRAPPERS_DEBUG_PRINTF("glDepthRangef()\n", zNear, zFar);
    glDepthRangef(zNear, zFar);
}
void
my_glDetachShader(GLuint program, GLuint shader)
{
    WRAPPERS_DEBUG_PRINTF("glDetachShader()\n", program, shader);
    glDetachShader(program, shader);
}
void
my_glDisable(GLenum cap)
{
    WRAPPERS_DEBUG_PRINTF("glDisable()\n", cap);
    glDisable(cap);
}
void
my_glDisableVertexAttribArray(GLuint index)
{
    WRAPPERS_DEBUG_PRINTF("glDisableVertexAttribArray()\n", index);
    glDisableVertexAttribArray(index);
}
void
my_glDrawArrays(GLenum mode, GLint first, GLsizei count)
{
    WRAPPERS_DEBUG_PRINTF("glDrawArrays()\n", mode, first, count);
    glDrawArrays(mode, first, count);
}
void
my_glDrawElements(GLenum mode, GLsizei count, GLenum type, const void *indices)
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
my_glEnableVertexAttribArray(GLuint index)
{
    WRAPPERS_DEBUG_PRINTF("glEnableVertexAttribArray()\n", index);
    glEnableVertexAttribArray(index);
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
my_glFramebufferRenderbuffer(GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer)
{
    WRAPPERS_DEBUG_PRINTF("glFramebufferRenderbuffer()\n", target, attachment, renderbuffertarget, renderbuffer);
    glFramebufferRenderbuffer(target, attachment, renderbuffertarget, renderbuffer);
}
void
my_glFramebufferTexture2D(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level)
{
    WRAPPERS_DEBUG_PRINTF("glFramebufferTexture2D()\n", target, attachment, textarget, texture, level);
    glFramebufferTexture2D(target, attachment, textarget, texture, level);
}
void
my_glFrontFace(GLenum mode)
{
    WRAPPERS_DEBUG_PRINTF("glFrontFace()\n", mode);
    glFrontFace(mode);
}
void
my_glGenBuffers(GLsizei n, GLuint *buffers)
{
    WRAPPERS_DEBUG_PRINTF("glGenBuffers()\n", n, buffers);
    glGenBuffers(n, buffers);
}
void
my_glGenerateMipmap(GLenum target)
{
    WRAPPERS_DEBUG_PRINTF("glGenerateMipmap()\n", target);
    glGenerateMipmap(target);
}
void
my_glGenFramebuffers(GLsizei n, GLuint *framebuffers)
{
    WRAPPERS_DEBUG_PRINTF("glGenFramebuffers()\n", n, framebuffers);
    glGenFramebuffers(n, framebuffers);
}
void
my_glGenRenderbuffers(GLsizei n, GLuint *renderbuffers)
{
    WRAPPERS_DEBUG_PRINTF("glGenRenderbuffers()\n", n, renderbuffers);
    glGenRenderbuffers(n, renderbuffers);
}
void
my_glGenTextures(GLsizei n, GLuint *textures)
{
    WRAPPERS_DEBUG_PRINTF("glGenTextures()\n", n, textures);
    glGenTextures(n, textures);
}
void
my_glGetActiveAttrib(GLuint program, GLuint index, GLsizei bufsize, GLsizei *length, GLint *size, GLenum *type, char *name)
{
    WRAPPERS_DEBUG_PRINTF("glGetActiveAttrib()\n", program, index, bufsize, length, size, type, name);
    glGetActiveAttrib(program, index, bufsize, length, size, type, name);
}
void
my_glGetActiveUniform(GLuint program, GLuint index, GLsizei bufsize, GLsizei *length, GLint *size, GLenum *type, char *name)
{
    WRAPPERS_DEBUG_PRINTF("glGetActiveUniform()\n", program, index, bufsize, length, size, type, name);
    glGetActiveUniform(program, index, bufsize, length, size, type, name);
}
void
my_glGetAttachedShaders(GLuint program, GLsizei maxcount, GLsizei *count, GLuint *shaders)
{
    WRAPPERS_DEBUG_PRINTF("glGetAttachedShaders()\n", program, maxcount, count, shaders);
    glGetAttachedShaders(program, maxcount, count, shaders);
}
int
my_glGetAttribLocation(GLuint program, const char *name)
{
    WRAPPERS_DEBUG_PRINTF("glGetAttribLocation()\n", program, name);
    return glGetAttribLocation(program, name);
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
GLenum
my_glGetError()
{
    WRAPPERS_DEBUG_PRINTF("glGetError()\n");
    return glGetError();
}
void
my_glGetFloatv(GLenum pname, GLfloat *params)
{
    WRAPPERS_DEBUG_PRINTF("glGetFloatv()\n", pname, params);
    glGetFloatv(pname, params);
}
void
my_glGetFramebufferAttachmentParameteriv(GLenum target, GLenum attachment, GLenum pname, GLint *params)
{
    WRAPPERS_DEBUG_PRINTF("glGetFramebufferAttachmentParameteriv()\n", target, attachment, pname, params);
    glGetFramebufferAttachmentParameteriv(target, attachment, pname, params);
}
void
my_glGetIntegerv(GLenum pname, GLint *params)
{
    WRAPPERS_DEBUG_PRINTF("glGetIntegerv()\n", pname, params);
    glGetIntegerv(pname, params);
}
void
my_glGetProgramiv(GLuint program, GLenum pname, GLint *params)
{
    WRAPPERS_DEBUG_PRINTF("glGetProgramiv()\n", program, pname, params);
    glGetProgramiv(program, pname, params);
}
void
my_glGetProgramInfoLog(GLuint program, GLsizei bufsize, GLsizei *length, char *infolog)
{
    WRAPPERS_DEBUG_PRINTF("glGetProgramInfoLog()\n", program, bufsize, length, infolog);
    glGetProgramInfoLog(program, bufsize, length, infolog);
}
void
my_glGetRenderbufferParameteriv(GLenum target, GLenum pname, GLint *params)
{
    WRAPPERS_DEBUG_PRINTF("glGetRenderbufferParameteriv()\n", target, pname, params);
    glGetRenderbufferParameteriv(target, pname, params);
}
void
my_glGetShaderiv(GLuint shader, GLenum pname, GLint *params)
{
    WRAPPERS_DEBUG_PRINTF("glGetShaderiv()\n", shader, pname, params);
    glGetShaderiv(shader, pname, params);
}
void
my_glGetShaderInfoLog(GLuint shader, GLsizei bufsize, GLsizei *length, char *infolog)
{
    WRAPPERS_DEBUG_PRINTF("glGetShaderInfoLog()\n", shader, bufsize, length, infolog);
    glGetShaderInfoLog(shader, bufsize, length, infolog);
}
void
my_glGetShaderPrecisionFormat(GLenum shadertype, GLenum precisiontype, GLint *range, GLint *precision)
{
    WRAPPERS_DEBUG_PRINTF("glGetShaderPrecisionFormat()\n", shadertype, precisiontype, range, precision);
    glGetShaderPrecisionFormat(shadertype, precisiontype, range, precision);
}
void
my_glGetShaderSource(GLuint shader, GLsizei bufsize, GLsizei *length, char *source)
{
    WRAPPERS_DEBUG_PRINTF("glGetShaderSource()\n", shader, bufsize, length, source);
    glGetShaderSource(shader, bufsize, length, source);
}
const GLubyte *
my_glGetString(GLenum name)
{
    WRAPPERS_DEBUG_PRINTF("glGetString()\n", name);
    return glGetString(name);
}
void
my_glGetTexParameterfv(GLenum target, GLenum pname, GLfloat *params)
{
    WRAPPERS_DEBUG_PRINTF("glGetTexParameterfv()\n", target, pname, params);
    glGetTexParameterfv(target, pname, params);
}
void
my_glGetTexParameteriv(GLenum target, GLenum pname, GLint *params)
{
    WRAPPERS_DEBUG_PRINTF("glGetTexParameteriv()\n", target, pname, params);
    glGetTexParameteriv(target, pname, params);
}
void
my_glGetUniformfv(GLuint program, GLint location, GLfloat *params)
{
    WRAPPERS_DEBUG_PRINTF("glGetUniformfv()\n", program, location, params);
    glGetUniformfv(program, location, params);
}
void
my_glGetUniformiv(GLuint program, GLint location, GLint *params)
{
    WRAPPERS_DEBUG_PRINTF("glGetUniformiv()\n", program, location, params);
    glGetUniformiv(program, location, params);
}
int
my_glGetUniformLocation(GLuint program, const char *name)
{
    WRAPPERS_DEBUG_PRINTF("glGetUniformLocation()\n", program, name);
    return glGetUniformLocation(program, name);
}
void
my_glGetVertexAttribfv(GLuint index, GLenum pname, GLfloat *params)
{
    WRAPPERS_DEBUG_PRINTF("glGetVertexAttribfv()\n", index, pname, params);
    glGetVertexAttribfv(index, pname, params);
}
void
my_glGetVertexAttribiv(GLuint index, GLenum pname, GLint *params)
{
    WRAPPERS_DEBUG_PRINTF("glGetVertexAttribiv()\n", index, pname, params);
    glGetVertexAttribiv(index, pname, params);
}
void
my_glGetVertexAttribPointerv(GLuint index, GLenum pname, void **pointer)
{
    WRAPPERS_DEBUG_PRINTF("glGetVertexAttribPointerv()\n", index, pname, pointer);
    glGetVertexAttribPointerv(index, pname, pointer);
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
my_glIsFramebuffer(GLuint framebuffer)
{
    WRAPPERS_DEBUG_PRINTF("glIsFramebuffer()\n", framebuffer);
    return glIsFramebuffer(framebuffer);
}
GLboolean
my_glIsProgram(GLuint program)
{
    WRAPPERS_DEBUG_PRINTF("glIsProgram()\n", program);
    return glIsProgram(program);
}
GLboolean
my_glIsRenderbuffer(GLuint renderbuffer)
{
    WRAPPERS_DEBUG_PRINTF("glIsRenderbuffer()\n", renderbuffer);
    return glIsRenderbuffer(renderbuffer);
}
GLboolean
my_glIsShader(GLuint shader)
{
    WRAPPERS_DEBUG_PRINTF("glIsShader()\n", shader);
    return glIsShader(shader);
}
GLboolean
my_glIsTexture(GLuint texture)
{
    WRAPPERS_DEBUG_PRINTF("glIsTexture()\n", texture);
    return glIsTexture(texture);
}
void
my_glLineWidth(GLfloat width)
{
    WRAPPERS_DEBUG_PRINTF("glLineWidth()\n", width);
    glLineWidth(width);
}
void
my_glLinkProgram(GLuint program)
{
    WRAPPERS_DEBUG_PRINTF("glLinkProgram()\n", program);
    glLinkProgram(program);
}
void
my_glPixelStorei(GLenum pname, GLint param)
{
    WRAPPERS_DEBUG_PRINTF("glPixelStorei()\n", pname, param);
    glPixelStorei(pname, param);
}
void
my_glPolygonOffset(GLfloat factor, GLfloat units)
{
    WRAPPERS_DEBUG_PRINTF("glPolygonOffset()\n", factor, units);
    glPolygonOffset(factor, units);
}
void
my_glReadPixels(GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, void *pixels)
{
    WRAPPERS_DEBUG_PRINTF("glReadPixels()\n", x, y, width, height, format, type, pixels);
    glReadPixels(x, y, width, height, format, type, pixels);
}
void
my_glReleaseShaderCompiler()
{
    WRAPPERS_DEBUG_PRINTF("glReleaseShaderCompiler()\n");
    glReleaseShaderCompiler();
}
void
my_glRenderbufferStorage(GLenum target, GLenum internalformat, GLsizei width, GLsizei height)
{
    WRAPPERS_DEBUG_PRINTF("glRenderbufferStorage()\n", target, internalformat, width, height);
    glRenderbufferStorage(target, internalformat, width, height);
}
void
my_glSampleCoverage(GLclampf value, GLboolean invert)
{
    WRAPPERS_DEBUG_PRINTF("glSampleCoverage()\n", value, invert);
    glSampleCoverage(value, invert);
}
void
my_glScissor(GLint x, GLint y, GLsizei width, GLsizei height)
{
    WRAPPERS_DEBUG_PRINTF("glScissor()\n", x, y, width, height);
    glScissor(x, y, width, height);
}
void
my_glShaderBinary(GLint n, const GLuint *shaders, GLenum binaryformat, const void *binary, GLint length)
{
    WRAPPERS_DEBUG_PRINTF("glShaderBinary()\n", n, shaders, binaryformat, binary, length);
    glShaderBinary(n, shaders, binaryformat, binary, length);
}
void
my_glShaderSource(GLuint shader, GLsizei count, const char **string, const GLint *length)
{
    WRAPPERS_DEBUG_PRINTF("glShaderSource()\n", shader, count, string, length);
    glShaderSource(shader, count, string, length);
}
void
my_glStencilFunc(GLenum func, GLint ref, GLuint mask)
{
    WRAPPERS_DEBUG_PRINTF("glStencilFunc()\n", func, ref, mask);
    glStencilFunc(func, ref, mask);
}
void
my_glStencilFuncSeparate(GLenum face, GLenum func, GLint ref, GLuint mask)
{
    WRAPPERS_DEBUG_PRINTF("glStencilFuncSeparate()\n", face, func, ref, mask);
    glStencilFuncSeparate(face, func, ref, mask);
}
void
my_glStencilMask(GLuint mask)
{
    WRAPPERS_DEBUG_PRINTF("glStencilMask()\n", mask);
    glStencilMask(mask);
}
void
my_glStencilMaskSeparate(GLenum face, GLuint mask)
{
    WRAPPERS_DEBUG_PRINTF("glStencilMaskSeparate()\n", face, mask);
    glStencilMaskSeparate(face, mask);
}
void
my_glStencilOp(GLenum fail, GLenum zfail, GLenum zpass)
{
    WRAPPERS_DEBUG_PRINTF("glStencilOp()\n", fail, zfail, zpass);
    glStencilOp(fail, zfail, zpass);
}
void
my_glStencilOpSeparate(GLenum face, GLenum fail, GLenum zfail, GLenum zpass)
{
    WRAPPERS_DEBUG_PRINTF("glStencilOpSeparate()\n", face, fail, zfail, zpass);
    glStencilOpSeparate(face, fail, zfail, zpass);
}
void
my_glTexImage2D(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const void *pixels)
{
    WRAPPERS_DEBUG_PRINTF("glTexImage2D()\n", target, level, internalformat, width, height, border, format, type, pixels);
    glTexImage2D(target, level, internalformat, width, height, border, format, type, pixels);
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
my_glTexParameteri(GLenum target, GLenum pname, GLint param)
{
    WRAPPERS_DEBUG_PRINTF("glTexParameteri()\n", target, pname, param);
    glTexParameteri(target, pname, param);
}
void
my_glTexParameteriv(GLenum target, GLenum pname, const GLint *params)
{
    WRAPPERS_DEBUG_PRINTF("glTexParameteriv()\n", target, pname, params);
    glTexParameteriv(target, pname, params);
}
void
my_glTexSubImage2D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const void *pixels)
{
    WRAPPERS_DEBUG_PRINTF("glTexSubImage2D()\n", target, level, xoffset, yoffset, width, height, format, type, pixels);
    glTexSubImage2D(target, level, xoffset, yoffset, width, height, format, type, pixels);
}
void
my_glUniform1f(GLint location, GLfloat x)
{
    WRAPPERS_DEBUG_PRINTF("glUniform1f()\n", location, x);
    glUniform1f(location, x);
}
void
my_glUniform1fv(GLint location, GLsizei count, const GLfloat *v)
{
    WRAPPERS_DEBUG_PRINTF("glUniform1fv()\n", location, count, v);
    glUniform1fv(location, count, v);
}
void
my_glUniform1i(GLint location, GLint x)
{
    WRAPPERS_DEBUG_PRINTF("glUniform1i()\n", location, x);
    glUniform1i(location, x);
}
void
my_glUniform1iv(GLint location, GLsizei count, const GLint *v)
{
    WRAPPERS_DEBUG_PRINTF("glUniform1iv()\n", location, count, v);
    glUniform1iv(location, count, v);
}
void
my_glUniform2f(GLint location, GLfloat x, GLfloat y)
{
    WRAPPERS_DEBUG_PRINTF("glUniform2f()\n", location, x, y);
    glUniform2f(location, x, y);
}
void
my_glUniform2fv(GLint location, GLsizei count, const GLfloat *v)
{
    WRAPPERS_DEBUG_PRINTF("glUniform2fv()\n", location, count, v);
    glUniform2fv(location, count, v);
}
void
my_glUniform2i(GLint location, GLint x, GLint y)
{
    WRAPPERS_DEBUG_PRINTF("glUniform2i()\n", location, x, y);
    glUniform2i(location, x, y);
}
void
my_glUniform2iv(GLint location, GLsizei count, const GLint *v)
{
    WRAPPERS_DEBUG_PRINTF("glUniform2iv()\n", location, count, v);
    glUniform2iv(location, count, v);
}
void
my_glUniform3f(GLint location, GLfloat x, GLfloat y, GLfloat z)
{
    WRAPPERS_DEBUG_PRINTF("glUniform3f()\n", location, x, y, z);
    glUniform3f(location, x, y, z);
}
void
my_glUniform3fv(GLint location, GLsizei count, const GLfloat *v)
{
    WRAPPERS_DEBUG_PRINTF("glUniform3fv()\n", location, count, v);
    glUniform3fv(location, count, v);
}
void
my_glUniform3i(GLint location, GLint x, GLint y, GLint z)
{
    WRAPPERS_DEBUG_PRINTF("glUniform3i()\n", location, x, y, z);
    glUniform3i(location, x, y, z);
}
void
my_glUniform3iv(GLint location, GLsizei count, const GLint *v)
{
    WRAPPERS_DEBUG_PRINTF("glUniform3iv()\n", location, count, v);
    glUniform3iv(location, count, v);
}
void
my_glUniform4f(GLint location, GLfloat x, GLfloat y, GLfloat z, GLfloat w)
{
    WRAPPERS_DEBUG_PRINTF("glUniform4f()\n", location, x, y, z, w);
    glUniform4f(location, x, y, z, w);
}
void
my_glUniform4fv(GLint location, GLsizei count, const GLfloat *v)
{
    WRAPPERS_DEBUG_PRINTF("glUniform4fv()\n", location, count, v);
    glUniform4fv(location, count, v);
}
void
my_glUniform4i(GLint location, GLint x, GLint y, GLint z, GLint w)
{
    WRAPPERS_DEBUG_PRINTF("glUniform4i()\n", location, x, y, z, w);
    glUniform4i(location, x, y, z, w);
}
void
my_glUniform4iv(GLint location, GLsizei count, const GLint *v)
{
    WRAPPERS_DEBUG_PRINTF("glUniform4iv()\n", location, count, v);
    glUniform4iv(location, count, v);
}
void
my_glUniformMatrix2fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value)
{
    WRAPPERS_DEBUG_PRINTF("glUniformMatrix2fv()\n", location, count, transpose, value);
    glUniformMatrix2fv(location, count, transpose, value);
}
void
my_glUniformMatrix3fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value)
{
    WRAPPERS_DEBUG_PRINTF("glUniformMatrix3fv()\n", location, count, transpose, value);
    glUniformMatrix3fv(location, count, transpose, value);
}
void
my_glUniformMatrix4fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value)
{
    WRAPPERS_DEBUG_PRINTF("glUniformMatrix4fv()\n", location, count, transpose, value);
    glUniformMatrix4fv(location, count, transpose, value);
}
void
my_glUseProgram(GLuint program)
{
    WRAPPERS_DEBUG_PRINTF("glUseProgram()\n", program);
    glUseProgram(program);
}
void
my_glValidateProgram(GLuint program)
{
    WRAPPERS_DEBUG_PRINTF("glValidateProgram()\n", program);
    glValidateProgram(program);
}
void
my_glVertexAttrib1f(GLuint indx, GLfloat x)
{
    WRAPPERS_DEBUG_PRINTF("glVertexAttrib1f()\n", indx, x);
    glVertexAttrib1f(indx, x);
}
void
my_glVertexAttrib1fv(GLuint indx, const GLfloat *values)
{
    WRAPPERS_DEBUG_PRINTF("glVertexAttrib1fv()\n", indx, values);
    glVertexAttrib1fv(indx, values);
}
void
my_glVertexAttrib2f(GLuint indx, GLfloat x, GLfloat y)
{
    WRAPPERS_DEBUG_PRINTF("glVertexAttrib2f()\n", indx, x, y);
    glVertexAttrib2f(indx, x, y);
}
void
my_glVertexAttrib2fv(GLuint indx, const GLfloat *values)
{
    WRAPPERS_DEBUG_PRINTF("glVertexAttrib2fv()\n", indx, values);
    glVertexAttrib2fv(indx, values);
}
void
my_glVertexAttrib3f(GLuint indx, GLfloat x, GLfloat y, GLfloat z)
{
    WRAPPERS_DEBUG_PRINTF("glVertexAttrib3f()\n", indx, x, y, z);
    glVertexAttrib3f(indx, x, y, z);
}
void
my_glVertexAttrib3fv(GLuint indx, const GLfloat *values)
{
    WRAPPERS_DEBUG_PRINTF("glVertexAttrib3fv()\n", indx, values);
    glVertexAttrib3fv(indx, values);
}
void
my_glVertexAttrib4f(GLuint indx, GLfloat x, GLfloat y, GLfloat z, GLfloat w)
{
    WRAPPERS_DEBUG_PRINTF("glVertexAttrib4f()\n", indx, x, y, z, w);
    glVertexAttrib4f(indx, x, y, z, w);
}
void
my_glVertexAttrib4fv(GLuint indx, const GLfloat *values)
{
    WRAPPERS_DEBUG_PRINTF("glVertexAttrib4fv()\n", indx, values);
    glVertexAttrib4fv(indx, values);
}
void
my_glVertexAttribPointer(GLuint indx, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void *ptr)
{
    WRAPPERS_DEBUG_PRINTF("glVertexAttribPointer()\n", indx, size, type, normalized, stride, ptr);
    glVertexAttribPointer(indx, size, type, normalized, stride, ptr);
}
void
my_glViewport(GLint x, GLint y, GLsizei width, GLsizei height)
{
    WRAPPERS_DEBUG_PRINTF("glViewport()\n", x, y, width, height);
    glViewport(x, y, width, height);
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
    WRAPPERS_DEBUG_PRINTF("glMapBufferOES()\n", target, access);
    /* No CALL */ printf("UNIMPLEMENTED: glMapBufferOES\n");
}
GLboolean
my_glUnmapBufferOES(GLenum target)
{
    WRAPPERS_DEBUG_PRINTF("glUnmapBufferOES()\n", target);
    /* No CALL */ printf("UNIMPLEMENTED: glUnmapBufferOES\n");
}
void
my_glGetBufferPointervOES(GLenum target, GLenum pname, void **params)
{
    WRAPPERS_DEBUG_PRINTF("glGetBufferPointervOES()\n", target, pname, params);
    /* No CALL */ printf("UNIMPLEMENTED: glGetBufferPointervOES\n");
}
void
my_glTexImage3DOES(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLenum format, GLenum type, const void *pixels)
{
    WRAPPERS_DEBUG_PRINTF("glTexImage3DOES()\n", target, level, internalformat, width, height, depth, border, format, type, pixels);
    /* No CALL */ printf("UNIMPLEMENTED: glTexImage3DOES\n");
}
void
my_glTexSubImage3DOES(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const void *pixels)
{
    WRAPPERS_DEBUG_PRINTF("glTexSubImage3DOES()\n", target, level, xoffset, yoffset, zoffset, width, height, depth, format, type, pixels);
    /* No CALL */ printf("UNIMPLEMENTED: glTexSubImage3DOES\n");
}
void
my_glCopyTexSubImage3DOES(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLint x, GLint y, GLsizei width, GLsizei height)
{
    WRAPPERS_DEBUG_PRINTF("glCopyTexSubImage3DOES()\n", target, level, xoffset, yoffset, zoffset, x, y, width, height);
    /* No CALL */ printf("UNIMPLEMENTED: glCopyTexSubImage3DOES\n");
}
void
my_glCompressedTexImage3DOES(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLsizei imageSize, const void *data)
{
    WRAPPERS_DEBUG_PRINTF("glCompressedTexImage3DOES()\n", target, level, internalformat, width, height, depth, border, imageSize, data);
    /* No CALL */ printf("UNIMPLEMENTED: glCompressedTexImage3DOES\n");
}
void
my_glCompressedTexSubImage3DOES(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLsizei imageSize, const void *data)
{
    WRAPPERS_DEBUG_PRINTF("glCompressedTexSubImage3DOES()\n", target, level, xoffset, yoffset, zoffset, width, height, depth, format, imageSize, data);
    /* No CALL */ printf("UNIMPLEMENTED: glCompressedTexSubImage3DOES\n");
}
void
my_glFramebufferTexture3DOES(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level, GLint zoffset)
{
    WRAPPERS_DEBUG_PRINTF("glFramebufferTexture3DOES()\n", target, attachment, textarget, texture, level, zoffset);
    /* No CALL */ printf("UNIMPLEMENTED: glFramebufferTexture3DOES\n");
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
}
void
my_glGetProgramBinaryOES(GLuint program, GLsizei bufSize, GLsizei *length, GLenum *binaryFormat, void *binary)
{
    WRAPPERS_DEBUG_PRINTF("glGetProgramBinaryOES()\n", program, bufSize, length, binaryFormat, binary);
    /* No CALL */ printf("UNIMPLEMENTED: glGetProgramBinaryOES\n");
}
void
my_glProgramBinaryOES(GLuint program, GLenum binaryFormat, const void *binary, GLint length)
{
    WRAPPERS_DEBUG_PRINTF("glProgramBinaryOES()\n", program, binaryFormat, binary, length);
    /* No CALL */ printf("UNIMPLEMENTED: glProgramBinaryOES\n");
}
#endif /* APKENV_GLES2 */
