#ifndef __API_ENGINE_H
#define __API_ENGINE_H

#include <EGL/egl.h>
#include <EGL/eglext.h>
#ifdef OES3_API
#include <GLES3/gl3.h>
#else
#include <GLES2/gl2.h>
#define GL_GLEXT_PROTOTYPES 1
#include <GLES2/gl2ext.h>
#endif

struct stApiVAP
{
    GLint       size;
    GLint       stride;
    GLint       type;
    GLvoid*     pointer;
    GLint       normalize;
    GLint       vbo;
};

struct stApiDrawStates
{
    GLint       program;
    GLint       vbo;
    GLint       fbo;
    GLint       viewport[4];
    GLint       texUnit;
    GLint       tex;
    GLboolean   depthTest;
    GLboolean   stencilTest;
    GLboolean   blend;

    stApiVAP    vap[32];
};

class CApiEngine
{
public:
    CApiEngine();
    ~CApiEngine();

public:
    // Engine
    GLuint          CreateProgram(GLuint vertex, GLuint fragment);
    GLvoid          DeleteProgram(GLuint program);
    GLuint          CreateShader(GLenum shaderType, const GLchar *str);
    GLvoid          DeleteShader(GLuint shader);
    GLvoid          SaveDrawStates();
    GLvoid          LoadDrawStates();
    GLvoid          CheckGLError();
    GLint           GetAttribLocation(GLuint program, const GLchar *name);
    GLint           GetUniformLocation(GLuint program, const GLchar *name);

    // FBO
    GLvoid          CheckFramebufferStatus(GLenum target);
    GLuint          GenFramebuffer(GLenum target);
    GLvoid          BindFramebuffer(GLenum target, GLuint fbo);
    GLvoid          FramebufferTexture2D(GLenum target, GLuint fbo, GLenum attachment, GLenum textarget, GLuint texture, GLint level);
    GLvoid          BlitFramebuffer(GLint width, GLint height, GLbitfield mask, GLenum filter);

    // Tex
    GLvoid          ActiveTexture(GLenum unit);
    GLuint          GenTexture(GLenum target);
    GLvoid          BindTexture(GLenum target, GLuint tex);
    GLvoid          TexImage2D(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid *pixels);
    GLvoid          TexParameters(GLenum target, GLenum minFilter, GLenum magFilter, GLenum wrapR, GLenum wrapS, GLenum wrapT, GLenum cmpMode);

    // saved draw states
    GLboolean       DrawStatesIsBindFBO();
    GLuint          DrawStatesFBO();
    GLint           DrawStatesViewportW();
    GLint           DrawStatesViewportH();
    GLenum          DrawStatesActiveTexUnit();

    // current draw states
    GLboolean       IsEnabled(GLenum cap);

    // dump depth
    GLvoid          CreateDumpDepthProgram();
    GLvoid          DeleteDumpDepthProgram();
    GLvoid          UseDumpDepthProgram();
    GLuint          GetDumpDepthProgram();

public:
    // Restore draw states
    stApiDrawStates m_drawStates;

    // Dump depth
    GLuint          m_dumpDepthVert;
    GLuint          m_dumpDepthFrag;
    GLuint          m_dumpDepthProg;
};

#endif /* __API_ENGINE_H */