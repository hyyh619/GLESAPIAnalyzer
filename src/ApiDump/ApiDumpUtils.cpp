#include <stdio.h>
#include <memory.h>
#include <string.h>
#ifdef WIN32
#include <Windows.h>
#endif
#include <GLES3/gl31.h>

#include "utils.h"
#include "bitmap.h"
#include "PngInterface.h"
#include "ApiDebug.h"
#include "ApiDumpUtils.h"

GLchar    dumpTmp1[TMP_BUF_SIZE];
GLchar    dumpTmp2[TMP_BUF_SIZE];
GLchar    dumpTmp3[TMP_BUF_SIZE];
GLchar    dumpTmp4[TMP_BUF_SIZE];
GLchar    dumpTmp5[TMP_BUF_SIZE];
GLchar    dumpTmp6[TMP_BUF_SIZE];

#ifdef WIN32
BOOL DumpFindFirstFileExists(LPCTSTR lpPath, DWORD dwFilter)
{
    WIN32_FIND_DATA fd;  
    HANDLE hFind = FindFirstFile(lpPath, &fd);  
    BOOL bFilter = (FALSE == dwFilter) ? TRUE : fd.dwFileAttributes & dwFilter;  
    BOOL RetValue = ((hFind != INVALID_HANDLE_VALUE) && bFilter) ? TRUE : FALSE;  
    FindClose(hFind);  
    return RetValue;  
}

/* 
 * 检查一个  路径 是否存在（绝对路径、相对路径，文件或文件夹均可） 
 * 存在则返回 1 (TRUE) 
 */  
BOOL DumpFilePathExists(LPCTSTR lpPath)
{
    return DumpFindFirstFileExists(lpPath, FALSE);
}

BOOL DumpCreateDirectory(LPCTSTR lpPath)
{
    return CreateDirectory(lpPath, NULL);
}
#endif

U32 DumpAQFloat16ToFloat(U16 in)
{
    U01 signIn = AQFLOAT16_GetSign(in);
    S06 expIn  = AQFLOAT16_GetExponent(in);

    if (expIn == 0)
        return FORMSINGLEFLOAT(signIn, 0, 0);

    if (expIn == AQFLOAT16_INFINITY)
        return FORMSINGLEFLOAT(signIn, SINGLEFLOATEXPINFINITY, 0);

    U10 manIn = U10(AQFLOAT16_GetMantissa(in) & AQFLOAT16_MANTISSAMASK);

    U08 expOut = expIn - AQFLOAT16_BIAS + SINGLEFLOATEXPBIAS;
    U23 manOut = U23(manIn) << (SINGLEFLOATMAN - AQFLOAT16_MANTISSASIZE);

    return FORMSINGLEFLOAT(signIn, expOut, manOut);
}

/*---------------------------------------------------------------------------
 *
 * Function:    AQFloatToFloat16
 *
 * Description: Convert a 32-bit floating point into a 16-bit floating point.
 *
 * Input:       FL32 In -> 32-bit floating point number to convert.
 *
 * Output:      F_S1E5M10 -> 16-bit floating point number.
 *
 */
F_S1E5M10 DumpAQFloatToFloat16(FL32 In)
{
    // Compute unbiased exponent.
    int Exponent = GETBITS(In, 23, 30) - 127;

    F_S1E5M10 Result;
    if (Exponent <= -AQFLOAT16_BIAS)
    {
        // Undeflow; set to zero.
        Result = AQFLOAT16_Construct(U01(GETBITS(In, 31, 31)), 0, 0);
    }
    else if (Exponent > AQFLOAT16_BIAS)
    {
        // Overflow; set to infinity.
        Result = AQFLOAT16_Construct(U01(GETBITS(In, 31, 31)),
                                     AQFLOAT16_INFINITY,
                                     0);
    }
    else
    {
        // Strip a number of bits.
        Result = AQFLOAT16_Construct(U01(GETBITS(In, 31, 31)),
                                     Exponent + AQFLOAT16_BIAS,
                                     U10(GETBITS(In, 13, 22)));
    }

    // Return result.
    return Result;
}

static GLvoid DumpDecodeETC1Block(GLubyte        *Output,
                                  GLsizei        Stride,
                                  GLsizei        Width,
                                  GLsizei        Height,
                                  const GLubyte  *Data)
{
    GLubyte     base[2][3];
    GLboolean   flip;
    GLboolean   diff;
    GLbyte      index[2];
    GLint       i;
    GLint       j;
    GLint       x;
    GLint       y;
    GLint       offset;

    static GLubyte table[][2] =
    {
        {  2,   8 },
        {  5,  17 },
        {  9,  29 },
        { 13,  42 },
        { 18,  60 },
        { 24,  80 },
        { 33, 106 },
        { 47, 183 },
    };

    diff = Data[3] & 0x2;
    flip = Data[3] & 0x1;

    if (diff)
    {
        GLbyte delta[3];

        base[0][0] = (Data[0] & 0xF8) | (Data[0] >> 5);
        base[0][1] = (Data[1] & 0xF8) | (Data[1] >> 5);
        base[0][2] = (Data[2] & 0xF8) | (Data[1] >> 5);

        delta[0] = (GLbyte) ((Data[0] & 0x7) << 5) >> 2;
        delta[1] = (GLbyte) ((Data[1] & 0x7) << 5) >> 2;
        delta[2] = (GLbyte) ((Data[2] & 0x7) << 5) >> 2;
        base[1][0] = base[0][0] + delta[0];
        base[1][1] = base[0][1] + delta[1];
        base[1][2] = base[0][2] + delta[2];
        base[1][0] |= base[1][0] >> 5;
        base[1][1] |= base[1][1] >> 5;
        base[1][2] |= base[1][2] >> 5;
    }
    else
    {
        base[0][0] = (Data[0] & 0xF0) | (Data[0] >> 4  );
        base[0][1] = (Data[1] & 0xF0) | (Data[1] >> 4  );
        base[0][2] = (Data[2] & 0xF0) | (Data[2] >> 4  );
        base[1][0] = (Data[0] << 4  ) | (Data[0] & 0x0F);
        base[1][1] = (Data[1] << 4  ) | (Data[1] & 0x0F);
        base[1][2] = (Data[2] << 4  ) | (Data[2] & 0x0F);
    }

    index[0] = (Data[3] & 0xE0) >> 5;
    index[1] = (Data[3] & 0x1C) >> 2;

    for (i = x = y = offset = 0; i < 2; ++i)
    {
        GLubyte msb = Data[5 - i];
        GLubyte lsb = Data[7 - i];

        for (j = 0; j < 8; ++j)
        {
            GLuint delta = 0;
            GLint r, g, b;
            GLint block = flip
                        ? (y < 2) ? 0 : 1
                        : (x < 2) ? 0 : 1;

            switch (((msb & 1) << 1) | (lsb & 1))
            {
            case 0x3: delta = -table[index[block]][1]; break;
            case 0x2: delta = -table[index[block]][0]; break;
            case 0x0: delta =  table[index[block]][0]; break;
            case 0x1: delta =  table[index[block]][1]; break;
            }

            r = base[block][0] + delta; r = MAX(0x00, MIN(r, 0xFF));
            g = base[block][1] + delta; g = MAX(0x00, MIN(g, 0xFF));
            b = base[block][2] + delta; b = MAX(0x00, MIN(b, 0xFF));

            if ((x < Width) && (y < Height))
            {
                Output[offset + 0] = (GLubyte) r;
                Output[offset + 1] = (GLubyte) g;
                Output[offset + 2] = (GLubyte) b;
            }

            offset += Stride;
            if (++y == 4)
            {
                y = 0;
                ++x;

                offset += 3 - 4 * Stride;
            }

            msb >>= 1;
            lsb >>= 1;
        }
    }
}

GLvoid* DumpDecompressETC1(GLsizei      Width,
                         GLsizei      Height,
                         GLsizei      ImageSize,
                         const GLvoid   *Data,
                         GLint         *Format)
{
    GLubyte         *pixels = NULL;
    GLubyte         *line   = NULL;
    const GLubyte   *data;
    GLsizei         x;
    GLsizei         y;
    GLsizei         stride;

    pixels = (GLubyte*)malloc(Width * Height * 3);
    if (pixels == NULL)
    {
        printf("%s(%d): allocate memory failed\n", __FUNCTION__, __LINE__);
        return NULL;
    }

    stride = Width * 3;
    data   = (const GLubyte*)Data;

    for (y = 0, line = pixels; y < Height; y += 4, line += stride * 4)
    {
        GLubyte * p = line;

        for (x = 0; x < Width; x += 4)
        {
            DumpDecodeETC1Block(p,
                                stride,
                                MIN(Width - x, 4),
                                MIN(Height - y, 4),
                                data);

            p         += 4 * 3;
            data      += 8;
            ImageSize -= 8;
        }
    }

    *Format = BGR_888;

    return pixels;
}

GLint DumpConvertFormat(GLenum texFormat, GLenum texType)
{
    GLint format = RGBA_8888;

    switch (texFormat)
    {
    case GL_ALPHA:
        switch (texType)
        {
        case GL_UNSIGNED_BYTE:
            format = L8;
            break;

        case GL_UNSIGNED_SHORT:
            format = RGBA_4444;
            break;

        case GL_UNSIGNED_INT:
            format = RGBA_8888;
            break;
        }
        break;

    case GL_RGB:
        switch (texType)
        {
        case GL_UNSIGNED_SHORT_4_4_4_4:
            format = RGBA_4444;
            break;

        case GL_UNSIGNED_SHORT_5_5_5_1:
            format = ARGB_1555;
            break;

        case GL_UNSIGNED_SHORT_5_6_5:
            format = RGB_565;
            break;

        case GL_UNSIGNED_BYTE:
            format = RGB_888;
            break;
        }
        break;

    case GL_RGB565:
        format = RGB_565;
        break;

    case GL_RGB8:
        format = RGB_888;
        break;

    case GL_RGBA:
        switch (texType)
        {
        case GL_UNSIGNED_BYTE:
            format = RGBA_8888;
            break;

        case GL_UNSIGNED_SHORT_4_4_4_4:
            format = RGBA_4444;
            break;

        case GL_UNSIGNED_SHORT_5_5_5_1:
            format = ARGB_1555;
            break;
        }
        break;

    case GL_RGBA8:
        format = RGBA_8888;
        break;

    case GL_LUMINANCE:
        switch (texType)
        {
        case GL_UNSIGNED_BYTE:
            format = L8;
            break;

        case GL_UNSIGNED_SHORT:
            format = RGBA_4444;
            break;

        case GL_UNSIGNED_INT:
            format = RGBA_8888;
            break;
        }
        break;

    case GL_BGRA_EXT:
        switch (texType)
        {
        case GL_UNSIGNED_BYTE:
            format = BGRA_8888;
            break;
        }
        break;

    case GL_RGBA4:
        format = RGBA_4444;
        break;

    case GL_ETC1_RGB8_OES:
        break;

    case GL_COMPRESSED_RGB8_ETC2:
        break;

    default:
        Abort("%s(%d): cannot match format(0x%04X).", __FUNCTION__, __LINE__, texFormat);
        break;
    }

    return format;
}

GLuint DumpGetShaderDataSize(GLenum type)
{
    GLuint size = 0;

    switch (type)
    {
    case GL_FLOAT:
        size = 4;
        break;

    case GL_FLOAT_VEC2:
        size = 8;
        break;

    case GL_FLOAT_VEC3:
        size = 12;
        break;

    case GL_FLOAT_VEC4:
        size = 16;
        break;

    case GL_SAMPLER_2D:
        size = 4;
        break;

    case GL_SAMPLER_CUBE:
        size = 4;
        break;

    case GL_INT:
        size = 4;
        break;

    case GL_INT_VEC2:
        size = 8;
        break;

    case GL_INT_VEC3:
        size = 12;
        break;

    case GL_INT_VEC4:
        size = 16;
        break;

    case GL_BOOL:
        size = 4;
        break;

    case GL_BOOL_VEC2:
        size = 8;
        break;

    case GL_BOOL_VEC3:
        size = 12;
        break;

    case GL_BOOL_VEC4:
        size = 16;
        break;

    case GL_FLOAT_MAT2:
        size = 16;
        break;

    case GL_FLOAT_MAT3:
        size = 36;
        break;

    case GL_FLOAT_MAT4:
        size = 64;
        break;

    default:
        break;
    }

    return size;
}

GLuint DumpGetDataSize(GLenum type)
{
    GLuint size = 0;

    switch (type)
    {
    case GL_UNSIGNED_BYTE:
        size = 1;
        break;

    case GL_UNSIGNED_SHORT:
    case GL_SHORT:
        size = 2;
        break;

    case GL_UNSIGNED_INT:
        size = 4;
        break;

    case GL_HALF_FLOAT_OES:
    case GL_HALF_FLOAT:
        size = 2;
        break;

    case GL_FLOAT:
        size = 4;
        break;

    default:
        Abort("%s(%d): type(0x%04X)\n", __FUNCTION__, __LINE__, type);
        break;
    }

    return size;
}

GLuint DumpGetPixelSize(GLenum texFormat, GLenum texType)
{
    GLuint bpp  = 0;

    switch (texFormat)
    {
    case GL_DEPTH_COMPONENT:
        switch (texType)
        {
        case GL_UNSIGNED_SHORT:
            bpp = 16;
            break;

        case GL_DEPTH_COMPONENT24_OES:
            bpp = 32;
            break;

        case GL_DEPTH_COMPONENT32_OES:
        case GL_UNSIGNED_INT:
            bpp = 32;
            break;
        }
        break;

    case GL_ALPHA:
        switch (texType)
        {
        case GL_UNSIGNED_BYTE:
            bpp = 8;
            break;

        case GL_UNSIGNED_SHORT:
            bpp = 16;
            break;

        case GL_UNSIGNED_INT:
            bpp = 32;
            break;

        case GL_HALF_FLOAT_OES:
            bpp = 16;
            break;

        case GL_FLOAT:
            bpp = 32;
            break;
        }
        break;

    case GL_RGB:
        switch (texType)
        {
        case GL_UNSIGNED_SHORT_5_6_5:
            bpp = 16;
            break;

        case GL_UNSIGNED_BYTE:
            bpp = 24;
            break;

        case GL_UNSIGNED_INT_2_10_10_10_REV_EXT:
            bpp = 32;
            break;

        case GL_HALF_FLOAT_OES:
            bpp = 64;
            break;
        }
        break;

    case GL_RGBA:
        switch (texType)
        {
        case GL_UNSIGNED_BYTE:
        case GL_UNSIGNED_INT_2_10_10_10_REV_EXT:
            bpp = 32;
            break;

        case GL_UNSIGNED_SHORT_4_4_4_4:
        case GL_UNSIGNED_SHORT_5_5_5_1:
            bpp = 16;
            break;

        case GL_HALF_FLOAT_OES:
            bpp = 64;
            break;
        }
        break;

    case GL_LUMINANCE:
        switch (texType)
        {
        case GL_UNSIGNED_BYTE:
            bpp = 8;
            break;

        case GL_UNSIGNED_SHORT:
            bpp = 16;
            break;

        case GL_UNSIGNED_INT:
            bpp = 32;
            break;

        case GL_HALF_FLOAT_OES:
            bpp = 16;
            break;

        case GL_FLOAT:
            bpp = 32;
            break;
        }
        break;

    case GL_LUMINANCE_ALPHA:
        switch (texType)
        {
        case GL_UNSIGNED_BYTE:
            bpp = 16;
            break;

        case GL_UNSIGNED_SHORT:
            bpp = 32;
            break;

        case GL_HALF_FLOAT_OES:
            bpp = 32;
            break;

        case GL_FLOAT:
            bpp = 64;
            break;
        }
        break;

    case GL_BGRA_EXT:
        switch (texType)
        {
        case GL_UNSIGNED_BYTE:
            bpp = 32;
            break;
        }
        break;

    case GL_DEPTH_STENCIL_OES:
        switch (texType)
        {
        case GL_UNSIGNED_INT_24_8_OES:
            bpp = 32;
            break;
        }
        break;

    case GL_ETC1_RGB8_OES:
    case GL_COMPRESSED_RGB8_ETC2:
        break;

    case GL_R8:
    case GL_R8I:
    case GL_R8UI:
        bpp = 8;
        break;

    case GL_R16F_EXT:
    case GL_RGB565:
    case GL_RGBA4:
    case GL_RGB5_A1:
    case GL_RG8:
    case GL_RG8I:
    case GL_RG8UI:
    case GL_R16I:
    case GL_R16UI:
        bpp = 16;
        break;

    case GL_RGB8UI:
    case GL_RGB8I:
    case GL_RGB8:
        bpp = 24;
        break;

    case GL_R32F:
    case GL_RG16F:
    case GL_RG16I:
    case GL_RG16UI:
    case GL_RGB10_A2:
    case GL_RGBA8UI:
    case GL_RGBA8I:
    case GL_R32I:
    case GL_R32UI:
    case GL_RGBA8:
    case GL_DEPTH24_STENCIL8:
        bpp = 32;
        break;

    case GL_RGB16F:
    case GL_RGB16UI:
    case GL_RGB16I:
        bpp = 48;
        break;

    case GL_RG32F:
    case GL_RG32I:
    case GL_RG32UI:
    case GL_RGBA16F:
    case GL_RGBA16UI:
        bpp = 64;
        break;

    case GL_RGB32F:
    case GL_RGB32UI:
    case GL_RGB32I:
    case GL_RGBA16I:
        bpp = 96;
        break;

    case GL_RGBA32F:
    case GL_RGBA32UI:
    case GL_RGBA32I:
        bpp = 128;
        break;

    default:
#ifdef WIN32
        __asm int 3;
#endif
        printf("%s(%d): cannot match format.", __FUNCTION__, __LINE__);
        break;
    }

    return bpp/8;
}

GLuint DumpGetPrimitiveCountPerMode(GLenum mode, GLuint count)
{
    GLuint primitive = 0;

    switch (mode)
    {
    case GL_POINTS:
        primitive = count * 2;
        break;

    case GL_LINES:
        primitive = count;
        break;

    case GL_LINE_LOOP:
        primitive = count * 2;
        break;

    case GL_LINE_STRIP:
        primitive = (count - 1) * 2;
        break;

    case GL_TRIANGLES:
        primitive = count / 3;
        break;

    case GL_TRIANGLE_STRIP:
    case GL_TRIANGLE_FAN:
        primitive = count - 2;
        break;

    default:
#ifdef WIN32
        __asm int 3;
#endif
        break;
    }

    return primitive;
}

GLvoid DumpTranslateBlendMode(GLenum BlendModeGL, GLchar *output)
{
    memset(output, 0, TMP_BUF_SIZE);

    switch (BlendModeGL)
    {
    case GL_FUNC_ADD:
        sprintf(output, "ADD             ");
        break;

    case GL_FUNC_SUBTRACT:
        sprintf(output, "SUBTRACT        ");
        break;

    case GL_FUNC_REVERSE_SUBTRACT:
        sprintf(output, "REVERSE_SUBTRACT");
        break;

    case GL_MIN_EXT:
        sprintf(output, "MIN             ");
        break;

    case GL_MAX_EXT:
        sprintf(output, "MAX             ");
        break;

    default:
        sprintf(output, "Invalid         ");
        break;
    }

    return;
}

GLvoid DumpTranslateBlendFunc(GLenum BlendFuncGL, GLchar *output)
{
    memset(output, 0, TMP_BUF_SIZE);

    switch (BlendFuncGL)
    {
    case GL_ZERO:
        sprintf(output, "ZERO           ");
        break;

    case GL_ONE:
        sprintf(output, "ONE            ");
        break;

    case GL_SRC_COLOR:
        sprintf(output, "SOURCE_COLOR   ");
        break;

    case GL_ONE_MINUS_SRC_COLOR:
        sprintf(output, "INV_SRC_COLOR  ");
        break;

    case GL_DST_COLOR:
        sprintf(output, "TARGET_COLOR   ");
        break;

    case GL_ONE_MINUS_DST_COLOR:
        sprintf(output, "INV_DST_COLOR  ");
        break;

    case GL_SRC_ALPHA:
        sprintf(output, "SOURCE_ALPHA   ");
        break;

    case GL_ONE_MINUS_SRC_ALPHA:
        sprintf(output, "INV_SRC_ALPHA  ");
        break;

    case GL_DST_ALPHA:
        sprintf(output, "DST_ALPHA      ");
        break;

    case GL_ONE_MINUS_DST_ALPHA:
        sprintf(output, "INV_DST_ALPHA  ");
        break;

    case GL_CONSTANT_COLOR:
        sprintf(output, "CONST_COLOR    ");
        break;

    case GL_ONE_MINUS_CONSTANT_COLOR:
        sprintf(output, "INV_CONST_COLOR");
        break;

    case GL_CONSTANT_ALPHA:
        sprintf(output, "CONST_ALPHA    ");
        break;

    case GL_ONE_MINUS_CONSTANT_ALPHA:
        sprintf(output, "INV_CONST_ALPHA");
        break;

    case GL_SRC_ALPHA_SATURATE:
        sprintf(output, "SRC_ALPHA_SAT  ");
        break;

    default:
        sprintf(output, "Invalid        ");
        break;
    }

    return;
}

GLvoid DumpTranslateFunc(GLenum Func, GLchar *output)
{
    memset(output, 0, TMP_BUF_SIZE);

    switch (Func)
    {
    case GL_NEVER:
        sprintf(output, "GL_NEVER   ");
        break;

    case GL_ALWAYS:
        sprintf(output, "GL_ALWAYS  ");
        break;

    case GL_LESS:
        sprintf(output, "GL_LESS    ");
        break;

    case GL_LEQUAL:
        sprintf(output, "GL_LEQUAL  ");
        break;

    case GL_EQUAL:
        sprintf(output, "GL_EQUAL   ");
        break;

    case GL_GREATER:
        sprintf(output, "GL_GREATER ");
        break;

    case GL_GEQUAL:
        sprintf(output, "GL_GEQUAL  ");
        break;

    case GL_NOTEQUAL:
        sprintf(output, "GL_NOTEQUAL");
        break;

    default:
        sprintf(output, "Invalid    ");
        break;
    }
}

GLvoid DumpTranslateOperation(GLenum Operation, GLchar *output)
{
    memset(output, 0, TMP_BUF_SIZE);

    switch (Operation)
    {
    case GL_KEEP:
        sprintf(output, "GL_KEEP        ");
        break;

    case GL_ZERO:
        sprintf(output, "GL_ZERO        ");
        break;

    case GL_REPLACE:
        sprintf(output, "GL_REPLACE     ");
        break;

    case GL_INCR:
        sprintf(output, "GL_INCR        ");
        break;

    case GL_DECR:
        sprintf(output, "GL_DECR        ");
        break;

    case GL_INVERT:
        sprintf(output, "GL_INVERT      ");
        break;

    case GL_INCR_WRAP:
        sprintf(output, "GL_INCR_WRAP   ");
        break;

    case GL_DECR_WRAP:
        sprintf(output, "GL_DECR_WRAP   ");
        break;

    default:
        sprintf(output, "Invalid        ");
        break;
    }
}

GLvoid DumpWriteFloat(FILE *pFile, GLfloat data)
{
    GLuint writeBytes = 0;

    writeBytes = fwrite(&data, 1, 4, pFile);
    if (writeBytes != 4)
    {
        printf("%s(%d): cannot write file(%d).",
            __FUNCTION__, __LINE__, writeBytes);
    }

    return;
}

GLvoid DumpWriteUShort(FILE *pFile, GLushort data)
{
    GLuint writeBytes = 0;

    writeBytes = fwrite(&data, 1, 2, pFile);
    if (writeBytes != 2)
    {
        printf("%s(%d): cannot write file(%d).",
            __FUNCTION__, __LINE__, writeBytes);
    }

    return;
}

GLvoid DumpWriteUInt(FILE *pFile, GLuint data)
{
    GLuint writeBytes = 0;

    writeBytes = fwrite(&data, 1, 4, pFile);
    if (writeBytes != 4)
    {
        printf("%s(%d): cannot write file(%d).",
            __FUNCTION__, __LINE__, writeBytes);
    }

    return;
}

GLvoid DumpWriteShort(FILE *pFile, GLshort data)
{
    GLuint writeBytes = 0;

    writeBytes = fwrite(&data, 1, 2, pFile);
    if (writeBytes != 2)
    {
        printf("%s(%d): cannot write file(%d).",
            __FUNCTION__, __LINE__, writeBytes);
    }

    return;
}

GLvoid DumpWriteInt(FILE *pFile, GLint data)
{
    GLuint writeBytes = 0;

    writeBytes = fwrite(&data, 1, 4, pFile);
    if (writeBytes != 4)
    {
        printf("%s(%d): cannot write file(%d).",
            __FUNCTION__, __LINE__, writeBytes);
    }

    return;
}

GLvoid DumpWriteBytes(FILE *pFile, GLvoid *pData, GLuint size)
{
    GLuint writeBytes = 0;

    DumpWriteInt(pFile, size);

    writeBytes = fwrite(pData, 1, size, pFile);
    if (writeBytes != size)
    {
        printf("%s(%d): cannot write file(%d, %d).",
            __FUNCTION__, __LINE__, writeBytes, size);
    }

    return;
}

GLvoid DumpShaderToSingleFile(GLchar *path1, GLuint name, GLenum type, GLchar *source)
{
    GLuint  len         = strlen(source);
    GLuint  writeBytes  = 0;
    FILE    *pFile      = NULL;
    GLchar  fileName[256];
    GLchar  path[256];

    memset(path, 0, 256);
    sprintf(path, "%sdraw/shader", path1);

#ifdef WIN32
    {
        BOOL bRes = DumpFilePathExists(path);
        if (!bRes)
        {
            bRes = DumpCreateDirectory(path);
            if (!bRes)
            {
                printf("%s(%d): execute %s.", __FUNCTION__, __LINE__, path);
                return;
            }
        }
    }
#endif

    memset(fileName, 0, 256);
    sprintf(fileName, "%s/%d.%s", path, name, type==GL_VERTEX_SHADER?"vert":"frag");
    pFile = fopen(fileName, "w");
    if (!pFile)
    {
        printf("%s(%d): cannot open file(%s).\n", __FUNCTION__, __LINE__, fileName);
        return;
    }

    writeBytes = fwrite(source, 1, len, pFile);
    if (len != writeBytes)
    {
        printf("%s(%d): cannot write shader source to file.\n", __FUNCTION__, __LINE__);
        fclose(pFile);
        return;
    }

    fclose(pFile);
}

GLvoid DumpWriteFile(FILE *pFile, const GLchar *fmt, ...)
{
    GLchar  buf[2048];
    va_list arguments;
    GLuint  writeBytes;
    GLuint  len = 0;

    if (pFile == NULL)
    {
        printf("%s(%d): dump draw file is null.", __FUNCTION__, __LINE__);
        return;
    }

    memset(buf, 0, 2048);
    va_start(arguments, fmt);
    vsnprintf(buf, 2048, fmt, arguments);
    va_end(arguments);

    len = strlen(buf);
    writeBytes = fwrite(buf, 1, len, pFile);
    if (len != writeBytes)
    {
        printf("%s(%d): cannot write to dump draw file.", __FUNCTION__, __LINE__);
        return;
    }
}

GLvoid DumpFlushFile(FILE *pFile)
{
    if (pFile)
    {
        fflush(pFile);
    }
}

GLvoid DumpWriteFileWithFlush(FILE *pFile, const GLchar *fmt, ...)
{
    GLchar  buf[2048];
    va_list arguments;
    GLuint  writeBytes;
    GLuint  len = 0;

    if (pFile == NULL)
    {
        printf("%s(%d): dump draw file is null.", __FUNCTION__, __LINE__);
        return;
    }

    memset(buf, 0, 2048);
    va_start(arguments, fmt);
    vsnprintf(buf, 2048, fmt, arguments);
    va_end(arguments);

    len = strlen(buf);
    writeBytes = fwrite(buf, 1, len, pFile);
    if (len != writeBytes)
    {
        printf("%s(%d): cannot write to dump draw file.", __FUNCTION__, __LINE__);
        return;
    }

    fflush(pFile);
}

GLvoid DumpSavePicture(GLchar *fileName, GLint width, GLint height, GLubyte *pData, GLboolean bPng)
{
    GLchar buf[256];

    memset(buf, 0, 256);
    sprintf(buf, "%s.%s", fileName, bPng?"png":"bmp");
    if (bPng)
    {
        pic_data pic_d;

        pic_d.width     = width;
        pic_d.height    = height;
        pic_d.bit_depth = 8;
        pic_d.flag      = HAVE_ALPHA;

        GLubyte   *rgba[4];
        GLubyte   *r, *g, *b, *a;
        GLubyte   *pdata = (GLubyte*)pData;

        r = rgba[0] = (GLubyte*)malloc(width*height);
        g = rgba[1] = (GLubyte*)malloc(width*height);
        b = rgba[2] = (GLubyte*)malloc(width*height);
        a = rgba[3] = (GLubyte*)malloc(width*height);
        r = rgba[0];
        g = rgba[1];
        b = rgba[2];
        a = rgba[3];

        for(GLint i = 0; i < width*height; i++)
        {
            *r = pdata[0];
            *g = pdata[1];
            *b = pdata[2];
            *a = pdata[3];
            r++;
            g++;
            b++;
            a++;
            pdata+=4;
        }

        pic_d.rgba = rgba;
        write_png_file(buf, &pic_d);

        free(rgba[0]);
        free(rgba[1]);
        free(rgba[2]);
        free(rgba[3]);
    }
    else
    {
        ApiWriteBitmap(buf, pData, width, height, width*4, RGBA_8888, GL_FALSE);
    }
}