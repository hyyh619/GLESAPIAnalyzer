#include "utils.h"
#include "bitmap.h"
#include "MemoryPool.h"
#include "TranslateGL.h"
#include "GLESShare.h"
#include "GLES3Tex.h"
#include "ApiGLES3Context.h"
#include "Analyzer.h"

GLchar* FakeBitmap(gceSURF_FORMAT   format,
                   GLvoid           *data,
                   GLuint           x,
                   GLuint           y,
                   GLuint           z,
                   GLuint           width,
                   GLuint           height,
                   GLuint           depth,
                   GLuint           texWidth,
                   GLuint           texHeight,
                   GLuint           texDepth,
                   GLuint           bpp)
{
    GLchar  *newData    = NULL;
    GLint   size        = (width)*(height)*(texDepth)*bpp/8;
    GLint   fakeSize    = (width)*(height)*(texDepth)*4;
    GLint   typeSize    = bpp/8;
    GLuint  realWidth   = x+width;
    GLuint  realHeight  = y+height;
    GLuint  realDepth   = z+depth;
    GLchar  *pDst;
    GLchar  *pSrc;

    if (format == gcvSURF_A8R8G8B8 ||
        format == gcvSURF_A8B8G8R8 ||
        format == gcvSURF_R5G6B5 ||
        format == gcvSURF_B8G8R8 ||
        format == gcvSURF_R8G8B8)
    {
        newData = (GLchar*)ApiAllocateTexMemory(size);
        if (newData == NULL)
        {
            printf("%s(%d): cannot allocate new memory for fake\n", __FUNCTION__, __LINE__);
            return NULL;
        }

        pSrc = (GLchar*)data;
        pDst = newData;
        for (GLuint k=0; k<texDepth; k++)
        {
            for (GLuint i=0; i<texHeight; i++)
            {
                for (GLuint j=0; j<texWidth; j++)
                {
                    if (CheckPixelInRect(j, i, k, x, y, z, realWidth, realHeight, realDepth))
                    {
                        for (GLint k=0; k<typeSize; k++)
                        {
                            *pDst++ = *pSrc++;
                        }
                    }
                    else
                    {
                        pSrc += typeSize;
                    }
                }
            }
        }
    }
    else if (format == gcvSURF_A8 ||
             format == gcvSURF_A8L8 ||
             format == gcvSURF_A4B4G4R4 ||
             format == gcvSURF_A4R4G4B4 ||
             format == gcvSURF_R4G4B4A4 ||
             format == gcvSURF_B4G4R4A4)
    {
        newData = (GLchar*)ApiAllocateTexMemory(fakeSize);
        if (newData == NULL)
        {
            printf("%s(%d): cannot allocate new memory for fake\n", __FUNCTION__, __LINE__);
            return NULL;
        }
    }
    else
    {
        newData = (GLchar*)ApiAllocateTexMemory(fakeSize);
        if (newData == NULL)
        {
            printf("%s(%d): cannot allocate new memory for fake\n", __FUNCTION__, __LINE__);
            return NULL;
        }
    }

    pSrc = (GLchar*)data;
    pDst = (GLchar*)newData;

    if (format == gcvSURF_A8)
    {
        for (GLuint k=0; k<texDepth; k++)
        {
            for (GLuint i=0; i<texHeight; i++)
            {
                for (GLuint j=0; j<texWidth; j++)
                {
                    if (CheckPixelInRect(j, i, k, x, y, z, realWidth, realHeight, realDepth))
                    {
                        *pDst++ = *pSrc;
                        *pDst++ = *pSrc;
                        *pDst++ = *pSrc;
                        *pDst++ = *pSrc;
                        pSrc ++;
                    }
                    else
                    {
                        pSrc ++;
                    }
                }
            }
        }
    }

    if (format == gcvSURF_A8L8)
    {
        for (GLuint k=0; k<texDepth; k++)
        {
            for (GLuint i=0; i<texHeight; i++)
            {
                for (GLuint j=0; j<texWidth; j++)
                {
                    if (CheckPixelInRect(j, i, k, x, y, z, realWidth, realHeight, realDepth))
                    {
                        *pDst++ = *pSrc++;
                        *pDst++ = *pSrc;
                        *pDst++ = *pSrc;
                        *pDst++ = *pSrc++;
                    }
                    else
                    {
                        pSrc += 2;
                    }
                }
            }
        }
    }

    if (format == gcvSURF_A4B4G4R4 ||
        format == gcvSURF_A4R4G4B4 ||
        format == gcvSURF_R4G4B4A4 ||
        format == gcvSURF_B4G4R4A4)
    {
        for (GLuint k=0; k<texDepth; k++)
        {
            for (GLuint i=0; i<texHeight; i++)
            {
                for (GLuint j=0; j<texWidth; j++)
                {
                    if (CheckPixelInRect(j, i, k, x, y, z, realWidth, realHeight, realDepth))
                    {
                        //*pDst++ = (*pSrc & 0xF) << 4;
                        //*pDst++ = ((*pSrc & 0xF0) >> 4) << 4;
                        //pSrc ++;
                        //*pDst++ = (*pSrc & 0xF) << 4;
                        //*pDst++ = ((*pSrc & 0xF0) >> 4) << 4;
                        //pSrc ++;
                        pDst[0] = ((pSrc[0] & 0x0F)) << 4;  // R->2
                        pDst[1] = ((pSrc[0] & 0xF0));       // G->1
                        pDst[2] = ((pSrc[1] & 0x0F)) << 4;  // B->0
                        pDst[3] = ((pSrc[1] & 0xF0));       // A->3
                        pDst += 4;
                        pSrc += 2;
                    }
                    else
                    {
                        pSrc += 2;
                    }
                }
            }
        }
    }

    if (format == gcvSURF_R32F)
    {
        for (GLuint k=0; k<texDepth; k++)
        {
            for (GLuint i=0; i<texHeight; i++)
            {
                for (GLuint j=0; j<texWidth; j++)
                {
                    if (CheckPixelInRect(j, i, k, x, y, z, realWidth, realHeight, realDepth))
                    {
                        GLfloat s = *(GLfloat*)pSrc;
                        GLuint  i1 = (GLuint)s;
                        GLchar  c1 = (GLchar)(((GLfloat)i1/(GLfloat)0xFFFFFFFF)*255);
                        *pDst = c1;
                    }

                    pSrc += 4;
                    pDst += 4;
                }
            }
        }
    }

    return newData;
}

CTexObj::CTexObj()
{
    texIndex        = 0;
    target          = 0;
    minFilter       = GL_NEAREST_MIPMAP_LINEAR;
    magFilter       = GL_LINEAR;
    anisoFilter     = 0;
    wrapS           = GL_REPEAT;
    wrapT           = GL_REPEAT;
    wrapR           = GL_REPEAT;
    baseLevel       = 0;
    maxLevel        = 1000;
    minLod          = -1000;
    maxLod          = 1000;
    genMipmap       = GL_FALSE;
    texCmpMode      = GL_NONE;
    texCmpFunc      = GL_LEQUAL;
    swizzleR        = GL_TEXTURE_SWIZZLE_R;
    swizzleG        = GL_TEXTURE_SWIZZLE_G;
    swizzleB        = GL_TEXTURE_SWIZZLE_B;
    swizzleA        = GL_TEXTURE_SWIZZLE_A;

    pTex2D          = NULL;
    pTex3D          = NULL;
    pTexExternal    = NULL;

    for (GLuint i=0; i<6; i++)
    {
        pTexCube[i] = NULL;
    }
}

CTexObj::~CTexObj()
{
    stTexImage *pTmp = NULL;
    stTexImage *pDel = NULL;

    if (target == GL_TEXTURE_2D)
    {
        pTmp = pTex2D;

        while (pTmp)
        {
            pDel = pTmp;
            pTmp = pTmp->next;

            delete pDel;
        }
    }
    else if (target == GL_TEXTURE_CUBE_MAP)
    {
        for (GLuint i=0; i<6; i++)
        {
            pTmp = pTexCube[i];

            while (pTmp)
            {
                pDel = pTmp;
                pTmp = pTmp->next;

                delete pDel;
            }
        }
    }
    else if (target == GL_TEXTURE_3D)
    {
    }
}

stTexImage*  CTexObj::GetTexImageByLevel(GLenum target, GLint level)
{
    stTexImage *pImg = NULL;

    switch (target)
    {
    case GL_TEXTURE_2D:
        pImg = pTex2D;
        break;

    case GL_TEXTURE_3D:
        pImg = pTex3D;
        break;

    case GL_TEXTURE_EXTERNAL_OES:
        pImg = pTexExternal;
        break;

    case GL_TEXTURE_CUBE_MAP_POSITIVE_X:
    case GL_TEXTURE_CUBE_MAP_NEGATIVE_X:
    case GL_TEXTURE_CUBE_MAP_POSITIVE_Y:
    case GL_TEXTURE_CUBE_MAP_NEGATIVE_Y:
    case GL_TEXTURE_CUBE_MAP_POSITIVE_Z:
    case GL_TEXTURE_CUBE_MAP_NEGATIVE_Z:
        pImg = pTexCube[target - GL_TEXTURE_CUBE_MAP_POSITIVE_X];
        break;

    case GL_TEXTURE_CUBE_MAP:
        pImg = pTexCube[0];
        break;

    default:
        Abort("%s(%d): invalid texture target(0x%04X)\n", __FUNCTION__, __LINE__, target);
        break;
    }

    while (pImg)
    {
        if (pImg->level == level)
            return pImg;

        pImg = pImg->next;
    }

    return NULL;
}

stTexImage*  CTexObj::GetTexImageByLevel(GLint level)
{
    return GetTexImageByLevel(target, level);
}

GLenum CTexObj::GetTexFormat()
{
    GLenum format = 0;

    switch (target)
    {
    case GL_TEXTURE_2D:
        format = pTex2D->internalformat;
        break;

    case GL_TEXTURE_3D:
        format = pTex3D->internalformat;
        break;

    case GL_TEXTURE_CUBE_MAP:
        format = pTexCube[0]->internalformat;
        break;

    case GL_TEXTURE_EXTERNAL_OES:
        format = pTexExternal->internalformat;
        break;

    default:
        Abort("Unknown format\n");
        break;
    }

    return format;
}

GLuint CTexObj::GetImageCount(GLenum target)
{
    GLuint      count = 0;
    stTexImage  *pImg = NULL;

    switch (target)
    {
    case GL_TEXTURE_2D:
        pImg = pTex2D;
        while (pImg)
        {
            count ++;
            pImg = pImg->next;
        }
        break;

    case GL_TEXTURE_CUBE_MAP_POSITIVE_X:
    case GL_TEXTURE_CUBE_MAP_NEGATIVE_X:
    case GL_TEXTURE_CUBE_MAP_POSITIVE_Y:
    case GL_TEXTURE_CUBE_MAP_NEGATIVE_Y:
    case GL_TEXTURE_CUBE_MAP_POSITIVE_Z:
    case GL_TEXTURE_CUBE_MAP_NEGATIVE_Z:
        pImg = pTexCube[target-GL_TEXTURE_CUBE_MAP_POSITIVE_X];
        while (pImg)
        {
            count ++;
            pImg = pImg->next;
        }
        break;

    case GL_TEXTURE_3D:
        pImg = pTex3D;
        while (pImg)
        {
            count ++;
            pImg = pImg->next;
        }
        break;

    default:
        break;
    }

    return count;
}

GLsizei CTexObj::GetWidth(GLint level)
{
    stTexImage  *p = GetTexImageByLevel(level);
    return p?p->width:0;
}

GLsizei CTexObj::GetHeight(GLint level)
{
    stTexImage  *p = GetTexImageByLevel(level);
    return p?p->height:0;
}

GLvoid CTexObj::TexParameteri(GLenum target, GLenum pname, GLint value)
{
    switch (pname)
    {
    case GL_TEXTURE_MIN_FILTER:
        minFilter = value;
        break;

    case GL_TEXTURE_MAG_FILTER:
        magFilter = value;
        break;

    case GL_TEXTURE_WRAP_T:
        wrapT = value;
        break;

    case GL_TEXTURE_WRAP_S:
        wrapS = value;
        break;

    case GL_TEXTURE_WRAP_R:
        wrapR = value;
        break;

    case GL_TEXTURE_MAX_LEVEL:
        maxLevel = value;
        break;

    case GL_TEXTURE_MAX_ANISOTROPY_EXT:
        anisoFilter = value;
        break;

    case GL_TEXTURE_COMPARE_MODE:
        texCmpMode = value;
        break;

    case GL_TEXTURE_COMPARE_FUNC:
        texCmpFunc = value;
        break;

    default:
        Abort("Invalid");
        break;
    }
}

GLvoid GetInternalFormatBppFromTextureFormat(GLenum          Format,
                                             GLenum          Type,
                                             gceSURF_FORMAT  *ImageFormat,
                                             GLuint          *Bpp,
                                             GLuint          *fakeBpp)
{
    GLsizei         bpp         = 0;
    gceSURF_FORMAT  imageFormat = gcvSURF_UNKNOWN;

    switch (Format)
    {
    case GL_DEPTH_COMPONENT:
        switch (Type)
        {
        case GL_UNSIGNED_SHORT:
            bpp = 16;
            imageFormat = gcvSURF_D16;
            break;

        case GL_DEPTH_COMPONENT24_OES:
            /* Some applications use DEPTH_COMPONENT24_OES,
               even though it's not part of the spec. */
            bpp = 32;
            imageFormat = gcvSURF_D24X8;
            break;

        case GL_DEPTH_COMPONENT32_OES:
            /* Fall through */
        case GL_UNSIGNED_INT:
            bpp = 32;
            imageFormat = gcvSURF_D32;
            break;
        }
        break;

    case GL_ALPHA:
        switch (Type)
        {
        case GL_UNSIGNED_BYTE:
            bpp = 8;
            imageFormat = gcvSURF_A8;
            break;

        case GL_UNSIGNED_SHORT:
            bpp = 16;
            imageFormat = gcvSURF_A16;
            break;

        case GL_UNSIGNED_INT:
            bpp = 32;
            imageFormat = gcvSURF_A32;
            break;

        case GL_HALF_FLOAT_OES:
            bpp = 16;
            imageFormat = gcvSURF_A16F;
            break;

        case GL_FLOAT:
            bpp = 32;
            imageFormat = gcvSURF_A32F;
            break;
        }
        break;

    case GL_R32F:
        bpp = 32;
        imageFormat = gcvSURF_R32F;
        break;

    case GL_RGB:
        switch (Type)
        {
        case GL_UNSIGNED_SHORT_4_4_4_4:
            bpp = 16;
            imageFormat = gcvSURF_X4R4G4B4;
            break;

        case GL_UNSIGNED_SHORT_5_5_5_1:
            bpp = 16;
            imageFormat = gcvSURF_X1R5G5B5;
            break;

        case GL_UNSIGNED_SHORT_5_6_5:
            bpp = 16;
            imageFormat = gcvSURF_R5G6B5;
            break;

        case GL_UNSIGNED_BYTE:
            bpp = 24;
            imageFormat = gcvSURF_B8G8R8;
            break;

        case GL_UNSIGNED_INT_2_10_10_10_REV_EXT:
            bpp = 32;
            imageFormat = gcvSURF_X2B10G10R10;
            break;

        case GL_HALF_FLOAT_OES:
            bpp = 64;
            imageFormat = gcvSURF_X16B16G16R16F;
            break;
        }
        break;

    case GL_RGB565:
        bpp = 16;
        imageFormat = gcvSURF_R5G6B5;
        break;

    case GL_RGBA:
        switch (Type)
        {
        case GL_UNSIGNED_BYTE:
            bpp = 32;
            imageFormat = gcvSURF_A8B8G8R8;
            break;

        case GL_UNSIGNED_SHORT_4_4_4_4:
            bpp = 16;
            imageFormat = gcvSURF_A4B4G4R4;
            break;

        case GL_UNSIGNED_SHORT_5_5_5_1:
            bpp = 16;
            imageFormat = gcvSURF_R5G5B5A1;
            break;

        case GL_UNSIGNED_INT_2_10_10_10_REV_EXT:
            bpp = 32;
            imageFormat = gcvSURF_A2B10G10R10;
            break;

        case GL_HALF_FLOAT_OES:
            bpp = 64;
            imageFormat = gcvSURF_A16B16G16R16F;
            break;
        }
        break;

    case GL_RGBA4:
        bpp = 16;
        imageFormat = gcvSURF_A4B4G4R4;
        break;

    case GL_RGB5_A1:
        bpp = 16;
        imageFormat = gcvSURF_R5G5B5A1;
        break;

    case GL_LUMINANCE:
        switch (Type)
        {
        case GL_UNSIGNED_BYTE:
            bpp = 8;
            imageFormat = gcvSURF_L8;
            break;
        case GL_UNSIGNED_SHORT:
            bpp = 16;
            imageFormat = gcvSURF_L16;
            break;

        case GL_UNSIGNED_INT:
            bpp = 32;
            imageFormat = gcvSURF_L32;
            break;

        case GL_HALF_FLOAT_OES:
            bpp = 16;
            imageFormat = gcvSURF_L16F;
            break;

        case GL_FLOAT:
            bpp = 32;
            imageFormat = gcvSURF_L32F;
            break;
        }
        break;

    case GL_LUMINANCE_ALPHA:
        switch (Type)
        {
        case GL_UNSIGNED_BYTE:
            bpp = 16;
            imageFormat = gcvSURF_A8L8;
            break;

        case GL_UNSIGNED_SHORT:
            bpp = 32;
            imageFormat = gcvSURF_A16L16;
            break;

        case GL_HALF_FLOAT_OES:
            bpp = 32;
            imageFormat = gcvSURF_A16L16F;
            break;

        case GL_FLOAT:
            bpp = 64;
            imageFormat = gcvSURF_A32L32F;
            break;
        }
        break;

    case GL_BGRA_EXT:
        switch (Type)
        {
        case GL_UNSIGNED_BYTE:
            bpp = 32;
            imageFormat = gcvSURF_A8R8G8B8;
            break;
        }
        break;

    case GL_DEPTH_STENCIL_OES:
        switch (Type)
        {
        case GL_UNSIGNED_INT_24_8_OES:
            bpp = 32;
            imageFormat = gcvSURF_D24S8;
            break;
        }
        break;

    case GL_R16F_EXT:
        switch (Type)
        {
        case GL_FLOAT:
            bpp = 16;
            imageFormat = gcvSURF_R16F;
            break;
        }
        break;

    default:
        break;
    }

    if (Bpp)
    {
        *Bpp = bpp;
    }

    if (fakeBpp)
    {
        *fakeBpp = 32;
    }

    if (ImageFormat)
    {
        *ImageFormat = imageFormat;
    }

    /* Success. */
    return ;
}

GLboolean CheckPixelInRect(GLuint posX, GLuint posY, GLuint posZ, GLuint left, GLuint top, GLuint zNear, GLuint right, GLuint bottom, GLuint zFar)
{
    if (posX >= left &&
        posY >= top &&
        posZ >= zNear &&
        posX <= right &&
        posY <= bottom &&
        posZ <= zFar)
    {
        return GL_TRUE;
    }
    else
    {
        return GL_FALSE;
    }
}

static gceSTATUS _gl2gcCompressedFormat(GLenum Format, gceSURF_FORMAT* TextureFormat)
{
    gceSURF_FORMAT  imageFormat = gcvSURF_UNKNOWN;

    switch (Format)
    {
    case GL_PALETTE4_RGBA4_OES:
    case GL_PALETTE8_RGBA4_OES:
        imageFormat = gcvSURF_A4R4G4B4;
        break;

    case GL_PALETTE4_RGB5_A1_OES:
    case GL_PALETTE8_RGB5_A1_OES:
        imageFormat = gcvSURF_A1R5G5B5;
        break;

    case GL_PALETTE4_R5_G6_B5_OES:
    case GL_PALETTE8_R5_G6_B5_OES:
        imageFormat = gcvSURF_R5G6B5;
        break;

    case GL_PALETTE4_RGB8_OES:
    case GL_PALETTE8_RGB8_OES:
        imageFormat = gcvSURF_X8R8G8B8;
        break;

    case GL_PALETTE4_RGBA8_OES:
    case GL_PALETTE8_RGBA8_OES:
        imageFormat = gcvSURF_A8R8G8B8;
        break;

    case GL_ETC1_RGB8_OES:
        imageFormat = gcvSURF_ETC1;
        break;

    case GL_COMPRESSED_RGB_S3TC_DXT1_EXT:
    case GL_COMPRESSED_RGBA_S3TC_DXT1_EXT:
        imageFormat = gcvSURF_DXT1;
        break;

    case GL_COMPRESSED_RGBA_S3TC_DXT3_EXT:
        imageFormat = gcvSURF_DXT3;
        break;

    case GL_COMPRESSED_RGBA_S3TC_DXT5_EXT:
        imageFormat = gcvSURF_DXT5;
        break;

    case GL_COMPRESSED_RGB_PVRTC_2BPPV1_IMG:
    case GL_COMPRESSED_RGBA_PVRTC_2BPPV1_IMG:
    case GL_COMPRESSED_RGB_PVRTC_4BPPV1_IMG:
    case GL_COMPRESSED_RGBA_PVRTC_4BPPV1_IMG:
        imageFormat = gcvSURF_A8R8G8B8;
        break;

    default:
        printf("%s(%d): unknown format(0x%X)\n", __FUNCTION__, __LINE__, Format);
        return gcvSTATUS_INVALID_ARGUMENT;
    }

    *TextureFormat = imageFormat;
    return gcvSTATUS_OK;
}

static void _DecodeETC1Block(GLubyte        *Output,
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

            r = base[block][0] + delta; r = gcmMAX(0x00, gcmMIN(r, 0xFF));
            g = base[block][1] + delta; g = gcmMAX(0x00, gcmMIN(g, 0xFF));
            b = base[block][2] + delta; b = gcmMAX(0x00, gcmMIN(b, 0xFF));

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

static void* _DecompressETC1(IN GLsizei         Width,
                             IN GLsizei         Height,
                             IN GLsizei         ImageSize,
                             IN const void      *Data,
                             OUT gceSURF_FORMAT *Format)
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
            _DecodeETC1Block(p,
                             stride,
                             gcmMIN(Width - x, 4),
                             gcmMIN(Height - y, 4),
                             data);

            p         += 4 * 3;
            data      += 8;
            ImageSize -= 8;
        }
    }

    *Format = gcvSURF_B8G8R8;

    return pixels;
}

#define glmRED(c)   ( (c) >> 11 )
#define glmGREEN(c) ( ((c) >> 5) & 0x3F )
#define glmBLUE(c)  ( (c) & 0x1F )

/* Decode 64-bits of color information. */
static void
_DecodeDXTColor16(
    IN GLsizei Width,
    IN GLsizei Height,
    IN GLsizei Stride,
    IN const GLubyte * Data,
    OUT GLubyte * Output
    )
{
    GLushort c0, c1;
    GLushort color[4];
    GLushort r, g, b;
    GLint x, y;

    /* Decode color 0. */
    c0 = *(GLushort*)Data;
    color[0] = 0x8000 | (c0 & 0x001F) | ((c0 & 0xFFC0) >> 1);

    /* Decode color 1. */
    c1 = *(GLushort*)(Data + 2);
    color[1] = 0x8000 | (c1 & 0x001F) | ((c1 & 0xFFC0) >> 1);

    if (c0 > c1)
    {
        /* Compute color 2: (c0 * 2 + c1) / 3. */
        r = (2 * glmRED  (c0) + glmRED  (c1)) / 3;
        g = (2 * glmGREEN(c0) + glmGREEN(c1)) / 3;
        b = (2 * glmBLUE (c0) + glmBLUE (c1)) / 3;
        color[2] = 0x8000 | (r << 10) | ((g & 0x3E) << 4) | b;


        /* Compute color 3: (c0 + 2 * c1) / 3. */
        r = (glmRED  (c0) + 2 * glmRED  (c1)) / 3;
        g = (glmGREEN(c0) + 2 * glmGREEN(c1)) / 3;
        b = (glmBLUE (c0) + 2 * glmBLUE (c1)) / 3;
        color[3] = 0x8000 | (r << 10) | ((g & 0x3E) << 4) | b;

    }
    else
    {
        /* Compute color 2: (c0 + c1) / 2. */
        r = (glmRED  (c0) + glmRED  (c1)) / 2;
        g = (glmGREEN(c0) + glmGREEN(c1)) / 2;
        b = (glmBLUE (c0) + glmBLUE (c1)) / 2;
        color[2] = 0x8000 | (r << 10) | ((g & 0x3E) << 4) | b;

        /* Color 3 is opaque black. */
        color[3] = 0;

    }
    /* Walk all lines. */
    for (y = 0; y < Height; y++)
    {
        /* Get lookup for line. */
        GLubyte bits = Data[4 + y];

        /* Walk all pixels. */
        for (x = 0; x < Width; x++, bits >>= 2, Output += 2)
        {
            /* Copy the color. */
            *(GLshort *) Output = color[bits & 3];
        }

        /* Next line. */
        Output += Stride - Width * 2;
    }
}

#define glmEXPAND_RED(c)   ( (((c) & 0xF800) << 8) | (((c) & 0xE000) << 3) )
#define glmEXPAND_GREEN(c) ( (((c) & 0x07E0) << 5) | (((c) & 0x0600) >> 1) )
#define glmEXPAND_BLUE(c)  ( (((c) & 0x001F) << 3) | (((c) & 0x001C) >> 2) )

/* Decode 64-bits of color information. */
static void
_DecodeDXTColor32(
    IN GLsizei Width,
    IN GLsizei Height,
    IN GLsizei Stride,
    IN const GLubyte * Data,
    IN const GLubyte *Alpha,
    OUT GLubyte * Output
    )
{
    GLuint color[4];
    GLushort c0, c1;
    GLint x, y;
    GLuint r, g, b;

    /* Decode color 0. */
    c0       = Data[0] | (Data[1] << 8);
    color[0] = glmEXPAND_RED(c0) | glmEXPAND_GREEN(c0) | glmEXPAND_BLUE(c0);

    /* Decode color 1. */
    c1       = Data[2] | (Data[3] << 8);
    color[1] = glmEXPAND_RED(c1) | glmEXPAND_GREEN(c1) | glmEXPAND_BLUE(c1);

    /* Compute color 2: (c0 * 2 + c1) / 3. */
    r = (2 * (color[0] & 0xFF0000) + (color[1] & 0xFF0000)) / 3;
    g = (2 * (color[0] & 0x00FF00) + (color[1] & 0x00FF00)) / 3;
    b = (2 * (color[0] & 0x0000FF) + (color[1] & 0x0000FF)) / 3;
    color[2] = (r & 0xFF0000) | (g & 0x00FF00) | (b & 0x0000FF);

    /* Compute color 3: (c0 + 2 * c1) / 3. */
    r = ((color[0] & 0xFF0000) + 2 * (color[1] & 0xFF0000)) / 3;
    g = ((color[0] & 0x00FF00) + 2 * (color[1] & 0x00FF00)) / 3;
    b = ((color[0] & 0x0000FF) + 2 * (color[1] & 0x0000FF)) / 3;
    color[3] = (r & 0xFF0000) | (g & 0x00FF00) | (b & 0x0000FF);

    /* Walk all lines. */
    for (y = 0; y < Height; y++)
    {
        /* Get lookup for line. */
        GLubyte bits = Data[4 + y];
        GLint   a    = y << 2;

        /* Walk all pixels. */
        for (x = 0; x < Width; x++, bits >>= 2, Output += 4)
        {
            /* Copmbine the lookup color with the alpha value. */
            GLuint c = color[bits & 3] | (Alpha[a++] << 24);
            *(GLuint *) Output = c;
        }

        /* Next line. */
        Output += Stride - Width * 4;
    }
}

/* Decode 64-bits of alpha information. */
static void
_DecodeDXT3Alpha(
    IN const GLubyte * Data,
    OUT GLubyte * Output
    )
{
    GLint i;
    GLubyte a;

    /* Walk all alpha pixels. */
    for (i = 0; i < 8; i++, Data++)
    {
        /* Get even alpha and expand into 8-bit. */
        a = *Data & 0x0F;
        *Output++ = a | (a << 4);

        /* Get odd alpha and expand into 8-bit. */
        a = *Data >> 4;
        *Output++ = a | (a << 4);
    }
}

/* Decode 64-bits of alpha information. */
static void
_DecodeDXT5Alpha(
    IN const GLubyte * Data,
    OUT GLubyte * Output
    )
{
    GLint i, j, n;
    GLubyte a[8];
    GLushort bits = 0;

    /* Load alphas 0 and 1. */
    a[0] = Data[0];
    a[1] = Data[1];

    if (a[0] > a[1])
    {
        /* Interpolate alphas 2 through 7. */
        a[2] = (GLubyte) ((6 * a[0] + 1 * a[1]) / 7);
        a[3] = (GLubyte) ((5 * a[0] + 2 * a[1]) / 7);
        a[4] = (GLubyte) ((4 * a[0] + 3 * a[1]) / 7);
        a[5] = (GLubyte) ((3 * a[0] + 4 * a[1]) / 7);
        a[6] = (GLubyte) ((2 * a[0] + 5 * a[1]) / 7);
        a[7] = (GLubyte) ((1 * a[0] + 6 * a[1]) / 7);
    }
    else
    {
        /* Interpolate alphas 2 through 5. */
        a[2] = (GLubyte) ((4 * a[0] + 1 * a[1]) / 5);
        a[3] = (GLubyte) ((3 * a[0] + 2 * a[1]) / 5);
        a[4] = (GLubyte) ((2 * a[0] + 3 * a[1]) / 5);
        a[5] = (GLubyte) ((1 * a[0] + 4 * a[1]) / 5);

        /* Set alphas 6 and 7. */
        a[6] = 0;
        a[7] = 255;
    }

    /* Walk all pixels. */
    for (i = 0, j = 2, n = 0; i < 16; i++, bits >>= 3, n -= 3)
    {
        /* Test if we have enough bits in the accumulator. */
        if (n < 3)
        {
            /* Load another chunk of bits in the accumulator. */
            bits |= Data[j++] << n;
            n += 8;
        }

        /* Copy decoded alpha value. */
        Output[i] = a[bits & 0x7];
    }
}

/* Decompress a DXT texture. */
static void * _DecompressDXT(IN GLsizei Width,
                             IN GLsizei Height,
                             IN GLsizei ImageSize,
                             IN const void * Data,
                             IN GLenum InternalFormat,
                             IN gceSURF_FORMAT Format,
                             OUT gceSURF_FORMAT *imageFormat)
{
    GLubyte         *pixels = NULL;
    GLubyte         *line   = NULL;
    const GLubyte   *data;
    GLsizei         x;
    GLsizei         y;
    GLsizei         stride;
    GLubyte         alpha[16];
    GLint           bpp;

    /* Determine bytes per pixel. */
    bpp = ((Format == gcvSURF_A1R5G5B5) || (Format == gcvSURF_R5G6B5)) ? 2 : 4;

    /* Allocate the decompressed memory. */
    pixels = (GLubyte*)malloc(Width * Height * bpp);
    if (pixels == NULL)
    {
        printf("%s(%d): allocate memory failed\n", __FUNCTION__, __LINE__);
        return NULL;
    }

    stride = Width * bpp;
    data   = (const GLubyte*)Data;

    /* Walk all lines, 4 lines per block. */
    for (y = 0, line = pixels; y < Height; y += 4, line += stride * 4)
    {
        GLubyte * p = line;

        /* Walk all pixels, 4 pixels per block. */
        for (x = 0; x < Width; x += 4)
        {
            /* Dispatch on format. */
            switch (InternalFormat)
            {
            default:
                /* Decompress one color block. */
                _DecodeDXTColor16(gcmMIN(Width - x, 4),
                                  gcmMIN(Height - y, 4),
                                  stride,
                                  data,
                                  p);

                /* 8 bytes per block. */
                data      += 8;
                ImageSize -= 8;
                break;

            case GL_COMPRESSED_RGBA_S3TC_DXT3_EXT:
                /* Decode DXT3 alpha. */
                _DecodeDXT3Alpha(data, alpha);

                /* Decompress one color block. */
                _DecodeDXTColor32(gcmMIN(Width - x, 4),
                                  gcmMIN(Height - y, 4),
                                  stride,
                                  data + 8,
                                  alpha,
                                  p);

                /* 16 bytes per block. */
                data      += 16;
                ImageSize -= 16;
                break;

            case GL_COMPRESSED_RGBA_S3TC_DXT5_EXT:
                /* Decode DXT5 alpha. */
                _DecodeDXT5Alpha(data, alpha);

                /* Decompress one color block. */
                _DecodeDXTColor32(gcmMIN(Width - x, 4),
                                  gcmMIN(Height - y, 4),
                                  stride,
                                  data + 8,
                                  alpha,
                                  p);

                /* 16 bytes per block. */
                data      += 16;
                ImageSize -= 16;
                break;
            }

            /* Next block. */
            p += 4 * bpp;
        }
    }

    if (bpp == 2)
    {
        *imageFormat = Format;
    }
    else if (bpp == 4)
    {
        *imageFormat = gcvSURF_A8R8G8B8;
    }

    /* Return pointer to decompressed data. */
    return pixels;
}

GLvoid DecompressTextureImage(GLint width,
                              GLint height,
                              GLenum internalformat,
                              GLsizei imageSize,
                              GLvoid *data,
                              gceSURF_FORMAT *Format,
                              GLvoid **decompressedImage)
{
    gceSURF_FORMAT  imageFormat = gcvSURF_UNKNOWN;
    gceSURF_FORMAT  format      = gcvSURF_UNKNOWN;
    GLubyte         *pixels     = NULL;

    _gl2gcCompressedFormat(internalformat, &format);

    switch (internalformat)
    {
    case GL_ETC1_RGB8_OES:
        /* Decompress ETC1 texture */
        pixels = (GLubyte*)_DecompressETC1(width,
                                           height,
                                           imageSize,
                                           data,
                                           &imageFormat);

        if (pixels == NULL)
        {
            /* Could not decompress the ETC1 texture. */
            printf("%s(%d): cannot decompress ETC1 texture.\n", __FUNCTION__, __LINE__);
            return;
        }
        break;

    case GL_COMPRESSED_RGB_S3TC_DXT1_EXT:
    case GL_COMPRESSED_RGBA_S3TC_DXT1_EXT:
    case GL_COMPRESSED_RGBA_S3TC_DXT3_EXT:
    case GL_COMPRESSED_RGBA_S3TC_DXT5_EXT:
        if ((format < gcvSURF_DXT1) || (format > gcvSURF_DXT5))
        {
            /* Decompress DXT texture. */
            pixels = (GLubyte*)_DecompressDXT(width, height,
                                              imageSize, data,
                                              internalformat,
                                              imageFormat = format,
                                              &imageFormat);

            if (pixels == NULL)
            {
                /* Could not decompress the DXT texture. */
                printf("%s(%d): cannot decompress DXT texture.\n", __FUNCTION__, __LINE__);
                return;
            }
        }
        break;

    /* Test for PVR compressed textures. */
    case GL_COMPRESSED_RGB_PVRTC_2BPPV1_IMG:
    case GL_COMPRESSED_RGBA_PVRTC_2BPPV1_IMG:
    case GL_COMPRESSED_RGB_PVRTC_4BPPV1_IMG:
    case GL_COMPRESSED_RGBA_PVRTC_4BPPV1_IMG:
        printf("%s(%d): cannot support GL_COMPRESSED_XXXX\n", __FUNCTION__, __LINE__);
        break;

    case GL_PALETTE4_RGBA4_OES:
    case GL_PALETTE4_RGB5_A1_OES:
    case GL_PALETTE4_R5_G6_B5_OES:
    case GL_PALETTE4_RGB8_OES:
    case GL_PALETTE4_RGBA8_OES:
    case GL_PALETTE8_RGBA4_OES:
    case GL_PALETTE8_RGB5_A1_OES:
    case GL_PALETTE8_R5_G6_B5_OES:
    case GL_PALETTE8_RGB8_OES:
    case GL_PALETTE8_RGBA8_OES:
        printf("%s(%d): cannot support GL_PALETTE4_XXXX\n", __FUNCTION__, __LINE__);
        break;

    default:
        printf("%s(%d): cannot support unknown format(0x%X)\n", __FUNCTION__, __LINE__, internalformat);
        break;
    }

    *decompressedImage  = pixels;
    *Format             = imageFormat;
}

CTexObj* CGLES3Context::CreateTextureObject(GLuint texId)
{
    TextureMap::iterator it = textureMap.find(texId);
    CTexObj *pTex = NULL;

    if (it != textureMap.end())
    {
        pTex = it->second;
    }

    if (pTex != NULL)
    {
        return pTex;
    }

    pTex = new CTexObj;
    if (pTex == NULL)
    {
        printf("Cannot allocate texture object.\n");
        return NULL;
    }

    textureMap[texId] = pTex;
    return pTex;
}

GLvoid CGLES3Context::DeleteTextureObject(GLuint texId)
{
    TextureMap::iterator it = textureMap.find(texId);
    CTexObj *p = NULL;

    if (it != textureMap.end())
    {
        p = it->second;
    }

    if (p == NULL)
        return;

    textureMap.erase(texId);
    delete p;
}

CTexObj* CGLES3Context::GetTexObjByUnit(GLuint unit, GLenum target)
{
    CTexObj *tex = NULL;

    unit -= GL_TEXTURE0;
    switch (target)
    {
    case GL_TEXTURE_2D:
        tex = texture2D[unit];
        break;

    case GL_TEXTURE_3D:
        tex = texture3D[unit];
        break;

    case GL_TEXTURE_EXTERNAL_OES:
        tex = textureExternal[unit];
        break;

    case GL_TEXTURE_CUBE_MAP:
    case GL_TEXTURE_CUBE_MAP_POSITIVE_X:
    case GL_TEXTURE_CUBE_MAP_NEGATIVE_X:
    case GL_TEXTURE_CUBE_MAP_POSITIVE_Y:
    case GL_TEXTURE_CUBE_MAP_NEGATIVE_Y:
    case GL_TEXTURE_CUBE_MAP_POSITIVE_Z:
    case GL_TEXTURE_CUBE_MAP_NEGATIVE_Z:
        tex = textureCube[unit];
        break;

    default:
        Abort("%s(%d): invalid texture target(0x%04X)\n", __FUNCTION__, __LINE__, target);
        break;
    }

    return tex;
}

CTexObj* CGLES3Context::GetActiveTexObj(GLenum target)
{
    return GetTexObjByUnit(activeTexUnit, target);
}

CTexObj* CGLES3Context::GetTexObjByName(GLuint name)
{
    return textureMap[name];
}

GLvoid CGLES3Context::ApiActiveTexture(GLenum texture)
{
    activeTexUnit = texture;
}

GLvoid CGLES3Context::ApiBindTexture(GLenum target, GLuint texture)
{
    GLuint  unit    = activeTexUnit - GL_TEXTURE0;
    CTexObj *pTex   = FindTexObj(texture);

    if (!pTex)
    {
        pTex = CreateTextureObject(texture);
    }

    pTex->target    = target;
    pTex->texIndex  = texture;
    switch (target)
    {
    case GL_TEXTURE_2D:
        texture2D[unit] = pTex;
        textureSampler[unit][TEXTURE_TYPE_2D] = texture;
        break;

    case GL_TEXTURE_3D:
        texture3D[unit] = pTex;
        textureSampler[unit][TEXTURE_TYPE_3D] = texture;
        break;

    case GL_TEXTURE_CUBE_MAP:
        textureCube[unit] = pTex;
        textureSampler[unit][TEXTURE_TYPE_CUBEMAP] = texture;
        break;

    case GL_TEXTURE_EXTERNAL_OES:
        textureExternal[unit] = pTex;
        textureSampler[unit][TEXTURE_TYPE_EXTERNAL] = texture;
        break;

    default:
        break;
    }
}

CTexObj* CGLES3Context::FindTexObj(GLuint texture)
{
    TextureMap::iterator it = textureMap.find(texture);

    if (it == textureMap.end())
    {
        return NULL;
    }
    else
    {
        return it->second;
    }
}

GLvoid CGLES3Context::ApiDeleteTextures(GLsizei n, const GLuint *textures)
{
    for (GLsizei i=0; i<n; i++)
    {
        GLuint tex = textures[i];
        DeleteTextureObject(tex);
    }
}

GLvoid CGLES3Context::ApiGenTextures(GLsizei n, GLuint *textures)
{
    for (GLsizei i=0; i<n; i++)
    {
        GLuint tex = textures[i];
        CreateTextureObject(tex);
    }

    m_pAnalyzer->AnalyzeGenTextures(GLOutput, GL_OUT_BUF_SIZE, n, textures);
}

GLvoid CGLES3Context::ApiGenerateMipmap(GLenum target)
{
    CTexObj *pTex = GetActiveTexObj(target);

    pTex->genMipmap = GL_TRUE;
}

GLvoid CTexObj::InsertTexImage(GLenum target, stTexImage *pTexImg)
{
    stTexImage *pTmp;
    stTexImage **ppFace;

    switch (target)
    {
    case GL_TEXTURE_2D:
        if (pTex2D == NULL)
        {
            pTex2D = pTexImg;
        }
        else
        {
            pTmp = pTex2D;

            while (pTmp->next)
            {
                pTmp = pTmp->next;
            }

            pTmp->next = pTexImg;
            pTexImg->next = NULL;
        }
        break;

    case GL_TEXTURE_3D:
        if (pTex3D == NULL)
        {
            pTex3D = pTexImg;
        }
        else
        {
            pTmp = pTex3D;

            while (pTmp->next)
            {
                pTmp = pTmp->next;
            }

            pTmp->next = pTexImg;
            pTexImg->next = NULL;
        }
        break;

    case GL_TEXTURE_EXTERNAL_OES:
        if (pTexExternal == NULL)
        {
            pTexExternal = pTexImg;
        }
        else
        {
            pTmp = pTexExternal;

            while (pTmp->next)
            {
                pTmp = pTmp->next;
            }

            pTmp->next = pTexImg;
            pTexImg->next = NULL;
        }
        break;

    case GL_TEXTURE_CUBE_MAP_POSITIVE_X:
    case GL_TEXTURE_CUBE_MAP_NEGATIVE_X:
    case GL_TEXTURE_CUBE_MAP_POSITIVE_Y:
    case GL_TEXTURE_CUBE_MAP_NEGATIVE_Y:
    case GL_TEXTURE_CUBE_MAP_POSITIVE_Z:
    case GL_TEXTURE_CUBE_MAP_NEGATIVE_Z:
        ppFace = &pTexCube[target-GL_TEXTURE_CUBE_MAP_POSITIVE_X];
        if (*ppFace == NULL)
        {
            *ppFace = pTexImg;
        }
        else
        {
            pTmp = *ppFace;

            while (pTmp->next)
            {
                pTmp = pTmp->next;
            }

            pTmp->next = pTexImg;
            pTexImg->next = NULL;
        }
        break;

    default:
        Abort("%s(%d): invalid texture target(0x%04X)\n", __FUNCTION__, __LINE__, target);
        break;
    }
}

stTexImage* CTexObj::CreateCompressedTexImage(GLenum target, GLuint width, GLuint height, GLuint depth, GLint level, GLenum format, GLenum type, const GLvoid *pixels, GLuint imgSize)
{
    stTexImage  *pTexImage  = NULL;

    pTexImage = new stTexImage();
    if (!pTexImage)
    {
        Abort("%s(%d): cannot allocate texture image.\n", __FUNCTION__, __LINE__);
        return NULL;
    }

    pTexImage->target           = target;
    pTexImage->format           = format;
    pTexImage->border           = 0;
    pTexImage->internalformat   = format;
    pTexImage->type             = type;
    pTexImage->width            = width;
    pTexImage->height           = height;
    pTexImage->depth            = depth;
    pTexImage->level            = level;
    pTexImage->imageSize        = imgSize;

    if (imgSize)
    {
        pTexImage->pixels = (GLubyte*)malloc(imgSize);
        if (!pTexImage->pixels)
        {
            printf("%s(%d): cannot allocate pixels' buffer.\n", __FUNCTION__, __LINE__);
        }

        if (pixels && pTexImage->pixels)
        {
            memcpy(pTexImage->pixels, pixels, imgSize);
        }
    }

    InsertTexImage(target, pTexImage);

    return pTexImage;
}

stTexImage* CTexObj::CreateTexImage(GLenum target, GLuint width, GLuint height, GLuint depth, GLint level, GLenum format, GLenum type, const GLvoid *pixels)
{
    stTexImage  *pTexImage  = NULL;
    GLuint      imageSize   = GetTexImageSize(format, type, width, height, depth);

    pTexImage = new stTexImage();
    if (!pTexImage)
    {
        Abort("%s(%d): cannot allocate texture image.\n", __FUNCTION__, __LINE__);
        return NULL;
    }

    pTexImage->target           = target;
    pTexImage->format           = format;
    pTexImage->border           = 0;
    pTexImage->internalformat   = format;
    pTexImage->type             = type;
    pTexImage->width            = width;
    pTexImage->height           = height;
    pTexImage->depth            = depth;
    pTexImage->level            = level;
    pTexImage->imageSize        = imageSize;

    if (imageSize)
    {
        pTexImage->pixels = (GLubyte*)malloc(imageSize);
        if (!pTexImage->pixels)
        {
            printf("%s(%d): cannot allocate pixels' buffer.\n", __FUNCTION__, __LINE__);
        }

        if (pixels && pTexImage->pixels)
        {
            memcpy(pTexImage->pixels, pixels, imageSize);
        }
    }

    InsertTexImage(target, pTexImage);

    return pTexImage;
}

GLvoid CGLES3Context::ApiTexImage2D(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const void *pixels)
{
    CTexObj     *pTexObj    = NULL;
    stTexImage  *pTexImg    = NULL;

    pTexObj = GetTexObjByUnit(activeTexUnit, target);
    pTexImg = pTexObj->GetTexImageByLevel(target, level);

    if (pTexImg == NULL)
    {
        pTexImg = pTexObj->CreateTexImage(target, width, height, 1, level, format, type, (GLvoid*)pixels);
    }
    else
    {
        pTexImg->target           = target;
        pTexImg->format           = format;
        pTexImg->border           = 0;
        pTexImg->internalformat   = format;
        pTexImg->type             = type;
        pTexImg->width            = width;
        pTexImg->height           = height;
        pTexImg->depth            = 1;
        pTexImg->level            = level;
        pTexImg->imageSize        = GetTexImageSize(format, type, width, height, 1);

        if (pTexImg->pixels)
        {
            free(pTexImg->pixels);
        }

        pTexImg->pixels = (GLubyte*)malloc(pTexImg->imageSize);
    }

    pTexImg->nPackAlignment     = packAlignment;
    pTexImg->nUnpackAlignment   = unpackAlignment;

    if (pixels == NULL)
    {
        return;
    }

    memcpy(pTexImg->pixels, pixels, pTexImg->imageSize);
    return;
}

GLvoid CGLES3Context::ApiTexSubImage2D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const void *pixels)
{
    CTexObj     *pTexObj    = GetActiveTexObj(target);
    stTexImage  *pTexImg    = pTexObj?pTexObj->GetTexImageByLevel(target, level):NULL;
    GLuint      pixelSize   = GetPixelSizeByTexFormat(format, type);
    GLubyte     *pDst       = pTexImg?pTexImg->pixels:NULL;
    GLubyte     *pSrc       = (GLubyte*)pixels;

    if (pTexImg == NULL)
        return;

    pTexImg->format             = format;
    pTexImg->type               = type;
    pTexImg->nPackAlignment     = packAlignment;
    pTexImg->nUnpackAlignment   = unpackAlignment;

    // When using glTexStorage2D, there is no type. So we don't allocate memory in glTexStorage2D.
    if (pDst == NULL)
    {
        GLuint pixelSize = GetPixelSizeByTexFormat(format, type);
        GLuint imageSize = pixelSize * pTexImg->width * pTexImg->height;
        pDst = (GLubyte*)malloc(imageSize);
        pTexImg->pixels = pDst;
    }

    // Update sub texture image.
    if (pSrc)
    {
        for (GLsizei i=0; i<pTexImg->height; i++)
        {
            for (GLsizei j=0; j<pTexImg->width; j++)
            {
                if (CheckPixelInRect(j, i, 1, xoffset, yoffset, 1, xoffset+width, yoffset+height, 1))
                {
                    for (GLuint k=0; k<pixelSize; k++)
                    {
                        *pDst ++ = *pSrc++;
                    }
                }
                else
                {
                    pDst += pixelSize;
                }
            }
        }
    }
}

GLvoid CGLES3Context::ApiTexImage3D(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLenum format, GLenum type, const void *pixels)
{
    CTexObj     *pTexObj    = NULL;
    stTexImage  *pTexImg    = NULL;

    pTexObj = GetTexObjByUnit(activeTexUnit, target);
    pTexImg = pTexObj->GetTexImageByLevel(target, level);

    if (pTexImg == NULL)
    {
        pTexImg = pTexObj->CreateTexImage(target, width, height, depth, level, format, type, (GLvoid*)pixels);
    }
    else
    {
        pTexImg->target           = target;
        pTexImg->format           = format;
        pTexImg->border           = 0;
        pTexImg->internalformat   = format;
        pTexImg->type             = type;
        pTexImg->width            = width;
        pTexImg->height           = height;
        pTexImg->depth            = 1;
        pTexImg->level            = level;
        pTexImg->imageSize        = GetTexImageSize(format, type, width, height, depth);

        if (pTexImg->pixels)
        {
            free(pTexImg->pixels);
        }

        pTexImg->pixels = (GLubyte*)malloc(pTexImg->imageSize);
    }

    pTexImg->nPackAlignment     = packAlignment;
    pTexImg->nUnpackAlignment   = unpackAlignment;

    if (pixels == NULL)
    {
        return;
    }

    memcpy(pTexImg->pixels, pixels, pTexImg->imageSize);
    return;
}

GLvoid CGLES3Context::ApiTexSubImage3D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const void *pixels)
{
    CTexObj     *pTexObj    = GetActiveTexObj(target);
    stTexImage  *pTexImg    = pTexObj?pTexObj->GetTexImageByLevel(target, level):NULL;
    GLuint      pixelSize   = GetPixelSizeByTexFormat(format, type);
    GLubyte     *pDst       = pTexImg?pTexImg->pixels:NULL;
    GLubyte     *pSrc       = (GLubyte*)pixels;

    if (!pTexImg)
        return;

    pTexImg->format             = format;
    pTexImg->type               = type;
    pTexImg->nPackAlignment     = packAlignment;
    pTexImg->nUnpackAlignment   = unpackAlignment;

    // When using glTexStorage3D, there is no type. So we don't allocate memory in glTexStorage2D.
    if (pDst == NULL)
    {
        GLuint imageSize = GetTexImageSize(format, type, pTexImg->width, pTexImg->height, pTexImg->depth);
        pDst = (GLubyte*)malloc(imageSize);
        pTexImg->pixels = pDst;
    }

    // Update sub texture image.
    if (pSrc)
    {
        for (GLsizei k=0; k<pTexImg->depth; k++)
        {
            for (GLsizei i=0; i<pTexImg->height; i++)
            {
                for (GLsizei j=0; j<pTexImg->width; j++)
                {
                    if (CheckPixelInRect(j, i, k, xoffset, yoffset, zoffset, xoffset+width, yoffset+height, zoffset+depth))
                    {
                        for (GLuint k=0; k<pixelSize; k++)
                        {
                            *pDst ++ = *pSrc++;
                        }
                    }
                    else
                    {
                        pDst += pixelSize;
                    }
                }
            }
        }
    }
}

GLvoid CGLES3Context::ApiTexStorage2D(GLenum target, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height)
{
    GLint   i, j;
    GLsizei levelWidth  = width;
    GLsizei levelHeight = height;
    CTexObj *pTexObj    = GetActiveTexObj(target);

    if (target == GL_TEXTURE_2D)
    {
        for (i=0; i<levels; i++)
        {
            pTexObj->CreateTexImage(target, levelWidth, levelHeight, 1, i, internalformat, 0, NULL);

            levelWidth  = MAX(1, levelWidth/2);
            levelHeight = MAX(1, levelHeight/2);
        }
    }
    else if (target == GL_TEXTURE_CUBE_MAP)
    {
        for (j=0; j<6; j++)
        {
            levelWidth  = width;
            levelHeight = height;
            for (i=0; i<levels; i++)
            {
                pTexObj->CreateTexImage(GL_TEXTURE_CUBE_MAP_POSITIVE_X+j, levelWidth, levelHeight, 1, i, internalformat, 0, NULL);

                levelWidth  = MAX(1, levelWidth/2);
                levelHeight = MAX(1, levelHeight/2);
            }
        }
    }
}

GLvoid CGLES3Context::ApiTexStorage3D(GLenum target, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth)
{
    GLint   i;
    GLsizei levelWidth  = width;
    GLsizei levelHeight = height;
    CTexObj *pTexObj    = GetActiveTexObj(target);

    if (target == GL_TEXTURE_3D)
    {
        for (i=0; i<levels; i++)
        {
            pTexObj->CreateTexImage(target, levelWidth, levelHeight, depth, i, internalformat, 0, NULL);

            levelWidth  = MAX(1, levelWidth/2);
            levelHeight = MAX(1, levelHeight/2);
        }
    }
}

GLvoid CGLES3Context::ApiCompressedTexImage2D(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLsizei imageSize, const void *data)
{
    GLenum      type        = GL_UNSIGNED_BYTE;
    CTexObj     *pTexObj    = GetActiveTexObj(target);
    stTexImage  *pTexImg    = pTexObj->GetTexImageByLevel(target, level);

    if (!pTexImg)
    {
        pTexImg = pTexObj->CreateCompressedTexImage(target, width, height, 1, level, internalformat, type, data, imageSize);
    }
    else
    {
        pTexImg->target           = target;
        pTexImg->format           = internalformat;
        pTexImg->border           = 0;
        pTexImg->internalformat   = internalformat;
        pTexImg->type             = type;
        pTexImg->width            = width;
        pTexImg->height           = height;
        pTexImg->depth            = 1;
        pTexImg->level            = level;
        pTexImg->imageSize        = imageSize;

        if (imageSize)
        {
            pTexImg->pixels = (GLubyte*)malloc(imageSize);
            if (!pTexImg->pixels)
            {
                printf("%s(%d): cannot allocate pixels' buffer.\n", __FUNCTION__, __LINE__);
            }
        }
    }

    pTexImg->nPackAlignment     = packAlignment;
    pTexImg->nUnpackAlignment   = unpackAlignment;

    if (data && pTexImg->pixels)
    {
        memcpy(pTexImg->pixels, data, imageSize);
    }

    m_pAnalyzer->AnalyzeCompressedTexImage2D(GLOutput, GL_OUT_BUF_SIZE, target, level, internalformat, width, height, border, imageSize, data);
}

GLvoid CGLES3Context::ApiCompressedTexSubImage2D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLsizei imageSize, const void *data)
{
    GLenum      type        = GL_UNSIGNED_BYTE;
    CTexObj     *pTexObj    = GetActiveTexObj(target);
    stTexImage  *pTexImg    = pTexObj->GetTexImageByLevel(target, level);

    if (!pTexImg)
        return;

    pTexImg->format             = format;
    pTexImg->imageSize          = imageSize;
    pTexImg->nPackAlignment     = packAlignment;
    pTexImg->nUnpackAlignment   = unpackAlignment;

    // When using glTexStorage2D, there is no type. So we don't allocate memory in glTexStorage2D.
    if (pTexImg->pixels == NULL)
    {
        pTexImg->pixels = (GLubyte*)malloc(imageSize);
        pTexImg->imageSize = imageSize;
    }

    if (data && pTexImg->pixels)
    {
        memcpy(pTexImg->pixels, data, imageSize);
    }
}

GLvoid CGLES3Context::ApiTexParameterf(GLenum target, GLenum pname, GLfloat param)
{
    CTexObj *pTex   = GetActiveTexObj(target);
    GLint   parami  = (GLint)param;

    if (!pTex)
        return;

    pTex->TexParameteri(target, pname, parami);
}

GLvoid CGLES3Context::ApiTexParameterfv(GLenum target, GLenum pname, const GLfloat *params)
{
    CTexObj *pTex   = GetActiveTexObj(target);
    GLint   parami  = (GLint)*params;

    if (!pTex)
        return;

    pTex->TexParameteri(target, pname, parami);
}

GLvoid CGLES3Context::ApiTexParameteri(GLenum target, GLenum pname, GLint param)
{
    CTexObj *pTex   = GetActiveTexObj(target);

    if (!pTex)
        return;

    pTex->TexParameteri(target, pname, param);
}

GLvoid CGLES3Context::ApiTexParameteriv(GLenum target, GLenum pname, const GLint *params)
{
    CTexObj *pTex   = GetActiveTexObj(target);
    GLint   parami  = (GLint)*params;

    if (!pTex)
        return;

    pTex->TexParameteri(target, pname, parami);
}

GLvoid CGLES3Context::ApiCopyTexImage2D(GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLsizei height, GLint border)
{
}

GLvoid CGLES3Context::ApiCopyTexSubImage2D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height)
{
}

GLvoid CGLES3Context::ApiGetTexParameterfv(GLenum target, GLenum pname, GLfloat *params)
{
}

GLvoid CGLES3Context::ApiGetTexParameteriv(GLenum target, GLenum pname, GLint *params)
{
}