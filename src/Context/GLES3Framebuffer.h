#ifndef __GLES3_FRAME_BUFFER_H
#define __GLES3_FRAME_BUFFER_H

#include "GLESGlobal.h"

class CFramebufferObj
{
public:
    CFramebufferObj()
    {
        name            = 0;
        color0          = 0;
        color0Level     = 0;
        color0Type      = 0;
        depth           = 0;
        depthLevel      = 0;
        depthType       = 0;
        stencil         = 0;
        stencilLevel    = 0;
        stencilType     = 0;
    }

    ~CFramebufferObj() {}

public:
    GLboolean           bUsed;

    GLuint              name;
    GLuint              color0;
    GLuint              color0Level;
    GLenum              color0Type;
    GLuint              depth;
    GLuint              depthLevel;
    GLenum              depthType;
    GLuint              stencil;
    GLuint              stencilLevel;
    GLenum              stencilType;
};

typedef std::map<GLuint, CFramebufferObj*>   FboMap;

class CRenderbufferObj
{
public:
    CRenderbufferObj();
    ~CRenderbufferObj();

    GLenum              GetFormat();

public:
    GLboolean           bUsed;

    GLenum              target;
    GLenum              internalformat;
    GLsizei             width;
    GLsizei             height;
};

typedef std::map<GLuint, CRenderbufferObj*>   RboMap;

#endif /* __GLES3_FRAME_BUFFER_H */