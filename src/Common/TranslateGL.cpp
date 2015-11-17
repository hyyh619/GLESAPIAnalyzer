#include <stdio.h>
#include <stdlib.h>
#include "utils.h"
#include "TranslateGL.h"


GLvoid TranslateGetName(GLenum name, char *output)
{
    memset(output, 0, TMP_BUF_SIZE);

    switch (name)
    {
    case GL_MAX_VERTEX_ATTRIBS:
        sprintf(output, "GL_MAX_VERTEX_ATTRIBS");
        break;

    case GL_MAX_TEXTURE_IMAGE_UNITS:
        sprintf(output, "GL_MAX_TEXTURE_IMAGE_UNITS");
        break;

    case GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS:
        sprintf(output, "GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS");
        break;

    case GL_MAX_RENDERBUFFER_SIZE:
        sprintf(output, "GL_MAX_RENDERBUFFER_SIZE");
        break;

    case GL_LINE_WIDTH:
        sprintf(output, "GL_LINE_WIDTH");
        break;

    case GL_ALIASED_POINT_SIZE_RANGE:
        sprintf(output, "GL_ALIASED_POINT_SIZE_RANGE");
        break;

    case GL_ALIASED_LINE_WIDTH_RANGE:
        sprintf(output, "GL_ALIASED_LINE_WIDTH_RANGE");
        break;

    case GL_CULL_FACE_MODE:
        sprintf(output, "GL_CULL_FACE_MODE");
        break;

    case GL_FRONT_FACE:
        sprintf(output, "GL_FRONT_FACE");
        break;

    case GL_DEPTH_RANGE:
        sprintf(output, "GL_DEPTH_RANGE");
        break;

    case GL_DEPTH_WRITEMASK:
        sprintf(output, "GL_DEPTH_WRITEMASK");
        break;

    case GL_DEPTH_CLEAR_VALUE:
        sprintf(output, "GL_DEPTH_CLEAR_VALUE");
        break;

    case GL_DEPTH_FUNC:
        sprintf(output, "GL_DEPTH_FUNC");
        break;

    case GL_STENCIL_CLEAR_VALUE:
        sprintf(output, "GL_STENCIL_CLEAR_VALUE");
        break;

    case GL_STENCIL_FUNC:
        sprintf(output, "GL_STENCIL_FUNC");
        break;

    case GL_STENCIL_FAIL:
        sprintf(output, "GL_STENCIL_FAIL");
        break;

    case GL_STENCIL_PASS_DEPTH_FAIL:
        sprintf(output, "GL_STENCIL_PASS_DEPTH_FAIL");
        break;

    case GL_STENCIL_PASS_DEPTH_PASS:
        sprintf(output, "GL_STENCIL_PASS_DEPTH_PASS");
        break;

    case GL_STENCIL_REF:
        sprintf(output, "GL_STENCIL_REF");
        break;

    case GL_STENCIL_VALUE_MASK:
        sprintf(output, "GL_STENCIL_VALUE_MASK");
        break;

    case GL_STENCIL_WRITEMASK:
        sprintf(output, "GL_STENCIL_WRITEMASK");
        break;

    case GL_STENCIL_BACK_FUNC:
        sprintf(output, "GL_STENCIL_BACK_FUNC");
        break;

    case GL_STENCIL_BACK_FAIL:
        sprintf(output, "GL_STENCIL_BACK_FAIL");
        break;

    case GL_STENCIL_BACK_PASS_DEPTH_FAIL:
        sprintf(output, "GL_STENCIL_BACK_PASS_DEPTH_FAIL");
        break;

    case GL_STENCIL_BACK_PASS_DEPTH_PASS:
        sprintf(output, "GL_STENCIL_BACK_PASS_DEPTH_PASS");
        break;

    case GL_STENCIL_BACK_REF:
        sprintf(output, "GL_STENCIL_BACK_REF");
        break;

    case GL_STENCIL_BACK_VALUE_MASK:
        sprintf(output, "GL_STENCIL_BACK_VALUE_MASK");
        break;

    case GL_STENCIL_BACK_WRITEMASK:
        sprintf(output, "GL_STENCIL_BACK_WRITEMASK");
        break;

    case GL_VIEWPORT:
        sprintf(output, "GL_VIEWPORT");
        break;

    case GL_SCISSOR_BOX:
        sprintf(output, "GL_SCISSOR_BOX");
        break;

    case GL_SCISSOR_TEST:
        sprintf(output, "GL_SCISSOR_TEST");
        break;

    case GL_COLOR_CLEAR_VALUE:
        sprintf(output, "GL_COLOR_CLEAR_VALUE");
        break;

    case GL_COLOR_WRITEMASK:
        sprintf(output, "GL_COLOR_WRITEMASK");
        break;

    case GL_UNPACK_ALIGNMENT:
        sprintf(output, "GL_UNPACK_ALIGNMENT");
        break;

    case GL_PACK_ALIGNMENT:
        sprintf(output, "GL_PACK_ALIGNMENT");
        break;

    case GL_MAX_TEXTURE_SIZE:
        sprintf(output, "GL_MAX_TEXTURE_SIZE");
        break;

    case GL_MAX_VIEWPORT_DIMS:
        sprintf(output, "GL_MAX_VIEWPORT_DIMS");
        break;

    case GL_SUBPIXEL_BITS:
        sprintf(output, "GL_SUBPIXEL_BITS");
        break;

    case GL_RED_BITS:
        sprintf(output, "GL_RED_BITS");
        break;

    case GL_GREEN_BITS:
        sprintf(output, "GL_GREEN_BITS");
        break;

    case GL_BLUE_BITS:
        sprintf(output, "GL_BLUE_BITS");
        break;

    case GL_ALPHA_BITS:
        sprintf(output, "GL_ALPHA_BITS");
        break;

    case GL_DEPTH_BITS:
        sprintf(output, "GL_DEPTH_BITS");
        break;

    case GL_STENCIL_BITS:
        sprintf(output, "GL_STENCIL_BITS");
        break;

    case GL_POLYGON_OFFSET_UNITS:
        sprintf(output, "GL_POLYGON_OFFSET_UNITS");
        break;

    case GL_POLYGON_OFFSET_FILL:
        sprintf(output, "GL_POLYGON_OFFSET_FILL");
        break;

    case GL_POLYGON_OFFSET_FACTOR:
        sprintf(output, "GL_POLYGON_OFFSET_FACTOR");
        break;

    case GL_TEXTURE_BINDING_2D:
        sprintf(output, "GL_TEXTURE_BINDING_2D");
        break;

    case GL_SAMPLE_BUFFERS:
        sprintf(output, "GL_SAMPLE_BUFFERS");
        break;

    case GL_SAMPLES:
        sprintf(output, "GL_SAMPLES");
        break;

    case GL_SAMPLE_COVERAGE_VALUE:
        sprintf(output, "GL_SAMPLE_COVERAGE_VALUE");
        break;

    case GL_SAMPLE_COVERAGE_INVERT:
        sprintf(output, "GL_SAMPLE_COVERAGE_INVERT");
        break;

    case GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT:
        sprintf(output, "GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT");
        break;

    case GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS:
        sprintf(output, "GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS");
        break;

    case GL_MAX_FRAGMENT_UNIFORM_VECTORS:
        sprintf(output, "GL_MAX_FRAGMENT_UNIFORM_VECTORS");
        break;

    case GL_MAX_VARYING_VECTORS:
        sprintf(output, "GL_MAX_VARYING_VECTORS");
        break;

    case GL_MAX_VERTEX_UNIFORM_VECTORS:
        sprintf(output, "GL_MAX_VERTEX_UNIFORM_VECTORS");
        break;

    case GL_MAX_SAMPLES_ANGLE:
        sprintf(output, "GL_MAX_SAMPLES_ANGLE");
        break;

    case GL_MAX_CUBE_MAP_TEXTURE_SIZE:
        sprintf(output, "GL_MAX_CUBE_MAP_TEXTURE_SIZE");
        break;

    case GL_CURRENT_PROGRAM:
        sprintf(output, "GL_CURRENT_PROGRAM");
        break;

    case GL_SHADER_COMPILER:
        sprintf(output, "GL_SHADER_COMPILER");
        break;

    case GL_BLEND_EQUATION:
        sprintf(output, "GL_BLEND_EQUATION");
        break;

    case GL_BLEND_EQUATION_ALPHA:
        sprintf(output, "GL_BLEND_EQUATION_ALPHA");
        break;

    case GL_BLEND_DST_RGB:
        sprintf(output, "GL_BLEND_DST_RGB");
        break;

    case GL_BLEND_SRC_RGB:
        sprintf(output, "GL_BLEND_SRC_RGB");
        break;

    case GL_BLEND_DST_ALPHA:
        sprintf(output, "GL_BLEND_DST_ALPHA");
        break;

    case GL_BLEND_SRC_ALPHA:
        sprintf(output, "GL_BLEND_SRC_ALPHA");
        break;

    case GL_GENERATE_MIPMAP_HINT:
        sprintf(output, "GL_GENERATE_MIPMAP_HINT");
        break;

    case GL_TEXTURE_BINDING_CUBE_MAP:
        sprintf(output, "GL_TEXTURE_BINDING_CUBE_MAP");
        break;

    case GL_ACTIVE_TEXTURE:
        sprintf(output, "GL_ACTIVE_TEXTURE");
        break;

    case GL_ARRAY_BUFFER_BINDING:
        sprintf(output, "GL_ARRAY_BUFFER_BINDING");
        break;

    case GL_ELEMENT_ARRAY_BUFFER_BINDING:
        sprintf(output, "GL_ELEMENT_ARRAY_BUFFER_BINDING");
        break;

    case GL_DONT_CARE:
        sprintf(output, "GL_DONT_CARE");
        break;

    case GL_FASTEST:
        sprintf(output, "GL_FASTEST");
        break;

    case GL_NICEST:
        sprintf(output, "GL_NICEST");
        break;

    case GL_VERTEX_ATTRIB_ARRAY_POINTER:
        sprintf(output, "GL_VERTEX_ATTRIB_ARRAY_POINTER");
        break;

    case GL_BLEND_COLOR:
        sprintf(output, "GL_BLEND_COLOR");
        break;

    case GL_NUM_PROGRAM_BINARY_FORMATS:
        sprintf(output, "GL_NUM_PROGRAM_BINARY_FORMATS");
        break;

    case GL_VENDOR:
        sprintf(output, "GL_VENDOR");
        break;

    case GL_RENDERER:
        sprintf(output, "GL_RENDERER");
        break;

    case GL_VERSION:
        sprintf(output, "GL_VERSION");
        break;

    case GL_EXTENSIONS:
        sprintf(output, "GL_EXTENSIONS");
        break;

    case GL_FRAMEBUFFER_BINDING:
        sprintf(output, "GL_FRAMEBUFFER_BINDING");
        break;

    case GL_RENDERBUFFER_BINDING:
        sprintf(output, "GL_RENDERBUFFER_BINDING");
        break;

    case GL_IMPLEMENTATION_COLOR_READ_TYPE:
        sprintf(output, "GL_IMPLEMENTATION_COLOR_READ_TYPE");
        break;

    case GL_IMPLEMENTATION_COLOR_READ_FORMAT:
        sprintf(output, "GL_IMPLEMENTATION_COLOR_READ_FORMAT");
        break;

    case GL_MAX_COLOR_ATTACHMENTS:
        sprintf(output, "GL_MAX_COLOR_ATTACHMENTS");
        break;

    case GL_MAX_UNIFORM_BLOCK_SIZE:
        sprintf(output, "GL_MAX_UNIFORM_BLOCK_SIZE");
        break;

    case GL_MAX_VERTEX_UNIFORM_COMPONENTS:
        sprintf(output, "GL_MAX_VERTEX_UNIFORM_COMPONENTS");
        break;

    case GL_TRANSFORM_FEEDBACK:
        sprintf(output, "GL_TRANSFORM_FEEDBACK");
        break;

    default:
        sprintf(output, "Unknown name: 0x%04X", name);
        break;
    }
}

GLvoid TranslatePixelStorei(GLenum Name, GLint Param, char *output)
{
    memset(output, 0, TMP_BUF_SIZE);

    /* Check the value. */
    if ((Param != 1) && (Param != 2) && (Param != 4) && (Param != 8))
    {
        sprintf(output, "Invalid GL_INVALID_PARAM");
        return ;
    }

    /* Dispatch on name. */
    switch (Name)
    {
    case GL_PACK_ALIGNMENT:
        sprintf(output, "GL_PACK_ALIGNMENT");
        break;

    case GL_UNPACK_ALIGNMENT:
        sprintf(output, "GL_UNPACK_ALIGNMENT");
        break;

    default:
        Abort(output, "Invalid");
        break;
    }
}

GLvoid TranslateShaderDataType(GLenum type, GLchar *output)
{
    memset(output, 0, TMP_BUF_SIZE);

    switch (type)
    {
    CASE_GL(GL_FLOAT);
    CASE_GL(GL_FLOAT_VEC2);
    CASE_GL(GL_FLOAT_VEC3);
    CASE_GL(GL_FLOAT_VEC4);
    CASE_GL(GL_INT);
    CASE_GL(GL_INT_VEC2);
    CASE_GL(GL_INT_VEC3);
    CASE_GL(GL_INT_VEC4);
    CASE_GL(GL_BOOL);
    CASE_GL(GL_BOOL_VEC2);
    CASE_GL(GL_BOOL_VEC3);
    CASE_GL(GL_BOOL_VEC4);
    CASE_GL(GL_FLOAT_MAT2);
    CASE_GL(GL_FLOAT_MAT3);
    CASE_GL(GL_FLOAT_MAT4);
    CASE_GL(GL_SAMPLER_2D);
    CASE_GL(GL_SAMPLER_CUBE);

    CASE_GL(GL_SAMPLER_3D);
    CASE_GL(GL_SAMPLER_2D_SHADOW);

    default:
        Abort("%s(%d)\n", __FUNCTION__, __LINE__);
        sprintf(output, "Invalid");
        break;
    }
}


void TranslateTexTarget(GLenum target, char *output)
{
    memset(output, 0, TMP_BUF_SIZE);

    switch (target)
    {
    case GL_TEXTURE_2D:
        sprintf(output, "GL_TEXTURE_2D");
        break;

    case GL_TEXTURE_CUBE_MAP_POSITIVE_X:
        sprintf(output, "GL_TEXTURE_CUBE_MAP_POSITIVE_X");
        break;

    case GL_TEXTURE_CUBE_MAP_NEGATIVE_X:
        sprintf(output, "GL_TEXTURE_CUBE_MAP_NEGATIVE_X");
        break;

    case GL_TEXTURE_CUBE_MAP_POSITIVE_Y:
        sprintf(output, "GL_TEXTURE_CUBE_MAP_POSITIVE_Y");
        break;

    case GL_TEXTURE_CUBE_MAP_NEGATIVE_Y:
        sprintf(output, "GL_TEXTURE_CUBE_MAP_NEGATIVE_Y");
        break;

    case GL_TEXTURE_CUBE_MAP_POSITIVE_Z:
        sprintf(output, "GL_TEXTURE_CUBE_MAP_POSITIVE_Z");
        break;

    case GL_TEXTURE_CUBE_MAP_NEGATIVE_Z:
        sprintf(output, "GL_TEXTURE_CUBE_MAP_NEGATIVE_Z");
        break;

    case GL_TEXTURE_3D:
        sprintf(output, "GL_TEXTURE_3D");
        break;

    case GL_TEXTURE_2D_ARRAY:
        sprintf(output, "GL_TEXTURE_2D_ARRAY");
        break;

    case GL_TEXTURE_CUBE_MAP:
        sprintf(output, "GL_TEXTURE_CUBE_MAP");
        break;

    case GL_TEXTURE_EXTERNAL_OES:
        sprintf(output, "GL_TEXTURE_EXTERNAL_OES");
        break;

    default:
        sprintf(output, "Invalid");
        break;
    }
}

void TranslateClearMask(int mask, char *output)
{
    memset(output, 0, TMP_BUF_SIZE);

    if (mask & GL_COLOR_BUFFER_BIT)
    {
        strcat(output, "GL_COLOR_BUFFER ");
    }

    if (mask & GL_STENCIL_BUFFER_BIT)
    {
        strcat(output, "GL_STENCIL_BUFFER ");
    }

    if (mask & GL_DEPTH_BUFFER_BIT)
    {
        strcat(output, "GL_DEPTH_BUFFER");
    }
}

void TranslateTexFormat(GLenum format, char *output)
{
    memset(output, 0, TMP_BUF_SIZE);

    switch (format)
    {
    CASE_GL(GL_ALPHA);
    CASE_GL(GL_RGB);
    CASE_GL(GL_RGBA);
    CASE_GL(GL_LUMINANCE);
    CASE_GL(GL_LUMINANCE_ALPHA);
    CASE_GL(GL_ETC1_RGB8_OES);
    CASE_GL(GL_COMPRESSED_RGB_S3TC_DXT1_EXT);
    CASE_GL(GL_COMPRESSED_RGBA_S3TC_DXT1_EXT);
    CASE_GL(GL_COMPRESSED_RGBA_S3TC_DXT3_EXT);
    CASE_GL(GL_COMPRESSED_RGBA_S3TC_DXT5_EXT);
    CASE_GL(GL_DEPTH_COMPONENT);
    CASE_GL(GL_DEPTH_STENCIL);
    CASE_GL(GL_DEPTH24_STENCIL8);
    CASE_GL(GL_RGB565);
    CASE_GL(GL_RGBA4);
    CASE_GL(GL_RGB5_A1);
    CASE_GL(GL_RG);
    CASE_GL(GL_RG_INTEGER);
    CASE_GL(GL_R8);
    CASE_GL(GL_RG8);
    CASE_GL(GL_R16F);
    CASE_GL(GL_R32F);
    CASE_GL(GL_RG16F);
    CASE_GL(GL_RG32F);
    CASE_GL(GL_R8I);
    CASE_GL(GL_R8UI);
    CASE_GL(GL_R16I);
    CASE_GL(GL_R16UI);
    CASE_GL(GL_R32I);
    CASE_GL(GL_R32UI);
    CASE_GL(GL_RG8I);
    CASE_GL(GL_RG8UI);
    CASE_GL(GL_RG16I);
    CASE_GL(GL_RG16UI);
    CASE_GL(GL_RG32I);
    CASE_GL(GL_RG32UI);
    CASE_GL(GL_RGBA32F);
    CASE_GL(GL_RGB32F);
    CASE_GL(GL_RGBA16F);
    CASE_GL(GL_RGB16F);
    CASE_GL(GL_RGB8);
    CASE_GL(GL_RGBA8);
    CASE_GL(GL_RGB10_A2);
    CASE_GL(GL_RGBA32UI);
    CASE_GL(GL_RGB32UI);
    CASE_GL(GL_RGBA16UI);
    CASE_GL(GL_RGB16UI);
    CASE_GL(GL_RGBA8UI);
    CASE_GL(GL_RGB8UI);
    CASE_GL(GL_RGBA32I);
    CASE_GL(GL_RGB32I);
    CASE_GL(GL_RGBA16I);
    CASE_GL(GL_RGB16I);
    CASE_GL(GL_RGBA8I);
    CASE_GL(GL_RGB8I);
    CASE_GL(GL_RED_INTEGER);
    CASE_GL(GL_RGB_INTEGER);
    CASE_GL(GL_RGBA_INTEGER);

    CASE_GL(GL_COMPRESSED_R11_EAC);
    CASE_GL(GL_COMPRESSED_SIGNED_R11_EAC);
    CASE_GL(GL_COMPRESSED_RG11_EAC);
    CASE_GL(GL_COMPRESSED_SIGNED_RG11_EAC);
    CASE_GL(GL_COMPRESSED_RGB8_ETC2);
    CASE_GL(GL_COMPRESSED_SRGB8_ETC2);
    CASE_GL(GL_COMPRESSED_RGB8_PUNCHTHROUGH_ALPHA1_ETC2);
    CASE_GL(GL_COMPRESSED_SRGB8_PUNCHTHROUGH_ALPHA1_ETC2);
    CASE_GL(GL_COMPRESSED_RGBA8_ETC2_EAC);
    CASE_GL(GL_COMPRESSED_SRGB8_ALPHA8_ETC2_EAC);

    CASE_GL(GL_DEPTH_COMPONENT16);
    CASE_GL(GL_STENCIL_INDEX8);

    default:
        Abort("Invalid");
        sprintf(output, "Invalid");
        break;
    }
}

void TranslateInternalFormat2String(gceSURF_FORMAT InFormat, char *output)
{
    memset(output, 0, TMP_BUF_SIZE);

    /* Dispatch on format. */
    switch (InFormat)
    {
    case gcvSURF_A4:
        /* Fall through */
    case gcvSURF_A8:
        /* Fall through */
    case gcvSURF_A12:
        /* Fall through */
    case gcvSURF_A16:
        /* Fall through */
    case gcvSURF_A32:
        sprintf(output, "gcvSURF_A8");
        break;

    case gcvSURF_L4:
        /* Fall through */
    case gcvSURF_L8:
        /* Fall through */
    case gcvSURF_L12:
        /* Fall through */
    case gcvSURF_L16:
        /* Fall through */
    case gcvSURF_L32:
        sprintf(output, "gcvSURF_L8");
        break;

    case gcvSURF_A4L4:
        /* Fall through */
    case gcvSURF_A2L6:
        /* Fall through */
    case gcvSURF_A8L8:
        /* Fall through */
    case gcvSURF_A4L12:
        /* Fall through */
    case gcvSURF_A12L12:
        /* Fall through */
    case gcvSURF_A16L16:
        sprintf(output, "gcvSURF_A8L8");
        break;

    case gcvSURF_R3G3B2:
        /* Fall through */
    case gcvSURF_R5G6B5:
        /* Fall through */
    case gcvSURF_B5G6R5:
        sprintf(output, "gcvSURF_R5G6B5");
        break;

    case gcvSURF_X4R4G4B4:
        sprintf(output, "gcvSURF_X4R4G4B4");
        break;

    case gcvSURF_A2R2G2B2:
        /* Fall through */
    case gcvSURF_A4R4G4B4:
        /* Fall through */
    case gcvSURF_A4B4G4R4:
        /* Fall through */
    case gcvSURF_R4G4B4A4:
        sprintf(output, "gcvSURF_A4R4G4B4");
        break;

    case gcvSURF_X1R5G5B5:
        sprintf(output, "gcvSURF_X1R5G5B5");
        break;

    case gcvSURF_A1R5G5B5:
        /* Fall through */
    case gcvSURF_R5G5B5A1:
        /* Fall through */
    case gcvSURF_A1B5G5R5:
        sprintf(output, "gcvSURF_A1R5G5B5");
        break;

    case gcvSURF_B8G8R8:
        /* Fall through */
    case gcvSURF_R8G8B8:
        /* Fall through */
    case gcvSURF_X8R8G8B8:
        /* Fall through */
    case gcvSURF_X8B8G8R8:
        /* Fall through */
    case gcvSURF_X12R12G12B12:
        /* Fall through */
    case gcvSURF_X16R16G16B16:
        sprintf(output, "gcvSURF_X8R8G8B8");
        break;

    case gcvSURF_A8R3G3B2:
        /* Fall through */
    case gcvSURF_A8R8G8B8:
        /* Fall through */
    case gcvSURF_R8G8B8A8:
        /* Fall through */
    case gcvSURF_B8G8R8A8:
        /* Fall through */
    case gcvSURF_A8B8G8R8:
        /* Fall through */
    case gcvSURF_A12R12G12B12:
        /* Fall through */
    case gcvSURF_A16R16G16B16:
        /* Fall through */
    case gcvSURF_A16B16G16R16:
        sprintf(output, "gcvSURF_A8R8G8B8");
        break;

    case gcvSURF_A2B10G10R10:
        sprintf(output, "gcvSURF_A2B10G10R10");
        break;
    case gcvSURF_X2B10G10R10:
        sprintf(output, "gcvSURF_X2B10G10R10");
        break;
    case gcvSURF_X16B16G16R16F:
        sprintf(output, "gcvSURF_X16B16G16R16F");
        break;
    case gcvSURF_A16B16G16R16F:
        sprintf(output, "gcvSURF_A16B16G16R16F");
        break;
    case gcvSURF_A16F:
        sprintf(output, "gcvSURF_A16F");
        break;
    case gcvSURF_A32F:
        sprintf(output, "gcvSURF_A32F");
        break;
    case gcvSURF_L16F:
        sprintf(output, "gcvSURF_L16F");
        break;
    case gcvSURF_L32F:
        sprintf(output, "gcvSURF_L32F");
        break;
    case gcvSURF_A16L16F:
        sprintf(output, "gcvSURF_A16L16F");
        break;
    case gcvSURF_A32L32F:
        sprintf(output, "gcvSURF_A32L32F");
        break;

    case gcvSURF_D16:
        sprintf(output, "gcvSURF_D16");
        break;

    case gcvSURF_D24X8:
        /* Fall through */
    case gcvSURF_D32:
        sprintf(output, "gcvSURF_D24X8");
        break;

    case gcvSURF_D24S8:
        sprintf(output, "gcvSURF_D24S8");
        break;

    case gcvSURF_DXT1:
        sprintf(output, "gcvSURF_DXT1");
        break;

    case gcvSURF_DXT2:
        sprintf(output, "gcvSURF_DXT2");
        break;
    case gcvSURF_DXT3:
        sprintf(output, "gcvSURF_DXT3");
        break;
    case gcvSURF_DXT4:
        sprintf(output, "gcvSURF_DXT4");
        break;
    case gcvSURF_DXT5:
        sprintf(output, "gcvSURF_DXT5");
        break;

    case gcvSURF_ETC1:
        sprintf(output, "gcvSURF_ETC1");
        break;

    case gcvSURF_YV12:
        sprintf(output, "gcvSURF_YV12");
        break;

    case gcvSURF_YUY2:
        sprintf(output, "gcvSURF_YUY2");
        break;

    default:
        sprintf(output, "Invalid");
        break;
    }

    return ;
}

void TranslateImageFormat2TextureFormat(gceSURF_FORMAT InFormat, char *OutFormat)
{
    memset(OutFormat, 0, TMP_BUF_SIZE);

    /* Dispatch on format. */
    switch (InFormat)
    {
    case gcvSURF_A4:
        /* Fall through */
    case gcvSURF_A8:
        /* Fall through */
    case gcvSURF_A12:
        /* Fall through */
    case gcvSURF_A16:
        /* Fall through */
    case gcvSURF_A32:
        sprintf(OutFormat, "gcvSURF_A8");
        break;

    case gcvSURF_L4:
        /* Fall through */
    case gcvSURF_L8:
        /* Fall through */
    case gcvSURF_L12:
        /* Fall through */
    case gcvSURF_L16:
        /* Fall through */
    case gcvSURF_L32:
        sprintf(OutFormat, "gcvSURF_L8");
        break;

    case gcvSURF_A4L4:
        /* Fall through */
    case gcvSURF_A2L6:
        /* Fall through */
    case gcvSURF_A8L8:
        /* Fall through */
    case gcvSURF_A4L12:
        /* Fall through */
    case gcvSURF_A12L12:
        /* Fall through */
    case gcvSURF_A16L16:
        sprintf(OutFormat, "gcvSURF_A8L8");
        break;

    case gcvSURF_R3G3B2:
        /* Fall through */
    case gcvSURF_R5G6B5:
        /* Fall through */
    case gcvSURF_B5G6R5:
        sprintf(OutFormat, "gcvSURF_R5G6B5");
        break;

    case gcvSURF_X4R4G4B4:
        sprintf(OutFormat, "gcvSURF_X4R4G4B4");
        break;

    case gcvSURF_A2R2G2B2:
        /* Fall through */
    case gcvSURF_A4R4G4B4:
        /* Fall through */
    case gcvSURF_A4B4G4R4:
        /* Fall through */
    case gcvSURF_R4G4B4A4:
        sprintf(OutFormat, "gcvSURF_A4R4G4B4");
        break;

    case gcvSURF_X1R5G5B5:
        sprintf(OutFormat, "gcvSURF_X1R5G5B5");
        break;

    case gcvSURF_A1R5G5B5:
        /* Fall through */
    case gcvSURF_R5G5B5A1:
        /* Fall through */
    case gcvSURF_A1B5G5R5:
        sprintf(OutFormat, "gcvSURF_A1R5G5B5");
        break;

    case gcvSURF_B8G8R8:
        /* Fall through */
    case gcvSURF_R8G8B8:
        /* Fall through */
    case gcvSURF_X8R8G8B8:
        /* Fall through */
    case gcvSURF_X8B8G8R8:
        /* Fall through */
    case gcvSURF_X12R12G12B12:
        /* Fall through */
    case gcvSURF_X16R16G16B16:
        sprintf(OutFormat, "gcvSURF_X8R8G8B8");
        break;

    case gcvSURF_A8R3G3B2:
        /* Fall through */
    case gcvSURF_A8R8G8B8:
        /* Fall through */
    case gcvSURF_R8G8B8A8:
        /* Fall through */
    case gcvSURF_B8G8R8A8:
        /* Fall through */
    case gcvSURF_A8B8G8R8:
        /* Fall through */
    case gcvSURF_A12R12G12B12:
        /* Fall through */
    case gcvSURF_A16R16G16B16:
        /* Fall through */
    case gcvSURF_A16B16G16R16:
        sprintf(OutFormat, "gcvSURF_A8R8G8B8");
        break;

    case gcvSURF_A2B10G10R10:
        sprintf(OutFormat, "gcvSURF_A2B10G10R10");
        break;
    case gcvSURF_X2B10G10R10:
        sprintf(OutFormat, "gcvSURF_X2B10G10R10");
        break;
    case gcvSURF_X16B16G16R16F:
        sprintf(OutFormat, "gcvSURF_X16B16G16R16F");
        break;
    case gcvSURF_A16B16G16R16F:
        sprintf(OutFormat, "gcvSURF_A16B16G16R16F");
        break;
    case gcvSURF_A16F:
        sprintf(OutFormat, "gcvSURF_A16F");
        break;
    case gcvSURF_A32F:
        sprintf(OutFormat, "gcvSURF_A32F");
        break;
    case gcvSURF_L16F:
        sprintf(OutFormat, "gcvSURF_L16F");
        break;
    case gcvSURF_L32F:
        sprintf(OutFormat, "gcvSURF_L32F");
        break;
    case gcvSURF_A16L16F:
        sprintf(OutFormat, "gcvSURF_A16L16F");
        break;
    case gcvSURF_A32L32F:
        sprintf(OutFormat, "gcvSURF_A32L32F");
        break;

    case gcvSURF_D16:
        sprintf(OutFormat, "gcvSURF_D16");
        break;

    case gcvSURF_D24X8:
        /* Fall through */
    case gcvSURF_D32:
        sprintf(OutFormat, "gcvSURF_D24X8");
        break;

    case gcvSURF_D24S8:
        sprintf(OutFormat, "gcvSURF_D24S8");
        break;

    case gcvSURF_DXT1:
        sprintf(OutFormat, "gcvSURF_DXT1");
        break;

    case gcvSURF_DXT2:
        sprintf(OutFormat, "gcvSURF_DXT2");
        break;
    case gcvSURF_DXT3:
        sprintf(OutFormat, "gcvSURF_DXT3");
        break;
    case gcvSURF_DXT4:
        sprintf(OutFormat, "gcvSURF_DXT4");
        break;
    case gcvSURF_DXT5:
        sprintf(OutFormat, "gcvSURF_DXT5");
        break;

    case gcvSURF_ETC1:
        sprintf(OutFormat, "gcvSURF_ETC1");
        break;

    case gcvSURF_YV12:
        sprintf(OutFormat, "gcvSURF_YV12");
        break;

    case gcvSURF_YUY2:
        sprintf(OutFormat, "gcvSURF_YUY2");
        break;

    default:
        sprintf(OutFormat, "Invalid");
        break;
    }

    return ;
}

void TranslateDataType(GLenum type, char *output)
{
    memset(output, 0, TMP_BUF_SIZE);

    switch (type)
    {
    case GL_BYTE:
        sprintf(output, "GL_BYTE");
        break;

    case GL_UNSIGNED_BYTE:
        sprintf(output, "GL_UNSIGNED_BYTE");
        break;

    case GL_SHORT:
        sprintf(output, "GL_SHORT");
        break;

    case GL_UNSIGNED_SHORT:
        sprintf(output, "GL_UNSIGNED_SHORT");
        break;

    case GL_FIXED:
        sprintf(output, "GL_FIXED");
        break;

    case GL_FLOAT:
        sprintf(output, "GL_FLOAT");
        break;

    case GL_HALF_FLOAT_OES:
        sprintf(output, "GL_HALF_FLOAT_OES");
        break;

    case GL_UNSIGNED_INT_10_10_10_2_OES:
        sprintf(output, "GL_UINT_1010102_OES");
        break;

    case GL_INT_10_10_10_2_OES:
        sprintf(output, "GL_INT_1010102_OES");
        break;

    case GL_UNSIGNED_SHORT_5_6_5:
        sprintf(output, "GL_UNSIGNED_SHORT_5_6_5");
        break;

    case GL_UNSIGNED_SHORT_4_4_4_4:
        sprintf(output, "GL_UNSIGNED_SHORT_4_4_4_4");
        break;

    case GL_UNSIGNED_SHORT_5_5_5_1:
        sprintf(output, "GL_UNSIGNED_SHORT_5_5_5_1");
        break;

    case GL_UNSIGNED_INT:
        sprintf(output, "GL_UNSIGNED_INT");
        break;

    case GL_UNSIGNED_INT_24_8_OES:
        sprintf(output, "GL_UNSIGNED_INT_24_8_OES");
        break;

    case GL_UNSIGNED_INT_2_10_10_10_REV_EXT:
        sprintf(output, "GL_UNSIGNED_INT_2_10_10_10_REV_EXT");
        break;

    case GL_HALF_FLOAT:
        sprintf(output, "GL_HALF_FLOAT");
        break;

    case GL_INT:
        sprintf(output, "GL_INT");
        break;

    default:
        sprintf(output, "Invalid");
        break;
    }
}

void TranslateDataTypeForSaveTexture(GLenum type, char *output)
{
    memset(output, 0, TMP_BUF_SIZE);

    switch (type)
    {
    case GL_BYTE:
        sprintf(output, "GL_BYTE");
        break;

    case GL_UNSIGNED_BYTE:
        sprintf(output, "GL_UNSIGNED_BYTE");
        break;

    case GL_SHORT:
        sprintf(output, "GL_SHORT");
        break;

    case GL_UNSIGNED_SHORT:
        sprintf(output, "GL_UNSIGNED_SHORT");
        break;

    case GL_FIXED:
        sprintf(output, "GL_FIXED");
        break;

    case GL_FLOAT:
        sprintf(output, "GL_FLOAT");
        break;

    case GL_HALF_FLOAT_OES:
        sprintf(output, "GL_HALF_FLOAT_OES");
        break;

    case GL_UNSIGNED_INT_10_10_10_2_OES:
        sprintf(output, "GL_UINT_1010102_OES");
        break;

    case GL_INT_10_10_10_2_OES:
        sprintf(output, "GL_INT_1010102_OES");
        break;

    case GL_UNSIGNED_SHORT_5_6_5:
        sprintf(output, "GL_UNSIGNED_SHORT_5_6_5");
        break;

    case GL_UNSIGNED_SHORT_4_4_4_4:
        sprintf(output, "GL_UNSIGNED_SHORT_4_4_4_4");
        break;

    case GL_UNSIGNED_SHORT_5_5_5_1:
        sprintf(output, "GL_UNSIGNED_SHORT_5_5_5_1");
        break;

    default:
        sprintf(output, "Invalid");
        break;
    }
}

GLvoid TranslateGL2GCFormat(GLenum Format,
                            GLenum Type,
                            GLchar *ImageFormat,
                            GLchar *TextureFormat,
                            GLchar *Bpp)
{
    GLsizei         bpp         = 1;
    gceSURF_FORMAT  imageFormat = gcvSURF_UNKNOWN;

    if (ImageFormat)
    {
        memset(ImageFormat, 0, TMP_BUF_SIZE);
    }

    if (TextureFormat)
    {
        memset(TextureFormat, 0, TMP_BUF_SIZE);
    }

    if (Bpp)
    {
        memset(Bpp, 0, TMP_BUF_SIZE);
    }

    switch (Format)
    {
    case GL_ETC1_RGB8_OES:
        switch (Type)
        {
        case GL_UNSIGNED_BYTE:
            bpp = 24;
            imageFormat = gcvSURF_R8G8B8;

        default:
            break;
        }
        break;

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

    case GL_RGBA:
        switch (Type)
        {
        case GL_UNSIGNED_BYTE:
            bpp = 32;
            imageFormat = gcvSURF_A8B8G8R8;
            break;

        case GL_UNSIGNED_SHORT_4_4_4_4:
            bpp = 16;
            imageFormat = gcvSURF_R4G4B4A4;
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

    default:
        break;
    }

    /* Check for a valid type. */
    switch (Type)
    {
        case GL_UNSIGNED_BYTE:
        case GL_UNSIGNED_SHORT:
        case GL_UNSIGNED_INT:
        case GL_UNSIGNED_SHORT_4_4_4_4:
        case GL_UNSIGNED_SHORT_5_5_5_1:
        case GL_UNSIGNED_SHORT_5_6_5:
        case GL_UNSIGNED_INT_2_10_10_10_REV_EXT:
        case GL_UNSIGNED_INT_24_8_OES:
        case GL_HALF_FLOAT_OES:
        case GL_FLOAT:
        case GL_DEPTH_COMPONENT24_OES:
        case GL_DEPTH_COMPONENT32_OES:
            break;

        default:
            return;
    }

    switch (Format)
    {
    case GL_ETC1_RGB8_OES:
        sprintf(Bpp, "24");
        sprintf(ImageFormat, "ETC1");
        break;

    case GL_DEPTH_COMPONENT:
        switch (Type)
        {
        case GL_UNSIGNED_SHORT:
            sprintf(Bpp, "16");
            sprintf(ImageFormat, "gcvSURF_D16");
            break;

        case GL_DEPTH_COMPONENT24_OES:
            sprintf(Bpp, "32");
            sprintf(ImageFormat, "gcvSURF_D24X8");
            break;

        case GL_DEPTH_COMPONENT32_OES:
        case GL_UNSIGNED_INT:
            sprintf(Bpp, "32");
            sprintf(ImageFormat, "gcvSURF_D32");
            break;
        }
        break;

    case GL_ALPHA:
        switch (Type)
        {
        case GL_UNSIGNED_BYTE:
            sprintf(Bpp, "8");
            sprintf(ImageFormat, "gcvSURF_A8");
            break;

        case GL_UNSIGNED_SHORT:
            sprintf(Bpp, "16");
            sprintf(ImageFormat, "gcvSURF_A16");
            break;

        case GL_UNSIGNED_INT:
            sprintf(Bpp, "32");
            sprintf(ImageFormat, "gcvSURF_A32");
            break;

        case GL_HALF_FLOAT_OES:
            sprintf(Bpp, "16");
            sprintf(ImageFormat, "gcvSURF_A16F");
            break;

        case GL_FLOAT:
            sprintf(Bpp, "32");
            sprintf(ImageFormat, "gcvSURF_A32F");
            break;
        }
        break;

    case GL_RGB:
        switch (Type)
        {
        case GL_UNSIGNED_SHORT_4_4_4_4:
            sprintf(Bpp, "16");
            sprintf(ImageFormat, "gcvSURF_X4R4G4B4");
            break;

        case GL_UNSIGNED_SHORT_5_5_5_1:
            sprintf(Bpp, "16");
            sprintf(ImageFormat, "gcvSURF_X1R5G5B5");
            break;

        case GL_UNSIGNED_SHORT_5_6_5:
            sprintf(Bpp, "16");
            sprintf(ImageFormat, "gcvSURF_R5G6B5");
            break;

        case GL_UNSIGNED_BYTE:
            sprintf(Bpp, "24");
            sprintf(ImageFormat, "gcvSURF_B8G8R8");
            break;

        case GL_UNSIGNED_INT_2_10_10_10_REV_EXT:
            sprintf(Bpp, "32");
            sprintf(ImageFormat, "gcvSURF_X2B10G10R10");
            break;

        case GL_HALF_FLOAT_OES:
            sprintf(Bpp, "64");
            sprintf(ImageFormat, "gcvSURF_X16B16G16R16F");
            break;
        }
        break;

    case GL_RGBA:
        switch (Type)
        {
        case GL_UNSIGNED_BYTE:
            sprintf(Bpp, "32");
            sprintf(ImageFormat, "gcvSURF_A8B8G8R8");
            break;

        case GL_UNSIGNED_SHORT_4_4_4_4:
            sprintf(Bpp, "16");
            sprintf(ImageFormat, "gcvSURF_R4G4B4A4");
            break;

        case GL_UNSIGNED_SHORT_5_5_5_1:
            sprintf(Bpp, "16");
            sprintf(ImageFormat, "gcvSURF_R5G5B5A1");
            break;

        case GL_UNSIGNED_INT_2_10_10_10_REV_EXT:
            sprintf(Bpp, "32");
            sprintf(ImageFormat, "gcvSURF_A2B10G10R10");
            break;

        case GL_HALF_FLOAT_OES:
            sprintf(Bpp, "64");
            sprintf(ImageFormat, "gcvSURF_A16B16G16R16F");
            break;
        }
        break;

    case GL_LUMINANCE:
        switch (Type)
        {
        case GL_UNSIGNED_BYTE:
            sprintf(Bpp, "8");
            sprintf(ImageFormat, "gcvSURF_L8");
            break;

        case GL_UNSIGNED_SHORT:
            sprintf(Bpp, "16");
            sprintf(ImageFormat, "gcvSURF_L16");
            break;

        case GL_UNSIGNED_INT:
            sprintf(Bpp, "32");
            sprintf(ImageFormat, "gcvSURF_L32");
            break;

        case GL_HALF_FLOAT_OES:
            sprintf(Bpp, "16");
            sprintf(ImageFormat, "gcvSURF_L16F");
            break;

        case GL_FLOAT:
            sprintf(Bpp, "32");
            sprintf(ImageFormat, "gcvSURF_L32F");
            break;
        }
        break;

    case GL_LUMINANCE_ALPHA:
        switch (Type)
        {
        case GL_UNSIGNED_BYTE:
            sprintf(Bpp, "16");
            sprintf(ImageFormat, "gcvSURF_A8L8");
            break;

        case GL_UNSIGNED_SHORT:
            sprintf(Bpp, "32");
            sprintf(ImageFormat, "gcvSURF_A16L16");
            break;

        case GL_HALF_FLOAT_OES:
            sprintf(Bpp, "32");
            sprintf(ImageFormat, "gcvSURF_A16L16F");
            break;

        case GL_FLOAT:
            sprintf(Bpp, "64");
            sprintf(ImageFormat, "gcvSURF_A32L32F");
            break;
        }
        break;

    case GL_BGRA_EXT:
        switch (Type)
        {
        case GL_UNSIGNED_BYTE:
            sprintf(Bpp, "32");
            sprintf(ImageFormat, "gcvSURF_A8R8G8B8");
            break;
        }
        break;

    case GL_DEPTH_STENCIL_OES:
        switch (Type)
        {
        case GL_UNSIGNED_INT_24_8_OES:
            sprintf(Bpp, "32");
            sprintf(ImageFormat, "gcvSURF_D24S8");
            break;
        }
        break;

    default:
        return;
    }

    if (TextureFormat)
    {
        TranslateImageFormat2TextureFormat(imageFormat, TextureFormat);

        if (!strcmp(ImageFormat, "gcvSURF_D16"))
        {
            /* Until the hardware has a D16 decompression part.
            format = gcvSURF_D16; */
            sprintf(TextureFormat, "gcvSURF_D24X8");
        }
    }

    //if (Bpp)
    //{
    //    *Bpp = bpp;
    //}

    /* Success. */
    return ;
}

void TranslateFace(GLenum face, char *output)
{
    memset(output, 0, TMP_BUF_SIZE);

    switch(face)
    {
    case GL_FRONT:
        sprintf(output, "GL_FRONT");
        break;

    case GL_BACK:
        sprintf(output, "GL_BACK");
        break;

    case GL_FRONT_AND_BACK:
        sprintf(output, "GL_FRONT_AND_BACK");
        break;

    default:
        sprintf(output, "Invalid");
        break;
    }
}

void TranslateOperation(GLenum Operation, char *output)
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

void TranslateTexValue(GLenum value, char *output)
{
    memset(output, 0, TMP_BUF_SIZE);

    switch (value)
    {
    CASE_GL(GL_NEAREST);
    CASE_GL(GL_LINEAR);
    CASE_GL(GL_NEAREST_MIPMAP_NEAREST);
    CASE_GL(GL_NEAREST_MIPMAP_LINEAR);
    CASE_GL(GL_LINEAR_MIPMAP_NEAREST);
    CASE_GL(GL_LINEAR_MIPMAP_LINEAR);
    CASE_GL(GL_REPEAT);
    CASE_GL(GL_CLAMP_TO_EDGE);
    CASE_GL(GL_MIRRORED_REPEAT);
    CASE_GL(0);
    CASE_GL(1);
    CASE_GL(2);
    CASE_GL(3);
    CASE_GL(4);
    CASE_GL(5);
    CASE_GL(6);
    CASE_GL(7);
    CASE_GL(8);
    CASE_GL(9);

    CASE_GL(GL_COMPARE_REF_TO_TEXTURE);
    CASE_GL(GL_NEVER);
    CASE_GL(GL_LESS);
    CASE_GL(GL_EQUAL);
    CASE_GL(GL_LEQUAL);
    CASE_GL(GL_GREATER);
    CASE_GL(GL_NOTEQUAL);
    CASE_GL(GL_GEQUAL);
    CASE_GL(GL_ALWAYS);

    default:
        Abort("Invalid");
        sprintf(output, "Invalid");
        break;
    }
}

void TranslateTexParamName(GLenum param, char *output)
{
    switch (param)
    {
        CASE_GL(GL_TEXTURE_MAG_FILTER);
        CASE_GL(GL_TEXTURE_MIN_FILTER);
        CASE_GL(GL_TEXTURE_MAX_ANISOTROPY_EXT);
        CASE_GL(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT);
        CASE_GL(GL_TEXTURE_WRAP_S);
        CASE_GL(GL_TEXTURE_WRAP_T);
        CASE_GL(GL_TEXTURE_WRAP_R_OES);
        CASE_GL(GL_TEXTURE_MAX_LEVEL_APPLE);

        CASE_GL(GL_TEXTURE_COMPARE_MODE);
        CASE_GL(GL_TEXTURE_COMPARE_FUNC);

        default:
            Abort("Invalid");
            sprintf(output, "Invalid");
            break;
    }
}

void TranslateTexParameter(GLenum param, GLenum value, char *output)
{
    char str1[TMP_BUF_SIZE];

    switch (param)
    {
    case GL_TEXTURE_MAG_FILTER:
        sprintf(output, "GL_TEXTURE_MAG_FILTER, ");

        switch (value)
        {
        case GL_NEAREST:
            strcat(output, "GL_NEAREST");
            break;

        case GL_LINEAR:
            strcat(output, "GL_LINEAR");
            break;

        default:
            Abort("Invalid");
            strcat(output, "Invalid");
            break;
        }
        break;

    case GL_TEXTURE_MIN_FILTER:
        TranslateTexValue(value, str1);
        sprintf(output, "GL_TEXTURE_MIN_FILTER, %s", str1);
        break;

    case GL_TEXTURE_MAX_ANISOTROPY_EXT:
        TranslateTexValue(value, str1);
        sprintf(output, "GL_TEXTURE_MAX_ANISOTROPY_EXT, %s", str1);
        break;

    case GL_TEXTURE_WRAP_S:
        TranslateTexValue(value, str1);
        sprintf(output, "GL_TEXTURE_WRAP_S, %s", str1);
        break;

    case GL_TEXTURE_WRAP_T:
        TranslateTexValue(value, str1);
        sprintf(output, "GL_TEXTURE_WRAP_T, %s", str1);
        break;

    case GL_TEXTURE_WRAP_R_OES:
        TranslateTexValue(value, str1);
        sprintf(output, "GL_TEXTURE_WRAP_R_OES, %s", str1);
        break;

    case GL_TEXTURE_MAX_LEVEL_APPLE:
        TranslateTexValue(value, str1);
        sprintf(output, "GL_TEXTURE_MAX_LEVEL_APPLE, %d", value);
        break;

    case GL_TEXTURE_IMMUTABLE_FORMAT:
        sprintf(output, "GL_TEXTURE_IMMUTABLE_FORMAT, %d", value);
        break;

    default:
        Abort("Invalid");
        sprintf(output, "Invalid");
        break;
    }
}

void TranslateShaderType(GLenum type, char *output)
{
    memset(output, 0, TMP_BUF_SIZE);

    switch (type)
    {
    case GL_VERTEX_SHADER:
        sprintf(output, "GL_VERTEX_SHADER");
        break;

    case GL_FRAGMENT_SHADER:
        sprintf(output, "GL_FRAGMENT_SHADER");
        break;

    default:
        sprintf(output, "UNKNOWN_SHADER");
        break;
    }
}

void TranslateUsageToString(GLenum usage, char *output)
{
    memset(output, 0, TMP_BUF_SIZE);

    switch (usage)
    {
    CASE_GL(GL_STREAM_DRAW);
    CASE_GL(GL_STATIC_DRAW);
    CASE_GL(GL_DYNAMIC_DRAW);
    CASE_GL(GL_STATIC_READ);
    CASE_GL(GL_STATIC_COPY);
    CASE_GL(GL_DYNAMIC_READ);
    CASE_GL(GL_DYNAMIC_COPY);

    default:
        Abort(output, "Invalid");
        break;
    }
}

GLvoid TranslateAccessBit(GLbitfield access, GLchar *output)
{
    memset(output, 0, TMP_BUF_SIZE);

    if (access & GL_MAP_READ_BIT)
    {
        OutputStrcat(output, TMP_BUF_SIZE, "GL_MAP_READ_BIT ");
    }

    if (access & GL_MAP_WRITE_BIT)
    {
        OutputStrcat(output, TMP_BUF_SIZE, "GL_MAP_WRITE_BIT ");
    }

    if (access & GL_MAP_INVALIDATE_RANGE_BIT)
    {
        OutputStrcat(output, TMP_BUF_SIZE, "GL_MAP_INVALIDATE_RANGE_BIT ");
    }

    if (access & GL_MAP_INVALIDATE_BUFFER_BIT)
    {
        OutputStrcat(output, TMP_BUF_SIZE, "GL_MAP_INVALIDATE_BUFFER_BIT ");
    }

    if (access & GL_MAP_FLUSH_EXPLICIT_BIT)
    {
        OutputStrcat(output, TMP_BUF_SIZE, "GL_MAP_FLUSH_EXPLICIT_BIT ");
    }

    if (access & GL_MAP_UNSYNCHRONIZED_BIT)
    {
        OutputStrcat(output, TMP_BUF_SIZE, "GL_MAP_UNSYNCHRONIZED_BIT ");
    }
}

GLvoid TranslateBufferTarget(GLenum target, GLchar *output)
{
    memset(output, 0, TMP_BUF_SIZE);

    switch (target)
    {
    case GL_ARRAY_BUFFER:
        sprintf(output, "GL_ARRAY_BUFFER");
        break;

    case GL_ELEMENT_ARRAY_BUFFER:
        sprintf(output, "GL_ELEMENT_ARRAY_BUFFER");
        break;

    case GL_COPY_READ_BUFFER: // Buffer copy source
        sprintf(output, "GL_COPY_READ_BUFFER");
        break;

    case GL_COPY_WRITE_BUFFER: // Buffer copy destination
        sprintf(output, "GL_COPY_WRITE_BUFFER");
        break;

    case GL_PIXEL_PACK_BUFFER: // Pixel read target
        sprintf(output, "GL_PIXEL_PACK_BUFFER");
        break;

    case GL_PIXEL_UNPACK_BUFFER: // Texture data source
        sprintf(output, "GL_PIXEL_UNPACK_BUFFER");
        break;

    case GL_TRANSFORM_FEEDBACK_BUFFER: // Transform feedback buffer
        sprintf(output, "GL_TRANSFORM_FEEDBACK_BUFFER");
        break;

    case GL_UNIFORM_BUFFER: // Uniform block storage
        sprintf(output, "GL_UNIFORM_BUFFER");
        break;

    default:
        sprintf(output, "Invalid");
        break;
    }
}

void TranslateBlendFunc(GLenum BlendFuncGL, char *output)
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

void TranslateBlendMode(GLenum BlendModeGL, char *output)
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

void TranslateCapability(GLenum cap, char *output, GLboolean flag)
{
    memset(output, 0, TMP_BUF_SIZE);

    switch (cap)
    {
    case GL_BLEND:
        sprintf(output, "GL_BLEND");
        break;

    case GL_CULL_FACE:
        sprintf(output, "GL_CULL_FACE");
        break;

    case GL_DEPTH_TEST:
        sprintf(output, "GL_DEPTH_TEST");
        break;

    case GL_DITHER:
        sprintf(output, "GL_DITHER");
        break;

    case GL_STENCIL_TEST:
        sprintf(output, "GL_STENCIL_TEST");
        break;

    case GL_SCISSOR_TEST:
        sprintf(output, "GL_SCISSOR_TEST");
        break;

    case GL_POLYGON_OFFSET_FILL:
        sprintf(output, "GL_POLYGON_OFFSET_FILL");
        break;

    case GL_SAMPLE_ALPHA_TO_COVERAGE:
        sprintf(output, "GL_SAMPLE_ALPHA_TO_COVERAGE");
        break;

    case GL_SAMPLE_COVERAGE:
        sprintf(output, "GL_SAMPLE_COVERAGE");
        break;

    case GL_TIMESTAMP_VIV0:
    case GL_TIMESTAMP_VIV1:
    case GL_TIMESTAMP_VIV2:
    case GL_TIMESTAMP_VIV3:
    case GL_TIMESTAMP_VIV4:
    case GL_TIMESTAMP_VIV5:
    case GL_TIMESTAMP_VIV6:
    case GL_TIMESTAMP_VIV7:
        sprintf(output, "GL_TIMESTAMP_VIV");
        break;

    case GL_TEXTURE_2D:
        sprintf(output, "GL_TEXTURE_2D");
        break;

    case GL_RASTERIZER_DISCARD:
        sprintf(output, "GL_RASTERIZER_DISCARD");
        break;

#if 0
    case GL_ALPHA_TEST:
        sprintf(output, "GL_ALPHA_TEST");
        break;

    case GL_CLIP_PLANE0:
    case GL_CLIP_PLANE1:
    case GL_CLIP_PLANE2:
    case GL_CLIP_PLANE3:
    case GL_CLIP_PLANE4:
    case GL_CLIP_PLANE5:
        sprintf(output, "GL_CLIP_PLANE");
        break;

    case GL_COLOR_LOGIC_OP:
        sprintf(output, "GL_COLOR_LOGIC_OP");
        break;

    case GL_COLOR_MATERIAL:
        sprintf(output, "GL_COLOR_MATERIAL");
        break;

    case GL_FOG:
        sprintf(output, "GL_FOG");
        break;

    case GL_LIGHT0:
    case GL_LIGHT1:
    case GL_LIGHT2:
    case GL_LIGHT3:
    case GL_LIGHT4:
    case GL_LIGHT5:
    case GL_LIGHT6:
    case GL_LIGHT7:
        sprintf(output, "GL_LIGHT");
        break;

    case GL_LIGHTING:
        sprintf(output, "GL_LIGHTING");
        break;

    case GL_LINE_SMOOTH:
        sprintf(output, "GL_LINE_SMOOTH");
        break;

    case GL_MULTISAMPLE:
        sprintf(output, "GL_MULTISAMPLE");
        break;

    case GL_NORMALIZE:
        sprintf(output, "GL_NORMALIZE");
        break;

    case GL_POINT_SMOOTH:
        sprintf(output, "GL_POINT_SMOOTH");
        break;

    case GL_POINT_SPRITE_OES:
        sprintf(output, "GL_POINT_SPRITE_OES");
        break;

    case GL_RESCALE_NORMAL:
        sprintf(output, "GL_RESCALE_NORMAL");
        break;

    case GL_SAMPLE_ALPHA_TO_ONE:
        sprintf(output, "GL_SAMPLE_ALPHA_TO_ONE");
        break;
#endif

    case GL_TEXTURE_EXTERNAL_OES:
        sprintf(output, "GL_TEXTURE_EXTERNAL_OES");
        break;

    default:
        Abort("Invalid");
        break;
    }
}

void TranslateDrawMode(GLenum mode, char *output)
{
    memset(output, 0, TMP_BUF_SIZE);

    switch(mode)
    {
    case GL_POINTS:
        sprintf(output, "GL_POINTS      ");
        break;

    case GL_LINES:
        sprintf(output, "GL_LINES       ");
        break;

    case GL_LINE_LOOP:
        sprintf(output, "GL_LINE_LOOP   ");
        break;

    case GL_LINE_STRIP:
        sprintf(output, "GL_LINE_STRIP  ");
        break;

    case GL_TRIANGLES:
        sprintf(output, "GL_TRIANGLES   ");
        break;

    case GL_TRIANGLE_STRIP:
        sprintf(output, "GL_TRI_STRIP   ");
        break;

    case GL_TRIANGLE_FAN:
        sprintf(output, "GL_TRIANGLE_FAN");
        break;

    default:
        sprintf(output, "Invalid        ");
        break;
    }
}

void TranslateFunc(GLenum Func, char *output)
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

void TranslateAttribStateName(GLenum state, char *output)
{
    memset(output, 0, TMP_BUF_SIZE);

    switch (state)
    {
    case GL_VERTEX_ATTRIB_ARRAY_ENABLED:
        sprintf(output, "GL_VERTEX_ATTRIB_ARRAY_ENABLED");
        break;
    case GL_VERTEX_ATTRIB_ARRAY_SIZE:
        sprintf(output, "GL_VERTEX_ATTRIB_ARRAY_SIZE");
        break;
    case GL_VERTEX_ATTRIB_ARRAY_STRIDE:
        sprintf(output, "GL_VERTEX_ATTRIB_ARRAY_STRIDE");
        break;
    case GL_VERTEX_ATTRIB_ARRAY_TYPE:
        sprintf(output, "GL_VERTEX_ATTRIB_ARRAY_TYPE");
        break;
    case GL_VERTEX_ATTRIB_ARRAY_NORMALIZED:
        sprintf(output, "GL_VERTEX_ATTRIB_ARRAY_NORMALIZED");
        break;
    case GL_VERTEX_ATTRIB_ARRAY_BUFFER_BINDING:
        sprintf(output, "GL_VERTEX_ATTRIB_ARRAY_BUFFER_BINDING");
        break;
    case GL_CURRENT_VERTEX_ATTRIB:
        sprintf(output, "GL_CURRENT_VERTEX_ATTRIB");
        break;
    default:
        sprintf(output, "Invalid");
        break;
    }
}

void TranslateFramebufferRenderbuffer(GLenum type, char *output)
{
    memset(output, 0, TMP_BUF_SIZE);

    switch (type)
    {
    case GL_RENDERBUFFER:
        sprintf(output, "GL_RENDERBUFFER");
        break;

    case GL_FRAMEBUFFER:
        sprintf(output, "GL_FRAMEBUFFER");
        break;

    case GL_DRAW_FRAMEBUFFER:
        sprintf(output, "GL_DRAW_FRAMEBUFFER");
        break;

    case GL_READ_FRAMEBUFFER:
        sprintf(output, "GL_READ_FRAMEBUFFER");
        break;

    default:
        sprintf(output, "invalid");
        break;
    }
}

void TranslateAttachment(GLenum Attachment, char *output)
{
    memset(output, 0, TMP_BUF_SIZE);

    switch(Attachment)
    {
    case GL_COLOR_ATTACHMENT0:
        sprintf(output, "GL_COLOR_ATTACHMENT0");
        break;
    case GL_DEPTH_ATTACHMENT:
        sprintf(output, "GL_DEPTH_ATTACHMENT");
        break;
    case GL_STENCIL_ATTACHMENT:
        sprintf(output, "GL_STENCIL_ATTACHMENT");
        break;
    case GL_DEPTH:
        sprintf(output, "GL_DEPTH");
        break;
    case GL_STENCIL:
        sprintf(output, "GL_STENCIL");
        break;
    case GL_COLOR:
        sprintf(output, "GL_COLOR");
        break;
    case GL_FRONT:
        sprintf(output, "GL_FRONT");
        break;
    case GL_BACK:
        sprintf(output, "GL_BACK");
        break;
    case GL_FRONT_AND_BACK:
        sprintf(output, "GL_FRONT_AND_BACK");
        break;
    case 0:
        sprintf(output, "0");
        break;
    default:
        Abort("Invalid");
        break;
    }
}

void TranslateError(GLenum error, char *output)
{
    memset(output, 0, TMP_BUF_SIZE);

    switch(error)
    {
    case GL_NO_ERROR:
        sprintf(output, "GL_NO_ERROR");
        break;

    case GL_INVALID_ENUM:
        sprintf(output, "GL_INVALID_ENUM");
        break;

    case GL_INVALID_VALUE:
        sprintf(output, "GL_INVALID_VALUE");;
        break;

    case GL_INVALID_OPERATION:
        sprintf(output, "GL_INVALID_OPERATION");
        break;

    case GL_OUT_OF_MEMORY:
        sprintf(output, "GL_OUT_OF_MEMORY");
        break;

    default:
        Abort("Unknown");
        break;
    }
}

void TranslateShaderInfo(GLenum pname, char *output)
{
    memset(output, 0, TMP_BUF_SIZE);

    switch (pname)
    {
    case GL_ACTIVE_UNIFORMS:
        sprintf(output, "GL_ACTIVE_UNIFORMS");
        break;

    case GL_SHADER_TYPE:
        sprintf(output, "GL_SHADER_TYPE");
        break;

    case GL_DELETE_STATUS:
        sprintf(output, "GL_DELETE_STATUS");
        break;

    case GL_COMPILE_STATUS:
        sprintf(output, "GL_COMPILE_STATUS");
        break;

    case GL_INFO_LOG_LENGTH:
        sprintf(output, "GL_INFO_LOG_LENGTH");
        break;

    case GL_SHADER_SOURCE_LENGTH:
        sprintf(output, "GL_SHADER_SOURCE_LENGTH");
        break;

    default:
        sprintf(output, "Invalid");
        break;
    }
}

void TranslateProgramInfo(GLenum pname, char *output)
{
    memset(output, 0, TMP_BUF_SIZE);

    switch (pname)
    {
    case GL_DELETE_STATUS:
        sprintf(output, "GL_DELETE_STATUS");
        break;

    case GL_LINK_STATUS:
        sprintf(output, "GL_LINK_STATUS");
        break;

    case GL_INFO_LOG_LENGTH:
        sprintf(output, "GL_INFO_LOG_LENGTH");
        break;

    case GL_ACTIVE_ATTRIBUTES:
        sprintf(output, "GL_ACTIVE_ATTRIBUTES");
        break;

    case GL_ACTIVE_ATTRIBUTE_MAX_LENGTH:
        sprintf(output, "GL_ACTIVE_ATTRIBUTE_MAX_LENGTH");
        break;

    case GL_ACTIVE_UNIFORMS:
        sprintf(output, "GL_ACTIVE_UNIFORMS");
        break;

    case GL_ACTIVE_UNIFORM_MAX_LENGTH:
        sprintf(output, "GL_ACTIVE_UNIFORM_MAX_LENGTH");
        break;

    case GL_VALIDATE_STATUS:
        sprintf(output, "GL_VALIDATE_STATUS");
        break;

    case GL_ATTACHED_SHADERS:
        sprintf(output, "GL_ATTACHED_SHADERS");
        break;

    case GL_PROGRAM_BINARY_LENGTH_OES:
        sprintf(output, "GL_PROGRAM_BINARY_LENGTH_OES");
        break;

    case GL_ACTIVE_UNIFORM_BLOCKS:
        sprintf(output, "GL_ACTIVE_UNIFORM_BLOCKS");
        break;

    default:
        Abort("Invalid");
        break;
    }
}