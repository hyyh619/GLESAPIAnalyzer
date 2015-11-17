#include "utils.h"
#include "GLES3VertexBuffer.h"
#include "ApiGLES3Context.h"

CBufObj::CBufObj()
{
    m_nSize         = 0;
    m_pData         = NULL;

    m_nMapIndex     = 0;
    m_nMapSize      = 0;
    m_nMapOffset    = 0;
}

CBufObj::~CBufObj()
{
    if (m_pData != NULL)
    {
        free(m_pData);
    }
}

GLvoid CBufObj::UpdateData(GLuint size, GLuint offset, GLchar *pSrc)
{
    if (m_pData == NULL)
    {
        printf("%s(%d): buffer is NULL.\n", __FUNCTION__, __LINE__);
        return;
    }

    GLchar *pDst = (GLchar*)(m_pData + offset);

    memcpy(pDst, pSrc, size);
}

GLvoid* CBufObj::GetCurrentDataPointer()
{
    return m_pData;
}

GLuint CBufObj::GetSize()
{
    return m_nSize;
}

GLvoid CBufObj::SetData(GLuint size, GLchar *pData)
{
    if (m_pData)
    {
        free(m_pData);;
    }

    m_pData = (GLchar*)malloc(size);
    m_nSize = size;

    if (pData)
    {
        memcpy(m_pData, pData, size);
    }
}

CBufObj* CGLES3Context::CreateBufObj(GLuint bufId)
{
    CBufObj *pBuffer = bufferMap[bufId];

    if (pBuffer != NULL)
    {
        return pBuffer;
    }

    if (!bufId)
        return NULL;

    pBuffer = new CBufObj;
    if (pBuffer == NULL)
    {
        printf("Cannot allocate buffer object.\n");
        return NULL;
    }

    bufferMap[bufId] = pBuffer;
    return pBuffer;
}

GLvoid CGLES3Context::DeleteBufObj(GLuint bufId)
{
    CBufObj *p = bufferMap[bufId];

    if (p == NULL)
        return;

    bufferMap.erase(bufId);
    delete p;
}

GLvoid CGLES3Context::InsertBufObj(GLuint name, CBufObj *p)
{
    bufferMap.insert(std::make_pair(name, p));
}

CBufObj* CGLES3Context::FindBufObj(GLuint bufId)
{
    return bufferMap[bufId];
}

CBufObj* CGLES3Context::FindBufObjByTarget(GLenum target)
{
    GLuint  bufID = 0;

    switch(target)
    {
    case GL_ARRAY_BUFFER:
        bufID = arrayBuffer;
        break;

    case GL_ELEMENT_ARRAY_BUFFER:
        bufID = elementArrayBuffer;
        break;

    case GL_UNIFORM_BUFFER:
        bufID = uniformBuffer;
        break;

    case GL_TRANSFORM_FEEDBACK_BUFFER:
        bufID = transFeedbackBuffer;
        break;

    default:
        break;
    }

    return FindBufObj(bufID);
}

GLuint CGLES3Context::GetBufIdByTarget(GLenum target)
{
    GLuint  bufID = 0;

    switch(target)
    {
    case GL_ARRAY_BUFFER:
        bufID = arrayBuffer;
        break;

    case GL_ELEMENT_ARRAY_BUFFER:
        bufID = elementArrayBuffer;
        break;

    case GL_UNIFORM_BUFFER:
        bufID = uniformBuffer;
        break;

    case GL_TRANSFORM_FEEDBACK_BUFFER:
        bufID = transFeedbackBuffer;
        break;

    default:
        break;
    }

    return bufID;
}

GLvoid CGLES3Context::SetBufObj(GLenum target, GLuint bufId)
{
    switch(target)
    {
    case GL_ARRAY_BUFFER:
        arrayBuffer = bufId;
        break;

    case GL_ELEMENT_ARRAY_BUFFER:
        elementArrayBuffer = bufId;
        break;

    case GL_UNIFORM_BUFFER:
        uniformBuffer = bufId;
        break;

    case GL_TRANSFORM_FEEDBACK_BUFFER:
        transFeedbackBuffer = bufId;
        break;

    default:
        break;
    }
}

GLvoid CGLES3Context::SaveVBO(GLenum target, GLint size, GLchar *data)
{
    GLuint bufId = GetBufIdByTarget(target);

#if 0
    GLchar      name[256];
    FILE        *file = NULL;
    static int  x = 0;

    memset(name, 0, 256);
    sprintf(name, "%s%s/buffer%d_%d.dat", strOnlySourcePath, RESOURCE_PATH, bufId, x++);

    if (IsFileExist(name))
    {
        return;
    }

    file = fopen(name, "wb");
    if (file == NULL)
    {
        return;
    }

    fwrite(data, 1, size, file);
    fclose(file);
#else
    // fwrite(data, 1, size, pVertexDataFile);
#endif
}

CVertexArrayObj* CGLES3Context::CreateVertexArrayObj(GLuint bufId)
{
    CVertexArrayObj *pVAO = vaoMap[bufId];

    if (pVAO != NULL)
    {
        return pVAO;
    }

    if (!bufId)
        return NULL;

    pVAO = new CVertexArrayObj;
    if (pVAO == NULL)
    {
        printf("Cannot allocate buffer object.\n");
        return NULL;
    }

    vaoMap[bufId] = pVAO;
    return pVAO;
}

GLvoid CGLES3Context::DeleteVertexArrayObj(GLuint bufId)
{
    CVertexArrayObj *p = vaoMap[bufId];

    if (p == NULL)
        return;

    vaoMap.erase(bufId);
    delete (p);
}

CTransFeedback* CGLES3Context::CreateTransFeedbackObj(GLuint bufId)
{
    CTransFeedback *pTFB = tfbMap[bufId];

    if (pTFB != NULL)
    {
        return pTFB;
    }

    if (!bufId)
        return NULL;

    pTFB = new CTransFeedback;
    if (pTFB == NULL)
    {
        printf("Cannot allocate buffer object.\n");
        return NULL;
    }

    tfbMap[bufId] = pTFB;
    return pTFB;
}

GLvoid CGLES3Context::DeleteTransFeedbackObj(GLuint bufId)
{
    CTransFeedback *p = tfbMap[bufId];

    if (p == NULL)
        return;

    tfbMap.erase(bufId);
    delete (p);
}

CTransFeedback* CGLES3Context::FindTransFeedbackObj()
{
    TransFeedbackMap::iterator it = tfbMap.find(transFeedback);

    if (it != tfbMap.end())
    {
        return it->second;
    }
    else
    {
        return NULL;
    }
}

GLvoid CGLES3Context::ApiGenBuffers(GLsizei n, GLuint *buffers)
{
    if (!buffers)
        return;

    for (GLsizei i=0; i<n; i++)
    {
        GLuint  name    = buffers[i];
        CBufObj *pBuf   = CreateBufObj(name);
    }

    m_pAnalyzer->AnalyzeGenBuffers(GLOutput, GL_OUT_BUF_SIZE, n, buffers);
}

GLvoid CGLES3Context::ApiDeleteBuffers(GLsizei n, const GLuint *buffers)
{
    if (!buffers)
        return;

    for (GLsizei i=0; i<n; i++)
    {
        DeleteBufObj(buffers[i]);
    }
}

GLvoid CGLES3Context::ApiBindBuffer(GLenum target, GLuint buffer)
{
    CBufObj *pBuffer = FindBufObj(buffer);

    if (pBuffer == NULL)
    {
        pBuffer = CreateBufObj(buffer);
    }

    SetBufObj(target, buffer);
}

GLvoid CGLES3Context::ApiBindBufferBase(GLenum target, GLuint index, GLuint buffer)
{
    CBufObj *pBuffer = FindBufObj(buffer);

    if (buffer == 0)
        return;

    if (pBuffer == NULL)
    {
        pBuffer = CreateBufObj(buffer);
    }

    SetBufObj(target, buffer);

    pBuffer->m_nMapIndex    = index;
    pBuffer->m_nMapSize     = pBuffer->m_nSize;
    pBuffer->m_nMapOffset   = 0;
}

GLvoid CGLES3Context::ApiBindBufferRange(GLenum target, GLuint index, GLuint buffer, GLintptr offset, GLsizeiptr size)
{
    CBufObj *pBuffer = FindBufObj(buffer);

    if (pBuffer == NULL)
    {
        pBuffer = CreateBufObj(buffer);
    }

    SetBufObj(target, buffer);

    pBuffer->m_nMapIndex    = index;
    pBuffer->m_nMapSize     = size;
    pBuffer->m_nMapOffset   = offset;
}

GLvoid CGLES3Context::ApiBufferData (GLenum target, GLsizeiptr size, const void *data, GLenum usage)
{
    CBufObj *pBuffer    = FindBufObjByTarget(target);
    GLchar  *pSrc       = (GLchar*)data;

    if (pBuffer->m_pData)
    {
        if (pBuffer->m_pData)
        {
            free(pBuffer->m_pData);
        }
    }

    pBuffer->m_pData = (GLchar*)malloc(size);
    pBuffer->m_nSize = size;

    if (data != 0)
    {
        memcpy(pBuffer->m_pData, pSrc, size);
    }

    if (pBuffer && pBuffer->m_pData)
    {
        SaveVBO(target, size, (GLchar*)pBuffer->m_pData);
    }
}

GLvoid CGLES3Context::ApiBufferSubData (GLenum target, GLintptr offset, GLsizeiptr size, const void *data)
{
    CBufObj *pBuffer    = FindBufObjByTarget(target);
    GLchar  *pSrc       = (GLchar*)data;

    if (data != 0)
    {
        GLchar *p;

        if (pBuffer->m_nSize == 0)
        {
            pBuffer->m_nSize = offset+size;
        }

        if (!pBuffer->m_pData)
        {
            pBuffer->m_pData = (GLchar*)malloc(pBuffer->m_nSize);
            memset(pBuffer->m_pData, 0, pBuffer->m_nSize);
        }

        if (pSrc)
        {
            /* Copy sub data to buffer */
            p = (GLchar*)pBuffer->m_pData;
            p += offset;
            memcpy(p, pSrc, size);
        }

        SaveVBO(target, size, (GLchar*)pBuffer->m_pData);
    }
}

GLvoid CGLES3Context::ApiGenVertexArrays(GLsizei n, GLuint *arrays)
{
    if (!arrays)
        return;

    for (GLsizei i=0; i<n; i++)
    {
        CreateVertexArrayObj(arrays[i]);
    }
}

GLvoid CGLES3Context::ApiDeleteVertexArrays(GLsizei n, const GLuint *arrays)
{
    if (!arrays)
        return;

    for (GLsizei i=0; i<n; i++)
    {
        DeleteVertexArrayObj(arrays[i]);
    }
}

GLvoid CGLES3Context::ApiBindVertexArray(GLuint array)
{
    CVertexArrayObj *p = vaoMap[array];

    if (p == NULL && array != 0)
    {
        p = CreateVertexArrayObj(array);
    }

    vertexArray = array;
}

GLvoid CGLES3Context::ApiDeleteTransformFeedbacks(GLsizei n, const GLuint *ids)
{
    if (!ids)
        return;

    for (GLsizei i=0; i<n; i++)
    {
        DeleteTransFeedbackObj(ids[i]);
    }
}

GLvoid CGLES3Context::ApiGenTransformFeedbacks(GLsizei n, GLuint *ids)
{
    if (!ids)
        return;

    for (GLsizei i=0; i<n; i++)
    {
        CreateTransFeedbackObj(ids[i]);
    }

    m_pAnalyzer->AnalyzeGenTransformFeedbacks(GLOutput, GL_OUT_BUF_SIZE, n, ids);
}

GLvoid CGLES3Context::ApiBindTransformFeedback(GLenum target, GLuint id)
{
    CTransFeedback *p = tfbMap[id];

    if (p == NULL && id != 0)
    {
        p = CreateTransFeedbackObj(id);
    }

    transFeedback = id;
}

GLvoid CGLES3Context::ApiBeginTransformFeedback(GLenum primitiveMode)
{
    CTransFeedback* p = FindTransFeedbackObj();

    if (p)
    {
        p->primMode = primitiveMode;
        p->bActive  = GL_TRUE;
    }
}

GLvoid CGLES3Context::ApiEndTransformFeedback(void)
{
    CTransFeedback* p = FindTransFeedbackObj();

    if (p)
    {
        p->bActive  = GL_FALSE;
    }
}

GLvoid CGLES3Context::ApiDisableVertexAttribArray(GLuint index)
{
    if ((index >= 0) && (index < API_VERTEX_ATTRIBUTES_NUM))
    {
        vertexAttribBits &= (~(1 << index));
    }
}

GLvoid CGLES3Context::ApiEnableVertexAttribArray(GLuint index)
{
    if ((index >= 0) && (index < API_VERTEX_ATTRIBUTES_NUM))
    {
        vertexAttribBits |= (1 << index);
    }
}

GLvoid  CGLES3Context::SetVertexAttrib1234fv(GLuint index, GLuint count, const GLfloat *pData)
{
    GLfloat *values = (GLfloat*)pData;
    stVertAttrPtr *p = &vertexAttribPointer[index];

    switch (count)
    {
    case 4:
        p->genericVal[3] = values[3];
    case 3:
        p->genericVal[2] = values[2];
    case 2:
        p->genericVal[1] = values[1];
    case 1:
        p->genericVal[0] = values[0];
        break;
    default:
        break;
    }
}

GLvoid CGLES3Context::SetVertexAttrib1234f(GLuint index, GLuint count, GLfloat f1, GLfloat f2, GLfloat f3, GLfloat f4)
{
    stVertAttrPtr *p = &vertexAttribPointer[index];

    switch (count)
    {
    case 4:
        p->genericVal[3] = f4;
    case 3:
        p->genericVal[2] = f3;
    case 2:
        p->genericVal[1] = f2;
    case 1:
        p->genericVal[0] = f1;
        break;
    default:
        break;
    }
}

GLvoid CGLES3Context::ApiVertexAttrib1f(GLuint index, GLfloat x)
{
    SetVertexAttrib1234f(index, 1, x, 0.0f, 0.0f, 0.0f);
}

GLvoid CGLES3Context::ApiVertexAttrib1fv(GLuint index, const GLfloat *v)
{
    SetVertexAttrib1234fv(index, 1, v);
}

GLvoid CGLES3Context::ApiVertexAttrib2f(GLuint index, GLfloat x, GLfloat y)
{
    SetVertexAttrib1234f(index, 2, x, y, 0.0f, 0.0f);
}

GLvoid CGLES3Context::ApiVertexAttrib2fv(GLuint index, const GLfloat *v)
{
    SetVertexAttrib1234fv(index, 2, v);
}

GLvoid CGLES3Context::ApiVertexAttrib3f(GLuint index, GLfloat x, GLfloat y, GLfloat z)
{
    SetVertexAttrib1234f(index, 3, x, y, z, 0.0f);
}

GLvoid CGLES3Context::ApiVertexAttrib3fv(GLuint index, const GLfloat *v)
{
    SetVertexAttrib1234fv(index, 3, v);
}

GLvoid CGLES3Context::ApiVertexAttrib4f(GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w)
{
    SetVertexAttrib1234f(index, 4, x, y, z, w);
}

GLvoid CGLES3Context::ApiVertexAttrib4fv(GLuint index, const GLfloat *v)
{
    SetVertexAttrib1234fv(index, 4, v);
}

GLvoid CGLES3Context::ApiVertexAttribPointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void *pointer)
{
    stVertAttrPtr   *pVertex    = &(vertexAttribPointer[index]);

    if (size == 0 && type == 0)
    {
        return;
    }

    if ((index >= 0) && (index < API_VERTEX_ATTRIBUTES_NUM))
    {
        if (stride == 0)
        {
            stride = GetDataTypeSize(type) * size;
        }

        pVertex->normalized = normalized;
        pVertex->pointer    = (GLuint)pointer;
        pVertex->size       = size;
        pVertex->stride     = stride;
        pVertex->type       = type;
        pVertex->vbo        = arrayBuffer;
        pVertex->bUseVBO    = arrayBuffer?GL_TRUE:GL_FALSE;
    }
}

GLvoid CGLES3Context::ApiVertexAttribIPointer(GLuint index, GLint size, GLenum type, GLsizei stride, const void *pointer)
{
    stVertAttrPtr   *pVertex    = &(vertexAttribPointer[index]);

    if (size == 0 && type == 0)
    {
        return;
    }

    if ((index >= 0) && (index < API_VERTEX_ATTRIBUTES_NUM))
    {
        if (stride == 0)
        {
            stride = GetDataTypeSize(type) * size;
        }

        pVertex->pointer    = (GLuint)pointer;
        pVertex->size       = size;
        pVertex->stride     = stride;
        pVertex->type       = type;
        pVertex->vbo        = arrayBuffer;
        pVertex->bUseVBO    = arrayBuffer?GL_TRUE:GL_FALSE;
    }
}

GLuint CGLES3Context::GetVertexAttribBits()
{
    return vertexAttribBits;
}

GLuint CGLES3Context::GetArrayBuffer()
{
    return arrayBuffer;
}

GLuint CGLES3Context::GetElementArrayBuffer()
{
    return elementArrayBuffer;
}

GLuint CGLES3Context::GetTransFeedbackBuffer()
{
    return transFeedbackBuffer;
}

GLuint CGLES3Context::GetUniformBuffer()
{
    return uniformBuffer;
}

stVertAttrPtr* CGLES3Context::GetVertexAttribPointer(GLint index)
{
    if (index < 0 || index > API_VERTEX_ATTRIBUTES_NUM)
        return NULL;
    else
        return &vertexAttribPointer[index];
}

GLvoid CGLES3Context::ApiGetBufferParameteriv(GLenum target, GLenum pname, GLint *params)
{
}

GLvoid CGLES3Context::ApiGetVertexAttribfv(GLuint index, GLenum pname, GLfloat *params)
{
}

GLvoid CGLES3Context::ApiGetVertexAttribiv(GLuint index, GLenum pname, GLint *params)
{
}

GLvoid CGLES3Context::ApiGetVertexAttribPointerv(GLuint index, GLenum pname, void **pointer)
{
}
