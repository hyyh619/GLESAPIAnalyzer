#include <stdio.h>
#include "ApiAnalyzer.h"
#include "ConvertApiDump.h"
#include "utils.h"
#include "ApiGLES3Context.h"
#include "ApiEGLContext.h"
#include "Analyzer.h"

using namespace std;

CApiAnalyzer::CApiAnalyzer()
{
    m_bAnalyzeApiDump = GL_FALSE;
}

GLvoid CApiAnalyzer::Release()
{
    if (!m_bAnalyzeApiDump)
        return;

    CURRENT_CONTEXT1()->Release();
}

CApiAnalyzer::~CApiAnalyzer()
{
}

#ifdef API_DUMP
CApiAnalyzer::CApiAnalyzer(GLboolean bAnalyzed, const GLchar *filePath, stCmdArgs& options) :
    m_options(options)
{
    GLchar  *p = new GLchar[256];
    if (p == NULL)
        return;

    memset(p, 0, 256);
    sprintf(p, "%s/vtracer", filePath);

    GLuint  len = strlen(p);

    while (p[--len] != '/');

    m_bAnalyzeApiDump = bAnalyzed; // GL_TRUE; // GL_FALSE;
    if (!m_bAnalyzeApiDump)
    {
        delete p;
        return;
    }

    g_Analyzer.InitAnalyzer(filePath);

    EGLInit();
    GLES3Init();

    g_opengl = (stEngine*)ConstructOpenGLEngine();

    delete p;
}
#endif

#ifdef API_DUMP
GLvoid CApiAnalyzer::SetPlayer(CPlayer *p)
{
    g_pPlayer = p;
}

GLvoid CApiAnalyzer::ParseEGLEvent(const stEvent &ev)
{
    GLchar        Line[256];
    const GLchar  *thread = "";
    const GLchar  *funcName = GetAPIName(ev.name);

    memset(Line, 0, 256);

    sprintf(Line, "${EGL %s", funcName);

    if ((ev.name == EGL_API_Index_eglCreateContext))
    {
        OutputStrcat(Line, 256, " 0x%08X 0x%08X 0x%08X (0x%08X) := 0x%08X",
            ev.args[0].val.h, ev.args[1].val.h, ev.args[2].val.h, ev.args[3].val.h, ev.args[4].val.h);
    }
    else if ((ev.name == EGL_API_Index_eglCreateWindowSurface))
    {
        int width  = 0;
        int height = 0;

        OutputStrcat(Line, 256, " 0x%08X 0x%08X 0x%08X (0x%08X) := 0x%08X (%dx%d)",
            ev.args[0].val.h, ev.args[1].val.h, ev.args[2].val.h, ev.args[3].val.h, ev.args[4].val.h, width, height);
    }
    else if ((ev.name == EGL_API_Index_eglGetDisplay))
    {
        OutputStrcat(Line, 256, " 0x%08X := 0x%08X}", ev.args[0].val.h, ev.args[1].val.h);
    }
    else if ((ev.name == EGL_API_Index_eglQueryString))
    {
        OutputStrcat(Line, 256, " 0x%08X 0x%08X := 0x%08X", ev.args[0].val.h, ev.args[1].val.h, ev.args[2].val.h);
    }
    else if ((ev.name == EGL_API_Index_eglQuerySurface))
    {
        OutputStrcat(Line, 256, " 0x%08X 0x%08X 0x%08X := 0x%08X}",
            ev.args[0].val.h, ev.args[1].val.h, ev.args[2].val.h, ev.args[3].val.h);
    }
    else if ((ev.name == EGL_API_Index_eglInitialize))
    {
        OutputStrcat(Line, 256, " 0x%08X}", ev.args[0].val.h);
    }
    else if ((ev.name == EGL_API_Index_eglChooseConfig))
    {
        OutputStrcat(Line, 256, " 0x%08X (0x%08X) (0x%08X) 0x%08X (0x%08X)",
            ev.args[0].val.h, ev.args[1].val.h, ev.args[2].val.h, ev.args[3].val.h, ev.args[4].val.h);
    }
    else
    {
        for (GLuint i=0; i<ev.argNum; i++)
        {
            OutputStrcat(Line, 256, " 0x%08X", ev.args[i].val.h);
        }
        OutputStrcat(Line, 256, "}");
    }

    if (strstr(Line, "eglSwapBuffers"))
    {
        // Output eglSwapBuffers
        //{
        //    GLchar buf[256];
        //    memset(buf, 0, 256);
        //    sprintf(buf, "eglSwapBuffers(display=0x%08X, draw=0x%08X) Frames: %04d ********************", ev.args[0].val.h, ev.args[1].val.h, CURRENT_EGL_CONTEXT()->nFrame);
        //    OutputToTarget(OUTPUT_TO_TOTAL_STRING, GL_TRUE, g_Analyzer.GetEventSequence(), thread, buf, g_pOutputFile);
        //}
        eglDisassemble(Line, Output, OUT_BUF_SIZE);

        //if (!OutputToTarget(OUTPUT_TO_FILE, GL_FALSE, g_Analyzer.GetEventSequence(), NULL, Output, g_pFrameFile))
        //{
        //    printf("#%d-EGL: frame file failed\n", nLineNum);
        //    return;
        //}
    
        //if (!OutputToTarget(OUTPUT_TO_TOTAL_STRING, GL_FALSE, g_Analyzer.GetEventSequence(), NULL, Output, g_pOutputFile))
        //{
        //    printf("#%d-EGL: output file failed\n", nLineNum);
        //    return;
        //}
    }
    else
    {
        eglDisassemble(Line, EGLOutput, EGL_OUT_BUF_SIZE);
    }
    //else if (strstr(Line, "veglGetThreadData"))
    //{
    //    OutputToTarget(OUTPUT_TO_FILE, GL_TRUE, g_Analyzer.GetEventSequence(), thread, Line, g_pOutputFile);
    //}
}

void CApiAnalyzer::ParseES3Event(const stEvent &ev)
{
    GLchar        Line[256];
    const GLchar  *thread = "";
    const GLchar  *funcName = GetAPIName(ev.name);

    memset(Line, 0, 256);

    sprintf(Line, "${ES20 %s", funcName);

    if ((ev.name == GL3_API_Index_glCreateProgram))
    {
        OutputStrcat(Line, 256, " := 0x%08X}", ev.args[0].val.h);
    }
    else if ((ev.name == GL3_API_Index_glGetString) ||
             (ev.name == GL3_API_Index_glCreateShader) ||
             (ev.name == GL3_API_Index_glCheckFramebufferStatus) ||
             (ev.name == GL3_API_Index_glIsBuffer) ||
             (ev.name == GL3_API_Index_glIsShader) ||
             (ev.name == GL3_API_Index_glIsProgram) ||
             (ev.name == GL3_API_Index_glIsRenderbuffer) ||
             (ev.name == GL3_API_Index_glIsEnabled) ||
             (ev.name == GL3_API_Index_glIsTexture) ||
             (ev.name == GL3_API_Index_glIsFramebuffer))
    {
        OutputStrcat(Line, 256, " 0x%08X := 0x%08X", ev.args[0].val.h, ev.args[1].val.h);
    }
    else if ((ev.name == GL3_API_Index_glGetIntegerv) ||
             (ev.name == GL3_API_Index_glGetFloatv) ||
             (ev.name == GL3_API_Index_glGetBooleanv) ||
             (ev.name == GL3_API_Index_glDeleteTextures) ||
             (ev.name == GL3_API_Index_glDeleteBuffers) ||
             (ev.name == GL3_API_Index_glDeleteRenderbuffers) ||
             (ev.name == GL3_API_Index_glDeleteFramebuffers) ||
             (ev.name == GL3_API_Index_glDeleteVertexArrays) ||
             (ev.name == GL3_API_Index_glDeleteTransformFeedbacks) ||
             (ev.name == GL3_API_Index_glGenTextures) ||
             (ev.name == GL3_API_Index_glGenFramebuffers) ||
             (ev.name == GL3_API_Index_glGenRenderbuffers) ||
             (ev.name == GL3_API_Index_glGenBuffers) ||
             (ev.name == GL3_API_Index_glGenVertexArrays) ||
             (ev.name == GL3_API_Index_glGenTransformFeedbacks) ||
             (ev.name == GL3_API_Index_glVertexAttrib1fv) ||
             (ev.name == GL3_API_Index_glDrawBuffers))
    {
        OutputStrcat(Line, 256, " 0x%08X (0x%08X)", ev.args[0].val.h, ev.args[1].val.h);
    }
    else if ((ev.name == GL3_API_Index_glGetShaderiv) ||
             (ev.name == GL3_API_Index_glGetProgramiv) ||
             (ev.name == GL3_API_Index_glUniform4fv) ||
             (ev.name == GL3_API_Index_glUniform3fv) ||
             (ev.name == GL3_API_Index_glUniform2fv) ||
             (ev.name == GL3_API_Index_glUniform1fv) ||
             (ev.name == GL3_API_Index_glUniform4iv) ||
             (ev.name == GL3_API_Index_glUniform3iv) ||
             (ev.name == GL3_API_Index_glUniform2iv) ||
             (ev.name == GL3_API_Index_glUniform1iv) ||
             (ev.name == GL3_API_Index_glBindAttribLocation) ||
             (ev.name == GL3_API_Index_glGetVertexAttribfv) ||
             (ev.name == GL3_API_Index_glGetVertexAttribiv) ||
             (ev.name == GL3_API_Index_glDiscardFramebufferEXT) ||
             (ev.name == GL3_API_Index_glGetVertexAttribPointerv) ||
             (ev.name == GL3_API_Index_glGetRenderbufferParameteriv) ||
             (ev.name == GL3_API_Index_glGetTexParameteriv) ||
             (ev.name == GL3_API_Index_glGetTexParameterfv) ||
             (ev.name == GL3_API_Index_glGetUniformfv) ||
             (ev.name == GL3_API_Index_glGetUniformiv) ||
             (ev.name == GL3_API_Index_glTexParameterfv) ||
             (ev.name == GL3_API_Index_glInvalidateFramebuffer))
    {
        OutputStrcat(Line, 256, " 0x%08X 0x%08X (0x%08X)", 
            ev.args[0].val.h, ev.args[1].val.h, ev.args[2].val.h);
    }
    else if ((ev.name == GL3_API_Index_glGetAttribLocation) ||
             (ev.name == GL3_API_Index_glGetUniformLocation))
    {
        OutputStrcat(Line, 256, " 0x%08X (0x%08X) := 0x%08X", 
            ev.args[0].val.h, ev.args[1].val.h, ev.args[2].val.h);
    }
    else if ((ev.name == GL3_API_Index_glGetProgramInfoLog) ||
             (ev.name == GL3_API_Index_glGetShaderInfoLog) ||
             (ev.name == GL3_API_Index_glShaderSource) ||
             (ev.name == GL3_API_Index_glGetShaderSource) ||
             (ev.name == GL3_API_Index_glGetAttachedShaders))
    {
        OutputStrcat(Line, 256, " 0x%08X 0x%08X (0x%08X) (0x%08X)",
            ev.args[0].val.h, ev.args[1].val.h, ev.args[2].val.h, ev.args[3].val.h);
    }
    else if ((ev.name == GL3_API_Index_glBufferData) ||
             (ev.name == GL3_API_Index_glProgramBinaryOES) ||
             (ev.name == GL3_API_Index_glProgramBinary))
    {
        OutputStrcat(Line, 256, " 0x%08X 0x%08X (0x%08X) 0x%08X",
            ev.args[0].val.h, ev.args[1].val.h, ev.args[2].val.h, ev.args[3].val.h);
    }
    else if ((ev.name == GL3_API_Index_glDrawElements) ||
             (ev.name == GL3_API_Index_glBufferSubData) ||
             (ev.name == GL3_API_Index_glUniformMatrix4fv) ||
             (ev.name == GL3_API_Index_glUniformMatrix3fv) ||
             (ev.name == GL3_API_Index_glUniformMatrix2fv) ||
             (ev.name == GL3_API_Index_glGetFramebufferAttachmentParameteriv))
    {
        OutputStrcat(Line, 256, " 0x%08X 0x%08X 0x%08X (0x%08X)",
            ev.args[0].val.h, ev.args[1].val.h, ev.args[2].val.h, ev.args[3].val.h);
    }
    else if ((ev.name == GL3_API_Index_glMapBufferRange))
    {
        OutputStrcat(Line, 256, " 0x%08X 0x%08X 0x%08X 0x%08X := 0x%08X", 
            ev.args[0].val.h, ev.args[1].val.h, ev.args[2].val.h, ev.args[3].val.h, ev.args[4].val.h);
    }
    else if ((ev.name == GL3_API_Index_glGetActiveAttrib) ||
             (ev.name == GL3_API_Index_glGetActiveUniform))
    {
        OutputStrcat(Line, 256, " 0x%08X 0x%08X 0x%08X (0x%08X) (0x%08X) (0x%08X) (0x%08X)",
            ev.args[0].val.h, ev.args[1].val.h, ev.args[2].val.h, ev.args[3].val.h,
            ev.args[4].val.h, ev.args[5].val.h, ev.args[6].val.h);
    }
    else if ((ev.name == GL3_API_Index_glReadPixels))
    {
        OutputStrcat(Line, 256, " 0x%08X 0x%08X 0x%08X 0x%08X 0x%08X 0x%08X (0x%08X)",
            ev.args[0].val.h, ev.args[1].val.h, ev.args[2].val.h, ev.args[3].val.h,
            ev.args[4].val.h, ev.args[5].val.h, ev.args[6].val.h);
    }
    else if ((ev.name == GL3_API_Index_glCompressedTexImage2D))
    {
        OutputStrcat(Line, 256, " 0x%08X 0x%08X 0x%08X 0x%08X 0x%08X 0x%08X 0x%08X (0x%08X)",
            ev.args[0].val.h, ev.args[1].val.h, ev.args[2].val.h, ev.args[3].val.h,
            ev.args[4].val.h, ev.args[5].val.h, ev.args[6].val.h, ev.args[7].val.h);
    }
    else if ((ev.name == GL3_API_Index_glTexImage2D) ||
             (ev.name == GL3_API_Index_glTexSubImage2D) ||
             (ev.name == GL3_API_Index_glCompressedTexSubImage2D))
    {
        OutputStrcat(Line, 256, " 0x%08X 0x%08X 0x%08X 0x%08X 0x%08X 0x%08X 0x%08X 0x%08X (0x%08X)",
            ev.args[0].val.h, ev.args[1].val.h, ev.args[2].val.h, ev.args[3].val.h,
            ev.args[4].val.h, ev.args[5].val.h, ev.args[6].val.h, ev.args[7].val.h, ev.args[8].val.h);
    }
    else if ((ev.name == GL3_API_Index_glTexImage3D))
    {
        OutputStrcat(Line, 256, " 0x%08X 0x%08X 0x%08X 0x%08X 0x%08X 0x%08X 0x%08X 0x%08X 0x%08X (0x%08X)",
            ev.args[0].val.h, ev.args[1].val.h, ev.args[2].val.h, ev.args[3].val.h, ev.args[4].val.h,
            ev.args[5].val.h, ev.args[6].val.h, ev.args[7].val.h, ev.args[8].val.h, ev.args[9].val.h);
    }
    else if ((ev.name == GL3_API_Index_glTexSubImage3D))
    {
        OutputStrcat(Line, 256, " 0x%08X 0x%08X 0x%08X 0x%08X 0x%08X 0x%08X 0x%08X 0x%08X 0x%08X 0x%08X (0x%08X)",
            ev.args[0].val.h, ev.args[1].val.h, ev.args[2].val.h, ev.args[3].val.h, ev.args[4].val.h,
            ev.args[5].val.h, ev.args[6].val.h, ev.args[7].val.h, ev.args[8].val.h, ev.args[9].val.h, ev.args[10].val.h);
    }
    else if ((ev.name == GL3_API_Index_glVertexAttrib4fv) ||
             (ev.name == GL3_API_Index_glVertexAttrib3fv) ||
             (ev.name == GL3_API_Index_glVertexAttrib2fv) ||
             (ev.name == GL3_API_Index_glVertexAttrib1fv))
    {
        OutputStrcat(Line, 256, " 0x%08X (0x%08X)", ev.args[0].val.h, ev.args[1].val.h);
    }
    else
    {
        for (GLuint i=0; i<ev.argNum; i++)
        {
            OutputStrcat(Line, 256, " 0x%08X", ev.args[i].val.h);
        }
        OutputStrcat(Line, 256, "}");
    }

    if (!strncmp(Line, "${ES20 ", 7))
    {
        if ((strstr(Line, "glDrawArrays")) ||
            (strstr(Line, "glDrawElements")))
        {
            // Output draw
            //{
            //    GLchar buf[256];
            //    memset(buf, 0, 256);
            //    sprintf(buf, "%s %08d", Line, CURRENT_CONTEXT1()->nDrawTotal+1);
            //    OutputToTarget(OUTPUT_TO_FILE, GL_TRUE, g_Analyzer.GetEventSequence(), thread, buf, g_pOutputFile);
            //}

            GLES3Disassemble(&Line[9], Draw, DRAW_BUF_SIZE);
        }
        else if (strstr(Line, "glShaderSource"))
        {
            //OutputToTarget(OUTPUT_TO_FILE, GL_TRUE, g_Analyzer.GetEventSequence(), thread, Line, g_pOutputFile);
            GLES3Disassemble(&Line[9], Uniform, UNIFORM_BUF_SIZE);
        }
        else if ((strstr(Line, "glUniformMatrix3fv"))
                 || (strstr(Line, "glUniformMatrix4fv"))
                 || (strstr(Line, "glUniform1fv"))
                 || (strstr(Line, "glUniform2fv"))
                 || (strstr(Line, "glUniform4fv"))
                 || (strstr(Line, "glUniform3fv"))
                 || (strstr(Line, "glGetString")))
        {
            GLES3Disassemble(&Line[9], Uniform, UNIFORM_BUF_SIZE);
    
            //if (!OutputToTarget(OUTPUT_TO_TOTAL_STRING, GL_TRUE, g_Analyzer.GetEventSequence(), thread, Uniform, g_pOutputFile))
            //{
            //    printf("#%d-ES20: output file failed\n", nLineNum);
            //    return;
            //}
        }
        else
        {
            GLES3Disassemble(&Line[9], GLOutput, GL_OUT_BUF_SIZE);

            //if (*GLOutput != '\0')
            //{
            //    if (!OutputToTarget(OUTPUT_TO_TOTAL_STRING, GL_TRUE, g_Analyzer.GetEventSequence(), thread, GLOutput, g_pOutputFile))
            //    {
            //        printf("#%d-ES20: output file failed\n", nLineNum);
            //        return;
            //    }
            //}
        }
    }

#if 0
    OutputToTarget(OUTPUT_TO_TOTAL_STRING, GL_TRUE, m_nEventSequence, thread, funcName, g_pOutputFile);
    OutputToTarget(OUTPUT_TO_TOTAL_STRING, GL_TRUE, m_nEventSequence, thread, "\n", g_pOutputFile);
#endif
}

GLvoid CApiAnalyzer::BeginEvent(const stEvent& ev)
{
    UpdateContext(ev);

    PEglThreadData  thread  = _GetThreadData();
    CGLES3Context   *pCtx   = thread->context?thread->context->pGLESContext:NULL;

    if (pCtx)
    {
        pCtx->m_pDumpInfo->BeginEvent((GLESAPIIndex)ev.name);

        if (ev.name == EGL_API_eglSwapBuffers)
        {
            CURRENT_CONTEXT1()->DumpFrame();
        }
    }

    if (m_bAnalyzeApiDump == GL_FALSE)
    {
        return;
    }

    if (ev.name >= EGL_API_Index_eglGetError && ev.name <= EGL_API_Index_eglPatchID) // api: 0 egl path
    {
        ParseEGLEvent(ev);
    }
    else if (ev.name >= GL3_API_Index_glVertexAttribPointer && ev.name <= GL3_API_Index_glVertexBindingDivisor) // gles2 path
    {
        ParseES3Event(ev);
    }
    else // gles1 path
    {
    }
}

GLvoid CApiAnalyzer::EndEvent(const stEvent& ev)
{
    PEglThreadData  thread  = _GetThreadData();
    CGLES3Context   *pCtx   = thread->context?thread->context->pGLESContext:NULL;

    if (pCtx)
    {
        pCtx->m_pDumpInfo->EndEvent((GLESAPIIndex)ev.name);
        pCtx->DumpDraw((GLESAPIIndex)ev.name);
    }

    if (m_bAnalyzeApiDump == GL_FALSE)
    {
        return;
    }
}

GLvoid* GetCurrentDataPtr(GLuint handle)
{
    CApiArray   *it = g_pPlayer->FindArray(handle);
    GLvoid      *p  = it?(EGLint*)it->GetCurrentDataPointer():NULL;
    return p;
}

GLvoid  CApiAnalyzer::ShaderSource(GLuint shaderIndex, GLsizei count, GLuint stringAddr, GLuint lenAddr)
{
#ifdef VTRACER_USE
    GLchar  **string    = NULL;
    GLchar  **strArray  = NULL;
    GLint   *length     = NULL;
    GLsizei i           = 0;
    GLsizei n           = 0;
    GLchar  *shader     = NULL;
    GLuint  total       = 1;

    strArray = (GLchar**)malloc(count*4);memset(strArray, 0, count*4);
    string   = (GLchar**)ReadData(READ_GL_UINT, stringAddr, count);
    length   = (GLint*)ReadData(READ_GL_UINT, lenAddr, count);

    if (string == NULL)
    {
        return;
    }

    for (i = 0; i < count; ++i)
    {
        if ((length != NULL) && (length[i] > 0))
        {
            total += length[i];
            n = length[i];
            strArray[i] = (GLchar*)ReadData(READ_GL_UCHAR, (GLuint)string[i], n);
        }
        else
        {
            strArray[i] = (GLchar*)ReadData(READ_GL_UCHAR, (GLuint)string[i], n);
            total += strlen(strArray[i]);
        }
    }

    shader = (GLchar*)malloc(total);
    memset(shader, 0, total);

    total = 0;
    for (i = 0; i < count; ++i)
    {
        if ((length != NULL) && (length[i] > 0))
        {
            n = length[i];
        }
        else if (strArray[i])
        {
            n = STRLEN(strArray[i]);
        }

        if (strArray[i] == NULL)
        {
            continue;
        }

        memcpy(shader+total, strArray[i], n);
        total += n;
        FreeData(strArray[i]);
    }

    CURRENT_CONTEXT1()->ApiShaderSource(shaderIndex, 1, &shader, NULL);
#elif defined(TCG_USE)
    GLchar  *string = NULL;
    GLint   *length = NULL;
    GLsizei i       = 0;
    GLsizei n       = 0;
    GLchar  *shader = NULL;
    GLchar  *tmp    = NULL;
    GLuint  len     = 0;

    string  = (GLchar*)ReadData(READ_GL_UINT, stringAddr, count);
    length  = (GLint*)ReadData(READ_GL_UINT, lenAddr, count);
    len     = strlen(string)+1;

    shader = (GLchar**)malloc(sizeof(GLchar*));
    shader->shaderSource[0] = (GLchar*)malloc(len);
    shader->shaderLength    = NULL;
    shader->shaderCount     = 1;

    //memset(shader->shaderLength, 0, sizeof(GLint)*count);
    memset(shader->shaderSource[0], 0, len);
    memcpy(shader->shaderSource[0], string, len-1);

    if (string == NULL)
    {
        return;
    }

    sprintf(output, "\n*************************************shader*************************************\n");
    OutputStrcat(output, outputSize, "glShaderSource(shader = %08d)\n\n", shaderIndex);
    OutputToTarget(OUTPUT_TO_FILE, GL_FALSE, 0, NULL, output, g_pOutputFile);
    OutputToShaderFile(output);

    OutputToTarget(OUTPUT_TO_FILE, GL_FALSE, 0, NULL, shader->shaderSource[0], g_pOutputFile);
    OutputToShaderFile(shader->shaderSource[0]);

    OutputStrcat(output, outputSize, "*************************************shader end*********************************\n\n");
    OutputToTarget(OUTPUT_TO_FILE, GL_FALSE, 0, NULL, output, g_pOutputFile);
    OutputToShaderFile(output);

    FreeData(string);
    FreeData(length);
#endif
}

GLvoid CApiAnalyzer::UpdateContext(const stEvent &ev)
{
    switch (ev.name)
    {
    case EGL_API_eglGetDisplay:
        {
            EGLNativeDisplayType display_id = (EGLNativeDisplayType)ev.args[0].val.i;
            EGLDisplay           dpy        = (EGLDisplay)ev.args[1].val.i;
            ApiGetDisplay(display_id, dpy);
        }
        break;

    case EGL_API_eglInitialize:
        break;

    case EGL_API_eglCreateContext:
        {
            EGLDisplay  display = (EGLDisplay)ev.args[0].val.i;
            EGLConfig   config  = (EGLConfig)ev.args[1].val.i;
            EGLContext  shCtx   = (EGLContext)ev.args[2].val.i;
            EGLint      *pAttr  = (EGLint*)GetCurrentDataPtr(ev.args[3].val.h);
            EGLContext  ret     = (EGLContext)ev.args[4].val.i;
            ApiCreateContext(display, config, shCtx, pAttr, ret);
        }
        break;

    case EGL_API_eglCreateWindowSurface:
        {
            EGLDisplay          display = (EGLDisplay)ev.args[0].val.i;
            EGLConfig           config  = (EGLConfig)ev.args[1].val.i;
            EGLNativeWindowType wndType = (EGLNativeWindowType)ev.args[2].val.i;
            EGLint              *pAttr  = (EGLint*)GetCurrentDataPtr(ev.args[3].val.h);
            EGLSurface          ret     = (EGLSurface)ev.args[4].val.i;
            ApiCreateWindowSurface(display, config, wndType, pAttr, ret);
        }
        break;

    case EGL_API_eglCreatePbufferSurface:
        {
            EGLDisplay          display = (EGLDisplay)ev.args[0].val.i;
            EGLConfig           config  = (EGLConfig)ev.args[1].val.i;
            EGLint              *pAttr  = (EGLint*)GetCurrentDataPtr(ev.args[2].val.h);
            EGLSurface          ret     = (EGLSurface)ev.args[3].val.i;
            ApiCreatePbufferSurface(display, config, pAttr, ret);
        }
        break;

    case EGL_API_eglCreatePixmapSurface:
        {
            EGLDisplay          display = (EGLDisplay)ev.args[0].val.i;
            EGLConfig           config  = (EGLConfig)ev.args[1].val.i;
            EGLNativePixmapType wndType = (EGLNativePixmapType)ev.args[2].val.i;
            EGLint              *pAttr  = (EGLint*)GetCurrentDataPtr(ev.args[3].val.h);
            EGLSurface          ret     = (EGLSurface)ev.args[4].val.i;
            ApiCreatePixmapSurface(display, config, wndType, pAttr, ret);
        }
        break;

    case EGL_API_eglDestroySurface:
        {
            EGLDisplay  display = (EGLDisplay)ev.args[0].val.i;
            EGLSurface  surf    = (EGLSurface)ev.args[1].val.i;
            EGLBoolean  ret     = (EGLBoolean)ev.args[2].val.i;
            ApiDestroySurface(display, surf, ret);
        }
        break;

    case EGL_API_eglDestroyContext:
        {
            EGLDisplay  display = (EGLDisplay)ev.args[0].val.i;
            EGLContext  ctx     = (EGLContext)ev.args[1].val.i;
            EGLBoolean  ret     = (EGLBoolean)ev.args[2].val.i;
            ApiDestroyContext(display, ctx, ret);
        }
        break;

    case EGL_API_eglMakeCurrent:
        {
            EGLDisplay  display = (EGLDisplay)ev.args[0].val.i;
            EGLSurface  draw    = (EGLSurface)ev.args[1].val.i;
            EGLSurface  read    = (EGLSurface)ev.args[2].val.i;
            EGLContext  ctx     = (EGLContext)ev.args[3].val.i;
            EGLBoolean  ret     = (EGLBoolean)ev.args[4].val.i;
            ApiMakeCurrent(display, draw, read, ctx, ret);
        }
        break;

    case EGL_API_eglChooseConfig:
        {
            EGLDisplay  display     = (EGLDisplay)ev.args[0].val.i;
            EGLint      *attribList = (EGLint*)ReadData(READ_GL_UCHAR, ev.args[1].val.i, 0);
            EGLConfig   *configs    = (EGLConfig*)ReadData(READ_GL_UCHAR, ev.args[2].val.i, 0);
            EGLint      confSize    = (EGLint)ev.args[3].val.i;
            EGLint      *configNum  = (EGLint*)ReadData(READ_GL_UCHAR, ev.args[4].val.i, 0);
            EGLBoolean  ret         = ev.args[5].val.i;
            ApiChooseConfig(display, attribList, configs, confSize, configNum, ret);
        }
        break;

    case EGL_API_eglGetError:
        ApiEglGetError(ev.args[0].val.i);
        break;

    case EGL_API_eglGetProcAddress:
        break;

    case GL3_API_glDrawArrays:
        CURRENT_CONTEXT1()->ApiDrawArrays(ev.args[0].val.i, ev.args[1].val.i, ev.args[2].val.i);
        break;

    case GL3_API_glDrawElements:
        {
            GLenum          mode    = ev.args[0].val.i;
            GLsizei         count   = ev.args[1].val.i;
            GLenum          type    = ev.args[2].val.i;
            GLuint          handle  = ev.args[3].val.h;
            const GLvoid*   indices = NULL;

            if (!CURRENT_CONTEXT1()->elementArrayBuffer)
            {
                GLvoid *pData = GetCurrentDataPtr(handle);
                indices = pData;
            }
            else
            {
                CBufObj *p = CURRENT_CONTEXT1()->FindBufObj(CURRENT_CONTEXT1()->elementArrayBuffer);
                indices = (const GLvoid*)((GLuint)p->GetCurrentDataPointer() + handle);
            }

            CURRENT_CONTEXT1()->ApiDrawElements(mode, count, type, indices);
        }
        break;

    case GL3_API_glActiveTexture:
        CURRENT_CONTEXT1()->ApiActiveTexture(ev.args[0].val.i);
        break;

    case GL3_API_glAttachShader:
        CURRENT_CONTEXT1()->ApiAttachShader(ev.args[0].val.i, ev.args[1].val.i);
        break;

    case GL3_API_glBindTexture:
        CURRENT_CONTEXT1()->ApiBindTexture(ev.args[0].val.i, ev.args[1].val.i);
        break;

    case GL3_API_glBindBuffer:
        CURRENT_CONTEXT1()->ApiBindBuffer(ev.args[0].val.i, ev.args[1].val.i);
        break;

    case GL3_API_glBindBufferBase:
        CURRENT_CONTEXT1()->ApiBindBufferBase(ev.args[0].val.i, ev.args[1].val.i, ev.args[2].val.i);
        break;

    case GL3_API_glBindBufferRange:
        CURRENT_CONTEXT1()->ApiBindBufferRange(ev.args[0].val.i, ev.args[1].val.i, ev.args[2].val.i, ev.args[3].val.i, ev.args[4].val.i);
        break;

    case GL3_API_glBufferData:
        {
            GLuint  size    = ev.args[1].val.i;
            GLchar *pData   = (GLchar*)ReadData(READ_GL_UCHAR, ev.args[2].val.i, size);
            CURRENT_CONTEXT1()->ApiBufferData(ev.args[0].val.i, size, pData, ev.args[3].val.i);
        }
        break;

    case GL3_API_glBufferSubData:
        {
            GLuint  offset  = ev.args[1].val.i;
            GLuint  size    = ev.args[2].val.i;
            GLchar *pData   = (GLchar*)ReadData(READ_GL_UCHAR, ev.args[3].val.i, size);
            CURRENT_CONTEXT1()->ApiBufferSubData(ev.args[0].val.i, offset, size, pData);
        }
        break;

    case GL3_API_glBindVertexArray:
        CURRENT_CONTEXT1()->ApiBindVertexArray(ev.args[0].val.i);
        break;

    case GL3_API_glBindTransformFeedback:
        CURRENT_CONTEXT1()->ApiBindTransformFeedback(ev.args[0].val.i, ev.args[1].val.i);
        break;

    case GL3_API_glBeginTransformFeedback:
        CURRENT_CONTEXT1()->ApiBeginTransformFeedback(ev.args[0].val.i);
        break;

    case GL3_API_glBindFramebuffer:
        CURRENT_CONTEXT1()->ApiBindFramebuffer(ev.args[0].val.i, ev.args[1].val.i);
        break;

    case GL3_API_glBindRenderbuffer:
        CURRENT_CONTEXT1()->ApiBindRenderbuffer(ev.args[0].val.i, ev.args[1].val.i);
        break;

    case GL3_API_glBindAttribLocation:
        {
            GLint   loc     = ev.args[1].val.i;
            GLuint  program = ev.args[0].val.i;
            GLchar  *name   = (GLchar*)ReadData(READ_GL_UCHAR, ev.args[2].val.i, 0);
            CURRENT_CONTEXT1()->ApiBindAttribLocation(program, loc, name);
        }
        break;

    case GL3_API_glBlendColor:
        CURRENT_CONTEXT1()->ApiBlendColor(ev.args[0].val.f, ev.args[1].val.f, ev.args[2].val.f, ev.args[3].val.f);
        break;

    case GL3_API_glBlendEquation:
        CURRENT_CONTEXT1()->ApiBlendEquation(ev.args[0].val.i);
        break;

    case GL3_API_glBlendEquationSeparate:
        CURRENT_CONTEXT1()->ApiBlendEquationSeparate(ev.args[0].val.i, ev.args[1].val.i);
        break;

    case GL3_API_glBlendFunc:
        CURRENT_CONTEXT1()->ApiBlendFunc(ev.args[0].val.i, ev.args[1].val.i);
        break;

    case GL3_API_glBlendFuncSeparate:
        CURRENT_CONTEXT1()->ApiBlendFuncSeparate(ev.args[0].val.i, ev.args[1].val.i, ev.args[2].val.i, ev.args[3].val.i);
        break;

    case GL3_API_glClear:
        CURRENT_CONTEXT1()->ApiClear(ev.args[0].val.i);
        break;

    case GL3_API_glClearColor:
        {
            GLfloat p1 = ev.args[0].val.f;
            GLfloat p2 = ev.args[1].val.f;
            GLfloat p3 = ev.args[2].val.f;
            GLfloat p4 = ev.args[3].val.f;
            CURRENT_CONTEXT1()->ApiClearColor(p1, p2, p3, p4);
        }
        break;

    case GL3_API_glClearDepthf:
        CURRENT_CONTEXT1()->ApiClearDepthf(ev.args[0].val.f);
        break;

    case GL3_API_glClearStencil:
        CURRENT_CONTEXT1()->ApiClearStencil(ev.args[0].val.i);
        break;

    case GL3_API_glCompressedTexImage2D:
        {
            GLenum  target      = ev.args[0].val.i;
            GLuint  level       = ev.args[1].val.i;
            GLenum  format      = ev.args[2].val.i;
            GLint   width       = ev.args[3].val.i;
            GLint   height      = ev.args[4].val.i;
            GLint   border      = ev.args[5].val.i;
            GLint   imageSize   = ev.args[6].val.i;
            GLuint  handle      = ev.args[7].val.i;
            GLvoid  *data       = (GLvoid*)ReadCompressedTex(READ_GL_UCHAR, handle, imageSize);

            CURRENT_CONTEXT1()->ApiCompressedTexImage2D(target, level, format, width, height, border, imageSize, data);
        }
        break;

    case GL3_API_glCompressedTexSubImage2D:
        {
            GLenum  target      = ev.args[0].val.i;
            GLuint  level       = ev.args[1].val.i;
            GLint   xoffset     = ev.args[2].val.i;
            GLint   yoffset     = ev.args[3].val.i;
            GLsizei width       = ev.args[4].val.i;
            GLsizei height      = ev.args[5].val.i;
            GLenum  format      = ev.args[6].val.i;
            GLsizei imageSize   = ev.args[7].val.i;
            GLuint  handle      = ev.args[8].val.i;
            GLvoid  *data       = (GLvoid*)ReadCompressedTex(READ_GL_UCHAR, handle, imageSize);

            CURRENT_CONTEXT1()->ApiCompressedTexSubImage2D(target, level, xoffset, yoffset, width, height, format, imageSize, data);
        }
        break;

    case GL3_API_glColorMask:
        CURRENT_CONTEXT1()->ApiColorMask(ev.args[0].val.i, ev.args[1].val.i, ev.args[2].val.i, ev.args[3].val.i);
        break;

    case GL3_API_glCompileShader:
        CURRENT_CONTEXT1()->ApiCompileShader(ev.args[0].val.i);
        break;

    case GL3_API_glCreateProgram:
        CURRENT_CONTEXT1()->ApiCreateProgram(ev.args[0].val.i);
        break;

    case GL3_API_glCreateShader:
        CURRENT_CONTEXT1()->ApiCreateShader(ev.args[0].val.i, ev.args[1].val.i);
        break;

    case GL3_API_glCullFace:
        CURRENT_CONTEXT1()->ApiCullFace(ev.args[0].val.i);
        break;

    case GL3_API_glDeleteBuffers:
        {
            GLuint  count   = ev.args[0].val.i;
            GLuint *pData   = (GLuint*)ReadData(READ_GL_UINT, ev.args[1].val.i, count);
            CURRENT_CONTEXT1()->ApiDeleteBuffers(count, pData);
        }
        break;

    case GL3_API_glDeleteFramebuffers:
        {
            GLuint  count   = ev.args[0].val.i;
            GLuint *pData   = (GLuint*)ReadData(READ_GL_UINT, ev.args[1].val.i, count);
            CURRENT_CONTEXT1()->ApiDeleteFramebuffers(count, pData);
        }
        break;

    case GL3_API_glDeleteProgram:
        CURRENT_CONTEXT1()->ApiDeleteProgram(ev.args[0].val.i);
        break;

    case GL3_API_glDeleteRenderbuffers:
        {
            GLuint  count   = ev.args[0].val.i;
            GLuint *pData   = (GLuint*)ReadData(READ_GL_UINT, ev.args[1].val.i, count);
            CURRENT_CONTEXT1()->ApiDeleteRenderbuffers(count, pData);
        }
        break;

    case GL3_API_glDeleteShader:
        CURRENT_CONTEXT1()->ApiDeleteShader(ev.args[0].val.i);
        break;

    case GL3_API_glDeleteTextures:
        {
            GLuint  count = ev.args[0].val.i;
            GLuint  *v = (GLuint*)ReadData(READ_GL_UINT, ev.args[1].val.i, count);
            CURRENT_CONTEXT1()->ApiDeleteTextures(count, v);
            break;
        }

    case GL3_API_glDeleteTransformFeedbacks:
        {
            GLuint  count   = ev.args[0].val.i;
            GLuint *pData   = (GLuint*)ReadData(READ_GL_UINT, ev.args[1].val.i, count);
            CURRENT_CONTEXT1()->ApiDeleteTransformFeedbacks(count, pData);
        }
        break;

    case GL3_API_glDeleteVertexArrays:
        {
            GLuint  count   = ev.args[0].val.i;
            GLuint *pData   = (GLuint*)ReadData(READ_GL_UINT, ev.args[1].val.i, count);
            CURRENT_CONTEXT1()->ApiDeleteVertexArrays(count, pData);
        }
        break;

    case GL3_API_glDepthFunc:
        CURRENT_CONTEXT1()->ApiDepthFunc(ev.args[0].val.i);
        break;

    case GL3_API_glDepthMask:
        CURRENT_CONTEXT1()->ApiDepthMask(ev.args[0].val.i);
        break;

    case GL3_API_glDepthRangef:
        CURRENT_CONTEXT1()->ApiDepthRangef(ev.args[0].val.f, ev.args[0].val.f);
        break;

    case GL3_API_glDetachShader:
        CURRENT_CONTEXT1()->ApiDetachShader(ev.args[0].val.i, ev.args[1].val.i);
        break;

    case GL3_API_glDisable:
        CURRENT_CONTEXT1()->ApiDisable(ev.args[0].val.i);
        break;

    case GL3_API_glDisableVertexAttribArray:
        CURRENT_CONTEXT1()->ApiDisableVertexAttribArray(ev.args[0].val.i);
        break;

    case GL3_API_glEnable:
        CURRENT_CONTEXT1()->ApiEnable(ev.args[0].val.i);
        break;

    case GL3_API_glEnableVertexAttribArray:
        CURRENT_CONTEXT1()->ApiEnableVertexAttribArray(ev.args[0].val.i);
        break;

    case GL3_API_glEndTransformFeedback:
        CURRENT_CONTEXT1()->ApiEndTransformFeedback();
        break;

    case GL3_API_glFramebufferRenderbuffer:
        CURRENT_CONTEXT1()->ApiFramebufferRenderbuffer(ev.args[0].val.i, ev.args[1].val.i, ev.args[2].val.i, ev.args[3].val.i);
        break;

    case GL3_API_glFramebufferTexture2D:
        CURRENT_CONTEXT1()->ApiFramebufferTexture2D(ev.args[0].val.i, ev.args[1].val.i, ev.args[2].val.i, ev.args[3].val.i, ev.args[4].val.i);
        break;

    case GL3_API_glFrontFace:
        CURRENT_CONTEXT1()->ApiFrontFace(ev.args[0].val.i);
        break;

    case GL3_API_glGenBuffers:
        {
            GLuint  count   = ev.args[0].val.i;
            GLuint *pData   = (GLuint*)ReadData(READ_GL_UINT, ev.args[1].val.i, count);
            CURRENT_CONTEXT1()->ApiGenBuffers(count, pData);
        }
        break;

    case GL3_API_glGenerateMipmap:
        CURRENT_CONTEXT1()->ApiGenerateMipmap(ev.args[0].val.i);
        break;

    case GL3_API_glGenFramebuffers:
        {
            GLuint  count   = ev.args[0].val.i;
            GLuint *pData   = (GLuint*)ReadData(READ_GL_UINT, ev.args[1].val.i, count);
            CURRENT_CONTEXT1()->ApiGenFramebuffers(count, pData);
        }
        break;

    case GL3_API_glGenRenderbuffers:
        {
            GLuint  count   = ev.args[0].val.i;
            GLuint *pData   = (GLuint*)ReadData(READ_GL_UINT, ev.args[1].val.i, count);
            CURRENT_CONTEXT1()->ApiGenRenderbuffers(count, pData);
        }
        break;

    case GL3_API_glGenTextures:
        {
            GLuint  count   = ev.args[0].val.i;
            GLuint *pData   = (GLuint*)ReadData(READ_GL_UINT, ev.args[1].val.i, count);
            CURRENT_CONTEXT1()->ApiGenTextures(count, pData);
        }
        break;

    case GL3_API_glGenTransformFeedbacks:
        {
            GLuint  count   = ev.args[0].val.i;
            GLuint *pData   = (GLuint*)ReadData(READ_GL_UINT, ev.args[1].val.i, count);
            CURRENT_CONTEXT1()->ApiGenTransformFeedbacks(count, pData);
        }
        break;

    case GL3_API_glGetUniformLocation:
        {
            GLchar  *name    = (GLchar*)ReadData(READ_GL_UCHAR, ev.args[1].val.i, 0);
            GLuint  location = ev.args[2].val.i;
            GLuint  program  = ev.args[0].val.i;
            CURRENT_CONTEXT1()->ApiGetUniformLocation(program, name, location);
        }
        break;

    case GL3_API_glGenVertexArrays:
        {
            GLuint  count   = ev.args[0].val.i;
            GLuint *pData   = (GLuint*)ReadData(READ_GL_UINT, ev.args[1].val.i, count);
            CURRENT_CONTEXT1()->ApiGenVertexArrays(count, pData);
        }
        break;

    case GL3_API_glGetAttribLocation:
        {
            GLuint  program = ev.args[0].val.i;
            GLchar  *name   = (GLchar*)ReadData(READ_GL_UCHAR, ev.args[1].val.i, 0);
            GLint   oldLoc  = ev.args[2].val.i;
            CURRENT_CONTEXT1()->ApiGetAttribLocation(program, name, oldLoc);
        }
        break;

    case GL3_API_glLineWidth:
        CURRENT_CONTEXT1()->ApiLineWidth(ev.args[0].val.f);
        break;

    case GL3_API_glLinkProgram:
        CURRENT_CONTEXT1()->ApiLinkProgram(ev.args[0].val.i);
        break;

    case GL3_API_glPixelStorei:
        CURRENT_CONTEXT1()->ApiPixelStorei(ev.args[0].val.i, ev.args[1].val.i);
        break;

    case GL3_API_glPolygonOffset:
        CURRENT_CONTEXT1()->ApiPolygonOffset(ev.args[0].val.f, ev.args[1].val.f);
        break;

    case GL3_API_glProgramBinary:
    case GL3_API_glProgramBinaryOES:
        {
            GLuint  program = ev.args[0].val.i;
            GLenum  format  = ev.args[1].val.i;
            GLuint  len     = ev.args[3].val.i;
            GLubyte *binary = (GLubyte*)ReadData(READ_GL_UCHAR, ev.args[2].val.h, len);

            CURRENT_CONTEXT1()->ApiProgramBinary(program, format, binary, len);
        }
        break;

    case GL3_API_glRenderbufferStorage:
        CURRENT_CONTEXT1()->ApiRenderbufferStorage(ev.args[0].val.i, ev.args[1].val.i, ev.args[2].val.i, ev.args[3].val.i);
        break;

    case GL3_API_glSampleCoverage:
        CURRENT_CONTEXT1()->ApiSampleCoverage(ev.args[0].val.f, ev.args[1].val.i);
        break;

    case GL3_API_glScissor:
        CURRENT_CONTEXT1()->ApiScissor(ev.args[0].val.i, ev.args[1].val.i, ev.args[2].val.i, ev.args[3].val.i);
        break;

    case GL3_API_glShaderSource:
        ShaderSource(ev.args[0].val.i, ev.args[1].val.i, ev.args[2].val.i, ev.args[3].val.i);
        break;

    case GL3_API_glStencilFunc:
        CURRENT_CONTEXT1()->ApiStencilFunc(ev.args[0].val.i, ev.args[1].val.i, ev.args[2].val.i);
        break;

    case GL3_API_glStencilFuncSeparate:
        CURRENT_CONTEXT1()->ApiStencilFuncSeparate(ev.args[0].val.i, ev.args[1].val.i, ev.args[2].val.i, ev.args[3].val.i);
        break;

    case GL3_API_glStencilMask:
        CURRENT_CONTEXT1()->ApiStencilMask(ev.args[0].val.i);
        break;

    case GL3_API_glStencilMaskSeparate:
        CURRENT_CONTEXT1()->ApiStencilMaskSeparate(ev.args[0].val.i, ev.args[1].val.i);
        break;

    case GL3_API_glStencilOp:
        CURRENT_CONTEXT1()->ApiStencilOp(ev.args[0].val.i, ev.args[1].val.i, ev.args[2].val.i);
        break;

    case GL3_API_glStencilOpSeparate:
        CURRENT_CONTEXT1()->ApiStencilOpSeparate(ev.args[0].val.i, ev.args[1].val.i, ev.args[2].val.i, ev.args[3].val.i);
        break;

    case GL3_API_glTexParameteri:
        CURRENT_CONTEXT1()->ApiTexParameteri(ev.args[0].val.i, ev.args[1].val.i, ev.args[2].val.i);
        break;

    case GL3_API_glTexParameteriv:
        {
            GLenum  target  = ev.args[0].val.i;
            GLenum  pname   = ev.args[1].val.i;
            GLuint  handle  = ev.args[2].val.i;
            GLvoid  *pData  = ReadData(READ_GL_UCHAR, handle, 4);
            CURRENT_CONTEXT1()->ApiTexParameteriv(target, pname, (GLint*)pData);
        }
        break;

    case GL3_API_glTexParameterf:
        CURRENT_CONTEXT1()->ApiTexParameterf(ev.args[0].val.i, ev.args[1].val.i, ev.args[2].val.f);
        break;

    case GL3_API_glTexParameterfv:
        {
            GLenum  target  = ev.args[0].val.i;
            GLenum  pname   = ev.args[1].val.i;
            GLuint  handle  = ev.args[2].val.i;
            GLvoid  *pData  = ReadData(READ_GL_UCHAR, handle, 4);
            CURRENT_CONTEXT1()->ApiTexParameterfv(target, pname, (GLfloat*)pData);
        }
        break;

    case GL3_API_glTexImage2D:
        {
            GLenum      target  = ev.args[0].val.i;
            GLint       level   = ev.args[1].val.i;
            GLint       infmt   = ev.args[2].val.i;
            GLsizei     width   = ev.args[3].val.i;
            GLsizei     height  = ev.args[4].val.i;
            GLint       border  = ev.args[5].val.i;
            GLenum      format  = ev.args[6].val.i;
            GLenum      type    = ev.args[7].val.i;
            GLuint      handle  = ev.args[8].val.i;
            GLuint      imgSize = GetTexImageSize(infmt, type, width, height, 1);
            GLchar      *pixels = handle?(GLchar*)ReadTexData(READ_GL_UCHAR, handle, imgSize):NULL;

            CURRENT_CONTEXT1()->ApiTexImage2D(target, level, infmt, width, height, border, format, type, pixels);
        }
        break;

    case GL3_API_glTexSubImage2D:
        {
            GLenum      target      = ev.args[0].val.i;
            GLint       level       = ev.args[1].val.i;
            GLint       xoffset     = ev.args[2].val.i;
            GLint       yoffset     = ev.args[3].val.i;
            GLsizei     width       = ev.args[4].val.i;
            GLsizei     height      = ev.args[5].val.i;
            GLenum      format      = ev.args[6].val.i;
            GLenum      type        = ev.args[7].val.i;
            GLuint      handle      = ev.args[8].val.i;
            GLuint      imgSize     = GetTexImageSize(format, type, width, height, 1);
            GLubyte      *pixels    = (GLubyte*)ReadData(READ_GL_UCHAR, handle, imgSize);

            CURRENT_CONTEXT1()->ApiTexSubImage2D(target, level, xoffset, yoffset, width, height, format, type, pixels);
        }
        break;

    case GL3_API_glTexImage3D:
        {
            GLenum      target  = ev.args[0].val.i;
            GLint       level   = ev.args[1].val.i;
            GLint       infmt   = ev.args[2].val.i;
            GLsizei     width   = ev.args[3].val.i;
            GLsizei     height  = ev.args[4].val.i;
            GLsizei     depth   = ev.args[5].val.i;
            GLint       border  = ev.args[6].val.i;
            GLenum      format  = ev.args[7].val.i;
            GLenum      type    = ev.args[8].val.i;
            GLuint      handle  = ev.args[9].val.i;
            GLuint      imgSize = GetTexImageSize(infmt, type, width, height, 1);
            GLchar      *pixels = handle?(GLchar*)ReadTexData(READ_GL_UCHAR, handle, imgSize):NULL;

            CURRENT_CONTEXT1()->ApiTexImage3D(target, level, infmt, width, height, depth, border, format, type, pixels);
        }
        break;

    case GL3_API_glTexSubImage3D:
        {
            GLenum      target      = ev.args[0].val.i;
            GLint       level       = ev.args[1].val.i;
            GLint       xoffset     = ev.args[2].val.i;
            GLint       yoffset     = ev.args[3].val.i;
            GLint       zoffset     = ev.args[4].val.i;
            GLsizei     width       = ev.args[5].val.i;
            GLsizei     height      = ev.args[6].val.i;
            GLsizei     depth       = ev.args[7].val.i;
            GLenum      format      = ev.args[8].val.i;
            GLenum      type        = ev.args[9].val.i;
            GLuint      handle      = ev.args[10].val.i;
            GLuint      imgSize     = GetTexImageSize(format, type, width, height, 1);
            GLubyte     *pixels     = handle?(GLubyte*)ReadTexData(READ_GL_UCHAR, handle, imgSize):NULL;

            CURRENT_CONTEXT1()->ApiTexSubImage3D(target, level, xoffset, yoffset, zoffset, width, height, depth, format, type, pixels);
        }
        break;

    case GL3_API_glTexStorage2D:
        {
            GLenum  target  = ev.args[0].val.i;
            GLint   levels  = ev.args[1].val.i;
            GLenum  format  = ev.args[2].val.i;
            GLsizei width   = ev.args[3].val.i;
            GLsizei height  = ev.args[4].val.i;

            CURRENT_CONTEXT1()->ApiTexStorage2D(target, levels, format, width, height);
        }
        break;

    case GL3_API_glTexStorage3D:
        {
            GLenum  target  = ev.args[0].val.i;
            GLint   levels  = ev.args[1].val.i;
            GLenum  format  = ev.args[2].val.i;
            GLsizei width   = ev.args[3].val.i;
            GLsizei height  = ev.args[4].val.i;
            GLsizei depth   = ev.args[5].val.i;

            CURRENT_CONTEXT1()->ApiTexStorage3D(target, levels, format, width, height, depth);
        }
        break;

    case GL3_API_glUniform1f:
        CURRENT_CONTEXT1()->ApiUniform1f(ev.args[0].val.i, ev.args[1].val.f);
        break;

    case GL3_API_glUniform2f:
        CURRENT_CONTEXT1()->ApiUniform2f(ev.args[0].val.i, ev.args[1].val.f, ev.args[2].val.f);
        break;

    case GL3_API_glUniform3f:
        CURRENT_CONTEXT1()->ApiUniform3f(ev.args[0].val.i, ev.args[1].val.f, ev.args[2].val.f, ev.args[3].val.f);
        break;

    case GL3_API_glUniform4f:
        CURRENT_CONTEXT1()->ApiUniform4f(ev.args[0].val.i, ev.args[1].val.f, ev.args[2].val.f, ev.args[3].val.f, ev.args[4].val.f);
        break;

    case GL3_API_glUniform1fv:
        {
            GLint   loc     = ev.args[0].val.i;
            GLsizei count   = ev.args[1].val.i;
            GLvoid  *pData  = ReadData(READ_GL_UCHAR, ev.args[2].val.h, 0);

            CURRENT_CONTEXT1()->ApiUniform1fv(loc, count, (const GLfloat*)pData);
        }
        break;

    case GL3_API_glUniform2fv:
        {
            GLint   loc     = ev.args[0].val.i;
            GLsizei count   = ev.args[1].val.i;
            GLvoid  *pData  = ReadData(READ_GL_UCHAR, ev.args[2].val.h, 0);

            CURRENT_CONTEXT1()->ApiUniform2fv(loc, count, (const GLfloat*)pData);
        }
        break;

    case GL3_API_glUniform3fv:
        {
            GLint   loc     = ev.args[0].val.i;
            GLsizei count   = ev.args[1].val.i;
            GLvoid  *pData  = ReadData(READ_GL_UCHAR, ev.args[2].val.h, 0);

            CURRENT_CONTEXT1()->ApiUniform3fv(loc, count, (const GLfloat*)pData);
        }
        break;

    case GL3_API_glUniform4fv:
        {
            GLint   loc     = ev.args[0].val.i;
            GLsizei count   = ev.args[1].val.i;
            GLvoid  *pData  = ReadData(READ_GL_UCHAR, ev.args[2].val.h, 0);

            CURRENT_CONTEXT1()->ApiUniform4fv(loc, count, (const GLfloat*)pData);
        }
        break;

    case GL3_API_glUniform1i:
        CURRENT_CONTEXT1()->ApiUniform1i(ev.args[0].val.i, ev.args[1].val.i);
        break;

    case GL3_API_glUniform2i:
        CURRENT_CONTEXT1()->ApiUniform2i(ev.args[0].val.i, ev.args[1].val.i, ev.args[2].val.i);
        break;

    case GL3_API_glUniform3i:
        CURRENT_CONTEXT1()->ApiUniform3i(ev.args[0].val.i, ev.args[1].val.i, ev.args[2].val.i, ev.args[3].val.i);
        break;

    case GL3_API_glUniform4i:
        CURRENT_CONTEXT1()->ApiUniform4i(ev.args[0].val.i, ev.args[1].val.i, ev.args[2].val.i, ev.args[3].val.i, ev.args[4].val.i);
        break;

    case GL3_API_glUniform1iv:
        {
            GLint   loc     = ev.args[0].val.i;
            GLsizei count   = ev.args[1].val.i;
            GLvoid  *pData  = ReadData(READ_GL_UCHAR, ev.args[2].val.h, 0);

            CURRENT_CONTEXT1()->ApiUniform1iv(loc, count, (const GLint*)pData);
        }
        break;

    case GL3_API_glUniform2iv:
        {
            GLint   loc     = ev.args[0].val.i;
            GLsizei count   = ev.args[1].val.i;
            GLvoid  *pData  = ReadData(READ_GL_UCHAR, ev.args[2].val.h, 0);

            CURRENT_CONTEXT1()->ApiUniform2iv(loc, count, (const GLint*)pData);
        }
        break;

    case GL3_API_glUniform3iv:
        {
            GLint   loc     = ev.args[0].val.i;
            GLsizei count   = ev.args[1].val.i;
            GLvoid  *pData  = ReadData(READ_GL_UCHAR, ev.args[2].val.h, 0);

            CURRENT_CONTEXT1()->ApiUniform3iv(loc, count, (const GLint*)pData);
        }
        break;

    case GL3_API_glUniform4iv:
        {
            GLint   loc     = ev.args[0].val.i;
            GLsizei count   = ev.args[1].val.i;
            GLvoid  *pData  = ReadData(READ_GL_UCHAR, ev.args[2].val.h, 0);

            CURRENT_CONTEXT1()->ApiUniform4iv(loc, count, (const GLint*)pData);
        }
        break;

    case GL3_API_glUniformMatrix2fv:
        {
            GLint       loc     = ev.args[0].val.i;
            GLsizei     count   = ev.args[1].val.i;
            GLboolean   trans   = ev.args[2].val.i;
            GLvoid      *pData  = ReadData(READ_GL_UCHAR, ev.args[3].val.h, 0);

            CURRENT_CONTEXT1()->ApiUniformMatrix2fv(loc, count, trans, (const GLfloat*)pData);
        }
        break;

    case GL3_API_glUniformMatrix3fv:
        {
            GLint       loc     = ev.args[0].val.i;
            GLsizei     count   = ev.args[1].val.i;
            GLboolean   trans   = ev.args[2].val.i;
            GLvoid      *pData  = ReadData(READ_GL_UCHAR, ev.args[3].val.h, 0);

            CURRENT_CONTEXT1()->ApiUniformMatrix3fv(loc, count, trans, (const GLfloat*)pData);
        }
        break;

    case GL3_API_glUniformMatrix4fv:
        {
            GLint       loc     = ev.args[0].val.i;
            GLsizei     count   = ev.args[1].val.i;
            GLboolean   trans   = ev.args[2].val.i;
            GLvoid      *pData  = ReadData(READ_GL_UCHAR, ev.args[3].val.h, 0);

            CURRENT_CONTEXT1()->ApiUniformMatrix4fv(loc, count, trans, (const GLfloat*)pData);
        }
        break;

    case GL3_API_glUseProgram:
        CURRENT_CONTEXT1()->ApiUseProgram(ev.args[0].val.i);
        break;

    case GL3_API_glVertexAttrib1f:
        CURRENT_CONTEXT1()->ApiVertexAttrib1f(ev.args[0].val.i, ev.args[1].val.f);
        break;

    case GL3_API_glVertexAttrib2f:
        CURRENT_CONTEXT1()->ApiVertexAttrib2f(ev.args[0].val.i, ev.args[1].val.f, ev.args[2].val.f);
        break;

    case GL3_API_glVertexAttrib3f:
        CURRENT_CONTEXT1()->ApiVertexAttrib3f(ev.args[0].val.i, ev.args[1].val.f, ev.args[2].val.f, ev.args[3].val.f);
        break;

    case GL3_API_glVertexAttrib4f:
        CURRENT_CONTEXT1()->ApiVertexAttrib4f(ev.args[0].val.i, ev.args[1].val.f, ev.args[2].val.f, ev.args[3].val.f, ev.args[4].val.f);
        break;

    case GL3_API_glVertexAttrib1fv:
        {
            GLuint  index   = ev.args[0].val.i;
            GLfloat *pData  = (GLfloat*)ReadData(READ_GL_UINT, ev.args[1].val.h, 1);
            CURRENT_CONTEXT1()->ApiVertexAttrib1fv(index, pData);
        }
        break;

    case GL3_API_glVertexAttrib2fv:
        {
            GLuint  index   = ev.args[0].val.i;
            GLfloat *pData  = (GLfloat*)ReadData(READ_GL_UINT, ev.args[1].val.h, 2);
            CURRENT_CONTEXT1()->ApiVertexAttrib2fv(index, pData);
        }
        break;

    case GL3_API_glVertexAttrib3fv:
        {
            GLuint  index   = ev.args[0].val.i;
            GLfloat *pData  = (GLfloat*)ReadData(READ_GL_UINT, ev.args[1].val.h, 3);
            CURRENT_CONTEXT1()->ApiVertexAttrib3fv(index, pData);
        }
        break;

    case GL3_API_glVertexAttrib4fv:
        {
            GLuint  index   = ev.args[0].val.i;
            GLfloat *pData  = (GLfloat*)ReadData(READ_GL_UINT, ev.args[1].val.h, 4);
            CURRENT_CONTEXT1()->ApiVertexAttrib4fv(index, pData);
        }
        break;

    case GL3_API_glVertexAttribPointer:
        {
            GLuint      index       = ev.args[0].val.i;
            GLint       size        = ev.args[1].val.i;
            GLenum      type        = ev.args[2].val.h;
            GLboolean   normalized  = ev.args[3].val.i;
            GLsizei     stride      = ev.args[4].val.i;
            GLuint      pointer     = ev.args[5].val.i;

            if (CURRENT_CONTEXT1()->arrayBuffer)
            {
                CURRENT_CONTEXT1()->ApiVertexAttribPointer(index, size, type, normalized, stride, (GLvoid*)pointer);
            }
            else
            {
                GLvoid *pData = ReadData(READ_GL_UCHAR, pointer, 0);
                CURRENT_CONTEXT1()->ApiVertexAttribPointer(index, size, type, normalized, stride, pData);
            }
        }
        break;

    case GL3_API_glVertexAttribIPointer:
        {
            GLuint      index       = ev.args[0].val.i;
            GLint       size        = ev.args[1].val.i;
            GLenum      type        = ev.args[2].val.h;
            GLsizei     stride      = ev.args[3].val.i;
            GLuint      pointer     = ev.args[4].val.i;

            if (CURRENT_CONTEXT1()->arrayBuffer)
            {
                CURRENT_CONTEXT1()->ApiVertexAttribIPointer(index, size, type, stride, (GLvoid*)pointer);
            }
            else
            {
                GLvoid *pData = ReadData(READ_GL_UCHAR, pointer, 0);
                CURRENT_CONTEXT1()->ApiVertexAttribIPointer(index, size, type, stride, pData);
            }
        }
        break;

    case GL3_API_glViewport:
        CURRENT_CONTEXT1()->ApiViewport(ev.args[0].val.i, ev.args[1].val.i, ev.args[2].val.i, ev.args[3].val.i);
        break;

    default:
        break;
    }
}

GLuint GetCurrentDataLen(GLuint handle)
{
    CApiArray   *p  = g_pPlayer->FindArray(handle);
    GLuint      len = p?p->size():0;
    return len;
}
#endif /* API_DUMP */

GLvoid* GetCurrentTexDataPtr(GLuint handle)
{
#ifdef TCG_USE
    CApiArray   *it = g_pPlayer->FindTextureArray(handle);
    GLvoid      *p  = it?(EGLint*)it->GetCurrentDataPointer():NULL;
    return p;
#elif defined(VTRACER_USE)
    CApiArray   *it = g_pPlayer->FindArray(handle);
    GLvoid      *p  = it?(EGLint*)it->GetCurrentDataPointer():NULL;
    return p;
#else
    return (GLvoid*)handle;
#endif
}

GLvoid* GetCurrentVAPDataPtr(GLuint handle)
{
#ifdef TCG_USE
    CApiArray   *it = g_pPlayer->FindVAPArray(handle);
    GLvoid      *p  = it?(EGLint*)it->GetCurrentDataPointer():NULL;
    return p;
#elif defined(VTRACER_USE)
    CApiArray   *it = g_pPlayer->FindArray(handle);
    GLvoid      *p  = it?(EGLint*)it->GetCurrentDataPointer():NULL;
    return p;
#else
    return (GLvoid*)handle;
#endif
}

GLuint GetCurrentVAPDataLen(GLuint handle)
{
#ifdef TCG_USE
    CApiArray   *it = g_pPlayer->FindVAPArray(handle);
    GLuint      len  = it?it->size():0;
    return len;
#elif defined(VTRACER_USE)
    CApiArray   *it = g_pPlayer->FindArray(handle);
    GLuint      len  = it?it->size():0;
    return len;
#else
    // TODO
    __asm int 3;
    return 0;
#endif
}

GLvoid ReleaseShaderSource(GLchar *p)
{
#ifdef VTRACER_USE
    if (p)
    {
        free(p);
    }
#endif
}