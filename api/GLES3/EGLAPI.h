#ifndef __EGL_API_H
#define __EGL_API_H

#include <EGL/egl.h>
#include <EGL/eglext.h>

#ifdef WIN32
#include <windows.h>
#endif

#include "ApiOESEngine.h"

#define API_DUMMY                   (void*)0xDEADBEEF
#define EGL_DISPLAY_SIGNATURE       apiCC('E','G','L','D')
#define EGL_SURFACE_SIGNATURE       apiCC('E','G','L','S')
#define EGL_CONTEXT_SIGNATURE       apiCC('E','G','L','C')
#define EGL_IMAGE_SIGNATURE         apiCC('E','G','L','I')
#define EGL_SYNC_SIGNATURE          apiCC('E','G','L','Y')

#define COUNTOF(a) \
( \
    sizeof(a) / sizeof(a[0]) \
)

typedef struct _veglLOOKUP
{
    const char *                                name;
    __eglMustCastToProperFunctionPointerType    function;
}
veglLOOKUP;

#define eglMAKE_LOOKUP(function) \
    { #function, (__eglMustCastToProperFunctionPointerType) function }

typedef struct eglThreadData*   PEGLThreadData;
typedef struct eglDisplay*      PEGLDisplay;
typedef struct eglContext*      PEGLContext;
typedef struct eglSurface*      PEGLSurface;
typedef struct eglConfig *      PEGLConfig;
typedef struct eglImageKHR*     PEGLImageKHR;
typedef struct eglSync*         PEGLSync;

struct eglDisplay
{
    /* Signature. */
    GLuint                  signature;

    /* Next EGLDisplay. */
    PEGLDisplay             next;

    /* Handle to device context.
     * hdc is always vdkDisplay.
     */
    NativeDisplayType       hdc;
    GLboolean               defaultDpy;

    /* Process handle. */
    void *                  process;

    /* Number of configurations. */
    EGLint                  configCount;

    /* Pointer to configurations. */
    PEGLConfig              config;

    /* Allocated resources. */
    PEGLSurface             surfaceStack;
    PEGLContext             contextStack;
    PEGLImageKHR            imageStack;

    /* Reference counter. */
    GLuint                  reference;

    /* real display */
    EGLDisplay              display;
};

struct eglConfig
{
    EGLint                  bufferSize;
    EGLint                  alphaSize;
    EGLint                  blueSize;
    EGLint                  greenSize;
    EGLint                  redSize;
    EGLint                  depthSize;
    EGLint                  stencilSize;
    EGLenum                 configCaveat;
    EGLint                  configId;
    EGLBoolean              nativeRenderable;
    EGLint                  nativeVisualType;
    EGLint                  samples;
    EGLint                  sampleBuffers;
    EGLenum                 surfaceType;
    EGLBoolean              bindToTetxureRGB;
    EGLBoolean              bindToTetxureRGBA;
    EGLint                  luminanceSize;
    EGLint                  alphaMaskSize;
    EGLenum                 colorBufferType;
    EGLenum                 renderableType;
    EGLint                  width;
    EGLint                  height;

    EGLConfig               config;
};

struct eglSurface
{
    /* Signature. */
    GLuint                  signature;

    /* Next EGLSurface. */
    PEGLSurface             next;

    /* EGLConfig */
    struct eglConfig        config;

    EGLint                  type;

    /* Native type Handle. */
    HANDLE                  handle;

    /* Native type's DC. */
    HDC                     hdc;

    /* PBuffer attributes. */
    EGLBoolean              largestPBuffer;
    EGLBoolean              mipmapTexture;
    EGLint                  mipmapLevel;
    EGLenum                 textureFormat;
    EGLenum                 textureTarget;

    EGLBoolean              bBounded;

    EGLSurface              surface;
};

struct eglImageKHR
{
    /* Signature. */
    GLuint                  signature;

    /* Internal image struct pointer. */
    khrEGL_IMAGE            image;

    /* Next eglImage. */
    PEGLImageKHR            next;

    EGLImageKHR             eglImage;
};

struct eglContext
{
    /* Signature. */
    GLuint                  signature;

    /* Next EGLContext. */
    PEGLContext             next;

    /* Bounded thread. */
    PEGLThreadData          thread;

    /* Bounded API. */
    EGLenum                 api;
    EGLint                  client;

    /* Attached display. */
    PEGLDisplay             display;

    /* Shared configuration. */
    PEGLContext             sharedContext;

    /* Current read/draw surface */
    PEGLSurface             read;
    PEGLSurface             draw;

    /* Context for Client API. */
    void *                  glContext;

    /* OpenGL's Render Context, since it is 
       bound to HDC.
     */
    HGLRC                   renderContext;

    EGLContext              context;
};

/* Thread Data */
struct eglThreadData
{
    /* Last known error code. */
    EGLenum                 error;

    /* Current API. */
    EGLenum                 api;
    EGLint                  clientVersion;

    /* Hardware capabilities. */
    EGLint                  maxWidth;
    EGLint                  maxHeight;
    EGLint                  maxSamples;

    /* Current context. */
    PEGLContext             context;
    EGLint                  lastClient;

    /* Allocated resources. */
    EGLBoolean              destroying;
};

#endif /* __EGL_API_H */