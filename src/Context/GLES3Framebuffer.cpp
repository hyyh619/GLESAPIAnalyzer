#include "GLES3Framebuffer.h"
#include "ApiGLES3Context.h"

CRenderbufferObj::CRenderbufferObj()
{
    bUsed           = GL_FALSE;
    target          = 0;
    internalformat  = 0;
    width           = 0;
    height          = 0;
}

CRenderbufferObj::~CRenderbufferObj()
{
}

GLenum CRenderbufferObj::GetFormat()
{
    return internalformat;
}

GLvoid CGLES3Context::ApiBindFramebuffer(GLenum target, GLuint fbo)
{
    CFramebufferObj *pFBO = NULL;

    if (target != GL_FRAMEBUFFER)
        return;

    currentFBO = fbo;

    if (fbo != 0)
    {
        pFBO = fboMap[fbo];
        if (pFBO == NULL)
        {
            pFBO = CreateFramebufferObj(fbo);
        }
    }
}

GLvoid CGLES3Context::ApiBindRenderbuffer(GLenum target, GLuint rbo)
{
    CRenderbufferObj *pRBO = NULL;

    if (target != GL_RENDERBUFFER)
        return;

    currentRBO = rbo;

    if (rbo != 0)
    {
        pRBO = rboMap[rbo];
        if (pRBO == NULL)
        {
            pRBO = CreateRenderbufferObj(rbo);
        }
    }
}

GLvoid CGLES3Context::ApiDeleteFramebuffers(GLsizei n, const GLuint *framebuffers)
{
    if (!framebuffers)
        return;

    for (GLsizei i=0; i<n; i++)
    {
        DeleteFramebufferObj(framebuffers[i]);
    }
}

GLvoid CGLES3Context::ApiDeleteRenderbuffers(GLsizei n, const GLuint *framebuffers)
{
    if (!framebuffers)
        return;

    for (GLsizei i=0; i<n; i++)
    {
        DeleteRenderbufferObj(framebuffers[i]);
    }
}

GLvoid CGLES3Context::ApiGenRenderbuffers(GLsizei n, GLuint *buffers)
{
    if (!buffers)
        return;

    for (GLsizei i=0; i<n; i++)
    {
        CRenderbufferObj *pBuf = CreateRenderbufferObj(buffers[i]);
    }
}

GLvoid CGLES3Context::ApiGenFramebuffers(GLsizei n, GLuint *buffers)
{
    if (!buffers)
        return;

    for (GLsizei i=0; i<n; i++)
    {
        CFramebufferObj *pBuf = CreateFramebufferObj(buffers[i]);
    }
}

CFramebufferObj* CGLES3Context::CreateFramebufferObj(GLuint fbo)
{
    CFramebufferObj *pFBO = fboMap[fbo];

    if (pFBO != NULL)
    {
        return pFBO;
    }

    pFBO = new CFramebufferObj;
    if (pFBO == NULL)
    {
        printf("Cannot allocate fbo object.\n");
        return NULL;
    }

    pFBO->bUsed = GL_TRUE;
    pFBO->name  = fbo;
    fboMap[fbo] = pFBO;
    return pFBO;
}

GLvoid CGLES3Context::DeleteFramebufferObj(GLuint fbo)
{
    CFramebufferObj *p = fboMap[fbo];

    if (p == NULL)
        return;

    fboMap.erase(fbo);
    delete p;
}

CFramebufferObj* CGLES3Context::GetCurrentFBO()
{
    GLuint fbo = currentFBO;

    if (fbo)
    {
        return fboMap[fbo];
    }
    else
    {
        return NULL;
    }
}

CRenderbufferObj* CGLES3Context::GetRBOByName(GLuint name)
{
    return rboMap[name];
}

CRenderbufferObj* CGLES3Context::CreateRenderbufferObj(GLuint fbo)
{
    CRenderbufferObj *pRBO = rboMap[fbo];

    if (pRBO != NULL)
    {
        return pRBO;
    }

    pRBO = new CRenderbufferObj;
    if (pRBO == NULL)
    {
        printf("Cannot allocate fbo object.\n");
        return NULL;
    }

    pRBO->bUsed = GL_TRUE;
    rboMap[fbo] = pRBO;
    return pRBO;
}

GLvoid CGLES3Context::DeleteRenderbufferObj(GLuint rbo)
{
    CRenderbufferObj *p = rboMap[rbo];

    if (p == NULL)
        return;

    rboMap.erase(rbo);
    delete p;
}

GLvoid CGLES3Context::ApiRenderbufferStorage (GLenum target, GLenum internalformat, GLsizei width, GLsizei height)
{
    CRenderbufferObj *pRbo = rboMap[currentRBO];

    if (pRbo)
    {
        pRbo->target            = target;
        pRbo->internalformat    = internalformat;
        pRbo->width             = width;
        pRbo->height            = height;
    }
}

GLvoid CGLES3Context::ApiFramebufferTexture2D(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level)
{
    GLuint          index   = currentFBO;
    CFramebufferObj *p      = fboMap[index];

    switch (attachment)
    {
    case GL_COLOR_ATTACHMENT0:
        p->color0       = texture;
        p->color0Type   = textarget;
        p->color0Level  = level;
        break;

    case GL_DEPTH_ATTACHMENT:
        p->depth        = texture;
        p->depthType    = textarget;
        p->depthLevel   = level;
        break;

    case GL_STENCIL_ATTACHMENT:
        p->stencil      = texture;
        p->stencilType  = textarget;
        p->stencilLevel = level;
        break;

    default:
        break;
    }
}

GLvoid CGLES3Context::ApiFramebufferRenderbuffer(GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer)
{
    GLuint          index   = currentFBO;
    CFramebufferObj *p      = fboMap[index];

    switch (attachment)
    {
    case GL_DEPTH_ATTACHMENT:
        p->depth        = renderbuffer;
        p->depthType    = renderbuffertarget;
        break;

    case GL_STENCIL_ATTACHMENT:
        p->stencil      = renderbuffer;
        p->stencilType  = renderbuffertarget;
        break;
    }
}

GLvoid CGLES3Context::ApiCheckFramebufferStatus(GLenum target, GLenum result)
{
}

GLvoid CGLES3Context::ApiGetFramebufferAttachmentParameteriv(GLenum target, GLenum attachment, GLenum pname, GLint *params)
{
}

GLvoid CGLES3Context::ApiGetRenderbufferParameteriv(GLenum target, GLenum pname, GLint *params)
{
}