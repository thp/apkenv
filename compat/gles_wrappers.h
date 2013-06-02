#ifdef APKENV_GLES
#include "../apkenv.h"
#ifndef IN_GLES_WRAPPERS
#include "gl_types.h"
#endif

void
my_glAlphaFunc(GLenum func, GLclampf ref) SOFTFP;
void
my_glClearColor(GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha) SOFTFP;
void
my_glClearDepthf(GLclampf depth) SOFTFP;
void
my_glClipPlanef(GLenum plane, const GLfloat *equation) SOFTFP;
void
my_glColor4f(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha) SOFTFP;
void
my_glDepthRangef(GLclampf zNear, GLclampf zFar) SOFTFP;
void
my_glFogf(GLenum pname, GLfloat param) SOFTFP;
void
my_glFogfv(GLenum pname, const GLfloat *params) SOFTFP;
void
my_glFrustumf(GLfloat left, GLfloat right, GLfloat bottom, GLfloat top, GLfloat zNear, GLfloat zFar) SOFTFP;
void
my_glGetClipPlanef(GLenum pname, GLfloat eqn[4]) SOFTFP;
void
my_glGetFloatv(GLenum pname, GLfloat *params) SOFTFP;
void
my_glGetLightfv(GLenum light, GLenum pname, GLfloat *params) SOFTFP;
void
my_glGetMaterialfv(GLenum face, GLenum pname, GLfloat *params) SOFTFP;
void
my_glGetTexEnvfv(GLenum env, GLenum pname, GLfloat *params) SOFTFP;
void
my_glGetTexParameterfv(GLenum target, GLenum pname, GLfloat *params) SOFTFP;
void
my_glLightModelf(GLenum pname, GLfloat param) SOFTFP;
void
my_glLightModelfv(GLenum pname, const GLfloat *params) SOFTFP;
void
my_glLightf(GLenum light, GLenum pname, GLfloat param) SOFTFP;
void
my_glLightfv(GLenum light, GLenum pname, const GLfloat *params) SOFTFP;
void
my_glLineWidth(GLfloat width) SOFTFP;
void
my_glLoadMatrixf(const GLfloat *m) SOFTFP;
void
my_glMaterialf(GLenum face, GLenum pname, GLfloat param) SOFTFP;
void
my_glMaterialfv(GLenum face, GLenum pname, const GLfloat *params) SOFTFP;
void
my_glMultMatrixf(const GLfloat *m) SOFTFP;
void
my_glMultiTexCoord4f(GLenum target, GLfloat s, GLfloat t, GLfloat r, GLfloat q) SOFTFP;
void
my_glNormal3f(GLfloat nx, GLfloat ny, GLfloat nz) SOFTFP;
void
my_glOrthof(GLfloat left, GLfloat right, GLfloat bottom, GLfloat top, GLfloat zNear, GLfloat zFar) SOFTFP;
void
my_glPointParameterf(GLenum pname, GLfloat param) SOFTFP;
void
my_glPointParameterfv(GLenum pname, const GLfloat *params) SOFTFP;
void
my_glPointSize(GLfloat size) SOFTFP;
void
my_glPolygonOffset(GLfloat factor, GLfloat units) SOFTFP;
void
my_glRotatef(GLfloat angle, GLfloat x, GLfloat y, GLfloat z) SOFTFP;
void
my_glScalef(GLfloat x, GLfloat y, GLfloat z) SOFTFP;
void
my_glTexEnvf(GLenum target, GLenum pname, GLfloat param) SOFTFP;
void
my_glTexEnvfv(GLenum target, GLenum pname, const GLfloat *params) SOFTFP;
void
my_glTexParameterf(GLenum target, GLenum pname, GLfloat param) SOFTFP;
void
my_glTexParameterfv(GLenum target, GLenum pname, const GLfloat *params) SOFTFP;
void
my_glTranslatef(GLfloat x, GLfloat y, GLfloat z) SOFTFP;
void
my_glActiveTexture(GLenum texture) SOFTFP;
void
my_glAlphaFuncx(GLenum func, GLclampx ref) SOFTFP;
void
my_glBindBuffer(GLenum target, GLuint buffer) SOFTFP;
void
my_glBindTexture(GLenum target, GLuint texture) SOFTFP;
void
my_glBlendFunc(GLenum sfactor, GLenum dfactor) SOFTFP;
void
my_glBufferData(GLenum target, GLsizeiptr size, const GLvoid *data, GLenum usage) SOFTFP;
void
my_glBufferSubData(GLenum target, GLintptr offset, GLsizeiptr size, const GLvoid *data) SOFTFP;
void
my_glClear(GLbitfield mask) SOFTFP;
void
my_glClearColorx(GLclampx red, GLclampx green, GLclampx blue, GLclampx alpha) SOFTFP;
void
my_glClearDepthx(GLclampx depth) SOFTFP;
void
my_glClearStencil(GLint s) SOFTFP;
void
my_glClientActiveTexture(GLenum texture) SOFTFP;
void
my_glClipPlanex(GLenum plane, const GLfixed *equation) SOFTFP;
void
my_glColor4ub(GLubyte red, GLubyte green, GLubyte blue, GLubyte alpha) SOFTFP;
void
my_glColor4x(GLfixed red, GLfixed green, GLfixed blue, GLfixed alpha) SOFTFP;
void
my_glColorMask(GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha) SOFTFP;
void
my_glColorPointer(GLint size, GLenum type, GLsizei stride, const GLvoid *pointer) SOFTFP;
void
my_glCompressedTexImage2D(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLsizei imageSize, const GLvoid *data) SOFTFP;
void
my_glCompressedTexSubImage2D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLsizei imageSize, const GLvoid *data) SOFTFP;
void
my_glCopyTexImage2D(GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLsizei height, GLint border) SOFTFP;
void
my_glCopyTexSubImage2D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height) SOFTFP;
void
my_glCullFace(GLenum mode) SOFTFP;
void
my_glDeleteBuffers(GLsizei n, const GLuint *buffers) SOFTFP;
void
my_glDeleteTextures(GLsizei n, const GLuint *textures) SOFTFP;
void
my_glDepthFunc(GLenum func) SOFTFP;
void
my_glDepthMask(GLboolean flag) SOFTFP;
void
my_glDepthRangex(GLclampx zNear, GLclampx zFar) SOFTFP;
void
my_glDisable(GLenum cap) SOFTFP;
void
my_glDisableClientState(GLenum array) SOFTFP;
void
my_glDrawArrays(GLenum mode, GLint first, GLsizei count) SOFTFP;
void
my_glDrawElements(GLenum mode, GLsizei count, GLenum type, const GLvoid *indices) SOFTFP;
void
my_glEnable(GLenum cap) SOFTFP;
void
my_glEnableClientState(GLenum array) SOFTFP;
void
my_glFinish() SOFTFP;
void
my_glFlush() SOFTFP;
void
my_glFogx(GLenum pname, GLfixed param) SOFTFP;
void
my_glFogxv(GLenum pname, const GLfixed *params) SOFTFP;
void
my_glFrontFace(GLenum mode) SOFTFP;
void
my_glFrustumx(GLfixed left, GLfixed right, GLfixed bottom, GLfixed top, GLfixed zNear, GLfixed zFar) SOFTFP;
void
my_glGetBooleanv(GLenum pname, GLboolean *params) SOFTFP;
void
my_glGetBufferParameteriv(GLenum target, GLenum pname, GLint *params) SOFTFP;
void
my_glGetClipPlanex(GLenum pname, GLfixed eqn[4]) SOFTFP;
void
my_glGenBuffers(GLsizei n, GLuint *buffers) SOFTFP;
void
my_glGenTextures(GLsizei n, GLuint *textures) SOFTFP;
GLenum
my_glGetError() SOFTFP;
void
my_glGetFixedv(GLenum pname, GLfixed *params) SOFTFP;
void
my_glGetIntegerv(GLenum pname, GLint *params) SOFTFP;
void
my_glGetLightxv(GLenum light, GLenum pname, GLfixed *params) SOFTFP;
void
my_glGetMaterialxv(GLenum face, GLenum pname, GLfixed *params) SOFTFP;
void
my_glGetPointerv(GLenum pname, void **params) SOFTFP;
const GLubyte *
my_glGetString(GLenum name) SOFTFP;
void
my_glGetTexEnviv(GLenum env, GLenum pname, GLint *params) SOFTFP;
void
my_glGetTexEnvxv(GLenum env, GLenum pname, GLfixed *params) SOFTFP;
void
my_glGetTexParameteriv(GLenum target, GLenum pname, GLint *params) SOFTFP;
void
my_glGetTexParameterxv(GLenum target, GLenum pname, GLfixed *params) SOFTFP;
void
my_glHint(GLenum target, GLenum mode) SOFTFP;
GLboolean
my_glIsBuffer(GLuint buffer) SOFTFP;
GLboolean
my_glIsEnabled(GLenum cap) SOFTFP;
GLboolean
my_glIsTexture(GLuint texture) SOFTFP;
void
my_glLightModelx(GLenum pname, GLfixed param) SOFTFP;
void
my_glLightModelxv(GLenum pname, const GLfixed *params) SOFTFP;
void
my_glLightx(GLenum light, GLenum pname, GLfixed param) SOFTFP;
void
my_glLightxv(GLenum light, GLenum pname, const GLfixed *params) SOFTFP;
void
my_glLineWidthx(GLfixed width) SOFTFP;
void
my_glLoadIdentity() SOFTFP;
void
my_glLoadMatrixx(const GLfixed *m) SOFTFP;
void
my_glLogicOp(GLenum opcode) SOFTFP;
void
my_glMaterialx(GLenum face, GLenum pname, GLfixed param) SOFTFP;
void
my_glMaterialxv(GLenum face, GLenum pname, const GLfixed *params) SOFTFP;
void
my_glMatrixMode(GLenum mode) SOFTFP;
void
my_glMultMatrixx(const GLfixed *m) SOFTFP;
void
my_glMultiTexCoord4x(GLenum target, GLfixed s, GLfixed t, GLfixed r, GLfixed q) SOFTFP;
void
my_glNormal3x(GLfixed nx, GLfixed ny, GLfixed nz) SOFTFP;
void
my_glNormalPointer(GLenum type, GLsizei stride, const GLvoid *pointer) SOFTFP;
void
my_glOrthox(GLfixed left, GLfixed right, GLfixed bottom, GLfixed top, GLfixed zNear, GLfixed zFar) SOFTFP;
void
my_glPixelStorei(GLenum pname, GLint param) SOFTFP;
void
my_glPointParameterx(GLenum pname, GLfixed param) SOFTFP;
void
my_glPointParameterxv(GLenum pname, const GLfixed *params) SOFTFP;
void
my_glPointSizex(GLfixed size) SOFTFP;
void
my_glPolygonOffsetx(GLfixed factor, GLfixed units) SOFTFP;
void
my_glPopMatrix() SOFTFP;
void
my_glPushMatrix() SOFTFP;
void
my_glReadPixels(GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, GLvoid *pixels) SOFTFP;
void
my_glRotatex(GLfixed angle, GLfixed x, GLfixed y, GLfixed z) SOFTFP;
void
my_glSampleCoverage(GLclampf value, GLboolean invert) SOFTFP;
void
my_glSampleCoveragex(GLclampx value, GLboolean invert) SOFTFP;
void
my_glScalex(GLfixed x, GLfixed y, GLfixed z) SOFTFP;
void
my_glScissor(GLint x, GLint y, GLsizei width, GLsizei height) SOFTFP;
void
my_glShadeModel(GLenum mode) SOFTFP;
void
my_glStencilFunc(GLenum func, GLint ref, GLuint mask) SOFTFP;
void
my_glStencilMask(GLuint mask) SOFTFP;
void
my_glStencilOp(GLenum fail, GLenum zfail, GLenum zpass) SOFTFP;
void
my_glTexCoordPointer(GLint size, GLenum type, GLsizei stride, const GLvoid *pointer) SOFTFP;
void
my_glTexEnvi(GLenum target, GLenum pname, GLint param) SOFTFP;
void
my_glTexEnvx(GLenum target, GLenum pname, GLfixed param) SOFTFP;
void
my_glTexEnviv(GLenum target, GLenum pname, const GLint *params) SOFTFP;
void
my_glTexEnvxv(GLenum target, GLenum pname, const GLfixed *params) SOFTFP;
void
my_glTexImage2D(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid *pixels) SOFTFP;
void
my_glTexParameteri(GLenum target, GLenum pname, GLint param) SOFTFP;
void
my_glTexParameterx(GLenum target, GLenum pname, GLfixed param) SOFTFP;
void
my_glTexParameteriv(GLenum target, GLenum pname, const GLint *params) SOFTFP;
void
my_glTexParameterxv(GLenum target, GLenum pname, const GLfixed *params) SOFTFP;
void
my_glTexSubImage2D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid *pixels) SOFTFP;
void
my_glTranslatex(GLfixed x, GLfixed y, GLfixed z) SOFTFP;
void
my_glVertexPointer(GLint size, GLenum type, GLsizei stride, const GLvoid *pointer) SOFTFP;
void
my_glViewport(GLint x, GLint y, GLsizei width, GLsizei height) SOFTFP;
void
my_glPointSizePointerOES(GLenum type, GLsizei stride, const GLvoid *pointer) SOFTFP;
void
my_glBlendEquationSeparateOES(GLenum modeRGB, GLenum modeAlpha) SOFTFP;
void
my_glBlendFuncSeparateOES(GLenum srcRGB, GLenum dstRGB, GLenum srcAlpha, GLenum dstAlpha) SOFTFP;
void
my_glBlendEquationOES(GLenum mode) SOFTFP;
void
my_glDrawTexsOES(GLshort x, GLshort y, GLshort z, GLshort width, GLshort height) SOFTFP;
void
my_glDrawTexiOES(GLint x, GLint y, GLint z, GLint width, GLint height) SOFTFP;
void
my_glDrawTexxOES(GLfixed x, GLfixed y, GLfixed z, GLfixed width, GLfixed height) SOFTFP;
void
my_glDrawTexsvOES(const GLshort *coords) SOFTFP;
void
my_glDrawTexivOES(const GLint *coords) SOFTFP;
void
my_glDrawTexxvOES(const GLfixed *coords) SOFTFP;
void
my_glDrawTexfOES(GLfloat x, GLfloat y, GLfloat z, GLfloat width, GLfloat height) SOFTFP;
void
my_glDrawTexfvOES(const GLfloat *coords) SOFTFP;
void
my_glAlphaFuncxOES(GLenum func, GLclampx ref) SOFTFP;
void
my_glClearColorxOES(GLclampx red, GLclampx green, GLclampx blue, GLclampx alpha) SOFTFP;
void
my_glClearDepthxOES(GLclampx depth) SOFTFP;
void
my_glClipPlanexOES(GLenum plane, const GLfixed *equation) SOFTFP;
void
my_glColor4xOES(GLfixed red, GLfixed green, GLfixed blue, GLfixed alpha) SOFTFP;
void
my_glDepthRangexOES(GLclampx zNear, GLclampx zFar) SOFTFP;
void
my_glFogxOES(GLenum pname, GLfixed param) SOFTFP;
void
my_glFogxvOES(GLenum pname, const GLfixed *params) SOFTFP;
void
my_glFrustumxOES(GLfixed left, GLfixed right, GLfixed bottom, GLfixed top, GLfixed zNear, GLfixed zFar) SOFTFP;
void
my_glGetClipPlanexOES(GLenum pname, GLfixed eqn[4]) SOFTFP;
void
my_glGetFixedvOES(GLenum pname, GLfixed *params) SOFTFP;
void
my_glGetLightxvOES(GLenum light, GLenum pname, GLfixed *params) SOFTFP;
void
my_glGetMaterialxvOES(GLenum face, GLenum pname, GLfixed *params) SOFTFP;
void
my_glGetTexEnvxvOES(GLenum env, GLenum pname, GLfixed *params) SOFTFP;
void
my_glGetTexParameterxvOES(GLenum target, GLenum pname, GLfixed *params) SOFTFP;
void
my_glLightModelxOES(GLenum pname, GLfixed param) SOFTFP;
void
my_glLightModelxvOES(GLenum pname, const GLfixed *params) SOFTFP;
void
my_glLightxOES(GLenum light, GLenum pname, GLfixed param) SOFTFP;
void
my_glLightxvOES(GLenum light, GLenum pname, const GLfixed *params) SOFTFP;
void
my_glLineWidthxOES(GLfixed width) SOFTFP;
void
my_glLoadMatrixxOES(const GLfixed *m) SOFTFP;
void
my_glMaterialxOES(GLenum face, GLenum pname, GLfixed param) SOFTFP;
void
my_glMaterialxvOES(GLenum face, GLenum pname, const GLfixed *params) SOFTFP;
void
my_glMultMatrixxOES(const GLfixed *m) SOFTFP;
void
my_glMultiTexCoord4xOES(GLenum target, GLfixed s, GLfixed t, GLfixed r, GLfixed q) SOFTFP;
void
my_glNormal3xOES(GLfixed nx, GLfixed ny, GLfixed nz) SOFTFP;
void
my_glOrthoxOES(GLfixed left, GLfixed right, GLfixed bottom, GLfixed top, GLfixed zNear, GLfixed zFar) SOFTFP;
void
my_glPointParameterxOES(GLenum pname, GLfixed param) SOFTFP;
void
my_glPointParameterxvOES(GLenum pname, const GLfixed *params) SOFTFP;
void
my_glPointSizexOES(GLfixed size) SOFTFP;
void
my_glPolygonOffsetxOES(GLfixed factor, GLfixed units) SOFTFP;
void
my_glRotatexOES(GLfixed angle, GLfixed x, GLfixed y, GLfixed z) SOFTFP;
void
my_glSampleCoveragexOES(GLclampx value, GLboolean invert) SOFTFP;
void
my_glScalexOES(GLfixed x, GLfixed y, GLfixed z) SOFTFP;
void
my_glTexEnvxOES(GLenum target, GLenum pname, GLfixed param) SOFTFP;
void
my_glTexEnvxvOES(GLenum target, GLenum pname, const GLfixed *params) SOFTFP;
void
my_glTexParameterxOES(GLenum target, GLenum pname, GLfixed param) SOFTFP;
void
my_glTexParameterxvOES(GLenum target, GLenum pname, const GLfixed *params) SOFTFP;
void
my_glTranslatexOES(GLfixed x, GLfixed y, GLfixed z) SOFTFP;
GLboolean
my_glIsRenderbufferOES(GLuint renderbuffer) SOFTFP;
void
my_glBindRenderbufferOES(GLenum target, GLuint renderbuffer) SOFTFP;
void
my_glDeleteRenderbuffersOES(GLsizei n, const GLuint *renderbuffers) SOFTFP;
void
my_glGenRenderbuffersOES(GLsizei n, GLuint *renderbuffers) SOFTFP;
void
my_glRenderbufferStorageOES(GLenum target, GLenum internalformat, GLsizei width, GLsizei height) SOFTFP;
void
my_glGetRenderbufferParameterivOES(GLenum target, GLenum pname, GLint *params) SOFTFP;
GLboolean
my_glIsFramebufferOES(GLuint framebuffer) SOFTFP;
void
my_glBindFramebufferOES(GLenum target, GLuint framebuffer) SOFTFP;
void
my_glDeleteFramebuffersOES(GLsizei n, const GLuint *framebuffers) SOFTFP;
void
my_glGenFramebuffersOES(GLsizei n, GLuint *framebuffers) SOFTFP;
GLenum
my_glCheckFramebufferStatusOES(GLenum target) SOFTFP;
void
my_glFramebufferRenderbufferOES(GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer) SOFTFP;
void
my_glFramebufferTexture2DOES(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level) SOFTFP;
void
my_glGetFramebufferAttachmentParameterivOES(GLenum target, GLenum attachment, GLenum pname, GLint *params) SOFTFP;
void
my_glGenerateMipmapOES(GLenum target) SOFTFP;
void
my_glCurrentPaletteMatrixOES(GLuint matrixpaletteindex) SOFTFP;
void
my_glLoadPaletteFromModelViewMatrixOES() SOFTFP;
void
my_glMatrixIndexPointerOES(GLint size, GLenum type, GLsizei stride, const GLvoid *pointer) SOFTFP;
void
my_glWeightPointerOES(GLint size, GLenum type, GLsizei stride, const GLvoid *pointer) SOFTFP;
GLbitfield
my_glQueryMatrixxOES(GLfixed mantissa[16], GLint exponent[16]) SOFTFP;
void
my_glDepthRangefOES(GLclampf zNear, GLclampf zFar) SOFTFP;
void
my_glFrustumfOES(GLfloat left, GLfloat right, GLfloat bottom, GLfloat top, GLfloat zNear, GLfloat zFar) SOFTFP;
void
my_glOrthofOES(GLfloat left, GLfloat right, GLfloat bottom, GLfloat top, GLfloat zNear, GLfloat zFar) SOFTFP;
void
my_glClipPlanefOES(GLenum plane, const GLfloat *equation) SOFTFP;
void
my_glGetClipPlanefOES(GLenum pname, GLfloat eqn[4]) SOFTFP;
void
my_glClearDepthfOES(GLclampf depth) SOFTFP;
void
my_glTexGenfOES(GLenum coord, GLenum pname, GLfloat param) SOFTFP;
void
my_glTexGenfvOES(GLenum coord, GLenum pname, const GLfloat *params) SOFTFP;
void
my_glTexGeniOES(GLenum coord, GLenum pname, GLint param) SOFTFP;
void
my_glTexGenivOES(GLenum coord, GLenum pname, const GLint *params) SOFTFP;
void
my_glTexGenxOES(GLenum coord, GLenum pname, GLfixed param) SOFTFP;
void
my_glTexGenxvOES(GLenum coord, GLenum pname, const GLfixed *params) SOFTFP;
void
my_glGetTexGenfvOES(GLenum coord, GLenum pname, GLfloat *params) SOFTFP;
void
my_glGetTexGenivOES(GLenum coord, GLenum pname, GLint *params) SOFTFP;
void
my_glGetTexGenxvOES(GLenum coord, GLenum pname, GLfixed *params) SOFTFP;
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
my_glTexBindStreamIMG(GLint device, GLint deviceoffset) SOFTFP;
void
my_glGetTexStreamDeviceAttributeivIMG(GLint device, GLenum pname, GLint *params) SOFTFP;
const GLubyte *
my_glGetTexStreamDeviceNameIMG(GLint device) SOFTFP;
void
my_glVertexAttrib4fARB(GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w) SOFTFP;
void
my_glProgramEnvParameter4fARB(GLenum target, GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w) SOFTFP;
void
my_glProgramEnvParameter4fvARB(GLenum target, GLuint index, const GLfloat *params) SOFTFP;
void
my_glProgramLocalParameter4fARB(GLenum target, GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w) SOFTFP;
void
my_glProgramLocalParameter4fvARB(GLenum target, GLuint index, const GLfloat *params) SOFTFP;
void
my_glVertexAttrib4xIMG(GLuint index, GLfixed x, GLfixed y, GLfixed z, GLfixed w) SOFTFP;
void
my_glProgramLocalParameter4xIMG(GLenum target, GLuint index, GLfixed x, GLfixed y, GLfixed z, GLfixed w) SOFTFP;
void
my_glProgramLocalParameter4xvIMG(GLenum target, GLuint index, const GLfixed *params) SOFTFP;
void
my_glProgramEnvParameter4xIMG(GLenum target, GLuint index, GLfixed x, GLfixed y, GLfixed z, GLfixed w) SOFTFP;
void
my_glProgramEnvParameter4xvIMG(GLenum target, GLuint index, const GLfixed *params) SOFTFP;
void
my_glVertexAttribPointerARB(GLuint index, GLsizei size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid *pointer) SOFTFP;
void
my_glEnableVertexAttribArrayARB(GLuint index) SOFTFP;
void
my_glDisableVertexAttribArrayARB(GLuint index) SOFTFP;
void
my_glProgramStringARB(GLenum target, GLenum format, GLsizei len, const void *string) SOFTFP;
void
my_glBindProgramARB(GLenum target, GLuint program) SOFTFP;
void
my_glDeleteProgramsARB(GLsizei n, const GLuint *programs) SOFTFP;
void
my_glGenProgramsARB(GLsizei n, GLuint *programs) SOFTFP;
void
my_glMultiDrawArraysEXT(GLenum mode, GLint *first, GLsizei *count, GLsizei primcount) SOFTFP;
void
my_glMultiDrawElementsEXT(GLenum mode, const GLsizei *count, GLenum type, const GLvoid **indices, GLsizei primcount) SOFTFP;


///
void
gles_extensions_init();

#else

#define gles_extensions_init()

#endif /* APKENV_GLES */
