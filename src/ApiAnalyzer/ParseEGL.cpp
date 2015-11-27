#include "MathAPI.h"
#include "ParseEGL.h"
#include "utils.h"
#include "bitmap.h"
#include "MemoryPool.h"
#include "TranslateGL.h"
#include "ParseGLES3.h"
#include "ApiEGLContext.h"
#include "GLESShare.h"
#include "egl.h"
#include "Analyzer.h"


GLvoid ParseBindAPI(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint   arg[2];

    if (sscanf(input, "${EGL eglBindAPI 0x%08X}", &arg[0]) == 1)
    {
        ANALYZER_FUNC2(BindAPI, arg[0], arg[1]);
    }
}

GLvoid ParseBindTexImage(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint   arg[4];

    if (sscanf(input,
               "${EGL eglBindTexImage 0x%08X 0x%08X 0x%08X}",
               &arg[0], &arg[1], &arg[2]) == 3)
    {
        ANALYZER_FUNC4(BindTexImage, (EGLDisplay)arg[0], (EGLSurface)arg[1], arg[2], arg[3]);
    }
}

GLvoid ParseCreateContext(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint   arg[5];

    if (sscanf(input,
               "${EGL eglCreateContext 0x%08X 0x%08X 0x%08X (0x%08X) := 0x%08X",
               &arg[0], &arg[1], &arg[2], &arg[3], &arg[4]) == 5)
    {
        EGLint *attrib_list = (EGLint*)ReadData(READ_GL_UINT, arg[3], 0);
        ANALYZER_FUNC5(CreateContext, (EGLDisplay)arg[0], (EGLConfig)arg[1], (EGLContext)arg[2], attrib_list, (EGLContext)arg[4]);
    }
}

GLvoid ParseChooseConfig(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint   arg[5];

    if (sscanf(input,
               "${EGL eglChooseConfig 0x%08X (0x%08X) (0x%08X) 0x%08X (0x%08X)",
               &arg[0], &arg[1], &arg[2], &arg[3], &arg[4]) == 5)
    {
        EGLint *attrib_list = (EGLint*)ReadData(READ_GL_UINT, arg[1], 0);
        ANALYZER_FUNC6(ChooseConfig, (EGLDisplay)arg[0], attrib_list, (EGLConfig*)arg[2], arg[3], (EGLint*)arg[4], EGL_TRUE);
    }
}

GLvoid ParseCreateImageKHR(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint   arg[6];

    if (sscanf(input,
               "${EGL eglCreateImageKHR 0x%08X 0x%08X 0x%08X 0x%08X (0x%08X) := 0x%08X",
               &arg[0], &arg[1], &arg[2], &arg[3], &arg[4], &arg[5]) == 6)
    {
        ANALYZER_FUNC6(CreateImageKHR, (EGLDisplay)arg[0], (EGLContext)arg[1], (EGLenum)arg[2], (EGLClientBuffer)arg[3], (EGLint*)arg[4], (EGLImageKHR)arg[5]);
    }
}

GLvoid ParseCreateSyncKHR(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint   arg[4];

    if (sscanf(input,
               "${EGL eglCreateSyncKHR 0x%08X 0x%08X (0x%08X) := 0x%08X",
               &arg[0], &arg[1], &arg[2], &arg[3]) == 4)
    {
        ANALYZER_FUNC4(CreateSyncKHR, (EGLDisplay)arg[0], arg[1], (EGLint*)arg[2], (EGLSyncKHR)arg[3]);
    }
}

GLvoid ParseClientWaitSyncKHR(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint   arg[5];
    U64     arg64;

    if (sscanf(input,
               "${EGL eglClientWaitSyncKHR 0x%08X 0x%08X 0x%08X 0x%16llX := 0x%08X}",
               &arg[0], &arg[1], &arg[2], &arg64, &arg[4]) == 5)
    {
        ANALYZER_FUNC5(ClientWaitSyncKHR, (EGLDisplay)arg[0], (EGLSyncKHR)arg[1], arg[2], arg64, arg[4]);
    }
}

GLvoid ParseCopyBuffers(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint   arg[3];

    if (sscanf(input,
               "${EGL eglCopyBuffers 0x%08X 0x%08X 0x%08X}",
               &arg[0], &arg[1], &arg[2]) == 3)
    {
        ANALYZER_FUNC4(CopyBuffers, (EGLDisplay)arg[0], (EGLSurface)arg[1], (EGLNativePixmapType)arg[2], EGL_TRUE);
    }
}

GLvoid ParseCreateWindowSurface(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint arg[7];

    if (sscanf(input,
               "${EGL eglCreateWindowSurface 0x%08X 0x%08X 0x%08X (0x%08X) := 0x%08X (%dx%d)",
               &arg[0], &arg[1], &arg[2], &arg[3], &arg[4], &arg[5], &arg[6]) == 7)
    {
        ANALYZER_FUNC5(CreateWindowSurface, (EGLDisplay)arg[0], (EGLConfig)arg[1], (EGLNativeWindowType)arg[2], (EGLint*)arg[3], (EGLSurface)arg[4]);
    }
}

GLvoid ParseCreatePbufferSurface(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint arg[7];

    if (sscanf(input,
               "${EGL eglCreatePbufferSurface 0x%08X 0x%08X (0x%08X) := 0x%08X (%dx%d)",
               &arg[0], &arg[1], &arg[2], &arg[3], &arg[4], &arg[5]) == 6)
    {
        ANALYZER_FUNC4(CreatePbufferSurface, (EGLDisplay)arg[0], (EGLConfig)arg[1], (EGLint*)arg[2], (EGLSurface)arg[3]);
    }
}

GLvoid ParseCreatePbufferFromClientBuffer(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint   arg[8];

    if (sscanf(input,
               "${EGL eglCreatePbufferFromClientBuffer 0x%08X 0x%08X 0x%08X 0x%08X (0x%08X) := 0x%08X (%dx%d)",
               &arg[0], &arg[1], &arg[2], &arg[3], &arg[4], &arg[5], &arg[6], &arg[7]) == 8)
    {
        ANALYZER_FUNC6(CreatePbufferFromClientBuffer, (EGLDisplay)arg[0], arg[1], (EGLClientBuffer)arg[2], (EGLConfig)arg[3], (EGLint*)arg[4], (EGLSurface)arg[5]);
    }
}

GLvoid ParseCreatePixmapSurface(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint   arg[7];

    if (sscanf(input,
               "${EGL eglCreatePixmapSurface 0x%08X 0x%08X 0x%08X (0x%08X) := 0x%08X (%dx%d)",
               &arg[0], &arg[1], &arg[2], &arg[3], &arg[4], &arg[5], &arg[6]) == 7)
    {
        ANALYZER_FUNC5(CreatePixmapSurface, (EGLDisplay)arg[0], (EGLConfig)arg[1], (EGLNativePixmapType)arg[2], (EGLint*)arg[3], (EGLSurface)arg[4]);
    }
}

GLvoid ParseDestroySurface(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint   arg[2];

    if (sscanf(input,
               "${EGL eglDestroySurface 0x%08X 0x%08X}",
               &arg[0], &arg[1]) == 2)
    {
        ANALYZER_FUNC3(DestroySurface, (EGLDisplay)arg[0], (EGLSurface)arg[1], arg[2]);
    }
}

GLvoid ParseDestroyContext(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint   arg[2];

    if (sscanf(input,
               "${EGL eglDestroyContext 0x%08X 0x%08X}",
               &arg[0], &arg[1]) == 2)
    {
        ANALYZER_FUNC3(DestroyContext, (EGLDisplay)arg[0], (EGLContext)arg[1], arg[2]);
    }
}

GLvoid ParseDestroyImageKHR(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint   arg[2];

    if (sscanf(input,
               "${EGL eglDestroyImageKHR 0x%08X 0x%08X}",
               &arg[0], &arg[1]) == 2)
    {
        ANALYZER_FUNC3(DestroyImageKHR, (EGLDisplay)arg[0], (EGLImageKHR)arg[1], arg[2]);
    }
}

GLvoid ParseDestroySyncKHR(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint   arg[2];

    if (sscanf(input,
               "${EGL eglDestroySyncKHR 0x%08X 0x%08X}",
               &arg[0], &arg[1]) == 2)
    {
        ANALYZER_FUNC3(DestroySyncKHR, (EGLDisplay)arg[0], (EGLSyncKHR)arg[1], arg[2]);
    }
}

GLvoid ParseGetConfigs(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint   arg[4];

    if (sscanf(input,
               "${EGL eglGetConfigs 0x%08X (0x%08X) 0x%08X (0x%08X)",
               &arg[0], &arg[1], &arg[2], &arg[3]) == 4)
    {
        ANALYZER_FUNC5(GetConfigs, (EGLDisplay)arg[0], (EGLConfig*)arg[1], arg[2], (EGLint*)arg[3], arg[4]);
    }
}

GLvoid ParseGetConfigAttrib(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint   arg[4];

    if (sscanf(input,
               "${EGL eglGetConfigAttrib 0x%08X 0x%08X 0x%08X := 0x%08X}",
               &arg[0], &arg[1], &arg[2], &arg[3]) == 4)
    {
        ANALYZER_FUNC5(GetConfigAttrib, (EGLDisplay)arg[0], (EGLConfig)arg[1], arg[2], (EGLint*)arg[3], arg[4]);
    }
}

GLvoid ParseEGLGetError(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint   arg[1];

    if (sscanf(input,
               "${EGL eglGetError := 0x%08X}",
               &arg[0]) == 1)
    {
        ANALYZER_FUNC1(EGLGetError, arg[0]);
    }
}

GLvoid ParseGetDisplay(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint   arg[2];

    if (sscanf(input,
               "${EGL eglGetDisplay 0x%08X := 0x%08X}",
               &arg[0], &arg[1]) == 2)
    {
        ANALYZER_FUNC2(GetDisplay, (EGLNativeDisplayType)arg[0], (EGLDisplay)arg[1]);
    }
}

GLvoid ParseGetCurrentContext(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint   arg[1];

    if (sscanf(input,
               "${EGL eglGetCurrentContext := 0x%08X}",
               &arg[0]) == 1)
    {
        ANALYZER_FUNC1(GetCurrentContext, (EGLContext)arg[0]);
    }
}

GLvoid ParseGetCurrentSurface(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint   arg[2];

    if (sscanf(input,
               "${EGL eglGetCurrentSurface 0x%08X := 0x%08X}",
               &arg[0], &arg[1]) == 2)
    {
        ANALYZER_FUNC2(GetCurrentSurface, arg[0], (EGLSurface)arg[1]);
    }
}

GLvoid ParseGetCurrentDisplay(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint   arg[1];

    if (sscanf(input,
               "${EGL eglGetCurrentDisplay := 0x%08X}",
               &arg[0]) == 1)
    {
        ANALYZER_FUNC1(GetCurrentDisplay, (EGLDisplay)arg[0]);
    }
}

GLvoid ParseGetSyncAttribKHR(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint   arg[4];

    if (sscanf(input,
               "${EGL eglGetSyncAttribKHR 0x%08X 0x%08X 0x%08X := 0x%08X}",
               &arg[0], &arg[1], &arg[2], &arg[3]) == 4)
    {
        ANALYZER_FUNC5(GetSyncAttribKHR, (EGLDisplay)arg[0], (EGLSyncKHR)arg[1], arg[2], (EGLint*)arg[3], EGL_TRUE);
    }
}

GLvoid ParseGetProcAddress(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint   arg[2];

    if (sscanf(input,
               "${EGL eglGetProcAddress (0x%08X) := 0x%08X",
               &arg[0], &arg[1]) == 2)
    {
        const GLchar* name = (const GLchar*)ReadData(READ_GL_UCHAR, arg[0], 0);
        ANALYZER_FUNC2(GetProcAddress, name, (void*)arg[1]);
    }
}

GLvoid ParseInitialize(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint   arg[1];

    if (sscanf(input,
               "${EGL eglInitialize 0x%08X}",
               &arg[0]) == 1)
    {
        ANALYZER_FUNC4(Initialize, (EGLDisplay)arg[0], NULL, NULL, EGL_TRUE);
    }
}

GLvoid ParseLockSurfaceKHR(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint   arg[3];

    if (sscanf(input,
               "${EGL eglLockSurfaceKHR 0x%08X 0x%08X (0x%08X)",
               &arg[0], &arg[1], &arg[2]) == 3)
    {
        ANALYZER_FUNC4(LockSurfaceKHR, (EGLDisplay)arg[0], (EGLSurface)arg[1], (EGLint*)arg[2], EGL_TRUE);
    }
}

GLvoid ParseMakeCurrent(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint   arg[4];

    if (sscanf(input,
               "${EGL eglMakeCurrent 0x%08X 0x%08X 0x%08X 0x%08X}",
               &arg[0], &arg[1], &arg[2], &arg[3]) == 4)
    {
        ANALYZER_FUNC5(MakeCurrent, (EGLDisplay)arg[0], (EGLSurface)arg[1], (EGLSurface)arg[2], (EGLContext)arg[3], EGL_TRUE);
    }
}

GLvoid ParseQueryAPI(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint   arg[1];

    if (sscanf(input,
               "${EGL eglQueryAPI := 0x%08X}",
               &arg[0]) == 1)
    {
        ANALYZER_FUNC1(QueryAPI, arg[0]);
    }
}

GLvoid ParseQueryString(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint   arg[3];

    if (sscanf(input,
               "${EGL eglQueryString 0x%08X 0x%08X := 0x%08X",
               &arg[0], &arg[1], &arg[2]) == 3)
    {
        GLchar *p  = (GLchar*)ReadData(READ_GL_UCHAR, arg[2], 0);
        ANALYZER_FUNC3(QueryString, (EGLDisplay)arg[0], arg[1], p);
    }
}

GLvoid ParseQueryContext(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint   arg[4];

    if (sscanf(input,
               "${EGL eglQueryContext 0x%08X 0x%08X 0x%08X := 0x%08X}",
               &arg[0], &arg[1], &arg[2], &arg[3]) == 4)
    {
        EGLint *attr = (EGLint*)ReadData(READ_GL_UINT, arg[3], 0);
        ANALYZER_FUNC5(QueryContext, (EGLDisplay)arg[0], (EGLContext)arg[1], arg[2], attr, EGL_TRUE);
    }
}

GLvoid ParseQuerySurface(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint   arg[4];

    if (sscanf(input,
               "${EGL eglQuerySurface 0x%08X 0x%08X 0x%08X := 0x%08X}",
               &arg[0], &arg[1], &arg[2], &arg[3]) == 4)
    {
        EGLint *attr = (EGLint*)ReadData(READ_GL_UINT, arg[3], 0);
        ANALYZER_FUNC5(QuerySurface, (EGLDisplay)arg[0], (EGLSurface)arg[1], arg[2], attr, EGL_TRUE);
    }
}

GLvoid ParseReleaseThread(GLchar *input, GLchar *output, GLint outputSize)
{
    if (strncmp(input,
                "${EGL eglReleaseThread}",
                23) == 0)
    {
        ANALYZER_FUNC1(ReleaseThread, EGL_TRUE);
    }
}

GLvoid ParseReleaseTexImage(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint   arg[3];

    if (sscanf(input,
               "${EGL eglReleaseTexImage 0x%08X 0x%08X 0x%08X}",
               &arg[0], &arg[1], &arg[2]) == 3)
    {
        ANALYZER_FUNC4(ReleaseTexImage, (EGLDisplay)arg[0], (EGLSurface)arg[1], arg[2], EGL_TRUE);
    }
}

GLvoid ParseSwapBuffers(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint   arg[2];
    GLint   pos = 0;

    if (sscanf(input,
               "${EGL eglSwapBuffers 0x%08X 0x%08X}",
               &arg[0], &arg[1]) == 2)
    {
        ANALYZER_FUNC3(SwapBuffers, (EGLDisplay)arg[0], (EGLSurface)arg[1], EGL_TRUE);
    }
}

GLvoid ParseSwapBuffersRegionEXT(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint   arg[4];

    if (sscanf(input,
               "${EGL eglSwapBuffersRegionEXT 0x%08X 0x%08X 0x%08X (0x%08X)",
               &arg[0], &arg[1], &arg[2], &arg[3]) == 4)
    {
        sprintf(output, "eglSwapBuffersRegionEXT() ****todo\n");
    }
}

GLvoid ParseSwapRectangleANDROID(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint   arg[6];

    if (sscanf(input,
               "${EGL eglSwapRectangleANDROID 0x%08X 0x%08X 0x%08X 0x%08X 0x%08X 0x%08X}",
               &arg[0], &arg[1], &arg[2], &arg[3], &arg[4], &arg[5]) == 6)
    {
        sprintf(output, "eglSwapRectangleANDROID() ****todo\n");
    }
}

GLvoid ParseSwapInterval(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint   arg[2];

    if (sscanf(input,
               "${EGL eglSwapInterval 0x%08X 0x%08X}",
               &arg[0], &arg[1]) == 2)
    {
        ANALYZER_FUNC3(SwapInterval, (EGLDisplay)arg[0], arg[1], EGL_TRUE);
    }
}

GLvoid ParseSurfaceAttrib(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint   arg[4];

    if (sscanf(input,
               "${EGL eglSurfaceAttrib 0x%08X 0x%08X 0x%08X 0x%08X}",
               &arg[0], &arg[1], &arg[2], &arg[3]) == 4)
    {
        ANALYZER_FUNC5(SurfaceAttrib, (EGLDisplay)arg[0], (EGLSurface)arg[1], arg[2], arg[3], EGL_TRUE);
    }
}

GLvoid ParseTerminate(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint   arg[1];

    if (sscanf(input,
               "${EGL eglTerminate 0x%08X}",
               &arg[0]) == 1)
    {
        ANALYZER_FUNC2(Terminate, (EGLDisplay)arg[0], EGL_TRUE);
    }
}

GLvoid ParseUnlockSurfaceKHR(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint   arg[2];

    if (sscanf(input,
               "${EGL eglUnlockSurfaceKHR 0x%08X 0x%08X}",
               &arg[0], &arg[1]) == 2)
    {
        ANALYZER_FUNC3(UnlockSurfaceKHR, (EGLDisplay)arg[0], (EGLSurface)arg[1], EGL_TRUE);
    }
}

GLvoid ParseWaitNative(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint   arg[1];

    if (sscanf(input,
               "${EGL eglWaitNative 0x%08X}",
               &arg[0]) == 1)
    {
        ANALYZER_FUNC2(WaitNative, arg[0], EGL_TRUE);
    }
}

GLvoid ParseWaitClient(GLchar *input, GLchar *output, GLint outputSize)
{
    if (strncmp(input,
                "${EGL eglWaitClient}",
                20) == 0)
    {
        ANALYZER_FUNC1(WaitClient, EGL_TRUE);
    }
}