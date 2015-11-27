#include "APIOS.h"
#include "ApiOESEngine.h"

#pragma warning (disable: 4127)

stEngine    *g_pOES3    = NULL;
stPrivacy   *g_pPrivacy = NULL;

static int
LoadEngine(
    IN stEngine *Eng,
    IN stPrivacy *Pri
    );


static GLboolean APIENTRY
isExtSupport(
    const char* Name
    );


GLvoid*
GetFuncAddress(
    int client,
    const GLchar *proc
    )
{
    GLvoid *func = NULL;

    switch (client)
    {
    case 3:
    case 2:
        func = apiOS_Dlresolve(g_pPrivacy->moduleGLES3, proc);
        break;

    case 1:
        func = apiOS_Dlresolve(g_pPrivacy->moduleGLES1, proc);
        break;

    default:
        func = apiOS_Dlresolve(g_pPrivacy->moduleGLES1, proc);
        break;
    }

    return func;
}

static void ApiLoadLibrary()
{
    if (g_pPrivacy == NULL)
        return;

    g_pPrivacy->moduleEGL   = apiOS_Dlopen("libEGL.dll");
    g_pPrivacy->moduleGLES1 = apiOS_Dlopen("libGLESv1.dll");
    g_pPrivacy->moduleGLES3 = apiOS_Dlopen("libGLESv3.dll");
}

static void ApiDestroyLibrary()
{
    if (g_pPrivacy == NULL)
        return;

    apiOS_Dlclose(g_pPrivacy->moduleEGL);
    g_pPrivacy->moduleEGL = NULL;

    apiOS_Dlclose(g_pPrivacy->moduleGLES1);
    g_pPrivacy->moduleGLES1 = NULL;

    apiOS_Dlclose(g_pPrivacy->moduleGLES3);
    g_pPrivacy->moduleGLES3 = NULL;
}

/*******************************************************************************
**
**    Reference desktop OpenGL Engine.
**    Thread Unsafe.
**
**    INPUT
**        Nothing
**
**    OUTPUT
**        ponter to desktop OpenGL Engine structure.
*/
const stEngine*
ConstructOpenGLEngine(
    void
    )
{
    /* Already loaded. Force Singlton */
    if (g_pOES3 != NULL)
    {
        g_pPrivacy->reference ++;
        return g_pOES3;
    }

    do
    {
        g_pOES3 = (stEngine*)malloc(sizeof(stEngine));
        if (g_pOES3 == NULL)
        {
            break;
        }

        memset(g_pOES3, 0, sizeof (stEngine));

        g_pPrivacy = (stPrivacy*)malloc(sizeof(stPrivacy));
        if (g_pPrivacy == NULL)
        {
            break;
        }

        memset(g_pPrivacy, 0, sizeof (stPrivacy));

        g_pPrivacy->reference = 1;

        ApiLoadLibrary();

        if (!LoadEngine(g_pOES3, g_pPrivacy))
        {
            ApiDestroyLibrary();
            break;
        }

        return g_pOES3;
    }
    while (0);

    /* Roll back. */
    if (g_pOES3 != NULL)
    {
        free(g_pOES3);
    }

    if (g_pPrivacy != NULL)
    {
        free(g_pPrivacy);
    }

    return NULL;
}

/*******************************************************************************
**
**    De-Reference desktop OpenGL Engine.
**
**    INPUT
**        Pointer to desktop OpenGL Engine structure.
**
**    OUTPUT
**        Nothing.
*/
void
DestroyOpenGLEngine(
    IN const stEngine *Eng
    )
{
    if (Eng == NULL)
    {
        return;
    }

    /* Should be equal to opengl structure & privacy is initialized. */
    assert(Eng == g_pOES3 && g_pPrivacy != NULL);

    g_pPrivacy->reference --;

    if (g_pPrivacy->reference == 0)
    {
        ApiDestroyLibrary();
        free(g_pPrivacy);
        g_pPrivacy = NULL;

        /* free engine */
        free(g_pOES3);
        g_pOES3 = NULL;
    }
}

static int
LoadEngine(
    IN stEngine *Eng,
    IN stPrivacy *Pri
    )
{
    do
    {
        /* EGL functions. */
        Eng->eglGetProcAddress          = (PFNEGLGETPROCADDRESS)apiOS_Dlresolve(g_pPrivacy->moduleEGL, "eglGetProcAddress");
        Eng->eglChooseConfig            = (PFNEGLCHOOSECONFIG)apiOS_Dlresolve(g_pPrivacy->moduleEGL, "eglChooseConfig");
        Eng->eglCopyBuffers             = (PFNEGLCOPYBUFFERS)apiOS_Dlresolve(g_pPrivacy->moduleEGL, "eglCopyBuffers");
        Eng->eglCreateContext           = (PFNEGLCREATECONTEXT)apiOS_Dlresolve(g_pPrivacy->moduleEGL, "eglCreateContext");
        Eng->eglCreatePbufferSurface    = (PFNEGLCREATEPBUFFERSURFACE)apiOS_Dlresolve(g_pPrivacy->moduleEGL, "eglCreatePbufferSurface");
        Eng->eglCreatePixmapSurface     = (PFNEGLCREATEPIXMAPSURFACE)apiOS_Dlresolve(g_pPrivacy->moduleEGL, "eglCreatePixmapSurface");
        Eng->eglCreateWindowSurface     = (PFNEGLCREATEWINDOWSURFACE)apiOS_Dlresolve(g_pPrivacy->moduleEGL, "eglCreateWindowSurface");

        Eng->eglDestroyContext          = (PFNEGLDESTROYCONTEXT)apiOS_Dlresolve(g_pPrivacy->moduleEGL, "eglDestroyContext");
        Eng->eglDestroySurface          = (PFNEGLDESTROYSURFACE)apiOS_Dlresolve(g_pPrivacy->moduleEGL, "eglDestroySurface");
        Eng->eglGetConfigAttrib         = (PFNEGLGETCONFIGATTRIB)apiOS_Dlresolve(g_pPrivacy->moduleEGL, "eglGetConfigAttrib");
        Eng->eglGetConfigs              = (PFNEGLGETCONFIGS)apiOS_Dlresolve(g_pPrivacy->moduleEGL, "eglGetConfigs");
        Eng->eglGetCurrentDisplay       = (PFNEGLGETCURRENTDISPLAY)apiOS_Dlresolve(g_pPrivacy->moduleEGL, "eglGetCurrentDisplay");

        Eng->eglGetCurrentSurface       = (PFNEGLGETCURRENTSURFACE)apiOS_Dlresolve(g_pPrivacy->moduleEGL, "eglGetCurrentSurface");
        Eng->eglGetDisplay              = (PFNEGLGETDISPLAY)apiOS_Dlresolve(g_pPrivacy->moduleEGL, "eglGetDisplay");
        Eng->eglGetError                = (PFNEGLGETERROR)apiOS_Dlresolve(g_pPrivacy->moduleEGL, "eglGetError");
        Eng->eglSetFIFO                 = (PFNEGLSETFIFO)apiOS_Dlresolve(g_pPrivacy->moduleEGL, "eglSetFIFO");
        Eng->eglInitialize              = (PFNEGLINITIALIZE)apiOS_Dlresolve(g_pPrivacy->moduleEGL, "eglInitialize");

        Eng->eglMakeCurrent             = (PFNEGLMAKECURRENT)apiOS_Dlresolve(g_pPrivacy->moduleEGL, "eglMakeCurrent");
        Eng->eglQueryContext            = (PFNEGLQUERYCONTEXT)apiOS_Dlresolve(g_pPrivacy->moduleEGL, "eglQueryContext");
        Eng->eglQueryString             = (PFNEGLQUERYSTRING)apiOS_Dlresolve(g_pPrivacy->moduleEGL, "eglQueryString");
        Eng->eglQuerySurface            = (PFNEGLQUERYSURFACE)apiOS_Dlresolve(g_pPrivacy->moduleEGL, "eglQuerySurface");
        Eng->eglSwapBuffers             = (PFNEGLSWAPBUFFERS)apiOS_Dlresolve(g_pPrivacy->moduleEGL, "eglSwapBuffers");

        Eng->eglTerminate               = (PFNEGLTERMINATE)apiOS_Dlresolve(g_pPrivacy->moduleEGL, "eglTerminate");
        Eng->eglWaitGL                  = (PFNEGLWAITGL)apiOS_Dlresolve(g_pPrivacy->moduleEGL, "eglWaitGL");
        Eng->eglWaitNative              = (PFNEGLWAITNATIVE)apiOS_Dlresolve(g_pPrivacy->moduleEGL, "eglWaitNative");
        Eng->eglBindTexImage            = (PFNEGLBINDTEXIMAGE)apiOS_Dlresolve(g_pPrivacy->moduleEGL, "eglBindTexImage");
        Eng->eglReleaseTexImage         = (PFNEGLRELEASETEXIMAGE)apiOS_Dlresolve(g_pPrivacy->moduleEGL, "eglReleaseTexImage");

        Eng->eglSurfaceAttrib           = (PFNEGLSURFACEATTRIB)apiOS_Dlresolve(g_pPrivacy->moduleEGL, "eglSurfaceAttrib");
        Eng->eglSwapInterval            = (PFNEGLSWAPINTERVAL)apiOS_Dlresolve(g_pPrivacy->moduleEGL, "eglSwapInterval");
        Eng->eglBindAPI                 = (PFNEGLBINDAPI)apiOS_Dlresolve(g_pPrivacy->moduleEGL, "eglBindAPI");
        Eng->eglQueryAPI                = (PFNEGLQUERYAPI)apiOS_Dlresolve(g_pPrivacy->moduleEGL, "eglQueryAPI");
        Eng->eglCreatePbufferFromClientBuffer = (PFNEGLCREATEPBUFFERFROMCLIENTBUFFER)apiOS_Dlresolve(g_pPrivacy->moduleEGL, "eglCreatePbufferFromClientBuffer");

        Eng->eglReleaseThread           = (PFNEGLRELEASETHREAD)apiOS_Dlresolve(g_pPrivacy->moduleEGL, "eglReleaseThread");
        Eng->eglWaitClient              = (PFNEGLWAITCLIENT)apiOS_Dlresolve(g_pPrivacy->moduleEGL, "eglWaitClient");
        Eng->eglGetCurrentContext       = (PFNEGLGETCURRENTCONTEXT)apiOS_Dlresolve(g_pPrivacy->moduleEGL, "eglGetCurrentContext");
        Eng->eglSetFIFO                 = (PFNEGLSETFIFO)apiOS_Dlresolve(g_pPrivacy->moduleEGL, "eglSetFIFO");

        /* glGetString */
        Eng->glGetString = (PFNGLGETSTRING)apiOS_Dlresolve(g_pPrivacy->moduleGLES3, "glGetString");
        // Pri->extensions = (char*)Eng->glGetString(GL_EXTENSIONS);

        // ES 2.0
        Eng->glClearColor = (PFNGLCLEARCOLOR) apiOS_Dlresolve(g_pPrivacy->moduleGLES3, "glClearColor");
        Eng->glClearDepthf = (PFNGLCLEARDEPTH) apiOS_Dlresolve(g_pPrivacy->moduleGLES3, "glClearDepthf");
        Eng->glDepthRangef = (PFNGLDEPTHRANGE) apiOS_Dlresolve(g_pPrivacy->moduleGLES3, "glDepthRangef");
        Eng->glGetFloatv = (PFNGLGETFLOATV) apiOS_Dlresolve(g_pPrivacy->moduleGLES3, "glGetFloatv");
        Eng->glGetTexParameterfv = (PFNGLGETTEXPARAMETERFV) apiOS_Dlresolve(g_pPrivacy->moduleGLES3, "glGetTexParameterfv");
        Eng->glLineWidth = (PFNGLLINEWIDTH) apiOS_Dlresolve(g_pPrivacy->moduleGLES3, "glLineWidth");
        Eng->glPolygonOffset = (PFNGLPOLYGONOFFSET) apiOS_Dlresolve(g_pPrivacy->moduleGLES3, "glPolygonOffset");
        Eng->glTexParameterf = (PFNGLTEXPARAMETERF) apiOS_Dlresolve(g_pPrivacy->moduleGLES3, "glTexParameterf");
        Eng->glTexParameterfv = (PFNGLTEXPARAMETERFV) apiOS_Dlresolve(g_pPrivacy->moduleGLES3, "glTexParameterfv");
        Eng->glBindTexture = (PFNGLBINDTEXTURE) apiOS_Dlresolve(g_pPrivacy->moduleGLES3, "glBindTexture");
        Eng->glBlendFunc = (PFNGLBLENDFUNC) apiOS_Dlresolve(g_pPrivacy->moduleGLES3, "glBlendFunc");
        Eng->glClear = (PFNGLCLEAR) apiOS_Dlresolve(g_pPrivacy->moduleGLES3, "glClear");
        Eng->glClearStencil = (PFNGLCLEARSTENCIL) apiOS_Dlresolve(g_pPrivacy->moduleGLES3, "glClearStencil");
        Eng->glColorMask = (PFNGLCOLORMASK) apiOS_Dlresolve(g_pPrivacy->moduleGLES3, "glColorMask");
        Eng->glCopyTexImage2D = (PFNGLCOPYTEXIMAGE2D) apiOS_Dlresolve(g_pPrivacy->moduleGLES3, "glCopyTexImage2D");
        Eng->glCopyTexSubImage2D = (PFNGLCOPYTEXSUBIMAGE2D) apiOS_Dlresolve(g_pPrivacy->moduleGLES3, "glCopyTexSubImage2D");
        Eng->glCullFace = (PFNGLCULLFACE) apiOS_Dlresolve(g_pPrivacy->moduleGLES3, "glCullFace");
        Eng->glDeleteTextures = (PFNGLDELETETEXTURES) apiOS_Dlresolve(g_pPrivacy->moduleGLES3, "glDeleteTextures");
        Eng->glDepthFunc = (PFNGLDEPTHFUNC) apiOS_Dlresolve(g_pPrivacy->moduleGLES3, "glDepthFunc");
        Eng->glDepthMask = (PFNGLDEPTHMASK) apiOS_Dlresolve(g_pPrivacy->moduleGLES3, "glDepthMask");
        Eng->glDisable = (PFNGLDISABLE) apiOS_Dlresolve(g_pPrivacy->moduleGLES3, "glDisable");
        Eng->glDrawArrays = (PFNGLDRAWARRAYS) apiOS_Dlresolve(g_pPrivacy->moduleGLES3, "glDrawArrays");
        Eng->glDrawElements = (PFNGLDRAWELEMENTS) apiOS_Dlresolve(g_pPrivacy->moduleGLES3, "glDrawElements");
        Eng->glEnable = (PFNGLENABLE) apiOS_Dlresolve(g_pPrivacy->moduleGLES3, "glEnable");
        Eng->glFinish = (PFNGLFINISH) apiOS_Dlresolve(g_pPrivacy->moduleGLES3, "glFinish");
        Eng->glFlush = (PFNGLFLUSH) apiOS_Dlresolve(g_pPrivacy->moduleGLES3, "glFlush");
        Eng->glFrontFace = (PFNGLFRONTFACE) apiOS_Dlresolve(g_pPrivacy->moduleGLES3, "glFrontFace");
        Eng->glGetBooleanv = (PFNGLGETBOOLEANV) apiOS_Dlresolve(g_pPrivacy->moduleGLES3, "glGetBooleanv");
        Eng->glGenTextures = (PFNGLGENTEXTURES) apiOS_Dlresolve(g_pPrivacy->moduleGLES3, "glGenTextures");
        Eng->glGetError = (PFNGLGETERROR) apiOS_Dlresolve(g_pPrivacy->moduleGLES3, "glGetError");
        Eng->glGetIntegerv = (PFNGLGETINTEGERV) apiOS_Dlresolve(g_pPrivacy->moduleGLES3, "glGetIntegerv");
        Eng->glGetTexParameteriv = (PFNGLGETTEXPARAMETERIV) apiOS_Dlresolve(g_pPrivacy->moduleGLES3, "glGetTexParameteriv");
        Eng->glHint = (PFNGLHINT) apiOS_Dlresolve(g_pPrivacy->moduleGLES3, "glHint");
        Eng->glIsEnabled = (PFNGLISENABLED) apiOS_Dlresolve(g_pPrivacy->moduleGLES3, "glIsEnabled");
        Eng->glIsTexture = (PFNGLISTEXTURE) apiOS_Dlresolve(g_pPrivacy->moduleGLES3, "glIsTexture");
        Eng->glPixelStorei = (PFNGLPIXELSTOREI) apiOS_Dlresolve(g_pPrivacy->moduleGLES3, "glPixelStorei");
        Eng->glReadPixels = (PFNGLREADPIXELS) apiOS_Dlresolve(g_pPrivacy->moduleGLES3, "glReadPixels");
        Eng->glScissor = (PFNGLSCISSOR) apiOS_Dlresolve(g_pPrivacy->moduleGLES3, "glScissor");
        Eng->glStencilFunc = (PFNGLSTENCILFUNC) apiOS_Dlresolve(g_pPrivacy->moduleGLES3, "glStencilFunc");
        Eng->glStencilMask = (PFNGLSTENCILMASK) apiOS_Dlresolve(g_pPrivacy->moduleGLES3, "glStencilMask");
        Eng->glStencilOp = (PFNGLSTENCILOP) apiOS_Dlresolve(g_pPrivacy->moduleGLES3, "glStencilOp");
        Eng->glTexImage2D = (PFNGLTEXIMAGE2D) apiOS_Dlresolve(g_pPrivacy->moduleGLES3, "glTexImage2D");
        Eng->glTexParameteri = (PFNGLTEXPARAMETERI) apiOS_Dlresolve(g_pPrivacy->moduleGLES3, "glTexParameteri");
        Eng->glTexParameteriv = (PFNGLTEXPARAMETERIV) apiOS_Dlresolve(g_pPrivacy->moduleGLES3, "glTexParameteriv");
        Eng->glTexSubImage2D = (PFNGLTEXSUBIMAGE2D) apiOS_Dlresolve(g_pPrivacy->moduleGLES3, "glTexSubImage2D");
        Eng->glViewport = (PFNGLVIEWPORT) apiOS_Dlresolve(g_pPrivacy->moduleGLES3, "glViewport");
        Eng->glActiveTexture = (PFNGLACTIVETEXTURE)apiOS_Dlresolve(g_pPrivacy->moduleGLES3, "glActiveTexture");
        Eng->glGenBuffers = (PFNGLGENBUFFERS)apiOS_Dlresolve(g_pPrivacy->moduleGLES3, "glGenBuffers");
        Eng->glBindBuffer = (PFNGLBINDBUFFER)apiOS_Dlresolve(g_pPrivacy->moduleGLES3, "glBindBuffer");
        Eng->glBufferData = (PFNGLBUFFERDATA)apiOS_Dlresolve(g_pPrivacy->moduleGLES3, "glBufferData");
        Eng->glDeleteBuffers = (PFNGLDELETEBUFFERS)apiOS_Dlresolve(g_pPrivacy->moduleGLES3, "glDeleteBuffers");
        Eng->glIsBuffer = (PFNGLISBUFFER)apiOS_Dlresolve(g_pPrivacy->moduleGLES3, "glIsBuffer");
        Eng->glBufferSubData = (PFNGLBUFFERSUBDATA)apiOS_Dlresolve(g_pPrivacy->moduleGLES3, "glBufferSubData");
        Eng->glGetBufferParameteriv = (PFNGLGETBUFFERPARAMETERIV)apiOS_Dlresolve(g_pPrivacy->moduleGLES3, "glGetBufferParameteriv");
        Eng->glTexImage3D = (PFNGLTEXIMAGE3D)apiOS_Dlresolve(g_pPrivacy->moduleGLES3, "glTexImage3D");
        Eng->glTexSubImage3D = (PFNGLTEXSUBIMAGE3D)apiOS_Dlresolve(g_pPrivacy->moduleGLES3, "glTexSubImage3D");
        Eng->glCopyTexSubImage3D = (PFNGLCOPYTEXSUBIMAGE3D)apiOS_Dlresolve(g_pPrivacy->moduleGLES3, "glCopyTexSubImage3D");
        Eng->glFramebufferTexture3D = (PFNGLFRAMEBUFFERTEXTURE3D)apiOS_Dlresolve(g_pPrivacy->moduleGLES3, "glFramebufferTexture3D");
        Eng->glCompressedTexImage2D = (PFNGLCOMPRESSEDTEXIMAGE2D)apiOS_Dlresolve(g_pPrivacy->moduleGLES3, "glCompressedTexImage2D");
        Eng->glCompressedTexSubImage2D = (PFNGLCOMPRESSEDTEXSUBIMAGE2D)apiOS_Dlresolve(g_pPrivacy->moduleGLES3, "glCompressedTexSubImage2D");
        Eng->glCompressedTexImage3D = (PFNGLCOMPRESSEDTEXIMAGE3D)apiOS_Dlresolve(g_pPrivacy->moduleGLES3, "glCompressedTexImage3D");
        Eng->glCompressedTexSubImage3D = (PFNGLCOMPRESSEDTEXSUBIMAGE3D)apiOS_Dlresolve(g_pPrivacy->moduleGLES3, "glCompressedTexSubImage3D");
        Eng->glFenceSync = (PFNFENCESYNC)apiOS_Dlresolve(g_pPrivacy->moduleGLES3, "glFenceSync");
        Eng->glDeleteSync = (PFNDELETESYNC)apiOS_Dlresolve(g_pPrivacy->moduleGLES3, "glDeleteSync");
        Eng->glClientWaitSync = (PFNCLIENTWAITSYNC)apiOS_Dlresolve(g_pPrivacy->moduleGLES3, "glClientWaitSync");
        Eng->glGetSynciv = (PFNGETSYNCIV)apiOS_Dlresolve(g_pPrivacy->moduleGLES3, "glGetSynciv");
        Eng->glIsSync = (PFNGLISSYNC)apiOS_Dlresolve(g_pPrivacy->moduleGLES3, "glIsSync");
        Eng->glSampleCoverage = (PFNGLSAMPLECOVERAGE)apiOS_Dlresolve(g_pPrivacy->moduleGLES3, "glSampleCoverage");

        /* OpenGL v2.0 */
        Eng->glAttachShader = (PFNGLATTACHSHADER)apiOS_Dlresolve(g_pPrivacy->moduleGLES3, "glAttachShader");
        Eng->glCompileShader = (PFNGLCOMPILESHADER)apiOS_Dlresolve(g_pPrivacy->moduleGLES3, "glCompileShader");
        Eng->glCreateProgram = (PFNGLCREATEPROGRAM)apiOS_Dlresolve(g_pPrivacy->moduleGLES3, "glCreateProgram");
        Eng->glCreateShader = (PFNGLCREATESHADER)apiOS_Dlresolve(g_pPrivacy->moduleGLES3, "glCreateShader");
        Eng->glDeleteProgram = (PFNGLDELETEPROGRAM)apiOS_Dlresolve(g_pPrivacy->moduleGLES3, "glDeleteProgram");
        Eng->glDeleteShader = (PFNGLDELETESHADER)apiOS_Dlresolve(g_pPrivacy->moduleGLES3, "glDeleteShader");
        Eng->glDetachShader = (PFNGLDETACHSHADER)apiOS_Dlresolve(g_pPrivacy->moduleGLES3, "glDetachShader");
        Eng->glGetActiveUniform = (PFNGLGETACTIVEUNIFORM)apiOS_Dlresolve(g_pPrivacy->moduleGLES3, "glGetActiveUniform");
        Eng->glGetAttachedShaders = (PFNGLGETATTACHEDSHADERS)apiOS_Dlresolve(g_pPrivacy->moduleGLES3, "glGetAttachedShaders");
        Eng->glLinkProgram = (PFNGLLINKPROGRAM)apiOS_Dlresolve(g_pPrivacy->moduleGLES3, "glLinkProgram");
        Eng->glGetShaderSource = (PFNGLGETSHADERSOURCE)apiOS_Dlresolve(g_pPrivacy->moduleGLES3, "glGetShaderSource");
        Eng->glGetUniformfv = (PFNGLGETUNIFORMFV)apiOS_Dlresolve(g_pPrivacy->moduleGLES3, "glGetUniformfv");
        Eng->glGetUniformiv = (PFNGLGETUNIFORMIV)apiOS_Dlresolve(g_pPrivacy->moduleGLES3, "glGetUniformiv");
        Eng->glGetUniformLocation = (PFNGLGETUNIFORMLOCATION)apiOS_Dlresolve(g_pPrivacy->moduleGLES3, "glGetUniformLocation");
        Eng->glUniform1f = (PFNGLUNIFORM1F)apiOS_Dlresolve(g_pPrivacy->moduleGLES3, "glUniform1f");
        Eng->glUniform1fv = (PFNGLUNIFORM1FV)apiOS_Dlresolve(g_pPrivacy->moduleGLES3, "glUniform1fv");
        Eng->glUniform1i = (PFNGLUNIFORM1I)apiOS_Dlresolve(g_pPrivacy->moduleGLES3, "glUniform1i");
        Eng->glUniform1iv = (PFNGLUNIFORM1IV)apiOS_Dlresolve(g_pPrivacy->moduleGLES3, "glUniform1iv");
        Eng->glUniform2f = (PFNGLUNIFORM2F)apiOS_Dlresolve(g_pPrivacy->moduleGLES3, "glUniform2f");
        Eng->glUniform2fv = (PFNGLUNIFORM2FV)apiOS_Dlresolve(g_pPrivacy->moduleGLES3, "glUniform2fv");
        Eng->glUniform2i = (PFNGLUNIFORM2I)apiOS_Dlresolve(g_pPrivacy->moduleGLES3, "glUniform2i");
        Eng->glUniform2iv = (PFNGLUNIFORM2IV)apiOS_Dlresolve(g_pPrivacy->moduleGLES3, "glUniform2iv");
        Eng->glUniform3f = (PFNGLUNIFORM3F)apiOS_Dlresolve(g_pPrivacy->moduleGLES3, "glUniform3f");
        Eng->glUniform3fv = (PFNGLUNIFORM3FV)apiOS_Dlresolve(g_pPrivacy->moduleGLES3, "glUniform3fv");
        Eng->glUniform3i = (PFNGLUNIFORM3I)apiOS_Dlresolve(g_pPrivacy->moduleGLES3, "glUniform3i");
        Eng->glUniform3iv = (PFNGLUNIFORM3IV)apiOS_Dlresolve(g_pPrivacy->moduleGLES3, "glUniform3iv");
        Eng->glUniform4f = (PFNGLUNIFORM4F)apiOS_Dlresolve(g_pPrivacy->moduleGLES3, "glUniform4f");
        Eng->glUniform4fv = (PFNGLUNIFORM4FV)apiOS_Dlresolve(g_pPrivacy->moduleGLES3, "glUniform4fv");
        Eng->glUniform4i = (PFNGLUNIFORM4I)apiOS_Dlresolve(g_pPrivacy->moduleGLES3, "glUniform4i");
        Eng->glUniform4iv = (PFNGLUNIFORM4IV)apiOS_Dlresolve(g_pPrivacy->moduleGLES3, "glUniform4iv");
        Eng->glUniformMatrix2fv = (PFNGLUNIFORMMATRIX2FV)apiOS_Dlresolve(g_pPrivacy->moduleGLES3, "glUniformMatrix2fv");
        Eng->glUniformMatrix3fv = (PFNGLUNIFORMMATRIX3FV)apiOS_Dlresolve(g_pPrivacy->moduleGLES3, "glUniformMatrix3fv");
        Eng->glUniformMatrix4fv = (PFNGLUNIFORMMATRIX4FV)apiOS_Dlresolve(g_pPrivacy->moduleGLES3, "glUniformMatrix4fv");
        Eng->glUseProgram = (PFNGLUSEPROGRAM)apiOS_Dlresolve(g_pPrivacy->moduleGLES3, "glUseProgram");
        Eng->glValidateProgram = (PFNGLVALIDATEPROGRAM)apiOS_Dlresolve(g_pPrivacy->moduleGLES3, "glValidateProgram");
        Eng->glShaderSource = (PFNGLSHADERSOURCE)apiOS_Dlresolve(g_pPrivacy->moduleGLES3, "glShaderSource");
        Eng->glGetShaderiv = (PFNGLGETSHADERIV)apiOS_Dlresolve(g_pPrivacy->moduleGLES3, "glGetShaderiv");

        Eng->glGetProgramiv = (PFNGLGETPROGRAMIV)apiOS_Dlresolve(g_pPrivacy->moduleGLES3, "glGetProgramiv");
        Eng->glEnableVertexAttribArray = (PFNGLENABLEVERTEXATTRIBARRAY)apiOS_Dlresolve(g_pPrivacy->moduleGLES3, "glEnableVertexAttribArray");
        Eng->glVertexAttribPointer = (PFNGLVERTEXATTRIBPOINTER)apiOS_Dlresolve(g_pPrivacy->moduleGLES3, "glVertexAttribPointer");
        Eng->glDisableVertexAttribArray = (PFNGLDISABLEVERTEXATTRIBARRAY)apiOS_Dlresolve(g_pPrivacy->moduleGLES3, "glDisableVertexAttribArray");
        Eng->glVertexAttrib1f = (PFNGLVERTEXATTRIB1F)apiOS_Dlresolve(g_pPrivacy->moduleGLES3, "glVertexAttrib1f");
        Eng->glVertexAttrib1fv = (PFNGLVERTEXATTRIB1FV)apiOS_Dlresolve(g_pPrivacy->moduleGLES3, "glVertexAttrib1fv");
        Eng->glVertexAttrib2f = (PFNGLVERTEXATTRIB2F)apiOS_Dlresolve(g_pPrivacy->moduleGLES3, "glVertexAttrib2f");
        Eng->glVertexAttrib2fv = (PFNGLVERTEXATTRIB2FV)apiOS_Dlresolve(g_pPrivacy->moduleGLES3, "glVertexAttrib2fv");
        Eng->glVertexAttrib3f = (PFNGLVERTEXATTRIB3F)apiOS_Dlresolve(g_pPrivacy->moduleGLES3, "glVertexAttrib3f");
        Eng->glVertexAttrib3fv = (PFNGLVERTEXATTRIB3FV)apiOS_Dlresolve(g_pPrivacy->moduleGLES3, "glVertexAttrib3fv");
        Eng->glVertexAttrib4f = (PFNGLVERTEXATTRIB4F)apiOS_Dlresolve(g_pPrivacy->moduleGLES3, "glVertexAttrib4f");
        Eng->glVertexAttrib4fv = (PFNGLVERTEXATTRIB4FV)apiOS_Dlresolve(g_pPrivacy->moduleGLES3, "glVertexAttrib4fv");
        Eng->glGetVertexAttribfv = (PFNGLGETVERTEXATTRIBFV)apiOS_Dlresolve(g_pPrivacy->moduleGLES3, "glGetVertexAttribfv");
        Eng->glGetVertexAttribiv = (PFNGLGETVERTEXATTRIBIV)apiOS_Dlresolve(g_pPrivacy->moduleGLES3, "glGetVertexAttribiv");
        Eng->glGetVertexAttribPointerv = (PFNGLGETVERTEXATTRIBPOINTERV)apiOS_Dlresolve(g_pPrivacy->moduleGLES3, "glGetVertexAttribPointerv");

        Eng->glGetAttribLocation = (PFNGLGETATTRIBLOCATION)apiOS_Dlresolve(g_pPrivacy->moduleGLES3, "glGetAttribLocation");
        Eng->glGetActiveAttrib = (PFNGLGETACTIVEATTRIB)apiOS_Dlresolve(g_pPrivacy->moduleGLES3, "glGetActiveAttrib");
        Eng->glBindAttribLocation = (PFNGLBINDATTRIBLOCATION)apiOS_Dlresolve(g_pPrivacy->moduleGLES3, "glBindAttribLocation");
        Eng->glIsProgram = (PFNGLISPROGRAM)apiOS_Dlresolve(g_pPrivacy->moduleGLES3, "glIsProgram");
        Eng->glGetProgramBinary = (PFNGLGETPROGRAMBINARY)apiOS_Dlresolve(g_pPrivacy->moduleGLES3, "glGetProgramBinary");
        Eng->glProgramBinary = (PFNGLPROGRAMBINARY)apiOS_Dlresolve(g_pPrivacy->moduleGLES3, "glProgramBinary");

        Eng->glBindFramebuffer = (PFNGLBINDFRAMEBUFFER)apiOS_Dlresolve(g_pPrivacy->moduleGLES3, "glBindFramebuffer");
        Eng->glBindRenderbuffer = (PFNGLBINDRENDERBUFFER)apiOS_Dlresolve(g_pPrivacy->moduleGLES3, "glBindRenderbuffer");
        Eng->glGenFramebuffers = (PFNGLGENFRAMEBUFFERS)apiOS_Dlresolve(g_pPrivacy->moduleGLES3, "glGenFramebuffers");
        Eng->glFramebufferRenderbuffer = (PFNGLFRAMEBUFFERRENDERBUFFER)apiOS_Dlresolve(g_pPrivacy->moduleGLES3, "glFramebufferRenderbuffer");
        Eng->glFramebufferTexture2D = (PFNGLFRAMEBUFFERTEXTURE2D)apiOS_Dlresolve(g_pPrivacy->moduleGLES3, "glFramebufferTexture2D");
        Eng->glCheckFramebufferStatus = (PFNGLCHECKFRAMEBUFFERSTATUS)apiOS_Dlresolve(g_pPrivacy->moduleGLES3, "glCheckFramebufferStatus");
        Eng->glDeleteFramebuffers = (PFNGLDELETEFRAMEBUFFERS)apiOS_Dlresolve(g_pPrivacy->moduleGLES3, "glDeleteFramebuffers");
        Eng->glDeleteRenderbuffers = (PFNGLDELETERENDERBUFFERS)apiOS_Dlresolve(g_pPrivacy->moduleGLES3, "glDeleteRenderbuffers");
        Eng->glGenRenderbuffers = (PFNGLGENRENDERBUFFERS)apiOS_Dlresolve(g_pPrivacy->moduleGLES3, "glGenRenderbuffers");
        Eng->glGetRenderbufferParameteriv = (PFNGLGETRENDERBUFFERPARAMETERIV)apiOS_Dlresolve(g_pPrivacy->moduleGLES3, "glGetRenderbufferParameteriv");
        Eng->glRenderbufferStorage = (PFNGLRENDERBUFFERSTORAGE)apiOS_Dlresolve(g_pPrivacy->moduleGLES3, "glRenderbufferStorage");
        Eng->glGetFramebufferAttachmentParameteriv = (PFNGLGETFRAMEBUFFERATTACHMENTPARAMETERIV)apiOS_Dlresolve(g_pPrivacy->moduleGLES3, "glGetFramebufferAttachmentParameteriv");
        Eng->glGenerateMipmap = (PFNGLGENERATEMIPMAP)apiOS_Dlresolve(g_pPrivacy->moduleGLES3, "glGenerateMipmap");
        Eng->glIsFramebuffer = (PFNGLISFRAMEBUFFER)apiOS_Dlresolve(g_pPrivacy->moduleGLES3, "glIsFramebuffer");
        Eng->glIsRenderbuffer = (PFNGLISRENDERBUFFER)apiOS_Dlresolve(g_pPrivacy->moduleGLES3, "glIsRenderbuffer");
        Eng->glBlendEquation = (PFNGLBLENDEQUATION)apiOS_Dlresolve(g_pPrivacy->moduleGLES3, "glBlendEquation");

        if (isExtSupport("GL_OES_get_program_binary"))
        {
            Eng->glGetProgramBinaryOES = (PFNGLGETPROGRAMBINARY)apiOS_Dlresolve(g_pPrivacy->moduleGLES3, "glGetProgramBinary");
            Eng->glProgramBinaryOES = (PFNGLPROGRAMBINARY)apiOS_Dlresolve(g_pPrivacy->moduleGLES3, "glProgramBinary");
        }

        Eng->glBindVertexArray = (PFNGLBINDVERTEXARRAY)apiOS_Dlresolve(g_pPrivacy->moduleGLES3, "glBindVertexArray");
        Eng->glDeleteVertexArrays = (PFNGLDELETEVERTEXARRAYS)apiOS_Dlresolve(g_pPrivacy->moduleGLES3, "glDeleteVertexArrays");
        Eng->glGenVertexArrays = (PFNGLGENVERTEXARRAYS)apiOS_Dlresolve(g_pPrivacy->moduleGLES3, "glGenVertexArrays");
        Eng->glIsVertexArray = (PFNGLISVERTEXARRAY)apiOS_Dlresolve(g_pPrivacy->moduleGLES3, "glIsVertexArray");

        if (isExtSupport("GL_EXT_framebuffer_blit"))
        {
            Eng->glBlitFramebufferEXT = (PFNGLBLITFRAMEBUFFEREXT)apiOS_Dlresolve(g_pPrivacy->moduleGLES3, "glBlitFramebufferEXT");
        }
        Eng->glBlitFramebuffer = (PFNGLBLITFRAMEBUFFEREXT)apiOS_Dlresolve(g_pPrivacy->moduleGLES3, "glBlitFramebuffer");

        if (isExtSupport("GL_EXT_framebuffer_multisample"))
        {
            Eng->glRenderbufferStorageMultisampleEXT = (PFNGLRENDERBUFFERSTORAGEMULTISAMPLEEXT)apiOS_Dlresolve(g_pPrivacy->moduleGLES3, "glRenderbufferStorageMultisampleEXT");
        }
        Eng->glRenderbufferStorageMultisample = (PFNGLRENDERBUFFERSTORAGEMULTISAMPLE)apiOS_Dlresolve(g_pPrivacy->moduleGLES3, "glRenderbufferStorageMultisample");

        /* Core 2.0 functions */
        Eng->glGetShaderInfoLog = (PFNGLGETSHADERINFOLOG)apiOS_Dlresolve(g_pPrivacy->moduleGLES3, "glGetShaderInfoLog");
        Eng->glGetProgramInfoLog = (PFNGLGETPROGRAMINFOLOG)apiOS_Dlresolve(g_pPrivacy->moduleGLES3, "glGetProgramInfoLog");
        Eng->glStencilMaskSeparate = (PFNGLSTENCILMASKSEPARATE)apiOS_Dlresolve(g_pPrivacy->moduleGLES3, "glStencilMaskSeparate");
        Eng->glStencilFuncSeparate = (PFNGLSTENCILFUNCSEPARATE)apiOS_Dlresolve(g_pPrivacy->moduleGLES3, "glStencilFuncSeparate");
        Eng->glStencilOpSeparate = (PFNGLSTENCILOPSEPARATE)apiOS_Dlresolve(g_pPrivacy->moduleGLES3, "glStencilOpSeparate");
        Eng->glBlendFuncSeparate = (PFNGLBLENDFUNCSEPARATE)apiOS_Dlresolve(g_pPrivacy->moduleGLES3, "glBlendFuncSeparate");
        Eng->glBlendEquationSeparate = (PFNGLBLENDEQUATIONSEPARATE)apiOS_Dlresolve(g_pPrivacy->moduleGLES3, "glBlendEquationSeparate");
        Eng->glBlendColor = (PFNGLBLENDCOLOR)apiOS_Dlresolve(g_pPrivacy->moduleGLES3, "glBlendColor");

        /* Not implemented yet */
        Eng->glIsShader = (PFNGLISSHADER)apiOS_Dlresolve(g_pPrivacy->moduleGLES3, "glIsShader");

        Eng->glReleaseShaderCompiler = (PFNGLRELEASESHADERCOMPILER)apiOS_Dlresolve(g_pPrivacy->moduleGLES3, "glReleaseShaderCompiler");
        Eng->glGetShaderPrecisionFormat = (PFNGLGETSHADERPRECISIONFORMAT)apiOS_Dlresolve(g_pPrivacy->moduleGLES3, "glGetShaderPrecisionFormat");
        Eng->glShaderBinary = (PFNGLSHADERBINARY)apiOS_Dlresolve(g_pPrivacy->moduleGLES3, "glShaderBinary");

        /* extra */
        Eng->glDrawBuffer = (PFNGLDRAWBUFFER) apiOS_Dlresolve(g_pPrivacy->moduleGLES3, "glDrawBuffer");

        /* isExtSupport */
        Eng->isExtSupport = &isExtSupport;

        /*OpenGL 3.3*/
        Eng->glReadBuffer = (PFNGLREADBUFFER)apiOS_Dlresolve(g_pPrivacy->moduleGLES3, "glReadBuffer");
        Eng->glDrawRangeElements = (PFNGLDRAWRANGEELEMENTS)apiOS_Dlresolve(g_pPrivacy->moduleGLES3, "glDrawRangeElements");
        Eng->glGenQueries = (PFNGLGENQUERIES)apiOS_Dlresolve(g_pPrivacy->moduleGLES3, "glGenQueries");
        Eng->glDeleteQueries = (PFNGLDELETEQUERIES)apiOS_Dlresolve(g_pPrivacy->moduleGLES3, "glDeleteQueries");
        Eng->glIsQuery = (PFNGLISQUERY)apiOS_Dlresolve(g_pPrivacy->moduleGLES3, "glIsQuery");
        Eng->glBeginQuery = (PFNGLBEGINQUERY)apiOS_Dlresolve(g_pPrivacy->moduleGLES3, "glBeginQuery");
        Eng->glEndQuery = (PFNGLENDQUERY)apiOS_Dlresolve(g_pPrivacy->moduleGLES3, "glEndQuery");
        Eng->glGetQueryiv = (PFNGLGETQUERYIV)apiOS_Dlresolve(g_pPrivacy->moduleGLES3, "glGetQueryiv");
        Eng->glGetQueryObjectuiv = (PFNGLGETQUERYOBJECTUIV)apiOS_Dlresolve(g_pPrivacy->moduleGLES3, "glGetQueryObjectuiv");
        Eng->glUnmapBuffer = (PFNGLUNMAPBUFFER)apiOS_Dlresolve(g_pPrivacy->moduleGLES3, "glUnmapBuffer");
        Eng->glGetBufferPointerv = (PFNGLGETBUFFERPOINTERV)apiOS_Dlresolve(g_pPrivacy->moduleGLES3, "glGetBufferPointerv");
        Eng->glDrawBuffers = (PFNGLDRAWBUFFERS)apiOS_Dlresolve(g_pPrivacy->moduleGLES3, "glDrawBuffers");
        Eng->glUniformMatrix2x3fv = (PFNGLUNIFORMMATRIX2X3FV)apiOS_Dlresolve(g_pPrivacy->moduleGLES3, "glUniformMatrix2x3fv");
        Eng->glUniformMatrix3x2fv = (PFNGLUNIFORMMATRIX3X2FV)apiOS_Dlresolve(g_pPrivacy->moduleGLES3, "glUniformMatrix3x2fv");
        Eng->glUniformMatrix2x4fv = (PFNGLUNIFORMMATRIX2X4FV)apiOS_Dlresolve(g_pPrivacy->moduleGLES3, "glUniformMatrix2x4fv");
        Eng->glUniformMatrix4x2fv = (PFNGLUNIFORMMATRIX4X2FV)apiOS_Dlresolve(g_pPrivacy->moduleGLES3, "glUniformMatrix4x2fv");
        Eng->glUniformMatrix3x4fv = (PFNGLUNIFORMMATRIX3X4FV)apiOS_Dlresolve(g_pPrivacy->moduleGLES3, "glUniformMatrix3x4fv");
        Eng->glUniformMatrix4x3fv = (PFNGLUNIFORMMATRIX4X3FV)apiOS_Dlresolve(g_pPrivacy->moduleGLES3, "glUniformMatrix4x3fv");
        Eng->glBlitFramebuffer = (PFNGLBLITFRAMEBUFFER)apiOS_Dlresolve(g_pPrivacy->moduleGLES3, "glBlitFramebuffer");
        Eng->glRenderbufferStorageMultisample = (PFNGLRENDERBUFFERSTORAGEMULTISAMPLE)apiOS_Dlresolve(g_pPrivacy->moduleGLES3, "glRenderbufferStorageMultisample");
        Eng->glFramebufferTextureLayer = (PFNGLFRAMEBUFFERTEXTURELAYER)apiOS_Dlresolve(g_pPrivacy->moduleGLES3, "glFramebufferTextureLayer");
        Eng->glMapBufferRange = (PFNGLMAPBUFFERRANGE)apiOS_Dlresolve(g_pPrivacy->moduleGLES3, "glMapBufferRange");
        Eng->glFlushMappedBufferRange = (PFNGLFLUSHMAPPEDBUFFERRANGE)apiOS_Dlresolve(g_pPrivacy->moduleGLES3, "glFlushMappedBufferRange");
        Eng->glGetIntegeri_v = (PFNGLGETINTEGERI_V)apiOS_Dlresolve(g_pPrivacy->moduleGLES3, "glGetIntegeri_v");
        Eng->glBeginTransformFeedback = (PFNGLBEGINTRANSFORMFEEDBACK)apiOS_Dlresolve(g_pPrivacy->moduleGLES3, "glBeginTransformFeedback");
        Eng->glEndTransformFeedback = (PFNGLENDTRANSFORMFEEDBACK)apiOS_Dlresolve(g_pPrivacy->moduleGLES3, "glEndTransformFeedback");
        Eng->glBindBufferRange = (PFNGLBINDBUFFERRANGE)apiOS_Dlresolve(g_pPrivacy->moduleGLES3, "glBindBufferRange");
        Eng->glBindBufferBase = (PFNGLBINDBUFFERBASE)apiOS_Dlresolve(g_pPrivacy->moduleGLES3, "glBindBufferBase");
        Eng->glTransformFeedbackVaryings = (PFNGLTRANSFORMFEEDBACKVARYINGS)apiOS_Dlresolve(g_pPrivacy->moduleGLES3, "glTransformFeedbackVaryings");
        Eng->glGetTransformFeedbackVarying = (PFNGLGETTRANSFORMFEEDBACKVARYING)apiOS_Dlresolve(g_pPrivacy->moduleGLES3, "glGetTransformFeedbackVarying");
        Eng->glVertexAttribIPointer = (PFNGLVERTEXATTRIBIPOINTER)apiOS_Dlresolve(g_pPrivacy->moduleGLES3, "glVertexAttribIPointer");
        Eng->glGetVertexAttribIiv = (PFNGLGETVERTEXATTRIBIIV)apiOS_Dlresolve(g_pPrivacy->moduleGLES3, "glGetVertexAttribIiv");
        Eng->glGetVertexAttribIuiv = (PFNGLGETVERTEXATTRIBIUIV)apiOS_Dlresolve(g_pPrivacy->moduleGLES3, "glGetVertexAttribIuiv");
        Eng->glVertexAttribI4i = (PFNGLVERTEXATTRIBI4I)apiOS_Dlresolve(g_pPrivacy->moduleGLES3, "glVertexAttribI4i");
        Eng->glVertexAttribI4ui = (PFNGLVERTEXATTRIBI4UI)apiOS_Dlresolve(g_pPrivacy->moduleGLES3, "glVertexAttribI4ui");
        Eng->glVertexAttribI4iv = (PFNGLVERTEXATTRIBI4IV)apiOS_Dlresolve(g_pPrivacy->moduleGLES3, "glVertexAttribI4iv");
        Eng->glVertexAttribI4uiv = (PFNGLVERTEXATTRIBI4UIV)apiOS_Dlresolve(g_pPrivacy->moduleGLES3, "glVertexAttribI4uiv");
        Eng->glGetUniformuiv = (PFNGLGETUNIFORMUIV)apiOS_Dlresolve(g_pPrivacy->moduleGLES3, "glGetUniformuiv");
        Eng->glGetFragDataLocation = (PFNGLGETFRAGDATALOCATION)apiOS_Dlresolve(g_pPrivacy->moduleGLES3, "glGetFragDataLocation");
        Eng->glUniform1ui = (PFNGLUNIFORM1UI)apiOS_Dlresolve(g_pPrivacy->moduleGLES3, "glUniform1ui");
        Eng->glUniform2ui = (PFNGLUNIFORM2UI)apiOS_Dlresolve(g_pPrivacy->moduleGLES3, "glUniform2ui");
        Eng->glUniform3ui = (PFNGLUNIFORM3UI)apiOS_Dlresolve(g_pPrivacy->moduleGLES3, "glUniform3ui");
        Eng->glUniform4ui = (PFNGLUNIFORM4UI)apiOS_Dlresolve(g_pPrivacy->moduleGLES3, "glUniform4ui");
        Eng->glUniform1uiv = (PFNGLUNIFORM1UIV)apiOS_Dlresolve(g_pPrivacy->moduleGLES3, "glUniform1uiv");
        Eng->glUniform2uiv = (PFNGLUNIFORM2UIV)apiOS_Dlresolve(g_pPrivacy->moduleGLES3, "glUniform2uiv");
        Eng->glUniform3uiv = (PFNGLUNIFORM3UIV)apiOS_Dlresolve(g_pPrivacy->moduleGLES3, "glUniform3uiv");
        Eng->glUniform4uiv = (PFNGLUNIFORM4UIV)apiOS_Dlresolve(g_pPrivacy->moduleGLES3, "glUniform4uiv");
        Eng->glClearBufferiv = (PFNGLCLEARBUFFERIV)apiOS_Dlresolve(g_pPrivacy->moduleGLES3, "glClearBufferiv");
        Eng->glClearBufferuiv = (PFNGLCLEARBUFFERUIV)apiOS_Dlresolve(g_pPrivacy->moduleGLES3, "glClearBufferuiv");
        Eng->glClearBufferfv = (PFNGLCLEARBUFFERFV)apiOS_Dlresolve(g_pPrivacy->moduleGLES3, "glClearBufferfv");
        Eng->glClearBufferfi = (PFNGLCLEARBUFFERFI)apiOS_Dlresolve(g_pPrivacy->moduleGLES3, "glClearBufferfi");
        Eng->glGetStringi = (PFNGLGETSTRINGI)apiOS_Dlresolve(g_pPrivacy->moduleGLES3, "glGetStringi");
        Eng->glCopyBufferSubData = (PFNGLCOPYBUFFERSUBDATA)apiOS_Dlresolve(g_pPrivacy->moduleGLES3, "glCopyBufferSubData");
        Eng->glGetUniformIndices = (PFNGLGETUNIFORMINDICES)apiOS_Dlresolve(g_pPrivacy->moduleGLES3, "glGetUniformIndices");
        Eng->glGetActiveUniformsiv = (PFNGLGETACTIVEUNIFORMSIV)apiOS_Dlresolve(g_pPrivacy->moduleGLES3, "glGetActiveUniformsiv");
        Eng->glGetUniformBlockIndex = (PFNGLGETUNIFORMBLOCKINDEX)apiOS_Dlresolve(g_pPrivacy->moduleGLES3, "glGetUniformBlockIndex");
        Eng->glGetActiveUniformBlockiv = (PFNGLGETACTIVEUNIFORMBLOCKIV)apiOS_Dlresolve(g_pPrivacy->moduleGLES3, "glGetActiveUniformBlockiv");
        Eng->glGetActiveUniformBlockName = (PFNGLGETACTIVEUNIFORMBLOCKNAME)apiOS_Dlresolve(g_pPrivacy->moduleGLES3, "glGetActiveUniformBlockName");
        Eng->glUniformBlockBinding = (PFNGLUNIFORMBLOCKBINDING)apiOS_Dlresolve(g_pPrivacy->moduleGLES3, "glUniformBlockBinding");
        Eng->glDrawArraysInstanced = (PFNGLDRAWARRAYSINSTANCED)apiOS_Dlresolve(g_pPrivacy->moduleGLES3, "glDrawArraysInstanced");
        Eng->glDrawElementsInstanced = (PFNGLDRAWELEMENTSINSTANCED)apiOS_Dlresolve(g_pPrivacy->moduleGLES3, "glDrawElementsInstanced");
        Eng->glWaitSync = (PFNGLWAITSYNC)apiOS_Dlresolve(g_pPrivacy->moduleGLES3, "glWaitSync");
        Eng->glGetInteger64v = (PFNGLGETINTEGER64V)apiOS_Dlresolve(g_pPrivacy->moduleGLES3, "glGetInteger64v");
        Eng->glGetInteger64i_v = (PFNGLGETINTEGER64I_V)apiOS_Dlresolve(g_pPrivacy->moduleGLES3, "glGetInteger64i_v");
        Eng->glGetBufferParameteri64v = (PFNGLGETBUFFERPARAMETERI64V)apiOS_Dlresolve(g_pPrivacy->moduleGLES3, "glGetBufferParameteri64v");
        Eng->glGenSamplers = (PFNGLGENSAMPLERS)apiOS_Dlresolve(g_pPrivacy->moduleGLES3, "glGenSamplers");
        Eng->glDeleteSamplers = (PFNGLDELETESAMPLERS)apiOS_Dlresolve(g_pPrivacy->moduleGLES3, "glDeleteSamplers");
        Eng->glIsSampler = (PFNGLISSAMPLER)apiOS_Dlresolve(g_pPrivacy->moduleGLES3, "glIsSampler");
        Eng->glBindSampler = (PFNGLBINDSAMPLER)apiOS_Dlresolve(g_pPrivacy->moduleGLES3, "glBindSampler");
        Eng->glSamplerParameteri = (PFNGLSAMPLERPARAMETERI)apiOS_Dlresolve(g_pPrivacy->moduleGLES3, "glSamplerParameteri");
        Eng->glSamplerParameteriv = (PFNGLSAMPLERPARAMETERIV)apiOS_Dlresolve(g_pPrivacy->moduleGLES3, "glSamplerParameteriv");
        Eng->glSamplerParameterf = (PFNGLSAMPLERPARAMETERF)apiOS_Dlresolve(g_pPrivacy->moduleGLES3, "glSamplerParameterf");
        Eng->glSamplerParameterfv = (PFNGLSAMPLERPARAMETERFV)apiOS_Dlresolve(g_pPrivacy->moduleGLES3, "glSamplerParameterfv");
        Eng->glGetSamplerParameteriv = (PFNGLGETSAMPLERPARAMETERIV)apiOS_Dlresolve(g_pPrivacy->moduleGLES3, "glGetSamplerParameteriv");
        Eng->glGetSamplerParameterfv = (PFNGLGETSAMPLERPARAMETERFV)apiOS_Dlresolve(g_pPrivacy->moduleGLES3, "glGetSamplerParameterfv");
        Eng->glVertexAttribDivisor = (PFNGLVERTEXATTRIBDIVISOR)apiOS_Dlresolve(g_pPrivacy->moduleGLES3, "glVertexAttribDivisor");
        Eng->glBindTransformFeedback = (PFNGLBINDTRANSFORMFEEDBACK)apiOS_Dlresolve(g_pPrivacy->moduleGLES3, "glBindTransformFeedback");
        Eng->glDeleteTransformFeedbacks = (PFNGLDELETETRANSFORMFEEDBACKS)apiOS_Dlresolve(g_pPrivacy->moduleGLES3, "glDeleteTransformFeedbacks");
        Eng->glGenTransformFeedbacks = (PFNGLGENTRANSFORMFEEDBACKS)apiOS_Dlresolve(g_pPrivacy->moduleGLES3, "glGenTransformFeedbacks");
        Eng->glIsTransformFeedback = (PFNGLISTRANSFORMFEEDBACK)apiOS_Dlresolve(g_pPrivacy->moduleGLES3, "glIsTransformFeedback");
        Eng->glPauseTransformFeedback = (PFNGLPAUSETRANSFORMFEEDBACK)apiOS_Dlresolve(g_pPrivacy->moduleGLES3, "glPauseTransformFeedback");
        Eng->glResumeTransformFeedback = (PFNGLRESUMETRANSFORMFEEDBACK)apiOS_Dlresolve(g_pPrivacy->moduleGLES3, "glResumeTransformFeedback");
        Eng->glProgramParameteri = (PFNGLPROGRAMPARAMETERI)apiOS_Dlresolve(g_pPrivacy->moduleGLES3, "glProgramParameteri");
        Eng->glInvalidateFramebuffer = (PFNGLINVALIDATEFRAMEBUFFER)apiOS_Dlresolve(g_pPrivacy->moduleGLES3, "glInvalidateFramebuffer");
        Eng->glInvalidateSubFramebuffer = (PFNGLINVALIDATESUBFRAMEBUFFER)apiOS_Dlresolve(g_pPrivacy->moduleGLES3, "glInvalidateSubFramebuffer");
        Eng->glTexStorage2D = (PFNGLTEXSTORAGE2D)apiOS_Dlresolve(g_pPrivacy->moduleGLES3, "glTexStorage2D");
        Eng->glTexStorage3D = (PFNGLTEXSTORAGE3D)apiOS_Dlresolve(g_pPrivacy->moduleGLES3, "glTexStorage3D");
        Eng->glGetInternalformativ = (PFNGLGETINTERNALFORMATIV)apiOS_Dlresolve(g_pPrivacy->moduleGLES3, "glGetInternalformativ");
        Eng->glGetBufferPointerv = (PFNGLGETBUFFERPOINTERVOESPROC)apiOS_Dlresolve(g_pPrivacy->moduleGLES3, "glGetBufferPointerv");

        return 1;
    }
    while (0);

    /* Roll back. */
    return 0;
}

GLboolean APIENTRY
isExtSupport(
    const char* Name
    )
{
    const char* extensionList = g_pPrivacy->extensions;

    if (extensionList != NULL)
    {
        while (*extensionList)
        {
            /* Find the length of the first extension substring */
            unsigned int firstExtensionLength = strcspn(extensionList, " ");

            if (strlen(Name) == firstExtensionLength &&
                strncmp(Name, extensionList, firstExtensionLength) == 0
                )
            {
                return 1;
            }

            /* Move to the next substring. */
            extensionList += firstExtensionLength + 1;
        }
    }

    return 0;
}