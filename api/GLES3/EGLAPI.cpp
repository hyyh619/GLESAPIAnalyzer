#include "ApiCore.h"
#include "EGLAPI.h"
#include "ES30API.h"
#include "ApiGLES3Context.h"

extern stPrivacy *privacy;

/* These resources should be per-process. */
static stEngine     *g_opengl       = NULL;
static PEGLDisplay  g_displayStack  = NULL;
static EGLBoolean   g_bFirst        = EGL_TRUE;
static HANDLE       g_mutex         = NULL;
static EGLint       g_threadValue   = TLS_OUT_OF_INDEXES;

static veglLOOKUP _veglLookup[] =
{
    eglMAKE_LOOKUP(eglGetError),
    eglMAKE_LOOKUP(eglGetDisplay),
    eglMAKE_LOOKUP(eglInitialize),
    eglMAKE_LOOKUP(eglTerminate),
    eglMAKE_LOOKUP(eglQueryString),
    eglMAKE_LOOKUP(eglGetConfigs),
    eglMAKE_LOOKUP(eglChooseConfig),
    eglMAKE_LOOKUP(eglGetConfigAttrib),
    eglMAKE_LOOKUP(eglCreateWindowSurface),
    eglMAKE_LOOKUP(eglCreatePbufferSurface),
    eglMAKE_LOOKUP(eglCreatePixmapSurface),
    eglMAKE_LOOKUP(eglDestroySurface),
    eglMAKE_LOOKUP(eglQuerySurface),
    eglMAKE_LOOKUP(eglBindAPI),
    eglMAKE_LOOKUP(eglQueryAPI),
    eglMAKE_LOOKUP(eglWaitClient),
    eglMAKE_LOOKUP(eglReleaseThread),
    eglMAKE_LOOKUP(eglCreatePbufferFromClientBuffer),
    eglMAKE_LOOKUP(eglSurfaceAttrib),
    eglMAKE_LOOKUP(eglBindTexImage),
    eglMAKE_LOOKUP(eglReleaseTexImage),
    eglMAKE_LOOKUP(eglSwapInterval),
    eglMAKE_LOOKUP(eglCreateContext),
    eglMAKE_LOOKUP(eglDestroyContext),
    eglMAKE_LOOKUP(eglMakeCurrent),
    eglMAKE_LOOKUP(eglGetCurrentContext),
    eglMAKE_LOOKUP(eglGetCurrentSurface),
    eglMAKE_LOOKUP(eglGetCurrentDisplay),
    eglMAKE_LOOKUP(eglQueryContext),
    eglMAKE_LOOKUP(eglWaitGL),
    eglMAKE_LOOKUP(eglWaitNative),
    eglMAKE_LOOKUP(eglSwapBuffers),
    eglMAKE_LOOKUP(eglCopyBuffers),
    eglMAKE_LOOKUP(eglGetProcAddress),
    //eglMAKE_LOOKUP(eglLockSurfaceKHR),
    //eglMAKE_LOOKUP(eglUnlockSurfaceKHR),
    //eglMAKE_LOOKUP(eglCreateImageKHR),
    //eglMAKE_LOOKUP(eglDestroyImageKHR),
    //eglMAKE_LOOKUP(eglCreateSyncKHR),
    //eglMAKE_LOOKUP(eglDestroySyncKHR),
    //eglMAKE_LOOKUP(eglClientWaitSyncKHR),
    //eglMAKE_LOOKUP(eglSignalSyncKHR),
    //eglMAKE_LOOKUP(eglGetSyncAttribKHR),
    { NULL, NULL }
};

static PEGLThreadData
_GetThreadData(
               void
               );

static void
_DestroyThreadData(
                   void
                   );

static void
_ReferenceDisplay(
                  PEGLDisplay dpy
                  );

static void
_DereferenceDisplay(
                    PEGLDisplay dpy,
                    EGLBoolean  always
                    );

static void* _VECreateContext(PEGLContext Ctx)
{
    void * context = NULL;

    /* Create Client Context */
    context = GLES3CreateContext(g_opengl);

    return context;
}

static void
_VEDestroyContext(
                  PEGLContext Ctx,
                  void* Context
                  )
{
    GLES3DestroyContext(Context);
}

static EGLBoolean
_VESetContext(
              PEGLContext Ctx,
              void* Context,
              const stEngine* Eng
              )
{
    /* Set Client Context */
    EGLBoolean result;
    result = (EGLBoolean)GLES3SetContext(Context, Eng);
    return result;
}

static void
_SyncEnter(
           void
           )
{
    if (g_bFirst)
    {
        g_mutex = apiOS_CreateSync();
        g_bFirst = EGL_FALSE;

        /* reference native g_opengl */
        if (g_opengl == NULL)
        {
            g_opengl = (stEngine*)ConstructOpenGLEngine();
        }
    }

    apiOS_SyncEnter(g_mutex);
}


static void
_SyncExit(
          void
          )
{
    apiOS_SyncExit(g_mutex);
}

static int
_FillInAll(
           HDC hDC,
           PEGLConfig* configs,
           int* numConfig
           )
{
    return 1;
}

static EGLBoolean
_ParseAttrbutes(
                EGLDisplay dpy,
                const EGLint *attrib_list,
                struct eglConfig *attribConfig
                )
{
    EGLenum attribute;
    EGLint  value;

    PEGLThreadData thread = _GetThreadData();

    /* Convert attrib_list into a configuration */
    attribConfig->bufferSize        = 0;
    attribConfig->alphaSize         = 0;
    attribConfig->blueSize          = 0;
    attribConfig->greenSize         = 0;
    attribConfig->redSize           = 0;
    attribConfig->depthSize         = 0;
    attribConfig->stencilSize       = 0;
    attribConfig->configCaveat      = (EGLenum) EGL_DONT_CARE;
    attribConfig->configId          = EGL_DONT_CARE;
    attribConfig->nativeRenderable  = (EGLBoolean) EGL_DONT_CARE;
    attribConfig->nativeVisualType  = EGL_DONT_CARE;
    attribConfig->samples           = 0;
    attribConfig->sampleBuffers     = 0;
    attribConfig->surfaceType       = (EGLenum) EGL_WINDOW_BIT;
    attribConfig->bindToTetxureRGB  = (EGLBoolean) EGL_DONT_CARE;
    attribConfig->bindToTetxureRGBA = (EGLBoolean) EGL_DONT_CARE;
    attribConfig->luminanceSize     = 0;
    attribConfig->alphaMaskSize     = 0;
    attribConfig->colorBufferType   = EGL_RGB_BUFFER;
    attribConfig->renderableType    = EGL_OPENGL_ES_BIT;

    do
    {
        if (attrib_list != NULL)
        {
            attribute      = attrib_list[0];
            value          = attrib_list[1];
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
            attribConfig->bufferSize = value;
            break;
        case EGL_ALPHA_SIZE:
            attribConfig->alphaSize = value;
            break;
        case EGL_BLUE_SIZE:
            attribConfig->blueSize = value;
            break;
        case EGL_GREEN_SIZE:
            attribConfig->greenSize = value;
            break;
        case EGL_RED_SIZE:
            attribConfig->redSize = value;
            break;
        case EGL_DEPTH_SIZE:
            attribConfig->depthSize = value;
            break;
        case EGL_STENCIL_SIZE:
            attribConfig->stencilSize = value;
            break;
        case EGL_CONFIG_CAVEAT:
            attribConfig->configCaveat = value;
            break;
        case EGL_CONFIG_ID:
            attribConfig->configId = value;
            break;
        case EGL_LEVEL:
            if ((value != EGL_DONT_CARE) && (value != 0))
            {
                return EGL_FALSE;
            }
            break;
        case EGL_MAX_PBUFFER_WIDTH:
            if ((value != EGL_DONT_CARE) && (value > thread->maxWidth))
            {
                return EGL_FALSE;
            }
            break;
        case EGL_MAX_PBUFFER_HEIGHT:
            if ((value != EGL_DONT_CARE) && (value > thread->maxHeight))
            {
                return EGL_FALSE;
            }
            break;
        case EGL_MAX_PBUFFER_PIXELS:
            if ( (value != EGL_DONT_CARE) &&
                (value > thread->maxWidth * thread->maxHeight) )
            {
                return EGL_FALSE;
            }
            break;
        case EGL_NATIVE_RENDERABLE:
            attribConfig->nativeRenderable = value;
            break;
        case EGL_NATIVE_VISUAL_ID:
            if
                (
                (value != EGL_DONT_CARE)
                &&
                ((void*) value != (void*) ((PEGLDisplay)dpy)->hdc)
                )
            {
                return EGL_FALSE;
            }
            break;
        case EGL_NATIVE_VISUAL_TYPE:
            attribConfig->nativeVisualType = value;
            break;
        case EGL_SAMPLES:
            attribConfig->samples = value;
            break;
        case EGL_SAMPLE_BUFFERS:
            attribConfig->sampleBuffers = value;
            break;
        case EGL_SURFACE_TYPE:
            attribConfig->surfaceType = value;
            break;
        case EGL_TRANSPARENT_TYPE:
            if ((value != EGL_NONE) && (value != EGL_DONT_CARE))
            {
                return EGL_FALSE;
            }
            break;
        case EGL_TRANSPARENT_BLUE_VALUE:
            if (value != EGL_DONT_CARE)
            {
                return EGL_FALSE;
            }
            break;
        case EGL_TRANSPARENT_GREEN_VALUE:
            if (value != EGL_DONT_CARE)
            {
                return EGL_FALSE;
            }
            break;
        case EGL_TRANSPARENT_RED_VALUE:
            if (value != EGL_DONT_CARE)
            {
                return EGL_FALSE;
            }
            break;
        case EGL_BIND_TO_TEXTURE_RGB:
            attribConfig->bindToTetxureRGB = value;
            break;
        case EGL_BIND_TO_TEXTURE_RGBA:
            attribConfig->bindToTetxureRGBA = value;
            break;
        case EGL_MIN_SWAP_INTERVAL:
            if ((value != EGL_DONT_CARE) && (value > 1))
            {
                return EGL_FALSE;
            }
            break;
        case EGL_MAX_SWAP_INTERVAL:
            if ((value != EGL_DONT_CARE) && (value > 1))
            {
                return EGL_FALSE;
            }
            break;
        case EGL_LUMINANCE_SIZE:
            attribConfig->luminanceSize = value;
            break;
        case EGL_ALPHA_MASK_SIZE:
            attribConfig->alphaMaskSize = value;
            break;
        case EGL_COLOR_BUFFER_TYPE:
            attribConfig->colorBufferType = value;
            break;
        case EGL_RENDERABLE_TYPE:
            attribConfig->renderableType = value;
            break;
        case EGL_WIDTH:
            attribConfig->width = value;
            break;
        case EGL_HEIGHT:
            attribConfig->height = value;
            break;
        case EGL_CONFORMANT:
            break;
        case EGL_NONE:
            break;

        default:
            /* Bad attribute. */
            thread->error = EGL_BAD_ATTRIBUTE;
            return EGL_FALSE;
        }
    }
    while (attribute != EGL_NONE);

    /* Success. */
    return EGL_TRUE;
}

static void
_MapAttributes(
               int* attrib_list,
               EGLConfig config
               )
{
    //int i;
    //PEGLConfig cfg = (PEGLConfig) config;

    //for (i = 0; attrib_list[i] != 0; i++)
    //{
    //    switch(attrib_list[i++])
    //    {
    //    default:
    //        break;
    //    }
    //}
}

EGLAPI EGLint EGLAPIENTRY
eglGetError(
            void
            )
{
    PEGLThreadData thread = _GetThreadData();
    thread->error = g_opengl->eglGetError();
    return thread->error;
}

EGLAPI EGLDisplay EGLAPIENTRY
eglGetDisplay(
              EGLNativeDisplayType display_id
              )
{
    PEGLDisplay display = NULL;

    _SyncEnter();

    /* find in g_displayStack */
    if (display_id == EGL_DEFAULT_DISPLAY)
    {
        for (display = g_displayStack; display != NULL; display = display->next)
        {
            if (display->defaultDpy == TRUE)
            {
                /* Got it. */
                _SyncExit();
                return display;
            }
        }
    }
    else
    {
        for (display = g_displayStack; display != NULL; display = display->next)
        {
            if (display->hdc == display_id)
            {
                /* Got it. */
                _SyncExit();
                return display;
            }
        }
    }

    _SyncExit();

    /* Create a new EGLDisplay if not found. */
    display = (PEGLDisplay) malloc(sizeof (struct eglDisplay));

    display->signature      = EGL_DISPLAY_SIGNATURE;
    display->hdc            = display_id;
    display->defaultDpy     = (display_id == EGL_DEFAULT_DISPLAY);
    display->reference      = 0;
    display->configCount    = 0;
    display->config         = NULL;
    display->surfaceStack   = NULL;
    display->contextStack   = NULL;
    display->imageStack     = NULL;
    display->process        = OpenProcess(PROCESS_ALL_ACCESS, FALSE, GetCurrentProcessId());
    CloseHandle(display->process);

    _SyncEnter();
    display->next   = g_displayStack;
    g_displayStack  = display;
    _SyncExit();

    // call egl functions
    display->display = g_opengl->eglGetDisplay(display_id);

    return (EGLDisplay)display;
}

EGLAPI EGLBoolean EGLAPIENTRY
eglInitialize(
              EGLDisplay dpy,
              EGLint *major,
              EGLint *minor
              )
{
    PEGLDisplay     display = (PEGLDisplay) dpy;
    PEGLThreadData  thread  = _GetThreadData();

    if (display == EGL_NO_DISPLAY ||
        display->signature != EGL_DISPLAY_SIGNATURE)
    {
        thread->error = EGL_BAD_DISPLAY;

        return EGL_FALSE;
    }

    _ReferenceDisplay(display);

    return g_opengl->eglInitialize(((PEGLDisplay)dpy)->display, major, minor);
}


EGLAPI EGLBoolean EGLAPIENTRY
eglTerminate(
             EGLDisplay dpy
             )
{
    PEGLDisplay     display = (PEGLDisplay)dpy;
    PEGLThreadData  thread  = _GetThreadData();

    _DereferenceDisplay(display, EGL_TRUE);

    /* destroy dispaly if no referenced */
    if (display->reference == 0)
    {
        _SyncEnter();
        /* remove dpy in g_displayStack */
        if (display == g_displayStack)
        {
            /* header */
            g_displayStack = display->next;
        }
        else
        {
            PEGLDisplay stack;

            for (stack = g_displayStack; stack->next != NULL; stack = stack->next)
            {
                if (stack->next == display)
                {
                    stack->next = display->next;
                    break;
                }
            }
        }

        free(display);

        _SyncExit();
    }

    return g_opengl->eglTerminate(((PEGLDisplay)dpy)->display);
}


EGLAPI const char *EGLAPIENTRY
eglQueryString(
               EGLDisplay dpy,
               EGLint name
               )
{
    return g_opengl->eglQueryString(((PEGLDisplay)dpy)->display, name);
}

EGLAPI EGLBoolean EGLAPIENTRY
eglGetConfigs(
              EGLDisplay dpy,
              EGLConfig *configs,
              EGLint config_size,
              EGLint *num_config
              )
{
    return g_opengl->eglGetConfigs(((PEGLDisplay)dpy)->display, configs, config_size, num_config);
}


EGLAPI EGLBoolean EGLAPIENTRY
eglChooseConfig(
                EGLDisplay dpy,
                const EGLint *attrib_list,
                EGLConfig *configs,
                EGLint config_size,
                EGLint *num_config
                )
{
    return g_opengl->eglChooseConfig(((PEGLDisplay)dpy)->display, attrib_list, configs, config_size, num_config);
}


EGLAPI EGLBoolean EGLAPIENTRY
eglGetConfigAttrib(
                   EGLDisplay dpy,
                   EGLConfig config,
                   EGLint attribute,
                   EGLint *value
                   )
{
    return g_opengl->eglGetConfigAttrib(((PEGLDisplay)dpy)->display, config, attribute, value);
}


EGLAPI EGLSurface EGLAPIENTRY
eglCreateWindowSurface(
                       EGLDisplay dpy,
                       EGLConfig config,
                       EGLNativeWindowType win,
                       const EGLint *attrib_list
                       )
{
    PEGLSurface     surface = (PEGLSurface)EGL_NO_SURFACE;
    PEGLConfig      cfg     = (PEGLConfig)config;
    PEGLDisplay     display = (PEGLDisplay)dpy;
    PEGLThreadData  thread  = _GetThreadData();

    if (display->reference == 0)
    {
        thread->error = EGL_NOT_INITIALIZED;
        return EGL_NO_SURFACE;
    }

    if (cfg == NULL)
    {
        thread->error = EGL_BAD_CONFIG;
        return EGL_NO_SURFACE;
    }

    do
    {
        /* Allocate surface structure. */
        surface = (PEGLSurface)malloc(sizeof (struct eglSurface));

        surface->signature      = EGL_SURFACE_SIGNATURE;

        _SyncEnter();
        surface->next = display->surfaceStack;
        display->surfaceStack = surface;
        _SyncExit();

        memcpy(&surface->config, config, sizeof (struct eglConfig));

        surface->type           = EGL_WINDOW_BIT;
        surface->largestPBuffer = EGL_FALSE;
        surface->mipmapTexture  = EGL_FALSE;
        surface->mipmapLevel    = 0;
        surface->textureFormat  = EGL_NO_TEXTURE;
        surface->textureTarget  = EGL_NO_TEXTURE;
        surface->bBounded       = EGL_FALSE;

        surface->surface = g_opengl->eglCreateWindowSurface(((PEGLDisplay)dpy)->display, cfg->config, win, attrib_list);

        return surface;
    }
    while (0);

    return EGL_NO_SURFACE;
}

EGLAPI EGLSurface EGLAPIENTRY
eglCreatePbufferSurface(EGLDisplay dpy,
                        EGLConfig config,
                        const EGLint *attrib_list)
{
    PEGLSurface     surface = (PEGLSurface)EGL_NO_SURFACE;
    PEGLConfig      cfg     = (PEGLConfig)config;
    PEGLDisplay     display = (PEGLDisplay)dpy;
    PEGLThreadData  thread  = _GetThreadData();

    if (display == EGL_NO_DISPLAY ||
        display->signature != EGL_DISPLAY_SIGNATURE)
    {
        thread->error = EGL_BAD_DISPLAY;

        return EGL_NO_SURFACE;
    }

    if (display->reference == 0)
    {
        thread->error = EGL_NOT_INITIALIZED;
        return EGL_NO_SURFACE;
    }

    if (cfg == NULL)
    {
        thread->error = EGL_BAD_CONFIG;
        return EGL_NO_SURFACE;
    }

    do
    {
        /* Allocate surface structure. */
        surface = (PEGLSurface)malloc(sizeof (struct eglSurface));
        surface->signature = EGL_SURFACE_SIGNATURE;

        _SyncEnter();
        surface->next = display->surfaceStack;
        display->surfaceStack = surface;
        _SyncExit();

        memcpy(&surface->config, config, sizeof (struct eglConfig));

        surface->type           = EGL_PBUFFER_BIT;
        surface->mipmapLevel    = 0;
        surface->bBounded       = EGL_FALSE;

        surface->surface = g_opengl->eglCreatePbufferSurface(((PEGLDisplay)dpy)->display, config, attrib_list);

        return surface;
    }
    while (0);

    return EGL_NO_SURFACE;
}

EGLAPI EGLSurface EGLAPIENTRY
eglCreatePixmapSurface(EGLDisplay dpy,
                       EGLConfig config,
                       EGLNativePixmapType pixmap,
                       const EGLint *attrib_list)
{
    PEGLSurface     surface = (PEGLSurface)EGL_NO_SURFACE;
    PEGLConfig      cfg     = (PEGLConfig) config;
    PEGLDisplay     display = (PEGLDisplay) dpy;
    PEGLThreadData  thread  = _GetThreadData();

    if (display == EGL_NO_DISPLAY ||
        display->signature != EGL_DISPLAY_SIGNATURE)
    {
        thread->error = EGL_BAD_DISPLAY;
        return EGL_NO_SURFACE;
    }

    if (display->reference == 0)
    {
        thread->error = EGL_NOT_INITIALIZED;
        return EGL_NO_SURFACE;
    }

    if (cfg == NULL)
    {
        thread->error = EGL_BAD_CONFIG;
        return EGL_NO_SURFACE;
    }

    if (pixmap == NULL)
    {
        thread->error = EGL_BAD_NATIVE_PIXMAP;
        return EGL_NO_SURFACE;
    }

    do
    {

        /* Allocate surface structure. */
        surface = (PEGLSurface)malloc(sizeof (struct eglSurface));
        surface->signature = EGL_SURFACE_SIGNATURE;

        _SyncEnter();
        surface->next = display->surfaceStack;
        display->surfaceStack = surface;
        _SyncExit();

        memcpy(&surface->config, config, sizeof (struct eglConfig));
        surface->type           = EGL_PIXMAP_BIT;
        surface->largestPBuffer = EGL_FALSE;
        surface->mipmapTexture  = EGL_FALSE;
        surface->mipmapLevel    = 0;
        surface->textureFormat  = EGL_NO_TEXTURE;
        surface->textureTarget  = EGL_NO_TEXTURE;
        surface->bBounded       = EGL_FALSE;

        surface->surface = eglCreatePixmapSurface(((PEGLDisplay)dpy)->display, config, pixmap, attrib_list);

        return surface;
    }
    while (0);

    return EGL_NO_SURFACE;
}

EGLAPI EGLBoolean EGLAPIENTRY
eglDestroySurface(
                  EGLDisplay dpy,
                  EGLSurface surface
                  )
{
    PEGLDisplay     display = (PEGLDisplay)dpy;
    PEGLSurface     surf    = (PEGLSurface) surface;
    PEGLThreadData  thread  = _GetThreadData();
    EGLBoolean      result  = EGL_FALSE;

    if (display == EGL_NO_DISPLAY ||
        display->signature != EGL_DISPLAY_SIGNATURE)
    {
        thread->error = EGL_BAD_DISPLAY;

        return EGL_FALSE;
    }

    if (display->reference == 0)
    {
        thread->error = EGL_NOT_INITIALIZED;
        return EGL_FALSE;
    }

    if (surf == EGL_NO_SURFACE ||
        surf->signature != EGL_SURFACE_SIGNATURE)
    {
        thread->error = EGL_BAD_SURFACE;
        return EGL_FALSE;
    }

    _SyncEnter();
    if (surf == display->surfaceStack)
    {
        /* Simple - it is the top of the stack. */
        display->surfaceStack = surf->next;
    }
    else
    {
        PEGLSurface stack;

        /* Walk the stack. */
        for (stack = display->surfaceStack; stack->next != NULL; stack = stack->next)
        {
            /* Check if the next surface on the stack is ours. */
            if (stack->next == surf)
            {
                /* Pop the surface from the stack. */
                stack->next = surf->next;
                break;
            }
        }
    }
    _SyncExit();

    result = g_opengl->eglDestroySurface(((PEGLDisplay)dpy)->display, surf->surface);

    free(surf);

    return EGL_TRUE;
}

EGLAPI EGLBoolean EGLAPIENTRY
eglBindAPI(
           EGLenum api
           )
{
    return g_opengl->eglBindAPI(api);
}

EGLAPI EGLenum EGLAPIENTRY
eglQueryAPI(
            void
            )
{
    return g_opengl->eglQueryAPI();
}

EGLAPI EGLBoolean EGLAPIENTRY
eglWaitClient(
              void
              )
{
    return g_opengl->eglWaitClient();
}

EGLAPI EGLBoolean EGLAPIENTRY
eglReleaseThread(
                 void
                 )
{
    return g_opengl->eglReleaseThread();
}

EGLAPI EGLSurface EGLAPIENTRY
eglCreatePbufferFromClientBuffer(
                                 EGLDisplay dpy,
                                 EGLenum buftype,
                                 EGLClientBuffer buffer,
                                 EGLConfig config,
                                 const EGLint *attrib_list
                                 )
{
    return g_opengl->eglCreatePbufferFromClientBuffer(((PEGLDisplay)dpy)->display, buftype, buffer, config, attrib_list);
}

EGLAPI EGLBoolean EGLAPIENTRY
eglQuerySurface(
                EGLDisplay dpy,
                EGLSurface surface,
                EGLint attribute,
                EGLint *value
                )
{
    PEGLDisplay display = (PEGLDisplay)dpy;
    PEGLSurface surf    = (PEGLSurface) surface;

    return g_opengl->eglQuerySurface(display->display, surf->surface, attribute, value);
}

EGLAPI EGLBoolean EGLAPIENTRY
eglSurfaceAttrib(
                 EGLDisplay dpy,
                 EGLSurface Surface,
                 EGLint attribute,
                 EGLint value
                 )
{
    PEGLDisplay display = (PEGLDisplay) dpy;
    PEGLSurface surface = (PEGLSurface) Surface;

    return g_opengl->eglSurfaceAttrib(display->display, surface->surface, attribute, value);
}

EGLAPI EGLBoolean EGLAPIENTRY
eglBindTexImage(
                EGLDisplay dpy,
                EGLSurface surface,
                EGLint buffer
                )
{
    PEGLDisplay display = (PEGLDisplay)dpy;
    PEGLSurface surf    = (PEGLSurface)surface;

    return g_opengl->eglBindTexImage(display->display, surf->surface, buffer);
}

EGLAPI EGLBoolean EGLAPIENTRY
eglReleaseTexImage(
                   EGLDisplay dpy,
                   EGLSurface Surface,
                   EGLint buffer
                   )
{
    PEGLDisplay display = (PEGLDisplay)dpy;
    PEGLSurface surface = (PEGLSurface)Surface;

    return g_opengl->eglReleaseTexImage(display->display, surface->surface, buffer);
}

EGLAPI EGLBoolean EGLAPIENTRY
eglSwapInterval(
                EGLDisplay dpy,
                EGLint Interval
                )
{
    return g_opengl->eglSwapInterval(((PEGLDisplay)dpy)->display, Interval);
}

EGLAPI EGLContext EGLAPIENTRY
eglCreateContext(EGLDisplay dpy,
                 EGLConfig config,
                 EGLContext share_context,
                 const EGLint *attrib_list)
{
    PEGLDisplay     display         = (PEGLDisplay)dpy;
    PEGLContext     context         = (PEGLContext)EGL_NO_CONTEXT;
    PEGLContext     pShareCtx       = (PEGLContext)share_context;
    PEGLThreadData  thread          = _GetThreadData();
    EGLint          client          = 1;
    EGLenum         ctxPriorityImg  = 0;

    if (display == EGL_NO_DISPLAY ||
        display->signature != EGL_DISPLAY_SIGNATURE)
    {
        thread->error = EGL_BAD_DISPLAY;
        return EGL_NO_CONTEXT;
    }

    if (display->reference == 0)
    {
        thread->error = EGL_NOT_INITIALIZED;
        return EGL_NO_CONTEXT;
    }

    if (config == NULL)
    {
        thread->error = EGL_BAD_CONFIG;
        return EGL_NO_CONTEXT;
    }

    /* Get attribute. */
    if (attrib_list != NULL)
    {
        const EGLint *pAttr = attrib_list;
        while (*pAttr != EGL_NONE)
        {
            switch (*pAttr++)
            {
            case EGL_CONTEXT_CLIENT_VERSION:
                if (thread->api == EGL_OPENGL_ES_API)
                {
                    /* Save client. */
                    client = *pAttr++;
                    break;
                }

            case EGL_CONTEXT_PRIORITY_LEVEL_IMG:
                ctxPriorityImg = *pAttr++;
                break;

                /* Pass through for error. */
            default:
                /* Invalid attrribute. or not supported */
                thread->error = EGL_BAD_MATCH;
                return EGL_NO_CONTEXT;
            }
        }
    }

    do
    {
        /* TODO: Handle other args. */
        context = (PEGLContext)malloc(sizeof (struct eglContext));

        context->signature      = EGL_CONTEXT_SIGNATURE;
        context->thread         = thread;
        context->api            = thread->api;
        context->client         = client;
        context->display        = (PEGLDisplay)dpy;
        context->sharedContext  = (PEGLContext)share_context;
        context->draw           = (PEGLSurface)EGL_NO_SURFACE;
        context->read           = (PEGLSurface)EGL_NO_SURFACE;
        context->renderContext  = (HGLRC)EGL_NO_CONTEXT;
        context->glContext      = _VECreateContext(context);

        if (context->glContext == EGL_NO_CONTEXT)
        {
            break;
        }

        _SyncEnter();
        context->next = display->contextStack;
        display->contextStack = context;
        _SyncExit();

        _ReferenceDisplay((PEGLDisplay)dpy);

        context->context = g_opengl->eglCreateContext(display->display, config, pShareCtx?pShareCtx->context:NULL, attrib_list);

        return context;
    }
    while (0);

    if (context != NULL)
    {
        free(context);
    }

    return EGL_NO_CONTEXT;
}

EGLAPI EGLBoolean EGLAPIENTRY
eglDestroyContext(EGLDisplay dpy,
                  EGLContext ctx)
{
    PEGLDisplay     display = (PEGLDisplay) dpy;
    PEGLContext     context = (PEGLContext) ctx;
    PEGLThreadData  thread  = _GetThreadData();
    EGLBoolean      result  = EGL_FALSE;

    if (display == EGL_NO_DISPLAY ||
        display->signature != EGL_DISPLAY_SIGNATURE)
    {
        thread->error = EGL_BAD_DISPLAY;
        return EGL_FALSE;
    }

    if (display->reference == 0)
    {
        thread->error = EGL_NOT_INITIALIZED;
        return EGL_FALSE;
    }

    if (thread->api == EGL_NONE)
    {
        thread->error = EGL_NOT_INITIALIZED;
        return EGL_FALSE;
    }

    result = g_opengl->eglDestroyContext(display->display, context->context);

    _SyncEnter();
    if (context == display->contextStack)
    {
        display->contextStack = context->next;
    }
    else
    {
        PEGLContext stack;

        for (stack = display->contextStack; stack->next != NULL; stack = stack->next)
        {
            if (stack->next == context)
            {
                stack->next = context->next;
                break;
            }
        }
    }
    _SyncExit();

    _DereferenceDisplay(display, EGL_FALSE);

    /* If it is currect context. */
    if (thread->context == context)
    {
        thread->context = (PEGLContext)EGL_NO_CONTEXT;
    }

    _VEDestroyContext(context, context->glContext);

    free(context);

    return EGL_TRUE;
}


EGLAPI EGLBoolean EGLAPIENTRY
eglMakeCurrent(
               EGLDisplay dpy,
               EGLSurface draw,
               EGLSurface read,
               EGLContext ctx
               )
{
    PEGLDisplay     display     = (PEGLDisplay) dpy;
    PEGLSurface     drawSurf    = (PEGLSurface) draw;
    PEGLSurface     readSurf    = (PEGLSurface) read;
    PEGLContext     context     = (PEGLContext) ctx;
    PEGLThreadData  thread      = _GetThreadData();

    thread->context = context;
    thread->context->draw = drawSurf;
    thread->context->read = readSurf;

    return g_opengl->eglMakeCurrent(display->display, drawSurf->surface, readSurf->surface, context->context);
}

EGLAPI EGLContext EGLAPIENTRY
eglGetCurrentContext(
                     void
                     )
{
    PEGLThreadData thread = _GetThreadData();

    if (thread->api == EGL_NONE)
    {
        return EGL_NO_CONTEXT;
    }

    return thread->context;
}

EGLAPI EGLSurface EGLAPIENTRY
eglGetCurrentSurface(
                     EGLint readdraw
                     )
{
    PEGLThreadData thread = _GetThreadData();

    if (thread->context == EGL_NO_CONTEXT)
    {
        return EGL_NO_SURFACE;
    }

    switch (readdraw)
    {
    case EGL_READ:
        return thread->context->read;

    case EGL_DRAW:
        return thread->context->draw;

    default:
        thread->error = EGL_BAD_PARAMETER;
        return EGL_NO_SURFACE;
    }
}

EGLAPI EGLDisplay EGLAPIENTRY
eglGetCurrentDisplay(
                     void
                     )
{
    PEGLThreadData thread = _GetThreadData();

    if (thread->context == EGL_NO_CONTEXT)
    {
        return EGL_NO_DISPLAY;
    }

    return thread->context->display;
}

EGLAPI EGLBoolean EGLAPIENTRY
eglQueryContext(
                EGLDisplay dpy,
                EGLContext ctx,
                EGLint attribute,
                EGLint *value
                )
{
    PEGLDisplay display = (PEGLDisplay)dpy;
    PEGLContext context = (PEGLContext) ctx;

    return g_opengl->eglQueryContext(display->display, context->context, attribute, value);
}

EGLAPI EGLBoolean EGLAPIENTRY
eglWaitGL(
          void
          )
{
    /* Nothing for Now: WaitGL */
    return g_opengl->eglWaitGL();
}

EGLAPI EGLBoolean EGLAPIENTRY
eglWaitNative(
              EGLint engine
              )
{
    /* Nothing for Now: WaitNative */
    return g_opengl->eglWaitNative(engine);
}

EGLAPI EGLBoolean EGLAPIENTRY
eglSwapBuffers(
               EGLDisplay dpy,
               EGLSurface surface
               )
{
    PEGLDisplay     display = (PEGLDisplay) dpy;
    PEGLSurface     surf    = (PEGLSurface) surface;
    PEGLThreadData  thread  = _GetThreadData();

    if (display == EGL_NO_DISPLAY ||
        display->signature != EGL_DISPLAY_SIGNATURE)
    {
        thread->error = EGL_BAD_DISPLAY;
        return EGL_FALSE;
    }

    if (display->reference == 0)
    {
        thread->error = EGL_NOT_INITIALIZED;
        return EGL_FALSE;
    }

    if (surf == EGL_NO_SURFACE ||
        surf->signature != EGL_SURFACE_SIGNATURE)
    {
        thread->error = EGL_BAD_SURFACE;
        return EGL_FALSE;
    }

    return g_opengl->eglSwapBuffers(display->display, surf->surface);
}

EGLAPI EGLBoolean EGLAPIENTRY
eglCopyBuffers(
               EGLDisplay dpy,
               EGLSurface surface,
               EGLNativePixmapType target
               )
{
    PEGLDisplay     display = (PEGLDisplay)dpy;
    PEGLSurface     surf    = (PEGLSurface) surface;
    PEGLThreadData  thread  = _GetThreadData();

    if (display == EGL_NO_DISPLAY ||
        display->signature != EGL_DISPLAY_SIGNATURE)
    {
        thread->error = EGL_BAD_DISPLAY;
        return EGL_FALSE;
    }

    if (display->reference == 0)
    {
        thread->error = EGL_NOT_INITIALIZED;
        return EGL_FALSE;
    }

    if (surf == EGL_NO_SURFACE ||
        surf->signature != EGL_SURFACE_SIGNATURE)
    {
        thread->error = EGL_BAD_SURFACE;
        return EGL_FALSE;
    }

    return g_opengl->eglCopyBuffers(display->display, surf->surface, target);
}

static PEGLImageKHR _CreateImagePixmap(
                                       PEGLThreadData Thread,
                                       PEGLDisplay Dpy,
                                       PEGLContext    Ctx,
                                       EGLClientBuffer buffer,
                                       const EGLint *attrib_list
                                       )
{
    PEGLImageKHR        image;
    NativePixmapType    pixmap;
    GLuint              width;
    GLuint              height;
    GLuint              format;
    void*               address;
    EGLint              stride;
    PEGLSurface         source;
    DIBSECTION          dib;

    /* Pixmap require context is EGL_NO_CONTEXT. */
    if (Ctx != EGL_NO_CONTEXT)
    {
        Thread->error = EGL_BAD_PARAMETER;
        return (PEGLImageKHR)EGL_NO_IMAGE_KHR;
    }

    /* Parse the attribute list. */
    if (attrib_list != NULL)
    {
        while (*attrib_list != EGL_NONE)
        {
            EGLint attribute = *attrib_list++;

            switch(attribute)
            {
            case EGL_IMAGE_PRESERVED_KHR:
                break;

            default:
                Thread->error = EGL_BAD_PARAMETER;
                return (PEGLImageKHR)EGL_NO_IMAGE_KHR;
            }

            attrib_list++;
        }
    }

    pixmap = (NativePixmapType)(buffer);

    for (source = Dpy->surfaceStack; source != NULL; source = source->next)
    {
        if (source != NULL)
        {
            if ((source->handle == pixmap) && (source->bBounded))
            {
                Thread->error = EGL_BAD_ACCESS;
                return (PEGLImageKHR)EGL_NO_IMAGE_KHR;
            }
        }
    }

    /* Cast buffer to native pixmap type. */
    pixmap = (NativePixmapType)(buffer);

    memset(&dib, 0, sizeof (DIBSECTION));
    dib.dsBmih.biSize = sizeof(dib.dsBmih);

    if (GetObject(pixmap, sizeof(dib), &dib) == 0)
    {
        /* Really bad native pixmap. */
        Thread->error = EGL_BAD_PARAMETER;
        return (PEGLImageKHR)EGL_NO_IMAGE_KHR;
    }

    width   = dib.dsBm.bmWidth;
    height  = dib.dsBm.bmHeight;
    format  = dib.dsBm.bmBitsPixel;
    stride  = dib.dsBm.bmWidthBytes;
    address = dib.dsBm.bmBits;

    /* Initialize an image struct. */
    image = (PEGLImageKHR)malloc(sizeof (struct eglImageKHR));
    if (!image)
    {
        Thread->error = EGL_BAD_ACCESS;
        return (PEGLImageKHR)EGL_NO_IMAGE_KHR;
    }

    image->image.magic  = KHR_EGL_IMAGE_MAGIC_NUM;
    image->image.type   = KHR_IMAGE_PIXMAP;

    image->image.u.pixmap.width   = width;
    image->image.u.pixmap.height  = height;
    image->image.u.pixmap.format  = format;
    image->image.u.pixmap.stride  = stride;
    image->image.u.pixmap.address = address;

    return image;
}

EGLAPI EGLImageKHR EGLAPIENTRY
eglCreateImageKHR(
                  EGLDisplay dpy,
                  EGLContext ctx,
                  EGLenum target,
                  EGLClientBuffer buffer,
                  const EGLint *attrib_list
                  )
{
    PEGLDisplay     display     = (PEGLDisplay) dpy;
    PEGLContext     context     = (PEGLContext) ctx;
    PEGLContext     Ctx         = (PEGLContext)EGL_NO_CONTEXT;
    PEGLThreadData  thread      = _GetThreadData();
    PEGLImageKHR    imageKHR    = (PEGLImageKHR)EGL_NO_IMAGE_KHR;

    /* Test for display. */
    if (display == EGL_NO_DISPLAY ||
        display->signature != EGL_DISPLAY_SIGNATURE)
    {
        thread->error = EGL_BAD_DISPLAY;
        return imageKHR;
    }

    if (display->reference == 0)
    {
        thread->error = EGL_NOT_INITIALIZED;
        return imageKHR;
    }

    /* Test for context. */
    if (context == EGL_NO_CONTEXT)
    {
        Ctx = context;
    }
    else
    {
        for (Ctx = display->contextStack; Ctx != EGL_NO_CONTEXT; Ctx = Ctx->next)
        {
            if (Ctx == (PEGLContext)(ctx))
            {
                break;
            }
        }

        if (Ctx == EGL_NO_CONTEXT)
        {
            thread->error = EGL_BAD_CONTEXT;
            return imageKHR;
        }
    }

    switch(target)
    {
    case EGL_NATIVE_PIXMAP_KHR:
        imageKHR = _CreateImagePixmap(thread, display, Ctx, buffer, attrib_list);
        break;

    case EGL_GL_TEXTURE_2D_KHR:
    case EGL_GL_TEXTURE_CUBE_MAP_POSITIVE_X_KHR:
    case EGL_GL_TEXTURE_CUBE_MAP_NEGATIVE_X_KHR:
    case EGL_GL_TEXTURE_CUBE_MAP_POSITIVE_Y_KHR:
    case EGL_GL_TEXTURE_CUBE_MAP_NEGATIVE_Y_KHR:
    case EGL_GL_TEXTURE_CUBE_MAP_POSITIVE_Z_KHR:
    case EGL_GL_TEXTURE_CUBE_MAP_NEGATIVE_Z_KHR:
    case EGL_GL_RENDERBUFFER_KHR:
    case EGL_VG_PARENT_IMAGE_KHR:

    default:
        /* Not a valid target type. */
        thread->error = EGL_BAD_PARAMETER;
        return imageKHR;
    }

    /* Test if create successful. */
    if (imageKHR == EGL_NO_IMAGE_KHR)
    {
        return imageKHR;
    }

    /* Push image into stack. */
    if (imageKHR->next != NULL)
    {
        PEGLImageKHR img = imageKHR;
        while (img->next != NULL)
        {
            img = img->next;
        }
        img->next = display->imageStack;
    }
    else
    {
        imageKHR->next = display->imageStack;
    }

    display->imageStack = imageKHR;

    /* Todo: implement egl image. */
    imageKHR->eglImage = NULL;

    return (EGLImageKHR)imageKHR;
}

EGLAPI EGLBoolean EGLAPIENTRY
eglDestroyImageKHR (
                    EGLDisplay dpy,
                    EGLImageKHR image
                    )
{
    PEGLDisplay     display     = (PEGLDisplay) dpy;
    PEGLImageKHR    imageKHR    = (PEGLImageKHR)image;
    PEGLImageKHR    stack       = NULL;
    PEGLThreadData  thread      = _GetThreadData();

    /* Test for display. */
    if (display == EGL_NO_DISPLAY ||
        display->signature != EGL_DISPLAY_SIGNATURE)
    {
        thread->error = EGL_BAD_DISPLAY;
        return EGL_FALSE;
    }

    if (display->reference == 0)
    {
        thread->error = EGL_BAD_DISPLAY;
        return EGL_FALSE;
    }

    /* Test for image. */
    if((imageKHR == NULL) || (imageKHR->signature != EGL_IMAGE_SIGNATURE))
    {
        thread->error = EGL_BAD_PARAMETER;
        return EGL_FALSE;
    }

    if (imageKHR == display->imageStack)
    {
        display->imageStack = imageKHR->next;
    }
    else
    {
        /* Walk the stack. */
        for (stack = display->imageStack; stack != NULL; stack = stack->next)
        {
            /* Check if the next image on the stack is ours. */
            if (stack->next == imageKHR)
            {
                /* Pop the image from the stack. */
                stack->next = imageKHR->next;
                break;
            }
        }
    }

    free(imageKHR);
    thread->error = EGL_SUCCESS;
    return EGL_TRUE;
}

EGLAPI EGLSyncKHR EGLAPIENTRY
eglCreateSyncKHR(
                 EGLDisplay dpy,
                 EGLenum type,
                 const EGLint *attrib_list
                 )
{
    PEGLDisplay display = (PEGLDisplay) dpy;

    // Todo
    return NULL;
}

EGLAPI EGLBoolean EGLAPIENTRY
eglDestroySyncKHR(
                  EGLDisplay dpy,
                  EGLSyncKHR syncKHR
                  )
{
    // Todo
    return EGL_TRUE;
}

EGLAPI EGLint EGLAPIENTRY
eglClientWaitSyncKHR(
                     EGLDisplay display,
                     EGLSyncKHR syncKHR,
                     EGLint flags,
                     EGLTimeKHR timeout
                     )
{
    // Todo
    return 1;
}


EGLAPI EGLBoolean EGLAPIENTRY
eglGetSyncAttribKHR(
                    EGLDisplay display,
                    EGLSyncKHR syncKHR,
                    EGLint attribute,
                    EGLint *value)
{
    // Todo
    return EGL_TRUE;
}

static __eglMustCastToProperFunctionPointerType
_Lookup(
        veglLOOKUP * Lookup,
        const char * Name,
        const char * Appendix
        )
{
    /* Test for lookup. */
    if (Lookup != NULL)
    {
        /* Loop while there are entries in the lookup tabke. */
        while (Lookup->name != NULL)
        {
            const char *p = Name;
            const char *q = Lookup->name;

            /* Compare the name and the lookup table. */
            while ((*p == *q) && (*p != '\0') && (*q != '\0'))
            {
                ++p;
                ++q;
            }

            /* No match yet, see if it matches if we append the appendix. */
            if ((*p != *q) && (*p == '\0') && (Appendix != NULL))
            {
                p = Appendix;

                /* Compare the appendix and the lookup table. */
                while ((*p == *q) && (*p != '\0') && (*q != '\0'))
                {
                    ++p;
                    ++q;
                }
            }

            /* See if we have a match. */
            if (*p == *q)
            {
                /* Return the function pointer. */
                return Lookup->function;
            }

            /* Next lookup entry. */
            ++Lookup;
        }
    }

    /* No match found. */
    return NULL;
}

EGLAPI __eglMustCastToProperFunctionPointerType EGLAPIENTRY
eglGetProcAddress(
                  const char *procname
                  )
{
    PEGLThreadData  thread  = _GetThreadData();
    void*           func    = NULL;

    if (procname[0] == '\0')
    {
        /* Handle empty string. */
        return NULL;
    }

    func = _Lookup(_veglLookup, procname, NULL);
    if (NULL != func)
        return (__eglMustCastToProperFunctionPointerType)func;

    return (__eglMustCastToProperFunctionPointerType)GetFuncAddress(thread->context->api, procname);
}


/* -------------- OS Related Functions --------------- */

PEGLThreadData
_GetThreadData(
               void
               )
{
    PEGLThreadData threadData = NULL;

    /* Has the object been allocated? */
    if (g_threadValue == TLS_OUT_OF_INDEXES)
    {
        g_threadValue = TlsAlloc();

        if (g_threadValue == TLS_OUT_OF_INDEXES)
        {
            /* Fatal. */
            return NULL;
        }
    }
    else
    {
        threadData = (PEGLThreadData)TlsGetValue(g_threadValue);
    }

    if (threadData == NULL)
    {
        /* Allocate. */
        threadData = (PEGLThreadData)malloc(sizeof(struct eglThreadData));

        /* Verify allocation. */
        if (threadData == NULL)
        {
            assert(0 && "Fatal Error");
            return NULL;
        }

        /* Initialize */
        threadData->error           = EGL_SUCCESS;

        threadData->api             = EGL_OPENGL_ES_API;
        threadData->clientVersion   = 2;

        threadData->maxWidth        = TARGET_CAPS_MAX_WIDTH;
        threadData->maxHeight       = TARGET_CAPS_MAX_HEIGHT;
        threadData->maxSamples      = TARGET_CAPS_MAX_SAMPLES;

        threadData->context         = (PEGLContext)EGL_NO_CONTEXT;
        threadData->lastClient      = 1;

        /* threadData->g_displayStack = EGL_NO_DISPLAY; */
        threadData->destroying      = EGL_FALSE;

        TlsSetValue(g_threadValue, threadData);
    }

    /* Success. */
    return threadData;
}

void
_DestroyThreadData(
                   void
                   )
{
    PEGLThreadData threadData = _GetThreadData();
    if (threadData != NULL)
    {
        threadData->destroying = EGL_TRUE;
        free(threadData);
        threadData = NULL;
    }
}

void
_ReferenceDisplay(
                  PEGLDisplay dpy
                  )
{
    _SyncEnter();

    /* Inc reference count */
    ++ dpy->reference;

    if (dpy->reference == 1)
    {
        /* first time reference */
        /* Fill in all configs */
        _FillInAll(dpy->hdc, &dpy->config, &dpy->configCount);
    }

    _SyncExit();
}

void
_DereferenceDisplay(
                    PEGLDisplay dpy,
                    EGLBoolean  always
                    )
{
    _SyncEnter();

    if (dpy->reference == 1 || always)
    {
        /* No referece, destroy it. */
        PEGLSurface     surface;
        PEGLContext     context;
        PEGLThreadData  thread = _GetThreadData();

        /* Congfis */
        free(dpy->config);

        /* Surfaces */
        while ((surface = dpy->surfaceStack) != NULL)
        {
            dpy->surfaceStack = surface->next;

            switch (surface->type)
            {
            case EGL_WINDOW_BIT:
                break;

            case EGL_PIXMAP_BIT:
                break;

            case EGL_PBUFFER_BIT:
                break;

            default:
                break;
            }

            free(surface);
        }

        /* Contexts */
        while ((context = dpy->contextStack) != NULL)
        {
            dpy->contextStack = context->next;

            /* If it is currect context. */
            if (thread->context == context)
            {
                thread->context = (PEGLContext)EGL_NO_CONTEXT;
            }

            _VEDestroyContext(context, context->context);

            free(context);
        }

        dpy->reference = 0;
    }
    else
    {
        -- dpy->reference;
    }

    _SyncExit();
}