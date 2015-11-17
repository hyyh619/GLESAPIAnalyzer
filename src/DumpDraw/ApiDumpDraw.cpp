#include <stdio.h>
#include <stdlib.h>

#include "utils.h"
#include "APIOS.h"
#include "PngInterface.h"
#include "bitmap.h"
#include "ApiDumpDraw.h"
#include "Analyzer.h"


CApiDumpDraw::CApiDumpDraw(CAnalyzer *pAnalyzer)
{
    m_options.dumpFrameBmp  = GL_FALSE;
    m_options.dumpFramePng  = GL_FALSE;
    m_options.dumpDraw      = GL_FALSE;
    m_options.dumpDrawPng   = GL_FALSE;
    m_options.dumpDrawStart = 26;
    m_options.dumpDrawEnd   = 26;
    m_options.dumpDepth     = GL_FALSE;

    m_pAnalyzer             = pAnalyzer;
    m_nCurrentFrameNum      = 1;
    m_nLoop                 = 1;

    m_dumpDepthVert         = 0;
    m_dumpDepthFrag         = 0;
    m_dumpDepthProg         = 0;
}

CApiDumpDraw::~CApiDumpDraw()
{
    //m_apiEngine.DeleteDumpDepthProgram();
}

GLvoid CApiDumpDraw::DumpPng(GLchar *fileName)
{
    GLint       width   = 0;
    GLint       height  = 0;
    GLint       x       = 0;
    GLint       y       = 0;
    GLubyte     *data   = NULL;
    GLubyte     *pData  = NULL;
    pic_data    pic_d;
    GLubyte     *rgba[4];
    GLubyte     *r,*g,*b,*a;
    GLint       viewport[4];

    glGetIntegerv(GL_VIEWPORT, viewport);
    x       = viewport[0];
    y       = viewport[1];
    width   = viewport[2];
    height  = viewport[3];
    data    = (GLubyte *)malloc(4*width*height);
    pData   = data;

    glReadPixels(0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, data);

    pic_d.width     = width;
    pic_d.height    = height;
    pic_d.bit_depth = 8;
    pic_d.flag      = HAVE_ALPHA;


    r = rgba[0] = (GLubyte*)malloc(width*height);
    g = rgba[1] = (GLubyte*)malloc(width*height);
    b = rgba[2] = (GLubyte*)malloc(width*height);
    a = rgba[3] = (GLubyte*)malloc(width*height);
    r = rgba[0]; g = rgba[1]; b = rgba[2]; a = rgba[3];

    for(GLint i = 0; i < width*height; i++)
    {
        *r = pData[0];
        *g = pData[1];
        *b = pData[2];
        *a = pData[3];
        r++;
        g++;
        b++;
        a++;
        pData += 4;
    }

    pic_d.rgba = rgba;
    write_png_file(fileName, &pic_d);

    free(data);
    free(rgba[0]);
    free(rgba[1]);
    free(rgba[2]);
    free(rgba[3]);

    m_pAnalyzer->DumpStringToApiDump(fileName);
    m_pAnalyzer->DumpStringToApiDump("\n");
}

GLvoid CApiDumpDraw::DumpBmp(GLchar *fileName)
{
    GLint   viewport[4];
    GLint   width   = 0;
    GLint   height  = 0;
    GLint   x       = 0;
    GLint   y       = 0;
    GLubyte *data   = NULL;

    glGetIntegerv(GL_VIEWPORT, viewport);
    x       = viewport[0];
    y       = viewport[1];
    width   = viewport[2];
    height  = viewport[3];
    data = (GLubyte*)malloc(4*width*height);

    glReadPixels(x, y, width, height, GL_RGBA, GL_UNSIGNED_BYTE, data);
    ApiWriteBitmap(fileName, data, width, height, 0, RGBA_8888, 0);
    free(data);

    m_pAnalyzer->DumpStringToApiDump(fileName);
    m_pAnalyzer->DumpStringToApiDump("\n");
}

GLvoid CApiDumpDraw::DumpFrame()
{
    if (m_options.dumpFrameBmp)
    {
        GLchar fileName[128] = {};
        sprintf(fileName,"%s/DumpFrame%dth.bmp", API_DUMP_DRAW_PATH, m_nCurrentFrameNum);
        DumpBmp(fileName);
    }
    else if (m_options.dumpFramePng)
    {
        GLchar fileName[128]={};
        sprintf(fileName, "%s/DumpFrame%dth.png", API_DUMP_DRAW_PATH, m_nCurrentFrameNum);
        DumpPng(fileName);
    }

    m_nCurrentFrameNum ++;
}

GLboolean CApiDumpDraw::GetDumpFileName(GLchar *fileName, const GLchar *dumpName, GLint loop, GLESAPIIndex func)
{
    GLint   objectName  = 0;
    GLint   objectType  = 0;
    GLint   texLevel    = 0;
    GLint   texFace     = 0;
    GLint   fbo         = 0;

    glGetIntegerv(GL_FRAMEBUFFER_BINDING, &fbo);

    glGetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER,
                                          GL_COLOR_ATTACHMENT0,
                                          GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE,
                                          &objectType);

    glGetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER,
                                          GL_COLOR_ATTACHMENT0,
                                          GL_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME,
                                          &objectName);

    if (objectType == 0)
    {
        return GL_FALSE;
    }

    if (objectType == GL_TEXTURE)
    {
        glGetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER,
                                              GL_COLOR_ATTACHMENT0,
                                              GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_LEVEL,
                                              &texLevel);

        glGetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER,
                                              GL_COLOR_ATTACHMENT0,
                                              GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_CUBE_MAP_FACE,
                                              &texFace);
    }

    switch (objectType)
    {
    case GL_TEXTURE:
        sprintf(fileName, "%s/DumpFrame%d_%d_fbo%d_%s_tex%d_level%d_face%d_%s.%s",
            API_DUMP_DRAW_PATH, m_nCurrentFrameNum, m_nLoop, fbo, dumpName, objectName, texLevel, texFace,
            GetAPIName(func), m_options.dumpDrawPng?"png":"bmp");
        break;

    case GL_RENDERBUFFER:
        sprintf(fileName, "%s/DumpFrame%d_%d_fbo%d_%s_rbo%d_%s.%s",
            API_DUMP_DRAW_PATH, m_nCurrentFrameNum, m_nLoop, fbo, dumpName, objectName,
            GetAPIName(func), m_options.dumpDrawPng?"png":"bmp");
        break;

    case GL_FRAMEBUFFER_DEFAULT:
        sprintf(fileName, "%s/DumpFrame%d_%d_fbo%d_%s_default_%s.%s",
            API_DUMP_DRAW_PATH, m_nCurrentFrameNum, m_nLoop, fbo, dumpName,
            GetAPIName(func), m_options.dumpDrawPng?"png":"bmp");
        break;

    default:
        sprintf(fileName, "%s/DumpFrame%d_%d_fbo%d_%s_unknown%d_%s.%s",
            API_DUMP_DRAW_PATH, m_nCurrentFrameNum, m_nLoop, fbo, dumpName, objectName,
            GetAPIName(func), m_options.dumpDrawPng?"png":"bmp");
        return GL_FALSE;
    }

    return GL_TRUE;
}

GLvoid CApiDumpDraw::DumpDraw(GLESAPIIndex func)
{
    if (!m_options.dumpDraw)
    {
        return;
    }

    if ((m_options.dumpDrawStart && m_options.dumpDrawStart > m_nCurrentFrameNum) ||
        (m_options.dumpDrawEnd && m_options.dumpDrawEnd < m_nCurrentFrameNum))
    {
        return;
    }

    if (func != GL3_API_glDrawArrays &&
        func != GL3_API_glDrawElements &&
        func != GL3_API_glClear &&
        func != GL3_API_glDrawArraysInstanced &&
        func != GL3_API_glDrawElementsInstanced &&
        func != GL3_API_glDrawRangeElements &&
        func != GL3_API_glMultiDrawArraysEXT &&
        func != GL3_API_glMultiDrawElementsEXT)
    {
        return;
    }

    // Dump depth buffer
    if (m_options.dumpDepth && glIsEnabled(GL_DEPTH_TEST))
    {
        m_apiEngine.SaveDrawStates();
        DumpDepth(func);
        m_apiEngine.LoadDrawStates();
    }
    else
    {
        m_apiEngine.SaveDrawStates();
    }

    GLchar fileName[256] = {};

    if (m_apiEngine.DrawStatesIsBindFBO())
    {
        if (!GetDumpFileName(fileName, "color", m_nLoop, func))
            return;
    }
    else
    {
        if (m_options.dumpDrawPng)
        {
            sprintf(fileName, "%s/DumpFrame%d_%d_color_%s.png", API_DUMP_DRAW_PATH, m_nCurrentFrameNum, m_nLoop, GetAPIName(func));
        }
        else
        {
            sprintf(fileName, "%s/DumpFrame%d_%d_color_%s.bmp", API_DUMP_DRAW_PATH, m_nCurrentFrameNum, m_nLoop, GetAPIName(func));
        }
    }

    if (m_options.dumpDrawPng)
    {
        DumpPng(fileName);
    }
    else
    {
        DumpBmp(fileName);
    }

    m_nLoop ++;
}

GLvoid CApiDumpDraw::DumpDepth(GLESAPIIndex func)
{
    GLchar fileName[256] = {};
    static GLboolean bFirst = GL_TRUE;

    if (bFirst)
    {
        m_apiEngine.CreateDumpDepthProgram();
        bFirst = GL_FALSE;
    }

    if (m_apiEngine.DrawStatesIsBindFBO())
    {
        if (!GetDumpFileName(fileName, "depth", m_nLoop, func))
        {
            return;
        }
    }
    else
    {
        if (m_options.dumpDrawPng)
        {
            sprintf(fileName, "%s/DumpFrame%d_%d_depth_%s.png", API_DUMP_DRAW_PATH, m_nCurrentFrameNum, m_nLoop, GetAPIName(func));
        }
        else
        {
            sprintf(fileName, "%s/DumpFrame%d_%d_depth_%s.bmp", API_DUMP_DRAW_PATH, m_nCurrentFrameNum, m_nLoop, GetAPIName(func));
        }
    }

    GLfloat position[] = {
        -1.0f,  1.0f, 0.0f, 1.0f,
         1.0f,  1.0f, 1.0f, 1.0f,
        -1.0f, -1.0f, 0.0f, 0.0f,
        -1.0f, -1.0f, 0.0f, 0.0f,
         1.0f,  1.0f, 1.0f, 1.0f,
         1.0f, -1.0f, 1.0f, 0.0f
    };

    GLint   width       = m_apiEngine.DrawStatesViewportW();
    GLint   height      = m_apiEngine.DrawStatesViewportH();
    GLuint  program     = m_apiEngine.GetDumpDepthProgram();
    GLenum  texUnit     = m_apiEngine.DrawStatesActiveTexUnit();
    GLuint  tex         = 0;
    GLuint  fbo         = 0;
    GLuint  fbo1        = 0;
    GLuint  tex1        = 0;
    GLint   locPos      = -1;
    GLint   locTex      = -1;
    GLint   locSampler  = -1;
    GLuint  drawFBO     = m_apiEngine.DrawStatesFBO();

    glGetError();

    m_apiEngine.ActiveTexture(texUnit);

    // Create texture
    tex = m_apiEngine.GenTexture(GL_TEXTURE_2D);
    m_apiEngine.TexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, width, height, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);
    m_apiEngine.TexParameters(GL_TEXTURE_2D, GL_NEAREST, GL_NEAREST, GL_REPEAT, GL_REPEAT, GL_REPEAT, GL_NONE);

    // Create FBO
    fbo = m_apiEngine.GenFramebuffer(GL_DRAW_FRAMEBUFFER);
    m_apiEngine.FramebufferTexture2D(GL_DRAW_FRAMEBUFFER, fbo, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, tex, 0);

    // Blit original color buffer to texture.
    m_apiEngine.BindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo);
    m_apiEngine.BindFramebuffer(GL_READ_FRAMEBUFFER, drawFBO);
    m_apiEngine.BlitFramebuffer(width, height, GL_DEPTH_BUFFER_BIT, GL_NEAREST);

    // Create texture
    tex1 = m_apiEngine.GenTexture(GL_TEXTURE_2D);
    m_apiEngine.TexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    m_apiEngine.TexParameters(GL_TEXTURE_2D, GL_NEAREST, GL_NEAREST, GL_REPEAT, GL_REPEAT, GL_REPEAT, GL_NONE);

    fbo1 = m_apiEngine.GenFramebuffer(GL_FRAMEBUFFER);
    m_apiEngine.FramebufferTexture2D(GL_FRAMEBUFFER, fbo1, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex1, 0);
    m_apiEngine.BindFramebuffer(GL_FRAMEBUFFER, fbo1);

    glClear(GL_COLOR_BUFFER_BIT);

    glDisable(GL_DEPTH_TEST);
    glDisable(GL_STENCIL_TEST);
    glDisable(GL_BLEND);

    m_apiEngine.UseDumpDepthProgram();
    locPos      = m_apiEngine.GetAttribLocation(program, "a_vertex");
    locTex      = m_apiEngine.GetAttribLocation(program, "a_texcoord");
    locSampler  = m_apiEngine.GetUniformLocation(program, "sShadow");

    m_apiEngine.ActiveTexture(texUnit);
    m_apiEngine.BindTexture(GL_TEXTURE_2D, tex);
    glUniform1i(locSampler, texUnit-GL_TEXTURE0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glEnableVertexAttribArray(locPos);
    glVertexAttribPointer(locPos, 2, GL_FLOAT, 0, 16, &position[0]);

    if (locTex != -1)
    {
        glEnableVertexAttribArray(locTex);
        glVertexAttribPointer(locTex, 2, GL_FLOAT, 0, 16, &position[2]);
    }

    glDrawArrays(GL_TRIANGLES, 0, 6);

    if (m_options.dumpDrawPng)
    {
        DumpPng(fileName);
    }
    else
    {
        DumpBmp(fileName);
    }

    glDeleteTextures(1, &tex);
    glDeleteFramebuffers(1, &fbo);
    glDeleteTextures(1, &tex1);
    glDeleteFramebuffers(1, &fbo1);
}