#include "utils.h"
#include "ConvertApiDump.h"
#include "GLESShare.h"
#include "TranslateGL.h"
#include "MathAPI.h"
#include "ApiGLES3Context.h"
#include "ApiEGLContext.h"
#include "Analyzer.h"
#include "egl.h"
#include "gles3.h"

CAnalyzer   g_Analyzer;

static void _ParseAttrbutes(GLchar *output, GLint outputSize, const EGLint *attrib_list)
{
    EGLenum     attribute;
    EGLint      value;
    GLchar      *ptr;
    EGLBoolean  bStr = EGL_FALSE;
    GLchar      str[256];

    do
    {
        if (attrib_list != NULL)
        {
            attribute   = attrib_list[0];
            value       = attrib_list[1];
            attrib_list += 2;
        }
        else
        {
            attribute = EGL_NONE;
            value     = EGL_DONT_CARE;
        }

        switch (attribute)
        {
        case EGL_BUFFER_SIZE:
            ptr = "EGL_BUFFER_SIZE";
            break;
        case EGL_ALPHA_SIZE:
            ptr = "EGL_ALPHA_SIZE";
            break;
        case EGL_BLUE_SIZE:
            ptr = "EGL_BLUE_SIZE";
            break;
        case EGL_GREEN_SIZE:
            ptr = "EGL_GREEN_SIZE";
            break;
        case EGL_RED_SIZE:
            ptr = "EGL_RED_SIZE";
            break;
        case EGL_DEPTH_SIZE:
            ptr = "EGL_DEPTH_SIZE";
            break;
        case EGL_STENCIL_SIZE:
            ptr = "EGL_STENCIL_SIZE";
            break;
        case EGL_CONFIG_CAVEAT:
            ptr = "EGL_CONFIG_CAVEAT";
            break;
        case EGL_CONFIG_ID:
            ptr = "EGL_CONFIG_ID";
            break;
        case EGL_LEVEL:
            ptr = "EGL_LEVEL";
            break;
        case EGL_MAX_PBUFFER_WIDTH:
            ptr = "EGL_MAX_PBUFFER_WIDTH";
            break;
        case EGL_MAX_PBUFFER_HEIGHT:
            ptr = "EGL_MAX_PBUFFER_HEIGHT";
            break;
        case EGL_MAX_PBUFFER_PIXELS:
            ptr = "EGL_MAX_PBUFFER_PIXELS";
            break;
        case EGL_NATIVE_RENDERABLE:
            ptr = "EGL_NATIVE_RENDERABLE";
            break;
        case EGL_NATIVE_VISUAL_ID:
            ptr = "EGL_NATIVE_VISUAL_ID";
            break;
        case EGL_NATIVE_VISUAL_TYPE:
            ptr = "EGL_NATIVE_VISUAL_TYPE";
            break;
        case EGL_SAMPLES:
            ptr = "EGL_SAMPLES";
            break;
        case EGL_SAMPLE_BUFFERS:
            ptr = "EGL_SAMPLE_BUFFERS";
            break;
        case EGL_SURFACE_TYPE:
            ptr = "EGL_SURFACE_TYPE";
            bStr = EGL_TRUE;

            if (value & EGL_WINDOW_BIT)
            {
                strcpy(str, "EGL_WINDOW_BIT ");
            }

            if (value & EGL_PIXMAP_BIT)
            {
                strcat(str, "EGL_PIXMAP_BIT ");
            }

            if (value & EGL_PBUFFER_BIT)
            {
                strcat(str, "EGL_PBUFFER_BIT ");
            }

            if (value & EGL_MULTISAMPLE_RESOLVE_BOX_BIT)
            {
                strcat(str, "EGL_MULTISAMPLE_RESOLVE_BOX_BIT ");
            }

            if (value & EGL_SWAP_BEHAVIOR_PRESERVED_BIT)
            {
                strcat(str, "EGL_SWAP_BEHAVIOR_PRESERVED_BIT");
            }

            if (value & EGL_VG_COLORSPACE_LINEAR_BIT)
            {
                strcat(str, "EGL_VG_COLORSPACE_LINEAR_BIT");
            }

            if (value & EGL_VG_ALPHA_FORMAT_PRE_BIT)
            {
                strcat(str, "EGL_VG_ALPHA_FORMAT_PRE_BIT");
            }
            break;

        case EGL_TRANSPARENT_TYPE:
            ptr = "EGL_TRANSPARENT_TYPE";
            break;
        case EGL_TRANSPARENT_BLUE_VALUE:
            ptr = "EGL_TRANSPARENT_BLUE_VALUE";
            break;
        case EGL_TRANSPARENT_GREEN_VALUE:
            ptr = "EGL_TRANSPARENT_GREEN_VALUE";
            break;
        case EGL_TRANSPARENT_RED_VALUE:
            ptr = "EGL_TRANSPARENT_RED_VALUE";
            break;
        case EGL_BIND_TO_TEXTURE_RGB:
            ptr = "EGL_BIND_TO_TEXTURE_RGB";
            break;
        case EGL_BIND_TO_TEXTURE_RGBA:
            ptr = "EGL_BIND_TO_TEXTURE_RGBA";
            break;
        case EGL_MIN_SWAP_INTERVAL:
            ptr = "EGL_MIN_SWAP_INTERVAL";
            break;
        case EGL_MAX_SWAP_INTERVAL:
            ptr = "EGL_MAX_SWAP_INTERVAL";
            break;
        case EGL_LUMINANCE_SIZE:
            ptr = "EGL_LUMINANCE_SIZE";
            break;
        case EGL_ALPHA_MASK_SIZE:
            ptr = "EGL_ALPHA_MASK_SIZE";
            break;
        case EGL_COLOR_BUFFER_TYPE:
            ptr = "EGL_COLOR_BUFFER_TYPE";
            break;
        case EGL_RENDERABLE_TYPE:
            ptr = "EGL_RENDERABLE_TYPE";
            bStr = EGL_TRUE;
            memset(str, 0, 256);
            if (value & EGL_OPENGL_BIT)
            {
                strcpy(str, "EGL_OPENGL_BIT ");
            }

            if (value & EGL_OPENGL_ES_BIT)
            {
                strcat(str, "EGL_OPENGL_ES_BIT ");
            }

            if (value & EGL_OPENGL_ES2_BIT)
            {
                strcat(str, "EGL_OPENGL_ES2_BIT ");
            }

            if (value & EGL_OPENVG_BIT)
            {
                strcat(str, "EGL_OPENVG_BIT ");
            }

            if (value & EGL_VG_ALPHA_FORMAT_PRE_BIT)
            {
                strcat(str, "EGL_VG_ALPHA_FORMAT_PRE_BIT");
            }
            break;

        case EGL_WIDTH:
            ptr = "EGL_WIDTH";
            break;
        case EGL_HEIGHT:
            ptr = "EGL_HEIGHT";
            break;
        case EGL_CONFORMANT:
            ptr = "EGL_CONFORMANT";
            break;
        case EGL_NONE:
            return;

        default:
            /* Bad attribute. */
            Abort("%s(%d)\n", __FUNCTION__, __LINE__);
            return;
        }

        if (bStr)
        {
            OutputStrcat(output, outputSize, "    %s: %s\n", ptr, str);
        }
        else
        {
            OutputStrcat(output, outputSize, "    %s: %d\n", ptr, value);
        }

        bStr = EGL_FALSE;
    }
    while (attribute != EGL_NONE);

    /* Success. */
    return;
}

CAnalyzer::CAnalyzer()
{
    m_bAnalyzeEnabled   = GL_TRUE;
    m_bErrorCheck       = GL_FALSE;

    m_pOutputFile       = NULL;
    m_pDrawFile         = NULL;
    m_pFrameFile        = NULL;
    m_pShaderFile       = NULL;
    m_pTextureFile      = NULL;

    m_bOutputToShaderFile   = GL_TRUE;
    m_bSaveShader           = GL_FALSE;
}

GLvoid CAnalyzer::OutputToShaderFile(GLchar *output)
{
    if (m_bOutputToShaderFile)
    {
        OutputToFile(GL_FALSE, 0, NULL, output, m_pShaderFile);
    }
}

CAnalyzer::~CAnalyzer()
{
    CloseDumpFile();
}

GLboolean CAnalyzer::OpenDumpFile()
{
    GLchar  strOutputFile[1024];
    GLchar  strDrawFile[1024];
    GLchar  strFrameFile[1024];
    GLchar  strShaderFile[1024];
    GLchar  strTextureFile[1024];

    memset(strOutputFile, 0, 1024);
    memset(strDrawFile, 0, 1024);
    memset(strFrameFile, 0, 1024);

    sprintf(strOutputFile, "%s.hy", strFileName);
    sprintf(strDrawFile, "%s_Draw.hy", strFileName);
    sprintf(strFrameFile, "%s_Frame.hy", strFileName);
    sprintf(strShaderFile, "%s_shader.hy", strFileName);
    sprintf(strTextureFile, "%s_texture.hy", strFileName);

    m_pOutputFile = fopen(strOutputFile, "w");
    if (m_pOutputFile == NULL)
    {
        printf("Cannot create output file: %s\n", strOutputFile);
        goto _Error;
    }

    m_pDrawFile = fopen(strDrawFile, "w");
    if (m_pDrawFile == NULL)
    {
        printf("Cannot create draw file: %s\n", strDrawFile);
        goto _Error;
    }

    m_pFrameFile = fopen(strFrameFile, "w");
    if (m_pFrameFile == NULL)
    {
        printf("Cannot create frame file: %s\n", strFrameFile);
        goto _Error;
    }

    m_pShaderFile = fopen(strShaderFile, "w");
    if (m_pShaderFile == NULL)
    {
        printf("Cannot create shader file: %s\n", strShaderFile);
        goto _Error;
    }

    m_pTextureFile = fopen(strTextureFile, "w");
    if (m_pTextureFile == NULL)
    {
        printf("Cannot create texture file: %s\n", strTextureFile);
        goto _Error;
    }

    APIInitNameMap();

    return GL_TRUE;

_Error:
    return GL_FALSE;
}

GLvoid CAnalyzer::CloseDumpFile()
{
    if (m_pOutputFile)
    {
        fclose(m_pOutputFile);
    }

    if (m_pDrawFile)
    {
        fclose(m_pDrawFile);
    }

    if (m_pFrameFile)
    {
        fclose(m_pFrameFile);
    }

    if (m_pShaderFile)
    {
        fclose(m_pShaderFile);
    }

    if (m_pTextureFile)
    {
        fclose(m_pTextureFile);
    }
}

GLvoid CAnalyzer::InitAnalyzer(const GLchar *filePath)
{
    static GLboolean bInit = GL_FALSE;
    GLchar  *p = new GLchar[256];
    GLint   n = 0;

    if (p == NULL)
        return;

    if (bInit)
        return;

    if (!m_bAnalyzeEnabled)
    {
        delete p;
        return;
    }

    memset(p, 0, 256);
    sprintf(p, "%s/vtracer", filePath);

    n = sizeof(parameter)/sizeof(parameter[0]);
    parameter[1] = p;
    ParseArguments1(n, parameter);

    if (!OpenDumpFile())
    {
        return;
    }

    EGLInit();
    GLES3Init();

    m_nEventSequence            = 0;
    m_nCurrentLoadEventSequence = 0;

    bInit = GL_TRUE;

    delete p;
}

GLvoid CAnalyzer::EnableAnalyze(GLboolean flag)
{
    m_bAnalyzeEnabled = flag;
}

GLboolean CAnalyzer::DumpStringToApiDump(const GLchar *string)
{
    if (!m_bAnalyzeEnabled)
        return GL_FALSE;

    OutputToFile(GL_FALSE, 0, "", string, m_pOutputFile);
    return GL_TRUE;
}

GLvoid CAnalyzer::BeginEvent()
{
    m_nEventSequence ++;
}

GLvoid CAnalyzer::EndEvent(GLESAPIIndex func)
{
    if (m_bErrorCheck)
    {
        ErrorCheck(func);
    }

    if (func >= EGL_API_eglGetError && func <= EGL_API_eglUnlockSurfaceKHR)
    {
        switch (func)
        {
        case EGL_API_eglSwapBuffers:
            OutputToTarget(OUTPUT_TO_FILE, GL_FALSE, g_Analyzer.GetEventSequence(), NULL, Output, m_pFrameFile);
            OutputToTarget(OUTPUT_TO_TOTAL_STRING, GL_FALSE, g_Analyzer.GetEventSequence(), NULL, Output, m_pOutputFile);
            break;

        default:
            OutputToTarget(OUTPUT_TO_TOTAL_STRING, GL_TRUE, g_Analyzer.GetEventSequence(), thread, EGLOutput, m_pOutputFile);
            break;
        }
    }
    else if (func >= GL3_API_glVertexAttribPointer && func <= GL3_API_glVertexBindingDivisor)
    {
        switch (func)
        {
        case GL3_API_glUniformMatrix3fv:
        case GL3_API_glUniformMatrix4fv:
        case GL3_API_glUniform1fv:
        case GL3_API_glUniform2fv:
        case GL3_API_glUniform4fv:
        case GL3_API_glUniform3fv:
        case GL3_API_glGetString:
        case GL3_API_glShaderSource:
            OutputToTarget(OUTPUT_TO_TOTAL_STRING, GL_TRUE, g_Analyzer.GetEventSequence(), thread, Uniform, m_pOutputFile);
            break;

        case GL3_API_glDrawArrays:
        case GL3_API_glDrawElements:
            break;

        default:
            OutputToTarget(OUTPUT_TO_TOTAL_STRING, GL_TRUE, g_Analyzer.GetEventSequence(), thread, GLOutput, m_pOutputFile);
            break;
        }
    }
}

GLboolean CAnalyzer::IsDrawCommand(GLESAPIIndex name)
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

GLvoid CAnalyzer::ErrorCheck(GLESAPIIndex func)
{
    GLchar errMsg[256];

    if (GetAPIIndex(func) == GLES_API_EGL)
    {
        //EGLint err = eglGetError();
        //if(err != EGL_SUCCESS)
        //{
        //    memset(errMsg, 0, 256);
        //    sprintf(errMsg, "err: 0x%04X\n", err);
        //    DumpStringToApiDump((const GLchar*)errMsg);
        //}
    }
    else
    {
        GLenum err = g_opengl->glGetError();
        if(err != GL_NO_ERROR)
        {
            memset(errMsg, 0, 256);
            sprintf(errMsg, "err: 0x%04X(%s)\n", err, ConvertErrCode2String(err));
            DumpStringToApiDump(errMsg);
        }
    }
}

GLvoid CAnalyzer::ReleaseAnalyzer()
{
    CloseDumpFile();
    GLES3Release();
    EGLRelease();
}

const GLchar* CAnalyzer::ConvertErrCode2String(GLenum err)
{
    switch (err)
    {
    case GL_INVALID_ENUM:       return "GL_INVALID_ENUM";
    case GL_INVALID_VALUE:      return "GL_INVALID_VALUE";
    case GL_INVALID_OPERATION:  return "GL_INVALID_OPERATION";
    case GL_OUT_OF_MEMORY:      return "GL_OUT_OF_MEMORY";
    default: return "GL_UNKNOWN_ERROR";
    }
}

GLuint CAnalyzer::GetEventSequence()
{
    return m_nEventSequence;
}

GLvoid CAnalyzer::AnalyzeGenBuffers(GLchar *output, GLint outputSize, GLsizei n, GLuint *buffers)
{
    ANALYZE_BEGIN_EVENT();
    GenDelObjects(CURRENT_CONTEXT1(), output, outputSize, n, buffers, "glGenBuffers", "vbo");
    ANALYZE_END_EVENT(GL3_API_glGenBuffers);
}

GLvoid CAnalyzer::AnalyzeDeleteBuffers(GLchar *output, GLint outputSize, GLsizei n, const GLuint *buffers)
{
    ANALYZE_BEGIN_EVENT();
    GenDelObjects(CURRENT_CONTEXT1(), output, outputSize, n, buffers, "glDeleteBuffers", "vbo");
    ANALYZE_END_EVENT(GL3_API_glDeleteBuffers);
}

GLvoid CAnalyzer::AnalyzeBindBuffer(GLchar *output, GLint outputSize, GLenum target, GLuint buffer)
{
    ANALYZE_BEGIN_EVENT();

    TranslateBufferTarget(target, tmp);
    PrintParams2(output, outputSize, "glBindBuffer",
                 (GLuint)tmp, "", STR_STR,
                 buffer, "buf", INT_04D);

    ANALYZE_END_EVENT(GL3_API_glBindBuffer);
}

GLvoid CAnalyzer::AnalyzeBindBufferBase(GLchar *output, GLint outputSize, GLenum target, GLuint index, GLuint buffer)
{
    ANALYZE_BEGIN_EVENT();

    TranslateBufferTarget(target, tmp1);
    sprintf(output, "glBindBufferBase(%s, index=%d, buf=%04d)\n", tmp1, index, buffer);

    ANALYZE_END_EVENT(GL3_API_glBindBufferBase);
}

GLvoid CAnalyzer::AnalyzeBindBufferRange(GLchar *output, GLint outputSize, GLenum target, GLuint index, GLuint buffer, GLintptr offset, GLsizeiptr size)
{
    ANALYZE_BEGIN_EVENT();

    TranslateBufferTarget(target, tmp1);
    sprintf(output, "glBindBufferRange(%s, index=%d, buf=%04d, offset=%d, size=%d)\n", tmp1, index, buffer, (GLint)offset, (GLint)size);

    ANALYZE_END_EVENT(GL3_API_glBindBufferRange);
}

GLvoid CAnalyzer::AnalyzeBufferData(GLchar *output, GLint outputSize, GLenum target, GLsizeiptr size, const void *data, GLenum usage)
{
    ANALYZE_BEGIN_EVENT();

    TranslateBufferTarget(target, tmp);
    TranslateUsageToString(usage, tmp1);
    sprintf(output, "glBufferData(%s, size=0x%08X, data=0x%08X, %s)\n", tmp, size, data, tmp1);

    ANALYZE_END_EVENT(GL3_API_glBufferData);
}

GLvoid CAnalyzer::AnalyzeBufferSubData(GLchar *output, GLint outputSize, GLenum target, GLintptr offset, GLsizeiptr size, const void *data)
{
    ANALYZE_BEGIN_EVENT();

    TranslateBufferTarget(target, tmp);
    sprintf(output, "glBufferSubData(%s, offset=0x%08X, size=0x%08X, data=0x%08X)\n", tmp, offset, size, data);

    ANALYZE_END_EVENT(GL3_API_glBufferSubData);
}

GLvoid CAnalyzer::AnalyzeDeleteVertexArrays(GLchar *output, GLint outputSize, GLsizei n, const GLuint *arrays)
{
    ANALYZE_BEGIN_EVENT();
    GenDelObjects(CURRENT_CONTEXT1(), output, outputSize, n, arrays, "glDeleteVertexArrays", "vao");
    ANALYZE_END_EVENT(GL3_API_glDeleteVertexArrays);
}

GLvoid CAnalyzer::AnalyzeGenVertexArrays(GLchar *output, GLint outputSize, GLsizei n, GLuint *arrays)
{
    ANALYZE_BEGIN_EVENT();
    GenDelObjects(CURRENT_CONTEXT1(), output, outputSize, n, arrays, "glGenVertexArrays", "vao");
    ANALYZE_END_EVENT(GL3_API_glGenVertexArrays);
}

GLvoid CAnalyzer::AnalyzeBindVertexArray(GLchar *output, GLint outputSize, GLuint array)
{
    ANALYZE_BEGIN_EVENT();
    sprintf(output, "glBindVertexArray(vao=%04d)\n", array);
    ANALYZE_END_EVENT(GL3_API_glBindVertexArray);
}

GLvoid CAnalyzer::AnalyzeBindTransformFeedback(GLchar *output, GLint outputSize, GLenum target, GLuint id)
{
    ANALYZE_BEGIN_EVENT();

    TranslateGetName(target, tmp1);
    sprintf(output, "glBindTransformFeedback(%s, id=%04d)\n", tmp1, id);

    ANALYZE_END_EVENT(GL3_API_glBindTransformFeedback);
}

GLvoid CAnalyzer::AnalyzeDeleteTransformFeedbacks(GLchar *output, GLint outputSize, GLsizei n, const GLuint *ids)
{
    ANALYZE_BEGIN_EVENT();
    GenDelObjects(CURRENT_CONTEXT1(), output, outputSize, n, ids, "glDeleteTransformFeedbacks", "tfb");
    ANALYZE_END_EVENT(GL3_API_glDeleteTransformFeedbacks);
}

GLvoid CAnalyzer::AnalyzeGenTransformFeedbacks(GLchar *output, GLint outputSize, GLsizei n, GLuint *ids)
{
    ANALYZE_BEGIN_EVENT();
    GenDelObjects(CURRENT_CONTEXT1(), output, outputSize, n, ids, "glGenTransformFeedbacks", "tfb");
    ANALYZE_END_EVENT(GL3_API_glGenTransformFeedbacks);
}

GLvoid CAnalyzer::AnalyzeIsTransformFeedback(GLchar *output, GLint outputSize, GLuint id, GLboolean res)
{
    ANALYZE_BEGIN_EVENT();
    ANALYZE_END_EVENT(GL3_API_glIsTransformFeedback);
}

GLvoid CAnalyzer::AnalyzeBeginTransformFeedback(GLchar *output, GLint outputSize, GLenum primitiveMode)
{
    ANALYZE_BEGIN_EVENT();

    TranslateDrawMode(primitiveMode, tmp1);
    sprintf(output, "glBeginTransformFeedback(%s)\n", tmp1);

    ANALYZE_END_EVENT(GL3_API_glBeginTransformFeedback);
}

GLvoid CAnalyzer::AnalyzeEndTransformFeedback(GLchar *output, GLint outputSize)
{
    ANALYZE_BEGIN_EVENT();
    sprintf(output, "glEndTransformFeedback()\n");
    ANALYZE_END_EVENT(GL3_API_glEndTransformFeedback);
}

GLvoid CAnalyzer::AnalyzePauseTransformFeedback(GLchar *output, GLint outputSize)
{
    ANALYZE_BEGIN_EVENT();
    sprintf(output, "glPauseTransformFeedback()\n");
    ANALYZE_END_EVENT(GL3_API_glPauseTransformFeedback);
}

GLvoid CAnalyzer::AnalyzeResumeTransformFeedback(GLchar *output, GLint outputSize)
{
    ANALYZE_BEGIN_EVENT();
    sprintf(output, "glResumeTransformFeedback()\n");
    ANALYZE_END_EVENT(GL3_API_glResumeTransformFeedback);
}

GLvoid CAnalyzer::AnalyzeGetTransformFeedbackVarying(GLchar *output, GLint outputSize, GLuint program, GLuint index, GLsizei bufSize, GLsizei *length, GLsizei *size, GLenum *type, GLchar *name)
{
    ANALYZE_BEGIN_EVENT();
    sprintf(output, "glGetTransformFeedbackVarying() ***todo\n");
    ANALYZE_END_EVENT(GL3_API_glGetTransformFeedbackVarying);
}

GLvoid CAnalyzer::AnalyzeDisableVertexAttribArray(GLchar *output, GLint outputSize, GLuint index)
{
    ANALYZE_BEGIN_EVENT();
    PrintParams1(output, outputSize, "glDisableVertexAttribArray", (GLuint)index, "attrib", INT_04D);
    ANALYZE_END_EVENT(GL3_API_glDisableVertexAttribArray);
}

GLvoid CAnalyzer::AnalyzeEnableVertexAttribArray(GLchar *output, GLint outputSize, GLuint index)
{
    ANALYZE_BEGIN_EVENT();
    PrintParams1(output, outputSize, "glEnableVertexAttribArray", (GLuint)index, "attrib", INT_04D);
    ANALYZE_END_EVENT(GL3_API_glEnableVertexAttribArray);
}

GLvoid CAnalyzer::AnalyzeVertexAttrib1f(GLchar *output, GLint outputSize, GLuint index, GLfloat x)
{
    ANALYZE_BEGIN_EVENT();
    ANALYZE_END_EVENT(GL3_API_glVertexAttrib1f);
}

GLvoid CAnalyzer::AnalyzeVertexAttrib1fv(GLchar *output, GLint outputSize, GLuint index, const GLfloat *v)
{
    ANALYZE_BEGIN_EVENT();
    ANALYZE_END_EVENT(GL3_API_glVertexAttrib1fv);
}

GLvoid CAnalyzer::AnalyzeVertexAttrib2f(GLchar *output, GLint outputSize, GLuint index, GLfloat x, GLfloat y)
{
    ANALYZE_BEGIN_EVENT();
    ANALYZE_END_EVENT(GL3_API_glVertexAttrib2f);
}

GLvoid CAnalyzer::AnalyzeVertexAttrib2fv(GLchar *output, GLint outputSize, GLuint index, const GLfloat *v)
{
    ANALYZE_BEGIN_EVENT();
    ANALYZE_END_EVENT(GL3_API_glVertexAttrib2fv);
}

GLvoid CAnalyzer::AnalyzeVertexAttrib3f(GLchar *output, GLint outputSize, GLuint index, GLfloat x, GLfloat y, GLfloat z)
{
    ANALYZE_BEGIN_EVENT();
    ANALYZE_END_EVENT(GL3_API_glVertexAttrib3f);
}

GLvoid CAnalyzer::AnalyzeVertexAttrib3fv(GLchar *output, GLint outputSize, GLuint index, const GLfloat *v)
{
    ANALYZE_BEGIN_EVENT();
    ANALYZE_END_EVENT(GL3_API_glVertexAttrib3fv);
}

GLvoid CAnalyzer::AnalyzeVertexAttrib4f(GLchar *output, GLint outputSize, GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w)
{
    ANALYZE_BEGIN_EVENT();
    ANALYZE_END_EVENT(GL3_API_glVertexAttrib4f);
}

GLvoid CAnalyzer::AnalyzeVertexAttrib4fv(GLchar *output, GLint outputSize, GLuint index, const GLfloat *v)
{
    ANALYZE_BEGIN_EVENT();
    ANALYZE_END_EVENT(GL3_API_glVertexAttrib4fv);
}

GLvoid CAnalyzer::AnalyzeVertexAttribPointer(GLchar *output, GLint outputSize, GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void *pointer)
{
    ANALYZE_BEGIN_EVENT();

    TranslateDataType(type, tmp);
    sprintf(output, "glVertexAttribPointer(attrib=%d, size=0x%08X, n=%d, stride=%d, p=0x%08X, %s)\n",
        index, size, normalized, stride, pointer, tmp);

    ANALYZE_END_EVENT(GL3_API_glVertexAttribPointer);
}

GLvoid CAnalyzer::AnalyzeVertexAttribIPointer(GLchar *output, GLint outputSize, GLuint index, GLint size, GLenum type, GLsizei stride, const void *pointer)
{
    ANALYZE_BEGIN_EVENT();

    TranslateDataType(type, tmp);
    sprintf(output, "glVertexAttribIPointer(attrib=%d, size=0x%08X, stride=%d, p=0x%08X, %s)\n",
        index, size, stride, pointer, tmp);

    ANALYZE_END_EVENT(GL3_API_glVertexAttribIPointer);
}

GLvoid CAnalyzer::AnalyzeGetBufferParameteriv(GLchar *output, GLint outputSize, GLenum target, GLenum pname, GLint *params)
{
    ANALYZE_BEGIN_EVENT();
    sprintf(output, "glGetBufferParameteriv() ********todo\n");
    ANALYZE_END_EVENT(GL3_API_glGetBufferParameteriv);
}

GLvoid CAnalyzer::AnalyzeGetVertexAttribfv(GLchar *output, GLint outputSize, GLuint index, GLenum pname, GLfloat *params)
{
    ANALYZE_BEGIN_EVENT();

    TranslateAttribStateName(pname, tmp5);
    sprintf(output, "glGetVertexAttribfv(index=%d, %s, %f)\n", index, tmp5, *params);

    ANALYZE_END_EVENT(GL3_API_glGetVertexAttribfv);
}

GLvoid CAnalyzer::AnalyzeGetVertexAttribiv(GLchar *output, GLint outputSize, GLuint index, GLenum pname, GLint *params)
{
    ANALYZE_BEGIN_EVENT();

    TranslateAttribStateName(pname, tmp5);
    sprintf(output, "glGetVertexAttribiv(index=%d, %s, %d)\n", index, tmp5, *params);

    ANALYZE_END_EVENT(GL3_API_glGetVertexAttribiv);
}

GLvoid CAnalyzer::AnalyzeGetVertexAttribPointerv(GLchar *output, GLint outputSize, GLuint index, GLenum pname, void **pointer)
{
    ANALYZE_BEGIN_EVENT();
    TranslateGetName(pname, tmp2);
    sprintf(output, "glGetVertexAttribPointerv(index=%d, %s, 0x%08X)\n", index, tmp2, pointer);
    ANALYZE_END_EVENT(GL3_API_glGetVertexAttribPointerv);
}

GLvoid CAnalyzer::AnalyzeCreateShader(GLchar *output, GLint outputSize, GLenum type, GLuint shader)
{
    ANALYZE_BEGIN_EVENT();

    TranslateShaderType(type, tmp);
    sprintf(output, "glCreateShader(%s)=%08d\n", tmp, shader);
    OutputToShaderFile(output);

    ANALYZE_END_EVENT(GL3_API_glCreateShader);
}

GLvoid CAnalyzer::AnalyzeShaderSource(GLchar *output, GLint outputSize, GLuint shader, GLsizei count, const GLchar *const*string, const GLint *length)
{
    ANALYZE_BEGIN_EVENT();

    CShaderObj *pShader = CURRENT_CONTEXT1()->FindShader(shader);

    sprintf(output, "\n*************************************shader*************************************\n");
    OutputStrcat(output, outputSize, "glShaderSource(shader = %08d)\n\n", shader);
    OutputStrcat(output, outputSize, "%s\n", pShader->m_source, m_pOutputFile);
    OutputStrcat(output, outputSize, "*************************************shader end*********************************\n\n");
    OutputToShaderFile(output);

    if (m_bSaveShader)
    {
        if (pShader->m_source != NULL)
        {
            SaveShaderToFile(shader, pShader->m_type, 1, &pShader->m_source);
        }
    }

    ANALYZE_END_EVENT(GL3_API_glShaderSource);
}

GLvoid CAnalyzer::AnalyzeDeleteShader(GLchar *output, GLint outputSize, GLuint shader)
{
    ANALYZE_BEGIN_EVENT();
    PrintParams1(output, outputSize, "glDeleteShader", (GLuint)shader, "sh", INT_08D);
    ANALYZE_END_EVENT(GL3_API_glDeleteShader);
}

GLvoid CAnalyzer::AnalyzeCreateProgram(GLchar *output, GLint outputSize, GLuint program)
{
    ANALYZE_BEGIN_EVENT();

    sprintf(output, "glCreateProgram()=%08d\n", program);
    OutputToShaderFile(output);

    ANALYZE_END_EVENT(GL3_API_glCreateProgram);
}

GLvoid CAnalyzer::AnalyzeUseProgram(GLchar *output, GLint outputSize, GLuint program)
{
    ANALYZE_BEGIN_EVENT();
    PrintParams1(output, outputSize, "glUseProgram", program, "prog", INT_08D);
    ANALYZE_END_EVENT(GL3_API_glUseProgram);
}

GLvoid CAnalyzer::AnalyzeAttachShader(GLchar *output, GLint outputSize, GLuint program, GLuint shader)
{
    ANALYZE_BEGIN_EVENT();

    CShaderObj  *pShader    = CURRENT_CONTEXT1()->FindShader(shader);
    GLenum      shaderType  = pShader?pShader->m_type:GL_FRAGMENT_SHADER;

    if (shaderType == GL_FRAGMENT_SHADER)
    {
        PrintParams2(output, outputSize, "glAttachShader",
            program, "prog", INT_08D,
            shader, "ps", INT_08D);
    }
    else if (shaderType == GL_VERTEX_SHADER)
    {
        PrintParams2(output, outputSize, "glAttachShader",
            program, "prog", INT_08D,
            shader, "vs", INT_08D);
    }
    else
    {
        Abort("%s(%d)\n", __FUNCTION__, __LINE__);
    }

    ANALYZE_END_EVENT(GL3_API_glAttachShader);
}

GLvoid CAnalyzer::AnalyzeGetUniformLocation(GLchar *output, GLint outputSize, GLuint program, const GLchar *name, GLint loc)
{
    ANALYZE_BEGIN_EVENT();

    CProgramObj *prog       = CURRENT_CONTEXT1()->FindProgram(program);
    CUniformObj *uniform    = prog->GetUniformByName(name);

    if (uniform == NULL)
    {
        sprintf(output, "glGetUniformLocation(prog=%08d, %s) invalid(%d)\n", program, name, loc);
        goto _Err;
    }

    if (name == NULL)
    {
        sprintf(output, "glGetUniformLocation(prog=%08d, NULL)\n", program);
    }
    else
    {
        sprintf(output, "glGetUniformLocation(prog=%08d, %s)=%d\n", program, name, loc);
    }

_Err:
    OutputToShaderFile(output);
    ANALYZE_END_EVENT(GL3_API_glGetUniformLocation);
}

GLvoid CAnalyzer::AnalyzeLinkProgram(GLchar *output, GLint outputSize, GLuint program)
{
    ANALYZE_BEGIN_EVENT();
    PrintParams1(output, outputSize, "glLinkProgram", (GLuint)program, "prog", INT_08D);
    ANALYZE_END_EVENT(GL3_API_glLinkProgram);
}

GLvoid CAnalyzer::AnalyzeCompileShader(GLchar *output, GLint outputSize, GLuint shader)
{
    ANALYZE_BEGIN_EVENT();
    PrintParams1(output, outputSize, "glCompileShader", (GLuint)shader, "sh", INT_08D);
    ANALYZE_END_EVENT(GL3_API_glCompileShader);
}

GLvoid CAnalyzer::AnalyzeDetachShader(GLchar *output, GLint outputSize, GLuint program, GLuint shader)
{
    ANALYZE_BEGIN_EVENT();
    PrintParams2(output, outputSize, "glDetachShader",
        (GLuint)program, "prog", INT_04D,
        (GLuint)shader, "sh", INT_04D);
    ANALYZE_END_EVENT(GL3_API_glDetachShader);
}

GLvoid CAnalyzer::AnalyzeDeleteProgram(GLchar *output, GLint outputSize, GLuint program)
{
    ANALYZE_BEGIN_EVENT();
    PrintParams1(output, outputSize, "glDeleteProgram", (GLuint)program, "prog", INT_08D);
    ANALYZE_END_EVENT(GL3_API_glDeleteProgram);
}

GLvoid CAnalyzer::AnalyzeGetAttribLocation(GLchar *output, GLint outputSize, GLuint program, const GLchar *name, GLint oldLoc)
{
    ANALYZE_BEGIN_EVENT();

    if (name)
    {
        sprintf(output, "glGetAttribLocation(prog=%08d, %s) = %d\n", program, name, oldLoc);
    }
    else
    {
        sprintf(output, "glGetAttribLocation(prog=%08d, null) = %d, data error\n", program, oldLoc);
    }

    OutputToShaderFile(output);

    ANALYZE_END_EVENT(GL3_API_glGetAttribLocation);
}

GLvoid CAnalyzer::AnalyzeBindAttribLocation(GLchar *output, GLint outputSize, GLuint program, GLint loc, const GLchar *name)
{
    ANALYZE_BEGIN_EVENT();

    sprintf(output, "glBindAttribLocation(prog=%08d, index=%d, %s)\n", program, loc, name);
    OutputToShaderFile(output);

    ANALYZE_END_EVENT(GL3_API_glBindAttribLocation);
}

GLvoid CAnalyzer::AnalyzeProgramBinary(GLchar *output, GLint outputSize, GLuint program, GLenum binaryFormat, const void *binary, GLsizei length)
{
    ANALYZE_BEGIN_EVENT();
    GLchar *str = NULL;

    switch (binaryFormat)
    {
    case GL_Z400_BINARY_AMD:
        str = "amd_binary";
        break;

    case GL_PROGRAM_BINARY_VIV:
        str = "viv_binary";
        break;

    default:
        sprintf(output, "glProgramBinary(prog=%04d, wrong binary format(0x%08X)\n", program, binaryFormat);
        goto _Err;
    }

    sprintf(output, "glProgramBinary(prog=%08d, %s, size=%d)\n", program, str, length);

_Err:
    ANALYZE_END_EVENT(GL3_API_glProgramBinary);
}

GLvoid CAnalyzer::AnalyzeUniform1f(GLchar *output, GLint outputSize, GLint location, GLfloat v0)
{
    ANALYZE_BEGIN_EVENT();
    PrintUniform1234if(output, outputSize, location, DATA_TYPE_FLOAT_X1, 1, (GLuint*)&v0, "glUniform1f");
    ANALYZE_END_EVENT(GL3_API_glUniform1f);
}

GLvoid CAnalyzer::AnalyzeUniform1fv(GLchar *output, GLint outputSize, GLint location, GLsizei count, const GLfloat *value)
{
    ANALYZE_BEGIN_EVENT();
    GLint pos = 0;
    PrintUniform1234ifv(&pos, output, outputSize, location, DATA_TYPE_FLOAT_X1, 1, count, (GLuint)value, "glUniform1fv");
    ANALYZE_END_EVENT(GL3_API_glUniform1fv);
}

GLvoid CAnalyzer::AnalyzeUniform1i(GLchar *output, GLint outputSize, GLint location, GLint v0)
{
    ANALYZE_BEGIN_EVENT();
    PrintUniform1234if(output, outputSize, location, DATA_TYPE_INTEGER_X1, 1, (GLuint*)&v0, "glUniform1i");
    ANALYZE_END_EVENT(GL3_API_glUniform1i);
}

GLvoid CAnalyzer::AnalyzeUniform1iv(GLchar *output, GLint outputSize, GLint location, GLsizei count, const GLint *value)
{
    ANALYZE_BEGIN_EVENT();
    GLint pos = 0;
    PrintUniform1234ifv(&pos, output, outputSize, location, DATA_TYPE_INTEGER_X1, 1, count, (GLuint)value, "glUniform1iv");
    ANALYZE_END_EVENT(GL3_API_glUniform1iv);
}

GLvoid CAnalyzer::AnalyzeUniform2f(GLchar *output, GLint outputSize, GLint location, GLfloat v0, GLfloat v1)
{
    ANALYZE_BEGIN_EVENT();
    GLfloat arg[2] = {v0, v1};
    PrintUniform1234if(output, outputSize, location, DATA_TYPE_FLOAT_X2, 1, (GLuint*)&arg[0], "glUniform2f");
    ANALYZE_END_EVENT(GL3_API_glUniform2f);
}

GLvoid CAnalyzer::AnalyzeUniform2fv(GLchar *output, GLint outputSize, GLint location, GLsizei count, const GLfloat *value)
{
    ANALYZE_BEGIN_EVENT();
    GLint pos = 0;
    PrintUniform1234ifv(&pos, output, outputSize, location, DATA_TYPE_FLOAT_X2, 2, count, (GLuint)value, "glUniform2fv");
    ANALYZE_END_EVENT(GL3_API_glUniform2fv);
}

GLvoid CAnalyzer::AnalyzeUniform2i(GLchar *output, GLint outputSize, GLint location, GLint v0, GLint v1)
{
    ANALYZE_BEGIN_EVENT();
    GLint arg[2] = {v0, v1};
    PrintUniform1234if(output, outputSize, location, DATA_TYPE_INTEGER_X2, 1, (GLuint*)&arg[0], "glUniform2i");
    ANALYZE_END_EVENT(GL3_API_glUniform2i);
}

GLvoid CAnalyzer::AnalyzeUniform2iv(GLchar *output, GLint outputSize, GLint location, GLsizei count, const GLint *value)
{
    ANALYZE_BEGIN_EVENT();
    GLint pos = 0;
    PrintUniform1234ifv(&pos, output, outputSize, location, DATA_TYPE_INTEGER_X2, 2, count, (GLuint)value, "glUniform2iv");
    ANALYZE_END_EVENT(GL3_API_glUniform2iv);
}

GLvoid CAnalyzer::AnalyzeUniform3f(GLchar *output, GLint outputSize, GLint location, GLfloat v0, GLfloat v1, GLfloat v2)
{
    ANALYZE_BEGIN_EVENT();
    GLfloat arg[3] = {v0, v1, v2};
    PrintUniform1234if(output, outputSize, location, DATA_TYPE_FLOAT_X3, 1, (GLuint*)&arg[0], "glUniform3f");
    ANALYZE_END_EVENT(GL3_API_glUniform3f);
}

GLvoid CAnalyzer::AnalyzeUniform3fv(GLchar *output, GLint outputSize, GLint location, GLsizei count, const GLfloat *value)
{
    ANALYZE_BEGIN_EVENT();
    GLint pos = 0;
    PrintUniform1234ifv(&pos, output, outputSize, location, DATA_TYPE_FLOAT_X3, 3, count, (GLuint)value, "glUniform3fv");
    ANALYZE_END_EVENT(GL3_API_glUniform3fv);
}

GLvoid CAnalyzer::AnalyzeUniform3i(GLchar *output, GLint outputSize, GLint location, GLint v0, GLint v1, GLint v2)
{
    ANALYZE_BEGIN_EVENT();
    GLint arg[3] = {v0, v1, v2};
    PrintUniform1234if(output, outputSize, location, DATA_TYPE_INTEGER_X3, 1, (GLuint*)&arg[0], "glUniform3i");
    ANALYZE_END_EVENT(GL3_API_glUniform3i);
}

GLvoid CAnalyzer::AnalyzeUniform3iv(GLchar *output, GLint outputSize, GLint location, GLsizei count, const GLint *value)
{
    ANALYZE_BEGIN_EVENT();
    GLint pos = 0;
    PrintUniform1234ifv(&pos, output, outputSize, location, DATA_TYPE_INTEGER_X3, 3, count, (GLuint)value, "glUniform3iv");
    ANALYZE_END_EVENT(GL3_API_glUniform3iv);
}

GLvoid CAnalyzer::AnalyzeUniform4f(GLchar *output, GLint outputSize, GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3)
{
    ANALYZE_BEGIN_EVENT();
    GLfloat arg[4] = {v0, v1, v2, v3};
    PrintUniform1234if(output, outputSize, location, DATA_TYPE_FLOAT_X4, 1, (GLuint*)&arg[0], "glUniform4f");
    ANALYZE_END_EVENT(GL3_API_glUniform4f);
}

GLvoid CAnalyzer::AnalyzeUniform4fv(GLchar *output, GLint outputSize, GLint location, GLsizei count, const GLfloat *value)
{
    ANALYZE_BEGIN_EVENT();
    GLint pos = 0;
    PrintUniform1234ifv(&pos, output, outputSize, location, DATA_TYPE_FLOAT_X4, 4, count, (GLuint)value, "glUniform4fv");
    ANALYZE_END_EVENT(GL3_API_glUniform4fv);
}

GLvoid CAnalyzer::AnalyzeUniform4i(GLchar *output, GLint outputSize, GLint location, GLint v0, GLint v1, GLint v2, GLint v3)
{
    ANALYZE_BEGIN_EVENT();
    GLint arg[4] = {v0, v1, v2, v3};
    PrintUniform1234if(output, outputSize, location, DATA_TYPE_INTEGER_X4, 1, (GLuint*)&arg[0], "glUniform4i");
    ANALYZE_END_EVENT(GL3_API_glUniform4i);
}

GLvoid CAnalyzer::AnalyzeUniform4iv(GLchar *output, GLint outputSize, GLint location, GLsizei count, const GLint *value)
{
    ANALYZE_BEGIN_EVENT();
    GLint pos = 0;
    PrintUniform1234ifv(&pos, output, outputSize, location, DATA_TYPE_INTEGER_X4, 4, count, (GLuint)value, "glUniform4iv");
    ANALYZE_END_EVENT(GL3_API_glUniform4iv);
}

GLvoid CAnalyzer::AnalyzeUniformMatrix2fv(GLchar *output, GLint outputSize, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value)
{
    ANALYZE_BEGIN_EVENT();
    GLint pos = 0;
    PrintUniformMatrix234fv(&pos, output, outputSize, location, DATA_TYPE_FLOAT_2X2,
        4, count, (GLuint)value, transpose, "glUniformMatrix2fv");
    ANALYZE_END_EVENT(GL3_API_glUniformMatrix2fv);
}

GLvoid CAnalyzer::AnalyzeUniformMatrix3fv(GLchar *output, GLint outputSize, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value)
{
    ANALYZE_BEGIN_EVENT();
    GLint pos = 0;
    PrintUniformMatrix234fv(&pos, output, outputSize, location, DATA_TYPE_FLOAT_3X3,
        9, count, (GLuint)value, transpose, "glUniformMatrix3fv");
    ANALYZE_END_EVENT(GL3_API_glUniformMatrix3fv);
}

GLvoid CAnalyzer::AnalyzeUniformMatrix4fv(GLchar *output, GLint outputSize, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value)
{
    ANALYZE_BEGIN_EVENT();
    GLint pos = 0;
    PrintUniformMatrix234fv(&pos, output, outputSize, location, DATA_TYPE_FLOAT_4X4,
        16, count, (GLuint)value, transpose, "glUniformMatrix4fv");
    ANALYZE_END_EVENT(GL3_API_glUniformMatrix4fv);
}

GLvoid CAnalyzer::AnalyzeGetActiveAttrib(GLchar *output, GLint outputSize, GLuint program, GLuint index, GLsizei bufSize, GLsizei *length, GLint *size, GLenum *type, GLchar *name)
{
    ANALYZE_BEGIN_EVENT();

    if (type)
    {
        TranslateShaderDataType(*type, tmp1);
    }
    else
    {
        sprintf(tmp1, "invalid");
    }

    sprintf(output, "glGetActiveAttrib(prog=%08d, index=%d, bufsize=%d, length=%d, size=%d, %s, %s)\n",
        program, index, bufSize,
        length?*length:0,
        size?*size:0,
        tmp1,
        name?name:"invalid");
    OutputToShaderFile(output);

    ANALYZE_END_EVENT(GL3_API_glGetActiveAttrib);
}

GLvoid CAnalyzer::AnalyzeGetActiveUniform(GLchar *output, GLint outputSize, GLuint program, GLuint index, GLsizei bufSize, GLsizei *length, GLint *size, GLenum *type, GLchar *name)
{
    ANALYZE_BEGIN_EVENT();

    if (type)
    {
        TranslateShaderDataType(*type, tmp1);
    }
    else
    {
        sprintf(tmp1, "invalid");
    }

    sprintf(output, "glGetActiveUniform(prog=%08d, index=%d, size=%d, length=%d, size=%d, %s, %s)\n",
        program, index, bufSize,
        length?*length:0,
        size?*size:0,
        tmp1,
        name?name:"invalid");

    OutputToShaderFile(output);

    ANALYZE_END_EVENT(GL3_API_glGetActiveUniform);
}

GLvoid CAnalyzer::AnalyzeGetAttachedShaders(GLchar *output, GLint outputSize, GLuint program, GLsizei maxCount, GLsizei *count, GLuint *shaders)
{
    ANALYZE_BEGIN_EVENT();
    sprintf(output, "glGetAttachedShaders(prog=%04d, max=%d, count=%d) =", program, maxCount, count);
    for (GLsizei i=0; i<*count; i++)
    {
        OutputStrcat(output, outputSize, " %04d,", shaders[i]);
    }
    ANALYZE_END_EVENT(GL3_API_glGetAttachedShaders);
}

GLvoid CAnalyzer::AnalyzeGetProgramiv(GLchar *output, GLint outputSize, GLuint program, GLenum pname, GLint *params)
{
    ANALYZE_BEGIN_EVENT();

    TranslateProgramInfo(pname, tmp);
    sprintf(output, "glGetProgramiv(prog=%08d, %s, value=%d)\n", program, tmp, *params);

    ANALYZE_END_EVENT(GL3_API_glGetProgramiv);
}

GLvoid CAnalyzer::AnalyzeGetProgramInfoLog(GLchar *output, GLint outputSize, GLuint program, GLsizei bufSize, GLsizei *length, GLchar *infoLog)
{
    ANALYZE_BEGIN_EVENT();
    sprintf(output, "glGetProgramInfoLog(program = %04d, %s)\n", program, infoLog);
    ANALYZE_END_EVENT(GL3_API_glGetProgramInfoLog);
}

GLvoid CAnalyzer::AnalyzeGetShaderiv(GLchar *output, GLint outputSize, GLuint shader, GLenum pname, GLint *params)
{
    ANALYZE_BEGIN_EVENT();
    TranslateShaderInfo(pname, tmp);
    sprintf(output, "glGetShaderiv(shader=%08d, %s, value=%d)\n", shader, tmp, *params);
    ANALYZE_END_EVENT(GL3_API_glGetShaderiv);
}

GLvoid CAnalyzer::AnalyzeGetShaderInfoLog(GLchar *output, GLint outputSize, GLuint shader, GLsizei bufSize, GLsizei *length, GLchar *infoLog)
{
    ANALYZE_BEGIN_EVENT();
    sprintf(output, "glGetShaderInfoLog(shader=%08d, %s)\n", shader, infoLog?infoLog:"");
    ANALYZE_END_EVENT(GL3_API_glGetShaderInfoLog);
}

GLvoid CAnalyzer::AnalyzeGetShaderPrecisionFormat(GLchar *output, GLint outputSize, GLenum shadertype, GLenum precisiontype, GLint *range, GLint *precision)
{
    ANALYZE_BEGIN_EVENT();
    sprintf(output, "glGetShaderPrecisionFormat() ********todo\n");
    ANALYZE_END_EVENT(GL3_API_glGetShaderPrecisionFormat);
}

GLvoid CAnalyzer::AnalyzeGetShaderSource(GLchar *output, GLint outputSize, GLuint shader, GLsizei bufSize, GLsizei *length, GLchar *source)
{
    ANALYZE_BEGIN_EVENT();
    sprintf(output, "glGetShaderSource(shader=%08d)\n", shader);
    ANALYZE_END_EVENT(GL3_API_glGetShaderSource);
}

GLvoid CAnalyzer::AnalyzeGetUniformfv(GLchar *output, GLint outputSize, GLuint program, GLint location, GLfloat *params)
{
    ANALYZE_BEGIN_EVENT();
    sprintf(output, "glGetUniformfv() ********todo\n");
    ANALYZE_END_EVENT(GL3_API_glGetUniformfv);
}

GLvoid CAnalyzer::AnalyzeGetUniformiv(GLchar *output, GLint outputSize, GLuint program, GLint location, GLint *params)
{
    ANALYZE_BEGIN_EVENT();
    sprintf(output, "glGetUniformiv() ********todo\n");
    ANALYZE_END_EVENT(GL3_API_glGetUniformiv);
}

GLvoid CAnalyzer::AnalyzeShaderBinary(GLchar *output, GLint outputSize, GLsizei count, const GLuint *shaders, GLenum binaryformat, const void *binary, GLsizei length)
{
    ANALYZE_BEGIN_EVENT();
    ANALYZE_END_EVENT(GL3_API_glShaderBinary);
}

GLvoid CAnalyzer::AnalyzeValidateProgram(GLchar *output, GLint outputSize, GLuint program)
{
    ANALYZE_BEGIN_EVENT();
    PrintParams1(output, outputSize, "glValidateProgram", (GLuint)program, "prog", INT_04D);
    ANALYZE_END_EVENT(GL3_API_glValidateProgram);
}

GLvoid CAnalyzer::AnalyzeActiveTexture(GLchar *output, GLint outputSize, GLenum texture)
{
    ANALYZE_BEGIN_EVENT();
    PrintParams1(output, outputSize, "glActiveTexture", texture - GL_TEXTURE0, "unit", INT_04D);
    ANALYZE_END_EVENT(GL3_API_glActiveTexture);
}

GLvoid CAnalyzer::AnalyzeDeleteTextures(GLchar *output, GLint outputSize, GLsizei n, const GLuint *textures)
{
    ANALYZE_BEGIN_EVENT();
    GenDelObjects(CURRENT_CONTEXT1(), output, outputSize, n, textures, "glDeleteTextures", "tex");
    ANALYZE_END_EVENT(GL3_API_glDeleteTextures);
}

GLvoid CAnalyzer::AnalyzeBindTexture(GLchar *output, GLint outputSize, GLenum target, GLuint texture)
{
    ANALYZE_BEGIN_EVENT();

    GLchar *ptr = NULL;

    switch (target)
    {
    case GL_TEXTURE_2D:
        ptr = "GL_TEXTURE_2D";
        break;

    case GL_TEXTURE_3D:
        ptr = "GL_TEXTURE_3D";
        break;

    case GL_TEXTURE_CUBE_MAP:
        ptr = "GL_TEXTURE_CUBE_MAP";
        break;

    case GL_TEXTURE_EXTERNAL_OES:
        ptr = "GL_TEXTURE_EXTERNAL_OES";
        break;

    default:
        ptr = "Invalid";
        break;
    }

    PrintParams2(output, outputSize, "glBindTexture",
        (GLuint)ptr, "", STR_STR,
        texture, "tex", INT_04D);

    ANALYZE_END_EVENT(GL3_API_glBindTexture);
}

GLvoid CAnalyzer::AnalyzeGenTextures(GLchar *output, GLint outputSize, GLsizei n, GLuint *textures)
{
    ANALYZE_BEGIN_EVENT();
    GenDelObjects(CURRENT_CONTEXT1(), output, outputSize, n, textures, "glGenTextures", "tex");
    ANALYZE_END_EVENT(GL3_API_glGenTextures);
}

GLvoid CAnalyzer::AnalyzeGenerateMipmap(GLchar *output, GLint outputSize, GLenum target)
{
    ANALYZE_BEGIN_EVENT();
    TranslateTexTarget(target, tmp);
    PrintParams1(output, outputSize, "glGenerateMipmap", (GLuint)tmp, "", STR_STR);
    ANALYZE_END_EVENT(GL3_API_glGenerateMipmap);
}

GLvoid CAnalyzer::AnalyzeTexImage2D(GLchar *output, GLint outputSize, GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const void *pixels)
{
    ANALYZE_BEGIN_EVENT();

    CTexObj     *pTexObj    = CURRENT_CONTEXT1()->GetActiveTexObj(target);
    stTexImage  *pTexImg    = pTexObj->GetTexImageByLevel(target, level);

    SaveTextureToFile(target, 0, 0, 0, width, height, width, height, 1, internalformat, type, level, (GLuint)pixels);

    TranslateTexTarget(target, tmp);
    TranslateTexFormat(internalformat, tmp1);
    TranslateDataType(type, tmp2);

    sprintf(output, "glTexImage2D(%s, level=%d, %s, %d, %d, %s) %s\n",
        tmp, level, tmp1, width, height, tmp2, pTexImg->texName);

    ANALYZE_END_EVENT(GL3_API_glTexImage2D);
}

GLvoid CAnalyzer::AnalyzeTexSubImage2D(GLchar *output, GLint outputSize, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const void *pixels)
{
    ANALYZE_BEGIN_EVENT();

    CTexObj     *pTexObj    = CURRENT_CONTEXT1()->GetActiveTexObj(target);
    stTexImage  *pTexImg    = pTexObj?pTexObj->GetTexImageByLevel(target, level):NULL;

    // Save sub texture image.
    SaveTextureToFile(target, xoffset, yoffset, 0, width, height,
                      pTexImg->width, pTexImg->height, pTexImg->depth,
                      format, type, level, (GLuint)pixels);

    TranslateTexTarget(target, tmp);
    TranslateDataType(type, tmp2);

    sprintf(output, "glTexSubImage2D(%s, level=%d, x=%d, y=%d, w=%d, h=%d, %s) %s\n",
        tmp, level, xoffset, yoffset, width, height, tmp2, pTexImg->texName);

    ANALYZE_END_EVENT(GL3_API_glTexSubImage2D);
}

GLvoid CAnalyzer::AnalyzeTexImage3D(GLchar *output, GLint outputSize, GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLenum format, GLenum type, const void *pixels)
{
    ANALYZE_BEGIN_EVENT();

    CTexObj     *pTexObj    = CURRENT_CONTEXT1()->GetActiveTexObj(target);
    stTexImage  *pTexImg    = pTexObj?pTexObj->GetTexImageByLevel(target, level):NULL;

    if (pixels)
    {
        SaveTextureToFile(target, 0, 0, 0, width, height, width, height, depth, internalformat, type, level, (GLuint)pixels);
    }

    TranslateTexTarget(target, tmp);
    TranslateTexFormat(internalformat, tmp1);
    TranslateDataType(type, tmp2);

    OutputStrcat(output, outputSize, "glTexImage3D(%s, level=%d, %s, %d, %d, depth=%d, %s) %s\n",
        tmp, level, tmp1, width, height, depth, tmp2, pTexImg->texName);

    ANALYZE_END_EVENT(GL3_API_glTexImage3D);
}

GLvoid CAnalyzer::AnalyzeTexSubImage3D(GLchar *output, GLint outputSize, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const void *pixels)
{
    ANALYZE_BEGIN_EVENT();

    CTexObj     *pTexObj    = CURRENT_CONTEXT1()->GetActiveTexObj(target);
    stTexImage  *pTexImg    = pTexObj?pTexObj->GetTexImageByLevel(target, level):NULL;

    // Update sub texture image.
    SaveTextureToFile(target, xoffset, yoffset, zoffset, width, height,
                      pTexImg->width, pTexImg->height,
                      depth, format, type, level, (GLuint)pixels);

    TranslateTexTarget(target, tmp);
    TranslateDataType(type, tmp2);
    OutputStrcat(output, outputSize, "glTexSubImage3D(%s, level=%d, x=%d, y=%d, z=%d, w=%d, h=%d, d=%d, %s) %s\n",
        tmp, level, xoffset, yoffset, zoffset, width, height, depth, tmp2, pTexImg->texName);

    ANALYZE_END_EVENT(GL3_API_glTexSubImage3D);
}

GLvoid CAnalyzer::AnalyzeTexStorage2D(GLchar *output, GLint outputSize, GLenum target, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height)
{
    ANALYZE_BEGIN_EVENT();

    TranslateTexTarget(target, tmp1);
    TranslateTexFormat(internalformat, tmp2);
    sprintf(output, "glTexStorage2D(%s, levels=%d, %s, w=%d, h=%d)\n", tmp1, levels, tmp2, width, height);

    ANALYZE_END_EVENT(GL3_API_glTexStorage2D);
}

GLvoid CAnalyzer::AnalyzeTexStorage3D(GLchar *output, GLint outputSize, GLenum target, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth)
{
    ANALYZE_BEGIN_EVENT();

    TranslateTexTarget(target, tmp1);
    TranslateTexFormat(internalformat, tmp2);
    sprintf(output, "glTexStorage3D(%s, levels=%d, %s, w=%d, h=%d, d=%d)\n", tmp1, levels, tmp2, width, height, depth);

    ANALYZE_END_EVENT(GL3_API_glTexStorage3D);
}

GLvoid CAnalyzer::AnalyzeCompressedTexImage2D(GLchar *output, GLint outputSize, GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLsizei imageSize, const void *data)
{

    ANALYZE_BEGIN_EVENT();

    CTexObj     *pTexObj    = CURRENT_CONTEXT1()->GetActiveTexObj(target);
    stTexImage  *pTexImg    = pTexObj->GetTexImageByLevel(target, level);

    SaveCompressedTextureToFile(target, 0, 0, 0, width, height, width, height, 1, internalformat, GL_UNSIGNED_BYTE, level, (GLuint)data);

    TranslateTexTarget(target, tmp);
    TranslateGL2GCFormat(internalformat, GL_UNSIGNED_BYTE, tmp2, tmp1, tmp3);
    OutputStrcat(output, outputSize, "glCompressedTexImage2D(GLchar *output, GLint outputSize, %s, level=%d, %s, w=%d, h=%d, size=0x%08X) %s\n",
        tmp, level, tmp2, width, height, imageSize, pTexImg->texName);

    ANALYZE_END_EVENT(GL3_API_glCompressedTexImage2D);
}

GLvoid CAnalyzer::AnalyzeCompressedTexSubImage2D(GLchar *output, GLint outputSize, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLsizei imageSize, const void *data)
{
    ANALYZE_BEGIN_EVENT();

    CTexObj     *pTexObj    = CURRENT_CONTEXT1()->GetActiveTexObj(target);
    stTexImage  *pTexImg    = pTexObj->GetTexImageByLevel(target, level);

    SaveCompressedTextureToFile(target, xoffset, yoffset, 0, width, height, width, height, 1, format, GL_UNSIGNED_BYTE, level, (GLuint)data);

    TranslateTexTarget(target, tmp);
    TranslateTexFormat(format, tmp1);
    sprintf(output, "glCompressedTexSubImage2D(%s, level=%d, xoff=%d, yoff=%d, w=%d, h=%d, %s, size=0x%08X) %s\n",
        tmp, level, xoffset, yoffset, width, height, tmp1, imageSize, pTexImg->texName);

    ANALYZE_END_EVENT(GL3_API_glCompressedTexSubImage2D);
}

GLvoid CAnalyzer::AnalyzeTexParameterf(GLchar *output, GLint outputSize, GLenum target, GLenum pname, GLfloat param)
{
    ANALYZE_BEGIN_EVENT();

    TranslateTexTarget(target, tmp);
    TranslateTexParamName(pname, tmp1);
    TranslateTexValue((GLenum)param, tmp2);
    PrintParams3(output, outputSize, "glTexParameterf",
        (GLuint)tmp, "", STR_STR,
        (GLuint)tmp1, "", STR_STR,
        (GLuint)tmp2, "", STR_STR);

    ANALYZE_END_EVENT(GL3_API_glTexParameterf);
}

GLvoid CAnalyzer::AnalyzeTexParameterfv(GLchar *output, GLint outputSize, GLenum target, GLenum pname, const GLfloat *params)
{
    ANALYZE_BEGIN_EVENT();

    TranslateTexTarget(target, tmp);
    TranslateTexParamName(pname, tmp1);
    TranslateTexValue((GLuint)*params, tmp2);
    PrintParams3(output, outputSize, "glTexParameterfv",
        (GLuint)tmp, "", STR_STR,
        (GLuint)tmp1, "", STR_STR,
        (GLuint)tmp2, "", STR_STR);

    ANALYZE_END_EVENT(GL3_API_glTexParameterfv);
}

GLvoid CAnalyzer::AnalyzeTexParameteri(GLchar *output, GLint outputSize, GLenum target, GLenum pname, GLint param)
{
    ANALYZE_BEGIN_EVENT();

    TranslateTexTarget(target, tmp);
    TranslateTexParamName(pname, tmp1);
    TranslateTexValue(param, tmp2);
    if (pname != GL_TEXTURE_MAX_ANISOTROPY_EXT)
    {
        PrintParams3(output, outputSize, "glTexParameteri",
            (GLuint)tmp, "", STR_STR,
            (GLuint)tmp1, "", STR_STR,
            (GLuint)tmp2, "", STR_STR);
    }
    else
    {
        PrintParams3(output, outputSize, "glTexParameteri",
            (GLuint)tmp, "", STR_STR,
            (GLuint)tmp1, "", STR_STR,
            (GLuint)param, "isotropic", INT_08D);
    }

    ANALYZE_END_EVENT(GL3_API_glTexParameteri);
}

GLvoid CAnalyzer::AnalyzeTexParameteriv(GLchar *output, GLint outputSize, GLenum target, GLenum pname, const GLint *params)
{
    ANALYZE_BEGIN_EVENT();

    TranslateTexTarget(target, tmp);
    TranslateTexParamName(pname, tmp1);
    TranslateTexValue(*params, tmp2);
    PrintParams3(output, outputSize, "glTexParameteriv",
        (GLuint)tmp, "", STR_STR,
        (GLuint)tmp1, "", STR_STR,
        (GLuint)tmp2, "", STR_STR);

    ANALYZE_END_EVENT(GL3_API_glTexParameteriv);
}

GLvoid CAnalyzer::AnalyzeCopyTexImage2D(GLchar *output, GLint outputSize, GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLsizei height, GLint border)
{
    ANALYZE_BEGIN_EVENT();

    TranslateTexTarget(target, tmp);
    TranslateGL2GCFormat(internalformat, GL_UNSIGNED_BYTE, tmp3, tmp1, tmp2);
    sprintf(output, "glCopyTexImage2D(%s, level=%d, %s, x=%d, y=%d, w=%d, h=%d)\n",
        tmp, level, tmp1, x, y, width, height);

    ANALYZE_END_EVENT(GL3_API_glCopyTexImage2D);
}

GLvoid CAnalyzer::AnalyzeCopyTexSubImage2D(GLchar *output, GLint outputSize, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height)
{
    ANALYZE_BEGIN_EVENT();

    TranslateTexTarget(target, tmp);
    sprintf(output, "glCopyTexSubImage2D(%s, level=%d, xoff=%d, yoff=%d, x=%d, y=%d, w=%d, h=%d)\n",
        tmp, level, xoffset, yoffset, x, y, width, height);

    ANALYZE_END_EVENT(GL3_API_glCopyTexSubImage2D);
}

GLvoid CAnalyzer::AnalyzeGetTexParameterfv(GLchar *output, GLint outputSize, GLenum target, GLenum pname, GLfloat *params)
{
    ANALYZE_BEGIN_EVENT();
    sprintf(output, "glGetTexParameterfv() ********todo\n");
    ANALYZE_END_EVENT(GL3_API_glGetTexParameterfv);
}

GLvoid CAnalyzer::AnalyzeGetTexParameteriv(GLchar *output, GLint outputSize, GLenum target, GLenum pname, GLint *params)
{
    ANALYZE_BEGIN_EVENT();

    TranslateTexTarget(target, tmp1);
    TranslateTexParameter(pname, *params, tmp2);
    sprintf(output, "glGetTexParameteriv(%s, %s)\n", tmp1, tmp2);

    ANALYZE_END_EVENT(GL3_API_glGetTexParameteriv);
}

GLvoid CAnalyzer::OutputFBOAttachment(GLenum type, GLuint name, const GLchar *attachName, GLchar *output, GLint outputSize)
{
    const GLchar        *targetName = NULL;
    GLboolean           bTex        = GL_TRUE;
    CTexObj             *pTex       = NULL;
    CRenderbufferObj    *pRbo       = NULL;
    GLenum              format      = 0;
    GLsizei             width       = 0;
    GLsizei             height      = 0;

    switch (type)
    {
    case GL_TEXTURE_2D:
        targetName = "tex2d";
        break;

    case GL_TEXTURE_CUBE_MAP_POSITIVE_X:
        targetName = "texCube_px";
        break;

    case GL_TEXTURE_CUBE_MAP_NEGATIVE_X:
        targetName = "texCube_nx";
        break;

    case GL_TEXTURE_CUBE_MAP_POSITIVE_Y:
        targetName = "texCube_py";
        break;

    case GL_TEXTURE_CUBE_MAP_NEGATIVE_Y:
        targetName = "texCube_ny";
        break;

    case GL_TEXTURE_CUBE_MAP_POSITIVE_Z:
        targetName = "texCube_pz";
        break;

    case GL_TEXTURE_CUBE_MAP_NEGATIVE_Z:
        targetName = "texCube_nz";
        break;

    case GL_RENDERBUFFER:
        targetName = "rbo";
        bTex = GL_FALSE;
        break;

    default:
        targetName = "Invalid";
        bTex = GL_FALSE;
        break;
    }

    if (bTex)
    {
        pTex    = CURRENT_CONTEXT1()->GetTexObjByName(name);
        format  = pTex->GetTexFormat();
        width   = pTex->GetWidth(0);
        height  = pTex->GetHeight(0);

        TranslateTexFormat(format, tmp1);
        OutputStrcat(output, outputSize, "%s: %s(%04d, %dx%d, %s)  ", attachName, targetName, name, width, height, tmp1);
    }
    else
    {
        pRbo = CURRENT_CONTEXT1()->GetRBOByName(name);
        format = pRbo->GetFormat();
        TranslateTexFormat(format, tmp1);
        OutputStrcat(output, outputSize, "%s: %s(%04d, %s)  ", attachName, targetName, name, tmp1);
    }
}

GLvoid CAnalyzer::OutputFBO(GLuint currentFBO, GLchar *output, GLint outputSize)
{
    CFramebufferObj *pFBO = CURRENT_CONTEXT1()->GetCurrentFBO();

    sprintf(output, "glBindFramebuffer(fbo=%04d)  ", CURRENT_CONTEXT1()->currentFBO);

    // Using FBO
    if (pFBO)
    {
        if (pFBO->color0 != 0)
        {
            OutputFBOAttachment(pFBO->color0Type, pFBO->color0, "color", output, outputSize);
        }

        if (pFBO->depth != 0)
        {
            OutputFBOAttachment(pFBO->depthType, pFBO->depth, "depth", output, outputSize);
        }

        if (pFBO->stencil != 0)
        {
            OutputFBOAttachment(pFBO->stencilType, pFBO->stencil, "stencil", output, outputSize);;
        }
    }
    else // Using RT
    {

    }

    OutputStrcat(output, outputSize, "\n");
}

GLvoid CAnalyzer::OutputCurrentFBO(GLint *pos, GLchar *output, GLint outputSize)
{
    CFramebufferObj *pFBO = CURRENT_CONTEXT1()->GetCurrentFBO();
    GLchar buf[128];

    if (pFBO)
    {
        OutputStringFast(drawStates34, pos, output, outputSize, "   FBO: %04d\n", pFBO->name);
        if (pFBO->color0 != 0)
        {
            memset(buf, 0, 128);
            OutputFBOAttachment(pFBO->color0Type, pFBO->color0, "        color  ", buf, 128);
            OutputStringFast(drawStates35, pos, output, outputSize, "%s\n", buf);
        }

        if (pFBO->depth != 0)
        {
            memset(buf, 0, 128);
            OutputFBOAttachment(pFBO->depthType, pFBO->depth, "        depth  ", buf, 128);
            OutputStringFast(drawStates36, pos, output, outputSize, "%s\n", buf);
        }

        if (pFBO->stencil != 0)
        {
            memset(buf, 0, 128);
            OutputFBOAttachment(pFBO->stencilType, pFBO->stencil, "        stencil", buf, 128);
            OutputStringFast(drawStates37, pos, output, outputSize, "%s\n", buf);
        }
    }
    else // Using RT
    {
        OutputStringFast(drawStates34, pos, output, outputSize, "   FBO: 0000  \n");
    }
}

GLvoid CAnalyzer::AnalyzeBindFramebuffer(GLchar *output, GLint outputSize, GLenum target, GLuint framebuffer)
{
    ANALYZE_BEGIN_EVENT();
    if (target == GL_FRAMEBUFFER)
    {
        OutputFBO(target, output, outputSize);

        // Dump result to Frame file
        OutputToTarget(OUTPUT_TO_FILE, GL_FALSE, 0, NULL, output, m_pFrameFile);
    }
    else
    {
        sprintf(output, "glBindFramebuffer(unknown(0x%08X))\n", target);
    }
    ANALYZE_END_EVENT(GL3_API_glBindFramebuffer);
}

GLvoid CAnalyzer::AnalyzeBindRenderbuffer(GLchar *output, GLint outputSize, GLenum target, GLuint renderbuffer)
{
    ANALYZE_BEGIN_EVENT();

    PrintParams2(output, outputSize, "glBindRenderbuffer",
        (GLuint)"GL_RENDERBUFFER", "", STR_STR,
        renderbuffer, "RBO", INT_04D);

    ANALYZE_END_EVENT(GL3_API_glBindRenderbuffer);
}

GLvoid CAnalyzer::AnalyzeDeleteFramebuffers(GLchar *output, GLint outputSize, GLsizei n, const GLuint *framebuffers)
{
    ANALYZE_BEGIN_EVENT();
    GenDelObjects(CURRENT_CONTEXT1(), output, outputSize, n, framebuffers, "glDeleteFramebuffers", "fbo");
    ANALYZE_END_EVENT(GL3_API_glDeleteFramebuffers);
}

GLvoid CAnalyzer::AnalyzeDeleteRenderbuffers(GLchar *output, GLint outputSize, GLsizei n, const GLuint *renderbuffers)
{
    ANALYZE_BEGIN_EVENT();
    GenDelObjects(CURRENT_CONTEXT1(), output, outputSize, n, renderbuffers, "glDeleteRenderbuffers", "rbo");
    ANALYZE_END_EVENT(GL3_API_glDeleteRenderbuffers);
}

GLvoid CAnalyzer::AnalyzeGenFramebuffers(GLchar *output, GLint outputSize, GLsizei n, GLuint *framebuffers)
{
    ANALYZE_BEGIN_EVENT();
    GenDelObjects(CURRENT_CONTEXT1(), output, outputSize, n, framebuffers, "glGenFramebuffers", "fbo");
    ANALYZE_END_EVENT(GL3_API_glGenFramebuffers);
}

GLvoid CAnalyzer::AnalyzeGenRenderbuffers(GLchar *output, GLint outputSize, GLsizei n, GLuint *renderbuffers)
{
    ANALYZE_BEGIN_EVENT();
    GenDelObjects(CURRENT_CONTEXT1(), output, outputSize, n, renderbuffers, "glGenRenderbuffers", "rbo");
    ANALYZE_END_EVENT(GL3_API_glGenRenderbuffers);
}

GLvoid CAnalyzer::AnalyzeRenderbufferStorage(GLchar *output, GLint outputSize, GLenum target, GLenum internalformat, GLsizei width, GLsizei height)
{
    ANALYZE_BEGIN_EVENT();

    TranslateRBOFormat(internalformat, tmp1);
    PrintParams4(output, outputSize, "glRenderbufferStorage",
        (GLuint)"GL_RENDERBUFFER", "", STR_STR,
        (GLuint)tmp1, "", STR_STR,
        (GLuint)width, "width", INT_04D,
        (GLuint)height, "height", INT_04D);

    ANALYZE_END_EVENT(GL3_API_glRenderbufferStorage);
}

GLvoid CAnalyzer::AnalyzeFramebufferRenderbuffer(GLchar *output, GLint outputSize, GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer)
{
    ANALYZE_BEGIN_EVENT();

    TranslateAttachment(attachment, tmp);
    TranslateFramebufferRenderbuffer(renderbuffertarget, tmp1);
    PrintParams4(output, outputSize, "glFramebufferRenderbuffer",
        (GLuint)"GL_FRAMEBUFFER", "", STR_STR,
        (GLuint)tmp, "", STR_STR,
        (GLuint)tmp1, "", STR_STR,
        (GLuint)renderbuffer, "RBO", INT_04D);

    ANALYZE_END_EVENT(GL3_API_glFramebufferRenderbuffer);
}

GLvoid CAnalyzer::AnalyzeFramebufferTexture2D(GLchar *output, GLint outputSize, GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level)
{
    ANALYZE_BEGIN_EVENT();

    TranslateAttachment(attachment, tmp);
    TranslateTexTarget(textarget, tmp1);

    if (target != GL_FRAMEBUFFER)
    {
        sprintf(output, "glFramebufferTexture2D(Invalid)\n");
    }
    else
    {
        sprintf(output, "glFramebufferTexture2D(%s, %s, tex=%d, level=%d)\n", tmp, tmp1, texture, level);
    }

    ANALYZE_END_EVENT(GL3_API_glFramebufferTexture2D);
}

GLvoid CAnalyzer::AnalyzeCheckFramebufferStatus(GLchar *output, GLint outputSize, GLenum target, GLenum result)
{
    ANALYZE_BEGIN_EVENT();

    GLchar *ptr;

    if (target == GL_FRAMEBUFFER)
    {
        switch (result)
        {
        case GL_FRAMEBUFFER_COMPLETE:
            ptr = "GL_FRAMEBUFFER_COMPLETE";
            break;

        case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
            ptr = "GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT";
            break;

        case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
            ptr = "GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT";
            break;

        case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS:
            ptr = "GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS";
            break;

        case GL_FRAMEBUFFER_UNSUPPORTED:
            ptr = "GL_FRAMEBUFFER_UNSUPPORTED";
            break;

        default:
            ptr = "Invalid";
            break;
        }

        sprintf(output, "glCheckFramebufferStatus()=%s\n", ptr);
    }
    else
    {
        sprintf(output, "glCheckFramebufferStatus(Invalid)\n");
    }

    ANALYZE_END_EVENT(GL3_API_glCheckFramebufferStatus);
}

GLvoid CAnalyzer::AnalyzeGetFramebufferAttachmentParameteriv(GLchar *output, GLint outputSize, GLenum target, GLenum attachment, GLenum pname, GLint *params)
{
    ANALYZE_BEGIN_EVENT();
    sprintf(output, "glGetFramebufferAttachmentParameteriv() ********todo\n");
    ANALYZE_END_EVENT(GL3_API_glGetFramebufferAttachmentParameteriv);
}

GLvoid CAnalyzer::AnalyzeGetRenderbufferParameteriv(GLchar *output, GLint outputSize, GLenum target, GLenum pname, GLint *params)
{
    ANALYZE_BEGIN_EVENT();
    sprintf(output, "glGetRenderbufferParameteriv() ********todo\n");
    ANALYZE_END_EVENT(GL3_API_glGetRenderbufferParameteriv);
}

GLvoid CAnalyzer::AnalyzeDisable(GLchar *output, GLint outputSize, GLenum cap)
{
    ANALYZE_BEGIN_EVENT();
    TranslateCapability(cap, tmp, GL_FALSE);
    PrintParams1(output, outputSize, "glDisable", (GLuint)tmp, "", STR_STR);
    ANALYZE_END_EVENT(GL3_API_glDisable);
}

GLvoid CAnalyzer::AnalyzeEnable(GLchar *output, GLint outputSize, GLenum cap)
{
    ANALYZE_BEGIN_EVENT();

    TranslateCapability(cap, tmp, GL_TRUE);
    PrintParams1(output, outputSize, "glEnable", (GLuint)tmp, "", STR_STR);

    ANALYZE_END_EVENT(GL3_API_glEnable);
}

GLvoid CAnalyzer::AnalyzeFrontFace(GLchar *output, GLint outputSize, GLenum mode)
{
    ANALYZE_BEGIN_EVENT();

    GLchar *ptr;
    switch (mode)
    {
    case GL_CW:
        ptr = "GL_CW";
        break;

    case GL_CCW:
        ptr = "GL_CCW";
        break;

    default:
        ptr = "Invalid";
        break;
    }
    PrintParams1(output, outputSize, "glFrontFace", (GLuint)ptr, "", STR_STR);

    ANALYZE_END_EVENT(GL3_API_glFrontFace);
}

GLvoid CAnalyzer::AnalyzeCullFace(GLchar *output, GLint outputSize, GLenum mode)
{
    ANALYZE_BEGIN_EVENT();
    TranslateFace(mode, tmp5);
    PrintParams1(output, outputSize, "glCullFace", (GLuint)tmp5, "F", STR_STR);
    ANALYZE_END_EVENT(GL3_API_glCullFace);
}

GLvoid CAnalyzer::AnalyzeBlendColor(GLchar *output, GLint outputSize, GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha)
{
    ANALYZE_BEGIN_EVENT();

    PrintParams4(output, outputSize, "glBlendColor",
        (GLuint)red, "R", FLOAT_1_1,
        (GLuint)green, "G", FLOAT_1_1,
        (GLuint)blue, "B", FLOAT_1_1,
        (GLuint)alpha, "A", FLOAT_1_1);

    ANALYZE_END_EVENT(GL3_API_glBlendColor);
}

GLvoid CAnalyzer::AnalyzeBlendEquation(GLchar *output, GLint outputSize, GLenum mode)
{
    ANALYZE_BEGIN_EVENT();

    TranslateBlendMode(mode, tmp);
    PrintParams1(output, outputSize, "glBlendEquation",
        (GLuint)tmp, "", STR_STR);

    ANALYZE_END_EVENT(GL3_API_glBlendEquation);
}

GLvoid CAnalyzer::AnalyzeBlendEquationSeparate(GLchar *output, GLint outputSize, GLenum modeRGB, GLenum modeAlpha)
{
    ANALYZE_BEGIN_EVENT();

    TranslateBlendMode(modeRGB, tmp);
    TranslateBlendMode(modeAlpha, tmp1);
    PrintParams2(output, outputSize, "glBlendEquationSeparate",
        (GLuint)tmp, "rgb", STR_STR,
        (GLuint)tmp1, "a", STR_STR);

    ANALYZE_END_EVENT(GL3_API_glBlendEquationSeparate);
}

GLvoid CAnalyzer::AnalyzeBlendFunc(GLchar *output, GLint outputSize, GLenum sfactor, GLenum dfactor)
{
    ANALYZE_BEGIN_EVENT();

    TranslateBlendFunc(sfactor, tmp);
    TranslateBlendFunc(dfactor, tmp1);
    PrintParams2(output, outputSize, "glBlendFunc",
        (GLuint)tmp, "src", STR_STR,
        (GLuint)tmp1, "dst", STR_STR);

    ANALYZE_END_EVENT(GL3_API_glBlendFunc);
}

GLvoid CAnalyzer::AnalyzeBlendFuncSeparate(GLchar *output, GLint outputSize, GLenum sfactorRGB, GLenum dfactorRGB, GLenum sfactorAlpha, GLenum dfactorAlpha)
{
    ANALYZE_BEGIN_EVENT();

    TranslateBlendFunc(sfactorRGB, tmp);
    TranslateBlendFunc(dfactorRGB, tmp1);
    TranslateBlendFunc(sfactorAlpha, tmp2);
    TranslateBlendFunc(dfactorAlpha, tmp3);
    PrintParams4(output, outputSize, "glBlendFuncSeparate",
        (GLuint)tmp, "srcRGB", STR_STR,
        (GLuint)tmp1, "dstRGB", STR_STR,
        (GLuint)tmp2, "srcAlpha", STR_STR,
        (GLuint)tmp3, "dstAlph", STR_STR);

    ANALYZE_END_EVENT(GL3_API_glBlendFuncSeparate);
}

GLvoid CAnalyzer::AnalyzeDepthFunc(GLchar *output, GLint outputSize, GLenum func)
{
    ANALYZE_BEGIN_EVENT();

    TranslateFunc(func, tmp);
    PrintParams1(output, outputSize, "glDepthFunc", (GLuint)tmp, "attrib", STR_STR);

    ANALYZE_END_EVENT(GL3_API_glDepthFunc);
}

GLvoid CAnalyzer::AnalyzeDepthRangef(GLchar *output, GLint outputSize, GLfloat n, GLfloat f)
{
    ANALYZE_BEGIN_EVENT();

    PrintParams2(output, outputSize, "glDepthRangef",
        *(GLuint*)&n, "near", FLOAT_1_5,
        *(GLuint*)&f, "far", FLOAT_1_5);

    ANALYZE_END_EVENT(GL3_API_glDepthRangef);
}

GLvoid CAnalyzer::AnalyzeClearColor(GLchar *output, GLint outputSize, GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha)
{
    ANALYZE_BEGIN_EVENT();
    PrintParams4(output, outputSize, "glClearColor",
        *(GLuint*)&red, "R", FLOAT_1_5,
        *(GLuint*)&green, "G", FLOAT_1_5,
        *(GLuint*)&blue, "B", FLOAT_1_5,
        *(GLuint*)&alpha, "A", FLOAT_1_5);
    ANALYZE_END_EVENT(GL3_API_glClearColor);
}

GLvoid CAnalyzer::AnalyzeClearDepthf(GLchar *output, GLint outputSize, GLfloat d)
{
    ANALYZE_BEGIN_EVENT();
    PrintParams1(output, outputSize, "glClearDepthf",
        *(GLuint*)&d, "D", FLOAT_1_5);
    ANALYZE_END_EVENT(GL3_API_glClearDepthf);
}

GLvoid CAnalyzer::AnalyzeClearStencil(GLchar *output, GLint outputSize, GLint s)
{
    ANALYZE_BEGIN_EVENT();
    PrintParams1(output, outputSize, "glClearStencil",
        (GLuint)s, "S", INT_0X08X);
    ANALYZE_END_EVENT(GL3_API_glClearStencil);
}

GLvoid CAnalyzer::AnalyzeColorMask(GLchar *output, GLint outputSize, GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha)
{
    ANALYZE_BEGIN_EVENT();
    PrintParams4(output, outputSize, "glColorMask",
        (GLuint)red, "R", INT_04D,
        (GLuint)green, "G", INT_04D,
        (GLuint)blue, "B", INT_04D,
        (GLuint)alpha, "A", INT_04D);
    ANALYZE_END_EVENT(GL3_API_glColorMask);
}

GLvoid CAnalyzer::AnalyzeDepthMask(GLchar *output, GLint outputSize, GLboolean flag)
{
    ANALYZE_BEGIN_EVENT();
    PrintParams1(output, outputSize, "glDepthMask", (GLuint)flag, "mask", INT_0X08X);
    ANALYZE_END_EVENT(GL3_API_glDepthMask);
}

GLvoid CAnalyzer::AnalyzeLineWidth(GLchar *output, GLint outputSize, GLfloat width)
{
    ANALYZE_BEGIN_EVENT();
    PrintParams1(output, outputSize, "glLineWidth", (GLuint)width, "width", FLOAT_5_1);
    ANALYZE_END_EVENT(GL3_API_glLineWidth);
}

GLvoid CAnalyzer::AnalyzePixelStorei(GLchar *output, GLint outputSize, GLenum pname, GLint param)
{
    ANALYZE_BEGIN_EVENT();
    TranslatePixelStorei(pname, param, tmp1);
    PrintParams2(output, outputSize, "glPixelStorei",
        (GLuint)tmp1, "", STR_STR,
        (GLuint)param, "param", INT_04D);
    ANALYZE_END_EVENT(GL3_API_glPixelStorei);
}

GLvoid CAnalyzer::AnalyzePolygonOffset(GLchar *output, GLint outputSize, GLfloat factor, GLfloat units)
{
    ANALYZE_BEGIN_EVENT();
    PrintParams2(output, outputSize, "glPolygonOffset",
        *(GLuint*)&factor, "factor", FLOAT_1_5,
        *(GLuint*)&units, "units", FLOAT_1_5);
    ANALYZE_END_EVENT(GL3_API_glPolygonOffset);
}

GLvoid CAnalyzer::AnalyzeScissor(GLchar *output, GLint outputSize, GLint x, GLint y, GLsizei width, GLsizei height)
{
    ANALYZE_BEGIN_EVENT();
    PrintParams4(output, outputSize, "glScissor",
        (GLuint)x, "", INT_04D_NO,
        (GLuint)y, "", INT_04D_NO,
        (GLuint)width, "", INT_04D_NO,
        (GLuint)height, "", INT_04D_NO);
    ANALYZE_END_EVENT(GL3_API_glScissor);
}

GLvoid CAnalyzer::AnalyzeStencilFunc(GLchar *output, GLint outputSize, GLenum func, GLint ref, GLuint mask)
{
    ANALYZE_BEGIN_EVENT();
    TranslateFunc(func, tmp);
    PrintParams3(output, outputSize, "glStencilFunc",
        (GLuint)tmp, "", STR_STR,
        (GLuint)ref, "ref", INT_0X08X,
        (GLuint)mask, "mask", INT_0X08X);
    ANALYZE_END_EVENT(GL3_API_glStencilFunc);
}

GLvoid CAnalyzer::AnalyzeStencilFuncSeparate(GLchar *output, GLint outputSize, GLenum face, GLenum func, GLint ref, GLuint mask)
{
    ANALYZE_BEGIN_EVENT();
    TranslateFace(face, tmp);
    TranslateFunc(func, tmp1);
    PrintParams4(output, outputSize, "glStencilFuncSeparate",
        (GLuint)tmp, "", STR_STR,
        (GLuint)tmp1, "", STR_STR,
        (GLuint)ref, "ref", INT_0X08X,
        (GLuint)mask, "mask", INT_0X08X);
    ANALYZE_END_EVENT(GL3_API_glStencilFuncSeparate);
}

GLvoid CAnalyzer::AnalyzeStencilMask(GLchar *output, GLint outputSize, GLuint mask)
{
    ANALYZE_BEGIN_EVENT();
    PrintParams1(output, outputSize, "glStencilMask", (GLuint)mask, "mask", INT_0X08X);
    ANALYZE_END_EVENT(GL3_API_glStencilMask);
}

GLvoid CAnalyzer::AnalyzeStencilMaskSeparate(GLchar *output, GLint outputSize, GLenum face, GLuint mask)
{
    ANALYZE_BEGIN_EVENT();
    TranslateFace(face, tmp);
    PrintParams2(output, outputSize, "glStencilMaskSeparate",
        (GLuint)tmp, "", STR_STR,
        (GLuint)mask, "mask", INT_0X08X);
    ANALYZE_END_EVENT(GL3_API_glStencilMaskSeparate);
}

GLvoid CAnalyzer::AnalyzeStencilOp(GLchar *output, GLint outputSize, GLenum fail, GLenum zfail, GLenum zpass)
{
    ANALYZE_BEGIN_EVENT();
    TranslateOperation(fail, tmp);
    TranslateOperation(zfail, tmp1);
    TranslateOperation(zpass, tmp2);
    PrintParams3(output, outputSize, "glStencilOp",
        (GLuint)tmp, "fail", STR_STR_NAME,
        (GLuint)tmp1, "zfail", STR_STR_NAME,
        (GLuint)tmp2, "zpass", STR_STR_NAME);
    ANALYZE_END_EVENT(GL3_API_glStencilOp);
}

GLvoid CAnalyzer::AnalyzeStencilOpSeparate(GLchar *output, GLint outputSize, GLenum face, GLenum sfail, GLenum dpfail, GLenum dppass)
{
    ANALYZE_BEGIN_EVENT();
    TranslateFace(face, tmp);
    TranslateOperation(sfail, tmp1);
    TranslateOperation(dpfail, tmp2);
    TranslateOperation(dppass, tmp3);
    PrintParams4(output, outputSize, "glStencilOpSeparate",
        (GLuint)tmp, "", STR_STR,
        (GLuint)tmp1, "fail", STR_STR_NAME,
        (GLuint)tmp2, "zfail", STR_STR_NAME,
        (GLuint)tmp3, "zpass", STR_STR_NAME);
    ANALYZE_END_EVENT(GL3_API_glStencilOpSeparate);
}

GLvoid CAnalyzer::AnalyzeSampleCoverage(GLchar *output, GLint outputSize, GLfloat value, GLboolean invert)
{
    ANALYZE_BEGIN_EVENT();
    PrintParams2(output, outputSize, "glSampleCoverage",
        *(GLuint*)&value, "value", FLOAT_1_5,
        (GLuint)invert, "invert", INT_04D);
    ANALYZE_END_EVENT(GL3_API_glSampleCoverage);
}

GLvoid CAnalyzer::AnalyzeViewport(GLchar *output, GLint outputSize, GLint x, GLint y, GLsizei width, GLsizei height)
{
    ANALYZE_BEGIN_EVENT();
    PrintParams4(output, outputSize, "glViewport",
        (GLuint)x, "", INT_04D_NO,
        (GLuint)y, "", INT_04D_NO,
        (GLuint)width, "", INT_04D_NO,
        (GLuint)height, "", INT_04D_NO);
    ANALYZE_END_EVENT(GL3_API_glViewport);
}

GLvoid CAnalyzer::AnalyzeGetBooleanv(GLchar *output, GLint outputSize, GLenum pname, GLboolean *data)
{
    ANALYZE_BEGIN_EVENT();
    TranslateGetName(pname, tmp1);
    sprintf(output, "glGetBooleanv(%s, value=%d)\n", tmp1, *data);
    ANALYZE_END_EVENT(GL3_API_glGetBooleanv);
}

GLvoid CAnalyzer::AnalyzeGetFloatv(GLchar *output, GLint outputSize, GLenum pname, GLfloat *data)
{
    ANALYZE_BEGIN_EVENT();

    TranslateGetName(pname, tmp2);
    sprintf(output, "glGetFloatv(%s, %f)\n", tmp2, *data);

    ANALYZE_END_EVENT(GL3_API_glGetFloatv);
}

GLvoid CAnalyzer::AnalyzeGetIntegerv(GLchar *output, GLint outputSize, GLenum pname, GLint *data)
{
    ANALYZE_BEGIN_EVENT();

    TranslateGetName(pname, tmp1);
    sprintf(output, "glGetIntegerv(%s, value=0x%08X)\n", tmp1, *data);

    ANALYZE_END_EVENT(GL3_API_glGetIntegerv);
}

GLvoid CAnalyzer::AnalyzeGetString(GLchar *output, GLint outputSize, GLenum name, const GLubyte *str)
{
    ANALYZE_BEGIN_EVENT();

    TranslateGetName(name, tmp1);
    sprintf(output, "glGetString(%s) : %s\n", tmp1, str);

    ANALYZE_END_EVENT(GL3_API_glGetString);
}

GLvoid CAnalyzer::AnalyzeIsBuffer(GLchar *output, GLint outputSize, GLuint buffer, GLboolean res)
{
    ANALYZE_BEGIN_EVENT();
    PrintParams1Return(output, outputSize, "glIsBuffer", (GLuint)buffer, "vbo", INT_04D, (GLuint)res, INT_04D);
    ANALYZE_END_EVENT(GL3_API_glIsBuffer);
}

GLvoid CAnalyzer::AnalyzeIsEnabled(GLchar *output, GLint outputSize, GLenum cap, GLboolean res)
{
    ANALYZE_BEGIN_EVENT();
    TranslateCapability(cap, tmp, GL_TRUE);
    sprintf(output, "glIsEnabled(%s) = %d\n", tmp, res);
    ANALYZE_END_EVENT(GL3_API_glIsEnabled);
}

GLvoid CAnalyzer::AnalyzeIsFramebuffer(GLchar *output, GLint outputSize, GLuint framebuffer, GLboolean res)
{
    ANALYZE_BEGIN_EVENT();
    PrintParams1Return(output, outputSize, "glIsFramebuffer", (GLuint)framebuffer, "FBO", INT_04D, (GLuint)res, INT_04D);
    ANALYZE_END_EVENT(GL3_API_glIsFramebuffer);
}

GLvoid CAnalyzer::AnalyzeIsProgram(GLchar *output, GLint outputSize, GLuint program, GLboolean res)
{
    ANALYZE_BEGIN_EVENT();
    PrintParams1Return(output, outputSize, "glIsRenderbuffer", (GLuint)program, "prog", INT_04D, (GLuint)res, INT_04D);
    ANALYZE_END_EVENT(GL3_API_glIsProgram);
}

GLvoid CAnalyzer::AnalyzeIsRenderbuffer(GLchar *output, GLint outputSize, GLuint renderbuffer, GLboolean res)
{
    ANALYZE_BEGIN_EVENT();
    PrintParams1Return(output, outputSize, "glIsRenderbuffer", (GLuint)renderbuffer, "RBO", INT_04D, (GLuint)res, INT_04D);
    ANALYZE_END_EVENT(GL3_API_glIsRenderbuffer);
}

GLvoid CAnalyzer::AnalyzeIsShader(GLchar *output, GLint outputSize, GLuint shader, GLboolean res)
{
    ANALYZE_BEGIN_EVENT();
    PrintParams1Return(output, outputSize, "glIsShader", (GLuint)shader, "sh", INT_04D, (GLuint)res, INT_04D);
    ANALYZE_END_EVENT(GL3_API_glIsShader);
}

GLvoid CAnalyzer::AnalyzeIsTexture(GLchar *output, GLint outputSize, GLuint texture, GLboolean res)
{
    ANALYZE_BEGIN_EVENT();
    PrintParams1Return(output, outputSize, "glIsTexture", (GLuint)texture, "tex", INT_04D, (GLuint)res, INT_04D);
    ANALYZE_END_EVENT(GL3_API_glIsTexture);
}

GLvoid CAnalyzer::AnalyzeGetError(GLchar *output, GLint outputSize, GLenum err)
{
    ANALYZE_BEGIN_EVENT();

    if (err != GL_NO_ERROR)
    {
        TranslateError(err, tmp);
        sprintf(output, "glGetError()=%s\n", tmp);
    }

    ANALYZE_END_EVENT(GL3_API_glGetError);
}

GLvoid CAnalyzer::AnalyzeFinish(GLchar *output, GLint outputSize)
{
    ANALYZE_BEGIN_EVENT();
    PrintParams0(output, outputSize, "glFinish");
    ANALYZE_END_EVENT(GL3_API_glFinish);
}

GLvoid CAnalyzer::AnalyzeFlush(GLchar *output, GLint outputSize)
{
    ANALYZE_BEGIN_EVENT();
    PrintParams0(output, outputSize, "glFlush");
    ANALYZE_END_EVENT(GL3_API_glFlush);
}

GLvoid CAnalyzer::AnalyzeHint(GLchar *output, GLint outputSize, GLenum target, GLenum mode)
{
    ANALYZE_BEGIN_EVENT();
    TranslateGetName(target, tmp1);
    TranslateGetName(mode, tmp2);
    sprintf(output, "glHint(%s, %s)\n", tmp1, tmp2);
    ANALYZE_END_EVENT(GL3_API_glHint);
}

GLvoid CAnalyzer::AnalyzeReadPixels(GLchar *output, GLint outputSize, GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, void *pixels)
{
    ANALYZE_BEGIN_EVENT();
    TranslateTexFormat(format, tmp1);
    TranslateDataType(type, tmp2);
    sprintf(output, "glReadPixels(%d, %d, %d, %d, %s, %s)\n", x, y, width, height, tmp1, tmp2);
    ANALYZE_END_EVENT(GL3_API_glReadPixels);
}

GLvoid CAnalyzer::AnalyzeReadBuffer(GLchar *output, GLint outputSize, GLenum mode)
{
    ANALYZE_BEGIN_EVENT();
    ANALYZE_END_EVENT(GL3_API_glReadBuffer);
}

GLvoid CAnalyzer::AnalyzeClear(GLchar *output, GLint outputSize, GLbitfield mask)
{
    ANALYZE_BEGIN_EVENT();
    TranslateClearMask(mask, tmp);
    PrintParams1(output, outputSize, "glClear", (GLuint)tmp, "B", STR_STR);
    OutputClearToFrameFile(mask);
    ANALYZE_END_EVENT(GL3_API_glClear);
}

GLvoid CAnalyzer::AnalyzeDrawArrays(GLchar *output, GLint outputSize, GLenum mode, GLint first, GLsizei count)
{
    ANALYZE_BEGIN_EVENT();
    OutputDrawArrays_es20(mode, first, count, output, outputSize);
    ANALYZE_END_EVENT(GL3_API_glDrawArrays);
}

GLvoid CAnalyzer::AnalyzeDrawElements(GLchar *output, GLint outputSize, GLenum mode, GLsizei count, GLenum type, const GLvoid* indices)
{
    ANALYZE_BEGIN_EVENT();
    OutputDrawElements_es20(mode, count, type, indices, output, outputSize);
    ANALYZE_END_EVENT(GL3_API_glDrawElements);
}

GLvoid CAnalyzer::AnalyzeBlitFramebufferEXT(GLchar *output, GLint outputSize, GLint srcX0, GLint srcY0, GLint srcX1, GLint srcY1, GLint dstX0, GLint dstY0, GLint dstX1, GLint dstY1, GLbitfield mask, GLenum filter)
{
    ANALYZE_BEGIN_EVENT();
    ANALYZE_END_EVENT(GL3_API_glBlitFramebuffer);
}

void CAnalyzer::AnalyzeChooseConfig(GLchar *output, GLint outputSize, EGLDisplay dpy, const EGLint *attrib_list, EGLConfig *configs, EGLint config_size, EGLint *num_config, EGLBoolean res)
{
    ANALYZE_BEGIN_EVENT();
    sprintf(output, "eglChooseConfig(display = 0x%08X, configs = 0x%08X, config_size = 0x%08X, num_config = 0x%08X)\n",
        dpy, configs, config_size, num_config);
    _ParseAttrbutes(output, outputSize, attrib_list);
    ANALYZE_END_EVENT(EGL_API_eglChooseConfig);
}

void CAnalyzer::AnalyzeCopyBuffers(GLchar *output, GLint outputSize, EGLDisplay dpy, EGLSurface surface, EGLNativePixmapType target, EGLBoolean res)
{
    ANALYZE_BEGIN_EVENT();
    PrintParams3(output, outputSize, "eglCopyBuffers",
        (GLuint)dpy, "display", INT_04D,
        (GLuint)surface, "surface", INT_0X08X,
        (GLuint)target, "target", INT_0X08X);
    ANALYZE_END_EVENT(EGL_API_eglCopyBuffers);
}

void CAnalyzer::AnalyzeCreateContext(GLchar *output, GLint outputSize, EGLDisplay dpy, EGLConfig config, EGLContext share_context, const EGLint *attrib_list, EGLContext res)
{
    ANALYZE_BEGIN_EVENT();

    CEGLContext     *pCtx   = _GetEglContext(dpy, res);
    CEGLContext     *pShCtx = share_context?_GetEglContext(dpy, share_context):NULL;
    const EGLint    *pAttr  = NULL;

    if (!pCtx)
    {
        sprintf(output, "eglCreateContext(invalid)\n");
        goto _Err;
    }

    sprintf(output, "eglCreateContext(display = 0x%08X, config = 0x%08X, SharedContext = %s) %s\n",
            dpy, config, pShCtx?pShCtx->name:"null", pCtx?pCtx->name:"null");

    pAttr = pCtx->attrib_list;
    if (pAttr)
    {
        while (*pAttr != EGL_NONE)
        {
            TranslateEGLAttrib(*pAttr, tmp5);
            OutputStrcat(output, outputSize, "    %s: 0x%04X\n", tmp5, *++pAttr);
            pAttr ++;
        }
    }

_Err:
    ANALYZE_END_EVENT(EGL_API_eglCreateContext);
}

void CAnalyzer::AnalyzeCreateImageKHR(GLchar *output, GLint outputSize, EGLDisplay dpy, EGLContext ctx, EGLenum target, EGLClientBuffer buffer, const EGLint *attrib_list, EGLImageKHR ret)
{
    ANALYZE_BEGIN_EVENT();

    const GLchar *image = NULL;

    switch (target)
    {
    case EGL_GL_TEXTURE_2D_KHR:
        image = "EGL_GL_TEXTURE_2D_KHR";
        break;

    case EGL_GL_TEXTURE_CUBE_MAP_POSITIVE_X_KHR:
    case EGL_GL_TEXTURE_CUBE_MAP_NEGATIVE_X_KHR:
    case EGL_GL_TEXTURE_CUBE_MAP_POSITIVE_Y_KHR:
    case EGL_GL_TEXTURE_CUBE_MAP_NEGATIVE_Y_KHR:
    case EGL_GL_TEXTURE_CUBE_MAP_POSITIVE_Z_KHR:
    case EGL_GL_TEXTURE_CUBE_MAP_NEGATIVE_Z_KHR:
        image = "EGL_GL_TEXTURE_CUBE_MAP";
        break;

    case EGL_NATIVE_PIXMAP_KHR:
        image = "EGL_NATIVE_PIXMAP_KHR";
        break;

    case EGL_GL_RENDERBUFFER_KHR:
        image = "EGL_GL_RENDERBUFFER_KHR";
        break;

    case EGL_NATIVE_BUFFER_ANDROID:
        image = "EGL_NATIVE_BUFFER_ANDROID";
        break;

    case EGL_VG_PARENT_IMAGE_KHR:
        image = "EGL_VG_PARENT_IMAGE_KHR";
        break;

    default:
        image = "invalid";
        break;
    }

    sprintf(output, "eglCreateImageKHR(%s, texture=0x%08X) = 0x%08X\n", image, buffer, ret);

    ANALYZE_END_EVENT(EGL_API_eglCreateImageKHR);
}

void CAnalyzer::AnalyzeDestroyImageKHR(GLchar *output, GLint outputSize, EGLDisplay dpy, EGLImageKHR image, EGLBoolean ret)
{
    ANALYZE_BEGIN_EVENT();
    PrintParams2(output, outputSize, "eglDestroyImageKHR",
        (GLuint)dpy, "display", INT_04D,
        (GLuint)image, "image", INT_0X08X);
    ANALYZE_END_EVENT(EGL_API_eglDestroyImageKHR);
}

void CAnalyzer::AnalyzeCreateSyncKHR(GLchar *output, GLint outputSize, EGLDisplay dpy, EGLenum type, const EGLint *attrib_list, EGLSyncKHR ret)
{
    ANALYZE_BEGIN_EVENT();
    TranslateEGLEnum(type, tmp1);
    sprintf(output, "eglCreateSyncKHR(display = 0x%08X, type = %s, attribList = 0x%08X) = 0x%08X\n", dpy, tmp1, attrib_list, ret);
    ANALYZE_END_EVENT(EGL_API_eglCreateSyncKHR);
}

void CAnalyzer::AnalyzeDestroySyncKHR(GLchar *output, GLint outputSize, EGLDisplay dpy, EGLSyncKHR sync, EGLBoolean ret)
{
    ANALYZE_BEGIN_EVENT();
    PrintParams2(output, outputSize, "eglDestroySyncKHR",
        (GLuint)dpy, "display", INT_04D,
        (GLuint)sync, "sync", INT_0X08X);
    ANALYZE_END_EVENT(EGL_API_eglDestroySyncKHR);
}

void CAnalyzer::AnalyzeClientWaitSyncKHR(GLchar *output, GLint outputSize, EGLDisplay dpy, EGLSyncKHR sync, EGLint flags, EGLTimeKHR timeout, EGLint ret)
{
    ANALYZE_BEGIN_EVENT();
    TranslateEGLEnum(flags, tmp1);

    memset(tmp1, 0, TMP_BUF_SIZE);
    if (timeout == EGL_FOREVER_KHR)
    {
        sprintf(tmp2, "EGL_FOREVER_KHR");
    }
    else
    {
        sprintf(tmp2, "%u ms", (GLuint)MATH_DivideUInt64(timeout, 1000000ull));
    }

    memset(tmp3, 0, TMP_BUF_SIZE);
    if (ret == EGL_FALSE)
    {
        sprintf(tmp3, "EGL_FALSE");
    }
    else
    {
        sprintf(tmp3, "EGL_TRUE");
    }

    sprintf(output, "eglClientWaitSyncKHR(display = 0x%08X, sync = 0x%08X, flags = %s, timeout = %s) = %s\n",
        dpy, sync, tmp1, tmp2, tmp3);
    ANALYZE_END_EVENT(EGL_API_eglClientWaitSyncKHR);
}

void CAnalyzer::AnalyzeGetSyncAttribKHR(GLchar *output, GLint outputSize, EGLDisplay dpy, EGLSyncKHR sync, EGLint attribute, EGLint *value, EGLBoolean ret)
{
    sprintf(output, "eglGetSyncAttribKHR() ****todo\n");
}

void CAnalyzer::AnalyzeCreatePbufferSurface(GLchar *output, GLint outputSize, EGLDisplay dpy, EGLConfig config, const EGLint *attrib_list, EGLSurface ret)
{
    ANALYZE_BEGIN_EVENT();

    static EGLint   pbufferCount = 0;
    stEglSurface    *p      = _GetSurface(dpy, ret);
    const EGLint    *pAttr  = NULL;

    if (!p)
    {
        sprintf(output, "eglCreateCreatePbufferSurface(invalid)\n");
        goto _Err;
    }

    sprintf(output, "eglCreatePbufferSurface(display = 0x%08X, config = 0x%08X, attrib_list = 0x%08X) 0x%08X\n",
        dpy, config, attrib_list, p->name, ret);

_Err:
    ANALYZE_END_EVENT(EGL_API_eglCreatePbufferSurface);
}

void CAnalyzer::AnalyzeCreatePixmapSurface(GLchar *output, GLint outputSize, EGLDisplay dpy, EGLConfig config, EGLNativePixmapType pixmap, const EGLint *attrib_list, EGLSurface res)
{
    ANALYZE_BEGIN_EVENT();
    sprintf(output, "eglCreatePixmapSurface() ****todo\n");
    ANALYZE_END_EVENT(EGL_API_eglCreatePixmapSurface);
}

void CAnalyzer::AnalyzeCreateWindowSurface(GLchar *output, GLint outputSize, EGLDisplay dpy, EGLConfig config, EGLNativeWindowType win, const EGLint *attrib_list, EGLSurface ret)
{
    ANALYZE_BEGIN_EVENT();

    stEglSurface    *p          = _GetSurface(dpy, ret);
    const EGLint    *pAttr      = NULL;

    if (!p)
    {
        sprintf(output, "eglCreateWindowSurface(invalid)\n");
        goto _Err;
    }

    sprintf(output, "eglCreateWindowSurface(display = 0x%08X, config = 0x%08X, window = 0x%08X) %dx%d, %s\n",
        dpy, config, win, p->width, p->height, p->name);

    pAttr = p->attrib_list;
    if (pAttr)
    {
        while (*pAttr != EGL_NONE)
        {
            EGLint name = pAttr[0];
            EGLint value = pAttr[1];

            TranslateEGLAttrib(name, tmp3);
            if (value == EGL_BACK_BUFFER)
            {
                TranslateEGLAttrib(value, tmp4);
                OutputStrcat(output, outputSize, "    %s: %s\n", tmp3, tmp4);
            }
            else
            {
                OutputStrcat(output, outputSize, "    %s: %d\n", tmp3, value);
            }

            pAttr += 2;
        }
    }

_Err:
    ANALYZE_END_EVENT(EGL_API_eglCreateWindowSurface);
}

void CAnalyzer::AnalyzeDestroyContext(GLchar *output, GLint outputSize, EGLDisplay dpy, EGLContext ctx, EGLBoolean res)
{
    ANALYZE_BEGIN_EVENT();
    PrintParams2(output, outputSize, "eglDestroyContext",
        (GLuint)dpy, "display", INT_04D,
        (GLuint)ctx, "context", INT_0X08X);
    ANALYZE_END_EVENT(EGL_API_eglDestroyContext);
}

void CAnalyzer::AnalyzeDestroySurface(GLchar *output, GLint outputSize, EGLDisplay dpy, EGLSurface surface, EGLBoolean res)
{
    ANALYZE_BEGIN_EVENT();
    PrintParams2(output, outputSize, "eglDestroySurface",
        (GLuint)dpy, "display", INT_04D,
        (GLuint)surface, "surface", INT_0X08X);
    ANALYZE_END_EVENT(EGL_API_eglDestroySurface);
}

void CAnalyzer::AnalyzeGetConfigAttrib(GLchar *output, GLint outputSize, EGLDisplay dpy, EGLConfig config, EGLint attribute, EGLint *value, EGLBoolean res)
{
    ANALYZE_BEGIN_EVENT();
    TranslateEGLAttrib(attribute, tmp5);
    sprintf(output, "eglGetConfigAttrib(display = 0x%08X, config = 0x%08X, attrib = %s, value = 0x%08X)\n",
        dpy, config, tmp5, value);
    ANALYZE_END_EVENT(EGL_API_eglGetConfigAttrib);
}

void CAnalyzer::AnalyzeGetConfigs(GLchar *output, GLint outputSize, EGLDisplay dpy, EGLConfig *configs, EGLint config_size, EGLint *num_config, EGLBoolean res)
{
    ANALYZE_BEGIN_EVENT();
    sprintf(output, "eglGetConfigs(display = 0x%08X, configs = 0x%08X, config_size = 0x%08X, num_config = 0x%08X)\n",
        dpy, configs, config_size, num_config);
    ANALYZE_END_EVENT(EGL_API_eglGetConfigs);
}

void CAnalyzer::AnalyzeGetCurrentDisplay(GLchar *output, GLint outputSize, EGLDisplay res)
{
    ANALYZE_BEGIN_EVENT();
    sprintf(output, "eglGetCurrentDisplay() = 0x%08X\n", res);
    ANALYZE_END_EVENT(EGL_API_eglGetCurrentDisplay);
}

void CAnalyzer::AnalyzeGetCurrentSurface(GLchar *output, GLint outputSize, EGLint readdraw, EGLSurface res)
{
    ANALYZE_BEGIN_EVENT();
    const GLchar *ptr = NULL;

    switch (readdraw)
    {
    case EGL_READ:
        ptr = "EGL_READ";
        break;

    case EGL_DRAW:
        ptr = "EGL_DRAW";
        break;

    default:
        ptr = "Invalid";
        break;
    }

    sprintf(output, "eglGetCurrentSurface(readdraw = %s) = 0x%08X\n", ptr, res);
    ANALYZE_END_EVENT(EGL_API_eglGetCurrentSurface);
}

void CAnalyzer::AnalyzeGetDisplay(GLchar *output, GLint outputSize, EGLNativeDisplayType display_id, EGLDisplay res)
{
    ANALYZE_BEGIN_EVENT();
    sprintf(output, "eglGetDisplay(display_id = 0x%08X) display = 0x%08X\n", display_id, res);
    ANALYZE_END_EVENT(EGL_API_eglGetDisplay);
}

void CAnalyzer::AnalyzeEGLGetError(GLchar *output, GLint outputSize, EGLint err)
{
    ANALYZE_BEGIN_EVENT();
    if (err != EGL_SUCCESS)
    {
        TranslateEglError(err, tmp1);
        sprintf(output, "eglGetError() = %s\n", tmp1);
    }
    ANALYZE_END_EVENT(EGL_API_eglGetError);
}

void CAnalyzer::AnalyzeSetFIFO(GLchar *output, GLint outputSize, void* fifo, EGLBoolean res)
{
    //ANALYZE_BEGIN_EVENT();
    //ANALYZE_END_EVENT(EGL_API_eglSetFIFO);
}

void CAnalyzer::AnalyzeGetProcAddress(GLchar *output, GLint outputSize, const char *procname, void *func)
{
    ANALYZE_BEGIN_EVENT();
    sprintf(output, "eglGetProcAddress(name=%s) = 0x%08X\n", procname, func);
    ANALYZE_END_EVENT(EGL_API_eglGetProcAddress);
}

void CAnalyzer::AnalyzeInitialize(GLchar *output, GLint outputSize, EGLDisplay dpy, EGLint *major, EGLint *minor, EGLBoolean res)
{
    ANALYZE_BEGIN_EVENT();
    PrintParams1(output, outputSize, "eglInitialize", (GLuint)dpy, "display", INT_04D);
    ANALYZE_END_EVENT(EGL_API_eglInitialize);
}

void CAnalyzer::AnalyzeMakeCurrent(GLchar *output, GLint outputSize, EGLDisplay dpy, EGLSurface draw, EGLSurface read, EGLContext ctx, EGLBoolean res)
{
    ANALYZE_BEGIN_EVENT();

    PEglThreadData  p       = _GetThreadData();
    CEGLContext     *pCtx   = NULL;
    stEglSurface    *pDraw  = _GetSurface(dpy, draw);
    stEglSurface    *pRead  = _GetSurface(dpy, read);

    if (ctx)
    {
        pCtx = p->context;
    }

    sprintf(output, "eglMakeCurrent(display = 0x%08X, Draw = %s, Read = %s, Context = %s)\n",
        dpy, pDraw?pDraw->name:"null", pRead?pRead->name:"null", pCtx?pCtx->name:"null");
    ANALYZE_END_EVENT(EGL_API_eglMakeCurrent);
}

void CAnalyzer::AnalyzeQueryContext(GLchar *output, GLint outputSize, EGLDisplay dpy, EGLContext ctx, EGLint attribute, EGLint *value, EGLBoolean res)
{
    ANALYZE_BEGIN_EVENT();

    const GLchar *ptr   = NULL;
    CEGLContext  *pCtx  = _GetEglContext(dpy, ctx);

    switch (attribute)
    {
    case EGL_CONFIG_ID:
        ptr = "EGL_CONFIG_ID";
        break;

    case EGL_CONTEXT_CLIENT_TYPE:
        ptr = "EGL_CONTEXT_CLIENT_TYPE";
        break;

    case EGL_CONTEXT_CLIENT_VERSION:
        ptr = "EGL_CONTEXT_CLIENT_VERSION";
        break;

    case EGL_RENDER_BUFFER:
        ptr = "EGL_RENDER_BUFFER";
        break;

    default:
        ptr = "Invalid";
        break;
    }

    sprintf(output, "eglQueryContex(display = 0x%08X, context = %s, attrib = %s, value = 0x%08X)\n",
        dpy, pCtx->name, ptr, *value);

    ANALYZE_END_EVENT(EGL_API_eglQueryContext);
}

void CAnalyzer::AnalyzeQueryString(GLchar *output, GLint outputSize, EGLDisplay dpy, EGLint name, const char* res)
{
    ANALYZE_BEGIN_EVENT();
    const GLchar *ptr = NULL;

    switch (name)
    {
    case EGL_CLIENT_APIS:
        ptr = "EGL_CLIENT_APIS";
        break;

    case EGL_EXTENSIONS:
        ptr = "EGL_EXTENSIONS";
        break;

    case EGL_VENDOR:
        ptr = "EGL_VENDOR";
        break;

    case EGL_VERSION:
        ptr = "EGL_VERSION";
        break;

    default:
        /* Bad parameter. */
        ptr = "Invalid";
        break;
    }

    sprintf(output, "eglQueryString(display = 0x%X, name = %s)\n", dpy, ptr);
    OutputStrcat(output, outputSize, "    %s\n", res);

    ANALYZE_END_EVENT(EGL_API_eglQueryString);
}

void CAnalyzer::AnalyzeQuerySurface(GLchar *output, GLint outputSize, EGLDisplay dpy, EGLSurface surface, EGLint attribute, EGLint *value, EGLBoolean res)
{
    ANALYZE_BEGIN_EVENT();

    const GLchar    *ptr    = NULL;
    stEglSurface    *pSurf  = _GetSurface(dpy, surface);

    switch (attribute)
    {
    case EGL_CONFIG_ID:
        ptr = "EGL_CONFIG_ID";
        break;

    case EGL_WIDTH:
        ptr = "EGL_WIDTH";
        break;

    case EGL_HEIGHT:
        ptr = "EGL_HEIGHT";
        break;

    case EGL_MIPMAP_TEXTURE:
        ptr = "EGL_MIPMAP_TEXTURE";
        break;

    case EGL_MIPMAP_LEVEL:
        ptr = "EGL_MIPMAP_LEVEL";
        break;

    case EGL_TEXTURE_FORMAT:
        ptr = "EGL_TEXTURE_FORMAT";
        break;

    case EGL_TEXTURE_TARGET:
        ptr = "EGL_TEXTURE_TARGET";
        break;

    case EGL_LARGEST_PBUFFER:
        ptr = "EGL_LARGEST_PBUFFER";
        break;

    case EGL_HORIZONTAL_RESOLUTION:
    case EGL_VERTICAL_RESOLUTION:
    case EGL_PIXEL_ASPECT_RATIO:
        ptr = "EGL_HORIZONTAL_RESOLUTION";
        break;

    case EGL_RENDER_BUFFER:
        ptr = "EGL_RENDER_BUFFER";
        break;

    case EGL_SWAP_BEHAVIOR:
        ptr = "EGL_SWAP_BEHAVIOR";
        break;

    case EGL_VG_ALPHA_FORMAT:
    case EGL_VG_COLORSPACE:
        /* Not yet implemented. */
        ptr = "Not Implemented";
        break;

    case EGL_BITMAP_POINTER_KHR:
        ptr = "EGL_BITMAP_POINTER_KHR";
        break;

    case EGL_BITMAP_PITCH_KHR:
        ptr = "EGL_BITMAP_PITCH_KHR";
        break;

    case EGL_BITMAP_ORIGIN_KHR:
        ptr = "EGL_BITMAP_ORIGIN_KHR";
        break;

    case EGL_BITMAP_PIXEL_RED_OFFSET_KHR:
    case EGL_BITMAP_PIXEL_GREEN_OFFSET_KHR:
    case EGL_BITMAP_PIXEL_BLUE_OFFSET_KHR:
    case EGL_BITMAP_PIXEL_ALPHA_OFFSET_KHR:
    case EGL_BITMAP_PIXEL_LUMINANCE_OFFSET_KHR:
        ptr = "EGL_BITMAP_PIXEL_RED_OFFSET_KHR";
        break;

    default:
        /* Bad attribute. */
        ptr = "Invalid";
        break;
    }

    sprintf(output, "eglQuerySurface(display = 0x%08X, surface = %s, attrib = %s, value = 0x%08X)\n",
        dpy, pSurf->name, ptr, *value);
    ANALYZE_END_EVENT(EGL_API_eglQuerySurface);
}

void CAnalyzer::AnalyzeSwapBuffers(GLchar *output, GLint outputSize, EGLDisplay dpy, EGLSurface surface, EGLBoolean res)
{
    ANALYZE_BEGIN_EVENT();
    GLint pos = 0;
    stEglSurface *pSurf = _GetSurface(dpy, surface);

    OutputStringFast(eglSwapBuffers1, &pos, output, outputSize, "\n************************************************eglSwapBuffers************************************************\n");
    OutputStringFast(eglSwapBuffers2, &pos, output, outputSize, "****%s eglSwapBuffers(display=0x%08X, draw=%s)\n", thread, dpy, pSurf->name);
    OutputStringFast(eglSwapBuffers3, &pos, output, outputSize, "**** Frames: %04d\n", CURRENT_EGL_CONTEXT()->nFrame++);
    OutputStringFast(eglSwapBuffers4, &pos, output, outputSize, "************************************************eglSwapBuffers************************************************\n\n");
    ANALYZE_END_EVENT(EGL_API_eglSwapBuffers);
}

void CAnalyzer::AnalyzeTerminate(GLchar *output, GLint outputSize, EGLDisplay dpy, EGLBoolean res)
{
    ANALYZE_BEGIN_EVENT();
    PrintParams1(output, outputSize, "eglTerminate", (GLuint)dpy, "display", INT_04D);
    ANALYZE_END_EVENT(EGL_API_eglTerminate);
}

void CAnalyzer::AnalyzeWaitGL(GLchar *output, GLint outputSize, EGLBoolean res)
{
    ANALYZE_BEGIN_EVENT();
    ANALYZE_END_EVENT(EGL_API_eglWaitGL);
}

void CAnalyzer::AnalyzeWaitNative(GLchar *output, GLint outputSize, EGLint engine, EGLBoolean res)
{
    ANALYZE_BEGIN_EVENT();
    sprintf(output, "eglWaitNative()\n");
    ANALYZE_END_EVENT(EGL_API_eglWaitNative);
}

void CAnalyzer::AnalyzeBindTexImage(GLchar *output, GLint outputSize, EGLDisplay dpy, EGLSurface surface, EGLint buffer, EGLBoolean res)
{
    ANALYZE_BEGIN_EVENT();

    PrintParams3(output, outputSize, "eglBindTexImage",
                (GLuint)dpy, "display", INT_04D,
                (GLuint)surface, "surface", INT_0X08X,
                (GLuint)buffer, "buffer", INT_0X08X);

    ANALYZE_END_EVENT(EGL_API_eglBindTexImage);
}

void CAnalyzer::AnalyzeReleaseTexImage(GLchar *output, GLint outputSize, EGLDisplay dpy, EGLSurface surface, EGLint buffer, EGLBoolean res)
{
    ANALYZE_BEGIN_EVENT();
    PrintParams3(output, outputSize, "eglReleaseTexImage",
        (GLuint)dpy, "display", INT_04D,
        (GLuint)surface, "surface", INT_0X08X,
        (GLuint)buffer, "buffer", INT_0X08X);
    ANALYZE_END_EVENT(EGL_API_eglReleaseTexImage);
}

void CAnalyzer::AnalyzeSurfaceAttrib(GLchar *output, GLint outputSize, EGLDisplay dpy, EGLSurface surface, EGLint attribute, EGLint value, EGLBoolean res)
{
    ANALYZE_BEGIN_EVENT();

    const GLchar *ptr = NULL;

    switch (attribute)
    {
    case EGL_MIPMAP_LEVEL:
        ptr = "EGL_MIPMAP_LEVEL";
        break;

    case EGL_SWAP_BEHAVIOR:
        ptr = "EGL_SWAP_BEHAVIOR";
        break;

    default:
        /* Invalid attribute. */
        ptr = "Invalid";
        break;
    }
    PrintParams4(output, outputSize, "eglSurfaceAttrib",
        (GLuint)dpy, "display", INT_04D,
        (GLuint)surface, "interval", INT_0X08X,
        (GLuint)ptr, "", STR_STR,
        (GLuint)value, "value", INT_0X08X);

    ANALYZE_END_EVENT(EGL_API_eglSurfaceAttrib);
}

void CAnalyzer::AnalyzeSwapInterval(GLchar *output, GLint outputSize, EGLDisplay dpy, EGLint interval, EGLBoolean res)
{
    ANALYZE_BEGIN_EVENT();
    PrintParams2(output, outputSize, "eglSwapInterval",
        (GLuint)dpy, "display", INT_04D,
        (GLuint)interval, "interval", INT_0X08X);
    ANALYZE_END_EVENT(EGL_API_eglSwapInterval);
}

void CAnalyzer::AnalyzeBindAPI(GLchar *output, GLint outputSize, EGLenum api, EGLBoolean res)
{
    ANALYZE_BEGIN_EVENT();

    GLchar * str;
    switch (api)
    {
    case EGL_OPENGL_ES_API:
        str = "EGL_OPENGL_ES_API";
        break;

    case EGL_OPENVG_API:
        str = "EGL_OPENVG_API";
        break;

    default:
        /* Bad parameter. */
        str = "Invalid";
        break;
    }

    PrintParams1(output, outputSize, "eglBindAPI", (GLuint)str, "", STR_STR);

    ANALYZE_END_EVENT(EGL_API_eglBindAPI);
}

void CAnalyzer::AnalyzeQueryAPI(GLchar *output, GLint outputSize, EGLenum res)
{
    ANALYZE_BEGIN_EVENT();
    sprintf(output, "eglQueryAPI() ****todo\n");
    ANALYZE_END_EVENT(EGL_API_eglQueryAPI);
}

void CAnalyzer::AnalyzeCreatePbufferFromClientBuffer(GLchar *output, GLint outputSize, EGLDisplay dpy, EGLenum buftype, EGLClientBuffer buffer, EGLConfig config, const EGLint *attrib_list, EGLSurface res)
{
    ANALYZE_BEGIN_EVENT();
    sprintf(output, "eglCreatePbufferFromClientBuffer() ****todo\n");
    ANALYZE_END_EVENT(EGL_API_eglCreatePbufferFromClientBuffer);
}

void CAnalyzer::AnalyzeReleaseThread(GLchar *output, GLint outputSize, EGLBoolean res)
{
    ANALYZE_BEGIN_EVENT();
    PrintParams0(output, outputSize, "eglReleaseThread");
    ANALYZE_END_EVENT(EGL_API_eglReleaseThread);
}

void CAnalyzer::AnalyzeWaitClient(GLchar *output, GLint outputSize, EGLBoolean res)
{
    ANALYZE_BEGIN_EVENT();
    sprintf(output, "eglWaitClient()\n");
    ANALYZE_END_EVENT(EGL_API_eglWaitClient);
}

void CAnalyzer::AnalyzeGetCurrentContext(GLchar *output, GLint outputSize, EGLContext res)
{
    ANALYZE_BEGIN_EVENT();
    sprintf(output, "eglGetCurrentContext() = 0x%08X\n", res);
    ANALYZE_END_EVENT(EGL_API_eglGetCurrentContext);
}

void CAnalyzer::AnalyzeLockSurfaceKHR(GLchar *output, GLint outputSize, EGLDisplay display, EGLSurface surface, const EGLint *attrib_list, EGLBoolean ret)
{
    ANALYZE_BEGIN_EVENT();
    sprintf(output, "eglLockSurfaceKHR() ****todo\n");
    ANALYZE_END_EVENT(EGL_API_eglLockSurfaceKHR);
}

void CAnalyzer::AnalyzeUnlockSurfaceKHR(GLchar *output, GLint outputSize, EGLDisplay display, EGLSurface surface, EGLBoolean ret)
{
    ANALYZE_BEGIN_EVENT();
    sprintf(output, "eglUnlockSurfaceKHR() ****todo\n");
    ANALYZE_END_EVENT(EGL_API_eglUnlockSurfaceKHR);
}

GLvoid CAnalyzer::AnalyzeDrawBuffers(GLchar *output, GLint outputSize, GLsizei n, const GLenum *bufs)
{
    ANALYZE_BEGIN_EVENT();

    sprintf(output, "glDrawBuffers(%d) ", n);

    for (GLint i=0; i<n; i++)
    {
        TranslateAttachment(bufs[i], tmp1);
        OutputStrcat(output, outputSize, "%s ", tmp1);
    }
    OutputStrcat(output, outputSize, "\n", tmp1);

    ANALYZE_END_EVENT(GL3_API_glDrawBuffers);
}

GLvoid CAnalyzer::AnalyzeMapBufferRange(GLchar *output, GLint outputSize, GLenum target, GLintptr offset, GLsizeiptr length, GLbitfield access, GLvoid *mapped)
{
    ANALYZE_BEGIN_EVENT();
    TranslateBufferTarget(target, tmp1);
    TranslateAccessBit(access, tmp2);
    sprintf(output, "glMapBufferRange(%s, offset=%d, len=%d, access=%s)\n", tmp1, (GLint)offset, (GLint)length, tmp2);
    ANALYZE_END_EVENT(GL3_API_glMapBufferRange);
}

GLvoid CAnalyzer::AnalyzeFlushMappedBufferRange(GLchar *output, GLint outputSize, GLenum target, GLintptr offset, GLsizeiptr length)
{
    ANALYZE_BEGIN_EVENT();
    TranslateBufferTarget(target, tmp1);
    sprintf(output, "glFlushMappedBufferRange(%s, offset=%d, len=%d)\n", tmp1, (GLint)offset, (GLint)length);
    ANALYZE_END_EVENT(GL3_API_glFlushMappedBufferRange);
}

GLvoid CAnalyzer::AnalyzeUnmapBuffer(GLchar *output, GLint outputSize, GLenum target, GLboolean ret)
{
    ANALYZE_BEGIN_EVENT();
    TranslateBufferTarget(target, tmp1);
    sprintf(output, "glUnmapBuffer(%s) = %d\n", tmp1, ret);
    ANALYZE_END_EVENT(GL3_API_glUnmapBuffer);
}

GLvoid CAnalyzer::AnalyzeInvalidateFramebuffer(GLchar *output, GLint outputSize, GLenum target, GLsizei numAttachments, const GLenum *attachments)
{
    ANALYZE_BEGIN_EVENT();
    TranslateFramebufferRenderbuffer(target, tmp1);
    sprintf(output, "glInvalidateFramebuffer(%s, %d, ", tmp1, numAttachments);

    for (GLint i=0; i<numAttachments; i++)
    {
        TranslateAttachment(attachments[i], tmp1);
        OutputStrcat(output, outputSize, "%s ", tmp1);
    }
    OutputStrcat(output, outputSize, ")\n", tmp1);
    ANALYZE_END_EVENT(GL3_API_glInvalidateFramebuffer);
}

GLvoid CAnalyzer::AnalyzeInvalidateSubFramebuffer(GLchar *output, GLint outputSize, GLenum target, GLsizei numAttachments, const GLenum *attachments, GLint x, GLint y, GLsizei width, GLsizei height)
{
    ANALYZE_BEGIN_EVENT();
    sprintf(output, "glInvalidateSubFramebuffer() ***todo\n");
    ANALYZE_END_EVENT(GL3_API_glInvalidateSubFramebuffer);
}

GLvoid CAnalyzer::AnalyzeGetProgramBinary(GLchar *output, GLint outputSize, GLuint program, GLsizei bufSize, GLsizei *length, GLenum *binaryFormat, void *binary)
{
    ANALYZE_BEGIN_EVENT();
    sprintf(output, "glGetProgramBinary(prog=%08d, %d, 0x%04X)\n", program, bufSize, *binaryFormat);
    ANALYZE_END_EVENT(GL3_API_glGetProgramBinary);
}

GLvoid CAnalyzer::AnalyzeTransformFeedbackVaryings(GLchar *output, GLint outputSize, GLuint program, GLsizei count, const GLchar *const*varyings, GLenum bufferMode)
{
    ANALYZE_BEGIN_EVENT();

    GLchar *pVaryings = NULL;
    GLchar *strBufMode = NULL;

    sprintf(output, "glTransformFeedbackVaryings(prog=%08d, %d, ", program, count);
    for (GLsizei i=0; i<count; i++)
    {
        pVaryings = (GLchar*)varyings[i];
        OutputStrcat(output, outputSize, "%s ", pVaryings);
    }

    switch (bufferMode)
    {
    case GL_INTERLEAVED_ATTRIBS:
        strBufMode = "GL_INTERLEAVED_ATTRIBS";
        break;

    case GL_SEPARATE_ATTRIBS:
        strBufMode = "GL_SEPARATE_ATTRIBS";
        break;

    default:
        strBufMode = "invalid";
        break;
    }

    OutputStrcat(output, outputSize, ", %s)\n", strBufMode);
    ANALYZE_END_EVENT(GL3_API_glTransformFeedbackVaryings);
}

GLvoid CAnalyzer::AnalyzeDiscardFramebufferEXT(GLchar *output, GLint outputSize, GLenum target, GLsizei numAttachments, const GLenum *attachments)
{
    ANALYZE_BEGIN_EVENT();
    sprintf(output, "glDiscardFramebufferEXT(FBO=%d)\n", attachments);
    ANALYZE_END_EVENT(GL3_API_glDiscardFramebufferEXT);
}

GLvoid CAnalyzer::AnalyzeEGLImageTargetTexture2DOES(GLchar *output, GLint outputSize, GLenum target, GLeglImageOES image)
{
    ANALYZE_BEGIN_EVENT();
    TranslateTexTarget(target, tmp1);
    PrintParams2(output, outputSize, "EGLImageTargetTexture2DOES",
        (GLuint)tmp1, "", STR_STR,
        (GLuint)image, "image", INT_0X08X);
    ANALYZE_END_EVENT(GL3_API_glDiscardFramebufferEXT);
}

GLvoid CAnalyzer::AnalyzeEGLImageTargetRenderbufferStorageOES(GLchar *output, GLint outputSize, GLenum target, GLeglImageOES image)
{
    ANALYZE_BEGIN_EVENT();
    PrintParams2(output, outputSize, "glEGLImageTargetRenderbufferStorageOES",
        (GLuint)target, "target", INT_0X08X,
        (GLuint)image, "image", INT_0X08X);
    ANALYZE_END_EVENT(GL3_API_glDiscardFramebufferEXT);
}

GLvoid CAnalyzer::AnalyzeGetProgramBinaryOES(GLchar *output, GLint outputSize, GLuint program, GLsizei bufSize, GLsizei *length, GLenum *binaryFormat, void *binary)
{
    ANALYZE_BEGIN_EVENT();
    sprintf(output, "glGetProgramBinaryOES() ***todo\n");
    ANALYZE_END_EVENT(GL3_API_glGetProgramBinaryOES);
}

GLvoid CAnalyzer::AnalyzeProgramBinaryOES(GLchar *output, GLint outputSize, GLuint program, GLenum binaryFormat, const void *binary, GLint length)
{
    ANALYZE_BEGIN_EVENT();
    if (binaryFormat != GL_PROGRAM_BINARY_VIV)
    {
        sprintf(output, "glProgramBinaryOES(prog=%04d, wrong binary format(0x%08X)\n", program, binaryFormat);
        goto _Err;
    }

    sprintf(output, "glProgramBinaryOES(prog=%08d, p=0x%08X, size=%d)\n", program, binary, length);

_Err:
    ANALYZE_END_EVENT(GL3_API_glProgramBinaryOES);
}

GLvoid CAnalyzer::PrintUniform1234if(
    GLchar *output,
    GLuint outputSize,
    GLuint location,
    eShaderType type,
    GLuint count,
    GLuint *value,
    const GLchar *funcName
    )
{
    GLuint      progID      = CURRENT_CONTEXT1()->curProgram;
    CProgramObj *prog       = CURRENT_CONTEXT1()->FindProgram(progID);
    CUniformObj *uniform    = prog->FindUniformByOldLoc(location);
    GLuint      totalSize   = GetSizeFromShaderDataType(type);

    if (location != 0xffffffff && uniform != NULL)
    {
        switch (type)
        {
        case DATA_TYPE_FLOAT_X1:
        case DATA_TYPE_FLOAT_X2:
        case DATA_TYPE_FLOAT_X3:
        case DATA_TYPE_FLOAT_X4:
            {
                GLfloat *fv = (GLfloat*)uniform->data;

                sprintf(output, "%s(%s,", funcName, uniform->name);
                for (GLuint i=0; i<count-1; i++)
                {
                    OutputStrcat(output, outputSize, " %f,", fv[0]);
                }
                OutputStrcat(output, outputSize, " %f)\n", fv[count-1]);
                break;
            }

        case DATA_TYPE_INTEGER_X1:
        case DATA_TYPE_INTEGER_X2:
        case DATA_TYPE_INTEGER_X3:
        case DATA_TYPE_INTEGER_X4:
            {
                GLint *iv = (GLint*)uniform->data;

                sprintf(output, "%s(%s,", funcName, uniform->name);
                for (GLuint i=0; i<count-1; i++)
                {
                    OutputStrcat(output, outputSize, " %d,", iv[0]);
                }
                OutputStrcat(output, outputSize, " %d)\n", iv[count-1]);
                break;
            }

        default:
            Abort("%s(%d): Unknown type(0x%04X)\n", __FUNCTION__, __LINE__, type);
            break;
        }
    }
    else
    {
        sprintf(output, "%s(%s) data error\n", funcName, uniform?uniform->name:"");
    }
}

GLvoid CAnalyzer::PrintUniform1234ifv(
    GLint *pos,
    GLchar *output,
    GLuint outputSize,
    GLuint location,
    eShaderType type,
    GLuint count,
    GLuint num,
    GLuint handle,
    const GLchar *funcName
    )
{
    GLuint      progID      = CURRENT_CONTEXT1()->curProgram;
    CProgramObj *prog       = CURRENT_CONTEXT1()->FindProgram(progID);
    CUniformObj *pUniform   = prog->FindUniformByOldLoc(location);
    GLuint      nDataSize   = num*count*4;

    if (location != 0xffffffff && pUniform != NULL)
    {
        GLchar *p = (GLchar*)pUniform->data;

        OutputStringFast(gles2Disassemble2, pos, output, outputSize, "%s(%s, %02d)\n", funcName, pUniform->name, num);
        if (p)
        {
            for (GLuint i=0; i<num; i++)
            {
                switch (type)
                {
                case DATA_TYPE_FLOAT_X1:
                case DATA_TYPE_FLOAT_X2:
                case DATA_TYPE_FLOAT_X3:
                case DATA_TYPE_FLOAT_X4:
                    DumpVector(pos, GL_FLOAT, count, p, output, outputSize);
                    break;

                case DATA_TYPE_INTEGER_X1:
                case DATA_TYPE_INTEGER_X2:
                case DATA_TYPE_INTEGER_X3:
                case DATA_TYPE_INTEGER_X4:
                    DumpVector(pos, GL_INT, count, p, output, outputSize);
                    break;

                default:
                    Abort("%s(%d): Unknown type(0x%04X)\n", __FUNCTION__, __LINE__, type);
                    break;
                }

                p += (count*4);
            }
        }
    }
    else
    {
        OutputStringFast(gles2Disassemble2, pos, output, outputSize,
            "%s(%s) loc(%d) data error\n", funcName, pUniform?pUniform->name:"null", location);
    }
}

GLvoid CAnalyzer::PrintUniformMatrix234fv(
    GLint *pos,
    GLchar *output,
    GLuint outputSize,
    GLuint location,
    eShaderType type,
    GLuint count,
    GLuint num,
    GLuint handle,
    GLuint transpose,
    const GLchar *funcName
    )
{
    GLuint      progID      = CURRENT_CONTEXT1()->curProgram;
    CProgramObj *prog       = CURRENT_CONTEXT1()->FindProgram(progID);
    CUniformObj *pUniform   = prog->FindUniformByOldLoc(location);

    if (location != 0xffffffff && pUniform != NULL)
    {
        GLfloat *p = (GLfloat*)pUniform->data;

        OutputStringFast(gles2Disassemble11, pos, output, outputSize, "%s(%s, %02d, t=%d)\n",
            funcName, pUniform->name, num, transpose);

        for (GLuint j=0; j<num; j++)
        {
            OutputStringFast(gles2Disassemble10, pos, output, outputSize, "m%02d:\n", j+1);

            if (count == 16)
            {
                DumpMatrix(pos, GL_FLOAT, 4, 4, p, output, outputSize);
            }
            else if (count == 9)
            {
                DumpMatrix(pos, GL_FLOAT, 3, 3, p, output, outputSize);
            }
            else if (count == 4)
            {
                DumpMatrix(pos, GL_FLOAT, 2, 2, p, output, outputSize);
            }

            p += count;
        }
    }
}

GLvoid CAnalyzer::AnalyzeCopyTexSubImage3D(GLchar *output, GLint outputSize, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLint x, GLint y, GLsizei width, GLsizei height)
{
    ANALYZE_BEGIN_EVENT();
    sprintf(output, "glCopyTexSubImage3D() ***todo\n");
    ANALYZE_END_EVENT(GL3_API_glCopyTexSubImage3D);
}

GLvoid CAnalyzer::AnalyzeCompressedTexImage3D(GLchar *output, GLint outputSize, GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLsizei imageSize, const void *data)
{
    ANALYZE_BEGIN_EVENT();
    sprintf(output, "glCompressedTexImage3D() ***todo\n");
    ANALYZE_END_EVENT(GL3_API_glCompressedTexImage3D);
}

GLvoid CAnalyzer::AnalyzeCompressedTexSubImage3D(GLchar *output, GLint outputSize, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLsizei imageSize, const void *data)
{
    ANALYZE_BEGIN_EVENT();
    sprintf(output, "glGetCompressedTexSubImage3D() ***todo\n");
    ANALYZE_END_EVENT(GL3_API_glCompressedTexSubImage3D);
}

GLvoid CAnalyzer::AnalyzeIsVertexArray(GLchar *output, GLint outputSize, GLuint array, GLboolean ret)
{
    ANALYZE_BEGIN_EVENT();
    PrintParams1Return(output, outputSize, "glIsVertexArray", (GLuint)array, "vao", INT_04D, (GLuint)ret, INT_04D);
    ANALYZE_END_EVENT(GL3_API_glIsVertexArray);
}