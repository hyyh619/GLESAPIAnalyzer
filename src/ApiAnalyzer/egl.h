#ifndef _EGL_H
#define _EGL_H

#define EGL_EGLEXT_PROTOTYPES
#include <EGL/egl.h>
#include <EGL/eglext.h>
#include "gles3.h"

#define TMP_BUF_SIZE        256

/* EGL_ANDROID_native_fence_sync
 */
#ifndef EGL_ANDROID_native_fence_sync
#define EGL_ANDROID_native_fence_sync 1
#define EGL_SYNC_NATIVE_FENCE_ANDROID                 0x3144
#define EGL_SYNC_NATIVE_FENCE_FD_ANDROID              0x3145
#define EGL_NO_NATIVE_FENCE_FD_ANDROID                -1
#define EGL_SYNC_NATIVE_FENCE_SIGNALED_ANDROID        0x3146
#ifdef EGL_EGLEXT_PROTOTYPES
EGLAPI EGLint EGLAPIENTRY eglDupNativeFenceFDANDROID(EGLDisplay dpy, EGLSyncKHR sync);
#endif
typedef EGLint (EGLAPIENTRYP PFNEGLDUPNATIVEFENCEFDANDROID) (EGLDisplay dpy, EGLSyncKHR sync);
#endif

GLboolean   eglDisassemble(GLchar *input, GLchar *output, GLint outputSize);
GLvoid      TranslateEglError(GLint error, GLchar *output);
GLvoid      TranslateEGLEnum(EGLenum value, GLchar *output);
GLvoid      TranslateEGLAttrib(EGLenum attr, GLchar *output);
GLvoid      EGLInit();
GLvoid      EGLRelease();
GLvoid      RunEglGetDisplay(GLint arg[10]);


typedef struct _EGL_CONFIG_MAP
{
    EGLConfig                       apiConfig;
    EGLConfig                       emulatorConfig;
    EGLint                          index;
} EGL_CONFIG_MAP;

extern GLchar                       tmp1[TMP_BUF_SIZE];
extern EGLDisplay                   eglDisplay;
extern EGLContext                   eglContext;
extern EGLSurface                   eglSurface;

EGL_CONFIG* eglGetEGLConfig(EGL_CONFIG *configs, EGLint configNum, EGLint addr);
EGL_CONFIG* eglGetFreeEGLConfig(EGL_CONFIG *configs, EGLint *configNum);
stEglContext* eglGetEGLContext(stEglContext *contexts, EGLint contextNum, EGLint addr);
stEglContext* eglGetFreeEGLContext(stEglContext *contexts, EGLint *contextNum);
stEglSurface* eglGetSurfaceObject(stEglSurface *surfaceArray, EGLint surfaceCount, EGLint addr);
stEglSurface* eglGetFreeSurfaceObject(stEglSurface *surfaceArray, EGLint *surfaceCount);

#endif