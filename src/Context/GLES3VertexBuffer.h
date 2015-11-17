#ifndef __GLES3_VERTEX_BUFFER_H
#define __GLES3_VERTEX_BUFFER_H

#include "GLESGlobal.h"

class CBufObj
{
public:
    CBufObj();
    ~CBufObj();

    GLvoid      UpdateData(GLuint size, GLuint offset, GLchar *pSrc);
    GLvoid      *GetCurrentDataPointer();
    GLuint      GetSize();
    GLvoid      SetData(GLuint size, GLchar *pData);

public:
    GLchar      *m_pData;
    GLuint      m_nSize;

    GLuint      m_nMapIndex;
    GLsizeiptr  m_nMapSize;         // size specifies the amount of data that can be read from or written to the buffer object while used as an indexed target
    GLintptr    m_nMapOffset;       // offset specifies a starting offset into the buffer object buffer
};

typedef std::map<GLuint, CBufObj*>   BufferMap;

class CVertexArrayObj
{
public:
    CVertexArrayObj()
    {
        bUsed = GL_FALSE;
    }

    ~CVertexArrayObj() {}

public:
    GLboolean           bUsed;
};

typedef std::map<GLuint, CVertexArrayObj*>  VaoMap;

class CTransFeedback
{
public:
    CTransFeedback()
    {
        primMode    = 0;
        bActive     = GL_FALSE;
    }

    ~CTransFeedback() {}

public:
    GLenum              primMode;
    GLboolean           bActive;
};

typedef std::map<GLuint, CTransFeedback*>   TransFeedbackMap;

typedef struct _stVertAttrPtr
{
    GLint       size;
    GLenum      type;
    GLboolean   normalized;
    GLsizei     stride;
    GLuint      pointer;
    GLboolean   bUseVBO;
    GLuint      vbo;            // vbo buffer name
    GLuint      offset;
    GLuint      bytesPerPixel;
    GLuint      realIndex;

    GLfloat     genericVal[4];

    _stVertAttrPtr()
    {
        size            = 0;
        type            = 0;
        normalized      = GL_FALSE;
        stride          = 0;
        pointer         = 0;

        bUseVBO         = GL_FALSE;
        vbo             = 0;
        offset          = 0;
        bytesPerPixel   = 0;
        realIndex       = 0;
    }
} stVertAttrPtr;

#endif /* __GLES3_VERTEX_BUFFER_H */