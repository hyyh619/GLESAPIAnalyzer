#include "utils.h"
#include "GLES3DrawStates.h"
#include "ApiGLES3Context.h"

GLvoid CGLES3Context::ApiFrontFace(GLenum mode)
{
    cullFront = mode;
}

GLvoid CGLES3Context::ApiCullFace(GLenum mode)
{
    cullMode = mode;
}

GLvoid CGLES3Context::ApiBlendFunc(GLenum sfactor, GLenum dfactor)
{
    blendFuncSourceRGB   = sfactor;
    blendFuncSourceAlpha = sfactor;
    blendFuncTargetRGB   = dfactor;
    blendFuncTargetAlpha = dfactor;
}

GLvoid CGLES3Context::ApiBlendFuncSeparate(GLenum sfactorRGB, GLenum dfactorRGB, GLenum sfactorAlpha, GLenum dfactorAlpha)
{
    blendFuncSourceRGB   = sfactorRGB;
    blendFuncSourceAlpha = sfactorAlpha;
    blendFuncTargetRGB   = dfactorRGB;
    blendFuncTargetAlpha = dfactorAlpha;
}

GLvoid CGLES3Context::ApiBlendColor(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha)
{
    blendColorRed   = red;
    blendColorGreen = green;
    blendColorBlue  = blue;
    blendColorAlpha = alpha;
}

GLvoid CGLES3Context::ApiBlendEquation(GLenum mode)
{
    blendModeAlpha = mode;
    blendModeRGB   = mode;
}

GLvoid CGLES3Context::ApiBlendEquationSeparate(GLenum modeRGB, GLenum modeAlpha)
{
    blendModeRGB   = modeRGB;
    blendModeAlpha = modeAlpha;
}

GLvoid CGLES3Context::ApiClearColor(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha)
{
    clearRed   = red;
    clearGreen = green;
    clearBlue  = blue;
    clearAlpha = alpha;
}

GLvoid CGLES3Context::ApiClearDepthf(GLfloat d)
{
    clearDepth = d;
}

GLvoid CGLES3Context::ApiClearStencil(GLint s)
{
    clearStencil = s;
}

GLvoid CGLES3Context::ApiColorMask(GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha)
{
    colorRedMask   = red;
    colorGreenMask = green;
    colorBlueMask  = blue;
    colorAlphaMask = alpha;
}

GLvoid CGLES3Context::ApiDepthMask(GLboolean flag)
{
    depthMask = flag;
}

GLvoid CGLES3Context::ApiStencilMask(GLuint mask)
{
    stencilWriteMask   = mask;
    stencilMaskFront   = mask;
    stencilMaskBack    = mask;
}

GLvoid CGLES3Context::SetCapability(GLenum cap, GLboolean flag)
{
    switch (cap)
    {
    case GL_BLEND:
        blendEnable = flag;
        break;

    case GL_CULL_FACE:
        cullEnable = flag;
        break;

    case GL_DEPTH_TEST:
        depthTest = flag;
        break;

    case GL_DITHER:
        ditherEnable = flag;
        break;

    case GL_STENCIL_TEST:
        stencilEnable = flag;
        break;

    case GL_SCISSOR_TEST:
        scissorEnable = flag;
        break;

    case GL_POLYGON_OFFSET_FILL:
        offsetEnable = flag;
        break;

    case GL_SAMPLE_ALPHA_TO_COVERAGE:
        sampleAlphaToCoverage = flag;
        break;

    case GL_SAMPLE_COVERAGE:
        sampleCoverage = flag;
        break;

    case GL_RASTERIZER_DISCARD:
        rasterDiscard = flag;
        break;

    default:
        Abort("Invalid");
        break;
    }
}

GLvoid CGLES3Context::ApiDisable(GLenum cap)
{
    SetCapability(cap, GL_FALSE);
}

GLvoid CGLES3Context::ApiEnable(GLenum cap)
{
    SetCapability(cap, GL_TRUE);
}

GLvoid CGLES3Context::ApiDepthFunc(GLenum func)
{
    depthFunc = func;
}

GLvoid CGLES3Context::ApiDepthRangef(GLfloat n, GLfloat f)
{
    depthNear  = n;
    depthFar   = f;
}

GLvoid CGLES3Context::ApiLineWidth(GLfloat width)
{
    lineWidth = width;
}

GLvoid CGLES3Context::ApiPixelStorei(GLenum pname, GLint param)
{
    /* Dispatch on name. */
    switch (pname)
    {
    case GL_PACK_ALIGNMENT:
        packAlignment = param;
        break;

    case GL_UNPACK_ALIGNMENT:
        unpackAlignment = param;
        break;

    default:
        Abort("Invalid");
        break;
    }
}

GLvoid CGLES3Context::ApiPolygonOffset(GLfloat factor, GLfloat units)
{
    offsetFactor   = factor;
    offsetUnits    = units;
}

GLvoid CGLES3Context::ApiScissor(GLint x, GLint y, GLsizei width, GLsizei height)
{
    scissorX        = x;
    scissorY        = y;
    scissorWidth    = width;
    scissorHeight   = height;
}

GLvoid CGLES3Context::ApiStencilFunc(GLenum func, GLint ref, GLuint mask)
{
    stencilFuncFront   = func;
    stencilFuncBack    = func;
    stencilRefFront    = ref;
    stencilRefBack     = ref;
    stencilMaskFront   = mask;
    stencilMaskBack    = mask;
}

GLvoid CGLES3Context::ApiStencilFuncSeparate(GLenum face, GLenum func, GLint ref, GLuint mask)
{
    if ((face == GL_FRONT) || (face == GL_FRONT_AND_BACK))
    {
        stencilFuncFront = func;
        stencilRefFront  = ref;
        stencilMaskFront = mask;
        stencilWriteMask = mask;
    }

    if ((face == GL_BACK) || (face == GL_FRONT_AND_BACK))
    {
        stencilFuncBack  = func;
        stencilRefBack   = ref;
        stencilMaskBack  = mask;
        stencilWriteMask = mask;
    }
}

GLvoid CGLES3Context::ApiStencilMaskSeparate(GLenum face, GLuint mask)
{
    if ((face == GL_FRONT) || (face == GL_FRONT_AND_BACK))
    {
        stencilWriteMask = mask;
    }

    if ((face == GL_BACK) || (face == GL_FRONT_AND_BACK))
    {
        stencilWriteMask = mask;
    }
}

GLvoid CGLES3Context::ApiStencilOp(GLenum fail, GLenum zfail, GLenum zpass)
{
    stencilOpFailFront      = fail;
    stencilOpDepthFailFront = zfail;
    stencilOpDepthPassFront = zpass;

    stencilOpFailBack       = fail;
    stencilOpDepthFailBack  = zfail;
    stencilOpDepthPassBack  = zpass;
}

GLvoid CGLES3Context::ApiStencilOpSeparate(GLenum face, GLenum sfail, GLenum dpfail, GLenum dppass)
{
    if ((face == GL_FRONT) || (face == GL_FRONT_AND_BACK))
    {
        stencilOpFailFront      = sfail;
        stencilOpDepthFailFront = dpfail;
        stencilOpDepthPassFront = dppass;
    }

    if ((face == GL_BACK) || (face == GL_FRONT_AND_BACK))
    {
        stencilOpFailBack       = sfail;
        stencilOpDepthFailBack  = dpfail;
        stencilOpDepthPassBack  = dppass;
    }
}

GLvoid CGLES3Context::ApiSampleCoverage(GLfloat value, GLboolean invert)
{
    sampleCoverageValue     = value;
    sampleCoverageInvert    = invert;
}

GLvoid CGLES3Context::ApiViewport(GLint x, GLint y, GLsizei width, GLsizei height)
{
    viewportX      = x;
    viewportY      = y;
    viewportWidth  = width;
    viewportHeight = height;
}

GLvoid CGLES3Context::ApiGetBooleanv(GLenum pname, GLboolean *data)
{
}

GLvoid CGLES3Context::ApiGetFloatv(GLenum pname, GLfloat *data)
{
}

GLvoid CGLES3Context::ApiGetIntegerv(GLenum pname, GLint *data)
{
}

GLvoid CGLES3Context::ApiGetError(GLenum err)
{
}

GLvoid CGLES3Context::ApiFinish()
{
}

GLvoid CGLES3Context::ApiFlush()
{
}

GLvoid CGLES3Context::ApiGetString(GLenum name, const GLubyte *str)
{
}

GLvoid CGLES3Context::ApiIsBuffer(GLuint buffer, GLboolean res)
{
}

GLvoid CGLES3Context::ApiIsEnabled(GLenum cap, GLboolean res)
{
}

GLvoid CGLES3Context::ApiIsFramebuffer(GLuint framebuffer, GLboolean res)
{
}

GLvoid CGLES3Context::ApiIsProgram(GLuint program, GLboolean res)
{
}

GLvoid CGLES3Context::ApiIsRenderbuffer(GLuint renderbuffer, GLboolean res)
{
}

GLvoid CGLES3Context::ApiIsShader(GLuint shader, GLboolean res)
{
}

GLvoid CGLES3Context::ApiIsTexture(GLuint texture, GLboolean res)
{
}

GLvoid CGLES3Context::ApiHint(GLenum target, GLenum mode)
{
}

GLvoid CGLES3Context::ApiReadPixels(GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, void *pixels)
{
}

GLvoid CGLES3Context::ApiReadBuffer(GLenum mode)
{
}

GLvoid CGLES3Context::ApiBlitFramebufferEXT(GLint srcX0, GLint srcY0, GLint srcX1, GLint srcY1, GLint dstX0, GLint dstY0, GLint dstX1, GLint dstY1, GLbitfield mask, GLenum filter)
{
}