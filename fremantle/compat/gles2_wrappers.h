#ifdef APKENV_GLES2
#include <GLES2/gl2.h>
#define GL_GLEXT_PROTOTYPES
#include <GLES2/gl2ext.h>

#include "../apkenv.h"
void
my_glActiveTexture(GLenum texture) SOFTFP;
void
my_glAttachShader(GLuint program, GLuint shader) SOFTFP;
void
my_glBindAttribLocation(GLuint program, GLuint index, const char *name) SOFTFP;
void
my_glBindBuffer(GLenum target, GLuint buffer) SOFTFP;
void
my_glBindFramebuffer(GLenum target, GLuint framebuffer) SOFTFP;
void
my_glBindRenderbuffer(GLenum target, GLuint renderbuffer) SOFTFP;
void
my_glBindTexture(GLenum target, GLuint texture) SOFTFP;
void
my_glBlendColor(GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha) SOFTFP;
void
my_glBlendEquation(GLenum mode) SOFTFP;
void
my_glBlendEquationSeparate(GLenum modeRGB, GLenum modeAlpha) SOFTFP;
void
my_glBlendFunc(GLenum sfactor, GLenum dfactor) SOFTFP;
void
my_glBlendFuncSeparate(GLenum srcRGB, GLenum dstRGB, GLenum srcAlpha, GLenum dstAlpha) SOFTFP;
void
my_glBufferData(GLenum target, GLsizeiptr size, const void *data, GLenum usage) SOFTFP;
void
my_glBufferSubData(GLenum target, GLintptr offset, GLsizeiptr size, const void *data) SOFTFP;
GLenum
my_glCheckFramebufferStatus(GLenum target) SOFTFP;
void
my_glClear(GLbitfield mask) SOFTFP;
void
my_glClearColor(GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha) SOFTFP;
void
my_glClearDepthf(GLclampf depth) SOFTFP;
void
my_glClearStencil(GLint s) SOFTFP;
void
my_glColorMask(GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha) SOFTFP;
void
my_glCompileShader(GLuint shader) SOFTFP;
void
my_glCompressedTexImage2D(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLsizei imageSize, const void *data) SOFTFP;
void
my_glCompressedTexSubImage2D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLsizei imageSize, const void *data) SOFTFP;
void
my_glCopyTexImage2D(GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLsizei height, GLint border) SOFTFP;
void
my_glCopyTexSubImage2D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height) SOFTFP;
GLuint
my_glCreateProgram() SOFTFP;
GLuint
my_glCreateShader(GLenum type) SOFTFP;
void
my_glCullFace(GLenum mode) SOFTFP;
void
my_glDeleteBuffers(GLsizei n, const GLuint *buffers) SOFTFP;
void
my_glDeleteFramebuffers(GLsizei n, const GLuint *framebuffers) SOFTFP;
void
my_glDeleteProgram(GLuint program) SOFTFP;
void
my_glDeleteRenderbuffers(GLsizei n, const GLuint *renderbuffers) SOFTFP;
void
my_glDeleteShader(GLuint shader) SOFTFP;
void
my_glDeleteTextures(GLsizei n, const GLuint *textures) SOFTFP;
void
my_glDepthFunc(GLenum func) SOFTFP;
void
my_glDepthMask(GLboolean flag) SOFTFP;
void
my_glDepthRangef(GLclampf zNear, GLclampf zFar) SOFTFP;
void
my_glDetachShader(GLuint program, GLuint shader) SOFTFP;
void
my_glDisable(GLenum cap) SOFTFP;
void
my_glDisableVertexAttribArray(GLuint index) SOFTFP;
void
my_glDrawArrays(GLenum mode, GLint first, GLsizei count) SOFTFP;
void
my_glDrawElements(GLenum mode, GLsizei count, GLenum type, const void *indices) SOFTFP;
void
my_glEnable(GLenum cap) SOFTFP;
void
my_glEnableVertexAttribArray(GLuint index) SOFTFP;
void
my_glFinish() SOFTFP;
void
my_glFlush() SOFTFP;
void
my_glFramebufferRenderbuffer(GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer) SOFTFP;
void
my_glFramebufferTexture2D(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level) SOFTFP;
void
my_glFrontFace(GLenum mode) SOFTFP;
void
my_glGenBuffers(GLsizei n, GLuint *buffers) SOFTFP;
void
my_glGenerateMipmap(GLenum target) SOFTFP;
void
my_glGenFramebuffers(GLsizei n, GLuint *framebuffers) SOFTFP;
void
my_glGenRenderbuffers(GLsizei n, GLuint *renderbuffers) SOFTFP;
void
my_glGenTextures(GLsizei n, GLuint *textures) SOFTFP;
void
my_glGetActiveAttrib(GLuint program, GLuint index, GLsizei bufsize, GLsizei *length, GLint *size, GLenum *type, char *name) SOFTFP;
void
my_glGetActiveUniform(GLuint program, GLuint index, GLsizei bufsize, GLsizei *length, GLint *size, GLenum *type, char *name) SOFTFP;
void
my_glGetAttachedShaders(GLuint program, GLsizei maxcount, GLsizei *count, GLuint *shaders) SOFTFP;
int
my_glGetAttribLocation(GLuint program, const char *name) SOFTFP;
void
my_glGetBooleanv(GLenum pname, GLboolean *params) SOFTFP;
void
my_glGetBufferParameteriv(GLenum target, GLenum pname, GLint *params) SOFTFP;
GLenum
my_glGetError() SOFTFP;
void
my_glGetFloatv(GLenum pname, GLfloat *params) SOFTFP;
void
my_glGetFramebufferAttachmentParameteriv(GLenum target, GLenum attachment, GLenum pname, GLint *params) SOFTFP;
void
my_glGetIntegerv(GLenum pname, GLint *params) SOFTFP;
void
my_glGetProgramiv(GLuint program, GLenum pname, GLint *params) SOFTFP;
void
my_glGetProgramInfoLog(GLuint program, GLsizei bufsize, GLsizei *length, char *infolog) SOFTFP;
void
my_glGetRenderbufferParameteriv(GLenum target, GLenum pname, GLint *params) SOFTFP;
void
my_glGetShaderiv(GLuint shader, GLenum pname, GLint *params) SOFTFP;
void
my_glGetShaderInfoLog(GLuint shader, GLsizei bufsize, GLsizei *length, char *infolog) SOFTFP;
void
my_glGetShaderPrecisionFormat(GLenum shadertype, GLenum precisiontype, GLint *range, GLint *precision) SOFTFP;
void
my_glGetShaderSource(GLuint shader, GLsizei bufsize, GLsizei *length, char *source) SOFTFP;
const GLubyte *
my_glGetString(GLenum name) SOFTFP;
void
my_glGetTexParameterfv(GLenum target, GLenum pname, GLfloat *params) SOFTFP;
void
my_glGetTexParameteriv(GLenum target, GLenum pname, GLint *params) SOFTFP;
void
my_glGetUniformfv(GLuint program, GLint location, GLfloat *params) SOFTFP;
void
my_glGetUniformiv(GLuint program, GLint location, GLint *params) SOFTFP;
int
my_glGetUniformLocation(GLuint program, const char *name) SOFTFP;
void
my_glGetVertexAttribfv(GLuint index, GLenum pname, GLfloat *params) SOFTFP;
void
my_glGetVertexAttribiv(GLuint index, GLenum pname, GLint *params) SOFTFP;
void
my_glGetVertexAttribPointerv(GLuint index, GLenum pname, void **pointer) SOFTFP;
void
my_glHint(GLenum target, GLenum mode) SOFTFP;
GLboolean
my_glIsBuffer(GLuint buffer) SOFTFP;
GLboolean
my_glIsEnabled(GLenum cap) SOFTFP;
GLboolean
my_glIsFramebuffer(GLuint framebuffer) SOFTFP;
GLboolean
my_glIsProgram(GLuint program) SOFTFP;
GLboolean
my_glIsRenderbuffer(GLuint renderbuffer) SOFTFP;
GLboolean
my_glIsShader(GLuint shader) SOFTFP;
GLboolean
my_glIsTexture(GLuint texture) SOFTFP;
void
my_glLineWidth(GLfloat width) SOFTFP;
void
my_glLinkProgram(GLuint program) SOFTFP;
void
my_glPixelStorei(GLenum pname, GLint param) SOFTFP;
void
my_glPolygonOffset(GLfloat factor, GLfloat units) SOFTFP;
void
my_glReadPixels(GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, void *pixels) SOFTFP;
void
my_glReleaseShaderCompiler() SOFTFP;
void
my_glRenderbufferStorage(GLenum target, GLenum internalformat, GLsizei width, GLsizei height) SOFTFP;
void
my_glSampleCoverage(GLclampf value, GLboolean invert) SOFTFP;
void
my_glScissor(GLint x, GLint y, GLsizei width, GLsizei height) SOFTFP;
void
my_glShaderBinary(GLint n, const GLuint *shaders, GLenum binaryformat, const void *binary, GLint length) SOFTFP;
void
my_glShaderSource(GLuint shader, GLsizei count, const char **string, const GLint *length) SOFTFP;
void
my_glStencilFunc(GLenum func, GLint ref, GLuint mask) SOFTFP;
void
my_glStencilFuncSeparate(GLenum face, GLenum func, GLint ref, GLuint mask) SOFTFP;
void
my_glStencilMask(GLuint mask) SOFTFP;
void
my_glStencilMaskSeparate(GLenum face, GLuint mask) SOFTFP;
void
my_glStencilOp(GLenum fail, GLenum zfail, GLenum zpass) SOFTFP;
void
my_glStencilOpSeparate(GLenum face, GLenum fail, GLenum zfail, GLenum zpass) SOFTFP;
void
my_glTexImage2D(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const void *pixels) SOFTFP;
void
my_glTexParameterf(GLenum target, GLenum pname, GLfloat param) SOFTFP;
void
my_glTexParameterfv(GLenum target, GLenum pname, const GLfloat *params) SOFTFP;
void
my_glTexParameteri(GLenum target, GLenum pname, GLint param) SOFTFP;
void
my_glTexParameteriv(GLenum target, GLenum pname, const GLint *params) SOFTFP;
void
my_glTexSubImage2D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const void *pixels) SOFTFP;
void
my_glUniform1f(GLint location, GLfloat x) SOFTFP;
void
my_glUniform1fv(GLint location, GLsizei count, const GLfloat *v) SOFTFP;
void
my_glUniform1i(GLint location, GLint x) SOFTFP;
void
my_glUniform1iv(GLint location, GLsizei count, const GLint *v) SOFTFP;
void
my_glUniform2f(GLint location, GLfloat x, GLfloat y) SOFTFP;
void
my_glUniform2fv(GLint location, GLsizei count, const GLfloat *v) SOFTFP;
void
my_glUniform2i(GLint location, GLint x, GLint y) SOFTFP;
void
my_glUniform2iv(GLint location, GLsizei count, const GLint *v) SOFTFP;
void
my_glUniform3f(GLint location, GLfloat x, GLfloat y, GLfloat z) SOFTFP;
void
my_glUniform3fv(GLint location, GLsizei count, const GLfloat *v) SOFTFP;
void
my_glUniform3i(GLint location, GLint x, GLint y, GLint z) SOFTFP;
void
my_glUniform3iv(GLint location, GLsizei count, const GLint *v) SOFTFP;
void
my_glUniform4f(GLint location, GLfloat x, GLfloat y, GLfloat z, GLfloat w) SOFTFP;
void
my_glUniform4fv(GLint location, GLsizei count, const GLfloat *v) SOFTFP;
void
my_glUniform4i(GLint location, GLint x, GLint y, GLint z, GLint w) SOFTFP;
void
my_glUniform4iv(GLint location, GLsizei count, const GLint *v) SOFTFP;
void
my_glUniformMatrix2fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value) SOFTFP;
void
my_glUniformMatrix3fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value) SOFTFP;
void
my_glUniformMatrix4fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value) SOFTFP;
void
my_glUseProgram(GLuint program) SOFTFP;
void
my_glValidateProgram(GLuint program) SOFTFP;
void
my_glVertexAttrib1f(GLuint indx, GLfloat x) SOFTFP;
void
my_glVertexAttrib1fv(GLuint indx, const GLfloat *values) SOFTFP;
void
my_glVertexAttrib2f(GLuint indx, GLfloat x, GLfloat y) SOFTFP;
void
my_glVertexAttrib2fv(GLuint indx, const GLfloat *values) SOFTFP;
void
my_glVertexAttrib3f(GLuint indx, GLfloat x, GLfloat y, GLfloat z) SOFTFP;
void
my_glVertexAttrib3fv(GLuint indx, const GLfloat *values) SOFTFP;
void
my_glVertexAttrib4f(GLuint indx, GLfloat x, GLfloat y, GLfloat z, GLfloat w) SOFTFP;
void
my_glVertexAttrib4fv(GLuint indx, const GLfloat *values) SOFTFP;
void
my_glVertexAttribPointer(GLuint indx, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void *ptr) SOFTFP;
void
my_glViewport(GLint x, GLint y, GLsizei width, GLsizei height) SOFTFP;
void
my_glEGLImageTargetTexture2DOES(GLenum target, GLeglImageOES image) SOFTFP;
void
my_glEGLImageTargetRenderbufferStorageOES(GLenum target, GLeglImageOES image) SOFTFP;
void *
my_glMapBufferOES(GLenum target, GLenum access) SOFTFP;
GLboolean
my_glUnmapBufferOES(GLenum target) SOFTFP;
void
my_glGetBufferPointervOES(GLenum target, GLenum pname, void **params) SOFTFP;
void
my_glTexImage3DOES(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLenum format, GLenum type, const void *pixels) SOFTFP;
void
my_glTexSubImage3DOES(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const void *pixels) SOFTFP;
void
my_glCopyTexSubImage3DOES(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLint x, GLint y, GLsizei width, GLsizei height) SOFTFP;
void
my_glCompressedTexImage3DOES(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLsizei imageSize, const void *data) SOFTFP;
void
my_glCompressedTexSubImage3DOES(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLsizei imageSize, const void *data) SOFTFP;
void
my_glFramebufferTexture3DOES(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level, GLint zoffset) SOFTFP;
void
my_glMultiDrawArraysEXT(GLenum mode, GLint *first, GLsizei *count, GLsizei primcount) SOFTFP;
void
my_glMultiDrawElementsEXT(GLenum mode, const GLsizei *count, GLenum type, const GLvoid **indices, GLsizei primcount) SOFTFP;
void
my_glTexBindStreamIMG(GLint device, GLint deviceoffset) SOFTFP;
void
my_glGetTexStreamDeviceAttributeivIMG(GLint device, GLenum pname, GLint *params) SOFTFP;
const GLubyte *
my_glGetTexStreamDeviceNameIMG(GLint device) SOFTFP;
void
my_glGetProgramBinaryOES(GLuint program, GLsizei bufSize, GLsizei *length, GLenum *binaryFormat, void *binary) SOFTFP;
void
my_glProgramBinaryOES(GLuint program, GLenum binaryFormat, const void *binary, GLint length) SOFTFP;
#endif /* APKENV_GLES2 */
