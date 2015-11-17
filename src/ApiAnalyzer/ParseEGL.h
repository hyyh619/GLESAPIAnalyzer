#ifndef __PARSE_EGL_H
#define __PARSE_EGL_H

#include <GLES3/gl3ext.h>
#include <GLES3/gl31.h>
#include <GLES2/gl2ext.h>
#include <GLES2/gl2.h>
#include <EGL/egl.h>

GLvoid ParseBindAPI(GLchar *input, GLchar *output, GLint outputSize);
GLvoid ParseBindTexImage(GLchar *input, GLchar *output, GLint outputSize);
GLvoid ParseCreateContext(GLchar *input, GLchar *output, GLint outputSize);
GLvoid ParseChooseConfig(GLchar *input, GLchar *output, GLint outputSize);
GLvoid ParseCreateImageKHR(GLchar *input, GLchar *output, GLint outputSize);
GLvoid ParseCreateSyncKHR(GLchar *input, GLchar *output, GLint outputSize);
GLvoid ParseClientWaitSyncKHR(GLchar *input, GLchar *output, GLint outputSize);
GLvoid ParseCopyBuffers(GLchar *input, GLchar *output, GLint outputSize);
GLvoid ParseCreateWindowSurface(GLchar *input, GLchar *output, GLint outputSize);
GLvoid ParseCreatePbufferSurface(GLchar *input, GLchar *output, GLint outputSize);
GLvoid ParseCreatePbufferFromClientBuffer(GLchar *input, GLchar *output, GLint outputSize);
GLvoid ParseCreatePixmapSurface(GLchar *input, GLchar *output, GLint outputSize);
GLvoid ParseDestroySurface(GLchar *input, GLchar *output, GLint outputSize);
GLvoid ParseDestroyContext(GLchar *input, GLchar *output, GLint outputSize);
GLvoid ParseDestroyImageKHR(GLchar *input, GLchar *output, GLint outputSize);
GLvoid ParseDestroySyncKHR(GLchar *input, GLchar *output, GLint outputSize);
GLvoid ParseGetConfigs(GLchar *input, GLchar *output, GLint outputSize);
GLvoid ParseGetConfigAttrib(GLchar *input, GLchar *output, GLint outputSize);
GLvoid ParseEGLGetError(GLchar *input, GLchar *output, GLint outputSize);
GLvoid ParseGetDisplay(GLchar *input, GLchar *output, GLint outputSize);
GLvoid ParseGetCurrentContext(GLchar *input, GLchar *output, GLint outputSize);
GLvoid ParseGetCurrentSurface(GLchar *input, GLchar *output, GLint outputSize);
GLvoid ParseGetCurrentDisplay(GLchar *input, GLchar *output, GLint outputSize);
GLvoid ParseGetSyncAttribKHR(GLchar *input, GLchar *output, GLint outputSize);
GLvoid ParseGetProcAddress(GLchar *input, GLchar *output, GLint outputSize);
GLvoid ParseInitialize(GLchar *input, GLchar *output, GLint outputSize);
GLvoid ParseLockSurfaceKHR(GLchar *input, GLchar *output, GLint outputSize);
GLvoid ParseMakeCurrent(GLchar *input, GLchar *output, GLint outputSize);
GLvoid ParseQueryAPI(GLchar *input, GLchar *output, GLint outputSize);
GLvoid ParseQueryString(GLchar *input, GLchar *output, GLint outputSize);
GLvoid ParseQueryContext(GLchar *input, GLchar *output, GLint outputSize);
GLvoid ParseQuerySurface(GLchar *input, GLchar *output, GLint outputSize);
GLvoid ParseReleaseThread(GLchar *input, GLchar *output, GLint outputSize);
GLvoid ParseReleaseTexImage(GLchar *input, GLchar *output, GLint outputSize);
GLvoid ParseSwapBuffers(GLchar *input, GLchar *output, GLint outputSize);
GLvoid ParseSwapBuffersRegionEXT(GLchar *input, GLchar *output, GLint outputSize);
GLvoid ParseSwapRectangleANDROID(GLchar *input, GLchar *output, GLint outputSize);
GLvoid ParseSwapInterval(GLchar *input, GLchar *output, GLint outputSize);
GLvoid ParseSurfaceAttrib(GLchar *input, GLchar *output, GLint outputSize);
GLvoid ParseTerminate(GLchar *input, GLchar *output, GLint outputSize);
GLvoid ParseUnlockSurfaceKHR(GLchar *input, GLchar *output, GLint outputSize);
GLvoid ParseWaitNative(GLchar *input, GLchar *output, GLint outputSize);
GLvoid ParseWaitClient(GLchar *input, GLchar *output, GLint outputSize);

#endif /* __PARSE_EGL_H */