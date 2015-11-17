#ifndef __GLES3_TEX_H
#define __GLES3_TEX_H

#include <GLES3/gl31.h>
#include "types.h"

typedef struct _stTexImage
{
    struct _stTexImage  *next;
    GLchar              texName[256];
    GLuint              texUpdateCount;

    GLenum              target;
    GLint               level;
    GLsizei             width;          /* Texture width. */
    GLsizei             height;         /* Texture height. */
    GLsizei             depth;          /* 3D texture's depth */
    GLint               internalformat;
    GLint               border;
    GLenum              format;
    GLenum              type;
    GLubyte             *pixels;
    GLuint              imageSize;      /* total texture image data size */

    GLint               nUnpackAlignment;
    GLint               nPackAlignment;

    _stTexImage()
    {
        next            = NULL;
        target          = 0;
        level           = 0;
        width           = 0;
        height          = 0;
        depth           = 0;
        internalformat  = 0;
        border          = 0;
        format          = 0;
        pixels          = NULL;
        texUpdateCount  = 0;
        imageSize       = 0;

        nUnpackAlignment    = 0;
        nPackAlignment      = 0;

        memset(texName, 0, 256);
    }

    ~_stTexImage()
    {
        if (pixels)
        {
            free(pixels);
        }
    }
} stTexImage;

class CTexObj
{
public:
    CTexObj();
    ~CTexObj();

    GLenum      GetTexFormat();
    stTexImage* GetTexImageByLevel(GLint level);
    stTexImage* GetTexImageByLevel(GLenum target, GLint level);
    GLsizei     GetWidth(GLint level);
    GLsizei     GetHeight(GLint level);
    GLvoid      TexParameteri(GLenum target, GLenum pname, GLint value);
    GLvoid      InsertTexImage(GLenum target, stTexImage *pTexImg);
    stTexImage* CreateTexImage(GLenum target, GLuint width, GLuint height, GLuint depth, GLint level, GLenum format, GLenum type, const GLvoid *pixels);
    stTexImage* CreateCompressedTexImage(GLenum target, GLuint width, GLuint height, GLuint depth, GLint level, GLenum format, GLenum type, const GLvoid *pixels, GLuint imgSize);
    GLuint      GetImageCount(GLenum target);

public:
    GLuint      texIndex;
    GLint       minFilter;
    GLint       magFilter;
    GLint       anisoFilter;
    GLint       wrapS;
    GLint       wrapT;
    GLint       wrapR;
    GLint       baseLevel;
    GLint       maxLevel;
    GLint       minLod;
    GLint       maxLod;
    GLenum      texCmpMode;
    GLenum      texCmpFunc;
    GLboolean   genMipmap;
    GLenum      swizzleR;
    GLenum      swizzleG;
    GLenum      swizzleB;
    GLenum      swizzleA;

    GLenum      target;

    stTexImage  *pTex2D;
    stTexImage  *pTexCube[6];
    stTexImage  *pTex3D;
    stTexImage  *pTexExternal;
};

typedef std::map<GLuint, CTexObj*>  TextureMap;

GLboolean   CheckPixelInRect(GLuint posX, GLuint posY, GLuint posZ, GLuint left, GLuint top, GLuint zNear, GLuint right, GLuint bottom, GLuint zFar);
GLvoid      DecompressTextureImage(GLint width,
                                   GLint height,
                                   GLenum internalformat,
                                   GLsizei imageSize,
                                   GLvoid *data,
                                   gceSURF_FORMAT *format,
                                   GLvoid **decompressedImage);
GLchar*     FakeBitmap(gceSURF_FORMAT   format,
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
                       GLuint           bpp);

#endif /* __GLES3_TEX_H */