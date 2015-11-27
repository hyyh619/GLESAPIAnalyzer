#include <stdio.h>
#include <stdlib.h>

#include "APIOS.h"
#include "ApiEngine.h"
#include "ApiOESEngine.h"

CApiEngine::CApiEngine()
{
    m_dumpDepthVert = 0;
    m_dumpDepthFrag = 0;
    m_dumpDepthProg = 0;
}

CApiEngine::~CApiEngine()
{
}

GLint CApiEngine::DrawStatesViewportW()
{
    return m_drawStates.viewport[2];
}

GLint CApiEngine::DrawStatesViewportH()
{
    return m_drawStates.viewport[3];
}

GLboolean CApiEngine::IsEnabled(GLenum cap)
{
    return g_opengl->glIsEnabled(cap);
}

GLboolean CApiEngine::DrawStatesIsBindFBO()
{
    if (m_drawStates.fbo)
    {
        return GL_TRUE;
    }
    else
    {
        return GL_FALSE;
    }
}

GLuint CApiEngine::DrawStatesFBO()
{
    return m_drawStates.fbo;
}

GLenum CApiEngine::DrawStatesActiveTexUnit()
{
    return m_drawStates.texUnit;
}

GLuint CApiEngine::CreateProgram(GLuint vertex, GLuint fragment)
{
    GLuint  program     = g_opengl->glCreateProgram();
    GLint   shaderStat  = 0;

    g_opengl->glAttachShader(program, vertex);
    g_opengl->glAttachShader(program, fragment);
    g_opengl->glLinkProgram(program);
    g_opengl->glGetProgramiv(program, GL_LINK_STATUS, &shaderStat);
    if (shaderStat != 1)
    {
        GLchar  *info;
        GLint   writeLen;

        g_opengl->glGetProgramiv(program, GL_INFO_LOG_LENGTH, &shaderStat);
        info = (GLchar*)malloc(shaderStat);
        g_opengl->glGetProgramInfoLog(program, shaderStat, &writeLen, info);
        printf("program: %s\n", info);
    }

    return program;
}

GLvoid CApiEngine::DeleteProgram(GLuint program)
{
    g_opengl->glDeleteProgram(program);
}

GLint CApiEngine::GetAttribLocation(GLuint program, const GLchar *name)
{
    return g_opengl->glGetAttribLocation(program, name);
}

GLint CApiEngine::GetUniformLocation(GLuint program, const GLchar *name)
{
    return g_opengl->glGetUniformLocation(program, name);
}

GLuint CApiEngine::CreateShader(GLenum shaderType, const GLchar *str)
{
    GLuint  shader = 0;
    GLint   shaderStat;

    shader = g_opengl->glCreateShader(shaderType);
    g_opengl->glShaderSource(shader, 1, &str, NULL);
    g_opengl->glCompileShader(shader);
    g_opengl->glGetShaderiv(shader, GL_COMPILE_STATUS, &shaderStat);
    if (shaderStat != 1)
    {
        GLchar  *info;
        GLint   writeLen;

        g_opengl->glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &shaderStat);
        info = (GLchar*)malloc(shaderStat);
        g_opengl->glGetShaderInfoLog(shader, shaderStat, &writeLen, info);
        printf("shader: %s\n", info);
    }

    return shader;
}

GLvoid CApiEngine::DeleteShader(GLuint shader)
{
    g_opengl->glDeleteShader(shader);
}

GLvoid CApiEngine::CheckGLError()
{
    GLenum err = g_opengl->glGetError();

    if (err != GL_NO_ERROR)
    {
        printf("glGetError() 0x%04X\n", err);
    }
}

GLvoid CApiEngine::SaveDrawStates()
{
    g_opengl->glGetIntegerv(GL_CURRENT_PROGRAM, &m_drawStates.program);
    g_opengl->glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &m_drawStates.vbo);
    g_opengl->glGetIntegerv(GL_FRAMEBUFFER_BINDING, &m_drawStates.fbo);
    g_opengl->glGetIntegerv(GL_VIEWPORT, m_drawStates.viewport);
    g_opengl->glGetIntegerv(GL_ACTIVE_TEXTURE, &m_drawStates.texUnit);
    g_opengl->glGetIntegerv(GL_TEXTURE_BINDING_2D, &m_drawStates.tex);

    // Vertex Attribute
    for (GLuint i=0; i<8; i++)
    {
        g_opengl->glGetVertexAttribiv(i, GL_VERTEX_ATTRIB_ARRAY_SIZE, &m_drawStates.vap[i].size);
        g_opengl->glGetVertexAttribiv(i, GL_VERTEX_ATTRIB_ARRAY_STRIDE, &m_drawStates.vap[i].stride);
        g_opengl->glGetVertexAttribiv(i, GL_VERTEX_ATTRIB_ARRAY_TYPE, &m_drawStates.vap[i].type);
        g_opengl->glGetVertexAttribiv(i, GL_VERTEX_ATTRIB_ARRAY_NORMALIZED, &m_drawStates.vap[i].normalize);
        g_opengl->glGetVertexAttribiv(i, GL_VERTEX_ATTRIB_ARRAY_BUFFER_BINDING, &m_drawStates.vap[i].vbo);
        g_opengl->glGetVertexAttribPointerv(i, GL_VERTEX_ATTRIB_ARRAY_POINTER, &m_drawStates.vap[i].pointer);
    }

    m_drawStates.depthTest      = g_opengl->glIsEnabled(GL_DEPTH_TEST);
    m_drawStates.stencilTest    = g_opengl->glIsEnabled(GL_STENCIL_TEST);
    m_drawStates.blend          = g_opengl->glIsEnabled(GL_BLEND);
}

GLvoid CApiEngine::LoadDrawStates()
{
    g_opengl->glUseProgram(m_drawStates.program);

    g_opengl->glBindFramebuffer(GL_FRAMEBUFFER, m_drawStates.fbo);
    g_opengl->glActiveTexture(m_drawStates.texUnit);
    g_opengl->glBindTexture(GL_TEXTURE_2D, m_drawStates.tex);

    // Vertex Attribute
    for (GLuint i=0; i<8; i++)
    {
        g_opengl->glBindBuffer(GL_ARRAY_BUFFER, m_drawStates.vap[i].vbo);
        g_opengl->glVertexAttribPointer(
            i,
            m_drawStates.vap[i].size,
            m_drawStates.vap[i].type,
            m_drawStates.vap[i].normalize,
            m_drawStates.vap[i].stride,
            m_drawStates.vap[i].pointer);
    }

    g_opengl->glBindBuffer(GL_ARRAY_BUFFER, m_drawStates.vbo);

    if (m_drawStates.depthTest)
        g_opengl->glEnable(GL_DEPTH_TEST);

    if (m_drawStates.stencilTest)
        g_opengl->glEnable(GL_STENCIL_TEST);

    if (m_drawStates.blend)
        g_opengl->glEnable(GL_BLEND);
}

GLvoid CApiEngine::CreateDumpDepthProgram()
{
    const GLchar *strVertex = "\n\
        attribute vec4 a_vertex;\n\
        attribute vec2 a_texcoord;\n\
        varying vec4 v_texcoord;\n\
        void main()\n\
        {\n\
            gl_Position = a_vertex;\n\
            v_texcoord = vec4(a_texcoord, 0.0, 1.0);\n\
        }";

    const GLchar *strFragment = "\n\
        uniform sampler2D   sShadow;\n\
        varying vec4        v_texcoord;\n\
        void main ()\n\
        {\n\
            float depth = texture2DProj(sShadow, v_texcoord).r;\n\
            vec3 color = vec3(pow(depth, 128.0));\n\
            //gl_FragColor = vec4(color, 1.0);\n\
            gl_FragColor = vec4(depth, depth, depth, 1.0);\n\
        }";

    GLuint  vertex      = CreateShader(GL_VERTEX_SHADER, strVertex);
    GLuint  fragment    = CreateShader(GL_FRAGMENT_SHADER, strFragment);
    GLuint  program     = CreateProgram(vertex, fragment);

    m_dumpDepthVert = vertex;
    m_dumpDepthFrag = fragment;
    m_dumpDepthProg = program;
}

GLvoid CApiEngine::DeleteDumpDepthProgram()
{
    DeleteShader(m_dumpDepthVert);
    DeleteShader(m_dumpDepthFrag);
    DeleteProgram(m_dumpDepthProg);
}

GLvoid CApiEngine::UseDumpDepthProgram()
{
    g_opengl->glUseProgram(m_dumpDepthProg);
}

GLuint CApiEngine::GetDumpDepthProgram()
{
    return m_dumpDepthProg;
}

GLvoid CApiEngine::CheckFramebufferStatus(GLenum target)
{
    GLenum err = g_opengl->glCheckFramebufferStatus(target);
    if (err != GL_FRAMEBUFFER_COMPLETE)
    {
        printf("frame: 0x%04X\n", err);
    }
}

GLuint CApiEngine::GenFramebuffer(GLenum target)
{
    GLuint fbo;
    g_opengl->glGenFramebuffers(1, &fbo);
    g_opengl->glBindFramebuffer(target, fbo);
    return fbo;
}

GLvoid CApiEngine::FramebufferTexture2D(GLenum target, GLuint fbo, GLenum attachment, GLenum textarget, GLuint texture, GLint level)
{
    g_opengl->glBindFramebuffer(target, fbo);
    g_opengl->glFramebufferTexture2D(target, attachment, textarget, texture, level);
    CheckFramebufferStatus(target);
}

GLvoid CApiEngine::BlitFramebuffer(GLint width, GLint height, GLbitfield mask, GLenum filter)
{
#ifdef OES3_API
    g_opengl->glBlitFramebuffer(0, 0, width, height, 0, 0, width, height, mask, filter);
#elif defined(__APPLE__)
    //g_opengl->glBlitFramebufferANGLE(0, 0, width, height, 0, 0, width, height, mask, filter);
#elif defined(WITH_TCG_TRACER_OES2)
#else
    g_opengl->glBlitFramebufferANGLE(0, 0, width, height, 0, 0, width, height, mask, filter);
#endif
}

GLvoid CApiEngine::BindFramebuffer(GLenum target, GLuint fbo)
{
    g_opengl->glBindFramebuffer(target, fbo);
}

GLvoid CApiEngine::ActiveTexture(GLenum unit)
{
    g_opengl->glActiveTexture(unit);
}

GLuint CApiEngine::GenTexture(GLenum target)
{
    GLuint tex;
    g_opengl->glGenTextures(1, &tex);
    g_opengl->glBindTexture(target, tex);
    CheckGLError();
    return tex;
}

GLvoid CApiEngine::BindTexture(GLenum target, GLuint tex)
{
    g_opengl->glBindTexture(target, tex);
}

GLvoid CApiEngine::TexImage2D(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid *pixels)
{
    g_opengl->glTexImage2D(target, level, internalformat, width, height, 0, format, type, pixels);
    CheckGLError();
}

GLvoid CApiEngine::TexParameters(GLenum target, GLenum minFilter, GLenum magFilter, GLenum wrapR, GLenum wrapS, GLenum wrapT, GLenum cmpMode)
{
    g_opengl->glTexParameteri(target, GL_TEXTURE_MAG_FILTER, magFilter);
    g_opengl->glTexParameteri(target, GL_TEXTURE_MIN_FILTER, minFilter);

#ifdef OES3_API
    g_opengl->glTexParameteri(target, GL_TEXTURE_WRAP_R, wrapR);
#else
    g_opengl->glTexParameteri(target, GL_TEXTURE_WRAP_R_OES, wrapR);
#endif
    g_opengl->glTexParameteri(target, GL_TEXTURE_WRAP_S, wrapS);
    g_opengl->glTexParameteri(target, GL_TEXTURE_WRAP_T, wrapT);

#ifdef OES3_API
    g_opengl->glTexParameteri(target, GL_TEXTURE_COMPARE_MODE, cmpMode);
#else
    g_opengl->glTexParameteri(target, GL_TEXTURE_COMPARE_MODE_EXT, cmpMode);
#endif

    CheckGLError();
}