#ifndef __API_GLES3_CONTEXT_H
#define __API_GLES3_CONTEXT_H

#include <GLES3/gl31.h>
#include <GLES3/gl3ext.h>
#include "GLESGlobal.h"
#include "GLSLShader.h"
#include "GLES3Tex.h"
#include "GLES3VertexBuffer.h"
#include "GLES3Framebuffer.h"
#include "Analyzer.h"
#include "ApiDumpDraw.h"
#include "ApiDumpInfo.h"


#define API_RENDER_THREAD_NUM   2
#if API_DUMP
#define CURRENT_CONTEXT1()      g_GLES3Context[0]
#define VERTEX_ATTRIB_BITS      g_GLES3Context[0].GetVertexAttribBits()
#define ARRAY_BUFFER            g_GLES3Context[0].GetArrayBuffer()
#define ELEMENT_ARRAY_BUFFER    g_GLES3Context[0].GetElementArrayBuffer()
#define TRANS_FEEDBACK_BUFFER   g_GLES3Context[0].GetTransFeedbackBuffer()
#define VERTEX_ATTRIB_PTR(i)    g_GLES3Context[0].GetVertexAttribPointer(i)
#define GET_VBO(name)           g_GLES3Context[0].FindBufObj(name)
#else
#define CURRENT_CONTEXT1()      (*(_GetThreadDataES3()))
#define VERTEX_ATTRIB_BITS      (*(_GetThreadDataES3())).GetVertexAttribBits()
#define ARRAY_BUFFER            (*(_GetThreadDataES3())).GetArrayBuffer()
#define ELEMENT_ARRAY_BUFFER    (*(_GetThreadDataES3())).GetElementArrayBuffer()
#define TRANS_FEEDBACK_BUFFER   (*(_GetThreadDataES3())).GetTransFeedbackBuffer()
#define VERTEX_ATTRIB_PTR(i)    (*(_GetThreadDataES3())).GetVertexAttribPointer(i)
#define GET_VBO(name)           (*(_GetThreadDataES3())).FindBufObj(name)
#endif

struct stEvent;

class CGLES3Context
{
public:
    CGLES3Context();
    ~CGLES3Context();

    GLvoid                  Initialize(const GLchar *filePath);
    GLvoid                  Release();
    GLuint                  GetEventSequence();
    GLvoid                  DumpDraw(GLESAPIIndex name);
    GLboolean               IsDrawCommand(GLESAPIIndex name);
    GLvoid                  DumpFrame();

    CAnalyzer               *m_pAnalyzer;
    CApiDumpDraw            *m_pDumpDraw;
    CDumpInfo               *m_pDumpInfo;

public:
    GLuint                  GetVertexAttribBits();
    GLuint                  GetArrayBuffer();
    GLuint                  GetElementArrayBuffer();
    GLuint                  GetTransFeedbackBuffer();
    GLuint                  GetUniformBuffer();
    stVertAttrPtr*          GetVertexAttribPointer(GLint index);

public: /* vertex buffer */
    GLvoid                  ApiGenBuffers(GLsizei n, GLuint *buffers);
    GLvoid                  ApiDeleteBuffers(GLsizei n, const GLuint *buffers);
    GLvoid                  ApiBindBuffer(GLenum target, GLuint buffer);
    GLvoid                  ApiBindBufferBase(GLenum target, GLuint index, GLuint buffer);
    GLvoid                  ApiBindBufferRange(GLenum target, GLuint index, GLuint buffer, GLintptr offset, GLsizeiptr size);
    GLvoid                  ApiBufferData(GLenum target, GLsizeiptr size, const void *data, GLenum usage);
    GLvoid                  ApiBufferSubData(GLenum target, GLintptr offset, GLsizeiptr size, const void *data);
    GLvoid                  ApiDeleteVertexArrays(GLsizei n, const GLuint *arrays);
    GLvoid                  ApiGenVertexArrays(GLsizei n, GLuint *arrays);
    GLvoid                  ApiBindVertexArray(GLuint array);
    GLvoid                  ApiBindTransformFeedback(GLenum target, GLuint id);
    GLvoid                  ApiDeleteTransformFeedbacks(GLsizei n, const GLuint *ids);
    GLvoid                  ApiGenTransformFeedbacks(GLsizei n, GLuint *ids);
    GLboolean               ApiIsTransformFeedback(GLuint id);
    GLvoid                  ApiBeginTransformFeedback(GLenum primitiveMode);
    GLvoid                  ApiEndTransformFeedback(void);
    GLvoid                  ApiDisableVertexAttribArray(GLuint index);
    GLvoid                  ApiEnableVertexAttribArray(GLuint index);
    GLvoid                  ApiVertexAttrib1f(GLuint index, GLfloat x);
    GLvoid                  ApiVertexAttrib1fv(GLuint index, const GLfloat *v);
    GLvoid                  ApiVertexAttrib2f(GLuint index, GLfloat x, GLfloat y);
    GLvoid                  ApiVertexAttrib2fv(GLuint index, const GLfloat *v);
    GLvoid                  ApiVertexAttrib3f(GLuint index, GLfloat x, GLfloat y, GLfloat z);
    GLvoid                  ApiVertexAttrib3fv(GLuint index, const GLfloat *v);
    GLvoid                  ApiVertexAttrib4f(GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
    GLvoid                  ApiVertexAttrib4fv(GLuint index, const GLfloat *v);
    GLvoid                  ApiVertexAttribPointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void *pointer);
    GLvoid                  ApiVertexAttribIPointer(GLuint index, GLint size, GLenum type, GLsizei stride, const void *pointer);
    GLvoid                  ApiGetBufferParameteriv(GLenum target, GLenum pname, GLint *params);
    GLvoid                  ApiGetVertexAttribfv(GLuint index, GLenum pname, GLfloat *params);
    GLvoid                  ApiGetVertexAttribiv(GLuint index, GLenum pname, GLint *params);
    GLvoid                  ApiGetVertexAttribPointerv(GLuint index, GLenum pname, void **pointer);

    /* Internal function */
    CBufObj*                FindBufObj(GLuint bufId);
    CBufObj*                FindBufObjByTarget(GLenum target);
    GLuint                  GetBufIdByTarget(GLenum target);
    CBufObj*                CreateBufObj(GLuint bufId);
    GLvoid                  DeleteBufObj(GLuint bufId);
    GLvoid                  InsertBufObj(GLuint name, CBufObj *p);
    GLvoid                  SetBufObj(GLenum target, GLuint bufId);
    GLvoid                  SaveVBO(GLenum target, GLint size, GLchar *data);
    GLvoid                  DeleteTransFeedbackObj(GLuint bufId);
    CTransFeedback*         CreateTransFeedbackObj(GLuint bufId);
    CTransFeedback*         FindTransFeedbackObj();
    GLvoid                  DeleteVertexArrayObj(GLuint bufId);
    CVertexArrayObj*        CreateVertexArrayObj(GLuint bufId);
    GLvoid                  SetVertexAttrib1234f(GLuint index, GLuint count, GLfloat f1, GLfloat f2, GLfloat f3, GLfloat f4);
    GLvoid                  SetVertexAttrib1234fv(GLuint index, GLuint count, const GLfloat *pData);

    /* data buffer */
    BufferMap               bufferMap;
    GLuint                  arrayBuffer;
    GLuint                  elementArrayBuffer;
    GLuint                  uniformBuffer;
    GLuint                  transFeedbackBuffer;

    /* vertex array object */
    VaoMap                  vaoMap;
    TransFeedbackMap        tfbMap;
    GLuint                  vertexArray;
    GLuint                  transFeedback;

    /* vertex array */
    stVertAttrPtr           vertexAttribPointer[API_VERTEX_ATTRIBUTES_NUM];
    GLuint                  vertexAttribBits;

public: /* shader */
    GLuint                  ApiCreateShader(GLenum type, GLuint shader);
    GLvoid                  ApiShaderSource(GLuint shader, GLsizei count, const GLchar *const*string, const GLint *length);
    GLvoid                  ApiDeleteShader(GLuint shader);
    GLuint                  ApiCreateProgram(GLuint program);
    GLvoid                  ApiUseProgram(GLuint program);
    GLvoid                  ApiAttachShader(GLuint program, GLuint shader);
    GLint                   ApiGetUniformLocation(GLuint program, const GLchar *name, GLint oldLoc);
    GLvoid                  ApiLinkProgram(GLuint program);
    GLvoid                  ApiCompileShader(GLuint shader);
    GLvoid                  ApiDetachShader(GLuint program, GLuint shader);
    GLvoid                  ApiDeleteProgram(GLuint program);
    GLvoid                  ApiGetAttribLocation(GLuint program, const GLchar *name, GLint oldLoc);
    GLvoid                  ApiBindAttribLocation(GLuint program, GLint loc, const GLchar *name);
    GLvoid                  ApiProgramBinary(GLuint program, GLenum binaryFormat, const void *binary, GLsizei length);
    GLvoid                  ApiUniform1f(GLint location, GLfloat v0);
    GLvoid                  ApiUniform1fv(GLint location, GLsizei count, const GLfloat *value);
    GLvoid                  ApiUniform1i(GLint location, GLint v0);
    GLvoid                  ApiUniform1iv(GLint location, GLsizei count, const GLint *value);
    GLvoid                  ApiUniform2f(GLint location, GLfloat v0, GLfloat v1);
    GLvoid                  ApiUniform2fv(GLint location, GLsizei count, const GLfloat *value);
    GLvoid                  ApiUniform2i(GLint location, GLint v0, GLint v1);
    GLvoid                  ApiUniform2iv(GLint location, GLsizei count, const GLint *value);
    GLvoid                  ApiUniform3f(GLint location, GLfloat v0, GLfloat v1, GLfloat v2);
    GLvoid                  ApiUniform3fv(GLint location, GLsizei count, const GLfloat *value);
    GLvoid                  ApiUniform3i(GLint location, GLint v0, GLint v1, GLint v2);
    GLvoid                  ApiUniform3iv(GLint location, GLsizei count, const GLint *value);
    GLvoid                  ApiUniform4f(GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3);
    GLvoid                  ApiUniform4fv(GLint location, GLsizei count, const GLfloat *value);
    GLvoid                  ApiUniform4i(GLint location, GLint v0, GLint v1, GLint v2, GLint v3);
    GLvoid                  ApiUniform4iv(GLint location, GLsizei count, const GLint *value);
    GLvoid                  ApiUniformMatrix2fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
    GLvoid                  ApiUniformMatrix3fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
    GLvoid                  ApiUniformMatrix4fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
    GLvoid                  ApiGetActiveAttrib(GLuint program, GLuint index, GLsizei bufSize, GLsizei *length, GLint *size, GLenum *type, GLchar *name);
    GLvoid                  ApiGetActiveUniform(GLuint program, GLuint index, GLsizei bufSize, GLsizei *length, GLint *size, GLenum *type, GLchar *name);
    GLvoid                  ApiGetAttachedShaders(GLuint program, GLsizei maxCount, GLsizei *count, GLuint *shaders);
    GLvoid                  ApiGetProgramiv(GLuint program, GLenum pname, GLint *params);
    GLvoid                  ApiGetProgramInfoLog(GLuint program, GLsizei bufSize, GLsizei *length, GLchar *infoLog);
    GLvoid                  ApiGetShaderiv(GLuint shader, GLenum pname, GLint *params);
    GLvoid                  ApiGetShaderInfoLog(GLuint shader, GLsizei bufSize, GLsizei *length, GLchar *infoLog);
    GLvoid                  ApiGetShaderPrecisionFormat(GLenum shadertype, GLenum precisiontype, GLint *range, GLint *precision);
    GLvoid                  ApiGetShaderSource(GLuint shader, GLsizei bufSize, GLsizei *length, GLchar *source);
    GLvoid                  ApiGetUniformfv(GLuint program, GLint location, GLfloat *params);
    GLvoid                  ApiGetUniformiv(GLuint program, GLint location, GLint *params);
    GLvoid                  ApiShaderBinary(GLsizei count, const GLuint *shaders, GLenum binaryformat, const void *binary, GLsizei length);
    GLvoid                  ApiValidateProgram(GLuint program);

    /* Internal Functions */
    CShaderObj*             FindShader(GLuint shader);
    CProgramObj*            FindProgram(GLuint program);
    GLboolean               LinkProgram(CProgramObj *pProgram);
    GLuint                  CompileShader(CShaderObj *pShader);
    GLvoid                  CreateShaderObject(GLenum type, GLuint shaderId);
    GLvoid                  DeleteShaderObject(GLuint shaderId);
    GLvoid                  ClearProgram(CProgramObj *pProgram);
    GLvoid                  CreateProgramObject(GLuint progId);
    GLvoid                  DeleteProgramObject(GLuint progId);

    ShaderMap               shaderMap;
    ProgramMap              programMap;
    GLuint                  curProgram;
    GLuint                  curShader;
    const GLchar            **curShaderSrc;
    GLuint                  curShaderCount;

public: /* texture */
    GLvoid                  ApiActiveTexture(GLenum texture);
    GLvoid                  ApiDeleteTextures(GLsizei n, const GLuint *textures);
    GLvoid                  ApiBindTexture(GLenum target, GLuint texture);
    GLvoid                  ApiGenTextures(GLsizei n, GLuint *textures);
    GLvoid                  ApiGenerateMipmap(GLenum target);
    GLvoid                  ApiTexImage2D(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const void *pixels);
    GLvoid                  ApiTexSubImage2D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const void *pixels);
    GLvoid                  ApiTexImage3D(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLenum format, GLenum type, const void *pixels);
    GLvoid                  ApiTexSubImage3D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const void *pixels);
    GLvoid                  ApiTexStorage2D(GLenum target, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height);
    GLvoid                  ApiTexStorage3D(GLenum target, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth);
    GLvoid                  ApiCompressedTexImage2D(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLsizei imageSize, const void *data);
    GLvoid                  ApiCompressedTexSubImage2D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLsizei imageSize, const void *data);
    GLvoid                  ApiTexParameterf(GLenum target, GLenum pname, GLfloat param);
    GLvoid                  ApiTexParameterfv(GLenum target, GLenum pname, const GLfloat *params);
    GLvoid                  ApiTexParameteri(GLenum target, GLenum pname, GLint param);
    GLvoid                  ApiTexParameteriv(GLenum target, GLenum pname, const GLint *params);
    GLvoid                  ApiCopyTexImage2D(GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLsizei height, GLint border);
    GLvoid                  ApiCopyTexSubImage2D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height);
    GLvoid                  ApiGetTexParameterfv(GLenum target, GLenum pname, GLfloat *params);
    GLvoid                  ApiGetTexParameteriv(GLenum target, GLenum pname, GLint *params);

    /* Internal Functions */
    CTexObj*                FindTexObj(GLuint texture);
    CTexObj*                CreateTextureObject(GLuint texId);
    GLvoid                  DeleteTextureObject(GLuint texId);
    CTexObj*                GetTexObjByUnit(GLuint unit, GLenum target);
    CTexObj*                GetActiveTexObj(GLenum target);
    CTexObj*                GetTexObjByName(GLuint name);

    GLint                   activeTexUnit;
    TextureMap              textureMap;
    CTexObj                 *texture2D[API_TEX_UNIT_NUM];
    CTexObj                 *texture3D[API_TEX_UNIT_NUM];
    CTexObj                 *textureCube[API_TEX_UNIT_NUM];
    CTexObj                 *textureExternal[API_TEX_UNIT_NUM];
    GLuint                  textureSampler[API_TEX_UNIT_NUM][API_TEXTURE_TYPE_NUM];
    UINT                    maxAniso;
    UINT                    maxTextureWidth;
    UINT                    maxTextureHeight;

public: /* FBO and RBO */
    GLvoid                  ApiBindFramebuffer(GLenum target, GLuint framebuffer);
    GLvoid                  ApiBindRenderbuffer(GLenum target, GLuint renderbuffer);
    GLvoid                  ApiDeleteFramebuffers(GLsizei n, const GLuint *framebuffers);
    GLvoid                  ApiDeleteRenderbuffers(GLsizei n, const GLuint *renderbuffers);
    GLvoid                  ApiGenFramebuffers(GLsizei n, GLuint *framebuffers);
    GLvoid                  ApiGenRenderbuffers(GLsizei n, GLuint *renderbuffers);
    GLvoid                  ApiRenderbufferStorage(GLenum target, GLenum internalformat, GLsizei width, GLsizei height);
    GLvoid                  ApiFramebufferRenderbuffer(GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer);
    GLvoid                  ApiFramebufferTexture2D(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level);
    GLvoid                  ApiCheckFramebufferStatus(GLenum target, GLenum result);
    GLvoid                  ApiGetFramebufferAttachmentParameteriv(GLenum target, GLenum attachment, GLenum pname, GLint *params);
    GLvoid                  ApiGetRenderbufferParameteriv(GLenum target, GLenum pname, GLint *params);

    // Internal functions
    CFramebufferObj*        CreateFramebufferObj(GLuint fbo);
    GLvoid                  DeleteFramebufferObj(GLuint fbo);
    CRenderbufferObj*       CreateRenderbufferObj(GLuint fbo);
    GLvoid                  DeleteRenderbufferObj(GLuint rbo);
    CFramebufferObj*        GetCurrentFBO();
    CRenderbufferObj*       GetRBOByName(GLuint name);

    /* FBO */
    FboMap                  fboMap;
    GLuint                  currentFBO;

    /* RBO */
    RboMap                  rboMap;
    GLuint                  currentRBO;

public: /* draw states */
    GLvoid                  ApiDisable(GLenum cap);
    GLvoid                  ApiEnable(GLenum cap);
    GLvoid                  SetCapability(GLenum cap, GLboolean flag);
    GLboolean               rasterDiscard;
    GLboolean               ditherEnable;               /* dither */

    // PA cull face
    GLvoid                  ApiFrontFace(GLenum mode);
    GLvoid                  ApiCullFace(GLenum mode);
    GLboolean               cullEnable;
    GLenum                  cullMode;
    GLenum                  cullFront;

    // PE blend
    GLvoid                  ApiBlendColor(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
    GLvoid                  ApiBlendEquation(GLenum mode);
    GLvoid                  ApiBlendEquationSeparate(GLenum modeRGB, GLenum modeAlpha);
    GLvoid                  ApiBlendFunc(GLenum sfactor, GLenum dfactor);
    GLvoid                  ApiBlendFuncSeparate(GLenum sfactorRGB, GLenum dfactorRGB, GLenum sfactorAlpha, GLenum dfactorAlpha);
    GLboolean               blendEnable;
    GLenum                  blendFuncSourceRGB, blendFuncSourceAlpha;
    GLenum                  blendFuncTargetRGB, blendFuncTargetAlpha;
    GLenum                  blendModeRGB, blendModeAlpha;
    GLclampf                blendColorRed, blendColorGreen, blendColorBlue;
    GLclampf                blendColorAlpha;

    // PE depth
    GLvoid                  ApiDepthFunc(GLenum func);
    GLvoid                  ApiDepthRangef(GLfloat n, GLfloat f);
    GLboolean               depthTest;
    GLenum                  depthFunc;
    GLclampf                depthNear;
    GLclampf                depthFar;

    // Clear
    GLvoid                  ApiClearColor(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
    GLvoid                  ApiClearDepthf(GLfloat d);
    GLvoid                  ApiClearStencil(GLint s);
    GLint                   clearStencil;
    GLclampf                clearRed;
    GLclampf                clearGreen;
    GLclampf                clearBlue;
    GLclampf                clearAlpha;
    GLclampf                clearDepth;

    // mask
    GLvoid                  ApiColorMask(GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha);
    GLvoid                  ApiDepthMask(GLboolean flag);
    GLuint                  colorRedMask;
    GLuint                  colorGreenMask;
    GLuint                  colorBlueMask;
    GLuint                  colorAlphaMask;
    GLboolean               depthMask;

    // line width
    GLvoid                  ApiLineWidth(GLfloat width);
    GLfloat                 lineWidth;

    // Pixel store
    GLvoid                  ApiPixelStorei(GLenum pname, GLint param);
    /* Pixel alignment for glReadPixels, glTexImage2D and glTexSubImage2D. */
    GLint                   unpackAlignment;
    GLint                   packAlignment;

    // RA polygon offset
    GLvoid                  ApiPolygonOffset(GLfloat factor, GLfloat units);
    GLboolean               offsetEnable;
    GLfloat                 offsetFactor;
    GLfloat                 offsetUnits;

    // PA scissor
    GLvoid                  ApiScissor(GLint x, GLint y, GLsizei width, GLsizei height);
    GLboolean               scissorEnable;
    GLint                   scissorX;
    GLint                   scissorY;
    GLuint                  scissorWidth;
    GLuint                  scissorHeight;

    // PE stencil
    GLvoid                  ApiStencilFunc(GLenum func, GLint ref, GLuint mask);
    GLvoid                  ApiStencilFuncSeparate(GLenum face, GLenum func, GLint ref, GLuint mask);
    GLvoid                  ApiStencilMask(GLuint mask);
    GLvoid                  ApiStencilMaskSeparate(GLenum face, GLuint mask);
    GLvoid                  ApiStencilOp(GLenum fail, GLenum zfail, GLenum zpass);
    GLvoid                  ApiStencilOpSeparate(GLenum face, GLenum sfail, GLenum dpfail, GLenum dppass);
    GLboolean               stencilEnable;
    GLuint                  stencilRefFront;
    GLuint                  stencilRefBack;
    GLenum                  stencilFuncFront;
    GLenum                  stencilFuncBack;
    GLuint                  stencilMaskFront;
    GLuint                  stencilMaskBack;
    GLenum                  stencilOpFailFront;
    GLenum                  stencilOpFailBack;
    GLenum                  stencilOpDepthFailFront;
    GLenum                  stencilOpDepthFailBack;
    GLenum                  stencilOpDepthPassFront;
    GLenum                  stencilOpDepthPassBack;
    GLuint                  stencilWriteMask;

    // Sample coverage and sample alpha
    GLvoid                  ApiSampleCoverage(GLfloat value, GLboolean invert);
    GLboolean               sampleAlphaToCoverage;
    GLboolean               sampleCoverage;
    GLfloat                 sampleCoverageValue;
    GLboolean               sampleCoverageInvert;

    // PA viewport
    GLvoid                  ApiViewport(GLint x, GLint y, GLsizei width, GLsizei height);
    GLint                   viewportX;
    GLint                   viewportY;
    GLuint                  viewportWidth;
    GLuint                  viewportHeight;

    // Query
    GLvoid                  ApiGetBooleanv(GLenum pname, GLboolean *data);
    GLvoid                  ApiGetFloatv(GLenum pname, GLfloat *data);
    GLvoid                  ApiGetIntegerv(GLenum pname, GLint *data);
    GLvoid                  ApiGetString(GLenum name, const GLubyte *str);
    GLvoid                  ApiIsBuffer(GLuint buffer, GLboolean res);
    GLvoid                  ApiIsEnabled(GLenum cap, GLboolean res);
    GLvoid                  ApiIsFramebuffer(GLuint framebuffer, GLboolean res);
    GLvoid                  ApiIsProgram(GLuint program, GLboolean res);
    GLvoid                  ApiIsRenderbuffer(GLuint renderbuffer, GLboolean res);
    GLvoid                  ApiIsShader(GLuint shader, GLboolean res);
    GLvoid                  ApiIsTexture(GLuint texture, GLboolean res);

    // Special
    GLvoid                  ApiGetError(GLenum err);
    GLvoid                  ApiFinish();
    GLvoid                  ApiFlush();
    GLvoid                  ApiHint(GLenum target, GLenum mode);
    GLvoid                  ApiReadPixels(GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, void *pixels);
    GLvoid                  ApiReadBuffer(GLenum mode);

    // Draw command
public:
    GLvoid                  ApiClear(GLbitfield mask);
    GLbitfield              clearBitsMask;

    GLvoid                  ApiDrawArrays(GLenum mode, GLint first, GLsizei count);
    GLvoid                  ApiDrawElements(GLenum mode, GLsizei count, GLenum type, const GLvoid* indices);
    GLenum                  drawMode;
    GLint                   drawFirst;
    GLsizei                 drawCount;
    GLenum                  drawIndicesType;
    GLvoid                  *drawIndices;
    GLuint                  drawVertexCount;

    /* draw statistic */
    GLuint                  nClear;
    GLuint                  nDrawArrays;
    GLuint                  nDrawElements;
    GLuint                  nDrawTotal;
    GLuint                  nFrameCount;

public: // OES2 extension
    GLvoid                  ApiBlitFramebufferEXT(GLint srcX0, GLint srcY0, GLint srcX1, GLint srcY1, GLint dstX0, GLint dstY0, GLint dstX1, GLint dstY1, GLbitfield mask, GLenum filter);
};

extern CGLES3Context g_GLES3Context[API_RENDER_THREAD_NUM];

#endif /* __API_GLES3_CONTEXT_H */