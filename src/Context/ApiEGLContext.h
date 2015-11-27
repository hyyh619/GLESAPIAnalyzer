#ifndef __API_EGL_CONTEXT_H
#define __API_EGL_CONTEXT_H

#include <EGL/egl.h>
#include <EGL/eglext.h>
#include "ApiOESEngine.h"
#include "ApiGLES3Context.h"

typedef enum _enSurfaceType
{
    SURFACE_TYPE_INVALID = 0,
    SURFACE_TYPE_PBUFFER = 1,
    SURFACE_TYPE_WINDOWSURFACE,
    SURFACE_TYPE_PIXMAP
} enSurfaceType;

// EGL_NONE 0x3038
typedef struct _stEglConfig
{
    EGLint              bufferSize;
    EGLint              alphaSize;          // EGL_ALPHA_SIZE               0x3021
    EGLint              blueSize;           // EGL_BLUE_SIZE                0x3022
    EGLint              greenSize;          // EGL_GREEN_SIZE               0x3023
    EGLint              redSize;            // EGL_RED_SIZE                 0x3024
    EGLint              depthSize;          // EGL_DEPTH_SIZE               0x3025
    EGLint              stencilSize;        // EGL_STENCIL_SIZE             0x3026
    EGLenum             configCaveat;
    EGLint              configId;
    EGLBoolean          nativeRenderable;
    EGLint              nativeVisualType;
    EGLint              samples;            // EGL_SAMPLES                  0x3031
    EGLint              sampleBuffers;
    EGLenum             surfaceType;        // EGL_SURFACE_TYPE             0x3033
    EGLBoolean          bindToTetxureRGB;
    EGLBoolean          bindToTetxureRGBA;
    EGLint              luminanceSize;
    EGLint              alphaMaskSize;
    EGLenum             colorBufferType;
    EGLenum             renderableType;     // EGL_RENDERABLE_TYPE          0x3040
    EGLint              width;
    EGLint              height;

    EGLConfig           config;             // EGL driver config.

    struct _stEglConfig *next;

    _stEglConfig()
    {
        width               = 0;
        height              = 0;

        bufferSize          = 0;
        alphaSize           = 0;
        blueSize            = 0;
        greenSize           = 0;
        redSize             = 0;
        depthSize           = 0;
        stencilSize         = 0;
        configCaveat        = (EGLenum) EGL_DONT_CARE;
        configId            = EGL_DONT_CARE;
        nativeRenderable    = (EGLBoolean) EGL_DONT_CARE;
        nativeVisualType    = EGL_DONT_CARE;
        samples             = 0;
        sampleBuffers       = 0;
        surfaceType         = (EGLenum) EGL_WINDOW_BIT;
        bindToTetxureRGB    = (EGLBoolean) EGL_DONT_CARE;
        bindToTetxureRGBA   = (EGLBoolean) EGL_DONT_CARE;
        luminanceSize       = 0;
        alphaMaskSize       = 0;
        colorBufferType     = EGL_RGB_BUFFER;
        renderableType      = EGL_OPENGL_ES_BIT;

        config              = 0;

        next                = NULL;
    }
} stEglConfig;

typedef struct _stEglSurface
{
    enSurfaceType       type;
    EGLint              width;
    EGLint              height;
    EGLint              addr;
    char                name[32];
    const EGLint        *attrib_list;

    EGLConfig           config;
    EGLSurface          surface;    // EGL driver surface
    EGLDisplay          display;    // EGL driver display

    _stEglSurface       *next;

    _stEglSurface()
    {
        type    = SURFACE_TYPE_INVALID;
        width   = 0;
        height  = 0;
        addr    = 0;
        attrib_list = NULL;

        display = NULL;
        config  = NULL;
        surface = NULL;

        next    = NULL;
    }
} stEglSurface;

class CEGLContext
{
public:
    CEGLContext();
    ~CEGLContext();

public:
    EGLint              addr;
    char                name[32];

    EGLSurface          draw;
    EGLSurface          read;
    EGLConfig           config;
    EGLContext          context;    // EGL driver context
    EGLContext          shareCtx;   // EGL share context
    const EGLint        *attrib_list;
    EGLenum             error;
    EGLint              client;

    CEGLContext         *next;

    CGLES3Context       *pGLESContext;

    EGLint              nFrame;
};

typedef struct _stEglDisplay
{
    EGLNativeDisplayType    displayId;
    EGLDisplay              display;
    _stEglDisplay           *next;

    CEGLContext             *eglCtxStack;
    stEglSurface            *eglSurfStack;
    stEglConfig             *eglConfStack;

    EGLint                  reference;

    _stEglDisplay()
    {
        displayId   = 0;
        display     = 0;
        next        = 0;
        reference   = 0;

        eglCtxStack     = NULL;
        eglSurfStack    = NULL;
        eglConfStack    = NULL;
    }
} stEglDisplay;

/* Thread Data */
typedef struct _stEglThreadData
{
    /* Last known error code. */
    EGLenum                 error;

    /* Current API. */
    EGLenum                 api;

    /* Hardware capabilities. */
    EGLint                  maxWidth;
    EGLint                  maxHeight;
    EGLint                  maxSamples;

    /* Current context. */
    CEGLContext             *context;
    EGLint                  lastClient;

    /* Allocated resources. */
    EGLBoolean              destroying;

    stEglDisplay            *eglDpyStack;
} stEglThreadData;

typedef stEglThreadData     *PEglThreadData;

void ApiChooseConfig(EGLDisplay dpy, const EGLint *attrib_list, EGLConfig *configs, EGLint config_size, EGLint *num_config, EGLBoolean ret);
void ApiCopyBuffers(EGLDisplay dpy, EGLSurface surface, EGLNativePixmapType target, EGLBoolean ret);
void ApiCreateContext(EGLDisplay dpy, EGLConfig config, EGLContext share_context, const EGLint *attrib_list, EGLContext ret);
void ApiCreatePbufferSurface(EGLDisplay dpy, EGLConfig config, const EGLint *attrib_list, EGLSurface ret);
void ApiCreatePixmapSurface(EGLDisplay dpy, EGLConfig config, EGLNativePixmapType pixmap, const EGLint *attrib_list, EGLSurface ret);
void ApiCreateWindowSurface(EGLDisplay dpy, EGLConfig config, EGLNativeWindowType win, const EGLint *attrib_list, EGLSurface ret);
void ApiDestroyContext(EGLDisplay dpy, EGLContext ctx, EGLBoolean ret);
void ApiDestroySurface(EGLDisplay dpy, EGLSurface surface, EGLBoolean ret);
void ApiGetConfigAttrib(EGLDisplay dpy, EGLConfig config, EGLint attribute, EGLint *value, EGLBoolean ret);
void ApiGetConfigs(EGLDisplay dpy, EGLConfig *configs, EGLint config_size, EGLint *num_config, EGLBoolean ret);
void ApiGetCurrentDisplay(EGLDisplay ret);
void ApiGetCurrentSurface(EGLint readdraw, EGLSurface ret);
void ApiGetDisplay(EGLNativeDisplayType display_id, EGLDisplay ret);
void ApiEglGetError(EGLint ret);
void ApiSetFIFO(void*, EGLBoolean ret);
void ApiGetProcAddress(const char *procname, void *func);
void ApiInitialize(EGLDisplay dpy, EGLint *major, EGLint *minor, EGLBoolean ret);
void ApiMakeCurrent(EGLDisplay dpy, EGLSurface draw, EGLSurface read, EGLContext ctx, EGLBoolean ret);
void ApiQueryContext(EGLDisplay dpy, EGLContext ctx, EGLint attribute, EGLint *value, EGLBoolean ret);
void ApiQueryString(EGLDisplay dpy, EGLint name, const char *ret);
void ApiQuerySurface(EGLDisplay dpy, EGLSurface surface, EGLint attribute, EGLint *value, EGLBoolean ret);
void ApiSwapBuffers(EGLDisplay dpy, EGLSurface surface, EGLBoolean ret);
void ApiTerminate(EGLDisplay dpy, EGLBoolean ret);
void ApiWaitGL(EGLBoolean ret);
void ApiWaitNative(EGLint engine, EGLBoolean ret);
void ApiBindAPI(EGLenum api, EGLBoolean ret);
void ApiQueryAPI(EGLenum ret);
void ApiCreatePbufferFromClientBuffer(EGLDisplay dpy, EGLenum buftype, EGLClientBuffer buffer, EGLConfig config, const EGLint *attrib_list, EGLSurface ret);
void ApiReleaseThread(EGLBoolean ret);
void ApiWaitClient(EGLBoolean ret);
void ApiBindTexImage(EGLDisplay dpy, EGLSurface surface, EGLint buffer, EGLBoolean ret);
void ApiReleaseTexImage(EGLDisplay dpy, EGLSurface surface, EGLint buffer, EGLBoolean ret);
void ApiSurfaceAttrib(EGLDisplay dpy, EGLSurface surface, EGLint attribute, EGLint value, EGLBoolean ret);
void ApiSwapInterval(EGLDisplay dpy, EGLint interval, EGLBoolean ret);
void ApiGetCurrentContext(EGLContext ret);

stEglThreadData*    _GetThreadData(void);
stEglDisplay*       _GetDisplay(EGLDisplay dpy);
CEGLContext*        _GetEglContext(EGLDisplay dpy, EGLContext context);
stEglSurface*       _GetSurface(EGLDisplay dpy, EGLSurface surf);
void                _SyncEnter();
void                _SyncExit();

extern stEngine     *g_opengl;

#endif /* __API_EGL_CONTEXT_H */
