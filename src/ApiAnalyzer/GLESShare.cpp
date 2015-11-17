#include "utils.h"
#include "bitmap.h"
#include "PngInterface.h"
#include "ConvertApiDump.h"
#include "MemoryPool.h"
#include "TranslateGL.h"
#include "gles3.h"
#include "GLESShare.h"
#include "ApiGLES3Context.h"


extern char             tmp[TMP_BUF_SIZE];
extern char             tmp1[TMP_BUF_SIZE];
extern char             tmp2[TMP_BUF_SIZE];
extern char             tmp3[TMP_BUF_SIZE];
extern char             tmp4[TMP_BUF_SIZE];
extern char             tmp5[TMP5_BUF_SIZE];
extern char             tmp6[TMP5_BUF_SIZE];

FRAME_BUFFER_MAPPING    frameBufferMap[FRAME_BUFFER_MAP_NUM];
RENDER_BUFFER_MAPPING   renderBufferMap[RENDER_BUFFER_MAP_NUM];

GLboolean               bUseBufferFunc  = GL_TRUE;
GLboolean               bUseUniformFunc = GL_TRUE;

GLvoid SaveCompressedTextureToFile(GLenum target,
                                   GLuint x,
                                   GLuint y,
                                   GLuint z,
                                   GLuint width,
                                   GLuint height,
                                   GLuint texWidth,
                                   GLuint texHeight,
                                   GLuint texDepth,
                                   GLenum format,
                                   GLenum type,
                                   GLuint level,
                                   GLuint srcPixels)
{
    GLchar          name[256];
    GLint           texIndex        = 0;
    GLuint          bpp             = 0;
    gceSURF_FORMAT  imageFormat     = gcvSURF_UNKNOWN;
    CTexObj         *pTexObj        = NULL;
    stTexImage      *pTexImg        = NULL;
    GLchar          *newData        = NULL;
    GLchar          *texFileName    = NULL;
    GLboolean       hasAlpha        = GL_FALSE;
    GLuint          bitsPerChannel  = 8;
    GLboolean       bSavePng        = GL_FALSE;
    GLuint          planeSize       = 0;
    GLubyte         *pSrc           = NULL;
    GLvoid          *pDecSrc        = NULL;
    GLenum          decFormat       = 0;
    GLint           newFormat       = 0;

    pTexObj     = CURRENT_CONTEXT1().GetTexObjByUnit(CURRENT_CONTEXT1().activeTexUnit, target);
    texIndex    = pTexObj->texIndex;
    pTexImg     = pTexObj->GetTexImageByLevel(target, level);
    planeSize   = pTexImg->imageSize/pTexImg->depth;

    if (srcPixels == NULL)
    {
        return;
    }

    if (g_bSaveTexture == GL_FALSE)
    {
        return;
    }

    TranslateTexTarget(target, tmp1);
    TranslateTexFormat(format, tmp2);
    TranslateDataTypeForSaveTexture(type, tmp3);

    for (GLuint i=0; i<texDepth; i++)
    {
        pSrc = (GLubyte*)((GLuint)pTexImg->pixels + planeSize*i);
        DecompressTextureImage(texWidth, texHeight, format, planeSize, pSrc, &imageFormat, &pDecSrc);
        if (!pDecSrc)
            continue;

        if (imageFormat == gcvSURF_B8G8R8)
        {
            bpp = 24;
        }

        memset(pTexImg->texName, 0, 256);
        memset(name, 0, 256);
        if (!bSavePng || imageFormat == gcvSURF_R5G6B5)
        {
            sprintf(pTexImg->texName, "%s_name%d_%s_%s_level%d_%dx%dx%d_%d.bmp",
                tmp1, texIndex, tmp2, tmp3, level,
                pTexImg->width, pTexImg->height, i, pTexImg->texUpdateCount++);
        }
        else
        {
            sprintf(pTexImg->texName, "%s_name%d_%s_%s_level%d_%dx%dx%d_%d.png",
                tmp1, texIndex, tmp2, tmp3, level,
                pTexImg->width, pTexImg->height, i, pTexImg->texUpdateCount++);
        }

        sprintf(name, "%s%s/%s", strOnlySourcePath, RESOURCE_PATH, pTexImg->texName);
        newData = FakeBitmap(imageFormat, pDecSrc, x, y, z, width, height, i, texWidth, texHeight, texDepth, bpp);
        if (!newData)
        {
            Abort("%s(%d): cannot allocate memory for fake bitmap.", __FUNCTION__, __LINE__);
        }

        // Fake A8
        switch (imageFormat)
        {
        case gcvSURF_A8:
        case gcvSURF_A8L8:
            newFormat = ARGB_8888;
            break;

        case gcvSURF_A8R8G8B8:
            hasAlpha = GL_TRUE;
            newFormat = ARGB_8888;
            break;

        case gcvSURF_A8B8G8R8:
            hasAlpha = GL_TRUE;
            newFormat = RGBA_8888;
            break;

        case gcvSURF_A4B4G4R4:
            hasAlpha = GL_TRUE;
            //newFormat = ARGB_8888;
            //newFormat = RGBA_8888;
            newFormat = ABGR_8888;
            break;

        case gcvSURF_A4R4G4B4:
            hasAlpha = GL_TRUE;
            newFormat = ARGB_8888;
            break;

        case gcvSURF_R4G4B4A4:
            hasAlpha = GL_TRUE;
            newFormat = RGBA_8888;
            break;

        case gcvSURF_B4G4R4A4:
            hasAlpha = GL_TRUE;
            newFormat = BGRA_8888;
            break;

        case gcvSURF_B8G8R8:
            newFormat = BGR_888;
            break;

        default:
            Abort("%s(%d)\n", __FUNCTION__, __LINE__);
            break;
        }

        // bit_depth: bits/channel
        if (!bSavePng || imageFormat == gcvSURF_R5G6B5)
        {
            ApiWriteBitmap(name, newData, width, height, 0, newFormat);
        }
        else
        {
            WritePNGFile(name, width, height, bitsPerChannel, newData, hasAlpha);
        }

        if (newData)
        {
            ApiReleaseTexMemory(newData);
        }

        if (pDecSrc)
        {
            free(pDecSrc);
        }
    }

    return;
}

GLvoid SaveTextureToFile(GLenum target,
                         GLuint x,
                         GLuint y,
                         GLuint z,
                         GLuint width,
                         GLuint height,
                         GLuint texWidth,
                         GLuint texHeight,
                         GLuint texDepth,
                         GLenum format,
                         GLenum type,
                         GLuint level,
                         GLuint srcPixels)
{
    GLchar          name[256];
    GLint           texIndex        = 0;
    GLuint          bpp             = 0;
    GLuint          newBpp          = 0;
    gceSURF_FORMAT  imageFormat     = gcvSURF_UNKNOWN;
    CTexObj         *pTexObj        = NULL;
    stTexImage      *pTexImg        = NULL;
    GLchar          *newData        = NULL;
    GLchar          *texFileName    = NULL;
    GLboolean       hasAlpha        = GL_FALSE;
    GLuint          bitsPerChannel  = 8;
    GLboolean       bSavePng        = GL_FALSE;
    GLuint          planeSize       = 0;
    GLubyte         *pSrc           = NULL;
    GLint           newFormat       = 0;

    pTexObj     = CURRENT_CONTEXT1().GetTexObjByUnit(CURRENT_CONTEXT1().activeTexUnit, target);
    texIndex    = pTexObj->texIndex;
    pTexImg     = pTexObj->GetTexImageByLevel(target, level);
    planeSize   = pTexImg->imageSize/pTexImg->depth;

    //if (texIndex == 34)
    //    __asm int 3;

    if (srcPixels == NULL)
    {
        return;
    }

    if (g_bSaveTexture == GL_FALSE)
    {
        return;
    }

    TranslateTexTarget(target, tmp1);
    TranslateTexFormat(format, tmp2);
    TranslateDataTypeForSaveTexture(type, tmp3);

    for (GLuint i=0; i<texDepth; i++)
    {
        pSrc = (GLubyte*)((GLuint)pTexImg->pixels + planeSize*i);
        memset(pTexImg->texName, 0, 256);
        memset(name, 0, 256);
        if (!bSavePng || imageFormat == gcvSURF_R5G6B5)
        {
            sprintf(pTexImg->texName, "%s_name%d_%s_%s_level%d_%dx%dx%d_%d.bmp",
                tmp1, texIndex, tmp2, tmp3, level,
                pTexImg->width, pTexImg->height, i, pTexImg->texUpdateCount++);
        }
        else
        {
            sprintf(pTexImg->texName, "%s_name%d_%s_%s_level%d_%dx%dx%d_%d.png",
                tmp1, texIndex, tmp2, tmp3, level,
                pTexImg->width, pTexImg->height, i, pTexImg->texUpdateCount++);
        }

        sprintf(name, "%s%s/%s", strOnlySourcePath, RESOURCE_PATH, pTexImg->texName);
        GetInternalFormatBppFromTextureFormat(format, type, &imageFormat, &bpp, &newBpp);

        newData = FakeBitmap(imageFormat, pSrc, x, y, z, width, height, i+1, texWidth, texHeight, texDepth, bpp);
        if (!newData)
        {
            Abort("%s(%d): cannot allocate memory for fake bitmap.", __FUNCTION__, __LINE__);
        }

        // Fake A8
        switch (imageFormat)
        {
        case gcvSURF_A8:
        case gcvSURF_A8L8:
            newFormat = ARGB_8888;
            break;

        case gcvSURF_A8R8G8B8:
            hasAlpha = GL_TRUE;
            newFormat = ARGB_8888;
            break;

        case gcvSURF_A8B8G8R8:
            hasAlpha = GL_TRUE;
            newFormat = RGBA_8888;
            break;

        case gcvSURF_A4B4G4R4:
            hasAlpha = GL_TRUE;
            //newFormat = ARGB_8888;
            //newFormat = RGBA_8888;
            newFormat = ABGR_8888;
            break;

        case gcvSURF_A4R4G4B4:
            hasAlpha = GL_TRUE;
            newFormat = ARGB_8888;
            break;

        case gcvSURF_R4G4B4A4:
            hasAlpha = GL_TRUE;
            newFormat = RGBA_8888;
            break;

        case gcvSURF_B4G4R4A4:
            hasAlpha = GL_TRUE;
            newFormat = BGRA_8888;
            break;

        case gcvSURF_B8G8R8:
            newFormat = BGR_888;
            break;

        case gcvSURF_R5G6B5:
            newFormat = RGB_565;
            break;

        case gcvSURF_R5G5B5A1:
            newFormat = ARGB_1555;
            break;

        default:
            Abort("%s(%d)\n", __FUNCTION__, __LINE__);
            break;
        }

        // bit_depth: bits/channel
        if (!bSavePng || imageFormat == gcvSURF_R5G6B5)
        {
            ApiWriteBitmap(name, newData, width, height, 0, newFormat);
        }
        else
        {
            WritePNGFile(name, width, height, bitsPerChannel, newData, hasAlpha);
        }

        if (newData)
        {
            ApiReleaseTexMemory(newData);
        }
    }

    return;
}

GLvoid TranslateRBOFormat(GLenum format, GLchar *output)
{
    memset(output, 0, TMP_BUF_SIZE);

    switch (format)
    {
    case GL_DEPTH_COMPONENT16:
        sprintf(output, "GL_DEPTH_COMPONENT16");
        break;

    case GL_RGBA4:
        sprintf(output, "GL_RGBA4");
        break;

    case GL_RGB5_A1:
        sprintf(output, "GL_RGB5_A1");
        break;

    case GL_RGB565:
        sprintf(output, "GL_RGB565");
        break;

    case GL_STENCIL_INDEX8:
        sprintf(output, "GL_STENCIL_INDEX8");
        break;

    case GL_DEPTH24_STENCIL8_OES:
        sprintf(output, "GL_DEPTH24_STENCIL8_OES");
        break;

    case GL_UNSIGNED_INT_24_8_OES:
        sprintf(output, "GL_UNSIGNED_INT_24_8_OES");
        break;

    case GL_DEPTH_STENCIL_OES:
        sprintf(output, "GL_DEPTH_STENCIL_OES");
        break;

    default:
        sprintf(output, "Invalid");
        break;
    }
}

void OutputData(int *pos, GLenum type, char *output, int outputSize, char *input)
{
    char    buf[128];
    char    c;
    short   s;
    int     n;
    float   f;
    GLfixed fixedData;

    if (input == NULL)
    {
        return;
    }

    memset(buf, 0, 32);
    switch (type)
    {
    case GL_BYTE:
        c = *input;
        n = 0x000000ff & c;
        sprintf(buf, "%03d", n);
        break;

    case GL_UNSIGNED_BYTE:
        c = *input;
        n = 0x000000ff & c;
        sprintf(buf, "%03u", n);
        break;

    case GL_SHORT:
        // memcpy_s(&s, sizeof(s), input, 2);
        memcpy(&s, input, 2);
        n = 0x0000ffff & s;
        sprintf(buf, "%05d", s);
        break;

    case GL_UNSIGNED_SHORT:
        //memcpy_s(&s, sizeof(s), input, 2);
        memcpy(&s, input, 2);
        n = 0x0000ffff & s;
        sprintf(buf, "%05u", n);
        break;

    case GL_FIXED:
        // memcpy_s(&f, sizeof(f), input, 4);
        memcpy(&fixedData, input, 4);
        sprintf(buf, "%08d", fixedData);
        break;

    case GL_FLOAT:
        // memcpy_s(&f, sizeof(f), input, 4);
        memcpy(&f, input, 4);
        //sprintf(buf, "%f", f);
        Float2String(f, buf);
        break;

    case GL_HALF_FLOAT_OES:
        // memcpy_s(&s, sizeof(s), input, 2);
        memcpy(&f, input, 4);
        f = (float)(0x0000ffff & (int)f);
        sprintf(buf, "%f", f);
        //Float2String(f, buf);
        break;

    case GL_UNSIGNED_INT_10_10_10_2_OES:
    case GL_INT_10_10_10_2_OES:
        /* Regardless of size being 3 or 4, the container size is 4. */
        sprintf(buf, "0x%08X", *(int*)input);
        break;

    case GL_INT:
        sprintf(buf, "%d", *(int*)input);
        break;

    default:
        /* Invalid type. */
        sprintf(buf, "0");
        break;
    }

    OutputStringFast(dumpData1, pos, output, outputSize, "%s", buf);
}

void OutputViewPort(int *pos, int esVersion, char *output, int outputSize)
{
    GLint     viewportX;
    GLint     viewportY;
    GLuint    viewportWidth;
    GLuint    viewportHeight;

    {
        viewportX       = CURRENT_CONTEXT1().viewportX;
        viewportY       = CURRENT_CONTEXT1().viewportY;
        viewportWidth   = CURRENT_CONTEXT1().viewportWidth;
        viewportHeight  = CURRENT_CONTEXT1().viewportHeight;
    }

    OutputStringFast(drawStates11, pos, output, outputSize, "****Viewport: (%04d, %04d, %04d, %04d)\n",
        viewportX, viewportY, viewportWidth, viewportHeight);
}

void OutputSampleCoverage(int *pos, int esVersion, char *output, int outputSize)
{
    GLboolean           sampleAlphaToCoverage;
    GLboolean           sampleCoverage;
    GLfloat             sampleCoverageValue;
    GLboolean           sampleCoverageInvert;

    sampleAlphaToCoverage   = CURRENT_CONTEXT1().sampleAlphaToCoverage;
    sampleCoverage          = CURRENT_CONTEXT1().sampleCoverage;
    sampleCoverageValue     = CURRENT_CONTEXT1().sampleCoverageValue;
    sampleCoverageInvert    = CURRENT_CONTEXT1().sampleCoverageInvert;

    if (sampleAlphaToCoverage)
    {
        OutputStringFast(drawStates12, pos, output, outputSize, "****SampleAlphaToCoverage: %s\n", sampleAlphaToCoverage?"on ":"off");
    }

    if (sampleCoverage)
    {
        OutputStringFast(drawStates13, pos, output, outputSize, "****SampleCoverage: %s", sampleCoverage?"on ":"off");
        if (sampleCoverage)
        {
            OutputStringFast(drawStates32, pos, output, outputSize, ", value: %08.3f, invert: %s\n",
                             sampleCoverageValue,  sampleCoverageInvert?"on ":"off");
        }
        else
        {
            OutputStringFast(oneChar, pos, output, outputSize, "\n");
        }
    }
}

void OutputPolygonOffset(int *pos, int esVersion, char *output, int outputSize)
{
    GLboolean           offsetEnable;
    GLfloat             offsetFactor;
    GLfloat             offsetUnits;

    {
        offsetEnable    = CURRENT_CONTEXT1().offsetEnable;
        offsetFactor    = CURRENT_CONTEXT1().offsetFactor;
        offsetUnits     = CURRENT_CONTEXT1().offsetUnits;
    }

    if (offsetEnable)
    {
        OutputStringFast(drawStates14, pos, output, outputSize, "****PolygonOffset: %s\t", offsetEnable?"on ":"off");

        if (offsetEnable)
        {
            OutputStringFast(drawStates15, pos, output, outputSize, "factor: %08.3f, units: %08.3f\n", offsetFactor, offsetUnits);
        }
        else
        {
            OutputStringFast(oneChar, pos, output, outputSize, "\n");
        }
    }
}

void OutputScissor(int *pos, int esVersion, char *output, int outputSize)
{
    GLboolean           scissorEnable;
    GLint               scissorX;
    GLint               scissorY;
    GLuint              scissorWidth;
    GLuint              scissorHeight;

    {
        scissorEnable   = CURRENT_CONTEXT1().scissorEnable;
        scissorX        = CURRENT_CONTEXT1().scissorX;
        scissorY        = CURRENT_CONTEXT1().scissorY;
        scissorWidth    = CURRENT_CONTEXT1().scissorWidth;
        scissorHeight   = CURRENT_CONTEXT1().scissorHeight;
    }

    if (scissorEnable)
    {
        OutputStringFast(drawStates17, pos, output, outputSize, "****Scissor: %s\t", scissorEnable?"on ":"off");

        if (scissorEnable)
        {
            OutputStringFast(drawStates18, pos, output, outputSize, "x: %04d, y: %04d, w: %04d, h: %04d\n",
                             scissorX, scissorY, scissorWidth, scissorHeight);
        }
        else
        {
            OutputStringFast(oneChar, pos, output, outputSize, "\n");
        }
    }
}

void OutputStencil(int *pos, int esVersion, char *output, int outputSize)
{
    GLboolean           stencilEnable;
    GLuint              stencilRefFront;
    GLuint              stencilRefBack;
    GLenum              stencilFuncFront;
    GLenum              stencilFuncBack;
    GLuint              stencilMaskFront;
    GLuint              stencilMaskBack;
    GLenum              stencilOpFailFront;
    GLenum              stencilOpFailBack;
    GLenum              stencilOpDepthFailFront;
    GLenum              stencilOpDepthFailBack;
    GLenum              stencilOpDepthPassFront;
    GLenum              stencilOpDepthPassBack;
    GLuint              stencilWriteMask;

    {
        stencilEnable               = CURRENT_CONTEXT1().stencilEnable;
        stencilRefFront             = CURRENT_CONTEXT1().stencilRefFront;
        stencilRefBack              = CURRENT_CONTEXT1().stencilRefBack;
        stencilFuncFront            = CURRENT_CONTEXT1().stencilFuncFront;
        stencilFuncBack             = CURRENT_CONTEXT1().stencilFuncBack;
        stencilMaskFront            = CURRENT_CONTEXT1().stencilMaskFront;
        stencilMaskBack             = CURRENT_CONTEXT1().stencilMaskBack;
        stencilOpFailFront          = CURRENT_CONTEXT1().stencilOpFailFront;
        stencilOpFailBack           = CURRENT_CONTEXT1().stencilOpFailBack;
        stencilOpDepthFailFront     = CURRENT_CONTEXT1().stencilOpDepthFailFront;
        stencilOpDepthFailBack      = CURRENT_CONTEXT1().stencilOpDepthFailBack;
        stencilOpDepthPassFront     = CURRENT_CONTEXT1().stencilOpDepthPassFront;
        stencilOpDepthPassBack      = CURRENT_CONTEXT1().stencilOpDepthPassBack;
        stencilWriteMask            = CURRENT_CONTEXT1().stencilWriteMask;
    }

    if (stencilEnable)
    {
        OutputStringFast(drawStates20, pos, output, outputSize, "****Stencil: %s", stencilEnable?"on ":"off");

        if (stencilEnable)
        {
            OutputStringFast(drawStates25, pos, output, outputSize, ", WriteMask: 0x%08X\t", stencilWriteMask);
            TranslateFunc(stencilFuncFront, tmp1);
            OutputStringFast(drawStates21, pos, output, outputSize, "\n****   FuncFront: %s, MaskFront: 0x%08X, RefFront: 0x%08X\n", tmp1, stencilMaskFront, stencilRefFront);
            TranslateFunc(stencilFuncBack, tmp1);
            OutputStringFast(drawStates22, pos, output, outputSize, "****   FuncBack: %s, MaskBack: 0x%08X, RefBack: 0x%08X\n", tmp1, stencilMaskBack, stencilRefBack);

            TranslateOperation(stencilOpFailFront, tmp1);
            TranslateOperation(stencilOpDepthFailFront, tmp2);
            TranslateOperation(stencilOpDepthPassFront, tmp3);
            OutputStringFast(drawStates23, pos, output, outputSize, "****   OpFailFront: %s, OpDepthFailFront: %s, OpDepthPassFront: %s\n", tmp1, tmp2, tmp3);

            TranslateOperation(stencilOpFailBack, tmp1);
            TranslateOperation(stencilOpDepthFailBack, tmp2);
            TranslateOperation(stencilOpDepthPassBack, tmp3);
            OutputStringFast(drawStates24, pos, output, outputSize, "****   OpFailBack: %s, OpDepthFailBack: %s, OpDepthPassBack: %s\n", tmp1, tmp2, tmp3);
        }
        else
        {
            OutputStringFast(oneChar, pos, output, outputSize, "\n");
        }
    }
}

void OutputBlend(int *pos, int esVersion, char *output, int outputSize)
{
    GLboolean   blendEnable;
    GLenum      blendFuncSourceRGB, blendFuncSourceAlpha;
    GLenum      blendFuncTargetRGB, blendFuncTargetAlpha;
    GLenum      blendModeRGB, blendModeAlpha;
    GLclampf    blendColorRed, blendColorGreen, blendColorBlue;
    GLclampf    blendColorAlpha;

    {
        blendEnable             = CURRENT_CONTEXT1().blendEnable;
        blendFuncSourceRGB      = CURRENT_CONTEXT1().blendFuncSourceRGB;
        blendFuncSourceAlpha    = CURRENT_CONTEXT1().blendFuncSourceAlpha;
        blendFuncTargetRGB      = CURRENT_CONTEXT1().blendFuncTargetRGB;
        blendFuncTargetAlpha    = CURRENT_CONTEXT1().blendFuncTargetAlpha;
        blendModeRGB            = CURRENT_CONTEXT1().blendModeRGB;
        blendModeAlpha          = CURRENT_CONTEXT1().blendModeAlpha;
        blendColorRed           = CURRENT_CONTEXT1().blendColorRed;
        blendColorGreen         = CURRENT_CONTEXT1().blendColorGreen;
        blendColorBlue          = CURRENT_CONTEXT1().blendColorBlue;
        blendColorAlpha         = CURRENT_CONTEXT1().blendColorAlpha;
    }

    if (blendEnable)
    {
        OutputStringFast(drawStates3, pos, output, outputSize, "****Blend: %s\t", blendEnable ? "on " : "off");

        if (blendEnable)
        {
            /* value */
            OutputStringFast(drawStates4, pos, output, outputSize, "R=%1.3f, G=%1.3f, B=%1.3f, A=%1.3f\n",
                             blendColorRed, blendColorGreen, blendColorBlue, blendColorAlpha);

            /* func */
            TranslateBlendFunc(blendFuncSourceRGB, tmp1);
            OutputStringFast(drawStates5, pos, output, outputSize, "****   srcRGB: %s\t", tmp1);
            TranslateBlendFunc(blendFuncSourceAlpha, tmp1);
            OutputStringFast(drawStates6, pos, output, outputSize, "srcAlpha: %s\n", tmp1);
            TranslateBlendFunc(blendFuncTargetRGB, tmp1);
            OutputStringFast(drawStates7, pos, output, outputSize, "****   dstRGB: %s\t", tmp1);
            TranslateBlendFunc(blendFuncTargetAlpha, tmp1);
            OutputStringFast(drawStates8, pos, output, outputSize, "dstAlpha: %s\n", tmp1);

            /* mode */
            TranslateBlendMode(blendModeRGB, tmp1);
            OutputStringFast(drawStates9, pos, output, outputSize, "****   modeRGB: %s", tmp1);
            TranslateBlendMode(blendModeAlpha, tmp1);
            OutputStringFast(drawStates10, pos, output, outputSize, "modeAlpha: %s\n", tmp1);
        }
        else
        {
            OutputStringFast(oneChar, pos, output, outputSize, "\n");
        }
    }
}

void OutputCullFace(int *pos, int esVersion, char *output, int outputSize)
{
    const char  *ptr = NULL;
    GLboolean   cullEnable;
    GLenum      cullMode;
    GLenum      cullFront;

    {
        cullEnable  = CURRENT_CONTEXT1().cullEnable;
        cullMode    = CURRENT_CONTEXT1().cullMode;
        cullFront   = CURRENT_CONTEXT1().cullFront;
    }

    if (cullEnable)
    {
        OutputStringFast(drawStates26, pos, output, outputSize, "****CullFace: %s\t", cullEnable?"on ":"off");

        if (cullEnable)
        {
            switch (cullMode)
            {
            case GL_FRONT:
                ptr = "GL_FRONT         ";
                break;

            case GL_BACK:
                ptr = "GL_BACK          ";
                break;

            case GL_FRONT_AND_BACK:
                ptr = "GL_FRONT_BACK    ";
                break;

            default:
                ptr = "Invalid          ";
                break;
            }

            OutputStringFast(drawStates27, pos, output, outputSize, "%s\t", ptr);

            switch (cullFront)
            {
            case GL_CW:
                ptr = "GL_CW    ";
                break;

            case GL_CCW:
                ptr = "GL_CCW   ";
                break;

            default:
                ptr = "Invalid  ";
                break;
            }
            OutputStringFast(drawStates28, pos, output, outputSize, "%s\n", ptr);
        }
        else
        {
            OutputStringFast(oneChar, pos, output, outputSize, "\n");
        }
    }
}

void OutputDepth(int *pos, int esVersion, char *output, int outputSize)
{
    GLboolean   depthTest;
    GLenum      depthFunc;
    GLboolean   depthMask;
    GLclampf    depthNear;
    GLclampf    depthFar;

    {
        depthTest   = CURRENT_CONTEXT1().depthTest;
        depthFunc   = CURRENT_CONTEXT1().depthFunc;
        depthMask   = CURRENT_CONTEXT1().depthMask;
        depthNear   = CURRENT_CONTEXT1().depthNear;
        depthFar    = CURRENT_CONTEXT1().depthFar;
    }

    if (depthTest)
    {
        OutputStringFast(drawStates30, pos, output, outputSize, "****DepthTest: %s\t", depthTest?"on ":"off");

        if (depthTest)
        {
            TranslateFunc(depthFunc, tmp1);
            OutputStringFast(drawStates31, pos, output, outputSize, "Near=%08.3f, Far=%08.3f, Func: %s, Mask: %s\n",
                             depthNear, depthFar, tmp1, depthMask?"on ":"off");
        }
        else
        {
            OutputStringFast(oneChar, pos, output, outputSize, "\n");
        }
    }
}

void OutputDither(int *pos, int esVersion, char *output, int outputSize)
{
    GLboolean   dither;

    {
        dither = CURRENT_CONTEXT1().ditherEnable;
    }

    if (dither)
    {
        OutputStringFast(drawStates33, pos, output, outputSize, "****Dither: %s\n", dither?"on ":"off");
    }
}

void GetPrimitiveCount(GLenum Mode, GLsizei Count, GLuint *PrimitiveCount)
{
    switch (Mode)
    {
    case GL_POINTS:
        *PrimitiveCount = (UINT) Count;
        break;

    case GL_LINES:
        *PrimitiveCount = (UINT) Count / 2;
        break;

    case GL_LINE_LOOP:
        *PrimitiveCount = (UINT) Count;
        break;

    case GL_LINE_STRIP:
        *PrimitiveCount = (UINT) Count - 1;
        break;

    case GL_TRIANGLES:
        *PrimitiveCount = (UINT) Count / 3;
        break;

    case GL_TRIANGLE_STRIP:
        *PrimitiveCount = (UINT) Count - 2;
        break;

    case GL_TRIANGLE_FAN:
        *PrimitiveCount = (UINT) Count - 2;
        break;

    case 0x7 /*GL_QUADS*/:
        *PrimitiveCount = (UINT) Count / 4;
        break;

    default:
        *PrimitiveCount = 0;
    }

    return;
}

void OutputStates(int *pos, int esVersion, char *output, int outputSize)
{
    OutputCurrentFBO(pos, output, outputSize);
    OutputStringFast(drawStates1, pos, output, outputSize, "\n*********************states*********************\n");
    OutputBlend(pos, esVersion, output, outputSize);
    OutputCullFace(pos, esVersion, output, outputSize);
    OutputDepth(pos, esVersion, output, outputSize);
    OutputDither(pos, esVersion, output, outputSize);
    OutputScissor(pos, esVersion, output, outputSize);
    OutputStencil(pos, esVersion, output, outputSize);
    OutputPolygonOffset(pos, esVersion, output, outputSize);
    OutputSampleCoverage(pos, esVersion, output, outputSize);
    OutputViewPort(pos, esVersion, output, outputSize);
    OutputStringFast(drawStates2, pos, output, outputSize, "*********************end************************\n");
}

void DumpMatrix(int *pos, GLenum type, int row, int column, GLvoid *data, char *output, int outputSize)
{
    GLint   i,j;
    GLuint  size    = GetDataTypeSize(type);
    GLchar  *p      = (GLchar*)data;

    for (i=0; i<row; i++)
    {
        OutputStringFast(dumpMatrix1, pos, output, outputSize, "    ");
        for (j=0; j<column; j++)
        {
            OutputData(pos, type, output, outputSize, p);
            OutputStringFast(twoChar, pos, output, outputSize, ", ");
            p += size;
        }
        OutputStringFast(oneChar, pos, output, outputSize, "\n");
    }
}

void DumpVector(int *pos, GLenum type, int num, GLvoid *data, char *output, int outputSize)
{
    GLuint  size    = GetDataTypeSize(type);
    GLchar  *p      = (GLchar*)data;

    OutputStringFast(fourChar, pos, output, outputSize, "   (");
    for (int i=0; i<num-1; i++)
    {
        OutputData(pos, type, output, outputSize, p);
        OutputStringFast(twoChar, pos, output, outputSize, ", ");
        p += size;
    }
    OutputData(pos, type, output, outputSize, p);
    OutputStringFast(twoChar, pos, output, outputSize, ")\n");
}

void GenDelObjects(GLvoid *pContext,
                   GLchar *output,
                   GLuint outputSize,
                   GLuint count,
                   const GLuint *buffers,
                   const GLchar *funcName,
                   const GLchar *objName)
{
    const GLuint  *v = buffers;

    if (v != NULL)
    {
        sprintf(output, "%s(%d) = ", funcName, count);
        for (unsigned int i=0; i<count; i++)
        {
            GLuint id = *(v+i);

            OutputStrcat(output, outputSize, "%04d, ", id);
        }
        OutputStrcat(output, outputSize, "\n");
    }
    else
    {
        sprintf(output, "%s(%d) data error\n", funcName, count);
    }
}

void PrintParamByFormat(int *pos, GLchar *output, GLuint outputSize, GLuint param, const GLchar *paramName, PARAM_FORMAT format)
{
    GLchar  *str = (GLchar*)param;

    switch (format)
    {
    case INT_04D:
        OutputStrcatFast(*pos, output, outputSize, "%s=%04d, ", paramName, param);
        *pos += (strlen(paramName) + 7);
        break;

    case INT_04D_NO:
        OutputStrcatFast(*pos, output, outputSize, "%04d, ", param);
        *pos += 6;
        break;

    case INT_08D:
        OutputStrcatFast(*pos, output, outputSize, "%s=%08d, ", paramName, param);
        *pos += (strlen(paramName) + 11);
        break;

    case INT_0X04X:
        OutputStrcatFast(*pos, output, outputSize, "%s=0x%04X, ", paramName, param);
        *pos += (strlen(paramName) + 9);
        break;

    case INT_0X08X:
        OutputStrcatFast(*pos, output, outputSize, "%s=0x%08X, ", paramName, param);
        *pos += (strlen(paramName) + 13);
        break;

    case STR_STR:
        OutputStrcatFast(*pos, output, outputSize, "%s, ", str);
        *pos += (strlen(str) + 2);
        break;

    case STR_STR_NAME:
        OutputStrcatFast(*pos, output, outputSize, "%s=%s, ", paramName, str);
        *pos += (strlen(paramName) + strlen(str) + 2);
        break;

    case FLOAT_1_1:
        OutputStrcatFast(*pos, output, outputSize, "%s=%1.1f, ", paramName, *(GLfloat*)&param);
        *pos += (strlen(paramName) + 6);
        break;

    case FLOAT_1_5:
        OutputStrcatFast(*pos, output, outputSize, "%s=%1.5f, ", paramName, *(GLfloat*)&param);
        *pos += (strlen(paramName) + 10);
        break;

    case FLOAT_5_1:
        OutputStrcatFast(*pos, output, outputSize, "%s=%05.1f, ", paramName, *(GLfloat*)&param);
        *pos += (strlen(paramName) + 10);
        break;
    }
}

void PrintParams0(GLchar *output, GLuint outputSize, const GLchar *funcName)
{
    sprintf(output, "%s()\n", funcName);
}

void PrintParams1(GLchar *output, GLuint outputSize, const GLchar *funcName,
                  GLuint param, const GLchar *paramName, PARAM_FORMAT format,
                  GLboolean bShader)
{
    int pos = 0;

    sprintf(output, "%s(", funcName); pos += (strlen(funcName)+1);
    PrintParamByFormat(&pos, output, outputSize, param, paramName, format);
    OutputStrcatFast(pos-2, output, outputSize, ")\n");

    if (bShader)
    {
        OutputToShaderFile(output);
    }
}

void PrintParams2(GLchar *output, GLuint outputSize, const GLchar *funcName,
                  GLuint param1, const GLchar *paramName1, PARAM_FORMAT format1,
                  GLuint param2, const GLchar *paramName2, PARAM_FORMAT format2,
                  GLboolean bShader)
{
    int pos = 0;

    sprintf(output, "%s(", funcName); pos += (strlen(funcName)+1);
    PrintParamByFormat(&pos, output, outputSize, param1, paramName1, format1);
    PrintParamByFormat(&pos, output, outputSize, param2, paramName2, format2);
    OutputStrcatFast(pos-2, output, outputSize, ")\n");

    if (bShader)
    {
        OutputToShaderFile(output);
    }
}

void PrintParams3(GLchar *output, GLuint outputSize, const GLchar *funcName,
                  GLuint param1, const GLchar *paramName1, PARAM_FORMAT format1,
                  GLuint param2, const GLchar *paramName2, PARAM_FORMAT format2,
                  GLuint param3, const GLchar *paramName3, PARAM_FORMAT format3)
{
    int pos = 0;

    sprintf(output, "%s(", funcName); pos += (strlen(funcName)+1);
    PrintParamByFormat(&pos, output, outputSize, param1, paramName1, format1);
    PrintParamByFormat(&pos, output, outputSize, param2, paramName2, format2);
    PrintParamByFormat(&pos, output, outputSize, param3, paramName3, format3);
    OutputStrcatFast(pos-2, output, outputSize, ")\n");
}

void PrintParams4(GLchar *output, GLuint outputSize, const GLchar *funcName,
                  GLuint param1, const GLchar *paramName1, PARAM_FORMAT format1,
                  GLuint param2, const GLchar *paramName2, PARAM_FORMAT format2,
                  GLuint param3, const GLchar *paramName3, PARAM_FORMAT format3,
                  GLuint param4, const GLchar *paramName4, PARAM_FORMAT format4)
{
    int pos = 0;

    sprintf(output, "%s(", funcName); pos += (strlen(funcName)+1);
    PrintParamByFormat(&pos, output, outputSize, param1, paramName1, format1);
    PrintParamByFormat(&pos, output, outputSize, param2, paramName2, format2);
    PrintParamByFormat(&pos, output, outputSize, param3, paramName3, format3);
    PrintParamByFormat(&pos, output, outputSize, param4, paramName4, format4);
    OutputStrcatFast(pos-2, output, outputSize, ")\n");
}

void PrintParams1Return(GLchar *output, GLuint outputSize, const GLchar *funcName,
                        GLuint param1, const GLchar *paramName1, PARAM_FORMAT format1,
                        GLuint ret, PARAM_FORMAT retFormat)
{
    int pos = 0;

    sprintf(output, "%s(", funcName); pos += (strlen(funcName)+1);
    PrintParamByFormat(&pos, output, outputSize, param1, paramName1, format1);
    OutputStrcatFast(pos-2, output, outputSize, ") = %d\n", ret);
}