#include "utils.h"
#include "GLESShare.h"
#include "TranslateGL.h"
#include "ApiGLES3Context.h"
#include "Analyzer.h"
#include "egl.h"
#include "gles3.h"

CAnalyzer::CAnalyzer()
{
    m_bAnalyzeEnabled   = GL_TRUE;
    m_bErrorCheck       = GL_FALSE;
}

CAnalyzer::~CAnalyzer()
{

}

GLvoid CAnalyzer::InitAnalyzer(const GLchar *filePath, CGLES3Context *pContext)
{
    GLchar  *p = new GLchar[256];
    if (p == NULL)
        return;

    if (!m_bAnalyzeEnabled)
    {
        delete p;
        return;
    }

    memset(p, 0, 256);
    sprintf(p, "%s/vtracer", filePath);

    if (!OpenDumpFile())
    {
        return;
    }

    EGLInit();
    GLES3Init();

    m_nEventSequence            = 0;
    m_nCurrentLoadEventSequence = 0;
    m_pContext                  = pContext;

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

    OutputToFile(GL_FALSE, 0, "", string, g_pOutputFile);
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
        GLenum err = glGetError();
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

GLvoid CAnalyzer::AnalyzeGenBuffers(GLchar *output, GLint outputSize, GLsizei n, GLuint *buffers)
{
    ANALYZE_BEGIN_EVENT();
    GenDelObjects(m_pContext, output, outputSize, n, buffers, "glGenBuffers", "vbo");
    ANALYZE_END_EVENT(GL3_API_glGenBuffers);
}

GLvoid CAnalyzer::AnalyzeDeleteBuffers(GLchar *output, GLint outputSize, GLsizei n, const GLuint *buffers)
{
    ANALYZE_BEGIN_EVENT();
    GenDelObjects(m_pContext, output, outputSize, n, buffers, "glDeleteBuffers", "vbo");
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
    ANALYZE_END_EVENT(GL3_API_glBindBufferBase);
}

GLvoid CAnalyzer::AnalyzeBindBufferRange(GLchar *output, GLint outputSize, GLenum target, GLuint index, GLuint buffer, GLintptr offset, GLsizeiptr size)
{
    ANALYZE_BEGIN_EVENT();
    ANALYZE_END_EVENT(GL3_API_glBindBufferRange);
}

GLvoid CAnalyzer::AnalyzeBufferData(GLchar *output, GLint outputSize, GLenum target, GLsizeiptr size, const void *data, GLenum usage)
{
    ANALYZE_BEGIN_EVENT();
    ANALYZE_END_EVENT(GL3_API_glBufferData);
}

GLvoid CAnalyzer::AnalyzeBufferSubData(GLchar *output, GLint outputSize, GLenum target, GLintptr offset, GLsizeiptr size, const void *data)
{
    ANALYZE_BEGIN_EVENT();
    ANALYZE_END_EVENT(GL3_API_glBufferSubData);
}

GLvoid CAnalyzer::AnalyzeDeleteVertexArrays(GLchar *output, GLint outputSize, GLsizei n, const GLuint *arrays)
{
    ANALYZE_BEGIN_EVENT();
    GenDelObjects(m_pContext, output, outputSize, n, arrays, "glDeleteVertexArrays", "vao");
    ANALYZE_END_EVENT(GL3_API_glDeleteVertexArrays);
}

GLvoid CAnalyzer::AnalyzeGenVertexArrays(GLchar *output, GLint outputSize, GLsizei n, GLuint *arrays)
{
    ANALYZE_BEGIN_EVENT();
    GenDelObjects(m_pContext, output, outputSize, n, arrays, "glGenVertexArrays", "vao");
    ANALYZE_END_EVENT(GL3_API_glGenVertexArrays);
}

GLvoid CAnalyzer::AnalyzeBindVertexArray(GLchar *output, GLint outputSize, GLuint array)
{
    ANALYZE_BEGIN_EVENT();
    ANALYZE_END_EVENT(GL3_API_glBindVertexArray);
}

GLvoid CAnalyzer::AnalyzeBindTransformFeedback(GLchar *output, GLint outputSize, GLenum target, GLuint id)
{
    ANALYZE_BEGIN_EVENT();
    ANALYZE_END_EVENT(GL3_API_glBindTransformFeedback);
}

GLvoid CAnalyzer::AnalyzeDeleteTransformFeedbacks(GLchar *output, GLint outputSize, GLsizei n, const GLuint *ids)
{
    ANALYZE_BEGIN_EVENT();
    GenDelObjects(m_pContext, output, outputSize, n, ids, "glDeleteTransformFeedbacks", "tfb");
    ANALYZE_END_EVENT(GL3_API_glDeleteTransformFeedbacks);
}

GLvoid CAnalyzer::AnalyzeGenTransformFeedbacks(GLchar *output, GLint outputSize, GLsizei n, GLuint *ids)
{
    ANALYZE_BEGIN_EVENT();
    GenDelObjects(m_pContext, output, outputSize, n, ids, "glGenTransformFeedbacks", "tfb");
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
    ANALYZE_END_EVENT(GL3_API_glBeginTransformFeedback);
}

GLvoid CAnalyzer::AnalyzeEndTransformFeedback(GLchar *output, GLint outputSize)
{
    ANALYZE_BEGIN_EVENT();
    ANALYZE_END_EVENT(GL3_API_glEndTransformFeedback);
}

GLvoid CAnalyzer::AnalyzeDisableVertexAttribArray(GLchar *output, GLint outputSize, GLuint index)
{
    ANALYZE_BEGIN_EVENT();
    ANALYZE_END_EVENT(GL3_API_glDisableVertexAttribArray);
}

GLvoid CAnalyzer::AnalyzeEnableVertexAttribArray(GLchar *output, GLint outputSize, GLuint index)
{
    ANALYZE_BEGIN_EVENT();
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
    ANALYZE_END_EVENT(GL3_API_glVertexAttribPointer);
}

GLvoid CAnalyzer::AnalyzeVertexAttribIPointer(GLchar *output, GLint outputSize, GLuint index, GLint size, GLenum type, GLsizei stride, const void *pointer)
{
    ANALYZE_BEGIN_EVENT();
    ANALYZE_END_EVENT(GL3_API_glVertexAttribIPointer);
}

GLvoid CAnalyzer::AnalyzeGetBufferParameteriv(GLchar *output, GLint outputSize, GLenum target, GLenum pname, GLint *params)
{
    ANALYZE_BEGIN_EVENT();
    ANALYZE_END_EVENT(GL3_API_glGetBufferParameteriv);
}

GLvoid CAnalyzer::AnalyzeGetVertexAttribfv(GLchar *output, GLint outputSize, GLuint index, GLenum pname, GLfloat *params)
{
    ANALYZE_BEGIN_EVENT();
    ANALYZE_END_EVENT(GL3_API_glGetVertexAttribfv);
}

GLvoid CAnalyzer::AnalyzeGetVertexAttribiv(GLchar *output, GLint outputSize, GLuint index, GLenum pname, GLint *params)
{
    ANALYZE_BEGIN_EVENT();
    ANALYZE_END_EVENT(GL3_API_glGetVertexAttribiv);
}

GLvoid CAnalyzer::AnalyzeGetVertexAttribPointerv(GLchar *output, GLint outputSize, GLuint index, GLenum pname, void **pointer)
{
    ANALYZE_BEGIN_EVENT();
    ANALYZE_END_EVENT(GL3_API_glGetVertexAttribPointerv);
}

GLvoid CAnalyzer::AnalyzeCreateShader(GLchar *output, GLint outputSize, GLenum type, GLuint shader)
{
    ANALYZE_BEGIN_EVENT();
    ANALYZE_END_EVENT(GL3_API_glCreateShader);
}

GLvoid CAnalyzer::AnalyzeShaderSource(GLchar *output, GLint outputSize, GLuint shader, GLsizei count, const GLchar *const*string, const GLint *length)
{
    ANALYZE_BEGIN_EVENT();
    ANALYZE_END_EVENT(GL3_API_glShaderSource);
}

GLvoid CAnalyzer::AnalyzeDeleteShader(GLchar *output, GLint outputSize, GLuint shader)
{
    ANALYZE_BEGIN_EVENT();
    ANALYZE_END_EVENT(GL3_API_glDeleteShader);
}

GLvoid CAnalyzer::AnalyzeCreateProgram(GLchar *output, GLint outputSize, GLuint program)
{
    ANALYZE_BEGIN_EVENT();
    ANALYZE_END_EVENT(GL3_API_glCreateProgram);
}

GLvoid CAnalyzer::AnalyzeUseProgram(GLchar *output, GLint outputSize, GLuint program)
{
    ANALYZE_BEGIN_EVENT();
    ANALYZE_END_EVENT(GL3_API_glUseProgram);
}

GLvoid CAnalyzer::AnalyzeAttachShader(GLchar *output, GLint outputSize, GLuint program, GLuint shader)
{
    ANALYZE_BEGIN_EVENT();
    ANALYZE_END_EVENT(GL3_API_glAttachShader);
}

GLvoid CAnalyzer::AnalyzeGetUniformLocation(GLchar *output, GLint outputSize, GLuint program, const GLchar *name, GLint oldLoc)
{
    ANALYZE_BEGIN_EVENT();
    ANALYZE_END_EVENT(GL3_API_glGetUniformLocation);
}

GLvoid CAnalyzer::AnalyzeLinkProgram(GLchar *output, GLint outputSize, GLuint program)
{
    ANALYZE_BEGIN_EVENT();
    ANALYZE_END_EVENT(GL3_API_glLinkProgram);
}

GLvoid CAnalyzer::AnalyzeCompileShader(GLchar *output, GLint outputSize, GLuint shader)
{
    ANALYZE_BEGIN_EVENT();
    ANALYZE_END_EVENT(GL3_API_glCompileShader);
}

GLvoid CAnalyzer::AnalyzeDetachShader(GLchar *output, GLint outputSize, GLuint program, GLuint shader)
{
    ANALYZE_BEGIN_EVENT();
    ANALYZE_END_EVENT(GL3_API_glDetachShader);
}

GLvoid CAnalyzer::AnalyzeDeleteProgram(GLchar *output, GLint outputSize, GLuint program)
{
    ANALYZE_BEGIN_EVENT();
    ANALYZE_END_EVENT(GL3_API_glDeleteProgram);
}

GLvoid CAnalyzer::AnalyzeGetAttribLocation(GLchar *output, GLint outputSize, GLuint program, const GLchar *name, GLint oldLoc)
{
    ANALYZE_BEGIN_EVENT();
    ANALYZE_END_EVENT(GL3_API_glGetAttribLocation);
}

GLvoid CAnalyzer::AnalyzeBindAttribLocation(GLchar *output, GLint outputSize, GLuint program, GLint loc, const GLchar *name)
{
    ANALYZE_BEGIN_EVENT();
    ANALYZE_END_EVENT(GL3_API_glBindAttribLocation);
}

GLvoid CAnalyzer::AnalyzeProgramBinary(GLchar *output, GLint outputSize, GLuint program, GLenum binaryFormat, const void *binary, GLsizei length)
{
    ANALYZE_BEGIN_EVENT();
    ANALYZE_END_EVENT(GL3_API_glProgramBinary);
}

GLvoid CAnalyzer::AnalyzeUniform1f(GLchar *output, GLint outputSize, GLint location, GLfloat v0)
{
    ANALYZE_BEGIN_EVENT();
    ANALYZE_END_EVENT(GL3_API_glUniform1f);
}

GLvoid CAnalyzer::AnalyzeUniform1fv(GLchar *output, GLint outputSize, GLint location, GLsizei count, const GLfloat *value)
{
    ANALYZE_BEGIN_EVENT();
    ANALYZE_END_EVENT(GL3_API_glUniform1fv);
}

GLvoid CAnalyzer::AnalyzeUniform1i(GLchar *output, GLint outputSize, GLint location, GLint v0)
{
    ANALYZE_BEGIN_EVENT();
    ANALYZE_END_EVENT(GL3_API_glUniform1i);
}

GLvoid CAnalyzer::AnalyzeUniform1iv(GLchar *output, GLint outputSize, GLint location, GLsizei count, const GLint *value)
{
    ANALYZE_BEGIN_EVENT();
    ANALYZE_END_EVENT(GL3_API_glUniform1iv);
}

GLvoid CAnalyzer::AnalyzeUniform2f(GLchar *output, GLint outputSize, GLint location, GLfloat v0, GLfloat v1)
{
    ANALYZE_BEGIN_EVENT();
    ANALYZE_END_EVENT(GL3_API_glUniform2f);
}

GLvoid CAnalyzer::AnalyzeUniform2fv(GLchar *output, GLint outputSize, GLint location, GLsizei count, const GLfloat *value)
{
    ANALYZE_BEGIN_EVENT();
    ANALYZE_END_EVENT(GL3_API_glUniform2fv);
}

GLvoid CAnalyzer::AnalyzeUniform2i(GLchar *output, GLint outputSize, GLint location, GLint v0, GLint v1)
{
    ANALYZE_BEGIN_EVENT();
    ANALYZE_END_EVENT(GL3_API_glUniform2i);
}

GLvoid CAnalyzer::AnalyzeUniform2iv(GLchar *output, GLint outputSize, GLint location, GLsizei count, const GLint *value)
{
    ANALYZE_BEGIN_EVENT();
    ANALYZE_END_EVENT(GL3_API_glUniform2iv);
}

GLvoid CAnalyzer::AnalyzeUniform3f(GLchar *output, GLint outputSize, GLint location, GLfloat v0, GLfloat v1, GLfloat v2)
{
    ANALYZE_BEGIN_EVENT();
    ANALYZE_END_EVENT(GL3_API_glUniform3f);
}

GLvoid CAnalyzer::AnalyzeUniform3fv(GLchar *output, GLint outputSize, GLint location, GLsizei count, const GLfloat *value)
{
    ANALYZE_BEGIN_EVENT();
    ANALYZE_END_EVENT(GL3_API_glUniform3fv);
}

GLvoid CAnalyzer::AnalyzeUniform3i(GLchar *output, GLint outputSize, GLint location, GLint v0, GLint v1, GLint v2)
{
    ANALYZE_BEGIN_EVENT();
    ANALYZE_END_EVENT(GL3_API_glUniform3i);
}

GLvoid CAnalyzer::AnalyzeUniform3iv(GLchar *output, GLint outputSize, GLint location, GLsizei count, const GLint *value)
{
    ANALYZE_BEGIN_EVENT();
    ANALYZE_END_EVENT(GL3_API_glUniform3iv);
}

GLvoid CAnalyzer::AnalyzeUniform4f(GLchar *output, GLint outputSize, GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3)
{
    ANALYZE_BEGIN_EVENT();
    ANALYZE_END_EVENT(GL3_API_glUniform4f);
}

GLvoid CAnalyzer::AnalyzeUniform4fv(GLchar *output, GLint outputSize, GLint location, GLsizei count, const GLfloat *value)
{
    ANALYZE_BEGIN_EVENT();
    ANALYZE_END_EVENT(GL3_API_glUniform4fv);
}

GLvoid CAnalyzer::AnalyzeUniform4i(GLchar *output, GLint outputSize, GLint location, GLint v0, GLint v1, GLint v2, GLint v3)
{
    ANALYZE_BEGIN_EVENT();
    ANALYZE_END_EVENT(GL3_API_glUniform4i);
}

GLvoid CAnalyzer::AnalyzeUniform4iv(GLchar *output, GLint outputSize, GLint location, GLsizei count, const GLint *value)
{
    ANALYZE_BEGIN_EVENT();
    ANALYZE_END_EVENT(GL3_API_glUniform4iv);
}

GLvoid CAnalyzer::AnalyzeUniformMatrix2fv(GLchar *output, GLint outputSize, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value)
{
    ANALYZE_BEGIN_EVENT();
    ANALYZE_END_EVENT(GL3_API_glUniformMatrix2fv);
}

GLvoid CAnalyzer::AnalyzeUniformMatrix3fv(GLchar *output, GLint outputSize, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value)
{
    ANALYZE_BEGIN_EVENT();
    ANALYZE_END_EVENT(GL3_API_glUniformMatrix3fv);
}

GLvoid CAnalyzer::AnalyzeUniformMatrix4fv(GLchar *output, GLint outputSize, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value)
{
    ANALYZE_BEGIN_EVENT();
    ANALYZE_END_EVENT(GL3_API_glUniformMatrix4fv);
}

GLvoid CAnalyzer::AnalyzeGetActiveAttrib(GLchar *output, GLint outputSize, GLuint program, GLuint index, GLsizei bufSize, GLsizei *length, GLint *size, GLenum *type, GLchar *name)
{
    ANALYZE_BEGIN_EVENT();
    ANALYZE_END_EVENT(GL3_API_glGetActiveAttrib);
}

GLvoid CAnalyzer::AnalyzeGetActiveUniform(GLchar *output, GLint outputSize, GLuint program, GLuint index, GLsizei bufSize, GLsizei *length, GLint *size, GLenum *type, GLchar *name)
{
    ANALYZE_BEGIN_EVENT();
    ANALYZE_END_EVENT(GL3_API_glGetActiveUniform);
}

GLvoid CAnalyzer::AnalyzeGetAttachedShaders(GLchar *output, GLint outputSize, GLuint program, GLsizei maxCount, GLsizei *count, GLuint *shaders)
{
    ANALYZE_BEGIN_EVENT();
    ANALYZE_END_EVENT(GL3_API_glGetAttachedShaders);
}

GLvoid CAnalyzer::AnalyzeGetProgramiv(GLchar *output, GLint outputSize, GLuint program, GLenum pname, GLint *params)
{
    ANALYZE_BEGIN_EVENT();
    ANALYZE_END_EVENT(GL3_API_glGetProgramiv);
}

GLvoid CAnalyzer::AnalyzeGetProgramInfoLog(GLchar *output, GLint outputSize, GLuint program, GLsizei bufSize, GLsizei *length, GLchar *infoLog)
{
    ANALYZE_BEGIN_EVENT();
    ANALYZE_END_EVENT(GL3_API_glGetProgramInfoLog);
}

GLvoid CAnalyzer::AnalyzeGetShaderiv(GLchar *output, GLint outputSize, GLuint shader, GLenum pname, GLint *params)
{
    ANALYZE_BEGIN_EVENT();
    ANALYZE_END_EVENT(GL3_API_glGetShaderiv);
}

GLvoid CAnalyzer::AnalyzeGetShaderInfoLog(GLchar *output, GLint outputSize, GLuint shader, GLsizei bufSize, GLsizei *length, GLchar *infoLog)
{
    ANALYZE_BEGIN_EVENT();
    ANALYZE_END_EVENT(GL3_API_glGetShaderInfoLog);
}

GLvoid CAnalyzer::AnalyzeGetShaderPrecisionFormat(GLchar *output, GLint outputSize, GLenum shadertype, GLenum precisiontype, GLint *range, GLint *precision)
{
    ANALYZE_BEGIN_EVENT();
    ANALYZE_END_EVENT(GL3_API_glGetShaderPrecisionFormat);
}

GLvoid CAnalyzer::AnalyzeGetShaderSource(GLchar *output, GLint outputSize, GLuint shader, GLsizei bufSize, GLsizei *length, GLchar *source)
{
    ANALYZE_BEGIN_EVENT();
    ANALYZE_END_EVENT(GL3_API_glGetShaderSource);
}

GLvoid CAnalyzer::AnalyzeGetUniformfv(GLchar *output, GLint outputSize, GLuint program, GLint location, GLfloat *params)
{
    ANALYZE_BEGIN_EVENT();
    ANALYZE_END_EVENT(GL3_API_glGetUniformfv);
}

GLvoid CAnalyzer::AnalyzeGetUniformiv(GLchar *output, GLint outputSize, GLuint program, GLint location, GLint *params)
{
    ANALYZE_BEGIN_EVENT();
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
    ANALYZE_END_EVENT(GL3_API_glValidateProgram);
}

GLvoid CAnalyzer::AnalyzeActiveTexture(GLchar *output, GLint outputSize, GLenum texture)
{
    ANALYZE_BEGIN_EVENT();
    ANALYZE_END_EVENT(GL3_API_glActiveTexture);
}

GLvoid CAnalyzer::AnalyzeDeleteTextures(GLchar *output, GLint outputSize, GLsizei n, const GLuint *textures)
{
    ANALYZE_BEGIN_EVENT();
    GenDelObjects(m_pContext, output, outputSize, n, textures, "glDeleteTextures", "tex");
    ANALYZE_END_EVENT(GL3_API_glDeleteTextures);
}

GLvoid CAnalyzer::AnalyzeBindTexture(GLchar *output, GLint outputSize, GLenum target, GLuint texture)
{
    ANALYZE_BEGIN_EVENT();
    ANALYZE_END_EVENT(GL3_API_glBindTexture);
}

GLvoid CAnalyzer::AnalyzeGenTextures(GLchar *output, GLint outputSize, GLsizei n, GLuint *textures)
{
    ANALYZE_BEGIN_EVENT();
    GenDelObjects(m_pContext, output, outputSize, n, textures, "glGenTextures", "tex");
    ANALYZE_END_EVENT(GL3_API_glGenTextures);
}

GLvoid CAnalyzer::AnalyzeGenerateMipmap(GLchar *output, GLint outputSize, GLenum target)
{
    ANALYZE_BEGIN_EVENT();
    ANALYZE_END_EVENT(GL3_API_glGenerateMipmap);
}

GLvoid CAnalyzer::AnalyzeTexImage2D(GLchar *output, GLint outputSize, GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const void *pixels)
{
    ANALYZE_BEGIN_EVENT();

    CTexObj     *pTexObj    = m_pContext->GetActiveTexObj(target);
    stTexImage  *pTexImg    = pTexObj->GetTexImageByLevel(target, level);

    SaveTextureToFile(target, 0, 0, 0, width, height, width, height, 1, internalformat, type, level, (GLuint)pixels);

    TranslateTexTarget(target, tmp);
    TranslateTexFormat(internalformat, tmp1);
    TranslateDataType(type, tmp2);

    sprintf(output, "glTexImage2D(%s, level=%d, %s, %d, %d, %s, 0x%08X) %s\n",
        tmp, level, tmp1, width, height, tmp2, (GLuint)pixels, pTexImg->texName);

    ANALYZE_END_EVENT(GL3_API_glTexImage2D);
}

GLvoid CAnalyzer::AnalyzeTexSubImage2D(GLchar *output, GLint outputSize, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const void *pixels)
{
    ANALYZE_BEGIN_EVENT();

    CTexObj     *pTexObj    = m_pContext->GetActiveTexObj(target);
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

    CTexObj     *pTexObj    = m_pContext->GetActiveTexObj(target);
    stTexImage  *pTexImg    = pTexObj?pTexObj->GetTexImageByLevel(target, level):NULL;

    if (pixels)
    {
        SaveTextureToFile(target, 0, 0, 0, width, height, width, height, depth, internalformat, type, level, (GLuint)pixels);
    }

    TranslateTexTarget(target, tmp);
    TranslateTexFormat(internalformat, tmp1);
    TranslateDataType(type, tmp2);

    OutputStrcat(output, outputSize, "glTexImage3D(%s, level=%d, %s, %d, %d, depth=%d, %s, 0x%08X) %s\n",
        tmp, level, tmp1, width, height, depth, tmp2, (GLuint)pixels, pTexImg->texName);

    ANALYZE_END_EVENT(GL3_API_glTexImage3D);
}

GLvoid CAnalyzer::AnalyzeTexSubImage3D(GLchar *output, GLint outputSize, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const void *pixels)
{
    ANALYZE_BEGIN_EVENT();

    CTexObj     *pTexObj    = m_pContext->GetActiveTexObj(target);
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
    ANALYZE_END_EVENT(GL3_API_glTexStorage2D);
}

GLvoid CAnalyzer::AnalyzeTexStorage3D(GLchar *output, GLint outputSize, GLenum target, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth)
{
    ANALYZE_BEGIN_EVENT();
    ANALYZE_END_EVENT(GL3_API_glTexStorage3D);
}

GLvoid CAnalyzer::AnalyzeCompressedTexImage2D(GLchar *output, GLint outputSize, GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLsizei imageSize, const void *data)
{

    ANALYZE_BEGIN_EVENT();

    CTexObj     *pTexObj    = m_pContext->GetActiveTexObj(target);
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

    CTexObj     *pTexObj    = m_pContext->GetActiveTexObj(target);
    stTexImage  *pTexImg    = pTexObj->GetTexImageByLevel(target, level);

    SaveCompressedTextureToFile(target, xoffset, yoffset, 0, width, height, width, height, 1, format, GL_UNSIGNED_BYTE, level, (GLuint)data);

    TranslateTexTarget(target, tmp);
    TranslateTexFormat(format, tmp1);
    sprintf(output, "glCompressedTexSubImage2D(%s, level=%d, xoff=%d, yoff=%d, w=%d, h=%d, format=%d, size=0x%08X) %s\n",
        tmp, level, xoffset, yoffset, width, height, tmp1, imageSize, pTexImg->texName);

    ANALYZE_END_EVENT(GL3_API_glCompressedTexSubImage2D);
}

GLvoid CAnalyzer::AnalyzeTexParameterf(GLchar *output, GLint outputSize, GLenum target, GLenum pname, GLfloat param)
{
    ANALYZE_BEGIN_EVENT();
    ANALYZE_END_EVENT(GL3_API_glTexParameterf);
}

GLvoid CAnalyzer::AnalyzeTexParameterfv(GLchar *output, GLint outputSize, GLenum target, GLenum pname, const GLfloat *params)
{
    ANALYZE_BEGIN_EVENT();
    ANALYZE_END_EVENT(GL3_API_glTexParameterfv);
}

GLvoid CAnalyzer::AnalyzeTexParameteri(GLchar *output, GLint outputSize, GLenum target, GLenum pname, GLint param)
{
    ANALYZE_BEGIN_EVENT();
    ANALYZE_END_EVENT(GL3_API_glTexParameteri);
}

GLvoid CAnalyzer::AnalyzeTexParameteriv(GLchar *output, GLint outputSize, GLenum target, GLenum pname, const GLint *params)
{
    ANALYZE_BEGIN_EVENT();
    ANALYZE_END_EVENT(GL3_API_glTexParameteriv);
}

GLvoid CAnalyzer::AnalyzeCopyTexImage2D(GLchar *output, GLint outputSize, GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLsizei height, GLint border)
{
    ANALYZE_BEGIN_EVENT();
    ANALYZE_END_EVENT(GL3_API_glCopyTexImage2D);
}

GLvoid CAnalyzer::AnalyzeCopyTexSubImage2D(GLchar *output, GLint outputSize, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height)
{
    ANALYZE_BEGIN_EVENT();
    ANALYZE_END_EVENT(GL3_API_glCopyTexSubImage2D);
}

GLvoid CAnalyzer::AnalyzeGetTexParameterfv(GLchar *output, GLint outputSize, GLenum target, GLenum pname, GLfloat *params)
{
    ANALYZE_BEGIN_EVENT();
    ANALYZE_END_EVENT(GL3_API_glGetTexParameterfv);
}

GLvoid CAnalyzer::AnalyzeGetTexParameteriv(GLchar *output, GLint outputSize, GLenum target, GLenum pname, GLint *params)
{
    ANALYZE_BEGIN_EVENT();
    ANALYZE_END_EVENT(GL3_API_glGetTexParameteriv);
}

GLvoid CAnalyzer::AnalyzeBindFramebuffer(GLchar *output, GLint outputSize, GLenum target, GLuint framebuffer)
{
    ANALYZE_BEGIN_EVENT();
    ANALYZE_END_EVENT(GL3_API_glBindFramebuffer);
}

GLvoid CAnalyzer::AnalyzeBindRenderbuffer(GLchar *output, GLint outputSize, GLenum target, GLuint renderbuffer)
{
    ANALYZE_BEGIN_EVENT();
    ANALYZE_END_EVENT(GL3_API_glBindRenderbuffer);
}

GLvoid CAnalyzer::AnalyzeDeleteFramebuffers(GLchar *output, GLint outputSize, GLsizei n, const GLuint *framebuffers)
{
    ANALYZE_BEGIN_EVENT();
    GenDelObjects(m_pContext, output, outputSize, n, framebuffers, "glDeleteFramebuffers", "fbo");
    ANALYZE_END_EVENT(GL3_API_glDeleteFramebuffers);
}

GLvoid CAnalyzer::AnalyzeDeleteRenderbuffers(GLchar *output, GLint outputSize, GLsizei n, const GLuint *renderbuffers)
{
    ANALYZE_BEGIN_EVENT();
    GenDelObjects(m_pContext, output, outputSize, n, renderbuffers, "glDeleteRenderbuffers", "rbo");
    ANALYZE_END_EVENT(GL3_API_glDeleteRenderbuffers);
}

GLvoid CAnalyzer::AnalyzeGenFramebuffers(GLchar *output, GLint outputSize, GLsizei n, GLuint *framebuffers)
{
    ANALYZE_BEGIN_EVENT();
    GenDelObjects(m_pContext, output, outputSize, n, framebuffers, "glGenFramebuffers", "fbo");
    ANALYZE_END_EVENT(GL3_API_glGenFramebuffers);
}

GLvoid CAnalyzer::AnalyzeGenRenderbuffers(GLchar *output, GLint outputSize, GLsizei n, GLuint *renderbuffers)
{
    ANALYZE_BEGIN_EVENT();
    GenDelObjects(m_pContext, output, outputSize, n, renderbuffers, "glGenRenderbuffers", "rbo");
    ANALYZE_END_EVENT(GL3_API_glGenRenderbuffers);
}

GLvoid CAnalyzer::AnalyzeRenderbufferStorage(GLchar *output, GLint outputSize, GLenum target, GLenum internalformat, GLsizei width, GLsizei height)
{
    ANALYZE_BEGIN_EVENT();
    ANALYZE_END_EVENT(GL3_API_glRenderbufferStorage);
}

GLvoid CAnalyzer::AnalyzeFramebufferRenderbuffer(GLchar *output, GLint outputSize, GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer)
{
    ANALYZE_BEGIN_EVENT();
    ANALYZE_END_EVENT(GL3_API_glFramebufferRenderbuffer);
}

GLvoid CAnalyzer::AnalyzeFramebufferTexture2D(GLchar *output, GLint outputSize, GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level)
{
    ANALYZE_BEGIN_EVENT();
    ANALYZE_END_EVENT(GL3_API_glFramebufferTexture2D);
}

GLvoid CAnalyzer::AnalyzeCheckFramebufferStatus(GLchar *output, GLint outputSize, GLenum target, GLenum result)
{
    ANALYZE_BEGIN_EVENT();
    ANALYZE_END_EVENT(GL3_API_glCheckFramebufferStatus);
}

GLvoid CAnalyzer::AnalyzeGetFramebufferAttachmentParameteriv(GLchar *output, GLint outputSize, GLenum target, GLenum attachment, GLenum pname, GLint *params)
{
    ANALYZE_BEGIN_EVENT();
    ANALYZE_END_EVENT(GL3_API_glGetFramebufferAttachmentParameteriv);
}

GLvoid CAnalyzer::AnalyzeGetRenderbufferParameteriv(GLchar *output, GLint outputSize, GLenum target, GLenum pname, GLint *params)
{
    ANALYZE_BEGIN_EVENT();
    ANALYZE_END_EVENT(GL3_API_glGetRenderbufferParameteriv);
}

GLvoid CAnalyzer::AnalyzeDisable(GLchar *output, GLint outputSize, GLenum cap)
{
    ANALYZE_BEGIN_EVENT();
    ANALYZE_END_EVENT(GL3_API_glDisable);
}

GLvoid CAnalyzer::AnalyzeEnable(GLchar *output, GLint outputSize, GLenum cap)
{
    ANALYZE_BEGIN_EVENT();
    ANALYZE_END_EVENT(GL3_API_glEnable);
}

GLvoid CAnalyzer::AnalyzeFrontFace(GLchar *output, GLint outputSize, GLenum mode)
{
    ANALYZE_BEGIN_EVENT();
    ANALYZE_END_EVENT(GL3_API_glFrontFace);
}

GLvoid CAnalyzer::AnalyzeCullFace(GLchar *output, GLint outputSize, GLenum mode)
{
    ANALYZE_BEGIN_EVENT();
    ANALYZE_END_EVENT(GL3_API_glCullFace);
}

GLvoid CAnalyzer::AnalyzeBlendColor(GLchar *output, GLint outputSize, GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha)
{
    ANALYZE_BEGIN_EVENT();
    ANALYZE_END_EVENT(GL3_API_glBlendColor);
}

GLvoid CAnalyzer::AnalyzeBlendEquation(GLchar *output, GLint outputSize, GLenum mode)
{
    ANALYZE_BEGIN_EVENT();
    ANALYZE_END_EVENT(GL3_API_glBlendEquation);
}

GLvoid CAnalyzer::AnalyzeBlendEquationSeparate(GLchar *output, GLint outputSize, GLenum modeRGB, GLenum modeAlpha)
{
    ANALYZE_BEGIN_EVENT();
    ANALYZE_END_EVENT(GL3_API_glBlendEquationSeparate);
}

GLvoid CAnalyzer::AnalyzeBlendFunc(GLchar *output, GLint outputSize, GLenum sfactor, GLenum dfactor)
{
    ANALYZE_BEGIN_EVENT();
    ANALYZE_END_EVENT(GL3_API_glBlendFunc);
}

GLvoid CAnalyzer::AnalyzeBlendFuncSeparate(GLchar *output, GLint outputSize, GLenum sfactorRGB, GLenum dfactorRGB, GLenum sfactorAlpha, GLenum dfactorAlpha)
{
    ANALYZE_BEGIN_EVENT();
    ANALYZE_END_EVENT(GL3_API_glBlendFuncSeparate);
}

GLvoid CAnalyzer::AnalyzeDepthFunc(GLchar *output, GLint outputSize, GLenum func)
{
    ANALYZE_BEGIN_EVENT();
    ANALYZE_END_EVENT(GL3_API_glDepthFunc);
}

GLvoid CAnalyzer::AnalyzeDepthRangef(GLchar *output, GLint outputSize, GLfloat n, GLfloat f)
{
    ANALYZE_BEGIN_EVENT();
    ANALYZE_END_EVENT(GL3_API_glDepthRangef);
}

GLvoid CAnalyzer::AnalyzeClearColor(GLchar *output, GLint outputSize, GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha)
{
    ANALYZE_BEGIN_EVENT();
    ANALYZE_END_EVENT(GL3_API_glClearColor);
}

GLvoid CAnalyzer::AnalyzeClearDepthf(GLchar *output, GLint outputSize, GLfloat d)
{
    ANALYZE_BEGIN_EVENT();
    ANALYZE_END_EVENT(GL3_API_glClearDepthf);
}

GLvoid CAnalyzer::AnalyzeClearStencil(GLchar *output, GLint outputSize, GLint s)
{
    ANALYZE_BEGIN_EVENT();
    ANALYZE_END_EVENT(GL3_API_glClearStencil);
}

GLvoid CAnalyzer::AnalyzeColorMask(GLchar *output, GLint outputSize, GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha)
{
    ANALYZE_BEGIN_EVENT();
    ANALYZE_END_EVENT(GL3_API_glColorMask);
}

GLvoid CAnalyzer::AnalyzeDepthMask(GLchar *output, GLint outputSize, GLboolean flag)
{
    ANALYZE_BEGIN_EVENT();
    ANALYZE_END_EVENT(GL3_API_glDepthMask);
}

GLvoid CAnalyzer::AnalyzeLineWidth(GLchar *output, GLint outputSize, GLfloat width)
{
    ANALYZE_BEGIN_EVENT();
    ANALYZE_END_EVENT(GL3_API_glLineWidth);
}

GLvoid CAnalyzer::AnalyzePixelStorei(GLchar *output, GLint outputSize, GLenum pname, GLint param)
{
    ANALYZE_BEGIN_EVENT();
    ANALYZE_END_EVENT(GL3_API_glPixelStorei);
}

GLvoid CAnalyzer::AnalyzePolygonOffset(GLchar *output, GLint outputSize, GLfloat factor, GLfloat units)
{
    ANALYZE_BEGIN_EVENT();
    ANALYZE_END_EVENT(GL3_API_glPolygonOffset);
}

GLvoid CAnalyzer::AnalyzeScissor(GLchar *output, GLint outputSize, GLint x, GLint y, GLsizei width, GLsizei height)
{
    ANALYZE_BEGIN_EVENT();
    ANALYZE_END_EVENT(GL3_API_glScissor);
}

GLvoid CAnalyzer::AnalyzeStencilFunc(GLchar *output, GLint outputSize, GLenum func, GLint ref, GLuint mask)
{
    ANALYZE_BEGIN_EVENT();
    ANALYZE_END_EVENT(GL3_API_glStencilFunc);
}

GLvoid CAnalyzer::AnalyzeStencilFuncSeparate(GLchar *output, GLint outputSize, GLenum face, GLenum func, GLint ref, GLuint mask)
{
    ANALYZE_BEGIN_EVENT();
    ANALYZE_END_EVENT(GL3_API_glStencilFuncSeparate);
}

GLvoid CAnalyzer::AnalyzeStencilMask(GLchar *output, GLint outputSize, GLuint mask)
{
    ANALYZE_BEGIN_EVENT();
    ANALYZE_END_EVENT(GL3_API_glStencilMask);
}

GLvoid CAnalyzer::AnalyzeStencilMaskSeparate(GLchar *output, GLint outputSize, GLenum face, GLuint mask)
{
    ANALYZE_BEGIN_EVENT();
    ANALYZE_END_EVENT(GL3_API_glStencilMaskSeparate);
}

GLvoid CAnalyzer::AnalyzeStencilOp(GLchar *output, GLint outputSize, GLenum fail, GLenum zfail, GLenum zpass)
{
    ANALYZE_BEGIN_EVENT();
    ANALYZE_END_EVENT(GL3_API_glStencilOp);
}

GLvoid CAnalyzer::AnalyzeStencilOpSeparate(GLchar *output, GLint outputSize, GLenum face, GLenum sfail, GLenum dpfail, GLenum dppass)
{
    ANALYZE_BEGIN_EVENT();
    ANALYZE_END_EVENT(GL3_API_glStencilOpSeparate);
}

GLvoid CAnalyzer::AnalyzeSampleCoverage(GLchar *output, GLint outputSize, GLfloat value, GLboolean invert)
{
    ANALYZE_BEGIN_EVENT();
    ANALYZE_END_EVENT(GL3_API_glSampleCoverage);
}

GLvoid CAnalyzer::AnalyzeViewport(GLchar *output, GLint outputSize, GLint x, GLint y, GLsizei width, GLsizei height)
{
    ANALYZE_BEGIN_EVENT();
    ANALYZE_END_EVENT(GL3_API_glViewport);
}

GLvoid CAnalyzer::AnalyzeGetBooleanv(GLchar *output, GLint outputSize, GLenum pname, GLboolean *data)
{
    ANALYZE_BEGIN_EVENT();
    ANALYZE_END_EVENT(GL3_API_glGetBooleanv);
}

GLvoid CAnalyzer::AnalyzeGetFloatv(GLchar *output, GLint outputSize, GLenum pname, GLfloat *data)
{
    ANALYZE_BEGIN_EVENT();
    ANALYZE_END_EVENT(GL3_API_glGetFloatv);
}

GLvoid CAnalyzer::AnalyzeGetIntegerv(GLchar *output, GLint outputSize, GLenum pname, GLint *data)
{
    ANALYZE_BEGIN_EVENT();
    ANALYZE_END_EVENT(GL3_API_glGetIntegerv);
}

GLvoid CAnalyzer::AnalyzeGetString(GLchar *output, GLint outputSize, GLenum name, const GLubyte *str)
{
    ANALYZE_BEGIN_EVENT();
    ANALYZE_END_EVENT(GL3_API_glGetString);
}

GLvoid CAnalyzer::AnalyzeIsBuffer(GLchar *output, GLint outputSize, GLuint buffer, GLboolean res)
{
    ANALYZE_BEGIN_EVENT();
    ANALYZE_END_EVENT(GL3_API_glIsBuffer);
}

GLvoid CAnalyzer::AnalyzeIsEnabled(GLchar *output, GLint outputSize, GLenum cap, GLboolean res)
{
    ANALYZE_BEGIN_EVENT();
    ANALYZE_END_EVENT(GL3_API_glIsEnabled);
}

GLvoid CAnalyzer::AnalyzeIsFramebuffer(GLchar *output, GLint outputSize, GLuint framebuffer, GLboolean res)
{
    ANALYZE_BEGIN_EVENT();
    ANALYZE_END_EVENT(GL3_API_glIsFramebuffer);
}

GLvoid CAnalyzer::AnalyzeIsProgram(GLchar *output, GLint outputSize, GLuint program, GLboolean res)
{
    ANALYZE_BEGIN_EVENT();
    ANALYZE_END_EVENT(GL3_API_glIsProgram);
}

GLvoid CAnalyzer::AnalyzeIsRenderbuffer(GLchar *output, GLint outputSize, GLuint renderbuffer, GLboolean res)
{
    ANALYZE_BEGIN_EVENT();
    ANALYZE_END_EVENT(GL3_API_glIsRenderbuffer);
}

GLvoid CAnalyzer::AnalyzeIsShader(GLchar *output, GLint outputSize, GLuint shader, GLboolean res)
{
    ANALYZE_BEGIN_EVENT();
    ANALYZE_END_EVENT(GL3_API_glIsShader);
}

GLvoid CAnalyzer::AnalyzeIsTexture(GLchar *output, GLint outputSize, GLuint texture, GLboolean res)
{
    ANALYZE_BEGIN_EVENT();
    ANALYZE_END_EVENT(GL3_API_glIsTexture);
}

GLvoid CAnalyzer::AnalyzeGetError(GLchar *output, GLint outputSize, GLenum err)
{
    ANALYZE_BEGIN_EVENT();
    ANALYZE_END_EVENT(GL3_API_glGetError);
}

GLvoid CAnalyzer::AnalyzeFinish(GLchar *output, GLint outputSize)
{
    ANALYZE_BEGIN_EVENT();
    ANALYZE_END_EVENT(GL3_API_glFinish);
}

GLvoid CAnalyzer::AnalyzeFlush(GLchar *output, GLint outputSize)
{
    ANALYZE_BEGIN_EVENT();
    ANALYZE_END_EVENT(GL3_API_glFlush);
}

GLvoid CAnalyzer::AnalyzeHint(GLchar *output, GLint outputSize, GLenum target, GLenum mode)
{
    ANALYZE_BEGIN_EVENT();
    ANALYZE_END_EVENT(GL3_API_glHint);
}

GLvoid CAnalyzer::AnalyzeReadPixels(GLchar *output, GLint outputSize, GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, void *pixels)
{
    ANALYZE_BEGIN_EVENT();
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
    ANALYZE_END_EVENT(GL3_API_glClear);
}

GLvoid CAnalyzer::AnalyzeDrawArrays(GLchar *output, GLint outputSize, GLenum mode, GLint first, GLsizei count)
{
    ANALYZE_BEGIN_EVENT();
    ANALYZE_END_EVENT(GL3_API_glDrawArrays);
}

GLvoid CAnalyzer::AnalyzeDrawElements(GLchar *output, GLint outputSize, GLenum mode, GLsizei count, GLenum type, const GLvoid* indices)
{
    ANALYZE_BEGIN_EVENT();
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
    ANALYZE_END_EVENT(EGL_API_eglChooseConfig);
}

void CAnalyzer::AnalyzeCopyBuffers(GLchar *output, GLint outputSize, EGLDisplay dpy, EGLSurface surface, EGLNativePixmapType target, EGLBoolean res)
{
    ANALYZE_BEGIN_EVENT();
    ANALYZE_END_EVENT(EGL_API_eglCopyBuffers);
}

void CAnalyzer::AnalyzeCreateContext(GLchar *output, GLint outputSize, EGLDisplay dpy, EGLConfig config, EGLContext share_context, const EGLint *attrib_list, EGLContext res)
{
    ANALYZE_BEGIN_EVENT();

    CEGLContext *p = NULL;

    p           = eglGetFreeEGLContext(egl_context.eglContext, &egl_context.eglContextCount);
    p->bUsed    = EGL_TRUE;
    p->addr     = (EGLint)res;
    memset(p->name, 0, 32);
    sprintf(p->name, "EGLContext%d", ++eglContextCount);

    sprintf(output, "eglCreateContext(display = 0x%08X, config = 0x%08X, SharedContext = 0x%08X, attrib_list = 0x%08X) %s\n",
            arg[0], arg[1], arg[2], arg[3], p?p->name:"null");

    if (pAttr)
    {
        while (*pAttr != EGL_NONE)
        {
            TranslateEGLAttrib(*pAttr, tmp5);
            OutputStrcat(output, outputSize, "    %s: 0x%04X\n", tmp5, *++pAttr);
            pAttr ++;
        }
    }

    ANALYZE_END_EVENT(EGL_API_eglCreateContext);
}

void CAnalyzer::AnalyzeCreatePbufferSurface(GLchar *output, GLint outputSize, EGLDisplay dpy, EGLConfig config, const EGLint *attrib_list, EGLSurface res)
{
    ANALYZE_BEGIN_EVENT();
    ANALYZE_END_EVENT(EGL_API_eglCreatePbufferSurface);
}

void CAnalyzer::AnalyzeCreatePixmapSurface(GLchar *output, GLint outputSize, EGLDisplay dpy, EGLConfig config, EGLNativePixmapType pixmap, const EGLint *attrib_list, EGLSurface res)
{
    ANALYZE_BEGIN_EVENT();
    ANALYZE_END_EVENT(EGL_API_eglCreatePixmapSurface);
}

void CAnalyzer::AnalyzeCreateWindowSurface(GLchar *output, GLint outputSize, EGLDisplay dpy, EGLConfig config, EGLNativeWindowType win, const EGLint *attrib_list, EGLSurface res)
{
    ANALYZE_BEGIN_EVENT();
    ANALYZE_END_EVENT(EGL_API_eglCreateWindowSurface);
}

void CAnalyzer::AnalyzeDestroyContext(GLchar *output, GLint outputSize, EGLDisplay dpy, EGLContext ctx, EGLBoolean res)
{
    ANALYZE_BEGIN_EVENT();
    ANALYZE_END_EVENT(EGL_API_eglDestroyContext);
}

void CAnalyzer::AnalyzeDestroySurface(GLchar *output, GLint outputSize, EGLDisplay dpy, EGLSurface surface, EGLBoolean res)
{
    ANALYZE_BEGIN_EVENT();
    ANALYZE_END_EVENT(EGL_API_eglDestroySurface);
}

void CAnalyzer::AnalyzeGetConfigAttrib(GLchar *output, GLint outputSize, EGLDisplay dpy, EGLConfig config, EGLint attribute, EGLint *value, EGLBoolean res)
{
    ANALYZE_BEGIN_EVENT();
    ANALYZE_END_EVENT(EGL_API_eglGetConfigAttrib);
}

void CAnalyzer::AnalyzeGetConfigs(GLchar *output, GLint outputSize, EGLDisplay dpy, EGLConfig *configs, EGLint config_size, EGLint *num_config, EGLBoolean res)
{
    ANALYZE_BEGIN_EVENT();
    ANALYZE_END_EVENT(EGL_API_eglGetConfigs);
}

void CAnalyzer::AnalyzeGetCurrentDisplay(GLchar *output, GLint outputSize, EGLDisplay res)
{
    ANALYZE_BEGIN_EVENT();
    ANALYZE_END_EVENT(EGL_API_eglGetCurrentDisplay);
}

void CAnalyzer::AnalyzeGetCurrentSurface(GLchar *output, GLint outputSize, EGLint readdraw, EGLSurface res)
{
    ANALYZE_BEGIN_EVENT();
    ANALYZE_END_EVENT(EGL_API_eglGetCurrentSurface);
}

void CAnalyzer::AnalyzeGetDisplay(GLchar *output, GLint outputSize, EGLNativeDisplayType display_id, EGLDisplay res)
{
    ANALYZE_BEGIN_EVENT();
    ANALYZE_END_EVENT(EGL_API_eglGetDisplay);
}

void CAnalyzer::eglGetError(GLchar *output, GLint outputSize, EGLint err)
{
    ANALYZE_BEGIN_EVENT();
    ANALYZE_END_EVENT(EGL_API_eglGetError);
}

void CAnalyzer::AnalyzeSetFIFO(GLchar *output, GLint outputSize, void* fifo, EGLBoolean res)
{
    //ANALYZE_BEGIN_EVENT();
    //ANALYZE_END_EVENT(EGL_API_eglSetFIFO);
}

void CAnalyzer::AnalyzeGetProcAddress(GLchar *output, GLint outputSize, const char *procname)
{
    ANALYZE_BEGIN_EVENT();
    ANALYZE_END_EVENT(EGL_API_eglGetProcAddress);
}

void CAnalyzer::AnalyzeInitialize(GLchar *output, GLint outputSize, EGLDisplay dpy, EGLint *major, EGLint *minor, EGLBoolean res)
{
    ANALYZE_BEGIN_EVENT();
    ANALYZE_END_EVENT(EGL_API_eglInitialize);
}

void CAnalyzer::AnalyzeMakeCurrent(GLchar *output, GLint outputSize, EGLDisplay dpy, EGLSurface draw, EGLSurface read, EGLContext ctx, EGLBoolean res)
{
    ANALYZE_BEGIN_EVENT();
    ANALYZE_END_EVENT(EGL_API_eglMakeCurrent);
}

void CAnalyzer::AnalyzeQueryContext(GLchar *output, GLint outputSize, EGLDisplay dpy, EGLContext ctx, EGLint attribute, EGLint *value, EGLBoolean res)
{
    ANALYZE_BEGIN_EVENT();
    ANALYZE_END_EVENT(EGL_API_eglQueryContext);
}

void CAnalyzer::AnalyzeQueryString(GLchar *output, GLint outputSize, EGLDisplay dpy, EGLint name, const char* res)
{
    ANALYZE_BEGIN_EVENT();
    ANALYZE_END_EVENT(EGL_API_eglQueryString);
}

void CAnalyzer::AnalyzeQuerySurface(GLchar *output, GLint outputSize, EGLDisplay dpy, EGLSurface surface, EGLint attribute, EGLint *value, EGLBoolean res)
{
    ANALYZE_BEGIN_EVENT();
    ANALYZE_END_EVENT(EGL_API_eglQuerySurface);
}

void CAnalyzer::AnalyzeSwapBuffers(GLchar *output, GLint outputSize, EGLDisplay dpy, EGLSurface surface, EGLBoolean res)
{
    ANALYZE_BEGIN_EVENT();
    ANALYZE_END_EVENT(EGL_API_eglSwapBuffers);
}

void CAnalyzer::AnalyzeTerminate(GLchar *output, GLint outputSize, EGLDisplay dpy, EGLBoolean res)
{
    ANALYZE_BEGIN_EVENT();
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
    ANALYZE_END_EVENT(EGL_API_eglReleaseTexImage);
}

void CAnalyzer::AnalyzeSurfaceAttrib(GLchar *output, GLint outputSize, EGLDisplay dpy, EGLSurface surface, EGLint attribute, EGLint value, EGLBoolean res)
{
    ANALYZE_BEGIN_EVENT();
    ANALYZE_END_EVENT(EGL_API_eglSurfaceAttrib);
}

void CAnalyzer::AnalyzeSwapInterval(GLchar *output, GLint outputSize, EGLDisplay dpy, EGLint interval, EGLBoolean res)
{
    ANALYZE_BEGIN_EVENT();
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
    ANALYZE_END_EVENT(EGL_API_eglQueryAPI);
}

void CAnalyzer::AnalyzeCreatePbufferFromClientBuffer(GLchar *output, GLint outputSize, EGLDisplay dpy, EGLenum buftype, EGLClientBuffer buffer, EGLConfig config, const EGLint *attrib_list, EGLSurface res)
{
    ANALYZE_BEGIN_EVENT();
    ANALYZE_END_EVENT(EGL_API_eglCreatePbufferFromClientBuffer);
}

void CAnalyzer::AnalyzeReleaseThread(GLchar *output, GLint outputSize, GLBoolean res)
{
    ANALYZE_BEGIN_EVENT();
    ANALYZE_END_EVENT(EGL_API_eglReleaseThread);
}

void CAnalyzer::AnalyzeWaitClient(GLchar *output, GLint outputSize, EGLBoolean res)
{
    ANALYZE_BEGIN_EVENT();
    ANALYZE_END_EVENT(EGL_API_eglWaitClient);
}

void CAnalyzer::AnalyzeGetCurrentContext(GLchar *output, GLint outputSize, EGLContext res)
{
    ANALYZE_BEGIN_EVENT();
    ANALYZE_END_EVENT(EGL_API_eglGetCurrentContext);
}
