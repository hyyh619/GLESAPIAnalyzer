#include "ES30API.h"
#include "ApiGLES3Context.h"

#ifndef API_DUMP

const stEngine  *g_opengl   = NULL;
GLint           threadValue = TLS_OUT_OF_INDEXES;

CGLES3Context*
_GetThreadDataES3(
               void
               )
{
    return (CGLES3Context*)TlsGetValue(threadValue);
}

GLvoid*
GLES3CreateContext(
                   const stEngine * Eng
                   )
{
    CGLES3Context   *p = NULL;

    p = new CGLES3Context();

    return p;
}

GLvoid
GLES3DestroyContext(
                    void *context
                    )
{
    CGLES3Context   *p = (CGLES3Context*)context;
    delete p;
    return;
}

GLint
GLES3SetContext(
                void *context,
                const stEngine *Eng
                )
{
    if (g_opengl == NULL)
    {
        g_opengl = Eng;
    }

    if (threadValue == TLS_OUT_OF_INDEXES)
    {
        threadValue = TlsAlloc();
        if (threadValue == TLS_OUT_OF_INDEXES)
        {
            return GL_FALSE;
        }
    }

    TlsSetValue(threadValue, context);

    return GL_TRUE;
}

void apiERROR(GLint e)
{
    CGLES3Context* thread = _GetThreadDataES3();

    if (thread != NULL)
    {
        if (e != GL_NO_ERROR)
        {
            //DebugBreak();
        }
    }
}

/*-------------------------------------------------------------------------
* Entrypoint definitions
*-----------------------------------------------------------------------*/

/* OpenGL ES 2.0 */

GL_APICALL void GL_APIENTRY
glActiveTexture(
                GLenum texture
                )
{
    if (texture < GL_TEXTURE0 ||
        texture > GL_TEXTURE31)
    {
        apiERROR(GL_INVALID_ENUM);
        return;
    }

    g_opengl->glActiveTexture(texture);
    CURRENT_GL_CONTEXT()->ApiActiveTexture(texture);
}

GL_APICALL void GL_APIENTRY
glAttachShader(
               GLuint program,
               GLuint shader
               )
{
    g_opengl->glAttachShader(program, shader);
    CURRENT_GL_CONTEXT()->ApiAttachShader(program, shader);
}

GL_APICALL void GL_APIENTRY
glBindAttribLocation (
                      GLuint program,
                      GLuint index,
                      const GLchar* name
                      )
{
    g_opengl->glBindAttribLocation(program, index, name);
    CURRENT_GL_CONTEXT()->ApiBindAttribLocation(program, index, name);
}

GL_APICALL void GL_APIENTRY
glBindBuffer(
             GLenum target,
             GLuint buffer
             )
{
    g_opengl->glBindBuffer(target, buffer);
    CURRENT_GL_CONTEXT()->ApiBindBuffer(target, buffer);
}

GL_APICALL void GL_APIENTRY
glBindFramebuffer(
                  GLenum target,
                  GLuint framebuffer
                  )
{
    g_opengl->glBindFramebuffer(target, framebuffer);
    CURRENT_GL_CONTEXT()->ApiBindFramebuffer(target, framebuffer);
}

GL_APICALL void GL_APIENTRY
glBindRenderbuffer(
                   GLenum target,
                   GLuint renderbuffer
                   )
{
    g_opengl->glBindRenderbuffer(target, renderbuffer);
    CURRENT_GL_CONTEXT()->ApiBindRenderbuffer(target, renderbuffer);
}

GL_APICALL void GL_APIENTRY
glBindTexture(
              GLenum target,
              GLuint texture
              )
{
    g_opengl->glBindTexture(target, texture);
    CURRENT_GL_CONTEXT()->ApiBindTexture(target, texture);
}

GL_APICALL void GL_APIENTRY
glBindVertexArrayOES (
                      GLuint array
                      )
{
    g_opengl->glBindVertexArray(array);
    CURRENT_GL_CONTEXT()->ApiBindVertexArray(array);
}

GL_APICALL void GL_APIENTRY
glBlendColor(
             GLfloat red,
             GLfloat green,
             GLfloat blue,
             GLfloat alpha
             )
{
    g_opengl->glBlendColor(red, green, blue, alpha);
    CURRENT_GL_CONTEXT()->ApiBlendColor(red, green, blue, alpha);
}

GL_APICALL void GL_APIENTRY
glBlendEquation(
                GLenum mode
                )
{
    g_opengl->glBlendEquation(mode);
    CURRENT_GL_CONTEXT()->ApiBlendEquation(mode);
}

GL_APICALL void GL_APIENTRY
glBlendEquationSeparate(
                        GLenum modeRGB,
                        GLenum modeAlpha
                        )
{
    g_opengl->glBlendEquationSeparate(modeRGB, modeAlpha);
    CURRENT_GL_CONTEXT()->ApiBlendEquationSeparate(modeRGB, modeAlpha);
}

GL_APICALL void GL_APIENTRY
glBlendFunc(
            GLenum sfactor,
            GLenum dfactor
            )
{
    g_opengl->glBlendFunc(sfactor, dfactor);
    CURRENT_GL_CONTEXT()->ApiBlendFunc(sfactor, dfactor);
}

GL_APICALL void GL_APIENTRY
glBlendFuncSeparate(
                    GLenum srcRGB,
                    GLenum dstRGB,
                    GLenum srcAlpha,
                    GLenum dstAlpha
                    )
{
    g_opengl->glBlendFuncSeparate(
        srcRGB,
        dstRGB,
        srcAlpha,
        dstAlpha
        );
    CURRENT_GL_CONTEXT()->ApiBlendFuncSeparate(srcRGB, dstRGB, srcAlpha, dstAlpha);
}

GL_APICALL void GL_APIENTRY
glBufferData(
             GLenum target,
             GLsizeiptr size,
             const GLvoid* data,
             GLenum usage
             )
{
    g_opengl->glBufferData(target, size, data, usage);
    CURRENT_GL_CONTEXT()->ApiBufferData(target, size, data, usage);
}

GL_APICALL void GL_APIENTRY
glBufferSubData(
                GLenum target,
                GLintptr offset,
                GLsizeiptr size,
                const GLvoid* data
                )
{
    g_opengl->glBufferSubData(target, offset, size, data);
    CURRENT_GL_CONTEXT()->ApiBufferSubData(target, offset, size, data);
}

GL_APICALL GLenum GL_APIENTRY
glCheckFramebufferStatus(
                         GLenum target
                         )
{
    GLenum en = g_opengl->glCheckFramebufferStatus(target);
    CURRENT_GL_CONTEXT()->ApiCheckFramebufferStatus(target, en);
    return en;
}

GL_APICALL void GL_APIENTRY
glClear(
        GLbitfield mask
        )
{
    g_opengl->glClear(mask);
    CURRENT_GL_CONTEXT()->ApiClear(mask);
}

GL_APICALL void GL_APIENTRY
glClearColor(
             GLfloat red,
             GLfloat green,
             GLfloat blue,
             GLfloat alpha
             )
{

    g_opengl->glClearColor(red, green, blue, alpha);
    CURRENT_GL_CONTEXT()->ApiClearColor(red, green, blue, alpha);
}

GL_APICALL void GL_APIENTRY
glClearDepthf(
              GLfloat depth
              )
{
    g_opengl->glClearDepthf(depth);
    CURRENT_GL_CONTEXT()->ApiClearDepthf(depth);
}

GL_APICALL void GL_APIENTRY
glClearStencil(
               GLint s
               )
{
    g_opengl->glClearStencil(s);
    CURRENT_GL_CONTEXT()->ApiClearStencil(s);
}

GL_APICALL void GL_APIENTRY
glColorMask(
            GLboolean red,
            GLboolean green,
            GLboolean blue,
            GLboolean alpha
            )
{
    g_opengl->glColorMask(red, green, blue, alpha);
    CURRENT_GL_CONTEXT()->ApiColorMask(red, green, blue, alpha);
}

GL_APICALL void GL_APIENTRY
glCompileShader(
                GLuint shader
                )
{
    g_opengl->glCompileShader(shader);
    CURRENT_GL_CONTEXT()->ApiCompileShader(shader);
}

GL_APICALL void GL_APIENTRY
glCompressedTexImage2D(
                       GLenum target,
                       GLint level,
                       GLenum internalformat,
                       GLsizei width,
                       GLsizei height,
                       GLint border,
                       GLsizei imageSize,
                       const GLvoid* data
                       )
{
    g_opengl->glCompressedTexImage2D(
        target,
        level,
        internalformat,
        width, height,
        border,
        imageSize,
        data
        );
    CURRENT_GL_CONTEXT()->ApiCompressedTexImage2D(target,
                                                  level,
                                                  internalformat,
                                                  width,
                                                  height,
                                                  border,
                                                  imageSize,
                                                  data);
}

GL_APICALL void GL_APIENTRY
glCompressedTexSubImage2D(
                          GLenum target,
                          GLint level,
                          GLint xoffset,
                          GLint yoffset,
                          GLsizei width,
                          GLsizei height,
                          GLenum format,
                          GLsizei imageSize,
                          const GLvoid* data
                          )
{
    g_opengl->glCompressedTexSubImage2D(target, level, xoffset, yoffset, width, height, format, imageSize, data);
    CURRENT_GL_CONTEXT()->ApiCompressedTexSubImage2D(target, level, xoffset, yoffset, width, height, format, imageSize, data);
}

GL_APICALL void GL_APIENTRY
glCopyTexImage2D(
                 GLenum target,
                 GLint level,
                 GLenum internalformat,
                 GLint x,
                 GLint y,
                 GLsizei width,
                 GLsizei height,
                 GLint border
                 )
{
    g_opengl->glCopyTexImage2D(
        target,
        level,
        internalformat,
        x, y,
        width, height,
        border
        );

    CURRENT_GL_CONTEXT()->ApiCopyTexImage2D(target,
                                            level,
                                            internalformat,
                                            x, y,
                                            width, height,
                                            border);
}

GL_APICALL void GL_APIENTRY
glCopyTexSubImage2D(
                    GLenum target,
                    GLint level,
                    GLint xoffset,
                    GLint yoffset,
                    GLint x,
                    GLint y,
                    GLsizei width,
                    GLsizei height
                    )
{
    g_opengl->glCopyTexSubImage2D(
        target,
        level,
        xoffset, yoffset,
        x, y,
        width, height
        );

    CURRENT_GL_CONTEXT()->ApiCopyTexSubImage2D(target,
                                               level,
                                               xoffset, yoffset,
                                               x, y,
                                               width, height);
}

GL_APICALL GLuint GL_APIENTRY
glCreateProgram(
                void
                )
{
    GLuint ret = g_opengl->glCreateProgram();
    CURRENT_GL_CONTEXT()->ApiCreateProgram(ret);
    return ret;
}

GL_APICALL GLuint GL_APIENTRY
glCreateShader(
               GLenum type
               )
{
    GLuint ret = g_opengl->glCreateShader(type);
    CURRENT_GL_CONTEXT()->ApiCreateShader(type, ret);
    return ret;
}

GL_APICALL void GL_APIENTRY
glCullFace(
           GLenum mode
           )
{
    g_opengl->glCullFace(mode);
    CURRENT_GL_CONTEXT()->ApiCullFace(mode);
}

GL_APICALL void GL_APIENTRY
glDeleteBuffers(
                GLsizei n,
                const GLuint* buffers
                )
{
    g_opengl->glDeleteBuffers(n, buffers);
    CURRENT_GL_CONTEXT()->ApiDeleteBuffers(n, buffers);
}

GL_APICALL void GL_APIENTRY
glDeleteFramebuffers(
                     GLsizei n,
                     const GLuint* framebuffers
                     )
{
    g_opengl->glDeleteFramebuffers(n, framebuffers);
    CURRENT_GL_CONTEXT()->ApiDeleteFramebuffers(n, framebuffers);
}

GL_APICALL void GL_APIENTRY
glDeleteProgram(
                GLuint program
                )
{
    g_opengl->glDeleteProgram(program);
    CURRENT_GL_CONTEXT()->ApiDeleteProgram(program);
}

GL_APICALL void GL_APIENTRY
glDeleteRenderbuffers(
                      GLsizei n,
                      const GLuint* renderbuffers
                      )
{
    g_opengl->glDeleteRenderbuffers(n, renderbuffers);
    CURRENT_GL_CONTEXT()->ApiDeleteRenderbuffers(n, renderbuffers);
}

GL_APICALL void GL_APIENTRY
glDeleteShader(
               GLuint shader
               )
{
    g_opengl->glDeleteShader(shader);
    CURRENT_GL_CONTEXT()->ApiDeleteShader(shader);
}

GL_APICALL void GL_APIENTRY
glDeleteTextures(
                 GLsizei n,
                 const GLuint* textures
                 )
{
    g_opengl->glDeleteTextures(n, textures);
    CURRENT_GL_CONTEXT()->ApiDeleteTextures(n, textures);
}

GL_APICALL void GL_APIENTRY
glDeleteVertexArraysOES (
                         GLsizei n,
                         const GLuint *arrays
                         )
{
    g_opengl->glDeleteVertexArrays(n, arrays);
    CURRENT_GL_CONTEXT()->ApiDeleteVertexArrays(n, arrays);
}

GL_APICALL void GL_APIENTRY
glDepthFunc(
            GLenum func
            )
{
    g_opengl->glDepthFunc(func);
    CURRENT_GL_CONTEXT()->ApiDepthFunc(func);
}

GL_APICALL void GL_APIENTRY
glDepthMask(
            GLboolean flag
            )
{
    g_opengl->glDepthMask(flag);
    CURRENT_GL_CONTEXT()->ApiDepthMask(flag);
}

GL_APICALL void GL_APIENTRY
glDepthRangef(
              GLfloat n,
              GLfloat f
              )
{
    g_opengl->glDepthRangef(n, f);
    CURRENT_GL_CONTEXT()->ApiDepthRangef(n, f);
}

GL_APICALL void GL_APIENTRY
glDetachShader(
               GLuint program,
               GLuint shader
               )
{
    g_opengl->glDetachShader(program, shader);
    CURRENT_GL_CONTEXT()->ApiDetachShader(program, shader);
}

GL_APICALL void GL_APIENTRY
glDisable(
          GLenum cap
          )
{
    g_opengl->glDisable(cap);
    CURRENT_GL_CONTEXT()->ApiDisable(cap);
}

GL_APICALL void GL_APIENTRY
glDisableVertexAttribArray(
                           GLuint index
                           )
{
    g_opengl->glDisableVertexAttribArray(index);
    CURRENT_GL_CONTEXT()->ApiDisableVertexAttribArray(index);
}

GL_APICALL void GL_APIENTRY
glDrawArrays(
             GLenum mode,
             GLint first,
             GLsizei count
             )
{
    g_opengl->glDrawArrays(mode, first, count);
    CURRENT_GL_CONTEXT()->ApiDrawArrays(mode, first, count);;
}

GL_APICALL void GL_APIENTRY
glDrawElements(
               GLenum mode,
               GLsizei count,
               GLenum type,
               const GLvoid* indices
               )
{
    g_opengl->glDrawElements(mode, count, type, indices);
    CURRENT_GL_CONTEXT()->ApiDrawElements(mode, count, type, indices);
}

GL_APICALL void GL_APIENTRY
glEnable(
         GLenum cap
         )
{
    g_opengl->glEnable(cap);
    CURRENT_GL_CONTEXT()->ApiEnable(cap);
}

GL_APICALL void GL_APIENTRY
glEnableVertexAttribArray(
                          GLuint index
                          )
{
    g_opengl->glEnableVertexAttribArray(index);
    CURRENT_GL_CONTEXT()->ApiEnableVertexAttribArray(index);
}

GL_APICALL void GL_APIENTRY
glFinish(
         void
         )
{
    g_opengl->glFinish();
    CURRENT_GL_CONTEXT()->ApiFinish();
}

GL_APICALL void GL_APIENTRY
glFlush(
        void
        )
{
    g_opengl->glFlush();

    CURRENT_GL_CONTEXT()->ApiFlush();
}

GL_APICALL void GL_APIENTRY
glFramebufferRenderbuffer(
                          GLenum target,
                          GLenum attachment,
                          GLenum renderbuffertarget,
                          GLuint renderbuffer
                          )
{
    g_opengl->glFramebufferRenderbuffer(
        target,
        attachment,
        renderbuffertarget,
        renderbuffer
        );

    CURRENT_GL_CONTEXT()->ApiFramebufferRenderbuffer(
        target,
        attachment,
        renderbuffertarget,
        renderbuffer
        );
}

GL_APICALL void GL_APIENTRY
glFramebufferTexture2D(
                       GLenum target,
                       GLenum attachment,
                       GLenum textarget,
                       GLuint texture,
                       GLint level
                       )
{
    g_opengl->glFramebufferTexture2D(
        target,
        attachment,
        textarget,
        texture,
        level
        );

    CURRENT_GL_CONTEXT()->ApiFramebufferTexture2D(
        target,
        attachment,
        textarget,
        texture,
        level
        );
}

GL_APICALL void GL_APIENTRY
glFrontFace(
            GLenum mode
            )
{
    g_opengl->glFrontFace(mode);

    CURRENT_GL_CONTEXT()->ApiFrontFace(mode);
}

GL_APICALL void GL_APIENTRY
glGenBuffers(
             GLsizei n,
             GLuint* buffers
             )
{
    g_opengl->glGenBuffers(n, buffers);

    CURRENT_GL_CONTEXT()->ApiGenBuffers(n, buffers);
}

GL_APICALL void GL_APIENTRY
glGenerateMipmap(
                 GLenum target
                 )
{
    g_opengl->glGenerateMipmap(target);

    CURRENT_GL_CONTEXT()->ApiGenerateMipmap(target);
}

GL_APICALL void GL_APIENTRY
glGenFramebuffers(
                  GLsizei n,
                  GLuint* framebuffers
                  )
{
    g_opengl->glGenFramebuffers(n, framebuffers);

    CURRENT_GL_CONTEXT()->ApiGenFramebuffers(n, framebuffers);
}

GL_APICALL void GL_APIENTRY
glGenRenderbuffers(
                   GLsizei n,
                   GLuint* renderbuffers
                   )
{
    g_opengl->glGenRenderbuffers(n, renderbuffers);

    CURRENT_GL_CONTEXT()->ApiGenRenderbuffers(n, renderbuffers);
}

GL_APICALL void GL_APIENTRY
glGenTextures(
              GLsizei n,
              GLuint* textures
              )
{
    g_opengl->glGenTextures(n, textures);

    CURRENT_GL_CONTEXT()->ApiGenTextures(n, textures);
}

GL_APICALL void GL_APIENTRY
glGenVertexArraysOES (
                      GLsizei n,
                      GLuint *arrays
                      )
{
    g_opengl->glGenVertexArrays(n, arrays);

    CURRENT_GL_CONTEXT()->ApiGenVertexArrays(n, arrays);
}

GL_APICALL void GL_APIENTRY
glGetActiveAttrib(
                  GLuint program,
                  GLuint index,
                  GLsizei bufsize,
                  GLsizei* length,
                  GLint* size,
                  GLenum* type,
                  GLchar* name
                  )
{
    g_opengl->glGetActiveAttrib(
        program,
        index,
        bufsize,
        length,
        size,
        type,
        name
        );

    CURRENT_GL_CONTEXT()->ApiGetActiveAttrib(
        program,
        index,
        bufsize,
        length,
        size,
        type,
        name
        );
}

GL_APICALL void GL_APIENTRY
glGetActiveUniform(
                   GLuint program,
                   GLuint index,
                   GLsizei bufsize,
                   GLsizei* length,
                   GLint* size,
                   GLenum* type,
                   GLchar* name
                   )
{
    g_opengl->glGetActiveUniform(
        program,
        index,
        bufsize,
        length,
        size,
        type,
        name
        );

    CURRENT_GL_CONTEXT()->ApiGetActiveUniform(
        program,
        index,
        bufsize,
        length,
        size,
        type,
        name
        );;
}

GL_APICALL void GL_APIENTRY
glGetAttachedShaders(
                     GLuint program,
                     GLsizei maxcount,
                     GLsizei* count,
                     GLuint* shaders
                     )
{
    g_opengl->glGetAttachedShaders(
        program,
        maxcount,
        count,
        shaders
        );

    CURRENT_GL_CONTEXT()->ApiGetAttachedShaders(
        program,
        maxcount,
        count,
        shaders
        );
}

GL_APICALL int GL_APIENTRY
glGetAttribLocation(
                    GLuint program,
                    const GLchar* name
                    )
{
    int loc = g_opengl->glGetAttribLocation(program, name);

    CURRENT_GL_CONTEXT()->ApiGetAttribLocation(program, name, loc);
    return loc;
}

GL_APICALL void GL_APIENTRY
glGetBooleanv(
              GLenum pname,
              GLboolean* params
              )
{
    g_opengl->glGetBooleanv(pname, params);
    CURRENT_GL_CONTEXT()->ApiGetBooleanv(pname, params);
}

GL_APICALL void GL_APIENTRY
glGetBufferParameteriv(
                       GLenum target,
                       GLenum pname,
                       GLint* params
                       )
{
    g_opengl->glGetBufferParameteriv(target, pname, params);

    CURRENT_GL_CONTEXT()->ApiGetBufferParameteriv(target, pname, params);
}

GL_APICALL GLenum GL_APIENTRY
glGetError(
           void
           )
{
    GLenum  err = g_opengl->glGetError();

    CURRENT_GL_CONTEXT()->ApiGetError(err);

    return err;
}

GL_APICALL void GL_APIENTRY
glGetFloatv(
            GLenum pname,
            GLfloat* params
            )
{
    g_opengl->glGetFloatv(pname, params);

    CURRENT_GL_CONTEXT()->ApiGetFloatv(pname, params);
}

GL_APICALL void GL_APIENTRY
glGetFramebufferAttachmentParameteriv(
                                      GLenum target,
                                      GLenum attachment,
                                      GLenum pname,
                                      GLint* params
                                      )
{
    g_opengl->glGetFramebufferAttachmentParameteriv(
        target,
        attachment,
        pname,
        params
        );

    CURRENT_GL_CONTEXT()->ApiGetFramebufferAttachmentParameteriv(
        target,
        attachment,
        pname,
        params
        );
}

GL_APICALL void GL_APIENTRY
glGetIntegerv(
              GLenum pname,
              GLint* params
              )
{
    g_opengl->glGetIntegerv(pname, params);

    CURRENT_GL_CONTEXT()->ApiGetIntegerv(pname, params);
}

GL_APICALL void GL_APIENTRY
glGetProgramiv(
               GLuint program,
               GLenum pname,
               GLint* params
               )
{
    g_opengl->glGetProgramiv(
        program,
        pname,
        params
        );

    CURRENT_GL_CONTEXT()->ApiGetProgramiv(
        program,
        pname,
        params
        );
}

GL_APICALL void GL_APIENTRY
glGetProgramInfoLog(
                    GLuint program,
                    GLsizei bufsize,
                    GLsizei* length,
                    GLchar* infolog
                    )
{
    g_opengl->glGetProgramInfoLog(
        program,
        bufsize,
        length,
        infolog
        );

    CURRENT_GL_CONTEXT()->ApiGetProgramInfoLog(
        program,
        bufsize,
        length,
        infolog
        );
}

GL_APICALL void GL_APIENTRY
glGetRenderbufferParameteriv(
                             GLenum target,
                             GLenum pname,
                             GLint* params
                             )
{
    g_opengl->glGetRenderbufferParameteriv(
        target,
        pname,
        params
        );

    CURRENT_GL_CONTEXT()->ApiGetRenderbufferParameteriv(
        target,
        pname,
        params
        );
}

GL_APICALL void GL_APIENTRY
glGetShaderiv(
              GLuint shader,
              GLenum pname,
              GLint* params
              )
{
    g_opengl->glGetShaderiv(shader, pname, params);

    CURRENT_GL_CONTEXT()->ApiGetShaderiv(shader, pname, params);
}

GL_APICALL void GL_APIENTRY
glGetShaderInfoLog(
                   GLuint shader,
                   GLsizei bufsize,
                   GLsizei* length,
                   GLchar* infolog
                   )
{
    g_opengl->glGetShaderInfoLog(
        shader,
        bufsize,
        length,
        infolog
        );

    CURRENT_GL_CONTEXT()->ApiGetShaderInfoLog(
        shader,
        bufsize,
        length,
        infolog
        );
}

GL_APICALL void GL_APIENTRY
glGetShaderPrecisionFormat(
                           GLenum shadertype,
                           GLenum precisiontype,
                           GLint* range,
                           GLint* precision
                           )
{
    g_opengl->glGetShaderPrecisionFormat(shadertype, precisiontype, range, precision);
    CURRENT_GL_CONTEXT()->ApiGetShaderPrecisionFormat(shadertype, precisiontype, range, precision);
}

GL_APICALL void GL_APIENTRY
glGetShaderSource(
                  GLuint shader,
                  GLsizei bufsize,
                  GLsizei* length,
                  GLchar* source
                  )
{
    g_opengl->glGetShaderSource(
        shader,
        bufsize,
        length,
        source
        );

    CURRENT_GL_CONTEXT()->ApiGetShaderSource(
        shader,
        bufsize,
        length,
        source
        );
}

GL_APICALL const GLubyte* GL_APIENTRY
glGetString(
            GLenum name
            )
{
    const GLubyte *ret = g_opengl->glGetString(name);
    CURRENT_GL_CONTEXT()->ApiGetString(name, ret);
    return ret;
}

GL_APICALL void GL_APIENTRY
glGetTexParameterfv(
                    GLenum target,
                    GLenum pname,
                    GLfloat* params
                    )
{
    g_opengl->glGetTexParameterfv(target, pname, params);

    CURRENT_GL_CONTEXT()->ApiGetTexParameterfv(target, pname, params);
}

GL_APICALL void GL_APIENTRY
glGetTexParameteriv(
                    GLenum target,
                    GLenum pname,
                    GLint* params
                    )
{
    g_opengl->glGetTexParameteriv(
        target,
        pname,
        params
        );

    CURRENT_GL_CONTEXT()->ApiGetTexParameteriv(
        target,
        pname,
        params
        );
}

GL_APICALL void GL_APIENTRY
glGetUniformfv(
               GLuint program,
               GLint location,
               GLfloat* params
               )
{
    g_opengl->glGetUniformfv(program, location, params);

    CURRENT_GL_CONTEXT()->ApiGetUniformfv(program, location, params);
}

GL_APICALL void GL_APIENTRY
glGetUniformiv(
               GLuint program,
               GLint location,
               GLint* params
               )
{
    g_opengl->glGetUniformiv(program, location, params);

    CURRENT_GL_CONTEXT()->ApiGetUniformiv(program, location, params);
}

GL_APICALL int GL_APIENTRY
glGetUniformLocation(
                     GLuint program,
                     const GLchar* name
                     )
{
    int locn = g_opengl->glGetUniformLocation(program, name);
    CURRENT_GL_CONTEXT()->ApiGetUniformLocation(program, name, locn);
    return locn;
}

GL_APICALL void GL_APIENTRY
glGetVertexAttribfv(
                    GLuint index,
                    GLenum pname,
                    GLfloat* params
                    )
{
    g_opengl->glGetVertexAttribfv(index, pname, params);

    CURRENT_GL_CONTEXT()->ApiGetVertexAttribfv(index, pname, params);
}

GL_APICALL void GL_APIENTRY
glGetVertexAttribiv(
                    GLuint index,
                    GLenum pname,
                    GLint* params
                    )
{
    g_opengl->glGetVertexAttribiv(index, pname, params);

    CURRENT_GL_CONTEXT()->ApiGetVertexAttribiv(index, pname, params);
}

GL_APICALL void GL_APIENTRY
glGetVertexAttribPointerv(
                          GLuint index,
                          GLenum pname,
                          GLvoid** pointer
                          )
{
    g_opengl->glGetVertexAttribPointerv(index, pname, pointer);

    CURRENT_GL_CONTEXT()->ApiGetVertexAttribPointerv(index, pname, pointer);
}

GL_APICALL void GL_APIENTRY
glHint(
       GLenum target,
       GLenum mode
       )
{
    g_opengl->glHint(target, mode);

    CURRENT_GL_CONTEXT()->ApiHint(target, mode);
}

GL_APICALL GLboolean GL_APIENTRY
glIsBuffer(
           GLuint buffer
           )
{
    GLboolean res = g_opengl->glIsBuffer(buffer);
    CURRENT_GL_CONTEXT()->ApiIsBuffer(buffer, res);
    return res;
}

GL_APICALL GLboolean GL_APIENTRY
glIsEnabled(
            GLenum cap
            )
{
    GLboolean result = g_opengl->glIsEnabled(cap);

    CURRENT_GL_CONTEXT()->ApiIsEnabled(cap, result);
    return result;
}

GL_APICALL GLboolean GL_APIENTRY
glIsFramebuffer(
                GLuint framebuffer
                )
{
    GLboolean res = g_opengl->glIsFramebuffer(framebuffer);
    CURRENT_GL_CONTEXT()->ApiIsFramebuffer(framebuffer, res);
    return res;
}

GL_APICALL GLboolean GL_APIENTRY
glIsProgram(
            GLuint program
            )
{
    GLboolean res = g_opengl->glIsProgram(program);
    CURRENT_GL_CONTEXT()->ApiIsProgram(program, res);
    return res;
}

GL_APICALL GLboolean GL_APIENTRY
glIsRenderbuffer(
                 GLuint renderbuffer
                 )
{
    GLboolean res = g_opengl->glIsRenderbuffer (renderbuffer);
    CURRENT_GL_CONTEXT()->ApiIsRenderbuffer (renderbuffer, res);
    return res;
}

GL_APICALL GLboolean GL_APIENTRY
glIsShader(
           GLuint shader
           )
{
    GLboolean res = g_opengl->glIsShader(shader);
    CURRENT_GL_CONTEXT()->ApiIsShader(shader, res);
    return res;
}

GL_APICALL GLboolean GL_APIENTRY
glIsTexture(
            GLuint texture
            )
{
    GLboolean res = g_opengl->glIsTexture(texture);
    CURRENT_GL_CONTEXT()->ApiIsTexture(texture, res);
    return res;
}

GL_APICALL GLboolean GL_APIENTRY
glIsVertexArrayOES(
                   GLuint array
                   )
{
    GLboolean result;
    result = g_opengl->glIsVertexArray(array);
    return result;
}

GL_APICALL void GL_APIENTRY
glLineWidth(
            GLfloat width
            )
{
    g_opengl->glLineWidth(width);

    CURRENT_GL_CONTEXT()->ApiLineWidth(width);
}

GL_APICALL void GL_APIENTRY
glLinkProgram(
              GLuint program
              )
{
    g_opengl->glLinkProgram(program);

    CURRENT_GL_CONTEXT()->ApiLinkProgram(program);
}

GL_APICALL void GL_APIENTRY
glPixelStorei(
              GLenum pname,
              GLint param
              )
{
    g_opengl->glPixelStorei(pname, param);

    CURRENT_GL_CONTEXT()->ApiPixelStorei(pname, param);
}

GL_APICALL void GL_APIENTRY
glPolygonOffset(
                GLfloat factor,
                GLfloat units
                )
{
    g_opengl->glPolygonOffset(factor, units);

    CURRENT_GL_CONTEXT()->ApiPolygonOffset(factor, units);
}

GL_APICALL void GL_APIENTRY
glReadPixels(
             GLint x,
             GLint y,
             GLsizei width,
             GLsizei height,
             GLenum format,
             GLenum type,
             GLvoid* pixels
             )
{
    g_opengl->glReadPixels(
        x, y,
        width, height,
        format,
        type,
        pixels
        );

    CURRENT_GL_CONTEXT()->ApiReadPixels(
        x, y,
        width, height,
        format,
        type,
        pixels
        );
}

GL_APICALL void GL_APIENTRY
glReleaseShaderCompiler(
                        void
                        )
{
    /* No OpenGL implementation for this optional function. */
    /* TODO: Might be needed if using vivante compiler. */
}

GL_APICALL void GL_APIENTRY
glRenderbufferStorage(
                      GLenum target,
                      GLenum internalformat,
                      GLsizei width,
                      GLsizei height
                      )
{
    g_opengl->glRenderbufferStorage(
        target,
        internalformat,
        width,
        height
        );

    CURRENT_GL_CONTEXT()->ApiRenderbufferStorage(
        target,
        internalformat,
        width,
        height
        );
}

GL_APICALL void GL_APIENTRY
glSampleCoverage(
                 GLfloat value,
                 GLboolean invert
                 )
{
    g_opengl->glSampleCoverage(value, invert);

    CURRENT_GL_CONTEXT()->ApiSampleCoverage(value, invert);
}

GL_APICALL void GL_APIENTRY
glScissor(
          GLint x,
          GLint y,
          GLsizei width,
          GLsizei height
          )
{
    g_opengl->glScissor(
        x, y,
        width, height
        );

    CURRENT_GL_CONTEXT()->ApiScissor(
        x, y,
        width, height
        );
}

GL_APICALL void GL_APIENTRY
glShaderBinary(
               GLsizei n,
               const GLuint* shaders,
               GLenum binaryformat,
               const GLvoid* binary,
               GLsizei length
               )
{
    g_opengl->glShaderBinary(n, shaders, binaryformat, binary, length);
    CURRENT_GL_CONTEXT()->ApiShaderBinary(n, shaders, binaryformat, binary, length);
}

GL_APICALL void GL_APIENTRY
glShaderSource(
               GLuint shader,
               GLsizei count,
               const GLchar* const* string,
               const GLint* length
               )
{
    g_opengl->glShaderSource(shader, count, string, length);
    CURRENT_GL_CONTEXT()->ApiShaderSource(shader, count, string, length);
}

GL_APICALL void GL_APIENTRY
glStencilFunc(
              GLenum func,
              GLint ref,
              GLuint mask
              )
{
    g_opengl->glStencilFunc(func, ref, mask);

    CURRENT_GL_CONTEXT()->ApiStencilFunc(func, ref, mask);
}

GL_APICALL void GL_APIENTRY
glStencilFuncSeparate(
                      GLenum face,
                      GLenum func,
                      GLint ref,
                      GLuint mask
                      )
{
    g_opengl->glStencilFuncSeparate(face, func, ref, mask);

    CURRENT_GL_CONTEXT()->ApiStencilFuncSeparate(face, func, ref, mask);
}

GL_APICALL void GL_APIENTRY
glStencilMask(
              GLuint mask
              )
{
    g_opengl->glStencilMask(mask);

    CURRENT_GL_CONTEXT()->ApiStencilMask(mask);
}

GL_APICALL void GL_APIENTRY
glStencilMaskSeparate(
                      GLenum face,
                      GLuint mask
                      )
{
    g_opengl->glStencilMaskSeparate(face, mask);

    CURRENT_GL_CONTEXT()->ApiStencilMaskSeparate(face, mask);
}

GL_APICALL void GL_APIENTRY
glStencilOp(
            GLenum fail,
            GLenum zfail,
            GLenum zpass
            )
{
    g_opengl->glStencilOp(fail, zfail, zpass);

    CURRENT_GL_CONTEXT()->ApiStencilOp(fail, zfail, zpass);
}

GL_APICALL void GL_APIENTRY
glStencilOpSeparate(
                    GLenum face,
                    GLenum fail,
                    GLenum zfail,
                    GLenum zpass
                    )
{
    g_opengl->glStencilOpSeparate(
        face,
        fail,
        zfail,
        zpass
        );

    CURRENT_GL_CONTEXT()->ApiStencilOpSeparate(
        face,
        fail,
        zfail,
        zpass
        );
}

GL_APICALL void GL_APIENTRY
glTexImage2D(
             GLenum target,
             GLint level,
             GLint internalformat,
             GLsizei width,
             GLsizei height,
             GLint border,
             GLenum format,
             GLenum type,
             const GLvoid* pixels
             )
{
    g_opengl->glTexImage2D(
        target,
        level,
        internalformat,
        width, height,
        border,
        format,
        type,
        pixels
        );

    CURRENT_GL_CONTEXT()->ApiTexImage2D(
        target,
        level,
        internalformat,
        width, height,
        border,
        format,
        type,
        pixels
        );
}

GL_APICALL void GL_APIENTRY
glTexParameterf(
                GLenum target,
                GLenum pname,
                GLfloat param
                )
{
    g_opengl->glTexParameterf(target, pname, param);

    CURRENT_GL_CONTEXT()->ApiTexParameterf(target, pname, param);
}

GL_APICALL void GL_APIENTRY
glTexParameterfv(
                 GLenum target,
                 GLenum pname,
                 const GLfloat* params
                 )
{
    g_opengl->glTexParameterfv(target, pname, params);

    CURRENT_GL_CONTEXT()->ApiTexParameterfv(target, pname, params);
}

GL_APICALL void GL_APIENTRY
glTexParameteri(
                GLenum target,
                GLenum pname,
                GLint param
                )
{
    g_opengl->glTexParameteri(target, pname, param);

    CURRENT_GL_CONTEXT()->ApiTexParameteri(target, pname, param);
}

GL_APICALL void GL_APIENTRY
glTexParameteriv(
                 GLenum target,
                 GLenum pname,
                 const GLint* params
                 )
{
    g_opengl->glTexParameteriv(target, pname, params);

    CURRENT_GL_CONTEXT()->ApiTexParameteriv(target, pname, params);
}

GL_APICALL void GL_APIENTRY
glTexSubImage2D(
                GLenum target,
                GLint level,
                GLint xoffset,
                GLint yoffset,
                GLsizei width,
                GLsizei height,
                GLenum format,
                GLenum type,
                const GLvoid* pixels
                )
{
    g_opengl->glTexSubImage2D(
        target,
        level,
        xoffset, yoffset,
        width, height,
        format,
        type,
        pixels
        );

    CURRENT_GL_CONTEXT()->ApiTexSubImage2D(
        target,
        level,
        xoffset, yoffset,
        width, height,
        format,
        type,
        pixels
        );
}

GL_APICALL void GL_APIENTRY
glUniform1f(
            GLint location,
            GLfloat x
            )
{
    g_opengl->glUniform1f(location, x);

    CURRENT_GL_CONTEXT()->ApiUniform1f(location, x);
}

GL_APICALL void GL_APIENTRY
glUniform1fv(
             GLint location,
             GLsizei count,
             const GLfloat* v
             )
{
    g_opengl->glUniform1fv(location, count, v);

    CURRENT_GL_CONTEXT()->ApiUniform1fv(location, count, v);
}

GL_APICALL void GL_APIENTRY
glUniform1i(
            GLint location,
            GLint x
            )
{
    g_opengl->glUniform1i(location, x);

    CURRENT_GL_CONTEXT()->ApiUniform1i(location, x);
}

GL_APICALL void GL_APIENTRY
glUniform1iv(
             GLint location,
             GLsizei count,
             const GLint* v
             )
{
    g_opengl->glUniform1iv(location, count, v);

    CURRENT_GL_CONTEXT()->ApiUniform1iv(location, count, v);
}

GL_APICALL void GL_APIENTRY
glUniform2f(
            GLint location,
            GLfloat x,
            GLfloat y
            )
{
    g_opengl->glUniform2f(location, x, y);

    CURRENT_GL_CONTEXT()->ApiUniform2f(location, x, y);
}

GL_APICALL void GL_APIENTRY
glUniform2fv(
             GLint location,
             GLsizei count,
             const GLfloat* v
             )
{
    g_opengl->glUniform2fv(location, count, v);

    CURRENT_GL_CONTEXT()->ApiUniform2fv(location, count, v);
}

GL_APICALL void GL_APIENTRY
glUniform2i(
            GLint location,
            GLint x,
            GLint y
            )
{
    g_opengl->glUniform2i(location, x, y);

    CURRENT_GL_CONTEXT()->ApiUniform2i(location, x, y);
}

GL_APICALL void GL_APIENTRY
glUniform2iv(
             GLint location,
             GLsizei count,
             const GLint* v
             )
{
    g_opengl->glUniform2iv(location, count, v);

    CURRENT_GL_CONTEXT()->ApiUniform2iv(location, count, v);
}

GL_APICALL void GL_APIENTRY
glUniform3f(
            GLint location,
            GLfloat x,
            GLfloat y,
            GLfloat z
            )
{
    g_opengl->glUniform3f(location, x, y, z);

    CURRENT_GL_CONTEXT()->ApiUniform3f(location, x, y, z);
}

GL_APICALL void GL_APIENTRY
glUniform3fv(
             GLint location,
             GLsizei count,
             const GLfloat* v
             )
{
    g_opengl->glUniform3fv(location, count, v);

    CURRENT_GL_CONTEXT()->ApiUniform3fv(location, count, v);
}

GL_APICALL void GL_APIENTRY
glUniform3i(
            GLint location,
            GLint x,
            GLint y,
            GLint z
            )
{
    g_opengl->glUniform3i(location, x, y, z);

    CURRENT_GL_CONTEXT()->ApiUniform3i(location, x, y, z);
}

GL_APICALL void GL_APIENTRY
glUniform3iv(
             GLint location,
             GLsizei count,
             const GLint* v
             )
{
    g_opengl->glUniform3iv(location, count, v);

    CURRENT_GL_CONTEXT()->ApiUniform3iv(location, count, v);
}

GL_APICALL void GL_APIENTRY
glUniform4f(
            GLint location,
            GLfloat x,
            GLfloat y,
            GLfloat z,
            GLfloat w
            )
{
    g_opengl->glUniform4f(location, x, y, z, w);

    CURRENT_GL_CONTEXT()->ApiUniform4f(location, x, y, z, w);
}

GL_APICALL void GL_APIENTRY
glUniform4fv(
             GLint location,
             GLsizei count,
             const GLfloat* v
             )
{
    g_opengl->glUniform4fv(location, count, v);

    CURRENT_GL_CONTEXT()->ApiUniform4fv(location, count, v);
}

GL_APICALL void GL_APIENTRY
glUniform4i(
            GLint location,
            GLint x,
            GLint y,
            GLint z,
            GLint w
            )
{
    g_opengl->glUniform4i(location, x, y, z, w);

    CURRENT_GL_CONTEXT()->ApiUniform4i(location, x, y, z, w);
}

GL_APICALL void GL_APIENTRY
glUniform4iv(
             GLint location,
             GLsizei count,
             const GLint* v
             )
{
    g_opengl->glUniform4iv(location, count, v);

    CURRENT_GL_CONTEXT()->ApiUniform4iv(location, count, v);
}

GL_APICALL void GL_APIENTRY
glUniformMatrix2fv(
                   GLint location,
                   GLsizei count,
                   GLboolean transpose,
                   const GLfloat* value
                   )
{
    g_opengl->glUniformMatrix2fv(
        location,
        count,
        transpose,
        value
        );

    CURRENT_GL_CONTEXT()->ApiUniformMatrix2fv(
        location,
        count,
        transpose,
        value
        );
}

GL_APICALL void GL_APIENTRY
glUniformMatrix3fv(
                   GLint location,
                   GLsizei count,
                   GLboolean transpose,
                   const GLfloat* value
                   )
{
    g_opengl->glUniformMatrix3fv(
        location,
        count,
        transpose,
        value
        );

    CURRENT_GL_CONTEXT()->ApiUniformMatrix3fv(
        location,
        count,
        transpose,
        value
        );
}

GL_APICALL void GL_APIENTRY
glUniformMatrix4fv(
                   GLint location,
                   GLsizei count,
                   GLboolean transpose,
                   const GLfloat* value
                   )
{
    g_opengl->glUniformMatrix4fv(
        location,
        count,
        transpose,
        value
        );

    CURRENT_GL_CONTEXT()->ApiUniformMatrix4fv(
        location,
        count,
        transpose,
        value
        );
}

GL_APICALL void GL_APIENTRY
glUseProgram(
             GLuint program
             )
{
    g_opengl->glUseProgram(program);

    CURRENT_GL_CONTEXT()->ApiUseProgram(program);
}

GL_APICALL void GL_APIENTRY
glValidateProgram(
                  GLuint program
                  )
{
    g_opengl->glValidateProgram(program);

    CURRENT_GL_CONTEXT()->ApiValidateProgram(program);
}

GL_APICALL void GL_APIENTRY
glVertexAttrib1f(
                 GLuint indx,
                 GLfloat x
                 )
{
    g_opengl->glVertexAttrib1f(indx, x);

    CURRENT_GL_CONTEXT()->ApiVertexAttrib1f(indx, x);
}

GL_APICALL void GL_APIENTRY
glVertexAttrib1fv(
                  GLuint indx,
                  const GLfloat* values
                  )
{
    g_opengl->glVertexAttrib1fv(indx, values);

    CURRENT_GL_CONTEXT()->ApiVertexAttrib1fv(indx, values);
}

GL_APICALL void GL_APIENTRY
glVertexAttrib2f(
                 GLuint indx,
                 GLfloat x,
                 GLfloat y
                 )
{
    g_opengl->glVertexAttrib2f(indx, x, y);

    CURRENT_GL_CONTEXT()->ApiVertexAttrib2f(indx, x, y);
}

GL_APICALL void GL_APIENTRY
glVertexAttrib2fv(
                  GLuint indx,
                  const GLfloat* values
                  )
{
    g_opengl->glVertexAttrib2fv(indx, values);

    CURRENT_GL_CONTEXT()->ApiVertexAttrib2fv(indx, values);
}

GL_APICALL void GL_APIENTRY
glVertexAttrib3f(
                 GLuint indx,
                 GLfloat x,
                 GLfloat y,
                 GLfloat z
                 )
{
    g_opengl->glVertexAttrib3f(indx, x, y, z);

    CURRENT_GL_CONTEXT()->ApiVertexAttrib3f(indx, x, y, z);
}

GL_APICALL void GL_APIENTRY
glVertexAttrib3fv(
                  GLuint indx,
                  const GLfloat* values
                  )
{
    g_opengl->glVertexAttrib3fv (indx, values);

    CURRENT_GL_CONTEXT()->ApiVertexAttrib3fv (indx, values);
}

GL_APICALL void GL_APIENTRY
glVertexAttrib4f(
                 GLuint indx,
                 GLfloat x,
                 GLfloat y,
                 GLfloat z,
                 GLfloat w
                 )
{
    g_opengl->glVertexAttrib4f(indx, x, y, z, w);

    CURRENT_GL_CONTEXT()->ApiVertexAttrib4f(indx, x, y, z, w);
}

GL_APICALL void GL_APIENTRY
glVertexAttrib4fv(
                  GLuint indx,
                  const GLfloat* values
                  )
{
    g_opengl->glVertexAttrib4fv(indx, values);

    CURRENT_GL_CONTEXT()->ApiVertexAttrib4fv(indx, values);
}

GL_APICALL void GL_APIENTRY
glVertexAttribPointer(
                      GLuint indx,
                      GLint size,
                      GLenum type,
                      GLboolean normalized,
                      GLsizei stride,
                      const GLvoid* ptr
                      )
{
    g_opengl->glVertexAttribPointer(
        indx,
        size,
        type,
        normalized,
        stride,
        ptr
        );

    CURRENT_GL_CONTEXT()->ApiVertexAttribPointer(
        indx,
        size,
        type,
        normalized,
        stride,
        ptr
        );
}

GL_APICALL void GL_APIENTRY
glViewport(
           GLint x,
           GLint y,
           GLsizei width,
           GLsizei height
           )
{
    g_opengl->glViewport(x, y, width, height);

    CURRENT_GL_CONTEXT()->ApiViewport(x, y, width, height);
}

/* OpenGL ES 3.0 */

GL_APICALL void GL_APIENTRY
glReadBuffer(
             GLenum mode
             )
{
    g_opengl->glReadBuffer(mode);

    CURRENT_GL_CONTEXT()->ApiReadBuffer(mode);
}

GL_APICALL void GL_APIENTRY
glBlitFramebufferANGLE(
                       GLint srcX0,
                       GLint srcY0,
                       GLint srcX1,
                       GLint srcY1,
                       GLint dstX0,
                       GLint dstY0,
                       GLint dstX1,
                       GLint dstY1,
                       GLbitfield mask,
                       GLenum filter
                       )
{
    g_opengl->glBlitFramebufferEXT(srcX0, srcY0, srcX1, srcY1, dstX0, dstY0, dstX1, dstY1, mask, filter);
    CURRENT_GL_CONTEXT()->ApiBlitFramebufferEXT(srcX0, srcY0, srcX1, srcY1, dstX0, dstY0, dstX1, dstY1, mask, filter);
}

#endif /* API_DUMP */

#if 0
GL_APICALL void GL_APIENTRY
glDrawRangeElements(
                    GLenum mode,
                    GLuint start,
                    GLuint end,
                    GLsizei count,
                    GLenum type,
                    const GLvoid* indices
                    )
{
    CGLES3Context* thread = _GetThreadDataES3();
    switch(mode)
    {
    case GL_POINTS:
    case GL_LINE_STRIP:
    case GL_LINE_LOOP:
    case GL_LINES:
    case GL_TRIANGLE_STRIP:
    case GL_TRIANGLE_FAN:
    case GL_TRIANGLES:
        break;
    default:
        apiERROR(GL_INVALID_ENUM);
        return;
    }
    if(count < 0)
    {
        apiERROR(GL_INVALID_VALUE);
        return;
    }

    if(end < start)
    {
        apiERROR(GL_INVALID_VALUE);
        return;
    }

    if(CURRENT_GL_CONTEXT()->ApiboundedFBO > 0)
    {
        if(g_opengl->glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        {
            apiERROR(GL_INVALID_FRAMEBUFFER_OPERATION);
            return;
        }
    }

    if(CURRENT_GL_CONTEXT()->ApicurrentTFBState == eActive)
    {
        apiERROR(GL_INVALID_OPERATION);
        return;
    }

    g_opengl->glDrawRangeElements(mode, start, end, count, type, indices);

    if (g_dbgForceDrawFront)
    {
        g_opengl->glFinish();
    }

    CURRENT_GL_CONTEXT()->Api;
}


GL_APICALL void GL_APIENTRY
glTexImage3D(
             GLenum target,
             GLint level,
             GLint internalformat,
             GLsizei width,
             GLsizei height,
             GLsizei depth,
             GLint border,
             GLenum format,
             GLenum type,
             const GLvoid* pixels
             )
{
    CGLES3Context* thread = _GetThreadDataES3();
    Texture* tex;
    tex = CURRENT_GL_CONTEXT()->ApiboundTexture[CURRENT_GL_CONTEXT()->ApiactiveTexture - GL_TEXTURE0];
    if (tex != NULL)
    {
        if(level == 0)
        {
            tex->internalformat = internalformat;
        }
    }
    else
    {
        tex = CreateTexture(&CURRENT_GL_CONTEXT()->ApitextureStack);
        tex->internalformat = internalformat;
        CURRENT_GL_CONTEXT()->ApiboundTexture[CURRENT_GL_CONTEXT()->ApiactiveTexture - GL_TEXTURE0] = tex;
    }

    g_opengl->glTexImage3D(
        target,
        level,
        internalformat,
        width, height, depth,
        border,
        format,
        type,
        pixels
        );



    CURRENT_GL_CONTEXT()->Api;
}

GL_APICALL void GL_APIENTRY
glTexSubImage3D(
                GLenum target,
                GLint level,
                GLint xoffset,
                GLint yoffset,
                GLint zoffset,
                GLsizei width,
                GLsizei height,
                GLsizei depth,
                GLenum format,
                GLenum type,
                const GLvoid* pixels
                )
{
    g_opengl->glTexSubImage3D(
        target,
        level,
        xoffset, yoffset, zoffset,
        width, height, depth,
        format,
        type,
        pixels
        );

    CURRENT_GL_CONTEXT()->Api;
}

GL_APICALL void GL_APIENTRY
glCopyTexSubImage3D(
                    GLenum target,
                    GLint level,
                    GLint xoffset,
                    GLint yoffset,
                    GLint zoffset,
                    GLint x,
                    GLint y,
                    GLsizei width,
                    GLsizei height
                    )
{
    g_opengl->glCopyTexSubImage3D(
        target,
        level,
        xoffset, yoffset, zoffset,
        x, y,
        width, height
        );

    CURRENT_GL_CONTEXT()->Api;
}

GL_APICALL void GL_APIENTRY
glCompressedTexImage3D(
                       GLenum target,
                       GLint level,
                       GLenum internalformat,
                       GLsizei width,
                       GLsizei height,
                       GLsizei depth,
                       GLint border,
                       GLsizei imageSize,
                       const GLvoid* data
                       )
{
    CGLES3Context* thread = _GetThreadDataES3();
    Texture* tex;

    tex = CURRENT_GL_CONTEXT()->ApiboundTexture[CURRENT_GL_CONTEXT()->ApiactiveTexture - GL_TEXTURE0];

    if (tex != NULL)
    {
        if(level == 0)
        {
            tex->internalformat = internalformat;
        }
    }
    else
    {
        tex = CreateTexture(&CURRENT_GL_CONTEXT()->ApitextureStack);
        tex->internalformat = internalformat;
        CURRENT_GL_CONTEXT()->ApiboundTexture[CURRENT_GL_CONTEXT()->ApiactiveTexture - GL_TEXTURE0] = tex;

    }
    g_opengl->glCompressedTexImage3D(target, level, internalformat, width, height, depth, border, imageSize, data);

    CURRENT_GL_CONTEXT()->Api;
}

GL_APICALL void GL_APIENTRY
glCompressedTexSubImage3D(
                          GLenum target,
                          GLint level,
                          GLint xoffset,
                          GLint yoffset,
                          GLint zoffset,
                          GLsizei width,
                          GLsizei height,
                          GLsizei depth,
                          GLenum format,
                          GLsizei imageSize,
                          const GLvoid* data
                          )
{
    CGLES3Context* thread = _GetThreadDataES3();
    Texture* tex;
    tex = CURRENT_GL_CONTEXT()->ApiboundTexture[CURRENT_GL_CONTEXT()->ApiactiveTexture - GL_TEXTURE0];
    switch (tex->internalformat)
    {
    case GL_COMPRESSED_R11_EAC:
    case GL_COMPRESSED_SIGNED_R11_EAC:
    case GL_COMPRESSED_RG11_EAC:
    case GL_COMPRESSED_SIGNED_RG11_EAC:
    case GL_COMPRESSED_RGB8_ETC2:
    case GL_COMPRESSED_SRGB8_ETC2:
    case GL_COMPRESSED_RGB8_PUNCHTHROUGH_ALPHA1_ETC2:
    case GL_COMPRESSED_SRGB8_PUNCHTHROUGH_ALPHA1_ETC2:
    case GL_COMPRESSED_RGBA8_ETC2_EAC:
    case GL_COMPRESSED_SRGB8_ALPHA8_ETC2_EAC:
        break;
    default:
        apiERROR(GL_INVALID_ENUM);
        return;
    }
    g_opengl->glCompressedTexSubImage3D(target, level, xoffset, yoffset, zoffset, width, height, depth, format, imageSize, data);

    CURRENT_GL_CONTEXT()->Api;
}

GL_APICALL void GL_APIENTRY
glGenQueries(
             GLsizei n,
             GLuint* ids
             )
{
    g_opengl->glGenQueries(n, ids);

    CURRENT_GL_CONTEXT()->Api;
}

GL_APICALL void GL_APIENTRY
glDeleteQueries(
                GLsizei n,
                const GLuint* ids
                )
{
    g_opengl->glDeleteQueries(n, ids);

    CURRENT_GL_CONTEXT()->Api;
}

GL_APICALL GLboolean GL_APIENTRY
glIsQuery(
          GLuint id
          )
{
    GLboolean result;
    result = g_opengl->glIsQuery(id);

    CURRENT_GL_CONTEXT()->Api;
    return result;
}

GL_APICALL void GL_APIENTRY
glBeginQuery(
             GLenum target,
             GLuint id
             )
{
    CGLES3Context* thread = _GetThreadDataES3();
    switch(target)
    {
    case GL_ANY_SAMPLES_PASSED:
    case GL_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN:
    case GL_ANY_SAMPLES_PASSED_CONSERVATIVE:
        break;
    default:
        apiERROR(GL_INVALID_ENUM);
        return;
    }
    g_opengl->glBeginQuery(target, id);

    CURRENT_GL_CONTEXT()->Api;
}

GL_APICALL void GL_APIENTRY
glEndQuery(
           GLenum target
           )
{
    CGLES3Context* thread = _GetThreadDataES3();
    switch(target)
    {
    case GL_ANY_SAMPLES_PASSED:
    case GL_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN:
    case GL_ANY_SAMPLES_PASSED_CONSERVATIVE:
        break;
    default:
        apiERROR(GL_INVALID_ENUM);
        return;
    }
    g_opengl->glEndQuery(target);

    CURRENT_GL_CONTEXT()->Api;
}

GL_APICALL void GL_APIENTRY
glGetQueryiv(
             GLenum target,
             GLenum pname,
             GLint* params
             )
{
    CGLES3Context* thread = _GetThreadDataES3();
    switch(target)
    {
    case GL_ANY_SAMPLES_PASSED:
    case GL_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN:
    case GL_ANY_SAMPLES_PASSED_CONSERVATIVE:
        break;
    default:
        apiERROR(GL_INVALID_ENUM);
        return;
    }
    g_opengl->glGetQueryiv(target, pname, params);

    CURRENT_GL_CONTEXT()->Api;
}

GL_APICALL void GL_APIENTRY
glGetQueryObjectuiv(
                    GLuint id,
                    GLenum pname,
                    GLuint* params
                    )
{
    g_opengl->glGetQueryObjectuiv(id, pname, params);

    CURRENT_GL_CONTEXT()->Api;
}

GL_APICALL GLboolean GL_APIENTRY
glUnmapBuffer(
              GLenum target
              )
{
    GLboolean result = GL_FALSE;
    switch(target)
    {
    case GL_ARRAY_BUFFER:
    case GL_ELEMENT_ARRAY_BUFFER:
    case GL_COPY_READ_BUFFER:
    case GL_COPY_WRITE_BUFFER:
    case GL_PIXEL_PACK_BUFFER:
    case GL_PIXEL_UNPACK_BUFFER:
    case GL_TRANSFORM_FEEDBACK_BUFFER:
    case GL_UNIFORM_BUFFER:
        break;
    default:
        apiERROR(GL_INVALID_ENUM);
        return result;
    }
    result = g_opengl->glUnmapBuffer(target);

    CURRENT_GL_CONTEXT()->Api;
    return result;
}

GL_APICALL void GL_APIENTRY
glGetBufferPointerv(
                    GLenum target,
                    GLenum pname,
                    GLvoid** params
                    )
{
    switch(target)
    {
    case GL_ARRAY_BUFFER:
    case GL_ELEMENT_ARRAY_BUFFER:
    case GL_COPY_READ_BUFFER:
    case GL_COPY_WRITE_BUFFER:
    case GL_PIXEL_PACK_BUFFER:
    case GL_PIXEL_UNPACK_BUFFER:
    case GL_TRANSFORM_FEEDBACK_BUFFER:
    case GL_UNIFORM_BUFFER:
        break;
    default:
        apiERROR(GL_INVALID_ENUM);
        return;
    }
    g_opengl->glGetBufferPointerv(target, pname, params);

    CURRENT_GL_CONTEXT()->Api;
}

GL_APICALL void GL_APIENTRY
glDrawBuffers(
              GLsizei n,
              const GLenum* bufs
              )
{
    g_opengl->glDrawBuffers(n, bufs);

    CURRENT_GL_CONTEXT()->Api;
}

GL_APICALL void GL_APIENTRY
glUniformMatrix2x3fv(
                     GLint location,
                     GLsizei count,
                     GLboolean transpose,
                     const GLfloat* value
                     )
{
    g_opengl->glUniformMatrix2x3fv(location, count, transpose, value);

    CURRENT_GL_CONTEXT()->Api;
}

GL_APICALL void GL_APIENTRY
glUniformMatrix3x2fv(
                     GLint location,
                     GLsizei count,
                     GLboolean transpose,
                     const GLfloat* value
                     )
{
    g_opengl->glUniformMatrix3x2fv(location, count, transpose, value);

    CURRENT_GL_CONTEXT()->Api;
}

GL_APICALL void GL_APIENTRY
glUniformMatrix2x4fv(
                     GLint location,
                     GLsizei count,
                     GLboolean transpose,
                     const GLfloat* value
                     )
{
    g_opengl->glUniformMatrix2x4fv(location, count, transpose, value);

    CURRENT_GL_CONTEXT()->Api;
}

GL_APICALL void GL_APIENTRY
glUniformMatrix4x2fv(
                     GLint location,
                     GLsizei count,
                     GLboolean transpose,
                     const GLfloat* value
                     )
{
    g_opengl->glUniformMatrix4x2fv(location, count, transpose, value);

    CURRENT_GL_CONTEXT()->Api;
}

GL_APICALL void GL_APIENTRY
glUniformMatrix3x4fv(
                     GLint location,
                     GLsizei count,
                     GLboolean transpose,
                     const GLfloat* value
                     )
{
    g_opengl->glUniformMatrix3x4fv(location, count, transpose, value);

    CURRENT_GL_CONTEXT()->Api;
}

GL_APICALL void GL_APIENTRY
glUniformMatrix4x3fv(
                     GLint location,
                     GLsizei count,
                     GLboolean transpose,
                     const GLfloat* value
                     )
{
    g_opengl->glUniformMatrix4x3fv(location, count, transpose, value);

    CURRENT_GL_CONTEXT()->Api;
}

GL_APICALL void GL_APIENTRY
glBlitFramebuffer(
                  GLint srcX0,
                  GLint srcY0,
                  GLint srcX1,
                  GLint srcY1,
                  GLint dstX0,
                  GLint dstY0,
                  GLint dstX1,
                  GLint dstY1,
                  GLbitfield mask,
                  GLenum filter
                  )
{
    g_opengl->glBlitFramebuffer(srcX0, srcY0, srcX1, srcY1, dstX0, dstY0, dstX1, dstY1, mask, filter);

    CURRENT_GL_CONTEXT()->Api;
}

GL_APICALL void GL_APIENTRY
glRenderbufferStorageMultisample(
                                 GLenum target,
                                 GLsizei samples,
                                 GLenum internalformat,
                                 GLsizei width,
                                 GLsizei height
                                 )
{
    g_opengl->glRenderbufferStorageMultisample(target, samples, internalformat, width, height);

    CURRENT_GL_CONTEXT()->Api;
}

GL_APICALL void GL_APIENTRY
glFramebufferTextureLayer(
                          GLenum target,
                          GLenum attachment,
                          GLuint texture,
                          GLint level,
                          GLint layer
                          )
{
    //TODO need add check, since some of the behaviors are different from 4.3
    // layer must neither be greater than GL_MAX_3D_TEXTURE_SIZE -1 for a 3D texture
    // nor greater than GL_MAX_ARRAY_TEXTURE_SIZE -1 for a 2D array texture

    g_opengl->glFramebufferTextureLayer(target, attachment, texture, level, layer);

    CURRENT_GL_CONTEXT()->Api;
}

GL_APICALL GLvoid* GL_APIENTRY
glMapBufferRange(
                 GLenum target,
                 GLintptr offset,
                 GLsizeiptr length,
                 GLbitfield access
                 )
{
    GLvoid* result = NULL;
    switch(target)
    {
    case GL_ARRAY_BUFFER:
    case GL_ELEMENT_ARRAY_BUFFER:
    case GL_COPY_READ_BUFFER:
    case GL_COPY_WRITE_BUFFER:
    case GL_PIXEL_PACK_BUFFER:
    case GL_PIXEL_UNPACK_BUFFER:
    case GL_TRANSFORM_FEEDBACK_BUFFER:
    case GL_UNIFORM_BUFFER:
        break;
    default:
        apiERROR(GL_INVALID_ENUM);
        return result;
    }
    result = g_opengl->glMapBufferRange(target, offset, length, access);

    CURRENT_GL_CONTEXT()->Api;
    return result;
}

GL_APICALL void GL_APIENTRY
glFlushMappedBufferRange(
                         GLenum target,
                         GLintptr offset,
                         GLsizeiptr length
                         )
{
    switch(target)
    {
    case GL_ARRAY_BUFFER:
    case GL_ELEMENT_ARRAY_BUFFER:
    case GL_COPY_READ_BUFFER:
    case GL_COPY_WRITE_BUFFER:
    case GL_PIXEL_PACK_BUFFER:
    case GL_PIXEL_UNPACK_BUFFER:
    case GL_TRANSFORM_FEEDBACK_BUFFER:
    case GL_UNIFORM_BUFFER:
        break;
    default:
        apiERROR(GL_INVALID_ENUM);
        return;
    }
    g_opengl->glFlushMappedBufferRange(target, offset, length);

    CURRENT_GL_CONTEXT()->Api;
}

GL_APICALL void GL_APIENTRY
glBindVertexArray(
                  GLuint array
                  )
{
    g_opengl->glBindVertexArray(array);

    CURRENT_GL_CONTEXT()->Api;
}

GL_APICALL void GL_APIENTRY
glDeleteVertexArrays(
                     GLsizei n,
                     const GLuint* arrays
                     )
{
    g_opengl->glDeleteVertexArrays(n, arrays);

    CURRENT_GL_CONTEXT()->Api;
}

GL_APICALL void GL_APIENTRY
glGenVertexArrays(
                  GLsizei n,
                  GLuint* arrays
                  )
{
    g_opengl->glGenVertexArrays(n, arrays);

    CURRENT_GL_CONTEXT()->Api;
}

GL_APICALL GLboolean GL_APIENTRY
glIsVertexArray(
                GLuint array
                )
{
    GLboolean result;
    result = g_opengl->glIsVertexArray(array);

    CURRENT_GL_CONTEXT()->Api;
    return result;
}

GL_APICALL void GL_APIENTRY
glGetIntegeri_v(
                GLenum target,
                GLuint index,
                GLint* data
                )
{
    CGLES3Context* thread = _GetThreadDataES3();
    //TODO
    if (data == NULL)
    {
        apiERROR(GL_INVALID_VALUE);
        return;
    }
    g_opengl->glGetIntegeri_v(target, index, data);
    CURRENT_GL_CONTEXT()->Api;
}

GL_APICALL void GL_APIENTRY
glBeginTransformFeedback(
                         GLenum primitiveMode
                         )
{
    CGLES3Context* thread = _GetThreadDataES3();
    if((primitiveMode != GL_POINTS) && (primitiveMode != GL_LINES) && (primitiveMode != GL_TRIANGLES))
    {
        apiERROR(GL_INVALID_ENUM);
        return;
    }
    g_opengl->glBeginTransformFeedback(primitiveMode);
    CURRENT_GL_CONTEXT()->Api;
    if(CURRENT_GL_CONTEXT()->Apierror == GL_NO_ERROR)
    {
        CURRENT_GL_CONTEXT()->ApicurrentTFBState = eActive;
        CURRENT_GL_CONTEXT()->ApicurrentTFBPrimMode = primitiveMode;
    }

}

GL_APICALL void GL_APIENTRY
glEndTransformFeedback(
                       void
                       )
{
    CGLES3Context* thread = _GetThreadDataES3();
    g_opengl->glEndTransformFeedback();
    CURRENT_GL_CONTEXT()->Api;
    if(CURRENT_GL_CONTEXT()->Apierror == GL_NO_ERROR)
    {
        CURRENT_GL_CONTEXT()->ApicurrentTFBState = eDeactive;
    }
}

GL_APICALL void GL_APIENTRY
glBindBufferRange(
                  GLenum target,
                  GLuint index,
                  GLuint buffer,
                  GLintptr offset,
                  GLsizeiptr size
                  )
{
    switch(target)
    {
    case GL_TRANSFORM_FEEDBACK_BUFFER:
    case GL_UNIFORM_BUFFER:
        break;
    default:
        apiERROR(GL_INVALID_ENUM);
        return;
    }
    g_opengl->glBindBufferRange(target, index, buffer, offset, size);

    CURRENT_GL_CONTEXT()->Api;
}

GL_APICALL void GL_APIENTRY
glBindBufferBase(
                 GLenum target,
                 GLuint index,
                 GLuint buffer
                 )
{
    switch(target)
    {
    case GL_TRANSFORM_FEEDBACK_BUFFER:
    case GL_UNIFORM_BUFFER:
        break;
    default:
        apiERROR(GL_INVALID_ENUM);
        return;
    }
    g_opengl->glBindBufferBase(target, index, buffer);

    CURRENT_GL_CONTEXT()->Api;
}

GL_APICALL void GL_APIENTRY
glTransformFeedbackVaryings(
                            GLuint program,
                            GLsizei count,
                            const GLchar* const* varyings,
                            GLenum bufferMode
                            )
{
    g_opengl->glTransformFeedbackVaryings(program, count, varyings, bufferMode);
    CURRENT_GL_CONTEXT()->Api;
}

GL_APICALL void GL_APIENTRY
glGetTransformFeedbackVarying(
                              GLuint program,
                              GLuint index,
                              GLsizei bufSize,
                              GLsizei* length,
                              GLsizei* size,
                              GLenum* type,
                              GLchar* name
                              )
{
    g_opengl->glGetTransformFeedbackVarying(program, index, bufSize, length, size, type, name);
    CURRENT_GL_CONTEXT()->Api;
}

GL_APICALL void GL_APIENTRY
glVertexAttribIPointer(
                       GLuint index,
                       GLint size,
                       GLenum type,
                       GLsizei stride,
                       const GLvoid* pointer
                       )
{
    g_opengl->glVertexAttribIPointer(index, size, type, stride, pointer);
    CURRENT_GL_CONTEXT()->Api;
}

GL_APICALL void GL_APIENTRY
glGetVertexAttribIiv(
                     GLuint index,
                     GLenum pname,
                     GLint* params
                     )
{
    g_opengl->glGetVertexAttribIiv(index, pname, params);
    CURRENT_GL_CONTEXT()->Api;
}

GL_APICALL void GL_APIENTRY
glGetVertexAttribIuiv(
                      GLuint index,
                      GLenum pname,
                      GLuint* params
                      )
{
    g_opengl->glGetVertexAttribIuiv(index, pname, params);
    CURRENT_GL_CONTEXT()->Api;
}

GL_APICALL void GL_APIENTRY
glVertexAttribI4i(
                  GLuint index,
                  GLint x,
                  GLint y,
                  GLint z,
                  GLint w
                  )
{
    g_opengl->glVertexAttribI4i(index, x, y, z, w);
    CURRENT_GL_CONTEXT()->Api;
}

GL_APICALL void GL_APIENTRY
glVertexAttribI4ui(
                   GLuint index,
                   GLuint x,
                   GLuint y,
                   GLuint z,
                   GLuint w
                   )
{
    g_opengl->glVertexAttribI4ui(index, x, y, z, w);
    CURRENT_GL_CONTEXT()->Api;
}

GL_APICALL void GL_APIENTRY
glVertexAttribI4iv(
                   GLuint index,
                   const GLint* v
                   )
{
    g_opengl->glVertexAttribI4iv(index, v);
    CURRENT_GL_CONTEXT()->Api;
}

GL_APICALL void GL_APIENTRY
glVertexAttribI4uiv(
                    GLuint index,
                    const GLuint* v
                    )
{
    g_opengl->glVertexAttribI4uiv(index, v);
    CURRENT_GL_CONTEXT()->Api;
}

GL_APICALL void GL_APIENTRY
glGetUniformuiv(
                GLuint program,
                GLint location,
                GLuint* params
                )
{
    g_opengl->glGetUniformuiv(program, location, params);
    CURRENT_GL_CONTEXT()->Api;
}

GL_APICALL GLint GL_APIENTRY
glGetFragDataLocation(
                      GLuint program,
                      const GLchar *name
                      )
{
    GLint location = -1;
    location = g_opengl->glGetFragDataLocation(program, name);
    CURRENT_GL_CONTEXT()->Api;
    return location;
}

GL_APICALL void GL_APIENTRY
glUniform1ui(
             GLint location,
             GLuint v0
             )
{
    g_opengl->glUniform1ui(location, v0);

    CURRENT_GL_CONTEXT()->Api;
}

GL_APICALL void GL_APIENTRY
glUniform2ui(
             GLint location,
             GLuint v0,
             GLuint v1
             )
{
    g_opengl->glUniform2ui(location, v0, v1);

    CURRENT_GL_CONTEXT()->Api;
}

GL_APICALL void GL_APIENTRY
glUniform3ui(
             GLint location,
             GLuint v0,
             GLuint v1,
             GLuint v2
             )
{
    g_opengl->glUniform3ui(location, v0, v1, v2);

    CURRENT_GL_CONTEXT()->Api;
}

GL_APICALL void GL_APIENTRY
glUniform4ui(
             GLint location,
             GLuint v0,
             GLuint v1,
             GLuint v2,
             GLuint v3
             )
{
    g_opengl->glUniform4ui(location, v0, v1, v2, v3);

    CURRENT_GL_CONTEXT()->Api;
}

GL_APICALL void GL_APIENTRY
glUniform1uiv(
              GLint location,
              GLsizei count,
              const GLuint* value
              )
{
    g_opengl->glUniform1uiv(location, count, value);

    CURRENT_GL_CONTEXT()->Api;
}

GL_APICALL void GL_APIENTRY
glUniform2uiv(
              GLint location,
              GLsizei count,
              const GLuint* value
              )
{
    g_opengl->glUniform2uiv(location, count, value);

    CURRENT_GL_CONTEXT()->Api;
}

GL_APICALL void GL_APIENTRY
glUniform3uiv(
              GLint location,
              GLsizei count,
              const GLuint* value
              )
{
    g_opengl->glUniform3uiv(location, count, value);

    CURRENT_GL_CONTEXT()->Api;
}

GL_APICALL void GL_APIENTRY
glUniform4uiv(
              GLint location,
              GLsizei count,
              const GLuint* value
              )
{
    g_opengl->glUniform4uiv(location, count, value);

    CURRENT_GL_CONTEXT()->Api;
}

GL_APICALL void GL_APIENTRY
glClearBufferiv(
                GLenum buffer,
                GLint drawbuffer,
                const GLint* value
                )
{
    g_opengl->glClearBufferiv(buffer, drawbuffer, value);
    CURRENT_GL_CONTEXT()->Api;
}

GL_APICALL void GL_APIENTRY
glClearBufferuiv(
                 GLenum buffer,
                 GLint drawbuffer,
                 const GLuint* value
                 )
{
    g_opengl->glClearBufferuiv(buffer, drawbuffer, value);
    CURRENT_GL_CONTEXT()->Api;
}

GL_APICALL void GL_APIENTRY
glClearBufferfv(
                GLenum buffer,
                GLint drawbuffer,
                const GLfloat* value
                )
{
    g_opengl->glClearBufferfv(buffer, drawbuffer, value);
    CURRENT_GL_CONTEXT()->Api;
}

GL_APICALL void GL_APIENTRY
glClearBufferfi(
                GLenum buffer,
                GLint drawbuffer,
                GLfloat depth,
                GLint stencil
                )
{
    g_opengl->glClearBufferfi(buffer, drawbuffer, depth, stencil);
    CURRENT_GL_CONTEXT()->Api;
}

GL_APICALL const GLubyte* GL_APIENTRY
glGetStringi(
             GLenum name,
             GLuint index
             )
{
    //TODO
    GLubyte* result = NULL;
    if(name != GL_EXTENSIONS)
    {
        return result;
    }

    return result;
}

GL_APICALL void GL_APIENTRY
glCopyBufferSubData(
                    GLenum readTarget,
                    GLenum writeTarget,
                    GLintptr readOffset,
                    GLintptr writeOffset,
                    GLsizeiptr size
                    )
{
    g_opengl->glCopyBufferSubData(readTarget, writeTarget, readOffset, writeOffset, size);
    CURRENT_GL_CONTEXT()->Api;
}

GL_APICALL void GL_APIENTRY
glGetUniformIndices(
                    GLuint program,
                    GLsizei uniformCount,
                    const GLchar* const* uniformNames,
                    GLuint* uniformIndices
                    )
{
    g_opengl->glGetUniformIndices(program, uniformCount, uniformNames, uniformIndices);
    CURRENT_GL_CONTEXT()->Api;
}

GL_APICALL void GL_APIENTRY
glGetActiveUniformsiv(
                      GLuint program,
                      GLsizei uniformCount,
                      const GLuint* uniformIndices,
                      GLenum pname,
                      GLint* params
                      )
{
    g_opengl->glGetActiveUniformsiv(program, uniformCount, uniformIndices, pname, params);
    CURRENT_GL_CONTEXT()->Api;
}

GL_APICALL GLuint GL_APIENTRY
glGetUniformBlockIndex(
                       GLuint program,
                       const GLchar* uniformBlockName
                       )
{
    GLuint index = GL_INVALID_INDEX;
    index = g_opengl->glGetUniformBlockIndex(program, uniformBlockName);
    CURRENT_GL_CONTEXT()->Api;
    return index;
}

GL_APICALL void GL_APIENTRY
glGetActiveUniformBlockiv(
                          GLuint program,
                          GLuint uniformBlockIndex,
                          GLenum pname,
                          GLint* params
                          )
{
    g_opengl->glGetActiveUniformBlockiv(program, uniformBlockIndex, pname, params);
    CURRENT_GL_CONTEXT()->Api;
}

GL_APICALL void GL_APIENTRY
glGetActiveUniformBlockName(
                            GLuint program,
                            GLuint uniformBlockIndex,
                            GLsizei bufSize,
                            GLsizei* length,
                            GLchar* uniformBlockName
                            )
{
    g_opengl->glGetActiveUniformBlockName(program, uniformBlockIndex, bufSize, length, uniformBlockName);
    CURRENT_GL_CONTEXT()->Api;
}

GL_APICALL void GL_APIENTRY
glUniformBlockBinding(
                      GLuint program,
                      GLuint uniformBlockIndex,
                      GLuint uniformBlockBinding
                      )
{
    g_opengl->glUniformBlockBinding(program, uniformBlockIndex, uniformBlockBinding);
    CURRENT_GL_CONTEXT()->Api;
}

GL_APICALL void GL_APIENTRY
glDrawArraysInstanced(
                      GLenum mode,
                      GLint first,
                      GLsizei count,
                      GLsizei instanceCount
                      )
{
    CGLES3Context* thread = _GetThreadDataES3();

    if(first < 0)
    {
        apiERROR(GL_INVALID_VALUE);
        return;
    }
    if(CURRENT_GL_CONTEXT()->ApiboundedFBO > 0)
    {
        if(g_opengl->glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        {
            apiERROR(GL_INVALID_FRAMEBUFFER_OPERATION);
            return;
        }
    }
    if(CURRENT_GL_CONTEXT()->ApicurrentTFBState == eActive)
    {
        if(CURRENT_GL_CONTEXT()->ApicurrentTFBPrimMode != mode)
        {
            apiERROR(GL_INVALID_OPERATION);
            return;
        }
    }

    g_opengl->glDrawArraysInstanced(mode, first, count, instanceCount);

    if (g_dbgForceDrawFront)
    {
        g_opengl->glFinish();
    }

    CURRENT_GL_CONTEXT()->Api;
}

GL_APICALL void GL_APIENTRY
glDrawElementsInstanced(
                        GLenum mode,
                        GLsizei count,
                        GLenum type,
                        const GLvoid* indices,
                        GLsizei instanceCount
                        )
{
    CGLES3Context* thread = _GetThreadDataES3();
    switch(mode)
    {
    case GL_POINTS:
    case GL_LINE_STRIP:
    case GL_LINE_LOOP:
    case GL_LINES:
    case GL_TRIANGLE_STRIP:
    case GL_TRIANGLE_FAN:
    case GL_TRIANGLES:
        break;
    default:
        apiERROR(GL_INVALID_ENUM);
        return;
    }
    if(CURRENT_GL_CONTEXT()->ApiboundedFBO > 0)
    {
        if(g_opengl->glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        {
            apiERROR(GL_INVALID_FRAMEBUFFER_OPERATION);
            return;
        }
    }
    if(CURRENT_GL_CONTEXT()->ApicurrentTFBState == eActive)
    {
        apiERROR(GL_INVALID_OPERATION);
        return;
    }
    g_opengl->glDrawElementsInstanced(mode, count, type, indices, instanceCount);

    if (g_dbgForceDrawFront)
    {
        g_opengl->glFinish();
    }

    CURRENT_GL_CONTEXT()->Api;
}

GL_APICALL GLsync GL_APIENTRY
glFenceSync(
            GLenum condition,
            GLbitfield flags
            )
{
    GLsync sync = NULL;
    sync = g_opengl->glFenceSync(condition, flags);
    CURRENT_GL_CONTEXT()->Api;
    return sync;
}

GL_APICALL GLboolean GL_APIENTRY
glIsSync(
         GLsync sync
         )
{
    GLboolean result = GL_FALSE;
    result = g_opengl->glIsSync(sync);
    CURRENT_GL_CONTEXT()->Api;
    return result;
}

GL_APICALL void GL_APIENTRY
glDeleteSync(
             GLsync sync
             )
{
    g_opengl->glDeleteSync(sync);
    CURRENT_GL_CONTEXT()->Api;
}

GL_APICALL GLenum GL_APIENTRY
glClientWaitSync(
                 GLsync sync,
                 GLbitfield flags,
                 GLuint64 timeout
                 )
{
    GLenum result;
    result = g_opengl->glClientWaitSync(sync, flags, timeout);
    CURRENT_GL_CONTEXT()->Api;
    return result;
}

GL_APICALL void GL_APIENTRY
glWaitSync(
           GLsync sync,
           GLbitfield flags,
           GLuint64 timeout
           )
{
    g_opengl->glWaitSync(sync, flags, timeout);
    CURRENT_GL_CONTEXT()->Api;
}

GL_APICALL void GL_APIENTRY
glGetInteger64v(
                GLenum pname,
                GLint64* params
                )
{
    CGLES3Context* thread = _GetThreadDataES3();
    //TODO
    if (params == NULL)
    {
        apiERROR(GL_INVALID_VALUE);
        return;
    }
    switch (pname)
    {
    case GL_COMPRESSED_TEXTURE_FORMATS:
        //GetCompressedTexFormats((GLenum*)params);
        params[0] = GetCompressedTexNum();
        break;
    case GL_MAJOR_VERSION:
        params[0] = 3;
        break;
    case GL_MAX_3D_TEXTURE_SIZE:
        params[0] = 8192;
        break;
    case GL_MAX_ARRAY_TEXTURE_LAYERS:
        params[0] = MAX_HW_ARRAY_SIZE;
        break;
    case GL_MAX_COLOR_ATTACHMENTS:
        params[0] = MAX_COLOR_ATTACHMENTS;
        break;
    case GL_MAX_COMBINED_FRAGMENT_UNIFORM_COMPONENTS:
        params[0] = 65535;
        break;
    case GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS:
        params[0] = (TEXTURE_CAPS_VERTEX_SAMPLIERS + TEXTURE_CAPS_PIXEL_SAMPLIERS);
        break;
    case GL_MAX_COMBINED_UNIFORM_BLOCKS:
        params[0] = 32;
        break;
    case GL_MAX_COMBINED_VERTEX_UNIFORM_COMPONENTS:
        params[0] = 65535;
        break;
    case GL_MAX_CUBE_MAP_TEXTURE_SIZE:
        params[0] = 8192;
        break;
    case GL_MAX_DRAW_BUFFERS:
        params[0] = MAX_DRAW_BUFFERS;
        break;
    case GL_MAX_ELEMENT_INDEX:
        params[0] = (1 << 24) - 1;
        break;
    case GL_MAX_ELEMENTS_INDICES:
        params[0] = 65535;
        break;
    case GL_MAX_ELEMENTS_VERTICES:
        params[0] = 32;
        break;
    case GL_MAX_FRAGMENT_INPUT_COMPONENTS:
        params[0] = SHADER_CAPS_VARYINGS * 4;
        break;
    case GL_MAX_FRAGMENT_UNIFORM_BLOCKS:
        params[0] = 16;
        break;
    case GL_MAX_FRAGMENT_UNIFORM_COMPONENTS:
        params[0] = SHADER_CAPS_FRAGMENT_UNIFORMS * 4;
        break;
    case GL_MAX_FRAGMENT_UNIFORM_VECTORS:
        params[0] = SHADER_CAPS_FRAGMENT_UNIFORMS;
        break;
    case GL_MAX_PROGRAM_TEXEL_OFFSET:
        params[0] = 7;
        break;
    case GL_MAX_RENDERBUFFER_SIZE:
        params[0] = 8192;
        break;
    case GL_MAX_SAMPLES:
        params[0] = 4;
        break;
    case GL_MAX_TEXTURE_IMAGE_UNITS:
        params[0] = TEXTURE_CAPS_PIXEL_SAMPLIERS;
        break;
    case GL_MAX_TEXTURE_LOD_BIAS:
        params[0] = 12;
        break;
    case GL_MAX_TEXTURE_SIZE:
        params[0] = 8192;
        break;
    case GL_MAX_TRANSFORM_FEEDBACK_INTERLEAVED_COMPONENTS:
        params[0] = 64;
        break;
    case GL_MAX_TRANSFORM_FEEDBACK_SEPARATE_ATTRIBS:
        params[0] = 4;
        break;
    case GL_MAX_TRANSFORM_FEEDBACK_SEPARATE_COMPONENTS:
        params[0] = 4;
        break;
    case GL_MAX_UNIFORM_BLOCK_SIZE:
        params[0] = 65535;
        break;
    case GL_MAX_UNIFORM_BUFFER_BINDINGS:
        params[0] = 16;
        break;
    case GL_MAX_VARYING_COMPONENTS:
        params[0] = SHADER_CAPS_VARYINGS * 4;
        break;
    case GL_MAX_VARYING_VECTORS:
        params[0] = SHADER_CAPS_VARYINGS;
        break;
    case GL_MAX_VERTEX_ATTRIBS:
        params[0] = STREAM_CAPS_MAX_ATTRIBUTES;
        break;
    case GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS:
        params[0] = TEXTURE_CAPS_VERTEX_SAMPLIERS;
        break;
    case GL_MAX_VERTEX_OUTPUT_COMPONENTS:
        params[0] = SHADER_CAPS_VARYINGS * 4;
        break;
    case GL_MAX_VERTEX_UNIFORM_BLOCKS:
        params[0] = 16;
        break;
    case GL_MAX_VERTEX_UNIFORM_COMPONENTS:
        params[0] = SHADER_CAPS_VERTEX_UNIFORMS * 4;
        break;
    case GL_MAX_VERTEX_UNIFORM_VECTORS:
        params[0] = SHADER_CAPS_VERTEX_UNIFORMS;
        break;
    case GL_MAX_VIEWPORT_DIMS:
        params[0] = 2048;
        break;
    case GL_MIN_PROGRAM_TEXEL_OFFSET:
        params[0] = -8;
        break;
    case GL_MINOR_VERSION:
        params[0] = 0;
        break;
    case GL_NUM_COMPRESSED_TEXTURE_FORMATS:
        params[0] = GetCompressedTexNum();
        break;
    case GL_NUM_EXTENSIONS:
        break;
        //  case GL_NUM_PROGRAM_BINARY_FORMATS:
        //params[0] = 0;
        //      break;
        //  case GL_NUM_SHADER_BINARY_FORMATS:
        //      params[0] = 0;
        //      break;
        //  case GL_PROGRAM_BINARY_FORMATS:
        //params[0] = 0;
        //      break;
        //case GL_SHADER_BINARY_FORMATS:
        //    /* VEmualtor does not support glShaderBinary. */
        //    params[0] = 0;
        //    /* params[0] = GL_PLATFORM_BINARY == 0 ? GL_FALSE: GL_TRUE; */
        //    break;
        //case GL_SHADER_COMPILER:
        //    params[0] = 1;
        //    break;
        //case GL_SAMPLE_ALPHA_TO_COVERAGE:
        //    break;
        //case GL_SAMPLE_COVERAGE:
        //    break;

        //case GL_TRANSFORM_FEEDBACK_BINDING:
        //    break;
    case GL_TRANSFORM_FEEDBACK_ACTIVE:
        params[0] = (CURRENT_GL_CONTEXT()->ApicurrentTFBState == eActive)? GL_TRUE : GL_FALSE;
        break;
    case GL_TRANSFORM_FEEDBACK_PAUSED:
        params[0] = (CURRENT_GL_CONTEXT()->ApicurrentTFBState == ePause)? GL_TRUE : GL_FALSE;
        break;
        //case GL_VERTEX_ARRAY_BINDING:
        //       break;
        //   case GL_COPY_READ_BUFFER_BINDING:
        //       break;
        //   case GL_COPY_WRITE_BUFFER_BINDING:
        //       break;
        //   case GL_CULL_FACE_MODE:
        //       break;
        //   case GL_FRONT_FACE:
        //       break;
        //   case GL_GENERATE_MIPMAP_HINT:
        //       break;
    default:
        g_opengl->glGetInteger64v(pname, params);
        break;
    }
    CURRENT_GL_CONTEXT()->Api;
}

GL_APICALL void GL_APIENTRY
glGetSynciv(
            GLsync sync,
            GLenum pname,
            GLsizei bufSize,
            GLsizei* length,
            GLint* values
            )
{
    g_opengl->glGetSynciv(sync, pname, bufSize, length, values);
    CURRENT_GL_CONTEXT()->Api;
}

GL_APICALL void GL_APIENTRY
glGetInteger64i_v(
                  GLenum target,
                  GLuint index,
                  GLint64* data
                  )
{
    CGLES3Context* thread = _GetThreadDataES3();
    //TODO
    if (data == NULL)
    {
        apiERROR(GL_INVALID_VALUE);
        return;
    }

    g_opengl->glGetInteger64i_v(target, index, data);
    CURRENT_GL_CONTEXT()->Api;
}

GL_APICALL void GL_APIENTRY
glGetBufferParameteri64v(
                         GLenum target,
                         GLenum pname,
                         GLint64* params
                         )
{
    CGLES3Context* thread = _GetThreadDataES3();
    if((pname == GL_BUFFER_ACCESS_FLAGS) || (pname == GL_BUFFER_MAPPED) || (pname == GL_BUFFER_USAGE))
    {
        apiERROR(GL_INVALID_ENUM);
        return;
    }
    if(pname == GL_BUFFER_MAP_LENGTH)
    {
        //TODO
    }
    else if(pname == GL_BUFFER_MAP_OFFSET)
    {
        //TODO
    }
    else
    {
        //g_opengl->glGetBufferParameteri64v(target, pname, params);
        //CURRENT_GL_CONTEXT()->Api;
    }
    g_opengl->glGetBufferParameteri64v(target, pname, params);
    CURRENT_GL_CONTEXT()->Api;
}

GL_APICALL void GL_APIENTRY
glGenSamplers(
              GLsizei count,
              GLuint* samplers
              )
{
    g_opengl->glGenSamplers(count, samplers);

    CURRENT_GL_CONTEXT()->Api;
}

GL_APICALL void GL_APIENTRY
glDeleteSamplers(
                 GLsizei count,
                 const GLuint* samplers
                 )
{
    g_opengl->glDeleteSamplers(count, samplers);

    CURRENT_GL_CONTEXT()->Api;
}

GL_APICALL GLboolean GL_APIENTRY
glIsSampler(
            GLuint sampler
            )
{
    GLboolean result;
    result = g_opengl->glIsSampler(sampler);

    CURRENT_GL_CONTEXT()->Api;
    return result;
}

GL_APICALL void GL_APIENTRY
glBindSampler(
              GLuint unit,
              GLuint sampler
              )
{
    g_opengl->glBindSampler(unit, sampler);

    CURRENT_GL_CONTEXT()->Api;
}

GL_APICALL void GL_APIENTRY
glSamplerParameteri(
                    GLuint sampler,
                    GLenum pname,
                    GLint param
                    )
{
    g_opengl->glSamplerParameteri(sampler, pname, param);
    CURRENT_GL_CONTEXT()->Api;
}

GL_APICALL void GL_APIENTRY
glSamplerParameteriv(
                     GLuint sampler,
                     GLenum pname,
                     const GLint* param
                     )
{
    g_opengl->glSamplerParameteriv(sampler, pname, param);
    CURRENT_GL_CONTEXT()->Api;
}

GL_APICALL void GL_APIENTRY
glSamplerParameterf(
                    GLuint sampler,
                    GLenum pname,
                    GLfloat param
                    )
{
    g_opengl->glSamplerParameterf(sampler, pname, param);
    CURRENT_GL_CONTEXT()->Api;
}

GL_APICALL void GL_APIENTRY
glSamplerParameterfv(
                     GLuint sampler,
                     GLenum pname,
                     const GLfloat* param
                     )
{
    g_opengl->glSamplerParameterfv(sampler, pname, param);
    CURRENT_GL_CONTEXT()->Api;
}

GL_APICALL void GL_APIENTRY
glGetSamplerParameteriv(
                        GLuint sampler,
                        GLenum pname,
                        GLint* params
                        )
{
    g_opengl->glGetSamplerParameteriv(sampler, pname, params);
    CURRENT_GL_CONTEXT()->Api;
}

GL_APICALL void GL_APIENTRY
glGetSamplerParameterfv(
                        GLuint sampler,
                        GLenum pname,
                        GLfloat* params
                        )
{
    g_opengl->glGetSamplerParameterfv(sampler, pname, params);
    CURRENT_GL_CONTEXT()->Api;
}

GL_APICALL void GL_APIENTRY
glVertexAttribDivisor(
                      GLuint index,
                      GLuint divisor
                      )
{
    g_opengl->glVertexAttribDivisor(index, divisor);

    CURRENT_GL_CONTEXT()->Api;
}

GL_APICALL void GL_APIENTRY
glBindTransformFeedback(
                        GLenum target,
                        GLuint id
                        )
{
    g_opengl->glBindTransformFeedback(target, id);
    CURRENT_GL_CONTEXT()->Api;
}

GL_APICALL void GL_APIENTRY
glDeleteTransformFeedbacks(
                           GLsizei n,
                           const GLuint* ids
                           )
{
    g_opengl->glDeleteTransformFeedbacks(n, ids);
    CURRENT_GL_CONTEXT()->Api;
}

GL_APICALL void GL_APIENTRY
glGenTransformFeedbacks(
                        GLsizei n,
                        GLuint* ids
                        )
{
    g_opengl->glGenTransformFeedbacks(n, ids);
    CURRENT_GL_CONTEXT()->Api;
}

GL_APICALL GLboolean GL_APIENTRY
glIsTransformFeedback(
                      GLuint id
                      )
{
    GLboolean result = GL_FALSE;
    result = g_opengl->glIsTransformFeedback(id);
    CURRENT_GL_CONTEXT()->Api;
    return result;
}

GL_APICALL void GL_APIENTRY
glPauseTransformFeedback(
                         void
                         )
{
    CGLES3Context* thread = _GetThreadDataES3();
    g_opengl->glPauseTransformFeedback();
    CURRENT_GL_CONTEXT()->Api;
    if(CURRENT_GL_CONTEXT()->Apierror == GL_NO_ERROR)
    {
        CURRENT_GL_CONTEXT()->ApicurrentTFBState = ePause;
    }
}

GL_APICALL void GL_APIENTRY
glResumeTransformFeedback(
                          void
                          )
{
    CGLES3Context* thread = _GetThreadDataES3();
    g_opengl->glResumeTransformFeedback();
    CURRENT_GL_CONTEXT()->Api;
    if(CURRENT_GL_CONTEXT()->Apierror == GL_NO_ERROR)
    {
        CURRENT_GL_CONTEXT()->ApicurrentTFBState = eActive;
    }
}

GL_APICALL void GL_APIENTRY
glGetProgramBinary(
                   GLuint program,
                   GLsizei bufSize,
                   GLsizei* length,
                   GLenum* binaryFormat,
                   GLvoid* binary
                   )
{
    g_opengl->glGetProgramBinary(program, bufSize, length, binaryFormat, binary);
    CURRENT_GL_CONTEXT()->Api;
}

GL_APICALL void GL_APIENTRY
glProgramBinary(
                GLuint program,
                GLenum binaryFormat,
                const GLvoid* binary,
                GLsizei length
                )
{
    g_opengl->glProgramBinary(program, binaryFormat, binary, length);

    CURRENT_GL_CONTEXT()->Api;
}

GL_APICALL void GL_APIENTRY
glProgramParameteri(
                    GLuint program,
                    GLenum pname,
                    GLint value
                    )
{
    if(pname != GL_PROGRAM_BINARY_RETRIEVABLE_HINT)
    {
        apiERROR(GL_INVALID_ENUM);
        return;
    }
    if((value != GL_FALSE) && (value != GL_TRUE))
    {
        apiERROR(GL_INVALID_VALUE);
        return;
    }
    g_opengl->glProgramParameteri(program, pname, value);
    CURRENT_GL_CONTEXT()->Api;
}

GL_APICALL void GL_APIENTRY
glInvalidateFramebuffer(
                        GLenum target,
                        GLsizei numAttachments,
                        const GLenum* attachments
                        )
{
    if(target != GL_FRAMEBUFFER)
    {
        apiERROR(GL_INVALID_ENUM);
        return;
    }
    g_opengl->glInvalidateFramebuffer(target, numAttachments, attachments);
    CURRENT_GL_CONTEXT()->Api;
}

GL_APICALL void GL_APIENTRY
glInvalidateSubFramebuffer(
                           GLenum target,
                           GLsizei numAttachments,
                           const GLenum* attachments,
                           GLint x,
                           GLint y,
                           GLsizei width,
                           GLsizei height
                           )
{
    if(target != GL_FRAMEBUFFER)
    {
        apiERROR(GL_INVALID_ENUM);
        return;
    }
    g_opengl->glInvalidateSubFramebuffer(target, numAttachments, attachments, x, y, width, height);
    CURRENT_GL_CONTEXT()->Api;
}

GL_APICALL void GL_APIENTRY
glTexStorage2D(
               GLenum target,
               GLsizei levels,
               GLenum internalformat,
               GLsizei width,
               GLsizei height
               )
{
    CGLES3Context* thread = _GetThreadDataES3();
    Texture* tex;
    tex = CURRENT_GL_CONTEXT()->ApiboundTexture[CURRENT_GL_CONTEXT()->ApiactiveTexture - GL_TEXTURE0];
    if(tex == NULL)
    {
        apiERROR(GL_INVALID_OPERATION);
        return;
    }
    switch(target)
    {
    case GL_TEXTURE_2D:
    case GL_TEXTURE_CUBE_MAP:
        break;
    default:
        apiERROR(GL_INVALID_ENUM);
        return;
    }

    tex->internalformat = internalformat;
    g_opengl->glTexStorage2D(target, levels, internalformat, width, height);
    CURRENT_GL_CONTEXT()->Api;
}

GL_APICALL void GL_APIENTRY
glTexStorage3D(
               GLenum target,
               GLsizei levels,
               GLenum internalformat,
               GLsizei width,
               GLsizei height,
               GLsizei depth
               )
{
    CGLES3Context* thread = _GetThreadDataES3();
    Texture* tex;
    tex = CURRENT_GL_CONTEXT()->ApiboundTexture[CURRENT_GL_CONTEXT()->ApiactiveTexture - GL_TEXTURE0];
    if(tex == NULL)
    {
        apiERROR(GL_INVALID_OPERATION);
        return;
    }
    switch(target)
    {
    case GL_TEXTURE_3D:
    case GL_TEXTURE_2D_ARRAY:
        break;
    default:
        apiERROR(GL_INVALID_ENUM);
        return;
    }

    tex->internalformat = internalformat;
    g_opengl->glTexStorage3D(target, levels, internalformat, width, height, depth);
    CURRENT_GL_CONTEXT()->Api;
}

GL_APICALL void GL_APIENTRY
glGetInternalformativ(
                      GLenum target,
                      GLenum internalformat,
                      GLenum pname,
                      GLsizei bufSize,
                      GLint* params
                      )
{
    if(target != GL_RENDERBUFFER)
    {
        apiERROR(GL_INVALID_ENUM);
        return;
    }
    g_opengl->glGetInternalformativ(target, internalformat, pname, bufSize, params);
    CURRENT_GL_CONTEXT()->Api;
}


GL_APICALL GLvoid* GL_APIENTRY glMapBufferOES(GLenum target, GLenum access)
{
    GLvoid * ret = g_opengl->glMapBufferOES(target, access);
    CURRENT_GL_CONTEXT()->Api;
    return ret;
}


GL_APICALL GLboolean GL_APIENTRY glUnmapBufferOES(GLenum target)
{
    GLboolean ret = g_opengl->glUnmapBufferOES(target);
    CURRENT_GL_CONTEXT()->Api;
    return ret;
}

GL_APICALL void GL_APIENTRY glGetBufferPointervOES (GLenum target, GLenum pname, GLvoid** params)
{
    g_opengl->glGetBufferPointervOES(target, pname, params);
    CURRENT_GL_CONTEXT()->Api;
}
#endif