#ifndef __PARSE_GLES3_H
#define __PARSE_GLES3_H

#define GL_GLEXT_PROTOTYPES
#include <GLES3/gl3ext.h>
#include <GLES3/gl31.h>
#include <GLES2/gl2ext.h>
#include <GLES2/gl2.h>
#include <EGL/egl.h>

// Texture
GLvoid ParseTexImage2D(GLchar *input, GLchar *output, GLint outputSize);
GLvoid ParseTexSubImage2D(GLchar *input, GLchar *output, GLint outputSize);
GLvoid ParseCompressedTexImage2D(GLchar *input, GLchar *output, GLint outputSize);
GLvoid ParseTexImage3D(GLchar *input, GLchar *output, GLint outputSize);
GLvoid ParseTexSubImage3D(GLchar *input, GLchar *output, GLint outputSize);
GLvoid ParseTexStorage2D(GLchar *input, GLchar *output, GLint outputSize);
GLvoid ParseTexStorage3D(GLchar *input, GLchar *output, GLint outputSize);
GLvoid ParseBindTexture(GLchar *input, GLchar *output, GLint outputSize);
GLvoid ParseTexParameteri(GLchar *input, GLchar *output, GLint outputSize);
GLvoid ParseTexParameterf(GLchar *input, GLchar *output, GLint outputSize);
GLvoid ParseTexParameteriv(GLchar *input, GLchar *output, GLint outputSize);
GLvoid ParseTexParameterfv(GLchar *input, GLchar *output, GLint outputSize);
GLvoid ParseActiveTexture(GLchar *input, GLchar *output, GLint outputSize);
GLvoid ParseCopyTexImage2D(GLchar *input, GLchar *output, GLint outputSize);
GLvoid ParseCopyTexSubImage2D(GLchar *input, GLchar *output, GLint outputSize);
GLvoid ParseCompressedTexSubImage2D(GLchar *input, GLchar *output, GLint outputSize);
GLvoid ParseCheckFramebufferStatus(GLchar *input, GLchar *output, GLint outputSize);
GLvoid ParseDeleteTextures(GLchar *input, GLchar *output, GLint outputSize);
GLvoid ParseEGLImageTargetTexture2DOES(GLchar *input, GLchar *output, GLint outputSize);
GLvoid ParseGenTextures(GLchar *input, GLchar *output, GLint outputSize);
GLvoid ParseGenerateMipmap(GLchar *input, GLchar *output, GLint outputSize);
GLvoid ParseGetTexParameteriv(GLchar *input, GLchar *output, GLint outputSize);
GLvoid ParseGetTexParameterfv(GLchar *input, GLchar *output, GLint outputSize);
GLvoid ParseIsTexture(GLchar *input, GLchar *output, GLint outputSize);

// vertex buffer
GLvoid ParseDrawBuffers(GLchar *input, GLchar *output, GLint outputSize);
GLvoid ParseMapBufferRange(GLchar *input, GLchar *output, GLint outputSize);
GLvoid ParseFlushMappedBufferRange(GLchar *input, GLchar *output, GLint outputSize);
GLvoid ParseUnmapBuffer(GLchar *input, GLchar *output, GLint outputSize);
GLvoid ParseBindBufferBase(GLchar *input, GLchar *output, GLint outputSize);
GLvoid ParseBindBufferRange(GLchar *input, GLchar *output, GLint outputSize);
GLvoid ParseBeginTransformFeedback(GLchar *input, GLchar *output, GLint outputSize);
GLvoid ParseEndTransformFeedback(GLchar *input, GLchar *output, GLint outputSize);
GLvoid ParsePauseTransformFeedback(GLchar *input, GLchar *output, GLint outputSize);
GLvoid ParseResumeTransformFeedback(GLchar *input, GLchar *output, GLint outputSize);
GLvoid ParseBindBuffer(GLchar *input, GLchar *output, GLint outputSize);
GLvoid ParseBindVertexArray(GLchar *input, GLchar *output, GLint outputSize);
GLvoid ParseBindTransformFeedback(GLchar *input, GLchar *output, GLint outputSize);
GLvoid ParseBufferData(GLchar *input, GLchar *output, GLint outputSize);
GLvoid ParseBufferSubData(GLchar *input, GLchar *output, GLint outputSize);
GLvoid ParseTransformFeedbackVaryings(GLchar *input, GLchar *output, GLint outputSize);
GLvoid ParseEnableVertexAttribArray(GLchar *input, GLchar *output, GLint outputSize);
GLvoid ParseDisableVertexAttribArray(GLchar *input, GLchar *output, GLint outputSize);
GLvoid ParseVertexAttribPointer(GLchar *input, GLchar *output, GLint outputSize);
GLvoid ParseVertexAttribIPointer(GLchar *input, GLchar *output, GLint outputSize);
GLvoid ParseDeleteBuffers(GLchar *input, GLchar *output, GLint outputSize);
GLvoid ParseDeleteVertexArrays(GLchar *input, GLchar *output, GLint outputSize);
GLvoid ParseDeleteTransformFeedback(GLchar *input, GLchar *output, GLint outputSize);
GLvoid ParseGenBuffers(GLchar *input, GLchar *output, GLint outputSize);
GLvoid ParseGenVertexArrays(GLchar *input, GLchar *output, GLint outputSize);
GLvoid ParseGenTransformFeedbacks(GLchar *input, GLchar *output, GLint outputSize);
GLvoid ParseGetVertexAttribfv(GLchar *input, GLchar *output, GLint outputSize);
GLvoid ParseGetVertexAttribPointerv(GLchar *input, GLchar *output, GLint outputSize);
GLvoid ParseGetVertexAttribiv(GLchar *input, GLchar *output, GLint outputSize);
GLvoid ParseGetBufferParameteriv(GLchar *input, GLchar *output, GLint outputSize);
GLvoid ParseIsBuffer(GLchar *input, GLchar *output, GLint outputSize);
GLvoid ParseVertexAttrib1fv(GLchar *input, GLchar *output, GLint outputSize);
GLvoid ParseVertexAttrib2fv(GLchar *input, GLchar *output, GLint outputSize);
GLvoid ParseVertexAttrib3fv(GLchar *input, GLchar *output, GLint outputSize);
GLvoid ParseVertexAttrib4fv(GLchar *input, GLchar *output, GLint outputSize);
GLvoid ParseVertexAttrib1f(GLchar *input, GLchar *output, GLint outputSize);
GLvoid ParseVertexAttrib2f(GLchar *input, GLchar *output, GLint outputSize);
GLvoid ParseVertexAttrib3f(GLchar *input, GLchar *output, GLint outputSize);
GLvoid ParseVertexAttrib4f(GLchar *input, GLchar *output, GLint outputSize);

// FBO and RBO
GLvoid ParseBindFramebuffer(GLchar *input, GLchar *output, GLint outputSize);
GLvoid ParseBindRenderbuffer(GLchar *input, GLchar *output, GLint outputSize);
GLvoid ParseRenderbufferStorage(GLchar *input, GLchar *output, GLint outputSize);
GLvoid ParseFramebufferTexture2D(GLchar *input, GLchar *output, GLint outputSize);
GLvoid ParseFramebufferRenderbuffer(GLchar *input, GLchar *output, GLint outputSize);
GLvoid ParseInvalidateFramebuffer(GLchar *input, GLchar *output, GLint outputSize);
GLvoid ParseDeleteRenderbuffers(GLchar *input, GLchar *output, GLint outputSize);
GLvoid ParseDeleteFramebuffers(GLchar *input, GLchar *output, GLint outputSize);
GLvoid ParseEGLImageTargetRenderbufferStorageOES(GLchar *input, GLchar *output, GLint outputSize);
GLvoid ParseGenFramebuffers(GLchar *input, GLchar *output, GLint outputSize);
GLvoid ParseGenRenderbuffers(GLchar *input, GLchar *output, GLint outputSize);
GLvoid ParseGetRenderbufferParameteriv(GLchar *input, GLchar *output, GLint outputSize);
GLvoid ParseGetFramebufferAttachmentParameteriv(GLchar *input, GLchar *output, GLint outputSize);
GLvoid ParseIsRenderbuffer(GLchar *input, GLchar *output, GLint outputSize);
GLvoid ParseIsFramebuffer(GLchar *input, GLchar *output, GLint outputSize);

// others
GLvoid ParseFlush(GLchar *input, GLchar *output, GLint outputSize);
GLvoid ParseFinish(GLchar *input, GLchar *output, GLint outputSize);
GLvoid ParseGetError(GLchar *input, GLchar *output, GLint outputSize);
GLvoid ParseDiscardFramebufferEXT(GLchar *input, GLchar *output, GLint outputSize);
GLvoid ParseGetString(GLchar *input, GLchar *output, GLint outputSize);
GLvoid ParseGetIntegerv(GLchar *input, GLchar *output, GLint outputSize);
GLvoid ParseGetBooleanv(GLchar *input, GLchar *output, GLint outputSize);
GLvoid ParseGetFloatv(GLchar *input, GLchar *output, GLint outputSize);
GLvoid ParseHint(GLchar *input, GLchar *output, GLint outputSize);
GLvoid ParseReadPixels(GLchar *input, GLchar *output, GLint outputSize);

// shader
GLvoid ParseGetActiveUniform(GLchar *input, GLchar *output, GLint outputSize);
GLvoid ParseGetUniformLocation(GLchar *input, GLchar *output, GLint outputSize);
GLvoid ParseGetProgramBinary(GLchar *input, GLchar *output, GLint outputSize);
GLvoid ParseBindAttribLocation(GLchar *input, GLchar *output, GLint outputSize);
GLvoid ParseAttachShader(GLchar *input, GLchar *output, GLint outputSize);
GLvoid ParseLinkProgram(GLchar *input, GLchar *output, GLint outputSize);
GLvoid ParseCreateShader(GLchar *input, GLchar *output, GLint outputSize);
GLvoid ParseCreateProgram(GLchar *input, GLchar *output, GLint outputSize);
GLvoid ParseUseProgram(GLchar *input, GLchar *output, GLint outputSize);
GLvoid ParseCompileShader(GLchar *input, GLchar *output, GLint outputSize);
GLvoid ParseDetachShader(GLchar *input, GLchar *output, GLint outputSize);
GLvoid ParseDeleteShader(GLchar *input, GLchar *output, GLint outputSize);
GLvoid ParseDeleteProgram(GLchar *input, GLchar *output, GLint outputSize);
GLvoid ParseGetShaderiv(GLchar *input, GLchar *output, GLint outputSize);
GLvoid ParseGetShaderInfoLog(GLchar *input, GLchar *output, GLint outputSize);
GLvoid ParseGetShaderSource(GLchar *input, GLchar *output, GLint outputSize);
GLvoid ParseGetProgramiv(GLchar *input, GLchar *output, GLint outputSize);
GLvoid ParseGetActiveAttrib(GLchar *input, GLchar *output, GLint outputSize);
GLvoid ParseGetAttribLocation(GLchar *input, GLchar *output, GLint outputSize);
GLvoid ParseGetAttachedShaders(GLchar *input, GLchar *output, GLint outputSize);
GLvoid ParseGetProgramInfoLog(GLchar *input, GLchar *output, GLint outputSize);
GLvoid ParseGetShaderPrecisionFormat(GLchar *input, GLchar *output, GLint outputSize);
GLvoid ParseGetUniformfv(GLchar *input, GLchar *output, GLint outputSize);
GLvoid ParseGetUniformiv(GLchar *input, GLchar *output, GLint outputSize);
GLvoid ParseIsShader(GLchar *input, GLchar *output, GLint outputSize);
GLvoid ParseIsProgram(GLchar *input, GLchar *output, GLint outputSize);
GLvoid ParseShaderSource(GLchar *input, GLchar *output, GLint outputSize);
GLvoid ParseUniform1f(GLchar *input, GLchar *output, GLint outputSize);
GLvoid ParseUniform1fv(GLchar *input, GLchar *output, GLint outputSize);
GLvoid ParseUniform1i(GLchar *input, GLchar *output, GLint outputSize);
GLvoid ParseUniform1iv(GLchar *input, GLchar *output, GLint outputSize);
GLvoid ParseUniform2f(GLchar *input, GLchar *output, GLint outputSize);
GLvoid ParseUniform2fv(GLchar *input, GLchar *output, GLint outputSize);
GLvoid ParseUniform2i(GLchar *input, GLchar *output, GLint outputSize);
GLvoid ParseUniform2iv(GLchar *input, GLchar *output, GLint outputSize);
GLvoid ParseUniform3f(GLchar *input, GLchar *output, GLint outputSize);
GLvoid ParseUniform3fv(GLchar *input, GLchar *output, GLint outputSize);
GLvoid ParseUniform3i(GLchar *input, GLchar *output, GLint outputSize);
GLvoid ParseUniform3iv(GLchar *input, GLchar *output, GLint outputSize);
GLvoid ParseUniform4f(GLchar *input, GLchar *output, GLint outputSize);
GLvoid ParseUniform4fv(GLchar *input, GLchar *output, GLint outputSize);
GLvoid ParseUniform4i(GLchar *input, GLchar *output, GLint outputSize);
GLvoid ParseUniform4iv(GLchar *input, GLchar *output, GLint outputSize);
GLvoid ParseUniformMatrix2fv(GLchar *input, GLchar *output, GLint outputSize);
GLvoid ParseUniformMatrix3fv(GLchar *input, GLchar *output, GLint outputSize);
GLvoid ParseUniformMatrix4fv(GLchar *input, GLchar *output, GLint outputSize);
GLvoid ParseValidateProgram(GLchar *input, GLchar *output, GLint outputSize);

// draw states
GLvoid ParseCullFace(GLchar *input, GLchar *output, GLint outputSize);
GLvoid ParseFrontFace(GLchar *input, GLchar *output, GLint outputSize);
GLvoid ParseBlendFunc(GLchar *input, GLchar *output, GLint outputSize);
GLvoid ParseBlendFuncSeparate(GLchar *input, GLchar *output, GLint outputSize);
GLvoid ParseBlendColor(GLchar *input, GLchar *output, GLint outputSize);
GLvoid ParseBlendEquation(GLchar *input, GLchar *output, GLint outputSize);
GLvoid ParseBlendEquationSeparate(GLchar *input, GLchar *output, GLint outputSize);
GLvoid ParseClearColor(GLchar *input, GLchar *output, GLint outputSize);
GLvoid ParseClearDepthf(GLchar *input, GLchar *output, GLint outputSize);
GLvoid ParseClearStencil(GLchar *input, GLchar *output, GLint outputSize);
GLvoid ParseClear(GLchar *input, GLchar *output, GLint outputSize);
GLvoid ParseColorMask(GLchar *input, GLchar *output, GLint outputSize);
GLvoid ParseDepthMask(GLchar *input, GLchar *output, GLint outputSize);
GLvoid ParseStencilMask(GLchar *input, GLchar *output, GLint outputSize);
GLvoid ParseDisable(GLchar *input, GLchar *output, GLint outputSize);
GLvoid ParseEnable(GLchar *input, GLchar *output, GLint outputSize);
GLvoid ParseDepthFunc(GLchar *input, GLchar *output, GLint outputSize);
GLvoid ParseDepthRangef(GLchar *input, GLchar *output, GLint outputSize);
GLvoid ParseIsEnabled(GLchar *input, GLchar *output, GLint outputSize);
GLvoid ParseLineWidth(GLchar *input, GLchar *output, GLint outputSize);
GLvoid ParsePixelStorei(GLchar *input, GLchar *output, GLint outputSize);
GLvoid ParsePolygonOffset(GLchar *input, GLchar *output, GLint outputSize);
GLvoid ParseProgramBinaryOES(GLchar *input, GLchar *output, GLint outputSize);
GLvoid ParseProgramBinary(GLchar *input, GLchar *output, GLint outputSize);
GLvoid ParseScissor(GLchar *input, GLchar *output, GLint outputSize);
GLvoid ParseStencilFunc(GLchar *input, GLchar *output, GLint outputSize);
GLvoid ParseStencilFuncSeparate(GLchar *input, GLchar *output, GLint outputSize);
GLvoid ParseStencilMaskSeparate(GLchar *input, GLchar *output, GLint outputSize);
GLvoid ParseStencilOp(GLchar *input, GLchar *output, GLint outputSize);
GLvoid ParseStencilOpSeparate(GLchar *input, GLchar *output, GLint outputSize);
GLvoid ParseSampleCoverage(GLchar *input, GLchar *output, GLint outputSize);
GLvoid ParseViewport(GLchar *input, GLchar *output, GLint outputSize);

// draw command
GLvoid ParseDrawArrays(GLchar *input, GLchar *output, GLint outputSize);
GLvoid ParseDrawElements(GLchar *input, GLchar *output, GLint outputSize);

#endif /* __PARSE_GLES3_H */