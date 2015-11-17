#ifndef __API_EGL_CONTEXT_H
#define __API_EGL_CONTEXT_H

#include <EGL/egl.h>
#include <EGL/eglext.h>
#include "ApiGLES3Context.h"

typedef enum _enSurfaceType
{
    SURFACE_TYPE_INVALID = 0,
    SURFACE_TYPE_PBUFFER = 1,
    SURFACE_TYPE_WINDOWSURFACE,
    SURFACE_TYPE_PIXMAP
} enSurfaceType;

// EGL_NONE 0x3038
typedef struct _enEglConfig
{
    EGLBoolean          bUsed;
    EGLint              addr;

    EGLint              eglRenderableType;  // EGL_RENDERABLE_TYPE          0x3040
    EGLint              redSize;            // EGL_RED_SIZE                 0x3024
    EGLint              greenSize;          // EGL_GREEN_SIZE               0x3023
    EGLint              blueSize;           // EGL_BLUE_SIZE                0x3022
    EGLint              alphaSize;          // EGL_ALPHA_SIZE               0x3021
    EGLint              depthSize;          // EGL_DEPTH_SIZE               0x3025
    EGLint              stencilSize;        // EGL_STENCIL_SIZE             0x3026
    EGLint              samples;            // EGL_SAMPLES                  0x3031
    EGLint              surfaceType;        // EGL_SURFACE_TYPE             0x3033

    EGLConfig           config;     // egl driver config.
} enEglConfig;

typedef struct _stEglSurface
{
    EGLBoolean          bUsed;
    enSurfaceType       type;
    EGLint              width;
    EGLint              height;
    EGLint              addr;
    char                name[32];
    enEglConfig         *config;

    EGLSurface          *surface;   // EGL driver surface
} stEglSurface;

class CEGLContext
{
public:
    CEGLContext();
    ~CEGLContext();

public:
    stEglSurface        *draw;
    stEglSurface        *read;
    EGLint              addr;
    char                name[32];

    EGLContext          context;    // EGL driver context

    CEGLContext         *next;

    CGLES3Context       gles3Context;
};

extern CEGLContext  *g_eglCtxStack;
extern stEglSurface *g_eglSurfStack;

#endif /* __API_EGL_CONTEXT_H */