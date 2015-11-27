#include "utils.h"
#include "GLES3DrawStates.h"
#include "ApiGLES3Context.h"

GLvoid CGLES3Context::ApiFrontFace(GLenum mode)
{
    cullFront = mode;
    CTX_ANALYZER_FUNC1(FrontFace, GLOutput, GL_OUT_BUF_SIZE, mode);
}

GLvoid CGLES3Context::ApiCullFace(GLenum mode)
{
    cullMode = mode;
    CTX_ANALYZER_FUNC1(CullFace, GLOutput, GL_OUT_BUF_SIZE, mode);
}

GLvoid CGLES3Context::ApiBlendFunc(GLenum sfactor, GLenum dfactor)
{
    blendFuncSourceRGB   = sfactor;
    blendFuncSourceAlpha = sfactor;
    blendFuncTargetRGB   = dfactor;
    blendFuncTargetAlpha = dfactor;
    CTX_ANALYZER_FUNC2(BlendFunc, GLOutput, GL_OUT_BUF_SIZE, sfactor, dfactor);
}

GLvoid CGLES3Context::ApiBlendFuncSeparate(GLenum sfactorRGB, GLenum dfactorRGB, GLenum sfactorAlpha, GLenum dfactorAlpha)
{
    blendFuncSourceRGB   = sfactorRGB;
    blendFuncSourceAlpha = sfactorAlpha;
    blendFuncTargetRGB   = dfactorRGB;
    blendFuncTargetAlpha = dfactorAlpha;
    CTX_ANALYZER_FUNC4(BlendFuncSeparate, GLOutput, GL_OUT_BUF_SIZE, sfactorRGB, dfactorRGB, sfactorAlpha, dfactorAlpha);
}

GLvoid CGLES3Context::ApiBlendColor(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha)
{
    blendColorRed   = red;
    blendColorGreen = green;
    blendColorBlue  = blue;
    blendColorAlpha = alpha;
    CTX_ANALYZER_FUNC4(BlendColor, GLOutput, GL_OUT_BUF_SIZE, red, green, blue, alpha);
}

GLvoid CGLES3Context::ApiBlendEquation(GLenum mode)
{
    blendModeAlpha = mode;
    blendModeRGB   = mode;
    CTX_ANALYZER_FUNC1(BlendEquation, GLOutput, GL_OUT_BUF_SIZE, mode);
}

GLvoid CGLES3Context::ApiBlendEquationSeparate(GLenum modeRGB, GLenum modeAlpha)
{
    blendModeRGB   = modeRGB;
    blendModeAlpha = modeAlpha;
    CTX_ANALYZER_FUNC2(BlendEquationSeparate, GLOutput, GL_OUT_BUF_SIZE, modeRGB, modeAlpha);
}

GLvoid CGLES3Context::ApiClearColor(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha)
{
    clearRed   = red;
    clearGreen = green;
    clearBlue  = blue;
    clearAlpha = alpha;
    CTX_ANALYZER_FUNC4(ClearColor, GLOutput, GL_OUT_BUF_SIZE, red, green, blue, alpha);
}

GLvoid CGLES3Context::ApiClearDepthf(GLfloat d)
{
    clearDepth = d;
    CTX_ANALYZER_FUNC1(ClearDepthf, GLOutput, GL_OUT_BUF_SIZE, d);
}

GLvoid CGLES3Context::ApiClearStencil(GLint s)
{
    clearStencil = s;
    CTX_ANALYZER_FUNC1(ClearStencil, GLOutput, GL_OUT_BUF_SIZE, s);
}

GLvoid CGLES3Context::ApiColorMask(GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha)
{
    colorRedMask   = red;
    colorGreenMask = green;
    colorBlueMask  = blue;
    colorAlphaMask = alpha;
    CTX_ANALYZER_FUNC4(ColorMask, GLOutput, GL_OUT_BUF_SIZE, red, green, blue, alpha);
}

GLvoid CGLES3Context::ApiDepthMask(GLboolean flag)
{
    depthMask = flag;
    CTX_ANALYZER_FUNC1(DepthMask, GLOutput, GL_OUT_BUF_SIZE, flag);
}

GLvoid CGLES3Context::ApiStencilMask(GLuint mask)
{
    stencilWriteMask   = mask;
    stencilMaskFront   = mask;
    stencilMaskBack    = mask;
    CTX_ANALYZER_FUNC1(StencilMask, GLOutput, GL_OUT_BUF_SIZE, mask);
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
    CTX_ANALYZER_FUNC1(Disable, GLOutput, GL_OUT_BUF_SIZE, cap);
}

GLvoid CGLES3Context::ApiEnable(GLenum cap)
{
    SetCapability(cap, GL_TRUE);
    CTX_ANALYZER_FUNC1(Enable, GLOutput, GL_OUT_BUF_SIZE, cap);
}

GLvoid CGLES3Context::ApiDepthFunc(GLenum func)
{
    depthFunc = func;
    CTX_ANALYZER_FUNC1(DepthFunc, GLOutput, GL_OUT_BUF_SIZE, func);
}

GLvoid CGLES3Context::ApiDepthRangef(GLfloat n, GLfloat f)
{
    depthNear  = n;
    depthFar   = f;
    CTX_ANALYZER_FUNC2(DepthRangef, GLOutput, GL_OUT_BUF_SIZE, n, f);
}

GLvoid CGLES3Context::ApiLineWidth(GLfloat width)
{
    lineWidth = width;
    CTX_ANALYZER_FUNC1(LineWidth, GLOutput, GL_OUT_BUF_SIZE, width);
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

    CTX_ANALYZER_FUNC2(PixelStorei, GLOutput, GL_OUT_BUF_SIZE, pname, param);
}

GLvoid CGLES3Context::ApiPolygonOffset(GLfloat factor, GLfloat units)
{
    offsetFactor   = factor;
    offsetUnits    = units;
    CTX_ANALYZER_FUNC2(PolygonOffset, GLOutput, GL_OUT_BUF_SIZE, factor, units);
}

GLvoid CGLES3Context::ApiScissor(GLint x, GLint y, GLsizei width, GLsizei height)
{
    scissorX        = x;
    scissorY        = y;
    scissorWidth    = width;
    scissorHeight   = height;
    CTX_ANALYZER_FUNC4(Scissor, GLOutput, GL_OUT_BUF_SIZE, x, y, width, height);
}

GLvoid CGLES3Context::ApiStencilFunc(GLenum func, GLint ref, GLuint mask)
{
    stencilFuncFront   = func;
    stencilFuncBack    = func;
    stencilRefFront    = ref;
    stencilRefBack     = ref;
    stencilMaskFront   = mask;
    stencilMaskBack    = mask;
    CTX_ANALYZER_FUNC3(StencilFunc, GLOutput, GL_OUT_BUF_SIZE, func, ref, mask);
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

    CTX_ANALYZER_FUNC4(StencilFuncSeparate, GLOutput, GL_OUT_BUF_SIZE, face, func, ref, mask);
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

    CTX_ANALYZER_FUNC2(StencilMaskSeparate, GLOutput, GL_OUT_BUF_SIZE, face, mask);
}

GLvoid CGLES3Context::ApiStencilOp(GLenum fail, GLenum zfail, GLenum zpass)
{
    stencilOpFailFront      = fail;
    stencilOpDepthFailFront = zfail;
    stencilOpDepthPassFront = zpass;

    stencilOpFailBack       = fail;
    stencilOpDepthFailBack  = zfail;
    stencilOpDepthPassBack  = zpass;

    CTX_ANALYZER_FUNC3(StencilOp, GLOutput, GL_OUT_BUF_SIZE, fail, zfail, zpass);
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

    CTX_ANALYZER_FUNC4(StencilOpSeparate, GLOutput, GL_OUT_BUF_SIZE, face, sfail, dpfail, dppass);
}

GLvoid CGLES3Context::ApiSampleCoverage(GLfloat value, GLboolean invert)
{
    sampleCoverageValue     = value;
    sampleCoverageInvert    = invert;
    CTX_ANALYZER_FUNC2(SampleCoverage, GLOutput, GL_OUT_BUF_SIZE, value, invert);
}

GLvoid CGLES3Context::ApiViewport(GLint x, GLint y, GLsizei width, GLsizei height)
{
    viewportX      = x;
    viewportY      = y;
    viewportWidth  = width;
    viewportHeight = height;
    CTX_ANALYZER_FUNC4(Viewport, GLOutput, GL_OUT_BUF_SIZE, x, y, width, height);
}

GLvoid CGLES3Context::ApiGetBooleanv(GLenum pname, GLboolean *data)
{
    CTX_ANALYZER_FUNC2(GetBooleanv, GLOutput, GL_OUT_BUF_SIZE, pname, data);
}

GLvoid CGLES3Context::ApiGetFloatv(GLenum pname, GLfloat *data)
{
    CTX_ANALYZER_FUNC2(GetFloatv, GLOutput, GL_OUT_BUF_SIZE, pname, data);
}

GLvoid CGLES3Context::ApiGetIntegerv(GLenum pname, GLint *data)
{
    CTX_ANALYZER_FUNC2(GetIntegerv, GLOutput, GL_OUT_BUF_SIZE, pname, data);
}

GLvoid CGLES3Context::ApiGetError(GLenum err)
{
    CTX_ANALYZER_FUNC1(GetError, GLOutput, GL_OUT_BUF_SIZE, err);
}

GLvoid CGLES3Context::ApiFinish()
{
    CTX_ANALYZER_FUNC0(Finish, GLOutput, GL_OUT_BUF_SIZE);
}

GLvoid CGLES3Context::ApiFlush()
{
    CTX_ANALYZER_FUNC0(Flush, GLOutput, GL_OUT_BUF_SIZE);
}

GLvoid CGLES3Context::ApiGetString(GLenum name, const GLubyte *str)
{
    CTX_ANALYZER_FUNC2(GetString, GLOutput, GL_OUT_BUF_SIZE, name, str);
}

GLvoid CGLES3Context::ApiIsBuffer(GLuint buffer, GLboolean res)
{
    CTX_ANALYZER_FUNC2(IsBuffer, GLOutput, GL_OUT_BUF_SIZE, buffer, res);
}

GLvoid CGLES3Context::ApiIsEnabled(GLenum cap, GLboolean res)
{
    CTX_ANALYZER_FUNC2(IsEnabled, GLOutput, GL_OUT_BUF_SIZE, cap, res);
}

GLvoid CGLES3Context::ApiIsFramebuffer(GLuint framebuffer, GLboolean res)
{
    CTX_ANALYZER_FUNC2(IsFramebuffer, GLOutput, GL_OUT_BUF_SIZE, framebuffer, res);
}

GLvoid CGLES3Context::ApiIsProgram(GLuint program, GLboolean res)
{
    CTX_ANALYZER_FUNC2(IsProgram, GLOutput, GL_OUT_BUF_SIZE, program, res);
}

GLvoid CGLES3Context::ApiIsRenderbuffer(GLuint renderbuffer, GLboolean res)
{
    CTX_ANALYZER_FUNC2(IsRenderbuffer, GLOutput, GL_OUT_BUF_SIZE, renderbuffer, res);
}

GLvoid CGLES3Context::ApiIsShader(GLuint shader, GLboolean res)
{
    CTX_ANALYZER_FUNC2(IsShader, GLOutput, GL_OUT_BUF_SIZE, shader, res);
}

GLvoid CGLES3Context::ApiIsTexture(GLuint texture, GLboolean res)
{
    CTX_ANALYZER_FUNC2(IsTexture, GLOutput, GL_OUT_BUF_SIZE, texture, res);
}

GLvoid CGLES3Context::ApiHint(GLenum target, GLenum mode)
{
    CTX_ANALYZER_FUNC2(Hint, GLOutput, GL_OUT_BUF_SIZE, target, mode);
}

GLvoid CGLES3Context::ApiReadPixels(GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, void *pixels)
{
    CTX_ANALYZER_FUNC7(ReadPixels, GLOutput, GL_OUT_BUF_SIZE, x, y, width, height, format, type, pixels);
}

GLvoid CGLES3Context::ApiReadBuffer(GLenum mode)
{
    CTX_ANALYZER_FUNC1(ReadBuffer, GLOutput, GL_OUT_BUF_SIZE, mode);
}

GLvoid CGLES3Context::ApiBlitFramebufferEXT(GLint srcX0, GLint srcY0, GLint srcX1, GLint srcY1, GLint dstX0, GLint dstY0, GLint dstX1, GLint dstY1, GLbitfield mask, GLenum filter)
{
    CTX_ANALYZER_FUNC10(BlitFramebufferEXT, GLOutput, GL_OUT_BUF_SIZE, srcX0, srcY0, srcX1, srcY1, dstX0, dstY0, dstX1, dstY1, mask, filter);
}