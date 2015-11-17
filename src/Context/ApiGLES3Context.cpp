#include "utils.h"
#include "TranslateGL.h"
#include "MemoryPool.h"
#include "bitmap.h"
#include "ApiGLES3Context.h"
#include "GLESShare.h"
#include "ApiAnalyzer.h"
#include "ApiDebug.h"

CGLES3Context g_GLES3Context[API_RENDER_THREAD_NUM];

CGLES3Context::CGLES3Context()
{
    /* blend */
    blendFuncSourceRGB      = GL_ONE;
    blendFuncSourceAlpha    = GL_ONE;
    blendFuncTargetRGB      = GL_ZERO;
    blendFuncTargetAlpha    = GL_ZERO;
    blendModeRGB            = GL_FUNC_ADD;
    blendModeAlpha          = GL_FUNC_ADD;
    blendColorRed           = 0.0f;
    blendColorGreen         = 0.0f;
    blendColorBlue          = 0.0f;
    blendColorAlpha         = 0.0f;

    /* cull */
    cullEnable = GL_FALSE;
    cullMode   = GL_BACK;
    cullFront  = GL_CCW;

    /* depth */
    depthMask  = GL_TRUE;
    depthTest  = GL_FALSE;
    depthFunc  = GL_LESS;
    depthNear  = 0.0f;
    depthFar   = 1.0f;

    /* dither */
    ditherEnable = GL_FALSE;

    /* viewport */
    viewportX      = 0;
    viewportY      = 0;
    viewportWidth  = 0;
    viewportHeight = 0;

    /* scissor */
    scissorEnable  = GL_FALSE;
    scissorX       = 0;
    scissorY       = 0;
    scissorWidth   = 0;
    scissorHeight  = 0;

    /* stencil */
    stencilEnable           = GL_FALSE;
    stencilRefFront         = 0;
    stencilRefBack          = 0;
    stencilFuncFront        = GL_ALWAYS;
    stencilFuncBack         = GL_ALWAYS;
    stencilMaskFront        = ~0U;
    stencilMaskBack         = ~0U;
    stencilWriteMask        = ~0U;
    stencilOpFailFront      = GL_KEEP;
    stencilOpFailBack       = GL_KEEP;
    stencilOpDepthFailFront = GL_KEEP;
    stencilOpDepthFailBack  = GL_KEEP;
    stencilOpDepthPassFront = GL_KEEP;
    stencilOpDepthPassBack  = GL_KEEP;

    /* polygon offset */
    offsetEnable = GL_FALSE;
    offsetFactor = 0.0f;
    offsetUnits  = 0.0f;

    /* coverage */
    sampleAlphaToCoverage = GL_FALSE;
    sampleCoverage        = GL_FALSE;
    sampleCoverageValue   = 1.0f;
    sampleCoverageInvert  = GL_FALSE;

    /* clear value */
    clearDepth     = 1.0f;
    clearStencil   = 0;
    clearRed       = 0.0f;
    clearGreen     = 0.0f;
    clearBlue      = 0.0f;
    clearAlpha     = 0.0f;
    colorRedMask   = 1;
    colorGreenMask = 1;
    colorBlueMask  = 1;
    colorAlphaMask = 1;

    /* FBO */
    currentFBO     = 0;

    /* texture */
    activeTexUnit  = GL_TEXTURE0;

    /* vertex buffer */
    arrayBuffer         = 0;
    elementArrayBuffer  = 0;
    uniformBuffer       = 0;
    transFeedbackBuffer = 0;
    vertexArray         = 0;

    // line width
    lineWidth           = 1.0f;

    // vertex attributes' bits
    vertexAttribBits    = 0;

    unpackAlignment     = 0;
    packAlignment       = 0;

    clearBitsMask       = 0;
    nClear              = 0;
    nDrawArrays         = 0;
    nDrawElements       = 0;
    nDrawTotal          = 0;
    nFrameCount         = 0;

    drawMode            = 0;
    drawFirst           = 0;
    drawCount           = 0;
    drawIndicesType     = 0;
    drawIndices         = NULL;
}

CGLES3Context::~CGLES3Context()
{
    return;
}

GLvoid CGLES3Context::ApiClear(GLbitfield mask)
{
    nClear ++;
    clearBitsMask = mask;
}

GLvoid CGLES3Context::ApiDrawArrays(GLenum mode, GLint first, GLsizei count)
{
    drawMode    = mode;
    drawFirst   = first;
    drawCount   = count;
}

GLvoid CGLES3Context::ApiDrawElements(GLenum mode, GLsizei count, GLenum type, const GLvoid* indices)
{
    drawMode        = mode;
    drawCount       = count;
    drawIndicesType = type;
    drawIndices     = (GLvoid*)indices;
}

GLvoid CGLES3Context::Initialize(const GLchar *filePath)
{
    m_pAnalyzer = new CAnalyzer();
    m_pDumpDraw = new CApiDumpDraw(m_pAnalyzer);
    m_pDumpInfo = new CDumpInfo(API_DUMP_OUTPUT_PATH, this);

    m_pAnalyzer->InitAnalyzer(filePath, this);
}

GLvoid CGLES3Context::Release()
{
    GLuint                  j = 0;
    ShaderMap::iterator     shaderIter;
    ProgramMap::iterator    programIter;
    BufferMap::iterator     bufferIter;
    TextureMap::iterator    texIter;
    FboMap::iterator        fboIter;
    RboMap::iterator        rboIter;
    VaoMap::iterator        vaoIter;

    m_pAnalyzer->ReleaseAnalyzer();
    m_pDumpInfo->Release();

    if (m_pAnalyzer)
        delete m_pAnalyzer;

    if (m_pDumpDraw)
        delete m_pDumpDraw;

    if (m_pDumpInfo)
        delete m_pDumpInfo;

    for (shaderIter = shaderMap.begin(); shaderIter != shaderMap.end();)
    {
        CShaderObj *p = shaderIter->second;

        if (!p)
            break;

        shaderMap.erase(shaderIter++);
        delete (p);
    }

    for (programIter = programMap.begin(); programIter != programMap.end();)
    {
        CProgramObj *p          = programIter->second;
        GLuint      size        = p->m_uniforms.size();
        CUniformObj *pUniform   = NULL;

        for (j=0; j<size; j++)
        {
            pUniform = p->m_uniforms[j];
            delete (pUniform);
        }
        p->m_uniforms.clear();

        size = p->m_attributes.size();
        for (j=0; j<size; j++)
        {
            CAttribObj *pAttr = p->m_attributes[j];
            delete pAttr;
        }
        p->m_attributes.clear();

        programMap.erase(programIter++);
        delete (p);
    }

    for (bufferIter = bufferMap.begin(); bufferIter != bufferMap.end();)
    {
        CBufObj *p = bufferIter->second;
        bufferMap.erase(bufferIter++);
        delete p;
    }

    for (texIter = textureMap.begin(); texIter != textureMap.end();)
    {
        CTexObj *p = texIter->second;

        if (p)
        {
            delete p;
        }

        textureMap.erase(texIter++);
    }

    for (fboIter = fboMap.begin(); fboIter != fboMap.end();)
    {
        CFramebufferObj *p = fboIter->second;

        if (p)
        {
            delete p;
        }

        fboMap.erase(fboIter++);
    }

    for (rboIter = rboMap.begin(); rboIter != rboMap.end();)
    {
        CRenderbufferObj *p = rboIter->second;

        if (p)
        {
            delete p;
        }

        rboMap.erase(rboIter++);
    }

    for (vaoIter = vaoMap.begin(); vaoIter != vaoMap.end();)
    {
        CVertexArrayObj *p = vaoIter->second;

        if (p)
        {
            delete p;
        }

        vaoMap.erase(vaoIter++);
    }
}

GLuint CGLES3Context::GetEventSequence()
{
    return m_pAnalyzer->m_nEventSequence;
}

GLvoid CGLES3Context::DumpDraw(GLESAPIIndex name)
{
    if (IsDrawCommand(name))
    {
        m_pDumpDraw->DumpDraw(name);
    }

    m_pDumpDraw->DumpDraw(name);
}

GLboolean CGLES3Context::IsDrawCommand(GLESAPIIndex name)
{
    if(name == GL3_API_glDrawArrays ||
       name == GL3_API_glDrawElements ||
       name == GL3_API_glClear||
       name == GL3_API_glDrawArraysInstanced ||
       name == GL3_API_glDrawElementsInstanced ||
       name == GL3_API_glDrawRangeElements ||
       name == GL3_API_glMultiDrawArraysEXT ||
       name == GL3_API_glMultiDrawElementsEXT)
    {
        return GL_TRUE;
    }

    return GL_FALSE;
}

GLvoid CGLES3Context::DumpFrame()
{
    m_pDumpDraw->DumpFrame();
}