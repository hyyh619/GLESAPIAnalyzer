#ifndef __API_OES_ENGINE_H
#define __API_OES_ENGINE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

#ifdef WIN32
#include <windows.h>
#endif
#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <GLES3/gl31.h>
#include <GLES3/gl2ext.h>

#include "APIOS.h"

typedef void                GLvoid;
typedef unsigned int        GLenum;
typedef unsigned char       GLboolean;
typedef unsigned int        GLbitfield;
typedef signed   char       GLbyte;
typedef short               GLshort;
typedef int                 GLint;
typedef int                 GLsizei;
typedef unsigned char       GLubyte;
typedef unsigned short      GLushort;
typedef unsigned int        GLuint;
typedef float               GLfloat;
typedef float               GLclampf;
typedef __int32             GLfixed;
typedef __int32             GLclampx;

typedef double              GLdouble;
typedef double              GLclampd;
typedef struct __GLsync     *GLsync;
typedef __int64             GLint64;
typedef unsigned __int64    GLuint64;
typedef char                GLchar;
typedef signed long int     GLintptr;
typedef signed   long  int  GLsizeiptr;
typedef int                 GLsizei;

#define GL_MULTISAMPLE      0x809D
#define GL_EXTENSIONS       0x1F03

/* Private data structure. */
typedef struct _stPrivacy
{
    LIB_HANDLE  moduleEGL;
    LIB_HANDLE  moduleGLES3;
    LIB_HANDLE  moduleGLES1;
    int         reference;
    char*       extensions;
    char*       wglextensions;
} stPrivacy;

typedef struct _stEngine
{
    /* EGL functions */
    EGLBoolean (APIENTRY * eglChooseConfig) (EGLDisplay dpy, const EGLint *attrib_list, EGLConfig *configs, EGLint config_size, EGLint *num_config);
    EGLBoolean (APIENTRY * eglCopyBuffers) (EGLDisplay dpy, EGLSurface surface, EGLNativePixmapType target);
    EGLContext (APIENTRY * eglCreateContext) (EGLDisplay dpy, EGLConfig config, EGLContext share_context, const EGLint *attrib_list);
    EGLSurface (APIENTRY * eglCreatePbufferSurface) (EGLDisplay dpy, EGLConfig config, const EGLint *attrib_list);
    EGLSurface (APIENTRY * eglCreatePixmapSurface) (EGLDisplay dpy, EGLConfig config, EGLNativePixmapType pixmap, const EGLint *attrib_list);
    EGLSurface (APIENTRY * eglCreateWindowSurface) (EGLDisplay dpy, EGLConfig config, EGLNativeWindowType win, const EGLint *attrib_list);
    EGLBoolean (APIENTRY * eglDestroyContext) (EGLDisplay dpy, EGLContext ctx);
    EGLBoolean (APIENTRY * eglDestroySurface) (EGLDisplay dpy, EGLSurface surface);
    EGLBoolean (APIENTRY * eglGetConfigAttrib) (EGLDisplay dpy, EGLConfig config, EGLint attribute, EGLint *value);
    EGLBoolean (APIENTRY * eglGetConfigs) (EGLDisplay dpy, EGLConfig *configs, EGLint config_size, EGLint *num_config);
    EGLDisplay (APIENTRY * eglGetCurrentDisplay) (void);
    EGLSurface (APIENTRY * eglGetCurrentSurface) (EGLint readdraw);
    EGLDisplay (APIENTRY * eglGetDisplay) (EGLNativeDisplayType display_id);
    EGLint (APIENTRY * eglGetError) (void);
    EGLBoolean (APIENTRY * eglSetFIFO) (void*);
    __eglMustCastToProperFunctionPointerType (APIENTRY * eglGetProcAddress) (const char *procname);
    EGLBoolean (APIENTRY * eglInitialize) (EGLDisplay dpy, EGLint *major, EGLint *minor);
    EGLBoolean (APIENTRY * eglMakeCurrent) (EGLDisplay dpy, EGLSurface draw, EGLSurface read, EGLContext ctx);
    EGLBoolean (APIENTRY * eglQueryContext) (EGLDisplay dpy, EGLContext ctx, EGLint attribute, EGLint *value);
    const char *(APIENTRY * eglQueryString) (EGLDisplay dpy, EGLint name);
    EGLBoolean (APIENTRY * eglQuerySurface) (EGLDisplay dpy, EGLSurface surface, EGLint attribute, EGLint *value);
    EGLBoolean (APIENTRY * eglSwapBuffers) (EGLDisplay dpy, EGLSurface surface);
    EGLBoolean (APIENTRY * eglTerminate) (EGLDisplay dpy);
    EGLBoolean (APIENTRY * eglWaitGL) (void);
    EGLBoolean (APIENTRY * eglWaitNative) (EGLint engine);
    EGLBoolean (APIENTRY * eglBindTexImage) (EGLDisplay dpy, EGLSurface surface, EGLint buffer);
    EGLBoolean (APIENTRY * eglReleaseTexImage) (EGLDisplay dpy, EGLSurface surface, EGLint buffer);
    EGLBoolean (APIENTRY * eglSurfaceAttrib) (EGLDisplay dpy, EGLSurface surface, EGLint attribute, EGLint value);
    EGLBoolean (APIENTRY * eglSwapInterval) (EGLDisplay dpy, EGLint interval);
    EGLBoolean (APIENTRY * eglBindAPI) (EGLenum api);
    EGLenum (APIENTRY * eglQueryAPI) (void);
    EGLSurface (APIENTRY * eglCreatePbufferFromClientBuffer) (EGLDisplay dpy, EGLenum buftype, EGLClientBuffer buffer, EGLConfig config, const EGLint *attrib_list);
    EGLBoolean (APIENTRY * eglReleaseThread) (void);
    EGLBoolean (APIENTRY * eglWaitClient) (void);
    EGLContext (APIENTRY * eglGetCurrentContext) (void);

    /* ------------- OpenGL ES 2.0 core functions -------------- */
    void            (APIENTRY * glActiveTexture) (GLenum texture);
    void            (APIENTRY * glAttachShader) (GLuint program, GLuint shader);
    void            (APIENTRY * glBindAttribLocation) (GLuint program, GLuint index, const char* name);
    void            (APIENTRY * glBindBuffer) (GLenum target, GLuint buffer);
    void            (APIENTRY * glBindFramebuffer) (GLenum target, GLuint framebuffer);
    void            (APIENTRY * glBindRenderbuffer) (GLenum target, GLuint renderbuffer);
    void            (APIENTRY * glBindTexture) (GLenum target, GLuint texture);
    void            (APIENTRY * glBlendColor) (GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha);
    void            (APIENTRY * glBlendEquation) ( GLenum mode );
    void            (APIENTRY * glBlendEquationSeparate) (GLenum modeRGB, GLenum modeAlpha);
    void            (APIENTRY * glBlendFunc) (GLenum sfactor, GLenum dfactor);
    void            (APIENTRY * glBlendFuncSeparate) (GLenum srcRGB, GLenum dstRGB, GLenum srcAlpha, GLenum dstAlpha);
    void            (APIENTRY * glBufferData) (GLenum target, int size, const void* data, GLenum usage);
    void            (APIENTRY * glBufferSubData) (GLenum target, int offset, int size, const void* data);
    GLenum          (APIENTRY * glCheckFramebufferStatus) (GLenum target);
    void            (APIENTRY * glClear) (GLbitfield mask);
    void            (APIENTRY * glClearColor) (GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha);
    void            (APIENTRY * glClearDepthf) (GLclampf depth);
    void            (APIENTRY * glClearStencil) (GLint s);
    void            (APIENTRY * glColorMask) (GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha);
    void            (APIENTRY * glCompileShader) (GLuint shader);
    void            (APIENTRY * glCompressedTexImage2D) (GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLsizei imageSize, const void* data);
    void            (APIENTRY * glCompressedTexSubImage2D) (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLsizei imageSize, const void* data);
    void            (APIENTRY * glCopyTexImage2D) (GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLsizei height, GLint border);
    void            (APIENTRY * glCopyTexSubImage2D) (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height);
    GLuint          (APIENTRY * glCreateProgram) (void);
    GLuint          (APIENTRY * glCreateShader) (GLenum type);
    void            (APIENTRY * glCullFace) (GLenum mode);
    void            (APIENTRY * glDeleteBuffers) (GLsizei n, const GLuint* buffers);
    void            (APIENTRY * glDeleteFramebuffers) (GLsizei n, const GLuint* framebuffers);
    void            (APIENTRY * glDeleteProgram) (GLuint program);
    void            (APIENTRY * glDeleteRenderbuffers) (GLsizei n, const GLuint* renderbuffers);
    void            (APIENTRY * glDeleteShader) (GLuint shader);
    void            (APIENTRY * glDeleteTextures) (GLsizei n, const GLuint* textures);
    void            (APIENTRY * glDepthFunc) (GLenum func);
    void            (APIENTRY * glDepthMask) (GLboolean flag);
    void            (APIENTRY * glDepthRangef) (GLclampf zNear, GLclampf zFar);
    void            (APIENTRY * glDetachShader) (GLuint program, GLuint shader);
    void            (APIENTRY * glDisable) (GLenum cap);
    void            (APIENTRY * glDisableVertexAttribArray) (GLuint index);
    void            (APIENTRY * glDrawArrays) (GLenum mode, GLint first, GLsizei count);
    void            (APIENTRY * glDrawElements) (GLenum mode, GLsizei count, GLenum type, const void* indices);
    void            (APIENTRY * glEnable) (GLenum cap);
    void            (APIENTRY * glEnableVertexAttribArray) (GLuint index);
    void            (APIENTRY * glFinish) (void);
    void            (APIENTRY * glFlush) (void);
    void            (APIENTRY * glFramebufferRenderbuffer) (GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer);
    void            (APIENTRY * glFramebufferTexture2D) (GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level);
    void            (APIENTRY * glFramebufferTexture3D) (GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level, GLint zoffset);
    void            (APIENTRY * glFrontFace) (GLenum mode);
    void            (APIENTRY * glGenBuffers) (GLsizei n, GLuint* buffers);
    void            (APIENTRY * glGenerateMipmap) (GLenum target);
    void            (APIENTRY * glGenFramebuffers) (GLsizei n, GLuint* framebuffers);
    void            (APIENTRY * glGenRenderbuffers) (GLsizei n, GLuint* renderbuffers);
    void            (APIENTRY * glGenTextures) (GLsizei n, GLuint* textures);
    void            (APIENTRY * glGetActiveAttrib) (GLuint program, GLuint index, GLsizei bufsize, GLsizei* length, GLint* size, GLenum* type, char* name);
    void            (APIENTRY * glGetActiveUniform) (GLuint program, GLuint index, GLsizei bufsize, GLsizei* length, GLint* size, GLenum* type, char* name);
    void            (APIENTRY * glGetAttachedShaders) (GLuint program, GLsizei maxcount, GLsizei* count, GLuint* shaders);
    int             (APIENTRY * glGetAttribLocation) (GLuint program, const char* name);
    void            (APIENTRY * glGetBooleanv) (GLenum pname, GLboolean* params);
    void            (APIENTRY * glGetBufferParameteriv) (GLenum target, GLenum pname, GLint* params);
    GLenum          (APIENTRY * glGetError) (void);
    void            (APIENTRY * glGetFloatv) (GLenum pname, GLfloat* params);
    void            (APIENTRY * glGetFramebufferAttachmentParameteriv) (GLenum target, GLenum attachment, GLenum pname, GLint* params);
    void            (APIENTRY * glGetIntegerv) (GLenum pname, GLint* params);
    void            (APIENTRY * glGetProgramiv) (GLuint program, GLenum pname, GLint* params);
    void            (APIENTRY * glGetProgramInfoLog) (GLuint program, GLsizei bufsize, GLsizei* length, char* infolog);
    void            (APIENTRY * glGetProgramBinary) (GLuint program, GLsizei bufSize, GLsizei *length, GLenum *binaryFormat, GLvoid *binary);
    void            (APIENTRY * glGetRenderbufferParameteriv) (GLenum target, GLenum pname, GLint* params);
    void            (APIENTRY * glGetShaderiv) (GLuint shader, GLenum pname, GLint* params);
    void            (APIENTRY * glGetShaderInfoLog) (GLuint shader, GLsizei bufsize, GLsizei* length, char* infolog);
    void            (APIENTRY * glGetShaderPrecisionFormat) (GLenum shadertype, GLenum precisiontype, GLint* range, GLint* precision);
    void            (APIENTRY * glGetShaderSource) (GLuint shader, GLsizei bufsize, GLsizei* length, char* source);
    const GLubyte * (APIENTRY * glGetString) (GLenum name);
    void            (APIENTRY * glGetTexParameterfv) (GLenum target, GLenum pname, GLfloat* params);
    void            (APIENTRY * glGetTexParameteriv) (GLenum target, GLenum pname, GLint* params);
    void            (APIENTRY * glGetUniformfv) (GLuint program, GLint location, GLfloat* params);
    void            (APIENTRY * glGetUniformiv) (GLuint program, GLint location, GLint* params);
    int             (APIENTRY * glGetUniformLocation) (GLuint program, const char* name);
    void            (APIENTRY * glGetVertexAttribfv) (GLuint index, GLenum pname, GLfloat* params);
    void            (APIENTRY * glGetVertexAttribiv) (GLuint index, GLenum pname, GLint* params);
    void            (APIENTRY * glGetVertexAttribPointerv) (GLuint index, GLenum pname, void** pointer);
    void            (APIENTRY * glHint) (GLenum target, GLenum mode);
    GLboolean       (APIENTRY * glIsBuffer) (GLuint buffer);
    GLboolean       (APIENTRY * glIsEnabled) (GLenum cap);
    GLboolean       (APIENTRY * glIsFramebuffer) (GLuint framebuffer);
    GLboolean       (APIENTRY * glIsProgram) (GLuint program);
    GLboolean       (APIENTRY * glIsRenderbuffer) (GLuint renderbuffer);
    GLboolean       (APIENTRY * glIsShader) (GLuint shader);
    GLboolean       (APIENTRY * glIsTexture) (GLuint texture);
    void            (APIENTRY * glLineWidth) (GLfloat width);
    void            (APIENTRY * glLinkProgram) (GLuint program);
    void            (APIENTRY * glProgramBinary) (GLuint program, GLenum binaryFormat, const GLvoid *binary, GLint length);
    void            (APIENTRY * glPixelStorei) (GLenum pname, GLint param);
    void            (APIENTRY * glPolygonOffset) (GLfloat factor, GLfloat units);
    void            (APIENTRY * glReadPixels) (GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, void* pixels);
    void            (APIENTRY * glReleaseShaderCompiler) (void);
    void            (APIENTRY * glRenderbufferStorage) (GLenum target, GLenum internalformat, GLsizei width, GLsizei height);
    void            (APIENTRY * glSampleCoverage) (GLclampf value, GLboolean invert);
    void            (APIENTRY * glScissor) (GLint x, GLint y, GLsizei width, GLsizei height);
    void            (APIENTRY * glShaderBinary) (GLint n, const GLuint* shaders, GLenum binaryformat, const void* binary, GLint length);
#ifndef EGL_OPENGL_ES3_BIT_KHR
    void            (APIENTRY * glShaderSource) (GLuint shader, GLsizei count, const GLchar** string, const GLint* length);
#else
    void            (APIENTRY * glShaderSource) (GLuint shader, GLsizei count, const GLchar* const* string, const GLint* length);
#endif
    void            (APIENTRY * glStencilFunc) (GLenum func, GLint ref, GLuint mask);
    void            (APIENTRY * glStencilFuncSeparate) (GLenum face, GLenum func, GLint ref, GLuint mask);
    void            (APIENTRY * glStencilMask) (GLuint mask);
    void            (APIENTRY * glStencilMaskSeparate) (GLenum face, GLuint mask);
    void            (APIENTRY * glStencilOp) (GLenum fail, GLenum zfail, GLenum zpass);
    void            (APIENTRY * glStencilOpSeparate) (GLenum face, GLenum fail, GLenum zfail, GLenum zpass);
    void            (APIENTRY * glTexImage2D) (GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const void* pixels);
    void            (APIENTRY * glTexParameterf) (GLenum target, GLenum pname, GLfloat param);
    void            (APIENTRY * glTexParameterfv) (GLenum target, GLenum pname, const GLfloat* params);
    void            (APIENTRY * glTexParameteri) (GLenum target, GLenum pname, GLint param);
    void            (APIENTRY * glTexParameteriv) (GLenum target, GLenum pname, const GLint* params);
    void            (APIENTRY * glTexSubImage2D) (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const void* pixels);
    void            (APIENTRY * glUniform1f) (GLint location, GLfloat x);
    void            (APIENTRY * glUniform1fv) (GLint location, GLsizei count, const GLfloat* v);
    void            (APIENTRY * glUniform1i) (GLint location, GLint x);
    void            (APIENTRY * glUniform1iv) (GLint location, GLsizei count, const GLint* v);
    void            (APIENTRY * glUniform2f) (GLint location, GLfloat x, GLfloat y);
    void            (APIENTRY * glUniform2fv) (GLint location, GLsizei count, const GLfloat* v);
    void            (APIENTRY * glUniform2i) (GLint location, GLint x, GLint y);
    void            (APIENTRY * glUniform2iv) (GLint location, GLsizei count, const GLint* v);
    void            (APIENTRY * glUniform3f) (GLint location, GLfloat x, GLfloat y, GLfloat z);
    void            (APIENTRY * glUniform3fv) (GLint location, GLsizei count, const GLfloat* v);
    void            (APIENTRY * glUniform3i) (GLint location, GLint x, GLint y, GLint z);
    void            (APIENTRY * glUniform3iv) (GLint location, GLsizei count, const GLint* v);
    void            (APIENTRY * glUniform4f) (GLint location, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
    void            (APIENTRY * glUniform4fv) (GLint location, GLsizei count, const GLfloat* v);
    void            (APIENTRY * glUniform4i) (GLint location, GLint x, GLint y, GLint z, GLint w);
    void            (APIENTRY * glUniform4iv) (GLint location, GLsizei count, const GLint* v);
    void            (APIENTRY * glUniformMatrix2fv) (GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
    void            (APIENTRY * glUniformMatrix3fv) (GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
    void            (APIENTRY * glUniformMatrix4fv) (GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
    void            (APIENTRY * glUseProgram) (GLuint program);
    void            (APIENTRY * glValidateProgram) (GLuint program);
    void            (APIENTRY * glVertexAttrib1f) (GLuint indx, GLfloat x);
    void            (APIENTRY * glVertexAttrib1fv) (GLuint indx, const GLfloat* values);
    void            (APIENTRY * glVertexAttrib2f) (GLuint indx, GLfloat x, GLfloat y);
    void            (APIENTRY * glVertexAttrib2fv) (GLuint indx, const GLfloat* values);
    void            (APIENTRY * glVertexAttrib3f) (GLuint indx, GLfloat x, GLfloat y, GLfloat z);
    void            (APIENTRY * glVertexAttrib3fv) (GLuint indx, const GLfloat* values);
    void            (APIENTRY * glVertexAttrib4f) (GLuint indx, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
    void            (APIENTRY * glVertexAttrib4fv) (GLuint indx, const GLfloat* values);
    void            (APIENTRY * glVertexAttribPointer) (GLuint indx, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void* ptr);
    void            (APIENTRY * glViewport) (GLint x, GLint y, GLsizei width, GLsizei height);

    /* es 2.0 extension functions*/
    void            (APIENTRY * glBindVertexArray) (GLuint array);
    void            (APIENTRY * glDeleteVertexArrays) (GLsizei n, const GLuint *arrays);
    void            (APIENTRY * glGenVertexArrays) (GLsizei n, GLuint *arrays);
    GLboolean       (APIENTRY * glIsVertexArray) (GLuint array);

    void            (APIENTRY * glBlitFramebufferEXT) (GLint srcX0, GLint srcY0, GLint srcX1, GLint srcY1, GLint dstX0, GLint dstY0, GLint dstX1, GLint dstY1, GLbitfield mask, GLenum filter);
    void            (APIENTRY * glRenderbufferStorageMultisampleEXT) (GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height);
    void            (APIENTRY * glGetProgramBinaryOES) (GLuint program, GLsizei bufSize, GLsizei *length, GLenum *binaryFormat, GLvoid *binary);
    void            (APIENTRY * glProgramBinaryOES) (GLuint program, GLenum binaryFormat, const GLvoid *binary, GLint length);

    /* custom function */
    GLboolean       (APIENTRY * isExtSupport)(const char* name);
    void            (APIENTRY * glBegin)(GLenum mode);
    void            (APIENTRY * glEnd)(void);
    void            (APIENTRY * glVertex3f)(GLfloat x, GLfloat y, GLfloat z);
    void            (APIENTRY * glTexCoord2f)(GLfloat s, GLfloat t);
    void            (APIENTRY * glMultiTexCoord2f)(GLenum target, GLfloat s, GLfloat t);
    void            (APIENTRY * glDrawBuffer)(GLenum mode);

    /* OpenGL ES 3.0 */
    void            (APIENTRY * glReadBuffer) (GLenum mode);
    void            (APIENTRY * glDrawRangeElements) (GLenum mode, GLuint start, GLuint end, GLsizei count, GLenum type, const GLvoid* indices);
    void            (APIENTRY * glGenQueries) (GLsizei n, GLuint* ids);
    void            (APIENTRY * glDeleteQueries) (GLsizei n, const GLuint* ids);
    GLboolean       (APIENTRY * glIsQuery) (GLuint id);
    void            (APIENTRY * glBeginQuery) (GLenum target, GLuint id);
    void            (APIENTRY * glEndQuery) (GLenum target);
    void            (APIENTRY * glGetQueryiv) (GLenum target, GLenum pname, GLint* params);
    void            (APIENTRY * glGetQueryObjectuiv) (GLuint id, GLenum pname, GLuint* params);
    GLboolean       (APIENTRY * glUnmapBuffer) (GLenum target);
    void            (APIENTRY * glGetBufferPointerv) (GLenum target, GLenum pname, GLvoid** params);
    void            (APIENTRY * glDrawBuffers) (GLsizei n, const GLenum* bufs);
    void            (APIENTRY * glUniformMatrix2x3fv) (GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
    void            (APIENTRY * glUniformMatrix3x2fv) (GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
    void            (APIENTRY * glUniformMatrix2x4fv) (GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
    void            (APIENTRY * glUniformMatrix4x2fv) (GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
    void            (APIENTRY * glUniformMatrix3x4fv) (GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
    void            (APIENTRY * glUniformMatrix4x3fv) (GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
    void            (APIENTRY * glBlitFramebuffer) (GLint srcX0, GLint srcY0, GLint srcX1, GLint srcY1, GLint dstX0, GLint dstY0, GLint dstX1, GLint dstY1, GLbitfield mask, GLenum filter);
    void            (APIENTRY * glRenderbufferStorageMultisample) (GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height);
    void            (APIENTRY * glFramebufferTextureLayer) (GLenum target, GLenum attachment, GLuint texture, GLint level, GLint layer);
    GLvoid*         (APIENTRY * glMapBufferRange) (GLenum target, GLintptr offset, GLsizeiptr length, GLbitfield access);
    void            (APIENTRY * glFlushMappedBufferRange) (GLenum target, GLintptr offset, GLsizeiptr length);
    void            (APIENTRY * glGetIntegeri_v) (GLenum target, GLuint index, GLint* data);
    void            (APIENTRY * glBeginTransformFeedback) (GLenum primitiveMode);
    void            (APIENTRY * glEndTransformFeedback) (void);
    void            (APIENTRY * glBindBufferRange) (GLenum target, GLuint index, GLuint buffer, GLintptr offset, GLsizeiptr size);
    void            (APIENTRY * glBindBufferBase) (GLenum target, GLuint index, GLuint buffer);
    void            (APIENTRY * glTransformFeedbackVaryings) (GLuint program, GLsizei count, const GLchar* const* varyings, GLenum bufferMode);
    void            (APIENTRY * glGetTransformFeedbackVarying) (GLuint program, GLuint index, GLsizei bufSize, GLsizei* length, GLsizei* size, GLenum* type, GLchar* name);
    void            (APIENTRY * glVertexAttribIPointer) (GLuint index, GLint size, GLenum type, GLsizei stride, const GLvoid* pointer);
    void            (APIENTRY * glGetVertexAttribIiv) (GLuint index, GLenum pname, GLint* params);
    void            (APIENTRY * glGetVertexAttribIuiv) (GLuint index, GLenum pname, GLuint* params);
    void            (APIENTRY * glVertexAttribI4i) (GLuint index, GLint x, GLint y, GLint z, GLint w);
    void            (APIENTRY * glVertexAttribI4ui) (GLuint index, GLuint x, GLuint y, GLuint z, GLuint w);
    void            (APIENTRY * glVertexAttribI4iv) (GLuint index, const GLint* v);
    void            (APIENTRY * glVertexAttribI4uiv) (GLuint index, const GLuint* v);
    void            (APIENTRY * glGetUniformuiv) (GLuint program, GLint location, GLuint* params);
    GLint           (APIENTRY * glGetFragDataLocation) (GLuint program, const GLchar *name);
    void            (APIENTRY * glUniform1ui) (GLint location, GLuint v0);
    void            (APIENTRY * glUniform2ui) (GLint location, GLuint v0, GLuint v1);
    void            (APIENTRY * glUniform3ui) (GLint location, GLuint v0, GLuint v1, GLuint v2);
    void            (APIENTRY * glUniform4ui) (GLint location, GLuint v0, GLuint v1, GLuint v2, GLuint v3);
    void            (APIENTRY * glUniform1uiv) (GLint location, GLsizei count, const GLuint* value);
    void            (APIENTRY * glUniform2uiv) (GLint location, GLsizei count, const GLuint* value);
    void            (APIENTRY * glUniform3uiv) (GLint location, GLsizei count, const GLuint* value);
    void            (APIENTRY * glUniform4uiv) (GLint location, GLsizei count, const GLuint* value);
    void            (APIENTRY * glClearBufferiv) (GLenum buffer, GLint drawbuffer, const GLint* value);
    void            (APIENTRY * glClearBufferuiv) (GLenum buffer, GLint drawbuffer, const GLuint* value);
    void            (APIENTRY * glClearBufferfv) (GLenum buffer, GLint drawbuffer, const GLfloat* value);
    void            (APIENTRY * glClearBufferfi) (GLenum buffer, GLint drawbuffer, GLfloat depth, GLint stencil);
    const GLubyte*  (APIENTRY * glGetStringi) (GLenum name, GLuint index);
    void            (APIENTRY * glCopyBufferSubData) (GLenum readTarget, GLenum writeTarget, GLintptr readOffset, GLintptr writeOffset, GLsizeiptr size);
    void            (APIENTRY * glGetUniformIndices) (GLuint program, GLsizei uniformCount, const GLchar* const* uniformNames, GLuint* uniformIndices);
    void            (APIENTRY * glGetActiveUniformsiv) (GLuint program, GLsizei uniformCount, const GLuint* uniformIndices, GLenum pname, GLint* params);
    GLuint          (APIENTRY * glGetUniformBlockIndex) (GLuint program, const GLchar* uniformBlockName);
    void            (APIENTRY * glGetActiveUniformBlockiv) (GLuint program, GLuint uniformBlockIndex, GLenum pname, GLint* params);
    void            (APIENTRY * glGetActiveUniformBlockName) (GLuint program, GLuint uniformBlockIndex, GLsizei bufSize, GLsizei* length, GLchar* uniformBlockName);
    void            (APIENTRY * glUniformBlockBinding) (GLuint program, GLuint uniformBlockIndex, GLuint uniformBlockBinding);
    void            (APIENTRY * glDrawArraysInstanced) (GLenum mode, GLint first, GLsizei count, GLsizei instanceCount);
    void            (APIENTRY * glDrawElementsInstanced) (GLenum mode, GLsizei count, GLenum type, const GLvoid* indices, GLsizei instanceCount);
    GLsync          (APIENTRY * glFenceSync) (GLenum condition, GLbitfield flags);
    GLboolean       (APIENTRY * glIsSync) (GLsync sync);
    void            (APIENTRY * glDeleteSync) (GLsync sync);
    GLenum          (APIENTRY * glClientWaitSync) (GLsync sync, GLbitfield flags, GLuint64 timeout);
    void            (APIENTRY * glWaitSync) (GLsync sync, GLbitfield flags, GLuint64 timeout);
    void            (APIENTRY * glGetInteger64v) (GLenum pname, GLint64* params);
    //void (APIENTRY * glGetSynciv) (GLsync sync, GLenum pname, GLsizei bufSize, GLsizei* length, GLint* values);
    void            (APIENTRY * glGetInteger64i_v) (GLenum target, GLuint index, GLint64* data);
    void            (APIENTRY * glGetBufferParameteri64v) (GLenum target, GLenum pname, GLint64* params);
    void            (APIENTRY * glGenSamplers) (GLsizei count, GLuint* samplers);
    void            (APIENTRY * glDeleteSamplers) (GLsizei count, const GLuint* samplers);
    GLboolean       (APIENTRY * glIsSampler) (GLuint sampler);
    void            (APIENTRY * glBindSampler) (GLuint unit, GLuint sampler);
    void            (APIENTRY * glSamplerParameteri) (GLuint sampler, GLenum pname, GLint param);
    void            (APIENTRY * glSamplerParameteriv) (GLuint sampler, GLenum pname, const GLint* param);
    void            (APIENTRY * glSamplerParameterf) (GLuint sampler, GLenum pname, GLfloat param);
    void            (APIENTRY * glSamplerParameterfv) (GLuint sampler, GLenum pname, const GLfloat* param);
    void            (APIENTRY * glGetSamplerParameteriv) (GLuint sampler, GLenum pname, GLint* params);
    void            (APIENTRY * glGetSamplerParameterfv) (GLuint sampler, GLenum pname, GLfloat* params);
    void            (APIENTRY * glVertexAttribDivisor) (GLuint index, GLuint divisor);
    void            (APIENTRY * glBindTransformFeedback) (GLenum target, GLuint id);
    void            (APIENTRY * glDeleteTransformFeedbacks) (GLsizei n, const GLuint* ids);
    void            (APIENTRY * glGenTransformFeedbacks) (GLsizei n, GLuint* ids);
    GLboolean       (APIENTRY * glIsTransformFeedback) (GLuint id);
    void            (APIENTRY * glPauseTransformFeedback) (void);
    void            (APIENTRY * glResumeTransformFeedback) (void);
    //void (APIENTRY * glGetProgramBinary) (GLuint program, GLsizei bufSize, GLsizei* length, GLenum* binaryFormat, GLvoid* binary);
    //void (APIENTRY * glProgramBinary) (GLuint program, GLenum binaryFormat, const GLvoid* binary, GLsizei length);
    void            (APIENTRY * glProgramParameteri) (GLuint program, GLenum pname, GLint value);
    void            (APIENTRY * glInvalidateFramebuffer) (GLenum target, GLsizei numAttachments, const GLenum* attachments);
    void            (APIENTRY * glInvalidateSubFramebuffer) (GLenum target, GLsizei numAttachments, const GLenum* attachments, GLint x, GLint y, GLsizei width, GLsizei height);
    void            (APIENTRY * glTexStorage2D) (GLenum target, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height);
    void            (APIENTRY * glTexStorage3D) (GLenum target, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth);
    void            (APIENTRY * glGetInternalformativ) (GLenum target, GLenum internalformat, GLenum pname, GLsizei bufSize, GLint* params);

    void            (APIENTRY * glTexImage3D) (GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLenum format, GLenum type, const void *pixels);
    void            (APIENTRY * glTexSubImage3D) (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const void *pixels);
    void            (APIENTRY * glCopyTexSubImage3D) (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLint x, GLint y, GLsizei width, GLsizei height);
    void            (APIENTRY * glCompressedTexImage3D) (GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLsizei imageSize, const void *data);
    void            (APIENTRY * glCompressedTexSubImage3D) (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLsizei imageSize, const void *data);
    void            (APIENTRY * glGetSynciv) (GLsync sync, GLenum pname, GLsizei bufSize, GLsizei *length, GLint *values);
} stEngine;

typedef __eglMustCastToProperFunctionPointerType (APIENTRY * PFNEGLGETPROCADDRESS) (const char *procname);
typedef void       (APIENTRY * PFNEGLCREATEIMAGEKHR) (EGLDisplay, EGLContext, EGLenum, EGLClientBuffer, const EGLint*);
typedef EGLBoolean (APIENTRY * PFNEGLCHOOSECONFIG) (EGLDisplay, const EGLint *, EGLConfig *, EGLint , EGLint *);
typedef EGLint     (APIENTRY * PFNEGLGETERROR) ();
typedef EGLBoolean (APIENTRY * PFNEGLINITIALIZE) (EGLDisplay, EGLint*, EGLint *);
typedef EGLContext (APIENTRY * PFNEGLCREATECONTEXT) (EGLDisplay, EGLConfig, EGLContext, const EGLint *);
typedef EGLBoolean (APIENTRY * PFNEGLMAKECURRENT) (EGLDisplay, EGLSurface, EGLSurface, EGLContext);
typedef EGLSurface (APIENTRY * PFNEGLCREATEPBUFFERSURFACE) (EGLDisplay, EGLConfig, const EGLint*);
typedef EGLBoolean (APIENTRY * PFNEGLGETCONFIGATTRIB) (EGLDisplay, EGLConfig, EGLint, EGLint*);
typedef EGLBoolean (APIENTRY * PFNEGLGETSWAPBUFFERS) (EGLDisplay, EGLSurface);
typedef EGLContext (APIENTRY * PFNEGLGETCURRENTCONTEXT) (void);
typedef EGLBoolean (APIENTRY * PFNEGLDESTROYSURFACE) (EGLDisplay, EGLSurface);
typedef EGLSurface (APIENTRY * PFNEGLCREATEWINDOWSURFACE) (EGLDisplay, EGLConfig, EGLNativeWindowType, const EGLint *);
typedef EGLBoolean (APIENTRY * PFNEGLQUERYSURFACE) (EGLDisplay, EGLSurface, EGLint, EGLint *);
typedef EGLBoolean (APIENTRY * PFNEGLCOPYBUFFERS) (EGLDisplay dpy, EGLSurface surface, EGLNativePixmapType target);
typedef EGLSurface (APIENTRY * PFNEGLCREATEPIXMAPSURFACE) (EGLDisplay dpy, EGLConfig config, EGLNativePixmapType pixmap, const EGLint *attrib_list);
typedef EGLBoolean (APIENTRY * PFNEGLDESTROYCONTEXT) (EGLDisplay dpy, EGLContext context);
typedef EGLBoolean (APIENTRY * PFNEGLGETCONFIGS) (EGLDisplay dpy, EGLConfig *configs, EGLint config_size, EGLint *num_config);
typedef EGLDisplay (APIENTRY * PFNEGLGETCURRENTDISPLAY) (void);
typedef EGLSurface (APIENTRY * PFNEGLGETCURRENTSURFACE) (EGLint readdraw);
typedef EGLDisplay (APIENTRY * PFNEGLGETDISPLAY) (EGLNativeDisplayType display_id);
typedef EGLBoolean (APIENTRY * PFNEGLSETFIFO) (void*);
typedef EGLBoolean (APIENTRY * PFNEGLQUERYCONTEXT) (EGLDisplay dpy, EGLContext ctx, EGLint attribute, EGLint *value);
typedef const char *(APIENTRY * PFNEGLQUERYSTRING) (EGLDisplay dpy, EGLint name);
typedef EGLBoolean (APIENTRY * PFNEGLQUERYSURFACE) (EGLDisplay dpy, EGLSurface surface, EGLint attribute, EGLint *value);
typedef EGLBoolean (APIENTRY * PFNEGLSWAPBUFFERS) (EGLDisplay dpy, EGLSurface surface);
typedef EGLBoolean (APIENTRY * PFNEGLTERMINATE) (EGLDisplay dpy);
typedef EGLBoolean (APIENTRY * PFNEGLWAITGL) (void);
typedef EGLBoolean (APIENTRY * PFNEGLWAITNATIVE) (EGLint engine);
typedef EGLBoolean (APIENTRY * PFNEGLBINDTEXIMAGE) (EGLDisplay dpy, EGLSurface surface, EGLint buffer);
typedef EGLBoolean (APIENTRY * PFNEGLRELEASETEXIMAGE) (EGLDisplay dpy, EGLSurface surface, EGLint buffer);
typedef EGLBoolean (APIENTRY * PFNEGLSURFACEATTRIB) (EGLDisplay dpy, EGLSurface surface, EGLint attribute, EGLint value);
typedef EGLBoolean (APIENTRY * PFNEGLSWAPINTERVAL) (EGLDisplay dpy, EGLint interval);
typedef EGLBoolean (APIENTRY * PFNEGLBINDAPI) (EGLenum api);
typedef EGLenum (APIENTRY * PFNEGLQUERYAPI) (void);
typedef EGLSurface (APIENTRY * PFNEGLCREATEPBUFFERFROMCLIENTBUFFER) (EGLDisplay dpy, EGLenum buftype, EGLClientBuffer buffer, EGLConfig config, const EGLint *attrib_list);
typedef EGLBoolean (APIENTRY * PFNEGLRELEASETHREAD) (void);
typedef EGLBoolean (APIENTRY * PFNEGLWAITCLIENT) (void);
typedef EGLContext (APIENTRY * PFNEGLGETCURRENTCONTEXT) (void);

typedef void (APIENTRY * PFNGLALPHAFUNC) (GLenum func, GLclampf ref);
typedef void (APIENTRY * PFNGLCLEARCOLOR) (GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha);
typedef void (APIENTRY * PFNGLCLEARDEPTH) (GLclampf depth);
typedef void (APIENTRY * PFNGLCLIPPLANE) (GLenum plane, const GLfloat *equation);
typedef void (APIENTRY * PFNGLCOLOR4F) (GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
typedef void (APIENTRY * PFNGLDEPTHRANGE) (GLclampf zNear, GLclampf zFar);
typedef void (APIENTRY * PFNGLFOGF) (GLenum pname, GLfloat param);
typedef void (APIENTRY * PFNGLFOGFV) (GLenum pname, const GLfloat *params);
typedef void (APIENTRY * PFNGLFRUSTUM) (GLfloat left, GLfloat right, GLfloat bottom, GLfloat top, GLfloat zNear, GLfloat zFar);
typedef void (APIENTRY * PFNGLGETCLIPPLANE) (GLenum pname, GLfloat eqn[4]);
typedef void (APIENTRY * PFNGLGETFLOATV) (GLenum pname, GLfloat *params);
typedef void (APIENTRY * PFNGLGETLIGHTFV) (GLenum light, GLenum pname, GLfloat *params);
typedef void (APIENTRY * PFNGLGETMATERIALFV) (GLenum face, GLenum pname, GLfloat *params);
typedef void (APIENTRY * PFNGLGETTEXENVFV) (GLenum env, GLenum pname, GLfloat *params);
typedef void (APIENTRY * PFNGLGETTEXPARAMETERFV) (GLenum target, GLenum pname, GLfloat *params);
typedef void (APIENTRY * PFNGLLIGHTMODELF) (GLenum pname, GLfloat param);
typedef void (APIENTRY * PFNGLLIGHTMODELFV) (GLenum pname, const GLfloat *params);
typedef void (APIENTRY * PFNGLLIGHTF) (GLenum light, GLenum pname, GLfloat param);
typedef void (APIENTRY * PFNGLLIGHTFV) (GLenum light, GLenum pname, const GLfloat *params);
typedef void (APIENTRY * PFNGLLINEWIDTH) (GLfloat width);
typedef void (APIENTRY * PFNGLLOADMATRIXF) (const GLfloat *m);
typedef void (APIENTRY * PFNGLMATERIALF) (GLenum face, GLenum pname, GLfloat param);
typedef void (APIENTRY * PFNGLMATERIALFV) (GLenum face, GLenum pname, const GLfloat *params);
typedef void (APIENTRY * PFNGLMULTMATRIXF) (const GLfloat *m);
typedef void (APIENTRY * PFNGLMULTITEXCOORD4F) (GLenum target, GLfloat s, GLfloat t, GLfloat r, GLfloat q);
typedef void (APIENTRY * PFNGLMULTITEXCOORD2F) (GLenum target, GLfloat s, GLfloat t);
typedef void (APIENTRY * PFNGLNORMAL3F) (GLfloat nx, GLfloat ny, GLfloat nz);
typedef void (APIENTRY * PFNGLORTHO) (GLfloat left, GLfloat right, GLfloat bottom, GLfloat top, GLfloat zNear, GLfloat zFar);
typedef void (APIENTRY * PFNGLPOINTPARAMETERF) (GLenum pname, GLfloat param);
typedef void (APIENTRY * PFNGLPOINTPARAMETERFV) (GLenum pname, const GLfloat *params);
typedef void (APIENTRY * PFNGLPOINTSIZE) (GLfloat size);
typedef void (APIENTRY * PFNGLPOLYGONOFFSET) (GLfloat factor, GLfloat units);
typedef void (APIENTRY * PFNGLROTATEF) (GLfloat angle, GLfloat x, GLfloat y, GLfloat z);
typedef void (APIENTRY * PFNGLSCALEF) (GLfloat x, GLfloat y, GLfloat z);
typedef void (APIENTRY * PFNGLTEXENVF) (GLenum target, GLenum pname, GLfloat param);
typedef void (APIENTRY * PFNGLTEXENVFV) (GLenum target, GLenum pname, const GLfloat *params);
typedef void (APIENTRY * PFNGLTEXPARAMETERF) (GLenum target, GLenum pname, GLfloat param);
typedef void (APIENTRY * PFNGLTEXPARAMETERFV) (GLenum target, GLenum pname, const GLfloat *params);
typedef void (APIENTRY * PFNGLTRANSLATEF) (GLfloat x, GLfloat y, GLfloat z);
typedef void (APIENTRY * PFNGLACTIVETEXTURE) (GLenum texture);
//typedef void (APIENTRY * PFNGLALPHAFUNCX) (GLenum func, GLclampx ref);
typedef void (APIENTRY * PFNGLBINDBUFFER) (GLenum target, GLuint buffer);
typedef void (APIENTRY * PFNGLBINDTEXTURE) (GLenum target, GLuint texture);
typedef void (APIENTRY * PFNGLBLENDFUNC) (GLenum sfactor, GLenum dfactor);
typedef void (APIENTRY * PFNGLBUFFERDATA) (GLenum target, int size, const GLvoid *data, GLenum usage);
typedef void (APIENTRY * PFNGLBUFFERSUBDATA) (GLenum target, int offset, int size, const GLvoid *data);
typedef void (APIENTRY * PFNGLCLEAR) (GLbitfield mask);
//typedef void (APIENTRY * PFNGLCLEARCOLORX) (GLclampx red, GLclampx green, GLclampx blue, GLclampx alpha);
//typedef void (APIENTRY * PFNGLCLEARDEPTHX) (GLclampx depth);
typedef void (APIENTRY * PFNGLCLEARSTENCIL) (GLint s);
typedef void (APIENTRY * PFNGLCLIENTACTIVETEXTURE) (GLenum texture);
//typedef void (APIENTRY * PFNGLCLIPPLANEX) (GLenum plane, const GLfixed *equation);
typedef void (APIENTRY * PFNGLCOLOR4UB) (GLubyte red, GLubyte green, GLubyte blue, GLubyte alpha);
//typedef void (APIENTRY * PFNGLCOLOR4X) (GLfixed red, GLfixed green, GLfixed blue, GLfixed alpha);
typedef void (APIENTRY * PFNGLCOLORMASK) (GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha);
typedef void (APIENTRY * PFNGLCOLORPOINTER) (GLint size, GLenum type, GLsizei stride, const GLvoid *pointer);
typedef void (APIENTRY * PFNGLCOMPRESSEDTEXIMAGE2D) (GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLsizei imageSize, const GLvoid *data);
typedef void (APIENTRY * PFNGLCOMPRESSEDTEXIMAGE3D) (GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLsizei imageSize, const GLvoid* data);
typedef void (APIENTRY * PFNGLCOMPRESSEDTEXSUBIMAGE2D) (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLsizei imageSize, const GLvoid *data);
typedef void (APIENTRY * PFNGLCOMPRESSEDTEXSUBIMAGE3D) (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLsizei imageSize, const GLvoid* data);
typedef void (APIENTRY * PFNGLCOPYTEXIMAGE2D) (GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLsizei height, GLint border);
typedef void (APIENTRY * PFNGLCOPYTEXSUBIMAGE2D) (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height);
typedef void (APIENTRY * PFNGLCOPYTEXSUBIMAGE3D) (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLint x, GLint y, GLsizei width, GLsizei height);
typedef void (APIENTRY * PFNGLCULLFACE) (GLenum mode);
typedef void (APIENTRY * PFNGLDELETEBUFFERS) (GLsizei n, const GLuint *buffers);
typedef void (APIENTRY * PFNGLDELETETEXTURES) (GLsizei n, const GLuint *textures);
typedef void (APIENTRY * PFNGLDEPTHFUNC) (GLenum func);
typedef void (APIENTRY * PFNGLDEPTHMASK) (GLboolean flag);
//typedef void (APIENTRY * PFNGLDEPTHRANGEX) (GLclampx zNear, GLclampx zFar);
typedef void (APIENTRY * PFNGLDISABLE) (GLenum cap);
typedef void (APIENTRY * PFNGLDISABLECLIENTSTATE) (GLenum array);
typedef void (APIENTRY * PFNGLDRAWARRAYS) (GLenum mode, GLint first, GLsizei count);
typedef void (APIENTRY * PFNGLDRAWELEMENTS) (GLenum mode, GLsizei count, GLenum type, const GLvoid *indices);
typedef void (APIENTRY * PFNGLENABLE) (GLenum cap);
typedef void (APIENTRY * PFNGLENABLECLIENTSTATE) (GLenum array);
typedef void (APIENTRY * PFNGLFINISH) (void);
typedef void (APIENTRY * PFNGLFLUSH) (void);
typedef void (APIENTRY * PFNGLFOGI) (GLenum pname, GLfixed param);
//typedef void (APIENTRY * PFNGLFOGIV) (GLenum pname, const GLfixed *params);
typedef void (APIENTRY * PFNGLFRONTFACE) (GLenum mode);
//typedef void (APIENTRY * PFNGLFRUSTUMX) (GLfixed left, GLfixed right, GLfixed bottom, GLfixed top, GLfixed zNear, GLfixed zFar);
typedef void (APIENTRY * PFNGLGETBOOLEANV) (GLenum pname, GLboolean *params);
typedef void (APIENTRY * PFNGLGETBUFFERPARAMETERIV) (GLenum target, GLenum pname, GLint *params);
//typedef void (APIENTRY * PFNGLGETCLIPPLANEX) (GLenum pname, GLfixed eqn[4]);
typedef void (APIENTRY * PFNGLGENBUFFERS) (GLsizei n, GLuint *buffers);
typedef void (APIENTRY * PFNGLGENTEXTURES) (GLsizei n, GLuint *textures);
typedef GLenum (APIENTRY * PFNGLGETERROR) (void);
//typedef void (APIENTRY * PFNGLGETFIXEDV) (GLenum pname, GLfixed *params);
typedef void (APIENTRY * PFNGLGETINTEGERV) (GLenum pname, GLint *params);
//typedef void (APIENTRY * PFNGLGETLIGHTXV) (GLenum light, GLenum pname, GLfixed *params);
//typedef void (APIENTRY * PFNGLGETMATERIALXV) (GLenum face, GLenum pname, GLfixed *params);
typedef void (APIENTRY * PFNGLGETPOINTERV) (GLenum pname, void **params);
typedef const GLubyte * (APIENTRY * PFNGLGETSTRING) (GLenum name);
typedef void (APIENTRY * PFNGLGETTEXENVIV) (GLenum env, GLenum pname, GLint *params);
typedef void (APIENTRY * PFNGLGETTEXGENFV)(GLenum coord, GLenum pname, GLfloat *params);
typedef void (APIENTRY * PFNGLGETTEXGENIV)(GLenum coord, GLenum pname, GLint *params);
//typedef void (APIENTRY * PFNGLGETTEXENVXV) (GLenum env, GLenum pname, GLfixed *params);
typedef void (APIENTRY * PFNGLGETTEXPARAMETERIV) (GLenum target, GLenum pname, GLint *params);
//typedef void (APIENTRY * PFNGLGETTEXPARAMETERXV) (GLenum target, GLenum pname, GLfixed *params);
typedef void (APIENTRY * PFNGLHINT) (GLenum target, GLenum mode);
typedef GLboolean (APIENTRY * PFNGLISBUFFER) (GLuint buffer);
typedef GLboolean (APIENTRY * PFNGLISENABLED) (GLenum cap);
typedef GLboolean (APIENTRY * PFNGLISTEXTURE) (GLuint texture);
//typedef void (APIENTRY * PFNGLLIGHTMODELX) (GLenum pname, GLfixed param);
//typedef void (APIENTRY * PFNGLLIGHTMODELXV) (GLenum pname, const GLfixed *params);
//typedef void (APIENTRY * PFNGLLIGHTX) (GLenum light, GLenum pname, GLfixed param);
//typedef void (APIENTRY * PFNGLLIGHTXV) (GLenum light, GLenum pname, const GLfixed *params);
//typedef void (APIENTRY * PFNGLLINEWIDTHX) (GLfixed width);
typedef void (APIENTRY * PFNGLLOADIDENTITY) (void);
//typedef void (APIENTRY * PFNGLLOADMATRIXX) (const GLfixed *m);
typedef void (APIENTRY * PFNGLLOGICOP) (GLenum opcode);
//typedef void (APIENTRY * PFNGLMATERIALX) (GLenum face, GLenum pname, GLfixed param);
//typedef void (APIENTRY * PFNGLMATERIALXV) (GLenum face, GLenum pname, const GLfixed *params);
typedef void (APIENTRY * PFNGLMATRIXMODE) (GLenum mode);
//typedef void (APIENTRY * PFNGLMULTMATRIXX) (const GLfixed *m);
//typedef void (APIENTRY * PFNGLMULTITEXCOORD4X) (GLenum target, GLfixed s, GLfixed t, GLfixed r, GLfixed q);
//typedef void (APIENTRY * PFNGLNORMAL3X) (GLfixed nx, GLfixed ny, GLfixed nz);
typedef void (APIENTRY * PFNGLNORMALPOINTER) (GLenum type, GLsizei stride, const GLvoid *pointer);
//typedef void (APIENTRY * PFNGLORTHOX) (GLfixed left, GLfixed right, GLfixed bottom, GLfixed top, GLfixed zNear, GLfixed zFar);
typedef void (APIENTRY * PFNGLPIXELSTOREI) (GLenum pname, GLint param);
typedef void (APIENTRY * PFNGLPOINTPARAMETERI) (GLenum pname, GLfixed param);
typedef void (APIENTRY * PFNGLPOINTPARAMETERIV) (GLenum pname, const GLfixed *params);
//typedef void (APIENTRY * PFNGLPOINTSIZEI) (GLfixed size);
//typedef void (APIENTRY * PFNGLPOLYGONOFFSETI) (GLfixed factor, GLfixed units);
typedef void (APIENTRY * PFNGLPOPMATRIX) (void);
typedef void (APIENTRY * PFNGLPUSHMATRIX) (void);
typedef void (APIENTRY * PFNGLREADPIXELS) (GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, GLvoid *pixels);
typedef void (APIENTRY * PFNGLROTATEI) (GLfixed angle, GLfixed x, GLfixed y, GLfixed z);
typedef void (APIENTRY * PFNGLSAMPLECOVERAGE) (GLclampf value, GLboolean invert);
//typedef void (APIENTRY * PFNGLSAMPLECOVERAGEX) (GLclampx value, GLboolean invert);
//typedef void (APIENTRY * PFNGLSCALEX) (GLfixed x, GLfixed y, GLfixed z);
typedef void (APIENTRY * PFNGLSCISSOR) (GLint x, GLint y, GLsizei width, GLsizei height);
typedef void (APIENTRY * PFNGLSHADEMODEL) (GLenum mode);
typedef void (APIENTRY * PFNGLSTENCILFUNC) (GLenum func, GLint ref, GLuint mask);
typedef void (APIENTRY * PFNGLSTENCILMASK) (GLuint mask);
typedef void (APIENTRY * PFNGLSTENCILOP) (GLenum fail, GLenum zfail, GLenum zpass);
typedef void (APIENTRY * PFNGLTEXCOORDPOINTER) (GLint size, GLenum type, GLsizei stride, const GLvoid *pointer);
typedef void (APIENTRY * PFNGLTEXENVI) (GLenum target, GLenum pname, GLint param);
typedef void (APIENTRY * PFNGLTEXENVIV) (GLenum target, GLenum pname, const GLint *params);
typedef void (APIENTRY * PFNGLTEXGENF) (GLenum coord, GLenum pname, GLfloat param);
typedef void (APIENTRY * PFNGLTEXGENFV) (GLenum coord, GLenum pname, const GLfloat *param);
typedef void (APIENTRY * PFNGLTEXGENI) (GLenum coord, GLenum pname, GLint param);
typedef void (APIENTRY * PFNGLTEXGENIV) (GLenum coord, GLenum pname, const GLint *param);
typedef void (APIENTRY * PFNGLTEXIMAGE2D) (GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid *pixels);
typedef void (APIENTRY * PFNGLTEXIMAGE3D) (GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLenum format, GLenum type, const GLvoid* pixels);
typedef void (APIENTRY * PFNGLTEXPARAMETERI) (GLenum target, GLenum pname, GLint param);
typedef void (APIENTRY * PFNGLTEXPARAMETERIV) (GLenum target, GLenum pname, const GLint *params);
typedef void (APIENTRY * PFNGLTEXSUBIMAGE2D) (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid *pixels);
typedef void (APIENTRY * PFNGLTEXSUBIMAGE3D) (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const GLvoid* pixels);
//typedef void (APIENTRY * PFNGLTRANSLATEX) (GLfixed x, GLfixed y, GLfixed z);
typedef void (APIENTRY * PFNGLVERTEXPOINTER) (GLint size, GLenum type, GLsizei stride, const GLvoid *pointer);
typedef void (APIENTRY * PFNGLVIEWPORT) (GLint x, GLint y, GLsizei width, GLsizei height);

typedef void (APIENTRY * PFNGLPOINTSIZEPOINTEROES) (GLenum type, GLsizei stride, const GLvoid *pointer);

typedef void (APIENTRY * PFNGLCURRENTPALETTEMATRIXARB) (GLuint matrixpaletteindex);
typedef void (APIENTRY * PFNGLLOADPALETTEFROMMODELVIEWMATRIXARB) (void);
typedef void (APIENTRY * PFNGLMATRIXINDEXPOINTERARB) (GLint size, GLenum type, GLsizei stride, const GLvoid *pointer);
typedef void (APIENTRY * PFNGLWEIGHTPOINTERARB) (GLint size, GLenum type, GLsizei stride, const GLvoid *pointer);
//typedef void (APIENTRY * PFNGLMULTIDRAWARRAYSEXTPROC) (GLenum mode, GLint *first, GLsizei *count, GLsizei primcount);
//typedef void (APIENTRY * PFNGLMULTIDRAWELEMENTSEXTPROC) (GLenum mode, const GLsizei *count, GLenum type, const GLvoid* *indices, GLsizei primcount);

#ifndef GL_OES_mapbuffer
typedef GLvoid* (APIENTRY * PFNGLMAPBUFFEROESPROC)(GLenum target, GLenum access);
typedef GLboolean (APIENTRY * PFNGLUNMAPBUFFEROESPROC)(GLenum target);
typedef void (APIENTRY * PFNGLGETBUFFERPOINTERVOESPROC) (GLenum target, GLenum pname, GLvoid** params);
#endif

typedef void (APIENTRY * PFNGLATTACHSHADER) (GLuint program, GLuint shader);
typedef void (APIENTRY * PFNGLBINDATTRIBLOCATION) (GLuint program, GLuint index, const char* name);
typedef void (APIENTRY * PFNGLBINDFRAMEBUFFER) (GLenum target, GLuint framebuffer);
typedef void (APIENTRY * PFNGLBINDRENDERBUFFER) (GLenum target, GLuint renderbuffer);
typedef void (APIENTRY * PFNGLBLENDCOLOR) (GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha);
typedef void (APIENTRY * PFNGLBLENDEQUATION) ( GLenum mode );
typedef void (APIENTRY * PFNGLBLENDEQUATIONSEPARATE) (GLenum modeRGB, GLenum modeAlpha);
typedef void (APIENTRY * PFNGLBLENDFUNCSEPARATE) (GLenum srcRGB, GLenum dstRGB, GLenum srcAlpha, GLenum dstAlpha);
typedef GLenum (APIENTRY * PFNGLCHECKFRAMEBUFFERSTATUS) (GLenum target);
typedef void (APIENTRY * PFNCLEARDEPTHF) (GLclampf depth);
typedef void (APIENTRY * PFNGLCOMPILESHADER) (GLuint shader);
typedef GLuint (APIENTRY * PFNGLCREATEPROGRAM) (void);
typedef GLuint (APIENTRY * PFNGLCREATESHADER) (GLenum type);
typedef void (APIENTRY * PFNGLDELETEFRAMEBUFFERS) (GLsizei n, const GLuint* framebuffers);
typedef void (APIENTRY * PFNGLDELETEPROGRAM) (GLuint program);
typedef void (APIENTRY * PFNGLDELETERENDERBUFFERS) (GLsizei n, const GLuint* renderbuffers);
typedef void (APIENTRY * PFNGLDELETESHADER) (GLuint shader);
typedef void (APIENTRY * PFNGLDETACHSHADER) (GLuint program, GLuint shader);
typedef void (APIENTRY * PFNGLDISABLEVERTEXATTRIBARRAY) (GLuint index);
typedef void (APIENTRY * PFNGLENABLEVERTEXATTRIBARRAY) (GLuint index);
typedef void (APIENTRY * PFNGLFRAMEBUFFERRENDERBUFFER) (GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer);
typedef void (APIENTRY * PFNGLFRAMEBUFFERTEXTURE2D) (GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level);
typedef void (APIENTRY * PFNGLFRAMEBUFFERTEXTURE3D) (GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level, GLint zoffset);
typedef void (APIENTRY * PFNGLGENERATEMIPMAP) (GLenum target);
typedef void (APIENTRY * PFNGLGENFRAMEBUFFERS) (GLsizei n, GLuint* framebuffers);
typedef void (APIENTRY * PFNGLGENRENDERBUFFERS) (GLsizei n, GLuint* renderbuffers);
typedef void (APIENTRY * PFNGLGETACTIVEATTRIB) (GLuint program, GLuint index, GLsizei bufsize, GLsizei* length, GLint* size, GLenum* type, char* name);
typedef void (APIENTRY * PFNGLGETACTIVEUNIFORM) (GLuint program, GLuint index, GLsizei bufsize, GLsizei* length, GLint* size, GLenum* type, char* name);
typedef void (APIENTRY * PFNGLGETATTACHEDSHADERS) (GLuint program, GLsizei maxcount, GLsizei* count, GLuint* shaders);
typedef int  (APIENTRY * PFNGLGETATTRIBLOCATION) (GLuint program, const char* name);
typedef void (APIENTRY * PFNGLGETFRAMEBUFFERATTACHMENTPARAMETERIV) (GLenum target, GLenum attachment, GLenum pname, GLint* params);
typedef void (APIENTRY * PFNGLGETPROGRAMIV) (GLuint program, GLenum pname, GLint* params);
typedef void (APIENTRY * PFNGLGETPROGRAMINFOLOG) (GLuint program, GLsizei bufsize, GLsizei* length, char* infolog);
typedef void (APIENTRY * PFNGLGETPROGRAMBINARY) (GLuint program, GLsizei bufSize, GLsizei *length, GLenum *binaryFormat, GLvoid *binary);
typedef void (APIENTRY * PFNGLGETRENDERBUFFERPARAMETERIV) (GLenum target, GLenum pname, GLint* params);
typedef void (APIENTRY * PFNGLGETSHADERIV) (GLuint shader, GLenum pname, GLint* params);
typedef void (APIENTRY * PFNGLGETSHADERINFOLOG) (GLuint shader, GLsizei bufsize, GLsizei* length, char* infolog);
typedef void (APIENTRY * PFNGLGETSHADERPRECISIONFORMAT) (GLenum shadertype, GLenum precisiontype, GLint* range, GLint* precision);
typedef void (APIENTRY * PFNGLGETSHADERSOURCE) (GLuint shader, GLsizei bufsize, GLsizei* length, char* source);
typedef void (APIENTRY * PFNGLGETUNIFORMFV) (GLuint program, GLint location, GLfloat* params);
typedef void (APIENTRY * PFNGLGETUNIFORMIV) (GLuint program, GLint location, GLint* params);
typedef int  (APIENTRY * PFNGLGETUNIFORMLOCATION) (GLuint program, const char* name);
typedef void (APIENTRY * PFNGLGETVERTEXATTRIBFV) (GLuint index, GLenum pname, GLfloat* params);
typedef void (APIENTRY * PFNGLGETVERTEXATTRIBIV) (GLuint index, GLenum pname, GLint* params);
typedef void (APIENTRY * PFNGLGETVERTEXATTRIBPOINTERV) (GLuint index, GLenum pname, void** pointer);
typedef GLboolean (APIENTRY * PFNGLISFRAMEBUFFER) (GLuint framebuffer);
typedef GLboolean (APIENTRY * PFNGLISPROGRAM) (GLuint program);
typedef GLboolean (APIENTRY * PFNGLISRENDERBUFFER) (GLuint renderbuffer);
typedef GLboolean (APIENTRY * PFNGLISSHADER) (GLuint shader);
typedef void (APIENTRY * PFNGLLINKPROGRAM) (GLuint program);
typedef void (APIENTRY * PFNGLPROGRAMBINARY) (GLuint program, GLenum binaryFormat, const GLvoid *binary, GLint length);
typedef void (APIENTRY * PFNGLRELEASESHADERCOMPILER) (void);
typedef void (APIENTRY * PFNGLRENDERBUFFERSTORAGE) (GLenum target, GLenum internalformat, GLsizei width, GLsizei height);
typedef void (APIENTRY * PFNGLSHADERBINARY) (GLint n, const GLuint* shaders, GLenum binaryformat, const void* binary, GLint length);
#ifndef EGL_OPENGL_ES3_BIT_KHR
typedef void (APIENTRY * PFNGLSHADERSOURCE) (GLuint shader, GLsizei count, const GLchar** string, const GLint* length);
#else
typedef void (APIENTRY * PFNGLSHADERSOURCE) (GLuint shader, GLsizei count, const GLchar* const* string, const GLint* length);
#endif
typedef void (APIENTRY * PFNGLSTENCILFUNCSEPARATE) (GLenum face, GLenum func, GLint ref, GLuint mask);
typedef void (APIENTRY * PFNGLSTENCILMASKSEPARATE) (GLenum face, GLuint mask);
typedef void (APIENTRY * PFNGLSTENCILOPSEPARATE) (GLenum face, GLenum fail, GLenum zfail, GLenum zpass);
typedef void (APIENTRY * PFNGLUNIFORM1F) (GLint location, GLfloat x);
typedef void (APIENTRY * PFNGLUNIFORM1FV) (GLint location, GLsizei count, const GLfloat* v);
typedef void (APIENTRY * PFNGLUNIFORM1I) (GLint location, GLint x);
typedef void (APIENTRY * PFNGLUNIFORM1IV) (GLint location, GLsizei count, const GLint* v);
typedef void (APIENTRY * PFNGLUNIFORM2F) (GLint location, GLfloat x, GLfloat y);
typedef void (APIENTRY * PFNGLUNIFORM2FV) (GLint location, GLsizei count, const GLfloat* v);
typedef void (APIENTRY * PFNGLUNIFORM2I) (GLint location, GLint x, GLint y);
typedef void (APIENTRY * PFNGLUNIFORM2IV) (GLint location, GLsizei count, const GLint* v);
typedef void (APIENTRY * PFNGLUNIFORM3F) (GLint location, GLfloat x, GLfloat y, GLfloat z);
typedef void (APIENTRY * PFNGLUNIFORM3FV) (GLint location, GLsizei count, const GLfloat* v);
typedef void (APIENTRY * PFNGLUNIFORM3I) (GLint location, GLint x, GLint y, GLint z);
typedef void (APIENTRY * PFNGLUNIFORM3IV) (GLint location, GLsizei count, const GLint* v);
typedef void (APIENTRY * PFNGLUNIFORM4F) (GLint location, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
typedef void (APIENTRY * PFNGLUNIFORM4FV) (GLint location, GLsizei count, const GLfloat* v);
typedef void (APIENTRY * PFNGLUNIFORM4I) (GLint location, GLint x, GLint y, GLint z, GLint w);
typedef void (APIENTRY * PFNGLUNIFORM4IV) (GLint location, GLsizei count, const GLint* v);
typedef void (APIENTRY * PFNGLUNIFORMMATRIX2FV) (GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
typedef void (APIENTRY * PFNGLUNIFORMMATRIX3FV) (GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
typedef void (APIENTRY * PFNGLUNIFORMMATRIX4FV) (GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
typedef void (APIENTRY * PFNGLUSEPROGRAM) (GLuint program);
typedef void (APIENTRY * PFNGLVALIDATEPROGRAM) (GLuint program);
typedef void (APIENTRY * PFNGLVERTEXATTRIB1F) (GLuint indx, GLfloat x);
typedef void (APIENTRY * PFNGLVERTEXATTRIB1FV) (GLuint indx, const GLfloat* values);
typedef void (APIENTRY * PFNGLVERTEXATTRIB2F) (GLuint indx, GLfloat x, GLfloat y);
typedef void (APIENTRY * PFNGLVERTEXATTRIB2FV) (GLuint indx, const GLfloat* values);
typedef void (APIENTRY * PFNGLVERTEXATTRIB3F) (GLuint indx, GLfloat x, GLfloat y, GLfloat z);
typedef void (APIENTRY * PFNGLVERTEXATTRIB3FV) (GLuint indx, const GLfloat* values);
typedef void (APIENTRY * PFNGLVERTEXATTRIB4F) (GLuint indx, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
typedef void (APIENTRY * PFNGLVERTEXATTRIB4FV) (GLuint indx, const GLfloat* values);
typedef void (APIENTRY * PFNGLVERTEXATTRIBPOINTER) (GLuint indx, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void* ptr);
typedef void (APIENTRY * PFNGLBINDVERTEXARRAY) (GLuint array);
typedef void (APIENTRY * PFNGLDELETEVERTEXARRAYS) (GLsizei n, const GLuint *arrays);
typedef void (APIENTRY * PFNGLGENVERTEXARRAYS) (GLsizei n, GLuint *arrays);
typedef GLboolean (APIENTRY * PFNGLISVERTEXARRAY) (GLuint array);
typedef void (APIENTRY * PFNGLBLITFRAMEBUFFEREXT) (GLint srcX0, GLint srcY0, GLint srcX1, GLint srcY1, GLint dstX0, GLint dstY0, GLint dstX1, GLint dstY1, GLbitfield mask, GLenum filter);
typedef void (APIENTRY * PFNGLRENDERBUFFERSTORAGEMULTISAMPLEEXT) (GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height);


typedef void (APIENTRY * PFNGLLOADMATRIXD) (const GLfloat *m);
typedef void (APIENTRY * PFNGLBEGIN)(GLenum mode);
typedef void (APIENTRY * PFNGLEND)(void);
typedef void (APIENTRY * PFNGLVERTEX3F)(GLfloat x, GLfloat y, GLfloat z);
typedef void (APIENTRY * PFNGLTEXCOORD2F)(GLfloat s, GLfloat t);
typedef void (APIENTRY * PFNGLDRAWBUFFER)(GLenum mode);

typedef GLsync (APIENTRY * PFNFENCESYNC)(GLenum condition, GLbitfield flags);
typedef void (APIENTRY * PFNDELETESYNC)(GLsync sync);
typedef GLenum (APIENTRY * PFNCLIENTWAITSYNC)(GLsync sync, GLbitfield flags, GLuint64 timeout);
typedef void (APIENTRY * PFNGETSYNCIV)(GLsync sync, GLenum pname, GLsizei bufSize, GLsizei *length, GLint *value);

typedef void (APIENTRY * PFNGLREADBUFFER) (GLenum mode);
typedef void (APIENTRY * PFNGLDRAWRANGEELEMENTS) (GLenum mode, GLuint start, GLuint end, GLsizei count, GLenum type, const GLvoid* indices);
typedef void (APIENTRY * PFNGLGENQUERIES) (GLsizei n, GLuint* ids);
typedef void (APIENTRY * PFNGLDELETEQUERIES) (GLsizei n, const GLuint* ids);
typedef GLboolean (APIENTRY * PFNGLISQUERY) (GLuint id);
typedef void (APIENTRY * PFNGLBEGINQUERY) (GLenum target, GLuint id);
typedef void (APIENTRY * PFNGLENDQUERY) (GLenum target);
typedef void (APIENTRY * PFNGLGETQUERYIV) (GLenum target, GLenum pname, GLint* params);
typedef void (APIENTRY * PFNGLGETQUERYOBJECTUIV) (GLuint id, GLenum pname, GLuint* params);
typedef GLboolean (APIENTRY * PFNGLUNMAPBUFFER) (GLenum target);
typedef void (APIENTRY * PFNGLGETBUFFERPOINTERV) (GLenum target, GLenum pname, GLvoid** params);
typedef void (APIENTRY * PFNGLDRAWBUFFERS) (GLsizei n, const GLenum* bufs);
typedef void (APIENTRY * PFNGLUNIFORMMATRIX2X3FV) (GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
typedef void (APIENTRY * PFNGLUNIFORMMATRIX3X2FV) (GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
typedef void (APIENTRY * PFNGLUNIFORMMATRIX2X4FV) (GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
typedef void (APIENTRY * PFNGLUNIFORMMATRIX4X2FV) (GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
typedef void (APIENTRY * PFNGLUNIFORMMATRIX3X4FV) (GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
typedef void (APIENTRY * PFNGLUNIFORMMATRIX4X3FV) (GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
typedef void (APIENTRY * PFNGLBLITFRAMEBUFFER) (GLint srcX0, GLint srcY0, GLint srcX1, GLint srcY1, GLint dstX0, GLint dstY0, GLint dstX1, GLint dstY1, GLbitfield mask, GLenum filter);
typedef void (APIENTRY * PFNGLRENDERBUFFERSTORAGEMULTISAMPLE) (GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height);
typedef void (APIENTRY * PFNGLFRAMEBUFFERTEXTURELAYER) (GLenum target, GLenum attachment, GLuint texture, GLint level, GLint layer);
typedef GLvoid* (APIENTRY * PFNGLMAPBUFFERRANGE) (GLenum target, GLintptr offset, GLsizeiptr length, GLbitfield access);
typedef void (APIENTRY * PFNGLFLUSHMAPPEDBUFFERRANGE) (GLenum target, GLintptr offset, GLsizeiptr length);
typedef void (APIENTRY * PFNGLGETINTEGERI_V) (GLenum target, GLuint index, GLint* data);
typedef void (APIENTRY * PFNGLBEGINTRANSFORMFEEDBACK) (GLenum primitiveMode);
typedef void (APIENTRY * PFNGLENDTRANSFORMFEEDBACK) (void);
typedef void (APIENTRY * PFNGLBINDBUFFERRANGE) (GLenum target, GLuint index, GLuint buffer, GLintptr offset, GLsizeiptr size);
typedef void (APIENTRY * PFNGLBINDBUFFERBASE) (GLenum target, GLuint index, GLuint buffer);
typedef void (APIENTRY * PFNGLTRANSFORMFEEDBACKVARYINGS) (GLuint program, GLsizei count, const GLchar* const* varyings, GLenum bufferMode);
typedef void (APIENTRY * PFNGLGETTRANSFORMFEEDBACKVARYING) (GLuint program, GLuint index, GLsizei bufSize, GLsizei* length, GLsizei* size, GLenum* type, GLchar* name);
typedef void (APIENTRY * PFNGLVERTEXATTRIBIPOINTER) (GLuint index, GLint size, GLenum type, GLsizei stride, const GLvoid* pointer);
typedef void (APIENTRY * PFNGLGETVERTEXATTRIBIIV) (GLuint index, GLenum pname, GLint* params);
typedef void (APIENTRY * PFNGLGETVERTEXATTRIBIUIV) (GLuint index, GLenum pname, GLuint* params);
typedef void (APIENTRY * PFNGLVERTEXATTRIBI4I) (GLuint index, GLint x, GLint y, GLint z, GLint w);
typedef void (APIENTRY * PFNGLVERTEXATTRIBI4UI) (GLuint index, GLuint x, GLuint y, GLuint z, GLuint w);
typedef void (APIENTRY * PFNGLVERTEXATTRIBI4IV) (GLuint index, const GLint* v);
typedef void (APIENTRY * PFNGLVERTEXATTRIBI4UIV) (GLuint index, const GLuint* v);
typedef void (APIENTRY * PFNGLGETUNIFORMUIV) (GLuint program, GLint location, GLuint* params);
typedef GLint (APIENTRY * PFNGLGETFRAGDATALOCATION) (GLuint program, const GLchar *name);
typedef void (APIENTRY * PFNGLUNIFORM1UI) (GLint location, GLuint v0);
typedef void (APIENTRY * PFNGLUNIFORM2UI) (GLint location, GLuint v0, GLuint v1);
typedef void (APIENTRY * PFNGLUNIFORM3UI) (GLint location, GLuint v0, GLuint v1, GLuint v2);
typedef void (APIENTRY * PFNGLUNIFORM4UI) (GLint location, GLuint v0, GLuint v1, GLuint v2, GLuint v3);
typedef void (APIENTRY * PFNGLUNIFORM1UIV) (GLint location, GLsizei count, const GLuint* value);
typedef void (APIENTRY * PFNGLUNIFORM2UIV) (GLint location, GLsizei count, const GLuint* value);
typedef void (APIENTRY * PFNGLUNIFORM3UIV) (GLint location, GLsizei count, const GLuint* value);
typedef void (APIENTRY * PFNGLUNIFORM4UIV) (GLint location, GLsizei count, const GLuint* value);
typedef void (APIENTRY * PFNGLCLEARBUFFERIV) (GLenum buffer, GLint drawbuffer, const GLint* value);
typedef void (APIENTRY * PFNGLCLEARBUFFERUIV) (GLenum buffer, GLint drawbuffer, const GLuint* value);
typedef void (APIENTRY * PFNGLCLEARBUFFERFV) (GLenum buffer, GLint drawbuffer, const GLfloat* value);
typedef void (APIENTRY * PFNGLCLEARBUFFERFI) (GLenum buffer, GLint drawbuffer, GLfloat depth, GLint stencil);
typedef const GLubyte* (APIENTRY * PFNGLGETSTRINGI) (GLenum name, GLuint index);
typedef void (APIENTRY * PFNGLCOPYBUFFERSUBDATA) (GLenum readTarget, GLenum writeTarget, GLintptr readOffset, GLintptr writeOffset, GLsizeiptr size);
typedef void (APIENTRY * PFNGLGETUNIFORMINDICES) (GLuint program, GLsizei uniformCount, const GLchar* const* uniformNames, GLuint* uniformIndices);
typedef void (APIENTRY * PFNGLGETACTIVEUNIFORMSIV) (GLuint program, GLsizei uniformCount, const GLuint* uniformIndices, GLenum pname, GLint* params);
typedef GLuint (APIENTRY * PFNGLGETUNIFORMBLOCKINDEX) (GLuint program, const GLchar* uniformBlockName);
typedef void (APIENTRY * PFNGLGETACTIVEUNIFORMBLOCKIV) (GLuint program, GLuint uniformBlockIndex, GLenum pname, GLint* params);
typedef void (APIENTRY * PFNGLGETACTIVEUNIFORMBLOCKNAME) (GLuint program, GLuint uniformBlockIndex, GLsizei bufSize, GLsizei* length, GLchar* uniformBlockName);
typedef void (APIENTRY * PFNGLUNIFORMBLOCKBINDING) (GLuint program, GLuint uniformBlockIndex, GLuint uniformBlockBinding);
typedef void (APIENTRY * PFNGLDRAWARRAYSINSTANCED) (GLenum mode, GLint first, GLsizei count, GLsizei instanceCount);
typedef void (APIENTRY * PFNGLDRAWELEMENTSINSTANCED) (GLenum mode, GLsizei count, GLenum type, const GLvoid* indices, GLsizei instanceCount);
//typedef GLsync (APIENTRY * PFNGLFENCESYNC) (GLenum condition, GLbitfield flags);
typedef GLboolean (APIENTRY * PFNGLISSYNC) (GLsync sync);
//typedef void (APIENTRY * PFNGLDELETESYNC) (GLsync sync);
//typedef GLenum (APIENTRY * PFNGLCLIENTWAITSYNC) (GLsync sync, GLbitfield flags, GLuint64 timeout);
typedef void (APIENTRY * PFNGLWAITSYNC) (GLsync sync, GLbitfield flags, GLuint64 timeout);
typedef void (APIENTRY * PFNGLGETINTEGER64V) (GLenum pname, GLint64* params);
//typedef void (APIENTRY * PFNGLGETSYNCIV) (GLsync sync, GLenum pname, GLsizei bufSize, GLsizei* length, GLint* values);
typedef void (APIENTRY * PFNGLGETINTEGER64I_V) (GLenum target, GLuint index, GLint64* data);
typedef void (APIENTRY * PFNGLGETBUFFERPARAMETERI64V) (GLenum target, GLenum pname, GLint64* params);
typedef void (APIENTRY * PFNGLGENSAMPLERS) (GLsizei count, GLuint* samplers);
typedef void (APIENTRY * PFNGLDELETESAMPLERS) (GLsizei count, const GLuint* samplers);
typedef GLboolean (APIENTRY * PFNGLISSAMPLER) (GLuint sampler);
typedef void (APIENTRY * PFNGLBINDSAMPLER) (GLuint unit, GLuint sampler);
typedef void (APIENTRY * PFNGLSAMPLERPARAMETERI) (GLuint sampler, GLenum pname, GLint param);
typedef void (APIENTRY * PFNGLSAMPLERPARAMETERIV) (GLuint sampler, GLenum pname, const GLint* param);
typedef void (APIENTRY * PFNGLSAMPLERPARAMETERF) (GLuint sampler, GLenum pname, GLfloat param);
typedef void (APIENTRY * PFNGLSAMPLERPARAMETERFV) (GLuint sampler, GLenum pname, const GLfloat* param);
typedef void (APIENTRY * PFNGLGETSAMPLERPARAMETERIV) (GLuint sampler, GLenum pname, GLint* params);
typedef void (APIENTRY * PFNGLGETSAMPLERPARAMETERFV) (GLuint sampler, GLenum pname, GLfloat* params);
typedef void (APIENTRY * PFNGLVERTEXATTRIBDIVISOR) (GLuint index, GLuint divisor);
typedef void (APIENTRY * PFNGLBINDTRANSFORMFEEDBACK) (GLenum target, GLuint id);
typedef void (APIENTRY * PFNGLDELETETRANSFORMFEEDBACKS) (GLsizei n, const GLuint* ids);
typedef void (APIENTRY * PFNGLGENTRANSFORMFEEDBACKS) (GLsizei n, GLuint* ids);
typedef GLboolean (APIENTRY * PFNGLISTRANSFORMFEEDBACK) (GLuint id);
typedef void (APIENTRY * PFNGLPAUSETRANSFORMFEEDBACK) (void);
typedef void (APIENTRY * PFNGLRESUMETRANSFORMFEEDBACK) (void);
//typedef void (APIENTRY * PFNGLGETPROGRAMBINARY) (GLuint program, GLsizei bufSize, GLsizei* length, GLenum* binaryFormat, GLvoid* binary);
//typedef void (APIENTRY * PFNGLPROGRAMBINARY) (GLuint program, GLenum binaryFormat, const GLvoid* binary, GLsizei length);
typedef void (APIENTRY * PFNGLPROGRAMPARAMETERI) (GLuint program, GLenum pname, GLint value);
typedef void (APIENTRY * PFNGLINVALIDATEFRAMEBUFFER) (GLenum target, GLsizei numAttachments, const GLenum* attachments);
typedef void (APIENTRY * PFNGLINVALIDATESUBFRAMEBUFFER) (GLenum target, GLsizei numAttachments, const GLenum* attachments, GLint x, GLint y, GLsizei width, GLsizei height);
typedef void (APIENTRY * PFNGLTEXSTORAGE2D) (GLenum target, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height);
typedef void (APIENTRY * PFNGLTEXSTORAGE3D) (GLenum target, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth);
typedef void (APIENTRY * PFNGLGETINTERNALFORMATIV) (GLenum target, GLenum internalformat, GLenum pname, GLsizei bufSize, GLint* params);
typedef void (APIENTRY * PFNGLTEXIMAGE3D) (GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLenum format, GLenum type, const void *pixels);
typedef void (APIENTRY * PFNGLTEXSUBIMAGE3D) (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const void *pixels);
typedef void (APIENTRY * PFNGLCOPYTEXSUBIMAGE3D) (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLint x, GLint y, GLsizei width, GLsizei height);
typedef void (APIENTRY * PFNGLCOMPRESSEDTEXIMAGE3D) (GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLsizei imageSize, const void *data);
typedef void (APIENTRY * PFNGLCOMPRESSEDTEXSUBIMAGE3D) (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLsizei imageSize, const void *data);
typedef void (APIENTRY * PFNGLGETSYNCIV) (GLsync sync, GLenum pname, GLsizei bufSize, GLsizei *length, GLint *values);
typedef void (APIENTRY * PFNGLRENDERBUFFERSTORAGEMULTISAMPLE) (GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height);


typedef enum _khrIMAGE_TYPE
{
    KHR_IMAGE_TEXTURE_2D = 1,
    KHR_IMAGE_TEXTURE_CUBE,
    KHR_IMAGE_TEXTURE_3D,
    KHR_IMAGE_RENDER_BUFFER,
    KHR_IMAGE_VG_IMAGE,
    KHR_IMAGE_PIXMAP,
#ifdef EGL_API_ANDROID
    KHR_IMAGE_ANDROID_NATIVE_BUFFER,
#endif
} khrIMAGE_TYPE;

#define apiCC(c1, c2, c3, c4) \
( \
    (char)(c1) \
    | \
    ((char)(c2) << 8) \
    | \
    ((char)(c3) << 16) \
    | \
    ((char)(c4) << 24) \
)

#define KHR_EGL_IMAGE_MAGIC_NUM        apiCC('I','M','A','G')

typedef struct _khrEGL_IMAGE
{
    GLuint magic;
    khrIMAGE_TYPE type;
    union
    {
        struct _khrEGL_IMAGE_TEXTURE
        {
            GLuint width;
            GLuint height;

            /* Format defined in GLES. */
            GLuint format;

            GLint level;
            GLint depth;

            GLint texture;
            void* object;
        } texture;

        struct _khrEGL_IMAGE_PIXMAP
        {
            GLuint width;
            GLuint height;
            GLuint format;
            GLint stride;
            void* address;
        } pixmap;

        struct _khrEGL_IMAGE_VGIMAGE
        {
            GLuint width;
            GLuint height;
            GLuint offset_x;
            GLuint offset_y;
            GLuint format;
            GLuint allowedQuality;
        } vgimage;

#ifdef EGL_API_ANDROID
        struct _khrEGL_IMAGE_ANDROID
        {
            GLuint width;
            GLuint height;
            GLuint format;
            GLint stride;
            void* address;
            GLuint nativeFormat;
            GLBoolean bCPURender;
            void* native;
        } androidNativeBuffer;
#endif
    } u;
} khrEGL_IMAGE;

/* Reference desktop OpenGL Engine. */
const stEngine*
ConstructOpenGLEngine(
    void
    );

/* De-reference desktop OpenGL Engine. */
void
DestroyOpenGLEngine(
    IN const stEngine *Eng
    );

GLvoid*
GetFuncAddress(
    int client,
    const GLchar *proc
    );

extern stEngine     *g_opengl;

#ifdef __cplusplus
}
#endif

#endif /* __API_OES_ENGINE_H */
