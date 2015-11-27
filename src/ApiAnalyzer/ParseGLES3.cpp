#include "utils.h"
#include "bitmap.h"
#include "MemoryPool.h"
#include "TranslateGL.h"
#include "ParseGLES3.h"
#include "GLESShare.h"
#include "gles3.h"
#include "Analyzer.h"
#include "ApiEGLContext.h"

GLvoid ParseCompressedTexImage2D(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint arg[8];

    if (sscanf(input,
               "CompressedTexImage2D 0x%08X 0x%08X 0x%08X 0x%08X 0x%08X 0x%08X 0x%08X (0x%08X)",
               &arg[0], &arg[1], &arg[2], &arg[3], &arg[4], &arg[5], &arg[6], &arg[7]) == 8)
    {
        GLenum      target      = arg[0];
        GLuint      level       = arg[1];
        GLenum      format      = arg[2];
        GLint       width       = arg[3];
        GLint       height      = arg[4];
        GLint       imageSize   = arg[6];
        GLenum      type        = GL_UNSIGNED_BYTE;
        CTexObj     *pTexObj    = CURRENT_CONTEXT1()->GetActiveTexObj(target);
        stTexImage  *pTexImg    = pTexObj->GetTexImageByLevel(target, level);
        GLvoid      *data       = pTexImg->pixels;

        ANALYZER_FUNC8(CompressedTexImage2D, target, level, format, width, height, arg[5], imageSize, data);

        FreeData(data);
    }
}

GLvoid ParseCompressedTexSubImage2D(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint arg[9];

    if (sscanf(input,
               "CompressedTexSubImage2D 0x%08X 0x%08X 0x%08X 0x%08X 0x%08X 0x%08X 0x%08X 0x%08X (0x%08X)",
               &arg[0], &arg[1], &arg[2], &arg[3], &arg[4], &arg[5], &arg[6], &arg[7], &arg[8]) == 9)
    {
        GLenum      target      = arg[0];
        GLuint      level       = arg[1];
        GLint       xoffset     = arg[2];
        GLint       yoffset     = arg[3];
        GLsizei     width       = arg[4];
        GLsizei     height      = arg[5];
        GLenum      format      = arg[6];
        GLsizei     imageSize   = arg[7];
        GLuint      handle      = arg[8];
        GLenum      type        = GL_UNSIGNED_BYTE;
        CTexObj     *pTexObj    = CURRENT_CONTEXT1()->GetActiveTexObj(target);
        stTexImage  *pTexImg    = pTexObj->GetTexImageByLevel(target, level);
        GLvoid      *data       = pTexImg->pixels;

        ANALYZER_FUNC9(CompressedTexSubImage2D, target, level, xoffset, yoffset, width, height, format, imageSize, data);
    }
}

GLvoid ParseTexImage2D(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint arg[10];

    if (sscanf(input,
               "TexImage2D 0x%08X 0x%08X 0x%08X 0x%08X 0x%08X 0x%08X 0x%08X 0x%08X (0x%08X)",
               &arg[0], &arg[1], &arg[2], &arg[3], &arg[4], &arg[5], &arg[6], &arg[7], &arg[8]) == 9)
    {
        GLenum      target          = arg[0];
        GLint       level           = arg[1];
        GLint       internalformat  = arg[2];
        GLsizei     width           = arg[3];
        GLsizei     height          = arg[4];
        GLint       border          = arg[5];
        GLenum      format          = arg[6];
        GLenum      type            = arg[7];
        CTexObj     *pTexObj        = CURRENT_CONTEXT1()->GetActiveTexObj(target);
        stTexImage  *pTexImg        = pTexObj->GetTexImageByLevel(target, level);
        GLvoid      *pixels         = pTexImg->pixels;

        if (!pTexObj || !pTexImg)
            return;

        ANALYZER_FUNC9(TexImage2D, target, level, internalformat, width, height, border, format, type, (GLvoid*)arg[8]);
    }
}

GLvoid ParseTexSubImage2D(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint arg[10];

    if (sscanf(input,
               "TexSubImage2D 0x%08X 0x%08X 0x%08X 0x%08X 0x%08X 0x%08X 0x%08X 0x%08X (0x%08X)",
               &arg[0], &arg[1], &arg[2], &arg[3], &arg[4], &arg[5], &arg[6], &arg[7], &arg[8]) == 9)
    {
        GLenum      target      = arg[0];
        GLint       level       = arg[1];
        GLint       xoffset     = arg[2];
        GLint       yoffset     = arg[3];
        GLsizei     width       = arg[4];
        GLsizei     height      = arg[5];
        GLenum      format      = arg[6];
        GLenum      type        = arg[7];
        CTexObj     *pTexObj    = CURRENT_CONTEXT1()->GetActiveTexObj(target);
        stTexImage  *pTexImg    = pTexObj?pTexObj->GetTexImageByLevel(target, level):NULL;

        if (pTexImg == NULL)
            return;

        ANALYZER_FUNC9(TexSubImage2D, target, level, xoffset, yoffset, width, height, format, type, (GLvoid*)arg[8]);
    }
}

GLvoid ParseTexImage3D(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint arg[10];

    if (sscanf(input,
               "TexImage3D 0x%08X 0x%08X 0x%08X 0x%08X 0x%08X 0x%08X 0x%08X 0x%08X 0x%08X (0x%08X)",
               &arg[0], &arg[1], &arg[2], &arg[3], &arg[4], &arg[5], &arg[6], &arg[7], &arg[8], &arg[9]) == 10)
    {
        GLenum      target          = arg[0];
        GLint       level           = arg[1];
        GLint       internalformat  = arg[2];
        GLsizei     width           = arg[3];
        GLsizei     height          = arg[4];
        GLsizei     depth           = arg[5];
        GLint       border          = arg[6];
        GLenum      format          = arg[7];
        GLenum      type            = arg[8];
        GLuint      dataPtr         = arg[9];
        CTexObj     *pTexObj        = CURRENT_CONTEXT1()->GetActiveTexObj(target);
        stTexImage  *pTexImg        = pTexObj?pTexObj->GetTexImageByLevel(target, level):NULL;

        if (!pTexObj)
            return;

        ANALYZER_FUNC10(TexImage3D, target, level, internalformat, width, height, depth, border, format, type, (GLvoid*)dataPtr);
    }
}

GLvoid ParseTexSubImage3D(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint arg[11];

    if (sscanf(input,
               "TexSubImage3D 0x%08X 0x%08X 0x%08X 0x%08X 0x%08X 0x%08X 0x%08X 0x%08X 0x%08X 0x%08X (0x%08X)",
               &arg[0], &arg[1], &arg[2], &arg[3], &arg[4], &arg[5], &arg[6], &arg[7], &arg[8], &arg[9], &arg[10]) == 11)
    {
        GLenum      target      = arg[0];
        GLint       level       = arg[1];
        GLint       xoffset     = arg[2];
        GLint       yoffset     = arg[3];
        GLint       zoffset     = arg[4];
        GLsizei     width       = arg[5];
        GLsizei     height      = arg[6];
        GLsizei     depth       = arg[7];
        GLenum      format      = arg[8];
        GLenum      type        = arg[9];
        GLubyte      *pixels    = NULL;

        CTexObj     *pTexObj    = CURRENT_CONTEXT1()->GetActiveTexObj(target);
        stTexImage  *pTexImg    = pTexObj?pTexObj->GetTexImageByLevel(target, level):NULL;
        GLuint      pixelSize   = GetPixelSizeByTexFormat(format, type);

        if (!pTexImg)
            return;

        ANALYZER_FUNC11(TexSubImage3D, target, level, xoffset, yoffset, zoffset, width, height, depth, format, type, (GLvoid*)arg[10]);
    }
}

GLvoid ParseTexStorage3D(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint arg[10];

    if (sscanf(input,
        "TexStorage3D 0x%08X 0x%08X 0x%08X 0x%08X 0x%08X 0x%08X}",
               &arg[0], &arg[1], &arg[2], &arg[3], &arg[4], &arg[5]) == 6)
    {
        GLenum  target  = arg[0];
        GLsizei levels  = arg[1];
        GLenum  format  = arg[2];
        GLsizei width   = arg[3];
        GLsizei height  = arg[4];
        GLsizei depth   = arg[5];

        ANALYZER_FUNC6(TexStorage3D, target, levels, format, width, height, depth);
    }
}

GLvoid ParseTexStorage2D(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint arg[10];

    if (sscanf(input,
        "TexStorage2D 0x%08X 0x%08X 0x%08X 0x%08X 0x%08X}",
               &arg[0], &arg[1], &arg[2], &arg[3], &arg[4]) == 5)
    {
        GLenum  target  = arg[0];
        GLsizei levels  = arg[1];
        GLenum  format  = arg[2];
        GLsizei width   = arg[3];
        GLsizei height  = arg[4];

        ANALYZER_FUNC5(TexStorage2D, target, levels, format, width, height);
    }
}

GLvoid ParseBindTexture(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint arg[2];

    if (sscanf(input, "BindTexture 0x%08X 0x%08X}", &arg[0], &arg[1]) == 2)
    {
        ANALYZER_FUNC2(BindTexture, arg[0], arg[1]);
    }
}

GLvoid ParseDrawBuffers(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint arg[3];

    if (sscanf(input, "DrawBuffers 0x%08X (0x%08X)", &arg[0], &arg[1]) == 2)
    {
        GLint   n           = arg[0];
        GLint   ptr         = arg[1];
        GLenum  *pBufName   = NULL;

        if (ptr)
        {
            pBufName = (GLenum*)ReadData(READ_GL_UINT, ptr, n);
        }

        ANALYZER_FUNC2(DrawBuffers, n, pBufName);
    }
}

GLvoid ParseMapBufferRange(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint arg[5];

    if (sscanf(input, "MapBufferRange 0x%08X 0x%08X 0x%08X 0x%08X := 0x%08X",
            &arg[0], &arg[1], &arg[2], &arg[3], &arg[4]) == 5)
    {
        GLenum      target  = arg[0];
        GLintptr    offset  = arg[1];
        GLsizeiptr  len     = arg[2];
        GLbitfield  access  = arg[3];
        GLuint      pointer = arg[4];

        ANALYZER_FUNC5(MapBufferRange, target, offset, len, access, (GLvoid*)pointer);
    }
}

GLvoid ParseFlushMappedBufferRange(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint arg[3];

    if (sscanf(input, "FlushMappedBufferRange 0x%08X 0x%08X 0x%08X}",
            &arg[0], &arg[1], &arg[2]) == 3)
    {
        GLenum      target  = arg[0];
        GLintptr    offset  = arg[1];
        GLsizeiptr  len     = arg[2];

        ANALYZER_FUNC3(FlushMappedBufferRange, target, offset, len);
    }
}

GLvoid ParseUnmapBuffer(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint arg[2];

    if (sscanf(input, "UnmapBuffer 0x%08X 0x%08X}",
            &arg[0], &arg[1]) == 2)
    {
        GLenum      target  = arg[0];
        GLboolean   result  = arg[1] ? GL_TRUE : GL_FALSE;
        ANALYZER_FUNC2(UnmapBuffer, target, result);
    }
}

GLvoid ParseInvalidateFramebuffer(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint arg[3];

    if (sscanf(input, "InvalidateFramebuffer 0x%08X 0x%08X (0x%08X)", &arg[0], &arg[1], &arg[2]) == 3)
    {
        GLenum  target      = arg[0];
        GLint   n           = arg[1];
        GLint   ptr         = arg[2];
        GLenum  *pBufName   = NULL;

        if (ptr)
        {
            pBufName = (GLenum*)ReadData(READ_GL_UINT, ptr, n);
        }

        ANALYZER_FUNC3(InvalidateFramebuffer, target, n, pBufName);
    }
}

GLvoid ParseGetProgramBinary(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint arg[5];

    if (sscanf(input, "GetProgramBinary 0x%08X 0x%08X 0x%08X 0x%08X 0x%08X}", &arg[0], &arg[1], &arg[2], &arg[3], &arg[4]) == 5)
    {
        GLuint  program         = arg[0];
        GLsizei bufSize         = arg[1];
        GLuint  lenPtr          = arg[2];
        GLuint  binFormatPtr    = arg[3];
        GLuint  binPtr          = arg[4];
        GLsizei *pLen           = 0;
        GLenum  *pBinFormat     = 0;
        GLvoid  *pBin           = NULL;

        if (lenPtr)
        {
            pLen = (GLsizei*)ReadData(READ_GL_UINT, lenPtr, 1);
        }

        if (binFormatPtr)
        {
            pBinFormat = (GLenum*)ReadData(READ_GL_UINT, binFormatPtr, 1);
        }

        if (binPtr)
        {
            pBin = (GLvoid*)ReadData(READ_GL_UCHAR, binPtr, pLen?*pLen:bufSize);
        }

        ANALYZER_FUNC5(GetProgramBinary, program, bufSize, pLen, pBinFormat, pBin);
    }
}

GLvoid ParseTransformFeedbackVaryings(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint arg[4];

    if (sscanf(input, "TransformFeedbackVaryings 0x%08X 0x%08X 0x%08X 0x%08X}", &arg[0], &arg[1], &arg[2], &arg[3]) == 4)
    {
        GLuint  program     = arg[0];
        GLuint  count       = arg[1];
        GLuint  varyings    = arg[2];
        GLuint  bufferMode  = arg[3];
        GLuint  *ppVaryings = NULL;
        GLvoid  *pVaryings  = NULL;
        GLchar  **p         = NULL;

        if (varyings)
        {
            ppVaryings  = (GLuint*)ReadData(READ_GL_UINT, varyings, count);
            p           = (GLchar**)malloc(sizeof(GLchar*)*count);
        }

        sprintf(output, "glTransformFeedbackVaryings(prog=%08d, %d, ", program, count);
        for (GLuint i=0; i<count; i++)
        {
            pVaryings = (GLchar*)ReadData(READ_GL_UCHAR, ppVaryings[i], 0);
            p[i] = (GLchar*)pVaryings;
        }

        ANALYZER_FUNC4(TransformFeedbackVaryings, program, count, p, bufferMode);
        FREE(p);
    }
}

GLvoid ParseBindBufferBase(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint arg[3];

    if (sscanf(input, "BindBufferBase 0x%08X 0x%08X 0x%08X}", &arg[0], &arg[1], &arg[2]) == 3)
    {
        GLenum  target  = arg[0];
        GLuint  index   = arg[1];
        GLuint  buffer  = arg[2];

        ANALYZER_FUNC3(BindBufferBase, target, index, buffer);
    }
}

GLvoid ParseBindBufferRange(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint arg[5];

    if (sscanf(input, "BindBufferRange 0x%08X 0x%08X 0x%08X 0x%08X 0x%08X}", &arg[0], &arg[1], &arg[2], &arg[3], &arg[4]) == 5)
    {
        GLenum      target  = arg[0];
        GLuint      index   = arg[1];
        GLuint      buffer  = arg[2];
        GLintptr    offset  = arg[3];
        GLsizeiptr  size    = arg[4];

        ANALYZER_FUNC5(BindBufferRange, target, index, buffer, offset, size);
    }
}

GLvoid ParseVertexAttribPointer(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint arg[6];

    if (sscanf(input,
               "VertexAttribPointer 0x%08X 0x%08X 0x%08X 0x%08X 0x%08X 0x%08X}",
               &arg[0], &arg[1], &arg[2], &arg[3], &arg[4], &arg[5]) == 6)
    {
        ANALYZER_FUNC6(VertexAttribPointer, arg[0], arg[1], arg[2], arg[3], arg[4], (GLvoid*)arg[5]);
    }
}

GLvoid ParseVertexAttribIPointer(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint arg[5];

    if (sscanf(input,
               "VertexAttribIPointer 0x%08X 0x%08X 0x%08X 0x%08X 0x%08X}",
               &arg[0], &arg[1], &arg[2], &arg[3], &arg[4]) == 5)
    {
        ANALYZER_FUNC5(VertexAttribIPointer, arg[0], arg[1], arg[2], arg[3], (GLvoid*)arg[4]);
    }
}

GLvoid ParseBeginTransformFeedback(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint arg[1];

    if (sscanf(input, "BeginTransformFeedback 0x%08X}", &arg[0]) == 1)
    {
        GLenum primitiveMode = arg[0];
        ANALYZER_FUNC1(BeginTransformFeedback, primitiveMode);
    }
}

GLvoid ParseEndTransformFeedback(GLchar *input, GLchar *output, GLint outputSize)
{
    ANALYZER_FUNC0(EndTransformFeedback);
}

GLvoid ParsePauseTransformFeedback(GLchar *input, GLchar *output, GLint outputSize)
{
    ANALYZER_FUNC0(PauseTransformFeedback);
}

GLvoid ParseResumeTransformFeedback(GLchar *input, GLchar *output, GLint outputSize)
{
    ANALYZER_FUNC0(ResumeTransformFeedback);
}

GLvoid ParseBindBuffer(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint arg[2];

    if (sscanf(input, "BindBuffer 0x%08X 0x%08X}", &arg[0], &arg[1]) == 2)
    {
        ANALYZER_FUNC2(BindBuffer, arg[0], arg[1]);
    }
}

GLvoid ParseBindVertexArray(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint arg[1];

    if (sscanf(input, "BindVertexArray 0x%08X}", &arg[0]) == 1)
    {
        GLuint name = arg[0];
        ANALYZER_FUNC1(BindVertexArray, name);
    }
}

GLvoid ParseBindTransformFeedback(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint arg[2];

    if (sscanf(input, "BindTransformFeedback 0x%08X 0x%08X}", &arg[0], &arg[1]) == 2)
    {
        GLenum  target  = arg[0];
        GLuint  name    = arg[1];
        ANALYZER_FUNC2(BindTransformFeedback, target, name);
    }
}

GLvoid ParseBufferData(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint arg[4];

    if (sscanf(input,
               "BufferData 0x%08X 0x%08X (0x%08X) 0x%08X",
               &arg[0], &arg[1], &arg[2], &arg[3]) == 4)
    {
        GLenum  target  = arg[0];
        GLuint  size    = arg[1];
        GLint   data    = arg[2];
        GLenum  usage   = arg[3];

        ANALYZER_FUNC4(BufferData, target, size, (GLvoid*)data, usage);
    }
}

GLvoid ParseBufferSubData(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint arg[4];

    if (sscanf(input,
               "BufferSubData 0x%08X 0x%08X 0x%08X (0x%08X)",
               &arg[0], &arg[1], &arg[2], &arg[3]) == 4)
    {
        GLenum  target  = arg[0];
        GLuint  offset  = arg[1];
        GLuint  size    = arg[2];
        GLuint  data    = arg[3];

        ANALYZER_FUNC4(BufferSubData, target, offset, size, (GLvoid*)data);
    }
}

GLvoid ParseAttachShader(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint arg[2];

    if (sscanf(input, "AttachShader 0x%08X 0x%08X}", &arg[0], &arg[1]) == 2)
    {
        GLuint  progId      = arg[0];
        GLuint  shaderId    = arg[1];
        ANALYZER_FUNC2(AttachShader, progId, shaderId);
    }
}

GLvoid ParseActiveTexture(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint arg[1];

    if (sscanf(input, "ActiveTexture 0x%08X}", &arg[0]) == 1)
    {
        GLenum unit = arg[0];
        ANALYZER_FUNC1(ActiveTexture, unit);
    }
}

GLvoid ParseBindAttribLocation(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint arg[3];

    if (sscanf(input, "BindAttribLocation 0x%08X 0x%08X (0x%08X)", &arg[0], &arg[1], &arg[2]) == 3)
    {
        GLchar *name = (GLchar*)ReadData(READ_GL_UCHAR, arg[2], 0);
        ANALYZER_FUNC3(BindAttribLocation, arg[0], arg[1], name);
    }
}

GLvoid ParseBindFramebuffer(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint arg[2];

    if (sscanf(input, "BindFramebuffer 0x%08X 0x%08X}", &arg[0], &arg[1]) == 2)
    {
        ANALYZER_FUNC2(BindFramebuffer, arg[0], arg[1]);
    }
}

GLvoid ParseBindRenderbuffer(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint arg[2];

    if (sscanf(input, "BindRenderbuffer 0x%08X 0x%08X}", &arg[0], &arg[1]) == 2)
    {
        ANALYZER_FUNC2(BindRenderbuffer, arg[0], arg[1]);
    }
}

GLvoid ParseRenderbufferStorage(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint arg[4];

    if (sscanf(input,
               "RenderbufferStorage 0x%08X 0x%08X 0x%08X 0x%08X}",
               &arg[0], &arg[1], &arg[2], &arg[3]) == 4)
    {
        ANALYZER_FUNC4(RenderbufferStorage, arg[0], arg[1], arg[2], arg[3]);
    }
}

GLvoid ParseFramebufferTexture2D(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint arg[5];

    if (sscanf(input,
               "FramebufferTexture2D 0x%08X 0x%08X 0x%08X 0x%08X 0x%08X}",
               &arg[0], &arg[1], &arg[2], &arg[3], &arg[4]) == 5)
    {
        ANALYZER_FUNC5(FramebufferTexture2D, arg[0], arg[1], arg[2], arg[3], arg[4]);
    }
}

GLvoid ParseTexParameteri(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint arg[3];

    if (sscanf(input, "TexParameteri 0x%08X 0x%08X 0x%08X}", &arg[0], &arg[1], &arg[2]) == 3)
    {
        GLenum  target  = arg[0];
        GLenum  pname   = arg[1];
        GLint   value   = arg[2];
        ANALYZER_FUNC3(TexParameteri, target, pname, value);
    }
}

GLvoid ParseTexParameterf(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint arg[3];

    if (sscanf(input, "TexParameterf 0x%08X 0x%08X 0x%08X}", &arg[0], &arg[1], &arg[2]) == 3)
    {
        GLenum  target  = arg[0];
        GLenum  pname   = arg[1];
        GLfloat value   = *(GLfloat*)&arg[2];
        ANALYZER_FUNC3(TexParameterf, target, pname, value);
    }
}

GLvoid ParseTexParameteriv(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint arg[3];

    if (sscanf(input, "TexParameteriv 0x%08X 0x%08X (0x%08X)", &arg[0], &arg[1], &arg[2]) == 3)
    {
        GLenum  target  = arg[0];
        GLenum  pname   = arg[1];
        GLint   value   = ReadUInt(arg[2]);
        ANALYZER_FUNC3(TexParameteriv, target, pname, &value);
    }
}

GLvoid ParseTexParameterfv(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint arg[3];

    if (sscanf(input,
               "TexParameterfv 0x%08X 0x%08X (0x%08X)",
               &arg[0], &arg[1], &arg[2]) == 3)
    {
        GLenum  target  = arg[0];
        GLenum  pname   = arg[1];
        GLuint  v       = ReadUInt(arg[2]);
        GLfloat *params = (GLfloat*)&v;
        ANALYZER_FUNC3(TexParameterfv, target, pname, params);
    }
}

GLvoid ParseGetActiveUniform(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint arg[7];

    if (sscanf(input,
               "GetActiveUniform 0x%08X 0x%08X 0x%08X (0x%08X) (0x%08X) (0x%08X) (0x%08X)",
               &arg[0], &arg[1], &arg[2], &arg[3], &arg[4], &arg[5], &arg[6]) == 7)
    {
        GLuint  program = arg[0];
        GLuint  index   = arg[1];
        GLsizei bufSize = arg[2];
        GLsizei *length = (GLsizei*)ReadData(READ_GL_UINT, arg[3], 0);
        GLint   *size   = (GLint*)ReadData(READ_GL_UINT, arg[4], 0);
        GLenum  *type   = (GLenum*)ReadData(READ_GL_UINT, arg[5], 0);
        GLchar  *name   = (GLchar*)ReadData(READ_GL_UCHAR, arg[6], 0);
        ANALYZER_FUNC7(GetActiveUniform, program, index, bufSize, length, size, type, name);
    }
}

GLvoid ParseFlush(GLchar *input, GLchar *output, GLint outputSize)
{
    if (strncmp(input, "Flush}", 6) == 0)
    {
        ANALYZER_FUNC0(Flush);
    }
}

GLvoid ParseFinish(GLchar *input, GLchar *output, GLint outputSize)
{
    if (strncmp(input, "Finish}", 7) == 0)
    {
        ANALYZER_FUNC0(Finish);
    }
}

GLvoid ParseFramebufferRenderbuffer(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint arg[4];

    if (sscanf(input,
               "FramebufferRenderbuffer 0x%08X 0x%08X 0x%08X 0x%08X}",
               &arg[0], &arg[1], &arg[2], &arg[3]) == 4)
    {
        ANALYZER_FUNC4(FramebufferRenderbuffer, arg[0], arg[1], arg[2], arg[3]);
    }
}

GLvoid ParseFrontFace(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint arg[1];

    if (sscanf(input,
               "FrontFace 0x%08X}",
               &arg[0]) == 1)
    {
        ANALYZER_FUNC1(FrontFace, arg[0]);
    }
}

GLvoid ParseCullFace(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint arg[1];

    if (sscanf(input,
               "CullFace 0x%08X}",
               &arg[0]) == 1)
    {
        ANALYZER_FUNC1(CullFace, arg[0]);
    }
}

GLvoid ParseBlendFunc(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint arg[2];

    if (sscanf(input,
               "BlendFunc 0x%08X 0x%08X}",
               &arg[0], &arg[1]) == 2)
    {
        ANALYZER_FUNC2(BlendFunc, arg[0], arg[1]);
    }
}

GLvoid ParseBlendFuncSeparate(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint arg[4];

    if (sscanf(input,
           "BlendFuncSeparate 0x%08X 0x%08X 0x%08X 0x%08X}",
           &arg[0], &arg[1], &arg[2], &arg[3]) == 4)
    {
        ANALYZER_FUNC4(BlendFuncSeparate, arg[0], arg[1], arg[2], arg[3]);
    }
}

GLvoid ParseBlendColor(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint arg[4];

    if (sscanf(input,
               "BlendColor 0x%08X 0x%08X 0x%08X 0x%08X}",
               &arg[0], &arg[1], &arg[2], &arg[3]) == 4)
    {
        ANALYZER_FUNC4(BlendColor, *(GLfloat*)&arg[0], *(GLfloat*)&arg[1], *(GLfloat*)&arg[2], *(GLfloat*)&arg[3]);
    }
}

GLvoid ParseBlendEquation(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint arg[1];

    if (sscanf(input,
               "BlendEquation 0x%08X}",
               &arg[0]) == 1)
    {
        ANALYZER_FUNC1(BlendEquation, arg[0]);
    }
}

GLvoid ParseBlendEquationSeparate(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint arg[2];

    if (sscanf(input,
               "BlendEquationSeparate 0x%08X 0x%08X}",
               &arg[0], &arg[1]) == 2)
    {
        ANALYZER_FUNC2(BlendEquationSeparate, arg[0], arg[1]);
    }
}

GLvoid ParseGetError(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint arg[1];

    if (sscanf(input, "GetError := 0x%08X}", &arg[0]) == 1)
    {
        ANALYZER_FUNC1(GetError, arg[0]);
    }
}

GLvoid ParseGetUniformLocation(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint arg[3];

    if (sscanf(input,
               "GetUniformLocation 0x%08X (0x%08X) := 0x%08X",
               &arg[0], &arg[1], &arg[2]) == 3)
    {
        GLchar  *name   = (GLchar*)ReadData(READ_GL_UCHAR, arg[1], 0);
        GLuint  loc     = arg[2];
        GLuint  program = arg[0];
        ANALYZER_FUNC3(GetUniformLocation, program, name, loc);
    }
}

GLvoid ParseLinkProgram(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint arg[1];

    if (sscanf(input,
               "LinkProgram 0x%08X}",
               &arg[0]) == 1)
    {
        ANALYZER_FUNC1(LinkProgram, arg[0]);
    }
}

GLvoid ParseClearColor(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint arg[4];

    if (sscanf(input,
               "ClearColor 0x%08X 0x%08X 0x%08X 0x%08X}",
               &arg[0], &arg[1], &arg[2], &arg[3]) == 4)
    {
        GLfloat red     = *(GLfloat*)&arg[0];
        GLfloat green   = *(GLfloat*)&arg[1];
        GLfloat blue    = *(GLfloat*)&arg[2];
        GLfloat alpha   = *(GLfloat*)&arg[3];
        ANALYZER_FUNC4(ClearColor, red, green, blue, alpha);
    }
}

GLvoid ParseClearDepthf(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint arg[1];

    if (sscanf(input,
               "ClearDepthf 0x%08X}",
               &arg[0]) == 1)
    {
        GLfloat depth = *(GLfloat*)&arg[0];
        ANALYZER_FUNC1(ClearDepthf, depth);
    }
}

GLvoid ParseClearStencil(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint arg[1];

    if (sscanf(input,
               "ClearStencil 0x%08X}",
               &arg[0]) == 1)
    {
        ANALYZER_FUNC1(ClearStencil, arg[0]);
    }
}

GLvoid ParseClear(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint arg[1];

    if (sscanf(input,
               "Clear 0x%08X}",
               &arg[0]) == 1)
    {
        ANALYZER_FUNC1(Clear, arg[0]);
    }
}

GLvoid ParseColorMask(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint arg[4];

    if (sscanf(input,
               "ColorMask 0x%08X 0x%08X 0x%08X 0x%08X}",
               &arg[0], &arg[1], &arg[2], &arg[3]) == 4)
    {
        ANALYZER_FUNC4(ColorMask, arg[0], arg[1], arg[2], arg[3]);
    }
}

GLvoid ParseDepthMask(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint arg[1];

    if (sscanf(input,
               "DepthMask 0x%08X}",
               &arg[0]) == 1)
    {
        ANALYZER_FUNC1(DepthMask, arg[0]);
    }
}

GLvoid ParseStencilMask(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint arg[1];

    if (sscanf(input,
               "StencilMask 0x%08X}",
               &arg[0]) == 1)
    {
        ANALYZER_FUNC1(StencilMask, arg[0]);
    }
}

GLvoid ParseCreateShader(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint arg[2];

    if (sscanf(input,
               "CreateShader 0x%08X := 0x%08X",
               &arg[0], &arg[1]) == 2)
    {
        ANALYZER_FUNC2(CreateShader, arg[0], arg[1]);
    }
}

GLvoid ParseCreateProgram(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint arg[1];

    if (sscanf(input,
               "CreateProgram := 0x%08X}",
               &arg[0]) == 1)
    {
        ANALYZER_FUNC1(CreateProgram, arg[0]);
    }
}

GLvoid ParseCompileShader(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint arg[1];

    if (sscanf(input,
               "CompileShader 0x%08X}", &arg[0]) == 1)
    {
        ANALYZER_FUNC1(CompileShader, arg[0]);
    }
}

GLvoid ParseCopyTexImage2D(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint arg[8];

    if (sscanf(input,
               "CopyTexImage2D 0x%08X 0x%08X 0x%08X 0x%08X 0x%08X 0x%08X 0x%08X 0x%08X}",
               &arg[0], &arg[1], &arg[2], &arg[3], &arg[4], &arg[5], &arg[6], &arg[7]) == 8)
    {
        ANALYZER_FUNC8(CopyTexImage2D, arg[0], arg[1], arg[2], arg[3], arg[4], arg[5], arg[6], arg[7]);
    }
}

GLvoid ParseCopyTexSubImage2D(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint arg[8];

    if (sscanf(input,
               "CopyTexSubImage2D 0x%08X 0x%08X 0x%08X 0x%08X 0x%08X 0x%08X 0x%08X 0x%08X}",
               &arg[0], &arg[1], &arg[2], &arg[3], &arg[4], &arg[5], &arg[6], &arg[7]) == 8)
    {
        ANALYZER_FUNC8(CopyTexSubImage2D, arg[0], arg[1], arg[2], arg[3], arg[4], arg[5], arg[6], arg[7]);
    }
}

GLvoid ParseCheckFramebufferStatus(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint arg[2];

    if (sscanf(input,
               "CheckFramebufferStatus 0x%08X := 0x%08X",
               &arg[0], &arg[1]) == 2)
    {
        ANALYZER_FUNC2(CheckFramebufferStatus, arg[0], arg[1]);
    }
}

GLvoid ParseDisable(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint arg[1];

    if (sscanf(input,
               "Disable 0x%08X}",
               &arg[0]) == 1)
    {
        ANALYZER_FUNC1(Disable, arg[0]);
    }
}

GLvoid ParseDisableVertexAttribArray(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint arg[1];

    if (sscanf(input,
               "DisableVertexAttribArray 0x%08X}",
               &arg[0]) == 1)
    {
        ANALYZER_FUNC1(DisableVertexAttribArray, arg[0]);
    }
}

GLvoid ParseEnable(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint arg[1];

    if (sscanf(input,
               "Enable 0x%08X}",
               &arg[0]) == 1)
    {
        ANALYZER_FUNC1(Enable, arg[0]);
    }
}

GLvoid ParseEnableVertexAttribArray(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint arg[1];

    if (sscanf(input,
               "EnableVertexAttribArray 0x%08X}",
               &arg[0]) == 1)
    {
        ANALYZER_FUNC1(EnableVertexAttribArray, arg[0]);
    }
}

GLvoid ParseDrawArrays(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint arg[3];

    if (sscanf(input,
               "DrawArrays 0x%08X 0x%08X 0x%08X",
               &arg[0], &arg[1], &arg[2]) == 3)
    {
        GLenum  mode    = arg[0];
        GLint   first   = arg[1];
        GLsizei count   = arg[2];

        ANALYZER_FUNC3(DrawArrays, mode, first, count);
    }
}

GLvoid ParseDrawElements(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint arg[4];

    if (sscanf(input,
               "DrawElements 0x%08X 0x%08X 0x%08X (0x%08X)",
               &arg[0], &arg[1], &arg[2], &arg[3]) == 4)
    {
        GLenum  mode        = arg[0];
        GLint   indexCount  = arg[1];
        GLsizei indexType   = arg[2];
        GLuint  indexAddr   = arg[3];
        GLvoid  *indices    = (GLvoid*)ReadData(READ_GL_UCHAR, indexAddr, 0);

        ANALYZER_FUNC4(DrawElements, mode, indexCount, indexType, indices);
    }
}

GLvoid ParseDetachShader(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint arg[2];

    if (sscanf(input,
               "DetachShader 0x%08X 0x%08X}",
               &arg[0], &arg[1]) == 2)
    {
        ANALYZER_FUNC2(DetachShader, arg[0], arg[1]);
    }
}

GLvoid ParseDepthFunc(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint arg[1];

    if (sscanf(input,
               "DepthFunc 0x%08X}",
               &arg[0]) == 1)
    {
        ANALYZER_FUNC1(DepthFunc, arg[0]);
    }
}

GLvoid ParseDepthRangef(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint arg[2];

    if (sscanf(input,
               "DepthRangef 0x%08X 0x%08X}",
               &arg[0], &arg[1]) == 2)
    {
        ANALYZER_FUNC2(DepthRangef, *(GLfloat*)&arg[0], *(GLfloat*)&arg[1]);
    }
}

GLvoid ParseDeleteShader(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint arg[1];

    if (sscanf(input,
               "DeleteShader 0x%08X}",
               &arg[0]) == 1)
    {
        ANALYZER_FUNC1(DeleteShader, arg[0]);
    }
}

GLvoid ParseDeleteTextures(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint arg[2];

    if (sscanf(input,
               "DeleteTextures 0x%08X (0x%08X)",
               &arg[0], &arg[1]) == 2)
    {
        GLuint *buffers = (GLuint*)ReadData(READ_GL_UINT, arg[1], arg[0]);
        ANALYZER_FUNC2(DeleteTextures, arg[0], buffers);
    }
}

GLvoid ParseDeleteBuffers(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint arg[2];

    if (sscanf(input,
               "DeleteBuffers 0x%08X (0x%08X)",
               &arg[0], &arg[1]) == 2)
    {
        GLuint *buffers = (GLuint*)ReadData(READ_GL_UINT, arg[1], arg[0]);
        ANALYZER_FUNC2(DeleteBuffers, arg[0], buffers);
    }
}

GLvoid ParseDeleteVertexArrays(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint arg[2];

    if (sscanf(input,
               "DeleteVertexArrays 0x%08X (0x%08X)",
               &arg[0], &arg[1]) == 2)
    {
        GLuint *buffers = (GLuint*)ReadData(READ_GL_UINT, arg[1], arg[0]);
        ANALYZER_FUNC2(DeleteVertexArrays, arg[0], buffers);
    }
}

GLvoid ParseDeleteTransformFeedback(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint arg[2];

    if (sscanf(input,
               "DeleteTransformFeedbacks 0x%08X (0x%08X)",
               &arg[0], &arg[1]) == 2)
    {
        GLuint *buffers = (GLuint*)ReadData(READ_GL_UINT, arg[1], arg[0]);
        ANALYZER_FUNC2(DeleteTransformFeedbacks, arg[0], buffers);
    }
}

GLvoid ParseDeleteProgram(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint arg[1];

    if (sscanf(input,
               "DeleteProgram 0x%08X}",
               &arg[0]) == 1)
    {
        ANALYZER_FUNC1(DeleteProgram, arg[0]);
    }
}

GLvoid ParseDeleteRenderbuffers(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint arg[2];

    if (sscanf(input,
               "DeleteRenderbuffers 0x%08X (0x%08X)",
               &arg[0], &arg[1]) == 2)
    {
        GLuint *buffers = (GLuint*)ReadData(READ_GL_UINT, arg[1], arg[0]);
        ANALYZER_FUNC2(DeleteRenderbuffers, arg[0], buffers);
    }
}

GLvoid ParseDeleteFramebuffers(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint arg[2];

    if (sscanf(input,
               "DeleteFramebuffers 0x%08X (0x%08X)",
               &arg[0], &arg[1]) == 2)
    {
        GLuint *buffers = (GLuint*)ReadData(READ_GL_UINT, arg[1], arg[0]);
        ANALYZER_FUNC2(DeleteFramebuffers, arg[0], buffers);
    }
}

GLvoid ParseDiscardFramebufferEXT(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint arg[3];

    if (sscanf(input,
               "DiscardFramebufferEXT 0x%08X 0x%08X (0x%08X)",
               &arg[0], &arg[1], &arg[2]) == 3)
    {
        ANALYZER_FUNC3(DiscardFramebufferEXT, arg[0], arg[1], (GLenum*)arg[2]);
    }
}

GLvoid ParseEGLImageTargetRenderbufferStorageOES(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint arg[2];

    if (sscanf(input,
               "EGLImageTargetRenderbufferStorageOES 0x%08X 0x%08X}",
               &arg[0], &arg[1]) == 2)
    {
        ANALYZER_FUNC2(EGLImageTargetRenderbufferStorageOES, arg[0], (GLeglImageOES)arg[1]);
    }
}

GLvoid ParseEGLImageTargetTexture2DOES(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint arg[2];

    if (sscanf(input,
               "EGLImageTargetTexture2DOES 0x%08X 0x%08X}",
               &arg[0], &arg[1]) == 2)
    {
        ANALYZER_FUNC2(EGLImageTargetTexture2DOES, arg[0], (GLeglImageOES)arg[1]);
    }
}

GLvoid ParseGenTextures(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint arg[2];

    if (sscanf(input,
               "GenTextures 0x%08X (0x%08X)",
               &arg[0], &arg[1]) == 2)
    {
        GLuint *textures = (GLuint*)ReadData(READ_GL_UINT, arg[1], arg[0]);
        ANALYZER_FUNC2(GenTextures, arg[0], textures);
    }
}

GLvoid ParseGenFramebuffers(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint arg[2];

    if (sscanf(input,
               "GenFramebuffers 0x%08X (0x%08x)",
               &arg[0], &arg[1]) == 2)
    {
        GLuint *buffers = (GLuint*)ReadData(READ_GL_UINT, arg[1], arg[0]);
        ANALYZER_FUNC2(GenFramebuffers, arg[0], buffers);
    }
}

GLvoid ParseGenRenderbuffers(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint arg[2];

    if (sscanf(input,
               "GenRenderbuffers 0x%08X (0x%08x)",
               &arg[0], &arg[1]) == 2)
    {
        GLuint *buffers = (GLuint*)ReadData(READ_GL_UINT, arg[1], arg[0]);
        ANALYZER_FUNC2(GenRenderbuffers, arg[0], buffers);
    }
}

GLvoid ParseGenBuffers(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint arg[2];

    if (sscanf(input,
               "GenBuffers 0x%08X (0x%08X)",
               &arg[0], &arg[1]) == 2)
    {
        GLuint *buffers = (GLuint*)ReadData(READ_GL_UINT, arg[1], arg[0]);
        ANALYZER_FUNC2(GenBuffers, arg[0], buffers);
    }
}

GLvoid ParseGenVertexArrays(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint arg[2];

    if (sscanf(input,
               "GenVertexArrays 0x%08X (0x%08X)",
               &arg[0], &arg[1]) == 2)
    {
        GLuint *buffers = (GLuint*)ReadData(READ_GL_UINT, arg[1], arg[0]);
        ANALYZER_FUNC2(GenVertexArrays, arg[0], buffers);
    }
}

GLvoid ParseGenTransformFeedbacks(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint arg[2];

    if (sscanf(input,
               "GenTransformFeedbacks 0x%08X (0x%08X)",
               &arg[0], &arg[1]) == 2)
    {
        GLuint *buffers = (GLuint*)ReadData(READ_GL_UINT, arg[1], arg[0]);
        ANALYZER_FUNC2(GenTransformFeedbacks, arg[0], buffers);
    }
}

GLvoid ParseGenerateMipmap(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint arg[1];

    if (sscanf(input,
               "GenerateMipmap 0x%08X}",
               &arg[0]) == 1)
    {
        ANALYZER_FUNC1(GenerateMipmap, arg[0]);
    }
}

GLvoid ParseGetShaderiv(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint arg[3];

    if (sscanf(input,
               "GetShaderiv 0x%08X 0x%08X (0x%08X)",
               &arg[0], &arg[1], &arg[2]) == 3)
    {
        GLint v = ReadUInt(arg[2]);
        ANALYZER_FUNC3(GetShaderiv, arg[0], arg[1], &v);
    }
}

GLvoid ParseGetShaderInfoLog(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint arg[4];

    if (sscanf(input,
               "GetShaderInfoLog 0x%08X 0x%08X (0x%08X) (0x%08X)",
               &arg[0], &arg[1], &arg[2], &arg[3]) == 4)
    {
        GLchar  *v = (GLchar*)ReadData(READ_GL_UCHAR, arg[3], 0);
        ANALYZER_FUNC4(GetShaderInfoLog, arg[0], arg[1], (GLsizei*)&arg[2], v);
    }
}

GLvoid ParseGetShaderSource(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint arg[4];

    if (sscanf(input,
               "GetShaderSource 0x%08X 0x%08X (0x%08X) (0x%08X)",
               &arg[0], &arg[1], &arg[2], &arg[3]) == 4)
    {
        ANALYZER_FUNC4(GetShaderSource, arg[0], arg[1], (GLsizei*)&arg[2], (GLchar*)&arg[3]);
    }
}

GLvoid ParseGetProgramiv(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint arg[3];

    if (sscanf(input,
               "GetProgramiv 0x%08X 0x%08X (0x%08X)",
               &arg[0], &arg[1], &arg[2]) == 3)
    {
        GLint v = ReadUInt(arg[2]);
        ANALYZER_FUNC3(GetProgramiv, arg[0], arg[1], &v);
    }
}

GLvoid ParseGetActiveAttrib(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint arg[7];

    if (sscanf(input,
               "GetActiveAttrib 0x%08X 0x%08X 0x%08X (0x%08X) (0x%08X) (0x%08X) (0x%08X)",
               &arg[0], &arg[1], &arg[2], &arg[3], &arg[4], &arg[5], &arg[6]) == 7)
    {
        GLuint  program = arg[0];
        GLuint  index   = arg[1];
        GLsizei bufsize = arg[2];
        GLsizei length  = ReadUInt(arg[3]);
        GLint   size    = ReadUInt(arg[4]);
        GLenum  type    = ReadUInt(arg[5]);
        GLchar  *name   = (GLchar*)ReadData(READ_GL_UCHAR, arg[6], 0);

        ANALYZER_FUNC7(GetActiveAttrib, program, index, bufsize, &length, &size, &type, name);
    }
}

GLvoid ParseGetAttribLocation(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint arg[3];

    if (sscanf(input,
               "GetAttribLocation 0x%08X (0x%08X) := 0x%08X",
               &arg[0], &arg[1], &arg[2]) == 3)
    {
        GLuint  id      = arg[0];
        GLchar  *name   = (GLchar*)ReadData(READ_GL_UCHAR, arg[1], 0);
        GLuint  loc     = arg[2];
        ANALYZER_FUNC3(GetAttribLocation, id, name, loc);
    }
}

GLvoid ParseGetAttachedShaders(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint arg[4];

    if (sscanf(input,
               "GetAttachedShaders 0x%08X 0x%08X (0x%08X) (0x%08X)",
               &arg[0], &arg[1], &arg[2], &arg[3]) == 4)
    {
        GLuint  prog     = arg[0];
        GLuint  maxCount = arg[1];
        GLint   count    = ReadUInt(arg[2]);
        GLuint  *name    = (GLuint*)ReadData(READ_GL_UINT, arg[3], count);

        ANALYZER_FUNC4(GetAttachedShaders, prog, maxCount, &count, name);
    }
}

GLvoid ParseGetProgramInfoLog(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint arg[4];

    if (sscanf(input,
               "GetProgramInfoLog 0x%08X 0x%08X (0x%08X) (0x%08X)",
               &arg[0], &arg[1], &arg[2], &arg[3]) == 4)
    {
        GLchar  *v = (GLchar*)ReadData(READ_GL_UCHAR, arg[3], 0);
        ANALYZER_FUNC4(GetProgramInfoLog, arg[0], arg[1], (GLsizei*)&arg[2], v);
    }
}

GLvoid ParseGetVertexAttribfv(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint arg[3];

    if (sscanf(input,
               "GetVertexAttribfv 0x%08X 0x%08X (0x%08X)",
               &arg[0], &arg[1], &arg[2]) == 3)
    {
        GLuint  index   = arg[0];
        GLuint  value   = ReadUInt(arg[2]);
        GLfloat f       = *(GLfloat*)&value;
        ANALYZER_FUNC3(GetVertexAttribfv, index, arg[1], &f);
    }
}

GLvoid ParseGetVertexAttribPointerv(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint arg[3];

    if (sscanf(input,
               "GetVertexAttribPointerv 0x%08X 0x%08X (0x%08X)",
               &arg[0], &arg[1], &arg[2]) == 3)
    {
        GLuint pointer = ReadUInt(arg[2]);
        ANALYZER_FUNC3(GetVertexAttribPointerv, arg[0], arg[1], (GLvoid**)pointer);
    }
}

GLvoid ParseGetVertexAttribiv(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint arg[3];

    if (sscanf(input,
               "GetVertexAttribiv 0x%08X 0x%08X (0x%08X)",
               &arg[0], &arg[1], &arg[2]) == 3)
    {
        GLuint  index   = arg[0];
        GLint   value   = ReadUInt(arg[2]);
        ANALYZER_FUNC3(GetVertexAttribiv, index, arg[1], &value);
    }
}

GLvoid ParseGetBufferParameteriv(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint arg[3];

    if (sscanf(input,
               "GetBufferParameteriv 0x%08X 0x%08X (0x%08X)",
               &arg[0], &arg[1], &arg[2]) == 3)
    {
        ANALYZER_FUNC3(GetBufferParameteriv, arg[0], arg[1], (GLint*)arg[2]);
    }
}

GLvoid ParseGetString(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint arg[2];

#ifdef API_DUMP
    if (sscanf(input, "GetString 0x%08X := 0x%08X", &arg[0], &arg[1]) == 2)
#else
    if (sscanf(input, "GetString 0x%08X :=", &arg[0]) == 1)
#endif
    {
        GLubyte *v = (GLubyte*)ReadData(READ_GL_UCHAR, arg[1], 0);
        ANALYZER_FUNC2(GetString, arg[0], v);
    }
}

GLvoid ParseGetIntegerv(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint arg[2];

    if (sscanf(input,
               "GetIntegerv 0x%08X (0x%08X)",
               &arg[0], &arg[1]) == 2)
    {
        GLint v = ReadUInt(arg[1]);
        ANALYZER_FUNC2(GetIntegerv, arg[0], &v);
    }
}

GLvoid ParseGetBooleanv(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint arg[2];

    if (sscanf(input,
               "GetBooleanv 0x%08X (0x%08X)",
               &arg[0], &arg[1]) == 2)
    {
        GLuint  v = ReadUInt(arg[1]);
        ANALYZER_FUNC2(GetBooleanv, arg[0], (GLboolean*)&v);
    }
}

GLvoid ParseGetFloatv(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint arg[2];

    if (sscanf(input,
               "GetFloatv 0x%08X (0x%08X)",
               &arg[0], &arg[1]) == 2)
    {
        GLuint  v = ReadUInt(arg[1]);
        GLfloat f = *(GLfloat*)&v;
        ANALYZER_FUNC2(GetFloatv, arg[0], &f);
    }
}

GLvoid ParseGetShaderPrecisionFormat(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint arg[4];

    if (sscanf(input,
               "GetShaderPrecisionFormat 0x%08X 0x%08X (0x%08X) (0x%08X)",
               &arg[0], &arg[1], &arg[2], &arg[3]) == 4)
    {
        ANALYZER_FUNC4(GetShaderPrecisionFormat, arg[0], arg[1], (GLint*)arg[2], (GLint*)arg[3]);
    }
}

GLvoid ParseGetRenderbufferParameteriv(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint arg[3];

    if (sscanf(input,
               "GetRenderbufferParameteriv 0x%08X 0x%08X (0x%08X)",
               &arg[0], &arg[1], &arg[2]) == 3)
    {
        ANALYZER_FUNC3(GetRenderbufferParameteriv, arg[0], arg[1], (GLint*)arg[2]);
    }
}

GLvoid ParseGetTexParameteriv(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint arg[3];

    if (sscanf(input,
               "GetTexParameteriv 0x%08X 0x%08X (0x%08X)",
               &arg[0], &arg[1], &arg[2]) == 3)
    {
        GLint v = ReadUInt(arg[2]);
        ANALYZER_FUNC3(GetTexParameteriv, arg[0], arg[1], &v);
    }
}

GLvoid ParseGetTexParameterfv(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint arg[3];

    if (sscanf(input,
               "GetTexParameterfv 0x%08X 0x%08X (0x%08X)",
               &arg[0], &arg[1], &arg[2]) == 3)
    {
        ANALYZER_FUNC3(GetTexParameterfv, arg[0], arg[1], (GLfloat*)arg[2]);
    }
}

GLvoid ParseGetUniformfv(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint arg[3];

    if (sscanf(input,
               "GetUniformfv 0x%08X 0x%08X (0x%08x)",
               &arg[0], &arg[1], &arg[2]) == 3)
    {
        ANALYZER_FUNC3(GetUniformfv, arg[0], arg[1], (GLfloat*)arg[2]);
    }
}

GLvoid ParseGetUniformiv(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint arg[3];

    if (sscanf(input,
               "GetUniformiv 0x%08X 0x%08X (0x%08x)",
               &arg[0], &arg[1], &arg[2]) == 3)
    {
        ANALYZER_FUNC3(GetUniformiv, arg[0], arg[1], (GLint*)arg[2]);
    }
}

GLvoid ParseGetFramebufferAttachmentParameteriv(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint arg[4];

    if (sscanf(input,
               "GetFramebufferAttachmentParameteriv 0x%08X 0x%08X 0x%08X (0x%08X)",
               &arg[0], &arg[1], &arg[2], &arg[3]) == 4)
    {
        ANALYZER_FUNC4(GetFramebufferAttachmentParameteriv, arg[0], arg[1], arg[2], (GLint*)arg[3]);
    }
}

GLvoid ParseHint(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint arg[2];

    if (sscanf(input,
               "Hint 0x%08X 0x%08X}",
               &arg[0], &arg[1]) == 2)
    {
        ANALYZER_FUNC2(Hint, arg[0], arg[1]);
    }
}

GLvoid ParseIsBuffer(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint arg[2];

    if (sscanf(input,
           "IsBuffer 0x%08X := 0x%08X",
           &arg[0], &arg[1]) == 2)
    {
        ANALYZER_FUNC2(IsBuffer, arg[0], arg[1]);
    }
}

GLvoid ParseIsShader(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint arg[2];

    if (sscanf(input,
               "IsShader 0x%08X := 0x%08X",
               &arg[0], &arg[1]) == 2)
    {
        ANALYZER_FUNC2(IsShader, arg[0], arg[1]);
    }
}

GLvoid ParseIsProgram(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint arg[2];

    if (sscanf(input,
               "IsProgram 0x%08X := 0x%08X",
               &arg[0], &arg[1]) == 2)
    {
        ANALYZER_FUNC2(IsProgram, arg[0], arg[1]);
    }
}

GLvoid ParseIsRenderbuffer(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint arg[2];

    if (sscanf(input,
               "IsRenderbuffer 0x%08X := 0x%08X",
               &arg[0], &arg[1]) == 2)
    {
        ANALYZER_FUNC2(IsRenderbuffer, arg[0], arg[1]);
    }
}

GLvoid ParseIsEnabled(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint arg[2];

    if (sscanf(input,
               "IsEnabled 0x%08X := 0x%08X",
               &arg[0], &arg[1]) == 2)
    {
        ANALYZER_FUNC2(IsEnabled, arg[0], arg[1]);
    }
}

GLvoid ParseIsTexture(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint arg[2];

    if (sscanf(input,
               "IsTexture 0x%08X := 0x%08X",
               &arg[0], &arg[1]) == 2)
    {
        ANALYZER_FUNC2(IsTexture, arg[0], arg[1]);
    }
}

GLvoid ParseIsFramebuffer(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint arg[2];

    if (sscanf(input,
               "IsFramebuffer 0x%08X := 0x%08X",
               &arg[0], &arg[1]) == 2)
    {
        ANALYZER_FUNC2(IsFramebuffer, arg[0], arg[1]);
    }
}

GLvoid ParseLineWidth(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint arg[1];

    if (sscanf(input, "LineWidth 0x%08X}", &arg[0]) == 1)
    {
        ANALYZER_FUNC1(LineWidth, *(GLfloat*)&arg[0]);
    }
}

GLvoid ParsePixelStorei(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint arg[2];

    if (sscanf(input,
               "PixelStorei 0x%08X 0x%08X}",
               &arg[0], &arg[1]) == 2)
    {
        ANALYZER_FUNC2(PixelStorei, arg[0], arg[1]);
    }
}

GLvoid ParsePolygonOffset(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint arg[2];

    if (sscanf(input,
               "PolygonOffset 0x%08X 0x%08X}",
               &arg[0], &arg[1]) == 2)
    {
        ANALYZER_FUNC2(PolygonOffset, *(GLfloat*)&arg[0], *(GLfloat*)&arg[1]);
    }
}

GLvoid ParseProgramBinaryOES(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint arg[4];

    if (sscanf(input,
               "ProgramBinaryOES 0x%08X 0x%08X (0x%08X) 0x%08X",
               &arg[0], &arg[1], &arg[2], &arg[3]) == 4)
    {
        ANALYZER_FUNC4(ProgramBinaryOES, arg[0], arg[1], (GLvoid*)arg[2], arg[3]);
    }
}

GLvoid ParseProgramBinary(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint arg[4];

    if (sscanf(input,
               "ProgramBinary 0x%08X 0x%08X (0x%08X) 0x%08X",
               &arg[0], &arg[1], &arg[2], &arg[3]) == 4)
    {
        ANALYZER_FUNC4(ProgramBinary, arg[0], arg[1], (GLvoid*)arg[2], arg[3]);
    }
}

GLvoid ParseReadPixels(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint arg[7];

    if (sscanf(input,
               "ReadPixels 0x%08X 0x%08X 0x%08X 0x%08X 0x%08X 0x%08X (0x%08X)",
               &arg[0], &arg[1], &arg[2], &arg[3], &arg[4], &arg[5], &arg[6]) == 7)
    {
        GLint   x       = arg[0];
        GLint   y       = arg[1];
        GLsizei width   = arg[2];
        GLsizei height  = arg[3];
        GLenum  format  = arg[4];
        GLenum  type    = arg[5];
        ANALYZER_FUNC7(ReadPixels, x, y, width, height, format, type, (GLvoid*)arg[6]);
    }
}

GLvoid ParseScissor(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint arg[4];

    if (sscanf(input,
               "Scissor 0x%08X 0x%08X 0x%08X 0x%08X}",
               &arg[0], &arg[1], &arg[2], &arg[3]) == 4)
    {
        ANALYZER_FUNC4(Scissor, arg[0], arg[1], arg[2], arg[3]);
    }
}

GLvoid ParseStencilFunc(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint arg[3];

    if (sscanf(input,
               "StencilFunc 0x%08X 0x%08X 0x%08X}",
               &arg[0], &arg[1], &arg[2]) == 3)
    {
        ANALYZER_FUNC3(StencilFunc, arg[0], arg[1], arg[2]);
    }
}

GLvoid ParseStencilFuncSeparate(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint arg[4];

    if (sscanf(input,
               "StencilFuncSeparate 0x%08X 0x%08X 0x%08X 0x%08X}",
               &arg[0], &arg[1], &arg[2], &arg[3]) == 4)
    {
        ANALYZER_FUNC4(StencilFuncSeparate, arg[0], arg[1], arg[2], arg[3]);
    }
}

GLvoid ParseStencilMaskSeparate(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint arg[2];

    if (sscanf(input,
               "StencilMaskSeparate 0x%08X 0x%08X}",
               &arg[0], &arg[1]) == 2)
    {
        ANALYZER_FUNC2(StencilMaskSeparate, arg[0], arg[1]);
    }
}

GLvoid ParseStencilOp(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint arg[3];

    if (sscanf(input,
               "StencilOp 0x%08X 0x%08X 0x%08X}",
               &arg[0], &arg[1], &arg[2]) == 3)
    {
        ANALYZER_FUNC3(StencilOp, arg[0], arg[1], arg[2]);
    }
}

GLvoid ParseStencilOpSeparate(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint arg[4];

    if (sscanf(input,
               "StencilOpSeparate 0x%08X 0x%08X 0x%08X 0x%08X}",
               &arg[0], &arg[1], &arg[2], &arg[3]) == 4)
    {
        ANALYZER_FUNC4(StencilOpSeparate, arg[0], arg[1], arg[2], arg[3]);
    }
}

GLvoid ParseShaderSource(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint arg[4];

    if (sscanf(input,
               "ShaderSource 0x%08X 0x%08X (0x%08X) (0x%08X)",
               &arg[0], &arg[1], &arg[2], &arg[3]) == 4)
    {
        ANALYZER_FUNC4(ShaderSource, arg[0], arg[1], (const GLchar *const*)arg[2], (const GLint*)arg[3]);
    }
}

GLvoid ParseSampleCoverage(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint arg[2];

    if (sscanf(input,
               "SampleCoverage 0x%08X 0x%08X}",
               &arg[0], &arg[1]) == 2)
    {
        ANALYZER_FUNC2(SampleCoverage, *(GLfloat*)arg[0], arg[1]);
    }
}

GLvoid ParseUniform1f(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint arg[2];

    if (sscanf(input,
               "Uniform1f 0x%08X 0x%08X}",
               &arg[0], &arg[1]) == 2)
    {
        ANALYZER_FUNC2(Uniform1f, arg[0], *(GLfloat*)&arg[1]);
    }
}

GLvoid ParseUniform1fv(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint arg[3];
    GLint pos = 0;

    if (sscanf(input,
               "Uniform1fv 0x%08X 0x%08X (0x%08X)",
               &arg[0], &arg[1], &arg[2]) == 3)
    {
        ANALYZER_FUNC3(Uniform1fv, arg[0], arg[1], (GLfloat*)arg[2]);
    }
}

GLvoid ParseUniform1i(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint arg[2];

    if (sscanf(input,
               "Uniform1i 0x%08X 0x%08X}",
               &arg[0], &arg[1]) == 2)
    {
        ANALYZER_FUNC2(Uniform1i, arg[0], arg[1]);
    }
}

GLvoid ParseUniform1iv(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint arg[3];
    GLint pos = 0;

    if (sscanf(input,
               "Uniform1iv 0x%08X 0x%08X (0x%08X)",
               &arg[0], &arg[1], &arg[2]) == 3)
    {
        ANALYZER_FUNC3(Uniform1iv, arg[0], arg[1], (GLint*)arg[2]);
    }
}

GLvoid ParseUniform2f(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint arg[3];

    if (sscanf(input,
               "Uniform2f 0x%08X 0x%08X 0x%08X}",
               &arg[0], &arg[1], &arg[2]) == 3)
    {
        ANALYZER_FUNC3(Uniform2f, arg[0], *(GLfloat*)arg[1], *(GLfloat*)arg[2]);
    }
}

GLvoid ParseUniform2fv(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint arg[3];
    GLint pos = 0;

    if (sscanf(input,
               "Uniform2fv 0x%08X 0x%08X (0x%08X)",
               &arg[0], &arg[1], &arg[2]) == 3)
    {
        ANALYZER_FUNC3(Uniform2fv, arg[0], arg[1], (GLfloat*)arg[2]);
    }
}

GLvoid ParseUniform2i(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint arg[3];

    if (sscanf(input,
               "Uniform2i 0x%08X 0x%08X 0x%08X}",
               &arg[0], &arg[1], &arg[2]) == 3)
    {
        ANALYZER_FUNC3(Uniform2i, arg[0], arg[1], arg[2]);
    }
}

GLvoid ParseUniform2iv(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint arg[3];
    GLint pos = 0;

    if (sscanf(input,
               "Uniform2iv 0x%08X 0x%08X (0x%08X)",
               &arg[0], &arg[1], &arg[2]) == 3)
    {
        ANALYZER_FUNC3(Uniform2iv, arg[0], arg[1], (GLint*)arg[2]);
    }
}

GLvoid ParseUniform3f(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint arg[4];

    if (sscanf(input,
               "Uniform3f 0x%08X 0x%08X 0x%08X 0x%08X}",
               &arg[0], &arg[1], &arg[2], &arg[3]) == 4)
    {
        ANALYZER_FUNC4(Uniform3f, arg[0], *(GLfloat*)arg[1], *(GLfloat*)arg[2], *(GLfloat*)arg[3]);
    }
}

GLvoid ParseUniform3fv(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint arg[3];
    GLint pos = 0;

    if (sscanf(input,
               "Uniform3fv 0x%08X 0x%08X (0x%08X)",
               &arg[0], &arg[1], &arg[2]) == 3)
    {
        ANALYZER_FUNC3(Uniform3fv, arg[0], arg[1], (GLfloat*)arg[2]);
    }
}

GLvoid ParseUniform3i(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint arg[4];

    if (sscanf(input,
               "Uniform3i 0x%08X 0x%08X 0x%08X 0x%08X}",
               &arg[0], &arg[1], &arg[2], &arg[3]) == 4)
    {
        ANALYZER_FUNC4(Uniform3i, arg[0], arg[1], arg[2], arg[3]);
    }
}

GLvoid ParseUniform3iv(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint arg[3];
    GLint pos = 0;

    if (sscanf(input,
               "Uniform3iv 0x%08X 0x%08X (0x%08X)",
               &arg[0], &arg[1], &arg[2]) == 3)
    {
        ANALYZER_FUNC3(Uniform3iv, arg[0], arg[1], (GLint*)arg[2]);
    }
}

GLvoid ParseUniform4f(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint arg[5];

    if (sscanf(input,
               "Uniform4f 0x%08X 0x%08X 0x%08X 0x%08X 0x%08X}",
               &arg[0], &arg[1], &arg[2], &arg[3], &arg[4]) == 5)
    {
        ANALYZER_FUNC5(Uniform4f, arg[0], *(GLfloat*)arg[1], *(GLfloat*)arg[2], *(GLfloat*)arg[3], *(GLfloat*)arg[4]);
    }
}

GLvoid ParseUniform4fv(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint arg[3];
    GLint pos = 0;

    if (sscanf(input,
               "Uniform4fv 0x%08X 0x%08X (0x%08X)",
               &arg[0], &arg[1], &arg[2]) == 3)
    {
        ANALYZER_FUNC3(Uniform4fv, arg[0], arg[1], (GLfloat*)arg[2]);
    }
}

GLvoid ParseUniform4i(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint arg[5];

    if (sscanf(input,
               "Uniform4i 0x%08X 0x%08X 0x%08X 0x%08X 0x%08X}",
               &arg[0], &arg[1], &arg[2], &arg[3], &arg[4]) == 5)
    {
        ANALYZER_FUNC5(Uniform4i, arg[0], arg[1], arg[2], arg[3], arg[4]);
    }
}

GLvoid ParseUniform4iv(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint arg[3];
    GLint pos = 0;

    if (sscanf(input,
               "Uniform4iv 0x%08X 0x%08X (0x%08X)",
               &arg[0], &arg[1], &arg[2]) == 3)
    {
        ANALYZER_FUNC3(Uniform4iv, arg[0], arg[1], (GLint*)arg[2]);
    }
}

GLvoid ParseUniformMatrix2fv(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint arg[4];
    GLint pos = 0;

    if (sscanf(input,
               "UniformMatrix2fv 0x%08X 0x%08X 0x%08X (0x%08X)",
               &arg[0], &arg[1], &arg[2], &arg[3]) == 4)
    {
        ANALYZER_FUNC4(UniformMatrix2fv, arg[0], arg[1], arg[2], (GLfloat*)arg[3]);
    }
}

GLvoid ParseUniformMatrix3fv(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint arg[4];
    GLint pos = 0;

    if (sscanf(input,
               "UniformMatrix3fv 0x%08X 0x%08X 0x%08X (0x%08X)",
               &arg[0], &arg[1], &arg[2], &arg[3]) == 4)
    {
        ANALYZER_FUNC4(UniformMatrix3fv, arg[0], arg[1], arg[2], (GLfloat*)arg[3]);
    }
}

GLvoid ParseUniformMatrix4fv(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint arg[4];
    GLint pos = 0;

    if (sscanf(input,
               "UniformMatrix4fv 0x%08X 0x%08X 0x%08X (0x%08X)",
               &arg[0], &arg[1], &arg[2], &arg[3]) == 4)
    {
        ANALYZER_FUNC4(UniformMatrix4fv, arg[0], arg[1], arg[2], (GLfloat*)arg[3]);
    }
}

GLvoid ParseUseProgram(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint arg[1];

    if (sscanf(input,
               "UseProgram 0x%08X}",
               &arg[0]) == 1)
    {
        ANALYZER_FUNC1(UseProgram, arg[0]);
    }
}

GLvoid PrintVertexAttrib1234fv(GLchar *output,
                               GLuint outputSize,
                               GLuint index,
                               GLuint count,
                               GLuint handle,
                               const GLchar *funcName)
{
    GLfloat *values = (GLfloat*)ReadData(READ_GL_UINT, handle, 2);

    switch (count)
    {
    case 1:
        sprintf(output, "%s(index=%d, %f)\n", funcName, index, *values);
        break;
    case 2:
        sprintf(output, "%s(index=%d, %f, %f)\n", funcName, index, *values, *(values+1));
        break;
    case 3:
        sprintf(output, "%s(index=%d, %f, %f, %f)\n", funcName, index, *values, *(values+1), *(values+2));
        break;
    case 4:
        sprintf(output, "%s(index=%d, %f, %f, %f, %f)\n", funcName, index, *values, *(values+1), *(values+2), *(values+3));
        break;
    }

    FreeData(values);
}

GLvoid PrintVertexAttrib1234f(GLchar *output,
                              GLuint outputSize,
                              GLuint index,
                              GLuint count,
                              GLfloat f1,
                              GLfloat f2,
                              GLfloat f3,
                              GLfloat f4,
                              const GLchar *funcName)
{
    switch (count)
    {
    case 1:
        sprintf(output, "%s(index=%d, %f)\n", funcName, index, f1);
        break;
    case 2:
        sprintf(output, "%s(index=%d, %f, %f)\n", funcName, index, f1, f2);
        break;
    case 3:
        sprintf(output, "%s(index=%d, %f, %f, %f)\n", funcName, index, f1, f2, f3);
        break;
    case 4:
        sprintf(output, "%s(index=%d, %f, %f, %f, %f)\n", funcName, index, f1, f2, f3, f4);
        break;
    }
}

GLvoid ParseVertexAttrib1fv(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint arg[2];

    if (sscanf(input,
               "VertexAttrib1fv 0x%08X (0x%08X)",
               &arg[0], &arg[1]) == 2)
    {
        PrintVertexAttrib1234fv(output, outputSize, arg[0], 1, arg[1], "glVertexAttrib1fv");
    }
}

GLvoid ParseVertexAttrib2fv(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint arg[2];

    if (sscanf(input,
               "VertexAttrib2fv 0x%08X (0x%08X)",
               &arg[0], &arg[1]) == 2)
    {
        PrintVertexAttrib1234fv(output, outputSize, arg[0], 2, arg[1], "glVertexAttrib2fv");
    }
}

GLvoid ParseVertexAttrib3fv(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint arg[2];

    if (sscanf(input,
               "VertexAttrib3fv 0x%08X (0x%08X)",
               &arg[0], &arg[1]) == 2)
    {
        PrintVertexAttrib1234fv(output, outputSize, arg[0], 3, arg[1], "glVertexAttrib3fv");
    }
}

GLvoid ParseVertexAttrib4fv(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint arg[2];

    if (sscanf(input,
               "VertexAttrib4fv 0x%08X (0x%08X)",
               &arg[0], &arg[1]) == 2)
    {
        PrintVertexAttrib1234fv(output, outputSize, arg[0], 4, arg[1], "glVertexAttrib4fv");
    }
}

GLvoid ParseVertexAttrib1f(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint arg[2];

    if (sscanf(input,
               "VertexAttrib1f 0x%08X 0x%08X}",
               &arg[0], &arg[1]) == 2)
    {
        PrintVertexAttrib1234f(output, outputSize, arg[0], 1, *(GLfloat*)&arg[1], 0, 0, 0, "glVertexAttrib1f");
    }
}

GLvoid ParseVertexAttrib2f(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint arg[3];

    if (sscanf(input,
               "VertexAttrib2f 0x%08X 0x%08X 0x%08X}",
               &arg[0], &arg[1], &arg[2]) == 3)
    {
        PrintVertexAttrib1234f(output, outputSize, arg[0], 2, *(GLfloat*)&arg[1], *(GLfloat*)&arg[2], 0, 0, "glVertexAttrib2f");
    }
}

GLvoid ParseVertexAttrib3f(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint arg[4];

    if (sscanf(input,
               "VertexAttrib3f 0x%08X 0x%08X 0x%08X 0x%08X}",
               &arg[0], &arg[1], &arg[2], &arg[3]) == 4)
    {
        PrintVertexAttrib1234f(output, outputSize, arg[0], 3, *(GLfloat*)&arg[1], *(GLfloat*)&arg[2], *(GLfloat*)&arg[3], 0, "glVertexAttrib3f");
    }
}

GLvoid ParseVertexAttrib4f(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint arg[5];

    if (sscanf(input,
               "VertexAttrib4f 0x%08X 0x%08X 0x%08X 0x%08X 0x%08X}",
               &arg[0], &arg[1], &arg[2], &arg[3], &arg[4]) == 5)
    {
        PrintVertexAttrib1234f(output, outputSize, arg[0], 4, *(GLfloat*)&arg[1], *(GLfloat*)&arg[2], *(GLfloat*)&arg[3], *(GLfloat*)&arg[4], "glVertexAttrib4f");
    }
}

GLvoid ParseViewport(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint arg[4];

    if (sscanf(input,
               "Viewport 0x%08X 0x%08X 0x%08X 0x%08X}",
               &arg[0], &arg[1], &arg[2], &arg[3]) == 4)
    {
        ANALYZER_FUNC4(Viewport, arg[0], arg[1], arg[2], arg[3]);
    }
}

GLvoid ParseValidateProgram(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint arg[1];

    if (sscanf(input,
               "ValidateProgram 0x%08X}",
               &arg[0]) == 1)
    {
        ANALYZER_FUNC1(ValidateProgram, arg[0]);
    }
}