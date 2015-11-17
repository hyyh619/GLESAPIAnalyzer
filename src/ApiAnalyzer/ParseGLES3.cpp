#include "utils.h"
#include "bitmap.h"
#include "MemoryPool.h"
#include "TranslateGL.h"
#include "ParseGLES3.h"
#include "GLESShare.h"
#include "gles3.h"
#include "Analyzer.h"

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
        CTexObj     *pTexObj    = CURRENT_CONTEXT1().GetActiveTexObj(target);
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
        CTexObj     *pTexObj    = CURRENT_CONTEXT1().GetActiveTexObj(target);
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
        CTexObj     *pTexObj        = CURRENT_CONTEXT1().GetActiveTexObj(target);
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
        CTexObj     *pTexObj    = CURRENT_CONTEXT1().GetActiveTexObj(target);
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
        CTexObj     *pTexObj        = CURRENT_CONTEXT1().GetActiveTexObj(target);
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

        CTexObj     *pTexObj    = CURRENT_CONTEXT1().GetActiveTexObj(target);
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
        GLenum      target      = arg[0];
        GLsizei     levels      = arg[1];
        GLenum      format      = arg[2];
        GLsizei     width       = arg[3];
        GLsizei     height      = arg[4];
        GLsizei     depth       = arg[5];
        GLsizei     levelWidth  = width;
        GLsizei     levelHeight = height;
        GLsizei     levelDepth  = depth;
        CTexObj     *pTexObj    = CURRENT_CONTEXT1().GetActiveTexObj(target);

        TranslateTexTarget(target, tmp1);
        TranslateTexFormat(format, tmp2);
        sprintf(output, "glTexStorage3D(%s, levels=%d, %s, w=%d, h=%d, d=%d)\n", tmp1, levels, tmp2, width, height, depth);
    }
}

GLvoid ParseTexStorage2D(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint arg[10];

    if (sscanf(input,
        "TexStorage2D 0x%08X 0x%08X 0x%08X 0x%08X 0x%08X}",
               &arg[0], &arg[1], &arg[2], &arg[3], &arg[4]) == 5)
    {
        GLenum      target      = arg[0];
        GLsizei     levels      = arg[1];
        GLenum      format      = arg[2];
        GLsizei     width       = arg[3];
        GLsizei     height      = arg[4];
        GLsizei     levelWidth  = width;
        GLsizei     levelHeight = height;
        CTexObj     *pTexObj    = CURRENT_CONTEXT1().GetActiveTexObj(target);

        TranslateTexTarget(target, tmp1);
        TranslateTexFormat(format, tmp2);
        sprintf(output, "glTexStorage2D(%s, levels=%d, %s, w=%d, h=%d)\n", tmp1, levels, tmp2, width, height);
    }
}

GLvoid ParseBindTexture(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint arg[2];
    const GLchar *ptr;

    if (sscanf(input, "BindTexture 0x%08X 0x%08X}", &arg[0], &arg[1]) == 2)
    {
        switch (arg[0])
        {
        case GL_TEXTURE_2D:
            ptr = "GL_TEXTURE_2D";
            break;

        case GL_TEXTURE_3D:
            ptr = "GL_TEXTURE_3D";
            break;

        case GL_TEXTURE_CUBE_MAP:
            ptr = "GL_TEXTURE_CUBE_MAP";
            break;

        case GL_TEXTURE_EXTERNAL_OES:
            ptr = "GL_TEXTURE_EXTERNAL_OES";
            break;

        default:
            ptr = "Invalid";
            break;
        }

        PrintParams2(output, outputSize, "glBindTexture",
                     (GLuint)ptr, "", STR_STR,
                     arg[1], "tex", INT_04D);
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

        sprintf(output, "glDrawBuffers(%d) ", n);

        for (GLint i=0; i<n; i++)
        {
            TranslateAttachment(pBufName[i], tmp1);
            OutputStrcat(output, outputSize, "%s ", tmp1);
        }
        OutputStrcat(output, outputSize, "\n", tmp1);
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

        TranslateBufferTarget(target, tmp1);
        TranslateAccessBit(access, tmp2);
        sprintf(output, "glMapBufferRange(%s, offset=%d, len=%d, access=%s) 0x%08X\n", tmp1, (GLint)offset, (GLint)len, tmp2, pointer);
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

        TranslateBufferTarget(target, tmp1);
        sprintf(output, "glFlushMappedBufferRange(%s, offset=%d, len=%d)\n", tmp1, (GLint)offset, (GLint)len);
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

        TranslateBufferTarget(target, tmp1);
        sprintf(output, "glUnmapBuffer(%s) = %d\n", tmp1, result);
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

        TranslateFramebufferRenderbuffer(target, tmp1);
        sprintf(output, "glInvalidateFramebuffer(%s, %d, ", tmp1, n);

        for (GLint i=0; i<n; i++)
        {
            TranslateAttachment(pBufName[i], tmp1);
            OutputStrcat(output, outputSize, "%s ", tmp1);
        }
        OutputStrcat(output, outputSize, ")\n", tmp1);
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

        sprintf(output, "glGetProgramBinary(prog=%08d, %d, 0x%04X) 0x%08X\n", program, bufSize, *pBinFormat, binPtr);
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
        const GLchar  *strBufMode = NULL;

        if (varyings)
        {
            ppVaryings = (GLuint*)ReadData(READ_GL_UINT, varyings, count);
        }

        sprintf(output, "glTransformFeedbackVaryings(prog=%08d, %d, ", program, count);
        for (GLuint i=0; i<count; i++)
        {
            pVaryings = (GLchar*)ReadData(READ_GL_UCHAR, ppVaryings[i], 0);
            OutputStrcat(output, outputSize, "%s ", pVaryings);
        }

        switch (bufferMode)
        {
        case GL_INTERLEAVED_ATTRIBS:
            strBufMode = "GL_INTERLEAVED_ATTRIBS";
            break;

        case GL_SEPARATE_ATTRIBS:
            strBufMode = "GL_SEPARATE_ATTRIBS";
            break;

        default:
            strBufMode = "invalid";
            break;
        }

        OutputStrcat(output, outputSize, ", %s)\n", strBufMode);
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

        TranslateBufferTarget(target, tmp1);
        sprintf(output, "glBindBufferBase(%s, index=%d, buf=%04d)\n", tmp1, index, buffer);
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

        TranslateBufferTarget(target, tmp1);
        sprintf(output, "glBindBufferRange(%s, index=%d, buf=%04d, offset=%d, size=%d)\n", tmp1, index, buffer, (GLint)offset, (GLint)size);
    }
}

GLvoid ParseVertexAttribPointer(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint arg[6];

    if (sscanf(input,
               "VertexAttribPointer 0x%08X 0x%08X 0x%08X 0x%08X 0x%08X 0x%08X}",
               &arg[0], &arg[1], &arg[2], &arg[3], &arg[4], &arg[5]) == 6)
    {
        TranslateDataType(arg[2], tmp);
        sprintf(output, "glVertexAttribPointer(attrib=%d, size=0x%08X, n=%d, stride=%d, p=0x%08X, %s)\n",
                arg[0], arg[1], arg[3], arg[4], arg[5], tmp);
    }
}

GLvoid ParseVertexAttribIPointer(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint arg[5];

    if (sscanf(input,
               "VertexAttribIPointer 0x%08X 0x%08X 0x%08X 0x%08X 0x%08X}",
               &arg[0], &arg[1], &arg[2], &arg[3], &arg[4]) == 5)
    {
        TranslateDataType(arg[2], tmp);
        sprintf(output, "glVertexAttribIPointer(attrib=%d, size=0x%08X, stride=%d, p=0x%08X, %s)\n",
                arg[0], arg[1], arg[3], arg[4], tmp);
    }
}

GLvoid ParseBeginTransformFeedback(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint arg[1];

    if (sscanf(input, "BeginTransformFeedback 0x%08X}", &arg[0]) == 1)
    {
        GLenum primitiveMode = arg[0];

        TranslateDrawMode(primitiveMode, tmp1);
        sprintf(output, "glBeginTransformFeedback(%s)\n", tmp1);
    }
}

GLvoid ParseEndTransformFeedback(GLchar *input, GLchar *output, GLint outputSize)
{
    sprintf(output, "glEndTransformFeedback()\n");
}

GLvoid ParsePauseTransformFeedback(GLchar *input, GLchar *output, GLint outputSize)
{
    sprintf(output, "glPauseTransformFeedback()\n");
}

GLvoid ParseResumeTransformFeedback(GLchar *input, GLchar *output, GLint outputSize)
{
    sprintf(output, "glResumeTransformFeedback()\n");
}

GLvoid ParseBindBuffer(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint arg[2];

    if (sscanf(input, "BindBuffer 0x%08X 0x%08X}", &arg[0], &arg[1]) == 2)
    {
        CURRENT_CONTEXT1().m_pAnalyzer->AnalyzeBindBuffer(output, outputSize, arg[0], arg[1]);
    }
}

GLvoid ParseBindVertexArray(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint arg[1];

    if (sscanf(input, "BindVertexArray 0x%08X}", &arg[0]) == 1)
    {
        GLuint name = arg[0];
        sprintf(output, "glBindVertexArray(vao=%04d)\n", name);
    }
}

GLvoid ParseBindTransformFeedback(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint arg[2];

    if (sscanf(input, "BindTransformFeedback 0x%08X 0x%08X}", &arg[0], &arg[1]) == 2)
    {
        GLenum  target  = arg[0];
        GLuint  name    = arg[1];

        TranslateGetName(target, tmp1);
        sprintf(output, "glBindTransformFeedback(%s, id=%04d)\n", tmp1, name);
    }
}

GLvoid ParseBufferData(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint arg[4];

    if (sscanf(input,
               "BufferData 0x%08X 0x%08X (0x%08X) 0x%08X",
               &arg[0], &arg[1], &arg[2], &arg[3]) == 4)
    {
        GLenum      target      = arg[0];
        GLuint      size        = arg[1];
        GLint       data        = arg[2];
        GLenum      usage       = arg[3];

        TranslateBufferTarget(target, tmp);
        TranslateUsageToString(usage, tmp1);
        sprintf(output, "glBufferData(%s, size=0x%08X, data=0x%08X, %s)\n", tmp, size, data, tmp1);
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

        TranslateBufferTarget(arg[0], tmp);
        sprintf(output, "glBufferSubData(%s, offset=0x%08X, size=0x%08X, data=0x%08X)\n", tmp, offset, size, data);
    }
}

GLvoid ParseAttachShader(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint arg[2];

    if (sscanf(input, "AttachShader 0x%08X 0x%08X}", &arg[0], &arg[1]) == 2)
    {
        GLuint      progId      = arg[0];
        GLuint      shaderId    = arg[1];
        CShaderObj  *pShader    = CURRENT_CONTEXT1().FindShader(shaderId);
        GLenum      shaderType  = pShader?pShader->m_type:GL_FRAGMENT_SHADER;

        if (shaderType == GL_FRAGMENT_SHADER)
        {
            PrintParams2(output, outputSize, "glAttachShader",
                         progId, "prog", INT_08D,
                         shaderId, "ps", INT_08D, GL_TRUE);
        }
        else
        {
            PrintParams2(output, outputSize, "glAttachShader",
                         progId, "prog", INT_08D,
                         shaderId, "vs", INT_08D, GL_TRUE);
        }
    }
}

GLvoid ParseActiveTexture(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint arg[1];

    if (sscanf(input, "ActiveTexture 0x%08X}", &arg[0]) == 1)
    {
        GLenum unit = arg[0];
        PrintParams1(output, outputSize, "glActiveTexture", unit - GL_TEXTURE0, "unit", INT_04D);
    }
}

GLvoid ParseBindAttribLocation(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint arg[3];

    if (sscanf(input, "BindAttribLocation 0x%08X 0x%08X (0x%08X)", &arg[0], &arg[1], &arg[2]) == 3)
    {
        GLuint  program = arg[0];
        GLint   loc     = arg[1];
        GLchar  *name   = (GLchar*)ReadData(READ_GL_UCHAR, arg[2], 0);
        sprintf(output, "glBindAttribLocation(prog=%08d, index=%d, %s)\n", program, loc, name);
        OutputToShaderFile(output);
    }
}

GLvoid ParseBindFramebuffer(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint arg[2];

    if (sscanf(input, "BindFramebuffer 0x%08X 0x%08X}", &arg[0], &arg[1]) == 2)
    {
        if (arg[0] == GL_FRAMEBUFFER)
        {
            OutputFBO(arg[1], output, outputSize);

            // Dump result to Frame file
            OutputToTarget(OUTPUT_TO_FILE, GL_FALSE, 0, NULL, output, g_pFrameFile);
        }
        else
        {
            sprintf(output, "glBindFramebuffer(unknown(0x%08X))\n", arg[0]);
        }
    }
}

GLvoid ParseBindRenderbuffer(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint arg[2];

    if (sscanf(input, "BindRenderbuffer 0x%08X 0x%08X}", &arg[0], &arg[1]) == 2)
    {
        PrintParams2(output, outputSize, "glBindRenderbuffer",
                     (GLuint)"GL_RENDERBUFFER", "", STR_STR,
                     arg[1], "RBO", INT_04D);
    }
}

GLvoid ParseRenderbufferStorage(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint arg[4];

    if (sscanf(input,
               "RenderbufferStorage 0x%08X 0x%08X 0x%08X 0x%08X}",
               &arg[0], &arg[1], &arg[2], &arg[3]) == 4)
    {
        const GLchar  *str = "GL_RENDERBUFFER";

        TranslateRBOFormat(arg[1], tmp1);
        PrintParams4(output, outputSize, "glRenderbufferStorage",
                     (GLuint)str, "", STR_STR,
                     (GLuint)tmp1, "", STR_STR,
                     (GLuint)arg[2], "width", INT_04D,
                     (GLuint)arg[3], "hegiht", INT_04D);
    }
}

GLvoid ParseFramebufferTexture2D(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint arg[5];

    if (sscanf(input,
               "FramebufferTexture2D 0x%08X 0x%08X 0x%08X 0x%08X 0x%08X}",
               &arg[0], &arg[1], &arg[2], &arg[3], &arg[4]) == 5)
    {
        TranslateAttachment(arg[1], tmp);
        TranslateTexTarget(arg[2], tmp1);

        if (arg[0] != GL_FRAMEBUFFER)
        {
            sprintf(output, "glFramebufferTexture2D(Invalid)\n");
        }
        else
        {
            sprintf(output, "glFramebufferTexture2D(%s, %s, tex=%d, level=%d)\n", tmp, tmp1, arg[3], arg[4]);
        }
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

        TranslateTexTarget(target, tmp);
        TranslateTexParamName(pname, tmp1);
        TranslateTexValue(value, tmp2);
        if (arg[1] != GL_TEXTURE_MAX_ANISOTROPY_EXT)
        {
            PrintParams3(output, outputSize, "glTexParameteri",
                         (GLuint)tmp, "", STR_STR,
                         (GLuint)tmp1, "", STR_STR,
                         (GLuint)tmp2, "", STR_STR);
        }
        else
        {
            PrintParams3(output, outputSize, "glTexParameteri",
                         (GLuint)tmp, "", STR_STR,
                         (GLuint)tmp1, "", STR_STR,
                         (GLuint)value, "isotropic", INT_08D);
        }
    }
}

GLvoid ParseTexParameterf(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint arg[3];

    if (sscanf(input, "TexParameterf 0x%08X 0x%08X 0x%08X}", &arg[0], &arg[1], &arg[2]) == 3)
    {
        GLenum  target  = arg[0];
        GLenum  pname   = arg[1];
        GLint   value   = (GLint)*(GLfloat*)&arg[2];

        TranslateTexTarget(target, tmp);
        TranslateTexParamName(pname, tmp1);
        TranslateTexValue(value, tmp2);
        PrintParams3(output, outputSize, "glTexParameterf",
                     (GLuint)tmp, "", STR_STR,
                     (GLuint)tmp1, "", STR_STR,
                     (GLuint)tmp2, "", STR_STR);
    }
}

GLvoid ParseTexParameteriv(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint arg[3];

    if (sscanf(input, "TexParameteriv 0x%08X 0x%08X (0x%08X)", &arg[0], &arg[1], &arg[2]) == 3)
    {
        GLenum  target  = arg[0];
        GLenum  pname   = arg[1];
        GLuint  value   = ReadUInt(arg[2]);

        TranslateTexTarget(target, tmp);
        TranslateTexParamName(pname, tmp1);
        TranslateTexValue(value, tmp2);
        PrintParams3(output, outputSize, "glTexParameteriv",
                     (GLuint)tmp, "", STR_STR,
                     (GLuint)tmp1, "", STR_STR,
                     (GLuint)tmp2, "", STR_STR);
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

        TranslateTexTarget(target, tmp);
        TranslateTexParamName(pname, tmp1);
        TranslateTexValue((GLuint)*params, tmp2);
        PrintParams3(output, outputSize, "glTexParameterfv",
                     (GLuint)tmp, "", STR_STR,
                     (GLuint)tmp1, "", STR_STR,
                     (GLuint)tmp2, "", STR_STR);
    }
}

GLvoid ParseGetActiveUniform(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint arg[7];

    if (sscanf(input,
               "GetActiveUniform 0x%08X 0x%08X 0x%08X (0x%08X) (0x%08X) (0x%08X) (0x%08X)",
               &arg[0], &arg[1], &arg[2], &arg[3], &arg[4], &arg[5], &arg[6]) == 7)
    {
        GLuint  progId      = arg[0];
        GLint   index       = arg[1];
        GLsizei bufsize     = arg[2];
        GLsizei length      = 0;
        GLint   size        = 0;
        GLenum  type        = 0;
        GLchar  *name       = 0;

        length  = (GLsizei)ReadUInt(arg[3]);
        size    = (GLint)ReadUInt(arg[4]);
        type    = (GLenum)ReadUInt(arg[5]);
        name    = (GLchar*)ReadData(READ_GL_UCHAR, arg[6], 0);
        TranslateShaderDataType(type, tmp1);

        if (!name) // strlen(name) > (GLuint)length)
        {
            sprintf(output, "glGetActiveUniform(prog=%08d, index=%d, bufsize=%d, length=%d, size=%d, %s, %s)\n",
                    progId, index, bufsize,
                    length,
                    size,
                    tmp1,
                    "invalid");
        }
        else
        {
            sprintf(output, "glGetActiveUniform(prog=%08d, index=%d, bufsize=%d, length=%d, size=%d, %s, %s)\n",
                    progId, index, bufsize,
                    length,
                    size,
                    tmp1,
                    name?name:"");
        }

        OutputToShaderFile(output);
        FreeData(name);
    }
}

GLvoid ParseFlush(GLchar *input, GLchar *output, GLint outputSize)
{
    if (strncmp(input, "Flush}", 6) == 0)
    {
        PrintParams0(output, outputSize, "glFlush");
    }
}

GLvoid ParseFinish(GLchar *input, GLchar *output, GLint outputSize)
{
    if (strncmp(input, "Finish}", 7) == 0)
    {
        PrintParams0(output, outputSize, "glFinish");
    }
}

GLvoid ParseFramebufferRenderbuffer(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint arg[4];

    if (sscanf(input,
               "FramebufferRenderbuffer 0x%08X 0x%08X 0x%08X 0x%08X}",
               &arg[0], &arg[1], &arg[2], &arg[3]) == 4)
    {
        TranslateAttachment(arg[1], tmp);
        TranslateFramebufferRenderbuffer(arg[2], tmp1);
        PrintParams4(output, outputSize, "glFramebufferRenderbuffer",
                     (GLuint)"GL_FRAMEBUFFER", "", STR_STR,
                     (GLuint)tmp, "", STR_STR,
                     (GLuint)tmp1, "", STR_STR,
                     (GLuint)arg[3], "RBO", INT_04D);
    }
}

GLvoid ParseFrontFace(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint arg[1];
    GLchar *ptr;

    if (sscanf(input,
               "FrontFace 0x%08X}",
               &arg[0]) == 1)
    {
        switch (arg[0])
        {
        case GL_CW:
            ptr = "GL_CW";
            break;

        case GL_CCW:
            ptr = "GL_CCW";
            break;

        default:
            ptr = "Invalid";
            break;
        }
        PrintParams1(output, outputSize, "glFrontFace", (GLuint)ptr, "", STR_STR);
    }
}

GLvoid ParseCullFace(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint arg[1];

    if (sscanf(input,
               "CullFace 0x%08X}",
               &arg[0]) == 1)
    {
        TranslateFace(arg[0], tmp5);
        PrintParams1(output, outputSize, "glCullFace", (GLuint)tmp5, "F", STR_STR);
    }
}

GLvoid ParseBlendFunc(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint arg[2];

    if (sscanf(input,
               "BlendFunc 0x%08X 0x%08X}",
               &arg[0], &arg[1]) == 2)
    {
        TranslateBlendFunc(arg[0], tmp);
        TranslateBlendFunc(arg[1], tmp1);
        PrintParams2(output, outputSize, "glBlendFunc",
                     (GLuint)tmp, "src", STR_STR,
                     (GLuint)tmp1, "dst", STR_STR);
    }
}

GLvoid ParseBlendFuncSeparate(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint arg[4];

    if (sscanf(input,
           "BlendFuncSeparate 0x%08X 0x%08X 0x%08X 0x%08X}",
           &arg[0], &arg[1], &arg[2], &arg[3]) == 4)
    {
        TranslateBlendFunc(arg[0], tmp);
        TranslateBlendFunc(arg[1], tmp1);
        TranslateBlendFunc(arg[2], tmp2);
        TranslateBlendFunc(arg[3], tmp3);
        PrintParams4(output, outputSize, "glBlendFuncSeparate",
                     (GLuint)tmp, "srcRGB", STR_STR,
                     (GLuint)tmp1, "dstRGB", STR_STR,
                     (GLuint)tmp2, "srcAlpha", STR_STR,
                     (GLuint)tmp3, "dstAlph", STR_STR);
    }
}

GLvoid ParseBlendColor(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint arg[4];

    if (sscanf(input,
               "BlendColor 0x%08X 0x%08X 0x%08X 0x%08X}",
               &arg[0], &arg[1], &arg[2], &arg[3]) == 4)
    {
        PrintParams4(output, outputSize, "glBlendColor",
                     (GLuint)arg[0], "R", FLOAT_1_1,
                     (GLuint)arg[1], "G", FLOAT_1_1,
                     (GLuint)arg[2], "B", FLOAT_1_1,
                     (GLuint)arg[3], "A", FLOAT_1_1);
    }
}

GLvoid ParseBlendEquation(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint arg[1];

    if (sscanf(input,
               "BlendEquation 0x%08X}",
               &arg[0]) == 1)
    {
        TranslateBlendMode(arg[0], tmp);
        PrintParams1(output, outputSize, "glBlendEquation",
                     (GLuint)tmp, "", STR_STR);
    }
}

GLvoid ParseBlendEquationSeparate(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint arg[2];

    if (sscanf(input,
               "BlendEquationSeparate 0x%08X 0x%08X}",
               &arg[0], &arg[1]) == 2)
    {
        TranslateBlendMode(arg[0], tmp);
        TranslateBlendMode(arg[1], tmp1);
        PrintParams2(output, outputSize, "glBlendEquationSeparate",
                     (GLuint)tmp, "rgb", STR_STR,
                     (GLuint)tmp1, "a", STR_STR);
    }
}

GLvoid ParseGetError(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint arg[1];

    if (sscanf(input, "GetError := 0x%08X}", &arg[0]) == 1)
    {
        TranslateError(arg[0], tmp);
        if (arg[0] != GL_NO_ERROR)
        {
            sprintf(output, "glGetError()=%s\n", tmp);
        }
    }
}

GLvoid ParseGetUniformLocation(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint arg[3];

    if (sscanf(input,
               "GetUniformLocation 0x%08X (0x%08X) := 0x%08X",
               &arg[0], &arg[1], &arg[2]) == 3)
    {
        GLchar      *name           = (GLchar*)ReadData(READ_GL_UCHAR, arg[1], 0);
        GLuint      location        = arg[2];
        GLuint      programArray    = arg[0];

        CProgramObj *prog       = CURRENT_CONTEXT1().FindProgram(programArray);
        CUniformObj *uniform    = prog->GetUniformByName(name);

        if (uniform == NULL)
        {
            sprintf(output, "glGetUniformLocation(prog=%08d, %s) invalid(%d)\n", arg[0], name, arg[2]);
            OutputToShaderFile(output);
            return;
        }

        if (name == NULL)
        {
            sprintf(output, "glGetUniformLocation(prog=%08d, NULL)\n", programArray);
        }
        else
        {
            sprintf(output, "glGetUniformLocation(prog=%08d, %s)=%d\n", programArray, name, location);
        }
        OutputToShaderFile(output);
        FreeData(name);
    }
}

GLvoid ParseLinkProgram(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint arg[1];

    if (sscanf(input,
               "LinkProgram 0x%08X}",
               &arg[0]) == 1)
    {
        PrintParams1(output, outputSize, "glLinkProgram", (GLuint)arg[0], "prog", INT_08D, GL_TRUE);
    }
}

GLvoid ParseClearColor(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint arg[4];

    if (sscanf(input,
               "ClearColor 0x%08X 0x%08X 0x%08X 0x%08X}",
               &arg[0], &arg[1], &arg[2], &arg[3]) == 4)
    {
        PrintParams4(output, outputSize, "glClearColor",
                     (GLuint)arg[0], "R", FLOAT_1_5,
                     (GLuint)arg[1], "G", FLOAT_1_5,
                     (GLuint)arg[2], "B", FLOAT_1_5,
                     (GLuint)arg[3], "A", FLOAT_1_5);
    }
}

GLvoid ParseClearDepthf(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint arg[1];

    if (sscanf(input,
               "ClearDepthf 0x%08X}",
               &arg[0]) == 1)
    {
        PrintParams1(output, outputSize, "glClearDepthf",
                     (GLuint)arg[0], "D", FLOAT_1_5);
    }
}

GLvoid ParseClearStencil(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint arg[1];

    if (sscanf(input,
               "ClearStencil 0x%08X}",
               &arg[0]) == 1)
    {
        PrintParams1(output, outputSize, "glClearStencil",
                     (GLuint)arg[0], "S", INT_0X08X);
    }
}

GLvoid AnalyzeClear(GLchar *output, GLint outputSize, GLint mask)
{
    TranslateClearMask(mask, tmp);
    PrintParams1(output, outputSize, "glClear", (GLuint)tmp, "B", STR_STR);
    OutputClearToFrameFile(mask);
}

GLvoid ParseClear(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint arg[1];

    if (sscanf(input,
               "Clear 0x%08X}",
               &arg[0]) == 1)
    {
        AnalyzeClear(output, outputSize, arg[0]);
    }
}

GLvoid ParseColorMask(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint arg[4];

    if (sscanf(input,
               "ColorMask 0x%08X 0x%08X 0x%08X 0x%08X}",
               &arg[0], &arg[1], &arg[2], &arg[3]) == 4)
    {
        PrintParams4(output, outputSize, "glColorMask",
                     (GLuint)arg[0], "R", INT_04D,
                     (GLuint)arg[1], "G", INT_04D,
                     (GLuint)arg[2], "B", INT_04D,
                     (GLuint)arg[3], "A", INT_04D);
    }
}

GLvoid ParseDepthMask(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint arg[1];

    if (sscanf(input,
               "DepthMask 0x%08X}",
               &arg[0]) == 1)
    {
        PrintParams1(output, outputSize, "glDepthMask", (GLuint)arg[0], "mask", INT_0X08X);
    }
}

GLvoid ParseStencilMask(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint arg[1];

    if (sscanf(input,
               "StencilMask 0x%08X}",
               &arg[0]) == 1)
    {
        PrintParams1(output, outputSize, "glStencilMask", (GLuint)arg[0], "mask", INT_0X08X);
    }
}

GLvoid ParseCreateShader(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint arg[2];

    if (sscanf(input,
               "CreateShader 0x%08X := 0x%08X",
               &arg[0], &arg[1]) == 2)
    {
        TranslateShaderType(arg[0], tmp);
        sprintf(output, "glCreateShader(%s)=%08d\n", tmp, arg[1]);
        OutputToShaderFile(output);
    }
}

GLvoid ParseCreateProgram(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint arg[1];

    if (sscanf(input,
               "CreateProgram := 0x%08X}",
               &arg[0]) == 1)
    {
        sprintf(output, "glCreateProgram()=%08d\n", arg[0]);
        OutputToShaderFile(output);
    }
}

GLvoid ParseCompileShader(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint arg[1];

    if (sscanf(input,
               "CompileShader 0x%08X}", &arg[0]) == 1)
    {
        PrintParams1(output, outputSize, "glCompileShader", (GLuint)arg[0], "sh", INT_08D, GL_TRUE);
    }
}

GLvoid ParseCopyTexImage2D(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint arg[8];

    if (sscanf(input,
               "CopyTexImage2D 0x%08X 0x%08X 0x%08X 0x%08X 0x%08X 0x%08X 0x%08X 0x%08X}",
               &arg[0], &arg[1], &arg[2], &arg[3], &arg[4], &arg[5], &arg[6], &arg[7]) == 8)
    {
        TranslateTexTarget(arg[0], tmp);
        TranslateGL2GCFormat(arg[2], GL_UNSIGNED_BYTE, tmp3, tmp1, tmp2);
        sprintf(output, "glCopyTexImage2D(%s, level=%d, %s, x=%d, y=%d, w=%d, h=%d)\n",
            tmp, arg[1], tmp1, arg[3], arg[4], arg[5], arg[6]);
    }
}

GLvoid ParseCopyTexSubImage2D(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint arg[8];

    if (sscanf(input,
               "CopyTexSubImage2D 0x%08X 0x%08X 0x%08X 0x%08X 0x%08X 0x%08X 0x%08X 0x%08X}",
               &arg[0], &arg[1], &arg[2], &arg[3], &arg[4], &arg[5], &arg[6], &arg[7]) == 8)
    {
        TranslateTexTarget(arg[0], tmp);
        sprintf(output, "glCopyTexSubImage2D(%s, level=%d, xoff=%d, yoff=%d, x=%d, y=%d, w=%d, h=%d)\n",
            tmp, arg[1], arg[2], arg[3], arg[4], arg[5], arg[6], arg[7]);
    }
}

GLvoid ParseCheckFramebufferStatus(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint arg[2];
    GLchar *ptr;

    if (sscanf(input,
               "CheckFramebufferStatus 0x%08X := 0x%08X",
               &arg[0], &arg[1]) == 2)
    {

        if (arg[0] == GL_FRAMEBUFFER)
        {
            switch (arg[1])
            {
            case GL_FRAMEBUFFER_COMPLETE:
                ptr = "GL_FRAMEBUFFER_COMPLETE";
                break;

            case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
                ptr = "GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT";
                break;

            case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
                ptr = "GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT";
                break;

            case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS:
                ptr = "GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS";
                break;

            case GL_FRAMEBUFFER_UNSUPPORTED:
                ptr = "GL_FRAMEBUFFER_UNSUPPORTED";
                break;

            default:
                ptr = "Invalid";
                break;
            }

            sprintf(output, "glCheckFramebufferStatus()=%s\n", ptr);
        }
        else
        {
            sprintf(output, "glCheckFramebufferStatus(Invalid)\n");
        }
    }
}

GLvoid ParseDisable(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint arg[1];

    if (sscanf(input,
               "Disable 0x%08X}",
               &arg[0]) == 1)
    {
        TranslateCapability(arg[0], tmp, GL_FALSE);
        PrintParams1(output, outputSize, "glDisable", (GLuint)tmp, "", STR_STR);
    }
}

GLvoid ParseDisableVertexAttribArray(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint arg[1];

    if (sscanf(input,
               "DisableVertexAttribArray 0x%08X}",
               &arg[0]) == 1)
    {
        PrintParams1(output, outputSize, "glDisableVertexAttribArray", (GLuint)arg[0], "attrib", INT_04D);
    }
}

GLvoid ParseEnable(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint arg[1];

    if (sscanf(input,
               "Enable 0x%08X}",
               &arg[0]) == 1)
    {
        TranslateCapability(arg[0], tmp, GL_TRUE);
        PrintParams1(output, outputSize, "glEnable", (GLuint)tmp, "", STR_STR);
    }
}

GLvoid ParseEnableVertexAttribArray(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint arg[1];

    if (sscanf(input,
               "EnableVertexAttribArray 0x%08X}",
               &arg[0]) == 1)
    {
        PrintParams1(output, outputSize, "glEnableVertexAttribArray", (GLuint)arg[0], "attrib", INT_04D);
    }
}

GLvoid ParseDrawArrays(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint arg[3];

    if (sscanf(input,
               "DrawArrays 0x%08X 0x%08X 0x%08X",
               &arg[0], &arg[1], &arg[2]) == 3)
    {
        GLenum      mode        = arg[0];
        GLint       first       = arg[1];
        GLsizei     count       = arg[2];

        OutputDrawArrays_es20(mode, first, count, output, outputSize);
    }
}

GLvoid ParseDrawElements(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint arg[4];

    if (sscanf(input,
               "DrawElements 0x%08X 0x%08X 0x%08X (0x%08X)",
               &arg[0], &arg[1], &arg[2], &arg[3]) == 4)
    {
        GLenum      mode        = arg[0];
        GLint       indexCount  = arg[1];
        GLsizei     indexType   = arg[2];
        GLuint      indexAddr   = arg[3];

        OutputDrawElements_es20(mode, indexCount, indexType, indexAddr, output, outputSize);
    }
}

GLvoid ParseDetachShader(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint arg[2];

    if (sscanf(input,
               "DetachShader 0x%08X 0x%08X}",
               &arg[0], &arg[1]) == 2)
    {
        PrintParams2(output, outputSize, "glDetachShader",
                    (GLuint)arg[0], "prog", INT_04D,
                    (GLuint)arg[1], "sh", INT_04D, GL_TRUE);
    }
}

GLvoid ParseDepthFunc(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint arg[1];

    if (sscanf(input,
               "DepthFunc 0x%08X}",
               &arg[0]) == 1)
    {
        TranslateFunc(arg[0], tmp);
        PrintParams1(output, outputSize, "glDepthFunc", (GLuint)tmp, "attrib", STR_STR);
    }
}

GLvoid ParseDepthRangef(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint arg[2];

    if (sscanf(input,
               "DepthRangef 0x%08X 0x%08X}",
               &arg[0], &arg[1]) == 2)
    {
        PrintParams2(output, outputSize, "glDepthRangef",
                    (GLuint)arg[0], "near", FLOAT_1_5,
                    (GLuint)arg[1], "far", FLOAT_1_5);
    }
}

GLvoid ParseDeleteShader(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint arg[1];

    if (sscanf(input,
               "DeleteShader 0x%08X}",
               &arg[0]) == 1)
    {
        PrintParams1(output, outputSize, "glDeleteShader", (GLuint)arg[0], "sh", INT_08D, GL_TRUE);
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
        CURRENT_CONTEXT1().m_pAnalyzer->AnalyzeDeleteTextures(output, outputSize, arg[0], buffers);
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
        CURRENT_CONTEXT1().m_pAnalyzer->AnalyzeDeleteBuffers(output, outputSize, arg[0], buffers);
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
        CURRENT_CONTEXT1().m_pAnalyzer->AnalyzeDeleteVertexArrays(output, outputSize, arg[0], buffers);
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
        CURRENT_CONTEXT1().m_pAnalyzer->AnalyzeDeleteTransformFeedbacks(output, outputSize, arg[0], buffers);
    }
}

GLvoid ParseDeleteProgram(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint arg[1];

    if (sscanf(input,
               "DeleteProgram 0x%08X}",
               &arg[0]) == 1)
    {
        PrintParams1(output, outputSize, "glDeleteProgram", (GLuint)arg[0], "prog", INT_08D, GL_TRUE);
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
        CURRENT_CONTEXT1().m_pAnalyzer->AnalyzeDeleteRenderbuffers(output, outputSize, arg[0], buffers);
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
        CURRENT_CONTEXT1().m_pAnalyzer->AnalyzeDeleteFramebuffers(output, outputSize, arg[0], buffers);
    }
}

GLvoid ParseDiscardFramebufferEXT(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint arg[3];

    if (sscanf(input,
               "DiscardFramebufferEXT 0x%08X 0x%08X (0x%08X)",
               &arg[0], &arg[1], &arg[2]) == 3)
    {
        sprintf(output, "glDiscardFramebufferEXT(FBO=%d)\n", arg[1]);
    }
}

GLvoid ParseEGLImageTargetRenderbufferStorageOES(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint arg[2];

    if (sscanf(input,
               "EGLImageTargetRenderbufferStorageOES 0x%08X 0x%08X}",
               &arg[0], &arg[1]) == 2)
    {
        PrintParams2(output, outputSize, "glEGLImageTargetRenderbufferStorageOES",
                     (GLuint)arg[0], "target", INT_0X08X,
                     (GLuint)arg[1], "image", INT_0X08X);
    }
}

GLvoid ParseEGLImageTargetTexture2DOES(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint arg[2];

    if (sscanf(input,
               "EGLImageTargetTexture2DOES 0x%08X 0x%08X}",
               &arg[0], &arg[1]) == 2)
    {
        GLenum  target  = arg[0];

        TranslateTexTarget(target, tmp1);
        PrintParams2(output, outputSize, "EGLImageTargetTexture2DOES",
                     (GLuint)tmp1, "", STR_STR,
                     (GLuint)arg[1], "image", INT_0X08X);
    }
}

GLvoid ParseGenTextures(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint arg[2];

    if (sscanf(input,
               "GenTextures 0x%08X (0x%08X)",
               &arg[0], &arg[1]) == 2)
    {
        GLuint *buffers = (GLuint*)ReadData(READ_GL_UINT, arg[1], arg[0]);
        CURRENT_CONTEXT1().m_pAnalyzer->AnalyzeGenTextures(output, outputSize, arg[0], buffers);
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
        CURRENT_CONTEXT1().m_pAnalyzer->AnalyzeGenFramebuffers(output, outputSize, arg[0], buffers);
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
        CURRENT_CONTEXT1().m_pAnalyzer->AnalyzeGenRenderbuffers(output, outputSize, arg[0], buffers);
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
        CURRENT_CONTEXT1().m_pAnalyzer->AnalyzeGenBuffers(output, outputSize, arg[0], buffers);
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
        CURRENT_CONTEXT1().m_pAnalyzer->AnalyzeGenVertexArrays(output, outputSize, arg[0], buffers);
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
        CURRENT_CONTEXT1().m_pAnalyzer->AnalyzeGenTransformFeedbacks(output, outputSize, arg[0], buffers);
    }
}

GLvoid ParseGenerateMipmap(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint arg[1];

    if (sscanf(input,
               "GenerateMipmap 0x%08X}",
               &arg[0]) == 1)
    {
        TranslateTexTarget(arg[0], tmp);
        PrintParams1(output, outputSize, "glGenerateMipmap", (GLuint)tmp, "", STR_STR);
    }
}

GLvoid ParseGetShaderiv(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint arg[3];

    if (sscanf(input,
               "GetShaderiv 0x%08X 0x%08X (0x%08X)",
               &arg[0], &arg[1], &arg[2]) == 3)
    {
        GLuint v = ReadUInt(arg[2]);
        TranslateShaderInfo(arg[1], tmp);
        sprintf(output, "glGetShaderiv(shader=%08d, %s, value=%d)\n", arg[0], tmp, v);
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
        FreeData(v);
        sprintf(output, "glGetShaderInfoLog(shader=%08d, %s)\n", arg[0], v?v:"");
    }
}

GLvoid ParseGetShaderSource(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint arg[4];

    if (sscanf(input,
               "GetShaderSource 0x%08X 0x%08X (0x%08X) (0x%08X)",
               &arg[0], &arg[1], &arg[2], &arg[3]) == 4)
    {
        sprintf(output, "glGetShaderSource(shader=%08d)\n", arg[0]);
    }
}

GLvoid ParseGetProgramiv(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint arg[3];

    if (sscanf(input,
               "GetProgramiv 0x%08X 0x%08X (0x%08X)",
               &arg[0], &arg[1], &arg[2]) == 3)
    {
        GLuint v = ReadUInt(arg[2]);
        TranslateProgramInfo(arg[1], tmp);
        sprintf(output, "glGetProgramiv(prog=%08d, %s, value=%d)\n", arg[0], tmp, v);
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

        TranslateShaderDataType(type, tmp1);

        sprintf(output, "glGetActiveAttrib(prog=%08d, index=%d, bufsize=%d, length=%d, size=%d, %s, %s)\n",
                program, index, bufsize,
                length,
                size,
                tmp1,
                name?name:"");
        OutputToShaderFile(output);
        FreeData(name);
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

        if (name)
        {
            sprintf(output, "glGetAttribLocation(prog=%08d, %s) = %d\n", arg[0], name, loc);
        }
        else
        {
            sprintf(output, "glGetAttribLocation(prog=%08d, null) = %d, data error\n", arg[0], loc);
        }

        OutputToShaderFile(output);
    }
}

GLvoid ParseGetAttachedShaders(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint arg[4];

    if (sscanf(input,
               "GetAttachedShaders 0x%08X 0x%08X (0x%08X) (0x%08X)",
               &arg[0], &arg[1], &arg[2], &arg[3]) == 4)
    {
        GLuint prog     = arg[0];
        GLuint maxCount = arg[1];
        GLuint count    = ReadUInt(arg[2]);
        GLuint *name    = (GLuint*)ReadData(READ_GL_UINT, arg[3], count);

        sprintf(output, "glGetAttachedShaders(prog=%04d, max=%d, count=%d) =", prog, maxCount, count);
        for (GLuint i=0; i<count; i++)
        {
            OutputStrcat(output, outputSize, " %04d,", name[i]);
        }
        FreeData(name);
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
        GLchar  buf[128];

        memset(buf, 0, 128);
        if (v)
        {
            sprintf(buf, "%s", v);
            FreeData(v);
        }

        sprintf(output, "glGetProgramInfoLog(program = %04d, %s)\n", arg[0], buf);
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

        TranslateAttribStateName(arg[1], tmp5);
        sprintf(output, "glGetVertexAttribfv(index=%d, %s, %f)\n", index, tmp5, f);
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
        TranslateGetName(arg[1], tmp2);
        sprintf(output, "glGetVertexAttribPointerv(index=%d, %s, 0x%08X)\n", arg[0], tmp2, pointer);
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
        GLuint  value   = ReadUInt(arg[2]);

        TranslateAttribStateName(arg[1], tmp5);
        sprintf(output, "glGetVertexAttribiv(index=%d, %s, %d)\n", index, tmp5, value);
    }
}

GLvoid ParseGetBufferParameteriv(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint arg[3];

    if (sscanf(input,
               "GetBufferParameteriv 0x%08X 0x%08X (0x%08X)",
               &arg[0], &arg[1], &arg[2]) == 3)
    {
        sprintf(output, "glGetBufferParameteriv() ********todo\n");
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
        GLchar *v = (GLchar*)ReadData(READ_GL_UCHAR, arg[1], 0);
        TranslateGetName(arg[0], tmp1);
        sprintf(output, "glGetString(%s) : %s\n", tmp1, v);
        FreeData(v);
    }
}

GLvoid ParseGetIntegerv(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint arg[2];

    if (sscanf(input,
               "GetIntegerv 0x%08X (0x%08X)",
               &arg[0], &arg[1]) == 2)
    {
        GLuint v = ReadUInt(arg[1]);
        TranslateGetName(arg[0], tmp1);
        sprintf(output, "glGetIntegerv(%s, value=0x%08X)\n", tmp1, v);
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
        TranslateGetName(arg[0], tmp1);
        sprintf(output, "glGetBooleanv(%s, value=%d)\n", tmp1, v);
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

        TranslateGetName(arg[0], tmp2);
        sprintf(output, "glGetFloatv(%s, %f)\n", tmp2, f);
    }
}

GLvoid ParseGetShaderPrecisionFormat(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint arg[4];

    if (sscanf(input,
               "GetShaderPrecisionFormat 0x%08X 0x%08X (0x%08X) (0x%08X)",
               &arg[0], &arg[1], &arg[2], &arg[3]) == 4)
    {
        sprintf(output, "glGetShaderPrecisionFormat() ********todo\n");
    }
}

GLvoid ParseGetRenderbufferParameteriv(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint arg[3];

    if (sscanf(input,
               "GetRenderbufferParameteriv 0x%08X 0x%08X (0x%08X)",
               &arg[0], &arg[1], &arg[2]) == 3)
    {
        sprintf(output, "glGetRenderbufferParameteriv() ********todo\n");
    }
}

GLvoid ParseGetTexParameteriv(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint arg[3];

    if (sscanf(input,
               "GetTexParameteriv 0x%08X 0x%08X (0x%08X)",
               &arg[0], &arg[1], &arg[2]) == 3)
    {
        GLuint v = ReadUInt(arg[2]);

        TranslateTexTarget(arg[0], tmp1);
        TranslateTexParameter(arg[1], v, tmp2);
        sprintf(output, "glGetTexParameteriv(%s, %s)\n", tmp1, tmp2);
    }
}

GLvoid ParseGetTexParameterfv(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint arg[3];

    if (sscanf(input,
               "GetTexParameterfv 0x%08X 0x%08X (0x%08X)",
               &arg[0], &arg[1], &arg[2]) == 3)
    {
        sprintf(output, "glGetTexParameterfv() ********todo\n");
    }
}

GLvoid ParseGetUniformfv(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint arg[3];

    if (sscanf(input,
               "GetUniformfv 0x%08X 0x%08X (0x%08x)",
               &arg[0], &arg[1], &arg[2]) == 3)
    {
        sprintf(output, "glGetUniformfv() ********todo\n");
    }
}

GLvoid ParseGetUniformiv(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint arg[3];

    if (sscanf(input,
               "GetUniformiv 0x%08X 0x%08X (0x%08x)",
               &arg[0], &arg[1], &arg[2]) == 3)
    {
        sprintf(output, "glGetUniformiv() ********todo\n");
    }
}

GLvoid ParseGetFramebufferAttachmentParameteriv(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint arg[4];

    if (sscanf(input,
               "GetFramebufferAttachmentParameteriv 0x%08X 0x%08X 0x%08X (0x%08X)",
               &arg[0], &arg[1], &arg[2], &arg[3]) == 4)
    {
        sprintf(output, "glGetFramebufferAttachmentParameteriv() ********todo\n");
    }
}

GLvoid ParseHint(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint arg[2];

    if (sscanf(input,
               "Hint 0x%08X 0x%08X}",
               &arg[0], &arg[1]) == 2)
    {
        TranslateGetName(arg[0], tmp1);
        TranslateGetName(arg[1], tmp2);
        sprintf(output, "glHint(%s, %s)\n", tmp1, tmp2);
    }
}

GLvoid ParseIsBuffer(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint arg[2];

    if (sscanf(input,
           "IsBuffer 0x%08X := 0x%08X",
           &arg[0], &arg[1]) == 2)
    {
        PrintParams1Return(output, outputSize, "glIsBuffer", (GLuint)arg[0], "vbo", INT_04D, (GLuint)arg[1], INT_04D);
    }
}

GLvoid ParseIsShader(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint arg[2];

    if (sscanf(input,
               "IsShader 0x%08X := 0x%08X",
               &arg[0], &arg[1]) == 2)
    {
        PrintParams1Return(output, outputSize, "glIsShader", (GLuint)arg[0], "sh", INT_04D, (GLuint)arg[1], INT_04D);
    }
}

GLvoid ParseIsProgram(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint arg[2];

    if (sscanf(input,
               "IsProgram 0x%08X := 0x%08X",
               &arg[0], &arg[1]) == 2)
    {
        PrintParams1Return(output, outputSize, "glIsRenderbuffer", (GLuint)arg[0], "prog", INT_04D, (GLuint)arg[1], INT_04D);
    }
}

GLvoid ParseIsRenderbuffer(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint arg[2];

    if (sscanf(input,
               "IsRenderbuffer 0x%08X := 0x%08X",
               &arg[0], &arg[1]) == 2)
    {
        PrintParams1Return(output, outputSize, "glIsRenderbuffer", (GLuint)arg[0], "RBO", INT_04D, (GLuint)arg[1], INT_04D);
    }
}

GLvoid ParseIsEnabled(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint arg[2];

    if (sscanf(input,
               "IsEnabled 0x%08X := 0x%08X",
               &arg[0], &arg[1]) == 2)
    {
        PrintParams1Return(output, outputSize, "glIsEnabled", (GLuint)arg[0], "bit", INT_04D, (GLuint)arg[1], INT_04D);
    }
}

GLvoid ParseIsTexture(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint arg[2];

    if (sscanf(input,
               "IsTexture 0x%08X := 0x%08X",
               &arg[0], &arg[1]) == 2)
    {
        PrintParams1Return(output, outputSize, "glIsTexture", (GLuint)arg[0], "tex", INT_04D, (GLuint)arg[1], INT_04D);
    }
}

GLvoid ParseIsFramebuffer(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint arg[2];

    if (sscanf(input,
               "IsFramebuffer 0x%08X := 0x%08X",
               &arg[0], &arg[1]) == 2)
    {
        PrintParams1Return(output, outputSize, "glIsFramebuffer", (GLuint)arg[0], "FBO", INT_04D, (GLuint)arg[1], INT_04D);
    }
}

GLvoid ParseLineWidth(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint arg[1];

    if (sscanf(input, "LineWidth 0x%08X}", &arg[0]) == 1)
    {
        PrintParams1(output, outputSize, "glLineWidth", (GLuint)arg[0], "width", FLOAT_5_1);
    }
}

GLvoid ParsePixelStorei(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint arg[2];

    if (sscanf(input,
               "PixelStorei 0x%08X 0x%08X}",
               &arg[0], &arg[1]) == 2)
    {
        TranslatePixelStorei(arg[0], arg[1], tmp1);
        PrintParams2(output, outputSize, "glPixelStorei",
                     (GLuint)tmp1, "", STR_STR,
                     (GLuint)arg[1], "param", INT_04D);
    }
}

GLvoid ParsePolygonOffset(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint arg[2];

    if (sscanf(input,
               "PolygonOffset 0x%08X 0x%08X}",
               &arg[0], &arg[1]) == 2)
    {
        PrintParams2(output, outputSize, "glPolygonOffset",
                     (GLuint)arg[0], "factor", FLOAT_1_5,
                     (GLuint)arg[1], "units", FLOAT_1_5);
    }
}

GLvoid ParseProgramBinaryOES(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint arg[4];

    if (sscanf(input,
               "ProgramBinaryOES 0x%08X 0x%08X (0x%08X) 0x%08X",
               &arg[0], &arg[1], &arg[2], &arg[3]) == 4)
    {
        if (arg[1] != GL_PROGRAM_BINARY_VIV)
        {
            sprintf(output, "glProgramBinaryOES(prog=%04d, wrong binary format(0x%08X)\n", arg[0], arg[1]);
            return;
        }

        sprintf(output, "glProgramBinaryOES(prog=%08d, p=0x%08X, size=%d)\n", arg[0], arg[2], arg[3]);
    }
}

GLvoid ParseProgramBinary(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint arg[4];

    if (sscanf(input,
               "ProgramBinary 0x%08X 0x%08X (0x%08X) 0x%08X",
               &arg[0], &arg[1], &arg[2], &arg[3]) == 4)
    {
        GLchar *str = NULL;

        switch (arg[1])
        {
        case GL_Z400_BINARY_AMD:
            str = "amd_binary";
            break;

        case GL_PROGRAM_BINARY_VIV:
            str = "viv_binary";
            break;

        default:
            sprintf(output, "glProgramBinary(prog=%04d, wrong binary format(0x%08X)\n", arg[0], arg[1]);
            return;
        }

        sprintf(output, "glProgramBinary(prog=%08d, %s, size=%d)\n", arg[0], str, arg[3]);
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

        TranslateTexFormat(format, tmp1);
        TranslateDataType(type, tmp2);
        sprintf(output, "glReadPixels(%d, %d, %d, %d, %s, %s)\n", x, y, width, height, tmp1, tmp2);
    }
}

GLvoid ParseScissor(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint arg[4];

    if (sscanf(input,
               "Scissor 0x%08X 0x%08X 0x%08X 0x%08X}",
               &arg[0], &arg[1], &arg[2], &arg[3]) == 4)
    {
        PrintParams4(output, outputSize, "glScissor",
                     (GLuint)arg[0], "", INT_04D_NO,
                     (GLuint)arg[1], "", INT_04D_NO,
                     (GLuint)arg[2], "", INT_04D_NO,
                     (GLuint)arg[3], "", INT_04D_NO);
    }
}

GLvoid ParseStencilFunc(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint arg[3];

    if (sscanf(input,
               "StencilFunc 0x%08X 0x%08X 0x%08X}",
               &arg[0], &arg[1], &arg[2]) == 3)
    {
        TranslateFunc(arg[0], tmp);
        PrintParams3(output, outputSize, "glStencilFunc",
                     (GLuint)tmp, "", STR_STR,
                     (GLuint)arg[1], "ref", INT_0X08X,
                     (GLuint)arg[2], "mask", INT_0X08X);
    }
}

GLvoid ParseStencilFuncSeparate(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint arg[4];

    if (sscanf(input,
               "StencilFuncSeparate 0x%08X 0x%08X 0x%08X 0x%08X}",
               &arg[0], &arg[1], &arg[2], &arg[3]) == 4)
    {
        TranslateFace(arg[0], tmp);
        TranslateFunc(arg[1], tmp1);
        PrintParams4(output, outputSize, "glStencilFuncSeparate",
                     (GLuint)tmp, "", STR_STR,
                     (GLuint)tmp1, "", STR_STR,
                     (GLuint)arg[2], "ref", INT_0X08X,
                     (GLuint)arg[3], "mask", INT_0X08X);
    }
}

GLvoid ParseStencilMaskSeparate(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint arg[2];

    if (sscanf(input,
               "StencilMaskSeparate 0x%08X 0x%08X}",
               &arg[0], &arg[1]) == 2)
    {
        TranslateFace(arg[0], tmp);
        PrintParams2(output, outputSize, "glStencilMaskSeparate",
                     (GLuint)tmp, "", STR_STR,
                     (GLuint)arg[1], "mask", INT_0X08X);
    }
}

GLvoid ParseStencilOp(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint arg[3];

    if (sscanf(input,
               "StencilOp 0x%08X 0x%08X 0x%08X}",
               &arg[0], &arg[1], &arg[2]) == 3)
    {
        TranslateOperation(arg[0], tmp);
        TranslateOperation(arg[1], tmp1);
        TranslateOperation(arg[2], tmp2);
        PrintParams3(output, outputSize, "glStencilOp",
                     (GLuint)tmp, "fail", STR_STR_NAME,
                     (GLuint)tmp1, "zfail", STR_STR_NAME,
                     (GLuint)tmp2, "zpass", STR_STR_NAME);
    }
}

GLvoid ParseStencilOpSeparate(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint arg[4];

    if (sscanf(input,
               "StencilOpSeparate 0x%08X 0x%08X 0x%08X 0x%08X}",
               &arg[0], &arg[1], &arg[2], &arg[3]) == 4)
    {
        TranslateFace(arg[0], tmp);
        TranslateOperation(arg[1], tmp1);
        TranslateOperation(arg[2], tmp2);
        TranslateOperation(arg[3], tmp3);
        PrintParams4(output, outputSize, "glStencilOpSeparate",
                     (GLuint)tmp, "", STR_STR,
                     (GLuint)tmp1, "fail", STR_STR_NAME,
                     (GLuint)tmp2, "zfail", STR_STR_NAME,
                     (GLuint)tmp3, "zpass", STR_STR_NAME);
    }
}

GLvoid ParseShaderSource(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint arg[4];

    if (sscanf(input,
               "ShaderSource 0x%08X 0x%08X (0x%08X) (0x%08X)",
               &arg[0], &arg[1], &arg[2], &arg[3]) == 4)
    {
        GLuint      shader      = arg[0];
        CShaderObj  *pShader    = CURRENT_CONTEXT1().FindShader(shader);

        if (g_bSaveShader)
        {
            if (pShader->m_source != NULL)
            {
                SaveShaderToFile(shader, pShader->m_type, 1, &pShader->m_source);
            }
        }

        if (!g_bSaveShader)
        {
            return;
        }

        sprintf(output, "\n*************************************shader*************************************\n");
        OutputStrcat(output, outputSize, "glShaderSource(shader = %08d)\n\n", shader);
        OutputToTarget(OUTPUT_TO_FILE, GL_FALSE, 0, NULL, output, g_pOutputFile);
        OutputToShaderFile(output);

        OutputToTarget(OUTPUT_TO_FILE, GL_FALSE, 0, NULL, pShader->m_source, g_pOutputFile);
        OutputToShaderFile(pShader->m_source);

        OutputStrcat(output, outputSize, "*************************************shader end*********************************\n\n");
        OutputToTarget(OUTPUT_TO_FILE, GL_FALSE, 0, NULL, output, g_pOutputFile);
        OutputToShaderFile(output);
    }
}

GLvoid ParseSampleCoverage(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint arg[2];

    if (sscanf(input,
               "SampleCoverage 0x%08X 0x%08X}",
               &arg[0], &arg[1]) == 2)
    {
        PrintParams2(output, outputSize, "glSampleCoverage",
                     (GLuint)arg[0], "value", FLOAT_1_5,
                     (GLuint)arg[1], "invert", INT_04D);
    }
}

GLvoid PrintUniform1234if(GLchar *output,
                          GLuint outputSize,
                          GLuint location,
                          eShaderType type,
                          GLuint count,
                          GLuint *value,
                          const GLchar *funcName)
{
    GLuint      progID      = CURRENT_CONTEXT1().curProgram;
    CProgramObj *prog       = CURRENT_CONTEXT1().FindProgram(progID);
    CUniformObj *uniform    = prog->FindUniformByOldLoc(location);
    GLuint      totalSize   = GetSizeFromShaderDataType(type);

    if (location != 0xffffffff && uniform != NULL)
    {
        switch (type)
        {
        case DATA_TYPE_FLOAT_X1:
        case DATA_TYPE_FLOAT_X2:
        case DATA_TYPE_FLOAT_X3:
        case DATA_TYPE_FLOAT_X4:
            {
                GLfloat *fv = (GLfloat*)uniform->data;

                sprintf(output, "%s(%s,", funcName, uniform->name);
                for (GLuint i=0; i<count-1; i++)
                {
                    OutputStrcat(output, outputSize, " %f,", fv[0]);
                }
                OutputStrcat(output, outputSize, " %f)\n", fv[count-1]);
                break;
            }

        case DATA_TYPE_INTEGER_X1:
        case DATA_TYPE_INTEGER_X2:
        case DATA_TYPE_INTEGER_X3:
        case DATA_TYPE_INTEGER_X4:
            {
                GLint *iv = (GLint*)uniform->data;

                sprintf(output, "%s(%s,", funcName, uniform->name);
                for (GLuint i=0; i<count-1; i++)
                {
                    OutputStrcat(output, outputSize, " %d,", iv[0]);
                }
                OutputStrcat(output, outputSize, " %d)\n", iv[count-1]);
                break;
            }

        default:
            Abort("%s(%d): Unknown type(0x%04X)\n", __FUNCTION__, __LINE__, type);
            break;
        }
    }
    else
    {
        sprintf(output, "%s(%s) data error\n", funcName, uniform?uniform->name:"");
    }
}

GLvoid PrintUniform1234ifv(GLint *pos,
                           GLchar *output,
                           GLuint outputSize,
                           GLuint location,
                           eShaderType type,
                           GLuint count,
                           GLuint num,
                           GLuint handle,
                           const GLchar *funcName)
{
    GLuint      progID      = CURRENT_CONTEXT1().curProgram;
    CProgramObj *prog       = CURRENT_CONTEXT1().FindProgram(progID);
    CUniformObj *pUniform   = prog->FindUniformByOldLoc(location);
    GLuint      nDataSize   = num*count*4;

    if (location != 0xffffffff && pUniform != NULL)
    {
        GLchar *p = (GLchar*)pUniform->data;

        OutputStringFast(gles2Disassemble2, pos, output, outputSize, "%s(%s, %02d)\n", funcName, pUniform->name, num);
        if (p)
        {
            for (GLuint i=0; i<num; i++)
            {
                switch (type)
                {
                case DATA_TYPE_FLOAT_X1:
                case DATA_TYPE_FLOAT_X2:
                case DATA_TYPE_FLOAT_X3:
                case DATA_TYPE_FLOAT_X4:
                    DumpVector(pos, GL_FLOAT, count, p, output, outputSize);
                    break;

                case DATA_TYPE_INTEGER_X1:
                case DATA_TYPE_INTEGER_X2:
                case DATA_TYPE_INTEGER_X3:
                case DATA_TYPE_INTEGER_X4:
                    DumpVector(pos, GL_INT, count, p, output, outputSize);
                    break;

                default:
                    Abort("%s(%d): Unknown type(0x%04X)\n", __FUNCTION__, __LINE__, type);
                    break;
                }

                p += (count*4);
            }
        }
    }
    else
    {
        OutputStringFast(gles2Disassemble2, pos, output, outputSize,
            "%s(%s) loc(%d) data error\n", funcName, pUniform?pUniform->name:"null", location);
    }
}

GLvoid ParseUniform1f(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint arg[2];

    if (sscanf(input,
               "Uniform1f 0x%08X 0x%08X}",
               &arg[0], &arg[1]) == 2)
    {
        PrintUniform1234if(output, outputSize, arg[0], DATA_TYPE_FLOAT_X1, 1, (GLuint*)&arg[1], "glUniform1f");
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
        PrintUniform1234ifv(&pos, output, outputSize, arg[0], DATA_TYPE_FLOAT_X1, 1, arg[1], arg[2], "glUniform1fv");
    }
}

GLvoid ParseUniform1i(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint arg[2];

    if (sscanf(input,
               "Uniform1i 0x%08X 0x%08X}",
               &arg[0], &arg[1]) == 2)
    {
        PrintUniform1234if(output, outputSize, arg[0], DATA_TYPE_INTEGER_X1, 1, (GLuint*)&arg[1], "glUniform1i");
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
        PrintUniform1234ifv(&pos, output, outputSize, arg[0], DATA_TYPE_INTEGER_X1, 1, arg[1], arg[2], "glUniform1iv");
    }
}

GLvoid ParseUniform2f(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint arg[3];

    if (sscanf(input,
               "Uniform2f 0x%08X 0x%08X 0x%08X}",
               &arg[0], &arg[1], &arg[2]) == 3)
    {
        PrintUniform1234if(output, outputSize, arg[0], DATA_TYPE_FLOAT_X2, 1, (GLuint*)&arg[1], "glUniform2f");
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
        PrintUniform1234ifv(&pos, output, outputSize, arg[0], DATA_TYPE_FLOAT_X2, 2, arg[1], arg[2], "glUniform2fv");
    }
}

GLvoid ParseUniform2i(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint arg[3];

    if (sscanf(input,
               "Uniform2i 0x%08X 0x%08X 0x%08X}",
               &arg[0], &arg[1], &arg[2]) == 3)
    {
        PrintUniform1234if(output, outputSize, arg[0], DATA_TYPE_INTEGER_X2, 1, (GLuint*)&arg[1], "glUniform2i");
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
        PrintUniform1234ifv(&pos, output, outputSize, arg[0], DATA_TYPE_INTEGER_X2, 2, arg[1], arg[2], "glUniform2iv");
    }
}

GLvoid ParseUniform3f(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint arg[4];

    if (sscanf(input,
               "Uniform3f 0x%08X 0x%08X 0x%08X 0x%08X}",
               &arg[0], &arg[1], &arg[2], &arg[3]) == 4)
    {
        PrintUniform1234if(output, outputSize, arg[0], DATA_TYPE_FLOAT_X3, 1, (GLuint*)&arg[1], "glUniform3f");
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
        PrintUniform1234ifv(&pos, output, outputSize, arg[0], DATA_TYPE_FLOAT_X3, 3, arg[1], arg[2], "glUniform3fv");
    }
}

GLvoid ParseUniform3i(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint arg[4];

    if (sscanf(input,
               "Uniform3i 0x%08X 0x%08X 0x%08X 0x%08X}",
               &arg[0], &arg[1], &arg[2], &arg[3]) == 4)
    {
        PrintUniform1234if(output, outputSize, arg[0], DATA_TYPE_INTEGER_X3, 1, (GLuint*)&arg[1], "glUniform3i");
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
        PrintUniform1234ifv(&pos, output, outputSize, arg[0], DATA_TYPE_INTEGER_X3, 3, arg[1], arg[2], "glUniform3iv");
    }
}

GLvoid ParseUniform4f(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint arg[5];

    if (sscanf(input,
               "Uniform4f 0x%08X 0x%08X 0x%08X 0x%08X 0x%08X}",
               &arg[0], &arg[1], &arg[2], &arg[3], &arg[4]) == 5)
    {
        PrintUniform1234if(output, outputSize, arg[0], DATA_TYPE_FLOAT_X4, 1, (GLuint*)&arg[1], "glUniform4f");
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
        PrintUniform1234ifv(&pos, output, outputSize, arg[0], DATA_TYPE_FLOAT_X4, 4, arg[1], arg[2], "glUniform4fv");
    }
}

GLvoid ParseUniform4i(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint arg[5];

    if (sscanf(input,
               "Uniform4i 0x%08X 0x%08X 0x%08X 0x%08X 0x%08X}",
               &arg[0], &arg[1], &arg[2], &arg[3], &arg[4]) == 5)
    {
        PrintUniform1234if(output, outputSize, arg[0], DATA_TYPE_INTEGER_X4, 1, (GLuint*)&arg[1], "glUniform4i");
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
        PrintUniform1234ifv(&pos, output, outputSize, arg[0], DATA_TYPE_INTEGER_X4, 4, arg[1], arg[2], "glUniform4iv");
    }
}

GLvoid PrintUniformMatrix234fv(GLint *pos,
                               GLchar *output,
                               GLuint outputSize,
                               GLuint location,
                               eShaderType type,
                               GLuint count,
                               GLuint num,
                               GLuint handle,
                               GLuint transpose,
                               const GLchar *funcName)
{
    GLuint      progID      = CURRENT_CONTEXT1().curProgram;
    CProgramObj *prog       = CURRENT_CONTEXT1().FindProgram(progID);
    CUniformObj *pUniform   = prog->FindUniformByOldLoc(location);

    if (location != 0xffffffff && pUniform != NULL)
    {
        GLfloat *p = (GLfloat*)pUniform->data;

        OutputStringFast(gles2Disassemble11, pos, output, outputSize, "%s(%s, %02d, t=%d)\n",
            funcName, pUniform->name, num, transpose);

        for (GLuint j=0; j<num; j++)
        {
            OutputStringFast(gles2Disassemble10, pos, output, outputSize, "m%02d:\n", j+1);

            if (count == 16)
            {
                DumpMatrix(pos, GL_FLOAT, 4, 4, p, output, outputSize);
            }
            else if (count == 9)
            {
                DumpMatrix(pos, GL_FLOAT, 3, 3, p, output, outputSize);
            }
            else if (count == 4)
            {
                DumpMatrix(pos, GL_FLOAT, 2, 2, p, output, outputSize);
            }

            p += count;
        }
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
        PrintUniformMatrix234fv(&pos, output, outputSize, arg[0], DATA_TYPE_FLOAT_2X2,
                                4, arg[1], arg[3], arg[2], "glUniformMatrix2fv");
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
        PrintUniformMatrix234fv(&pos, output, outputSize, arg[0], DATA_TYPE_FLOAT_3X3,
                                9, arg[1], arg[3], arg[2], "glUniformMatrix3fv");
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
        PrintUniformMatrix234fv(&pos, output, outputSize, arg[0], DATA_TYPE_FLOAT_4X4,
                                16, arg[1], arg[3], arg[2], "glUniformMatrix4fv");
    }
}

GLvoid ParseUseProgram(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint arg[1];

    if (sscanf(input,
               "UseProgram 0x%08X}",
               &arg[0]) == 1)
    {
        PrintParams1(output, outputSize, "glUseProgram", arg[0], "prog", INT_08D);
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
        PrintParams4(output, outputSize, "glViewport",
                     (GLuint)arg[0], "", INT_04D_NO,
                     (GLuint)arg[1], "", INT_04D_NO,
                     (GLuint)arg[2], "", INT_04D_NO,
                     (GLuint)arg[3], "", INT_04D_NO);
    }
}

GLvoid ParseValidateProgram(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint arg[1];

    if (sscanf(input,
               "ValidateProgram 0x%08X}",
               &arg[0]) == 1)
    {
        PrintParams1(output, outputSize, "glValidateProgram", (GLuint)arg[0], "prog", INT_04D);
    }
}