#ifdef APKENV_GLES2
#include <GLES2/gl2.h>
#define GL_GLEXT_PROTOTYPES
#include <GLES2/gl2ext.h>

#include "../apkenv.h"
void
my_gles2_glActiveTexture(GLenum texture) SOFTFP;
void
my_gles2_glAttachShader(GLuint program, GLuint shader) SOFTFP;
void
my_gles2_glBindAttribLocation(GLuint program, GLuint index, const char *name) SOFTFP;
void
my_gles2_glBindBuffer(GLenum target, GLuint buffer) SOFTFP;
void
my_gles2_glBindFramebuffer(GLenum target, GLuint framebuffer) SOFTFP;
void
my_gles2_glBindRenderbuffer(GLenum target, GLuint renderbuffer) SOFTFP;
void
my_gles2_glBindTexture(GLenum target, GLuint texture) SOFTFP;
void
my_gles2_glBlendColor(GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha) SOFTFP;
void
my_gles2_glBlendEquation(GLenum mode) SOFTFP;
void
my_gles2_glBlendEquationSeparate(GLenum modeRGB, GLenum modeAlpha) SOFTFP;
void
my_gles2_glBlendFunc(GLenum sfactor, GLenum dfactor) SOFTFP;
void
my_gles2_glBlendFuncSeparate(GLenum srcRGB, GLenum dstRGB, GLenum srcAlpha, GLenum dstAlpha) SOFTFP;
void
my_gles2_glBufferData(GLenum target, GLsizeiptr size, const void *data, GLenum usage) SOFTFP;
void
my_gles2_glBufferSubData(GLenum target, GLintptr offset, GLsizeiptr size, const void *data) SOFTFP;
GLenum
my_gles2_glCheckFramebufferStatus(GLenum target) SOFTFP;
void
my_gles2_glClear(GLbitfield mask) SOFTFP;
void
my_gles2_glClearColor(GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha) SOFTFP;
void
my_gles2_glClearDepthf(GLclampf depth) SOFTFP;
void
my_gles2_glClearStencil(GLint s) SOFTFP;
void
my_gles2_glColorMask(GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha) SOFTFP;
void
my_gles2_glCompileShader(GLuint shader) SOFTFP;
void
my_gles2_glCompressedTexImage2D(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLsizei imageSize, const void *data) SOFTFP;
void
my_gles2_glCompressedTexSubImage2D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLsizei imageSize, const void *data) SOFTFP;
void
my_gles2_glCopyTexImage2D(GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLsizei height, GLint border) SOFTFP;
void
my_gles2_glCopyTexSubImage2D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height) SOFTFP;
GLuint
my_gles2_glCreateProgram() SOFTFP;
GLuint
my_gles2_glCreateShader(GLenum type) SOFTFP;
void
my_gles2_glCullFace(GLenum mode) SOFTFP;
void
my_gles2_glDeleteBuffers(GLsizei n, const GLuint *buffers) SOFTFP;
void
my_gles2_glDeleteFramebuffers(GLsizei n, const GLuint *framebuffers) SOFTFP;
void
my_gles2_glDeleteProgram(GLuint program) SOFTFP;
void
my_gles2_glDeleteRenderbuffers(GLsizei n, const GLuint *renderbuffers) SOFTFP;
void
my_gles2_glDeleteShader(GLuint shader) SOFTFP;
void
my_gles2_glDeleteTextures(GLsizei n, const GLuint *textures) SOFTFP;
void
my_gles2_glDepthFunc(GLenum func) SOFTFP;
void
my_gles2_glDepthMask(GLboolean flag) SOFTFP;
void
my_gles2_glDepthRangef(GLclampf zNear, GLclampf zFar) SOFTFP;
void
my_gles2_glDetachShader(GLuint program, GLuint shader) SOFTFP;
void
my_gles2_glDisable(GLenum cap) SOFTFP;
void
my_gles2_glDisableVertexAttribArray(GLuint index) SOFTFP;
void
my_gles2_glDrawArrays(GLenum mode, GLint first, GLsizei count) SOFTFP;
void
my_gles2_glDrawElements(GLenum mode, GLsizei count, GLenum type, const void *indices) SOFTFP;
void
my_gles2_glEnable(GLenum cap) SOFTFP;
void
my_gles2_glEnableVertexAttribArray(GLuint index) SOFTFP;
void
my_gles2_glFinish() SOFTFP;
void
my_gles2_glFlush() SOFTFP;
void
my_gles2_glFramebufferRenderbuffer(GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer) SOFTFP;
void
my_gles2_glFramebufferTexture2D(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level) SOFTFP;
void
my_gles2_glFrontFace(GLenum mode) SOFTFP;
void
my_gles2_glGenBuffers(GLsizei n, GLuint *buffers) SOFTFP;
void
my_gles2_glGenerateMipmap(GLenum target) SOFTFP;
void
my_gles2_glGenFramebuffers(GLsizei n, GLuint *framebuffers) SOFTFP;
void
my_gles2_glGenRenderbuffers(GLsizei n, GLuint *renderbuffers) SOFTFP;
void
my_gles2_glGenTextures(GLsizei n, GLuint *textures) SOFTFP;
void
my_gles2_glGetActiveAttrib(GLuint program, GLuint index, GLsizei bufsize, GLsizei *length, GLint *size, GLenum *type, char *name) SOFTFP;
void
my_gles2_glGetActiveUniform(GLuint program, GLuint index, GLsizei bufsize, GLsizei *length, GLint *size, GLenum *type, char *name) SOFTFP;
void
my_gles2_glGetAttachedShaders(GLuint program, GLsizei maxcount, GLsizei *count, GLuint *shaders) SOFTFP;
int
my_gles2_glGetAttribLocation(GLuint program, const char *name) SOFTFP;
void
my_gles2_glGetBooleanv(GLenum pname, GLboolean *params) SOFTFP;
void
my_gles2_glGetBufferParameteriv(GLenum target, GLenum pname, GLint *params) SOFTFP;
GLenum
my_gles2_glGetError() SOFTFP;
void
my_gles2_glGetFloatv(GLenum pname, GLfloat *params) SOFTFP;
void
my_gles2_glGetFramebufferAttachmentParameteriv(GLenum target, GLenum attachment, GLenum pname, GLint *params) SOFTFP;
void
my_gles2_glGetIntegerv(GLenum pname, GLint *params) SOFTFP;
void
my_gles2_glGetProgramiv(GLuint program, GLenum pname, GLint *params) SOFTFP;
void
my_gles2_glGetProgramInfoLog(GLuint program, GLsizei bufsize, GLsizei *length, char *infolog) SOFTFP;
void
my_gles2_glGetRenderbufferParameteriv(GLenum target, GLenum pname, GLint *params) SOFTFP;
void
my_gles2_glGetShaderiv(GLuint shader, GLenum pname, GLint *params) SOFTFP;
void
my_gles2_glGetShaderInfoLog(GLuint shader, GLsizei bufsize, GLsizei *length, char *infolog) SOFTFP;
void
my_gles2_glGetShaderPrecisionFormat(GLenum shadertype, GLenum precisiontype, GLint *range, GLint *precision) SOFTFP;
void
my_gles2_glGetShaderSource(GLuint shader, GLsizei bufsize, GLsizei *length, char *source) SOFTFP;
const GLubyte *
my_gles2_glGetString(GLenum name) SOFTFP;
void
my_gles2_glGetTexParameterfv(GLenum target, GLenum pname, GLfloat *params) SOFTFP;
void
my_gles2_glGetTexParameteriv(GLenum target, GLenum pname, GLint *params) SOFTFP;
void
my_gles2_glGetUniformfv(GLuint program, GLint location, GLfloat *params) SOFTFP;
void
my_gles2_glGetUniformiv(GLuint program, GLint location, GLint *params) SOFTFP;
int
my_gles2_glGetUniformLocation(GLuint program, const char *name) SOFTFP;
void
my_gles2_glGetVertexAttribfv(GLuint index, GLenum pname, GLfloat *params) SOFTFP;
void
my_gles2_glGetVertexAttribiv(GLuint index, GLenum pname, GLint *params) SOFTFP;
void
my_gles2_glGetVertexAttribPointerv(GLuint index, GLenum pname, void **pointer) SOFTFP;
void
my_gles2_glHint(GLenum target, GLenum mode) SOFTFP;
GLboolean
my_gles2_glIsBuffer(GLuint buffer) SOFTFP;
GLboolean
my_gles2_glIsEnabled(GLenum cap) SOFTFP;
GLboolean
my_gles2_glIsFramebuffer(GLuint framebuffer) SOFTFP;
GLboolean
my_gles2_glIsProgram(GLuint program) SOFTFP;
GLboolean
my_gles2_glIsRenderbuffer(GLuint renderbuffer) SOFTFP;
GLboolean
my_gles2_glIsShader(GLuint shader) SOFTFP;
GLboolean
my_gles2_glIsTexture(GLuint texture) SOFTFP;
void
my_gles2_glLineWidth(GLfloat width) SOFTFP;
void
my_gles2_glLinkProgram(GLuint program) SOFTFP;
void
my_gles2_glPixelStorei(GLenum pname, GLint param) SOFTFP;
void
my_gles2_glPolygonOffset(GLfloat factor, GLfloat units) SOFTFP;
void
my_gles2_glReadPixels(GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, void *pixels) SOFTFP;
void
my_gles2_glReleaseShaderCompiler() SOFTFP;
void
my_gles2_glRenderbufferStorage(GLenum target, GLenum internalformat, GLsizei width, GLsizei height) SOFTFP;
void
my_gles2_glSampleCoverage(GLclampf value, GLboolean invert) SOFTFP;
void
my_gles2_glScissor(GLint x, GLint y, GLsizei width, GLsizei height) SOFTFP;
void
my_gles2_glShaderBinary(GLint n, const GLuint *shaders, GLenum binaryformat, const void *binary, GLint length) SOFTFP;
void
my_gles2_glShaderSource(GLuint shader, GLsizei count, const char **string, const GLint *length) SOFTFP;
void
my_gles2_glStencilFunc(GLenum func, GLint ref, GLuint mask) SOFTFP;
void
my_gles2_glStencilFuncSeparate(GLenum face, GLenum func, GLint ref, GLuint mask) SOFTFP;
void
my_gles2_glStencilMask(GLuint mask) SOFTFP;
void
my_gles2_glStencilMaskSeparate(GLenum face, GLuint mask) SOFTFP;
void
my_gles2_glStencilOp(GLenum fail, GLenum zfail, GLenum zpass) SOFTFP;
void
my_gles2_glStencilOpSeparate(GLenum face, GLenum fail, GLenum zfail, GLenum zpass) SOFTFP;
void
my_gles2_glTexImage2D(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const void *pixels) SOFTFP;
void
my_gles2_glTexParameterf(GLenum target, GLenum pname, GLfloat param) SOFTFP;
void
my_gles2_glTexParameterfv(GLenum target, GLenum pname, const GLfloat *params) SOFTFP;
void
my_gles2_glTexParameteri(GLenum target, GLenum pname, GLint param) SOFTFP;
void
my_gles2_glTexParameteriv(GLenum target, GLenum pname, const GLint *params) SOFTFP;
void
my_gles2_glTexSubImage2D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const void *pixels) SOFTFP;
void
my_gles2_glUniform1f(GLint location, GLfloat x) SOFTFP;
void
my_gles2_glUniform1fv(GLint location, GLsizei count, const GLfloat *v) SOFTFP;
void
my_gles2_glUniform1i(GLint location, GLint x) SOFTFP;
void
my_gles2_glUniform1iv(GLint location, GLsizei count, const GLint *v) SOFTFP;
void
my_gles2_glUniform2f(GLint location, GLfloat x, GLfloat y) SOFTFP;
void
my_gles2_glUniform2fv(GLint location, GLsizei count, const GLfloat *v) SOFTFP;
void
my_gles2_glUniform2i(GLint location, GLint x, GLint y) SOFTFP;
void
my_gles2_glUniform2iv(GLint location, GLsizei count, const GLint *v) SOFTFP;
void
my_gles2_glUniform3f(GLint location, GLfloat x, GLfloat y, GLfloat z) SOFTFP;
void
my_gles2_glUniform3fv(GLint location, GLsizei count, const GLfloat *v) SOFTFP;
void
my_gles2_glUniform3i(GLint location, GLint x, GLint y, GLint z) SOFTFP;
void
my_gles2_glUniform3iv(GLint location, GLsizei count, const GLint *v) SOFTFP;
void
my_gles2_glUniform4f(GLint location, GLfloat x, GLfloat y, GLfloat z, GLfloat w) SOFTFP;
void
my_gles2_glUniform4fv(GLint location, GLsizei count, const GLfloat *v) SOFTFP;
void
my_gles2_glUniform4i(GLint location, GLint x, GLint y, GLint z, GLint w) SOFTFP;
void
my_gles2_glUniform4iv(GLint location, GLsizei count, const GLint *v) SOFTFP;
void
my_gles2_glUniformMatrix2fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value) SOFTFP;
void
my_gles2_glUniformMatrix3fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value) SOFTFP;
void
my_gles2_glUniformMatrix4fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value) SOFTFP;
void
my_gles2_glUseProgram(GLuint program) SOFTFP;
void
my_gles2_glValidateProgram(GLuint program) SOFTFP;
void
my_gles2_glVertexAttrib1f(GLuint indx, GLfloat x) SOFTFP;
void
my_gles2_glVertexAttrib1fv(GLuint indx, const GLfloat *values) SOFTFP;
void
my_gles2_glVertexAttrib2f(GLuint indx, GLfloat x, GLfloat y) SOFTFP;
void
my_gles2_glVertexAttrib2fv(GLuint indx, const GLfloat *values) SOFTFP;
void
my_gles2_glVertexAttrib3f(GLuint indx, GLfloat x, GLfloat y, GLfloat z) SOFTFP;
void
my_gles2_glVertexAttrib3fv(GLuint indx, const GLfloat *values) SOFTFP;
void
my_gles2_glVertexAttrib4f(GLuint indx, GLfloat x, GLfloat y, GLfloat z, GLfloat w) SOFTFP;
void
my_gles2_glVertexAttrib4fv(GLuint indx, const GLfloat *values) SOFTFP;
void
my_gles2_glVertexAttribPointer(GLuint indx, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void *ptr) SOFTFP;
void
my_gles2_glViewport(GLint x, GLint y, GLsizei width, GLsizei height) SOFTFP;
void
my_gles2_glEGLImageTargetTexture2DOES(GLenum target, GLeglImageOES image) SOFTFP;
void
my_gles2_glEGLImageTargetRenderbufferStorageOES(GLenum target, GLeglImageOES image) SOFTFP;
void *
my_gles2_glMapBufferOES(GLenum target, GLenum access) SOFTFP;
GLboolean
my_gles2_glUnmapBufferOES(GLenum target) SOFTFP;
void
my_gles2_glGetBufferPointervOES(GLenum target, GLenum pname, void **params) SOFTFP;
void
my_gles2_glTexImage3DOES(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLenum format, GLenum type, const void *pixels) SOFTFP;
void
my_gles2_glTexSubImage3DOES(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const void *pixels) SOFTFP;
void
my_gles2_glCopyTexSubImage3DOES(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLint x, GLint y, GLsizei width, GLsizei height) SOFTFP;
void
my_gles2_glCompressedTexImage3DOES(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLsizei imageSize, const void *data) SOFTFP;
void
my_gles2_glCompressedTexSubImage3DOES(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLsizei imageSize, const void *data) SOFTFP;
void
my_gles2_glFramebufferTexture3DOES(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level, GLint zoffset) SOFTFP;
void
my_gles2_glMultiDrawArraysEXT(GLenum mode, GLint *first, GLsizei *count, GLsizei primcount) SOFTFP;
void
my_gles2_glMultiDrawElementsEXT(GLenum mode, const GLsizei *count, GLenum type, const GLvoid **indices, GLsizei primcount) SOFTFP;
void
my_gles2_glTexBindStreamIMG(GLint device, GLint deviceoffset) SOFTFP;
void
my_gles2_glGetTexStreamDeviceAttributeivIMG(GLint device, GLenum pname, GLint *params) SOFTFP;
const GLubyte *
my_gles2_glGetTexStreamDeviceNameIMG(GLint device) SOFTFP;
void
my_gles2_glGetProgramBinaryOES(GLuint program, GLsizei bufSize, GLsizei *length, GLenum *binaryFormat, void *binary) SOFTFP;
void
my_gles2_glProgramBinaryOES(GLuint program, GLenum binaryFormat, const void *binary, GLint length) SOFTFP;

void gles2_init(void);

#else

#define gles2_init()

#endif /* APKENV_GLES2 */
