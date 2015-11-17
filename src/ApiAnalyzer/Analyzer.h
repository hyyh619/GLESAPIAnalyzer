#ifndef __ANALYZER_GLES3_H
#define __ANALYZER_GLES3_H

#include <GLES3/gl31.h>

#define ANALYZE_BEGIN_EVENT()   \
    if (!m_bAnalyzeEnabled) \
        return; \
    BeginEvent()

#define ANALYZE_END_EVENT(func) EndEvent(func)

#define ANALYZER_FUNC1(funcName, arg1)  \
    CURRENT_CONTEXT1().m_pAnalyzer->Analyze##funcName(output, outputSize, arg1)
#define ANALYZER_FUNC2(funcName, arg1, arg2)  \
    CURRENT_CONTEXT1().m_pAnalyzer->Analyze##funcName(output, outputSize, arg1, arg2)
#define ANALYZER_FUNC3(funcName, arg1, arg2, arg3)  \
    CURRENT_CONTEXT1().m_pAnalyzer->Analyze##funcName(output, outputSize, arg1, arg2, arg3)
#define ANALYZER_FUNC4(funcName, arg1, arg2, arg3, arg4)  \
    CURRENT_CONTEXT1().m_pAnalyzer->Analyze##funcName(output, outputSize, arg1, arg2, arg3, arg4)
#define ANALYZER_FUNC5(funcName, arg1, arg2, arg3, arg4, arg5)  \
    CURRENT_CONTEXT1().m_pAnalyzer->Analyze##funcName(output, outputSize, arg1, arg2, arg3, arg4, arg5)
#define ANALYZER_FUNC6(funcName, arg1, arg2, arg3, arg4, arg5, arg6)  \
    CURRENT_CONTEXT1().m_pAnalyzer->Analyze##funcName(output, outputSize, arg1, arg2, arg3, arg4, arg5, arg6)
#define ANALYZER_FUNC7(funcName, arg1, arg2, arg3, arg4, arg5, arg6, arg7)  \
    CURRENT_CONTEXT1().m_pAnalyzer->Analyze##funcName(output, outputSize, arg1, arg2, arg3, arg4, arg5, arg6, arg7)
#define ANALYZER_FUNC8(funcName, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8)  \
    CURRENT_CONTEXT1().m_pAnalyzer->Analyze##funcName(output, outputSize, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8)
#define ANALYZER_FUNC9(funcName, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9)  \
    CURRENT_CONTEXT1().m_pAnalyzer->Analyze##funcName(output, outputSize, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9)
#define ANALYZER_FUNC10(funcName, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10)  \
    CURRENT_CONTEXT1().m_pAnalyzer->Analyze##funcName(output, outputSize, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10)
#define ANALYZER_FUNC11(funcName, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11)  \
    CURRENT_CONTEXT1().m_pAnalyzer->Analyze##funcName(output, outputSize, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11)

class CGLES3Context;

class CAnalyzer
{
public:
    CAnalyzer();
    ~CAnalyzer();

    GLvoid                  InitAnalyzer(const GLchar *filePath, CGLES3Context *pContext);
    GLvoid                  ReleaseAnalyzer();
    GLvoid                  EnableAnalyze(GLboolean flag);
    GLboolean               DumpStringToApiDump(const GLchar *string);
    GLvoid                  BeginEvent();
    GLvoid                  EndEvent(GLESAPIIndex func);
    GLboolean               IsDrawCommand(GLESAPIIndex name);
    GLvoid                  ErrorCheck(GLESAPIIndex func);
    const GLchar*           ConvertErrCode2String(GLenum err);

public:
    CGLES3Context           *m_pContext;

    GLboolean               m_bAnalyzeEnabled;
    GLboolean               m_bErrorCheck;
    GLuint                  m_nEventSequence;
    GLuint                  m_nCurrentLoadEventSequence;

public:
    void                    AnalyzeChooseConfig(GLchar *output, GLint outputSize, EGLDisplay dpy, const EGLint *attrib_list, EGLConfig *configs, EGLint config_size, EGLint *num_config, EGLBoolean res);
    void                    AnalyzeCopyBuffers(GLchar *output, GLint outputSize, EGLDisplay dpy, EGLSurface surface, EGLNativePixmapType target, EGLBoolean res);
    void                    AnalyzeCreateContext(GLchar *output, GLint outputSize, EGLDisplay dpy, EGLConfig config, EGLContext share_context, const EGLint *attrib_list, EGLContext res);
    void                    AnalyzeCreatePbufferSurface(GLchar *output, GLint outputSize, EGLDisplay dpy, EGLConfig config, const EGLint *attrib_list, EGLSurface res);
    void                    AnalyzeCreatePixmapSurface(GLchar *output, GLint outputSize, EGLDisplay dpy, EGLConfig config, EGLNativePixmapType pixmap, const EGLint *attrib_list, EGLSurface res);
    void                    AnalyzeCreateWindowSurface(GLchar *output, GLint outputSize, EGLDisplay dpy, EGLConfig config, EGLNativeWindowType win, const EGLint *attrib_list, EGLSurface res);
    void                    AnalyzeDestroyContext(GLchar *output, GLint outputSize, EGLDisplay dpy, EGLContext ctx, EGLBoolean res);
    void                    AnalyzeDestroySurface(GLchar *output, GLint outputSize, EGLDisplay dpy, EGLSurface surface, EGLBoolean res);
    void                    AnalyzeGetConfigAttrib(GLchar *output, GLint outputSize, EGLDisplay dpy, EGLConfig config, EGLint attribute, EGLint *value, EGLBoolean res);
    void                    AnalyzeGetConfigs(GLchar *output, GLint outputSize, EGLDisplay dpy, EGLConfig *configs, EGLint config_size, EGLint *num_config, EGLBoolean res);
    void                    AnalyzeGetCurrentDisplay(GLchar *output, GLint outputSize, EGLDisplay res);
    void                    AnalyzeGetCurrentSurface(GLchar *output, GLint outputSize, EGLint readdraw, EGLSurface res);
    void                    AnalyzeGetDisplay(GLchar *output, GLint outputSize, EGLNativeDisplayType display_id, EGLDisplay res);
    void                    eglGetError(GLchar *output, GLint outputSize, EGLint err);
    void                    AnalyzeSetFIFO(GLchar *output, GLint outputSize, void* fifo, EGLBoolean res);
    void                    AnalyzeGetProcAddress(GLchar *output, GLint outputSize, const char *procname);
    void                    AnalyzeInitialize(GLchar *output, GLint outputSize, EGLDisplay dpy, EGLint *major, EGLint *minor, EGLBoolean res);
    void                    AnalyzeMakeCurrent(GLchar *output, GLint outputSize, EGLDisplay dpy, EGLSurface draw, EGLSurface read, EGLContext ctx, EGLBoolean res);
    void                    AnalyzeQueryContext(GLchar *output, GLint outputSize, EGLDisplay dpy, EGLContext ctx, EGLint attribute, EGLint *value, EGLBoolean res);
    void                    AnalyzeQueryString(GLchar *output, GLint outputSize, EGLDisplay dpy, EGLint name, const char* res);
    void                    AnalyzeQuerySurface(GLchar *output, GLint outputSize, EGLDisplay dpy, EGLSurface surface, EGLint attribute, EGLint *value, EGLBoolean res);
    void                    AnalyzeSwapBuffers(GLchar *output, GLint outputSize, EGLDisplay dpy, EGLSurface surface, EGLBoolean res);
    void                    AnalyzeTerminate(GLchar *output, GLint outputSize, EGLDisplay dpy, EGLBoolean res);
    void                    AnalyzeWaitGL(GLchar *output, GLint outputSize, EGLBoolean res);
    void                    AnalyzeWaitNative(GLchar *output, GLint outputSize, EGLint engine, EGLBoolean res);
    void                    AnalyzeBindTexImage(GLchar *output, GLint outputSize, EGLDisplay dpy, EGLSurface surface, EGLint buffer, EGLBoolean res);
    void                    AnalyzeReleaseTexImage(GLchar *output, GLint outputSize, EGLDisplay dpy, EGLSurface surface, EGLint buffer, EGLBoolean res);
    void                    AnalyzeSurfaceAttrib(GLchar *output, GLint outputSize, EGLDisplay dpy, EGLSurface surface, EGLint attribute, EGLint value, EGLBoolean res);
    void                    AnalyzeSwapInterval(GLchar *output, GLint outputSize, EGLDisplay dpy, EGLint interval, EGLBoolean res);
    void                    AnalyzeBindAPI(GLchar *output, GLint outputSize, EGLenum api, EGLBoolean res);
    void                    AnalyzeQueryAPI(GLchar *output, GLint outputSize, EGLenum res);
    void                    AnalyzeCreatePbufferFromClientBuffer(GLchar *output, GLint outputSize, EGLDisplay dpy, EGLenum buftype, EGLClientBuffer buffer, EGLConfig config, const EGLint *attrib_list, EGLSurface res);
    void                    AnalyzeReleaseThread(GLchar *output, GLint outputSize, EGLBoolean res);
    void                    AnalyzeWaitClient(GLchar *output, GLint outputSize, EGLBoolean res);
    void                    AnalyzeGetCurrentContext(GLchar *output, GLint outputSize, EGLContext res);

    GLvoid                  AnalyzeGenBuffers(GLchar *output, GLint outputSize, GLsizei n, GLuint *buffers);
    GLvoid                  AnalyzeDeleteBuffers(GLchar *output, GLint outputSize, GLsizei n, const GLuint *buffers);
    GLvoid                  AnalyzeBindBuffer(GLchar *output, GLint outputSize, GLenum target, GLuint buffer);
    GLvoid                  AnalyzeBindBufferBase(GLchar *output, GLint outputSize, GLenum target, GLuint index, GLuint buffer);
    GLvoid                  AnalyzeBindBufferRange(GLchar *output, GLint outputSize, GLenum target, GLuint index, GLuint buffer, GLintptr offset, GLsizeiptr size);
    GLvoid                  AnalyzeBufferData(GLchar *output, GLint outputSize, GLenum target, GLsizeiptr size, const void *data, GLenum usage);
    GLvoid                  AnalyzeBufferSubData(GLchar *output, GLint outputSize, GLenum target, GLintptr offset, GLsizeiptr size, const void *data);
    GLvoid                  AnalyzeDeleteVertexArrays(GLchar *output, GLint outputSize, GLsizei n, const GLuint *arrays);
    GLvoid                  AnalyzeGenVertexArrays(GLchar *output, GLint outputSize, GLsizei n, GLuint *arrays);
    GLvoid                  AnalyzeBindVertexArray(GLchar *output, GLint outputSize, GLuint array);
    GLvoid                  AnalyzeBindTransformFeedback(GLchar *output, GLint outputSize, GLenum target, GLuint id);
    GLvoid                  AnalyzeDeleteTransformFeedbacks(GLchar *output, GLint outputSize, GLsizei n, const GLuint *ids);
    GLvoid                  AnalyzeGenTransformFeedbacks(GLchar *output, GLint outputSize, GLsizei n, GLuint *ids);
    GLvoid                  AnalyzeIsTransformFeedback(GLchar *output, GLint outputSize, GLuint id, GLboolean res);
    GLvoid                  AnalyzeBeginTransformFeedback(GLchar *output, GLint outputSize, GLenum primitiveMode);
    GLvoid                  AnalyzeEndTransformFeedback(GLchar *output, GLint outputSize);
    GLvoid                  AnalyzeDisableVertexAttribArray(GLchar *output, GLint outputSize, GLuint index);
    GLvoid                  AnalyzeEnableVertexAttribArray(GLchar *output, GLint outputSize, GLuint index);
    GLvoid                  AnalyzeVertexAttrib1f(GLchar *output, GLint outputSize, GLuint index, GLfloat x);
    GLvoid                  AnalyzeVertexAttrib1fv(GLchar *output, GLint outputSize, GLuint index, const GLfloat *v);
    GLvoid                  AnalyzeVertexAttrib2f(GLchar *output, GLint outputSize, GLuint index, GLfloat x, GLfloat y);
    GLvoid                  AnalyzeVertexAttrib2fv(GLchar *output, GLint outputSize, GLuint index, const GLfloat *v);
    GLvoid                  AnalyzeVertexAttrib3f(GLchar *output, GLint outputSize, GLuint index, GLfloat x, GLfloat y, GLfloat z);
    GLvoid                  AnalyzeVertexAttrib3fv(GLchar *output, GLint outputSize, GLuint index, const GLfloat *v);
    GLvoid                  AnalyzeVertexAttrib4f(GLchar *output, GLint outputSize, GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
    GLvoid                  AnalyzeVertexAttrib4fv(GLchar *output, GLint outputSize, GLuint index, const GLfloat *v);
    GLvoid                  AnalyzeVertexAttribPointer(GLchar *output, GLint outputSize, GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void *pointer);
    GLvoid                  AnalyzeVertexAttribIPointer(GLchar *output, GLint outputSize, GLuint index, GLint size, GLenum type, GLsizei stride, const void *pointer);
    GLvoid                  AnalyzeGetBufferParameteriv(GLchar *output, GLint outputSize, GLenum target, GLenum pname, GLint *params);
    GLvoid                  AnalyzeGetVertexAttribfv(GLchar *output, GLint outputSize, GLuint index, GLenum pname, GLfloat *params);
    GLvoid                  AnalyzeGetVertexAttribiv(GLchar *output, GLint outputSize, GLuint index, GLenum pname, GLint *params);
    GLvoid                  AnalyzeGetVertexAttribPointerv(GLchar *output, GLint outputSize, GLuint index, GLenum pname, void **pointer);

    GLvoid                  AnalyzeCreateShader(GLchar *output, GLint outputSize, GLenum type, GLuint shader);
    GLvoid                  AnalyzeShaderSource(GLchar *output, GLint outputSize, GLuint shader, GLsizei count, const GLchar *const*string, const GLint *length);
    GLvoid                  AnalyzeDeleteShader(GLchar *output, GLint outputSize, GLuint shader);
    GLvoid                  AnalyzeCreateProgram(GLchar *output, GLint outputSize, GLuint program);
    GLvoid                  AnalyzeUseProgram(GLchar *output, GLint outputSize, GLuint program);
    GLvoid                  AnalyzeAttachShader(GLchar *output, GLint outputSize, GLuint program, GLuint shader);
    GLvoid                  AnalyzeGetUniformLocation(GLchar *output, GLint outputSize, GLuint program, const GLchar *name, GLint oldLoc);
    GLvoid                  AnalyzeLinkProgram(GLchar *output, GLint outputSize, GLuint program);
    GLvoid                  AnalyzeCompileShader(GLchar *output, GLint outputSize, GLuint shader);
    GLvoid                  AnalyzeDetachShader(GLchar *output, GLint outputSize, GLuint program, GLuint shader);
    GLvoid                  AnalyzeDeleteProgram(GLchar *output, GLint outputSize, GLuint program);
    GLvoid                  AnalyzeGetAttribLocation(GLchar *output, GLint outputSize, GLuint program, const GLchar *name, GLint oldLoc);
    GLvoid                  AnalyzeBindAttribLocation(GLchar *output, GLint outputSize, GLuint program, GLint loc, const GLchar *name);
    GLvoid                  AnalyzeProgramBinary(GLchar *output, GLint outputSize, GLuint program, GLenum binaryFormat, const void *binary, GLsizei length);
    GLvoid                  AnalyzeUniform1f(GLchar *output, GLint outputSize, GLint location, GLfloat v0);
    GLvoid                  AnalyzeUniform1fv(GLchar *output, GLint outputSize, GLint location, GLsizei count, const GLfloat *value);
    GLvoid                  AnalyzeUniform1i(GLchar *output, GLint outputSize, GLint location, GLint v0);
    GLvoid                  AnalyzeUniform1iv(GLchar *output, GLint outputSize, GLint location, GLsizei count, const GLint *value);
    GLvoid                  AnalyzeUniform2f(GLchar *output, GLint outputSize, GLint location, GLfloat v0, GLfloat v1);
    GLvoid                  AnalyzeUniform2fv(GLchar *output, GLint outputSize, GLint location, GLsizei count, const GLfloat *value);
    GLvoid                  AnalyzeUniform2i(GLchar *output, GLint outputSize, GLint location, GLint v0, GLint v1);
    GLvoid                  AnalyzeUniform2iv(GLchar *output, GLint outputSize, GLint location, GLsizei count, const GLint *value);
    GLvoid                  AnalyzeUniform3f(GLchar *output, GLint outputSize, GLint location, GLfloat v0, GLfloat v1, GLfloat v2);
    GLvoid                  AnalyzeUniform3fv(GLchar *output, GLint outputSize, GLint location, GLsizei count, const GLfloat *value);
    GLvoid                  AnalyzeUniform3i(GLchar *output, GLint outputSize, GLint location, GLint v0, GLint v1, GLint v2);
    GLvoid                  AnalyzeUniform3iv(GLchar *output, GLint outputSize, GLint location, GLsizei count, const GLint *value);
    GLvoid                  AnalyzeUniform4f(GLchar *output, GLint outputSize, GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3);
    GLvoid                  AnalyzeUniform4fv(GLchar *output, GLint outputSize, GLint location, GLsizei count, const GLfloat *value);
    GLvoid                  AnalyzeUniform4i(GLchar *output, GLint outputSize, GLint location, GLint v0, GLint v1, GLint v2, GLint v3);
    GLvoid                  AnalyzeUniform4iv(GLchar *output, GLint outputSize, GLint location, GLsizei count, const GLint *value);
    GLvoid                  AnalyzeUniformMatrix2fv(GLchar *output, GLint outputSize, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
    GLvoid                  AnalyzeUniformMatrix3fv(GLchar *output, GLint outputSize, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
    GLvoid                  AnalyzeUniformMatrix4fv(GLchar *output, GLint outputSize, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
    GLvoid                  AnalyzeGetActiveAttrib(GLchar *output, GLint outputSize, GLuint program, GLuint index, GLsizei bufSize, GLsizei *length, GLint *size, GLenum *type, GLchar *name);
    GLvoid                  AnalyzeGetActiveUniform(GLchar *output, GLint outputSize, GLuint program, GLuint index, GLsizei bufSize, GLsizei *length, GLint *size, GLenum *type, GLchar *name);
    GLvoid                  AnalyzeGetAttachedShaders(GLchar *output, GLint outputSize, GLuint program, GLsizei maxCount, GLsizei *count, GLuint *shaders);
    GLvoid                  AnalyzeGetProgramiv(GLchar *output, GLint outputSize, GLuint program, GLenum pname, GLint *params);
    GLvoid                  AnalyzeGetProgramInfoLog(GLchar *output, GLint outputSize, GLuint program, GLsizei bufSize, GLsizei *length, GLchar *infoLog);
    GLvoid                  AnalyzeGetShaderiv(GLchar *output, GLint outputSize, GLuint shader, GLenum pname, GLint *params);
    GLvoid                  AnalyzeGetShaderInfoLog(GLchar *output, GLint outputSize, GLuint shader, GLsizei bufSize, GLsizei *length, GLchar *infoLog);
    GLvoid                  AnalyzeGetShaderPrecisionFormat(GLchar *output, GLint outputSize, GLenum shadertype, GLenum precisiontype, GLint *range, GLint *precision);
    GLvoid                  AnalyzeGetShaderSource(GLchar *output, GLint outputSize, GLuint shader, GLsizei bufSize, GLsizei *length, GLchar *source);
    GLvoid                  AnalyzeGetUniformfv(GLchar *output, GLint outputSize, GLuint program, GLint location, GLfloat *params);
    GLvoid                  AnalyzeGetUniformiv(GLchar *output, GLint outputSize, GLuint program, GLint location, GLint *params);
    GLvoid                  AnalyzeShaderBinary(GLchar *output, GLint outputSize, GLsizei count, const GLuint *shaders, GLenum binaryformat, const void *binary, GLsizei length);
    GLvoid                  AnalyzeValidateProgram(GLchar *output, GLint outputSize, GLuint program);

    GLvoid                  AnalyzeActiveTexture(GLchar *output, GLint outputSize, GLenum texture);
    GLvoid                  AnalyzeDeleteTextures(GLchar *output, GLint outputSize, GLsizei n, const GLuint *textures);
    GLvoid                  AnalyzeBindTexture(GLchar *output, GLint outputSize, GLenum target, GLuint texture);
    GLvoid                  AnalyzeGenTextures(GLchar *output, GLint outputSize, GLsizei n, GLuint *textures);
    GLvoid                  AnalyzeGenerateMipmap(GLchar *output, GLint outputSize, GLenum target);
    GLvoid                  AnalyzeTexImage2D(GLchar *output, GLint outputSize, GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const void *pixels);
    GLvoid                  AnalyzeTexSubImage2D(GLchar *output, GLint outputSize, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const void *pixels);
    GLvoid                  AnalyzeTexImage3D(GLchar *output, GLint outputSize, GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLenum format, GLenum type, const void *pixels);
    GLvoid                  AnalyzeTexSubImage3D(GLchar *output, GLint outputSize, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const void *pixels);
    GLvoid                  AnalyzeTexStorage2D(GLchar *output, GLint outputSize, GLenum target, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height);
    GLvoid                  AnalyzeTexStorage3D(GLchar *output, GLint outputSize, GLenum target, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth);
    GLvoid                  AnalyzeCompressedTexImage2D(GLchar *output, GLint outputSize, GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLsizei imageSize, const void *data);
    GLvoid                  AnalyzeCompressedTexSubImage2D(GLchar *output, GLint outputSize, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLsizei imageSize, const void *data);
    GLvoid                  AnalyzeTexParameterf(GLchar *output, GLint outputSize, GLenum target, GLenum pname, GLfloat param);
    GLvoid                  AnalyzeTexParameterfv(GLchar *output, GLint outputSize, GLenum target, GLenum pname, const GLfloat *params);
    GLvoid                  AnalyzeTexParameteri(GLchar *output, GLint outputSize, GLenum target, GLenum pname, GLint param);
    GLvoid                  AnalyzeTexParameteriv(GLchar *output, GLint outputSize, GLenum target, GLenum pname, const GLint *params);
    GLvoid                  AnalyzeCopyTexImage2D(GLchar *output, GLint outputSize, GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLsizei height, GLint border);
    GLvoid                  AnalyzeCopyTexSubImage2D(GLchar *output, GLint outputSize, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height);
    GLvoid                  AnalyzeGetTexParameterfv(GLchar *output, GLint outputSize, GLenum target, GLenum pname, GLfloat *params);
    GLvoid                  AnalyzeGetTexParameteriv(GLchar *output, GLint outputSize, GLenum target, GLenum pname, GLint *params);

    GLvoid                  AnalyzeBindFramebuffer(GLchar *output, GLint outputSize, GLenum target, GLuint framebuffer);
    GLvoid                  AnalyzeBindRenderbuffer(GLchar *output, GLint outputSize, GLenum target, GLuint renderbuffer);
    GLvoid                  AnalyzeDeleteFramebuffers(GLchar *output, GLint outputSize, GLsizei n, const GLuint *framebuffers);
    GLvoid                  AnalyzeDeleteRenderbuffers(GLchar *output, GLint outputSize, GLsizei n, const GLuint *renderbuffers);
    GLvoid                  AnalyzeGenFramebuffers(GLchar *output, GLint outputSize, GLsizei n, GLuint *framebuffers);
    GLvoid                  AnalyzeGenRenderbuffers(GLchar *output, GLint outputSize, GLsizei n, GLuint *renderbuffers);
    GLvoid                  AnalyzeRenderbufferStorage(GLchar *output, GLint outputSize, GLenum target, GLenum internalformat, GLsizei width, GLsizei height);
    GLvoid                  AnalyzeFramebufferRenderbuffer(GLchar *output, GLint outputSize, GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer);
    GLvoid                  AnalyzeFramebufferTexture2D(GLchar *output, GLint outputSize, GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level);
    GLvoid                  AnalyzeCheckFramebufferStatus(GLchar *output, GLint outputSize, GLenum target, GLenum result);
    GLvoid                  AnalyzeGetFramebufferAttachmentParameteriv(GLchar *output, GLint outputSize, GLenum target, GLenum attachment, GLenum pname, GLint *params);
    GLvoid                  AnalyzeGetRenderbufferParameteriv(GLchar *output, GLint outputSize, GLenum target, GLenum pname, GLint *params);

    GLvoid                  AnalyzeDisable(GLchar *output, GLint outputSize, GLenum cap);
    GLvoid                  AnalyzeEnable(GLchar *output, GLint outputSize, GLenum cap);
    GLvoid                  AnalyzeFrontFace(GLchar *output, GLint outputSize, GLenum mode);
    GLvoid                  AnalyzeCullFace(GLchar *output, GLint outputSize, GLenum mode);
    GLvoid                  AnalyzeBlendColor(GLchar *output, GLint outputSize, GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
    GLvoid                  AnalyzeBlendEquation(GLchar *output, GLint outputSize, GLenum mode);
    GLvoid                  AnalyzeBlendEquationSeparate(GLchar *output, GLint outputSize, GLenum modeRGB, GLenum modeAlpha);
    GLvoid                  AnalyzeBlendFunc(GLchar *output, GLint outputSize, GLenum sfactor, GLenum dfactor);
    GLvoid                  AnalyzeBlendFuncSeparate(GLchar *output, GLint outputSize, GLenum sfactorRGB, GLenum dfactorRGB, GLenum sfactorAlpha, GLenum dfactorAlpha);
    GLvoid                  AnalyzeDepthFunc(GLchar *output, GLint outputSize, GLenum func);
    GLvoid                  AnalyzeDepthRangef(GLchar *output, GLint outputSize, GLfloat n, GLfloat f);
    GLvoid                  AnalyzeClearColor(GLchar *output, GLint outputSize, GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
    GLvoid                  AnalyzeClearDepthf(GLchar *output, GLint outputSize, GLfloat d);
    GLvoid                  AnalyzeClearStencil(GLchar *output, GLint outputSize, GLint s);
    GLvoid                  AnalyzeColorMask(GLchar *output, GLint outputSize, GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha);
    GLvoid                  AnalyzeDepthMask(GLchar *output, GLint outputSize, GLboolean flag);
    GLvoid                  AnalyzeLineWidth(GLchar *output, GLint outputSize, GLfloat width);
    GLvoid                  AnalyzePixelStorei(GLchar *output, GLint outputSize, GLenum pname, GLint param);
    GLvoid                  AnalyzePolygonOffset(GLchar *output, GLint outputSize, GLfloat factor, GLfloat units);
    GLvoid                  AnalyzeScissor(GLchar *output, GLint outputSize, GLint x, GLint y, GLsizei width, GLsizei height);
    GLvoid                  AnalyzeStencilFunc(GLchar *output, GLint outputSize, GLenum func, GLint ref, GLuint mask);
    GLvoid                  AnalyzeStencilFuncSeparate(GLchar *output, GLint outputSize, GLenum face, GLenum func, GLint ref, GLuint mask);
    GLvoid                  AnalyzeStencilMask(GLchar *output, GLint outputSize, GLuint mask);
    GLvoid                  AnalyzeStencilMaskSeparate(GLchar *output, GLint outputSize, GLenum face, GLuint mask);
    GLvoid                  AnalyzeStencilOp(GLchar *output, GLint outputSize, GLenum fail, GLenum zfail, GLenum zpass);
    GLvoid                  AnalyzeStencilOpSeparate(GLchar *output, GLint outputSize, GLenum face, GLenum sfail, GLenum dpfail, GLenum dppass);
    GLvoid                  AnalyzeSampleCoverage(GLchar *output, GLint outputSize, GLfloat value, GLboolean invert);
    GLvoid                  AnalyzeViewport(GLchar *output, GLint outputSize, GLint x, GLint y, GLsizei width, GLsizei height);
    GLvoid                  AnalyzeGetBooleanv(GLchar *output, GLint outputSize, GLenum pname, GLboolean *data);
    GLvoid                  AnalyzeGetFloatv(GLchar *output, GLint outputSize, GLenum pname, GLfloat *data);
    GLvoid                  AnalyzeGetIntegerv(GLchar *output, GLint outputSize, GLenum pname, GLint *data);
    GLvoid                  AnalyzeGetString(GLchar *output, GLint outputSize, GLenum name, const GLubyte *str);
    GLvoid                  AnalyzeIsBuffer(GLchar *output, GLint outputSize, GLuint buffer, GLboolean res);
    GLvoid                  AnalyzeIsEnabled(GLchar *output, GLint outputSize, GLenum cap, GLboolean res);
    GLvoid                  AnalyzeIsFramebuffer(GLchar *output, GLint outputSize, GLuint framebuffer, GLboolean res);
    GLvoid                  AnalyzeIsProgram(GLchar *output, GLint outputSize, GLuint program, GLboolean res);
    GLvoid                  AnalyzeIsRenderbuffer(GLchar *output, GLint outputSize, GLuint renderbuffer, GLboolean res);
    GLvoid                  AnalyzeIsShader(GLchar *output, GLint outputSize, GLuint shader, GLboolean res);
    GLvoid                  AnalyzeIsTexture(GLchar *output, GLint outputSize, GLuint texture, GLboolean res);
    GLvoid                  AnalyzeGetError(GLchar *output, GLint outputSize, GLenum err);
    GLvoid                  AnalyzeFinish(GLchar *output, GLint outputSize);
    GLvoid                  AnalyzeFlush(GLchar *output, GLint outputSize);
    GLvoid                  AnalyzeHint(GLchar *output, GLint outputSize, GLenum target, GLenum mode);
    GLvoid                  AnalyzeReadPixels(GLchar *output, GLint outputSize, GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, void *pixels);
    GLvoid                  AnalyzeReadBuffer(GLchar *output, GLint outputSize, GLenum mode);

    GLvoid                  AnalyzeClear(GLchar *output, GLint outputSize, GLbitfield mask);
    GLvoid                  AnalyzeDrawArrays(GLchar *output, GLint outputSize, GLenum mode, GLint first, GLsizei count);
    GLvoid                  AnalyzeDrawElements(GLchar *output, GLint outputSize, GLenum mode, GLsizei count, GLenum type, const GLvoid* indices);

    GLvoid                  AnalyzeBlitFramebufferEXT(GLchar *output, GLint outputSize, GLint srcX0, GLint srcY0, GLint srcX1, GLint srcY1, GLint dstX0, GLint dstY0, GLint dstX1, GLint dstY1, GLbitfield mask, GLenum filter);
};

#endif /* __ANALYZER_GLES3_H */