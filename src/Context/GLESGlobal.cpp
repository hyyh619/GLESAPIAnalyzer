#include <memory.h>
#include <GLES3/gl31.h>
#include <GLES2/gl2ext.h>
#include "utils.h"
#include "GLESGlobal.h"

GLuint GetDataTypeSize(GLenum type)
{
    GLuint              size;

    switch (type)
    {
    case GL_BYTE:
        size    = sizeof(GLbyte);
        break;

    case GL_UNSIGNED_BYTE:
        size    = sizeof(GLubyte);
        break;

    case GL_SHORT:
        size    = sizeof(GLshort);
        break;

    case GL_UNSIGNED_SHORT:
        size    = sizeof(GLushort);
        break;

    case GL_FIXED:
        size    = sizeof(GLfixed);
        break;

    case GL_FLOAT:
        size    = sizeof(GLfloat);
        break;

    case GL_HALF_FLOAT_OES:
        size    = (sizeof(GLfloat) / 2);
        break;

    case GL_UNSIGNED_INT_10_10_10_2_OES:
    case GL_INT_10_10_10_2_OES:
        /* Regardless of size being 3 or 4, the container size is 4. */
        size    = 4 * sizeof(GLubyte);
        break;

    default:
        /* Invalid type. */
        size    = 0;
        break;
    }

    return size;
}

GLuint GetPixelSizeByTexFormat(GLenum format, GLenum type)
{
    GLuint pixelSize = 0;

    switch (type)
    {
    case 0:
        switch (format)
        {
        case GL_RGBA8:
            pixelSize = 4;
            break;

        case GL_RGB565:
        case GL_RGBA4:
            pixelSize = 2;
            break;

        case GL_COMPRESSED_RGB8_ETC2:
            pixelSize = 0;
            break;

        case GL_RGB8_OES:
            pixelSize = 3;
            break;

        case GL_DEPTH24_STENCIL8:
            pixelSize = 4;
            break;

        default:
            Abort("%s(%d): unknown format(0x%04X)\n", __FUNCTION__, __LINE__, format);
            break;
        }
        break;

    case GL_UNSIGNED_BYTE:
        {
            switch (format)
            {
            case GL_ALPHA:
            case GL_LUMINANCE:
                pixelSize = 1;
                break;

            case GL_LUMINANCE_ALPHA:
                pixelSize = 2;
                break;

            case GL_RGB:
                pixelSize = 3;
                break;

            case GL_RGBA:
            case GL_BGRA_EXT:
                pixelSize = 4;
                break;

            case GL_COMPRESSED_RGB_S3TC_DXT1_EXT:
            case GL_COMPRESSED_RGBA_S3TC_DXT1_EXT:
            case GL_COMPRESSED_RGBA_S3TC_DXT3_EXT:
            case GL_COMPRESSED_RGBA_S3TC_DXT5_EXT:
            case GL_ETC1_RGB8_OES:
                pixelSize = 0;
                break;

            default:
                Abort("%s(%d): unknown format(0x%04X)\n", __FUNCTION__, __LINE__, format);
                break;
            }
        }
        break;

    case GL_UNSIGNED_SHORT_5_6_5:
    case GL_UNSIGNED_SHORT_4_4_4_4:
    case GL_UNSIGNED_SHORT_5_5_5_1:
        pixelSize = 2;
        break;

    case GL_UNSIGNED_INT_24_8_OES:
        pixelSize = 4;
        break;

    case GL_FLOAT:
        {
            switch (format)
            {
            case GL_RGBA:
                pixelSize = 16;
                break;

            case GL_RGB:
                pixelSize = 12;
                break;

            case GL_R32F:
                pixelSize = 4;
                break;

            case GL_RG32F:
            case GL_RG:
                pixelSize = 8;
                break;

            case GL_RGBA32F:
                pixelSize = 16;
                break;

            case GL_RED:
                pixelSize = 4;
                break;

            default:
                Abort("%s(%d): unknown format(0x%04X)\n", __FUNCTION__, __LINE__, format);
                break;
            }
        }
        break;

    case GL_HALF_FLOAT_OES:
        {
            switch (format)
            {
            case GL_RGBA:
                pixelSize = 8;
                break;

            case GL_RGB:
                pixelSize = 6;
                break;

            default:
                Abort("%s(%d): unknown format(0x%04X)\n", __FUNCTION__, __LINE__, format);
                break;
            }
        }
        break;

    case GL_UNSIGNED_INT_2_10_10_10_REV:
        pixelSize = 4;
        break;

    case GL_UNSIGNED_SHORT:
        {
            switch (format)
            {
            case GL_DEPTH_COMPONENT:
                pixelSize = 2;
                break;

            default:
                Abort("%s(%d): unknown format(0x%04X)\n", __FUNCTION__, __LINE__, format);
                break;
            }
        }
        break;

    case GL_HALF_FLOAT:
        switch (format)
        {
        case GL_R16F:
        case GL_RED:
            pixelSize = 2;
            break;

        case GL_RG16F:
        case GL_RG:
            pixelSize = 4;
            break;

        case GL_RGBA16F:
        case GL_RGBA:
            pixelSize = 8;
            break;

        default:
            Abort("%s(%d): unknown format(0x%04X)\n", __FUNCTION__, __LINE__, format);
            break;
        }
        break;

    case GL_INT:
        switch (format)
        {
        case GL_RGBA32I:
        case GL_RGBA_INTEGER:
            pixelSize = 16;
            break;

        case GL_RG32I:
        case GL_RG_INTEGER:
            pixelSize = 8;
            break;

        case GL_R32I:
        case GL_RED_INTEGER:
            pixelSize = 4;
            break;

        default:
            Abort("%s(%d): unknown format(0x%04X)\n", __FUNCTION__, __LINE__, format);
            break;
        }
        break;

    default:
        Abort("%s(%d): unknown type(0x%04X)\n", __FUNCTION__, __LINE__, type);
        break;
    }

    return pixelSize;
}

GLuint GetTexImageSize(GLenum format, GLenum type, GLsizei width, GLsizei height, GLsizei depth)
{
    GLuint  pixelSize   = GetPixelSizeByTexFormat(format, type);
    GLuint  imageSize   = pixelSize*width*height*depth;

    return imageSize;
}