#include "ApiCore.h"
#include "APIOS.h"
#include "ApiEGLContext.h"

static DWORD    g_threadValue   = TLS_OUT_OF_INDEXES;

/* These resources should be per-process. */
stEngine        *g_opengl       = NULL;
EGLBoolean      g_bFirst        = EGL_TRUE;
HANDLE          g_mutex         = NULL;

static void _DestroyContext(stEglDisplay *display, CEGLContext *ctx, void* Context);

void _SyncEnter()
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

    apiOS_SyncEnter((void*)g_mutex);
}


void _SyncExit()
{
    apiOS_SyncExit((void*)g_mutex);
}

stEglDisplay* _GetDisplay(EGLDisplay dpy)
{
    PEglThreadData  thread = _GetThreadData();
    stEglDisplay    *display = NULL;

    for (display = thread->eglDpyStack; display; display = display->next)
    {
        if (display->display == dpy)
            return display;
    }

    return NULL;
}

CEGLContext* _GetEglContext(EGLDisplay dpy, EGLContext context)
{
    stEglDisplay *display = _GetDisplay(dpy);
    CEGLContext  *ctx = NULL;

    for (ctx = display->eglCtxStack; ctx; ctx = ctx->next)
    {
        if (ctx->context == context)
            return ctx;
    }

    return NULL;
}

stEglSurface* _GetSurface(EGLDisplay dpy, EGLSurface surf)
{
    stEglDisplay *display = _GetDisplay(dpy);
    stEglSurface *surface = NULL;

    for (surface = display->eglSurfStack; surface; surface = surface->next)
    {
        if (surface->surface == surf)
            return surface;
    }

    return NULL;
}

void _ReferenceDisplay(stEglDisplay *dpy)
{
    _SyncEnter();

    /* Inc reference count */
    ++ dpy->reference;

    if (dpy->reference == 1)
    {
    }

    _SyncExit();
}

void _DereferenceDisplay(stEglDisplay *dpy, EGLBoolean always)
{
    _SyncEnter();

    if (dpy->reference == 1 || always)
    {
        /* No reference, destroy it. */
        stEglSurface    *surface;
        stEglConfig     *config;
        CEGLContext     *context;
        PEglThreadData  thread = _GetThreadData();

        /* Configures */
        while ((config = dpy->eglConfStack) != NULL)
        {
            dpy->eglConfStack = config->next;
            delete (config);
        }

        /* Surfaces */
        while ((surface = dpy->eglSurfStack) != NULL)
        {
            dpy->eglSurfStack = surface->next;

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

            delete (surface);
        }

        /* Contexts */
        while ((context = dpy->eglCtxStack) != NULL)
        {
            dpy->eglCtxStack = context->next;

            /* If it is correct context. */
            if (thread->context == context)
            {
                thread->context = (CEGLContext*)EGL_NO_CONTEXT;
            }

            _DestroyContext(dpy, context, context->context);

            delete (context);
        }

        dpy->reference = 0;
    }
    else
    {
        -- dpy->reference;
    }

    _SyncExit();
}

static CEGLContext* _CreateContext(EGLDisplay dpy, EGLConfig config, EGLContext share_context, const EGLint *attrib_list, EGLContext ret)
{
    static EGLint   ctxCount    = 0;
    PEglThreadData  threadData  = _GetThreadData();
    stEglDisplay    *display    = _GetDisplay(dpy);
    CEGLContext     *pCtx       = new CEGLContext();
    EGLint          client      = 0;

    pCtx->context       = ret;
    pCtx->shareCtx      = share_context;
    pCtx->config        = config;
    pCtx->attrib_list   = attrib_list;

    /* Get attribute. */
    if (attrib_list != NULL)
    {
        const EGLint *pAttr = attrib_list;
        while (*pAttr != EGL_NONE)
        {
            switch (*pAttr++)
            {
            case EGL_CONTEXT_CLIENT_VERSION:
                /* Save client. */
                client = *pAttr++;
                break;

            case EGL_CONTEXT_PRIORITY_LEVEL_IMG:
                break;

                /* Pass through for error. */
            default:
                /* Invalid attribute. or not supported */
                goto _Err;
            }
        }

        switch (client)
        {
        case 3:
        case 2:
            pCtx->pGLESContext = new CGLES3Context();
            pCtx->pGLESContext->Initialize();
            pCtx->client = client;
            break;

        default:
            Abort("%s(%d): wrong client(%d)\n", __FUNCTION__, __LINE__);
            break;
        }

        _SyncEnter();
        if (display->eglCtxStack)
        {
            CEGLContext *pTmp = display->eglCtxStack;
            display->eglCtxStack = pCtx;
            pCtx->next = pTmp;
        }
        else
        {
            display->eglCtxStack = pCtx;
        }
        _SyncExit();

        _ReferenceDisplay(display);

        memset(pCtx->name, 0, 32);
        sprintf(pCtx->name, "Context%d", ++ctxCount);

        return pCtx;
    }
    else
    {
        goto _Err;
    }

_Err:
    if (pCtx)
        delete pCtx;

    return NULL;
}

static void _DestroyContext(stEglDisplay *display, CEGLContext *ctx, void* Context)
{
    CEGLContext *pTmp   = display->eglCtxStack;
    CEGLContext *pPrev  = display->eglCtxStack;

    while (pTmp)
    {
        if (pTmp == ctx)
        {
            if (pPrev == pTmp)
            {
                display->eglCtxStack = pTmp->next;
            }
            else
            {
                pPrev->next = pTmp->next;
            }

            _DereferenceDisplay(display, EGL_TRUE);

            pTmp->pGLESContext->Release();
            delete pTmp;
            break;
        }

        pTmp = pTmp->next;
        pPrev = pTmp;
    }
}

stEglThreadData* _GetThreadData(void)
{
    stEglThreadData *threadData = NULL;

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
        threadData = (stEglThreadData*)TlsGetValue(g_threadValue);
    }

    if (threadData == NULL)
    {
        /* Allocate. */
        threadData = (stEglThreadData*)malloc(sizeof(stEglThreadData));

        /* Verify allocation. */
        if (threadData == NULL)
        {
            Abort("%s(%d): Fatal Error.\n", __FUNCTION__, __LINE__);
            return NULL;
        }

        /* Initialize */
        threadData->error           = EGL_SUCCESS;

        threadData->api             = EGL_OPENGL_ES_API;

        threadData->maxWidth        = TARGET_CAPS_MAX_WIDTH;
        threadData->maxHeight       = TARGET_CAPS_MAX_HEIGHT;
        threadData->maxSamples      = TARGET_CAPS_MAX_SAMPLES;

        threadData->context         = (CEGLContext *)EGL_NO_CONTEXT;
        threadData->lastClient      = 1;

        threadData->destroying      = EGL_FALSE;

        threadData->eglDpyStack     = NULL;

        TlsSetValue(g_threadValue, threadData);
    }

    /* Success. */
    return threadData;
}


void _DestroyThreadData(EGLDisplay dpy)
{
    stEglThreadData *threadData = _GetThreadData();
    stEglDisplay    *display    = _GetDisplay(dpy);

    if (threadData != NULL)
    {
        threadData->destroying = EGL_TRUE;

        while (display->eglCtxStack)
        {
            CEGLContext *p = display->eglCtxStack;
            display->eglCtxStack = display->eglCtxStack->next;
            delete p;
        }

        while (display->eglSurfStack)
        {
            stEglSurface *p = display->eglSurfStack;
            display->eglSurfStack = display->eglSurfStack->next;
            delete p;
        }

        while (threadData->eglDpyStack)
        {
            stEglDisplay *p = threadData->eglDpyStack;
            threadData->eglDpyStack = threadData->eglDpyStack->next;
            delete p;
        }

        free(threadData);
        threadData = NULL;
    }
}

void ApiEglGetError(EGLint ret)
{
    PEglThreadData thread = _GetThreadData();
    thread->error = ret;
    CTX_ANALYZER_FUNC1(EGLGetError, EGLOutput, EGL_OUT_BUF_SIZE, ret);
}

void ApiGetDisplay(EGLNativeDisplayType display_id, EGLDisplay ret)
{
    PEglThreadData  threadData  = _GetThreadData();
    stEglDisplay    *display    = NULL;

    for (display = threadData->eglDpyStack; display; display = display->next)
    {
        if (display->displayId == display_id)
            return;
    }

    display = new stEglDisplay;

    display->display    = ret;
    display->displayId  = display_id;

    _SyncEnter();
    if (threadData->eglDpyStack)
    {
        stEglDisplay *pTmp = threadData->eglDpyStack;

        threadData->eglDpyStack = display;
        display->next = pTmp;
    }
    else
    {
        threadData->eglDpyStack = display;
    }
    _SyncExit();

    CTX_ANALYZER_FUNC2(GetDisplay, EGLOutput, EGL_OUT_BUF_SIZE, display_id, ret);
}

void ApiCreateContext(EGLDisplay dpy, EGLConfig config, EGLContext share_context, const EGLint *attrib_list, EGLContext ret)
{
    CEGLContext *pCtx = _CreateContext(dpy, config, share_context, attrib_list, ret);
    CTX_ANALYZER_FUNC5(CreateContext, EGLOutput, EGL_OUT_BUF_SIZE, dpy, config, share_context, attrib_list, ret);
}

void ApiCreateWindowSurface(EGLDisplay dpy, EGLConfig config, EGLNativeWindowType win, const EGLint *attrib_list, EGLSurface ret)
{
    static EGLint   windowCount = 0;
    stEglDisplay    *display    = _GetDisplay(dpy);
    stEglSurface    *pSurf      = new stEglSurface;

    pSurf->type         = SURFACE_TYPE_WINDOWSURFACE;
    pSurf->display      = dpy;
    pSurf->config       = config;
    pSurf->surface      = ret;
    pSurf->attrib_list  = attrib_list;

    if (display->eglSurfStack)
    {
        stEglSurface *pTmp = display->eglSurfStack;
        display->eglSurfStack = pSurf;
        pSurf->next = pTmp;
    }
    else
    {
        display->eglSurfStack = pSurf;
    }

    memset(pSurf->name, 0, 32);
    sprintf(pSurf->name, "WindowSurface%d", ++windowCount);

    CTX_ANALYZER_FUNC5(CreateWindowSurface, EGLOutput, EGL_OUT_BUF_SIZE, dpy, config, win, attrib_list, ret);
}

void ApiCreatePbufferSurface(EGLDisplay dpy, EGLConfig config, const EGLint *attrib_list, EGLSurface ret)
{
    static EGLint   windowCount = 0;
    stEglDisplay    *display    = _GetDisplay(dpy);
    stEglSurface    *pSurf      = new stEglSurface;

    pSurf->type         = SURFACE_TYPE_PBUFFER;
    pSurf->display      = dpy;
    pSurf->config       = config;
    pSurf->surface      = ret;
    pSurf->attrib_list  = attrib_list;

    if (display->eglSurfStack)
    {
        stEglSurface *pTmp = display->eglSurfStack;
        display->eglSurfStack = pSurf;
        pSurf->next = pTmp;
    }
    else
    {
        display->eglSurfStack = pSurf;
    }

    memset(pSurf->name, 0, 32);
    sprintf(pSurf->name, "PBufferSurface%d", ++windowCount);

    CTX_ANALYZER_FUNC4(CreatePbufferSurface, EGLOutput, EGL_OUT_BUF_SIZE, dpy, config, attrib_list, ret);
}

void ApiCreatePixmapSurface(EGLDisplay dpy, EGLConfig config, EGLNativePixmapType pixmap, const EGLint *attrib_list, EGLSurface ret)
{
    Abort("%s(%d)\n", __FUNCTION__, __LINE__);
    CTX_ANALYZER_FUNC5(CreatePixmapSurface, EGLOutput, EGL_OUT_BUF_SIZE, dpy, config, pixmap, attrib_list, ret);
}

void ApiDestroyContext(EGLDisplay dpy, EGLContext context, EGLBoolean ret)
{
    stEglDisplay *display   = _GetDisplay(dpy);
    CEGLContext  *ctx       = _GetEglContext(dpy, context);

    _DestroyContext(display, ctx, ctx->context);

    CTX_ANALYZER_FUNC3(DestroyContext, EGLOutput, EGL_OUT_BUF_SIZE, dpy, context, ret);
}

void ApiDestroySurface(EGLDisplay dpy, EGLSurface surface, EGLBoolean ret)
{
    stEglDisplay    *display    = _GetDisplay(dpy);
    stEglSurface    *pTmp       = display->eglSurfStack;
    stEglSurface    *pPrev      = display->eglSurfStack;

    while (pTmp)
    {
        if (pTmp->surface == surface)
        {
            if (pPrev == pTmp)
            {
                display->eglSurfStack = pTmp->next;
            }
            else
            {
                pPrev->next = pTmp->next;
            }

            delete pTmp;
            break;
        }

        pTmp = pTmp->next;
        pPrev = pTmp;
    }

    CTX_ANALYZER_FUNC3(DestroySurface, EGLOutput, EGL_OUT_BUF_SIZE, dpy, surface, ret);
}

void ApiMakeCurrent(EGLDisplay dpy, EGLSurface draw, EGLSurface read, EGLContext ctx, EGLBoolean ret)
{
    PEglThreadData  threadData  = _GetThreadData();
    stEglDisplay    *display    = _GetDisplay(dpy);
    CEGLContext     *pCtx       = display->eglCtxStack;

    if (ctx)
    {
        for (;pCtx; pCtx = pCtx->next)
        {
            if (pCtx->context == ctx)
            {
                pCtx->read = read;
                pCtx->draw = draw;
                threadData->context = pCtx;
                break;
            }
        }
    }
    else
    {
        pCtx = threadData->context;
        pCtx->read   = NULL;
        pCtx->draw   = NULL;
        threadData->context = NULL;
    }

    CTX_ANALYZER_FUNC5(MakeCurrent, EGLOutput, EGL_OUT_BUF_SIZE, dpy, draw, read, ctx, ret);
}

static EGLBoolean _ParseAttrbutes(EGLDisplay dpy, const EGLint *attrib_list, stEglConfig *attribConfig)
{
    EGLenum attribute;
    EGLint  value;

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
        case EGL_NATIVE_RENDERABLE:
            attribConfig->nativeRenderable = value;
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

        case EGL_MAX_PBUFFER_WIDTH:
            Abort("%s(%d)\n", __FUNCTION__, __LINE__);
            break;
        case EGL_MAX_PBUFFER_HEIGHT:
            Abort("%s(%d)\n", __FUNCTION__, __LINE__);
            break;
        case EGL_MAX_PBUFFER_PIXELS:
            Abort("%s(%d)\n", __FUNCTION__, __LINE__);
            break;
        case EGL_NATIVE_VISUAL_ID:
            Abort("%s(%d)\n", __FUNCTION__, __LINE__);
            break;

        default:
            Abort("%s(%d)\n", __FUNCTION__, __LINE__);
            break;
        }
    }
    while (attribute != EGL_NONE);

    /* Success. */
    return EGL_TRUE;
}

void ApiChooseConfig(EGLDisplay dpy, const EGLint *attrib_list, EGLConfig *configs, EGLint config_size, EGLint *num_config, EGLBoolean ret)
{
    stEglDisplay    *display    = _GetDisplay(dpy);
    const EGLint    *value      = attrib_list;
    stEglConfig     *p          = NULL;
    EGLBoolean      bFinish     = EGL_FALSE;
    EGLint          num         = num_config ? *num_config : 0;

    if (configs == NULL)
    {
        return;
    }

    for (GLint i=0; i<num; i++)
    {
        p = new stEglConfig();

        if (p)
        {
            p->config = configs[i];
            _ParseAttrbutes(dpy, value, p);
        }
        else
        {
            Abort("%s(%d)\n", __FUNCTION__, __LINE__);
            return;
        }

        if (display->eglConfStack)
        {
            stEglConfig *pTmp = display->eglConfStack;
            display->eglConfStack = p;
            p->next = pTmp;
        }
        else
        {
            display->eglConfStack = p;
        }
    }

    CTX_ANALYZER_FUNC6(ChooseConfig, EGLOutput, EGL_OUT_BUF_SIZE, dpy, attrib_list, configs, config_size, num_config, ret);
}

CEGLContext::CEGLContext()
{
    draw        = NULL;
    read        = NULL;
    addr        = 0;
    context     = NULL;
    shareCtx    = NULL;
    config      = NULL;
    next        = NULL;
    nFrame      = 1;
    error       = EGL_SUCCESS;
    client      = 1;

    memset(name, 0, 32);
}

CEGLContext::~CEGLContext()
{
}

void ApiTerminate(EGLDisplay dpy, EGLBoolean ret)
{
    stEglDisplay    *display    = _GetDisplay(dpy);
    PEglThreadData  threadData  = _GetThreadData();

    _DereferenceDisplay(display, EGL_TRUE);

    /* destroy display if no referenced */
    if (display->reference == 0)
    {
        _SyncEnter();

        /* remove dpy in g_displayStack */
        if (display == threadData->eglDpyStack)
        {
            /* header */
            threadData->eglDpyStack = display->next;
        }
        else
        {
            stEglDisplay *stack;

            for (stack = threadData->eglDpyStack; stack->next != NULL; stack = stack->next)
            {
                if (stack->next == display)
                {
                    stack->next = display->next;
                    break;
                }
            }
        }

        delete (display);

        _SyncExit();
    }

    CTX_ANALYZER_FUNC2(Terminate, EGLOutput, EGL_OUT_BUF_SIZE, dpy, ret);
}

void ApiInitialize(EGLDisplay dpy, EGLint *major, EGLint *minor, EGLBoolean ret)
{
    stEglDisplay *display = _GetDisplay(dpy);
    _ReferenceDisplay(display);

    CTX_ANALYZER_FUNC4(Initialize, EGLOutput, EGL_OUT_BUF_SIZE, dpy, major, minor, ret);
}

void ApiCopyBuffers(EGLDisplay dpy, EGLSurface surface, EGLNativePixmapType target, EGLBoolean ret)
{
    CTX_ANALYZER_FUNC4(CopyBuffers, EGLOutput, EGL_OUT_BUF_SIZE, dpy, surface, target, ret);
}

void ApiGetConfigAttrib(EGLDisplay dpy, EGLConfig config, EGLint attribute, EGLint *value, EGLBoolean ret)
{
    CTX_ANALYZER_FUNC5(GetConfigAttrib, EGLOutput, EGL_OUT_BUF_SIZE, dpy, config, attribute, value, ret);
}

void ApiGetConfigs(EGLDisplay dpy, EGLConfig *configs, EGLint config_size, EGLint *num_config, EGLBoolean ret)
{
    CTX_ANALYZER_FUNC5(GetConfigs, EGLOutput, EGL_OUT_BUF_SIZE, dpy, configs, config_size, num_config, ret);
}

void ApiGetCurrentDisplay(EGLDisplay ret)
{
    CTX_ANALYZER_FUNC1(GetCurrentDisplay, EGLOutput, EGL_OUT_BUF_SIZE, ret);
}

void ApiGetCurrentSurface(EGLint readdraw, EGLSurface ret)
{
    CTX_ANALYZER_FUNC2(GetCurrentSurface, EGLOutput, EGL_OUT_BUF_SIZE, readdraw, ret);
}

void ApiSetFIFO(void* fifo, EGLBoolean ret)
{
    CTX_ANALYZER_FUNC2(SetFIFO, EGLOutput, EGL_OUT_BUF_SIZE, fifo, ret);
}

void ApiGetProcAddress(const char *procname, void *func)
{
    CTX_ANALYZER_FUNC2(GetProcAddress, EGLOutput, EGL_OUT_BUF_SIZE, procname, func);
}

void ApiQueryContext(EGLDisplay dpy, EGLContext ctx, EGLint attribute, EGLint *value, EGLBoolean ret)
{
    CTX_ANALYZER_FUNC5(QueryContext, EGLOutput, EGL_OUT_BUF_SIZE, dpy, ctx, attribute, value, ret);
}

void ApiQueryString(EGLDisplay dpy, EGLint name, const char *ret)
{
    CTX_ANALYZER_FUNC3(QueryString, EGLOutput, EGL_OUT_BUF_SIZE, dpy, name, ret);
}

void ApiQuerySurface(EGLDisplay dpy, EGLSurface surface, EGLint attribute, EGLint *value, EGLBoolean ret)
{
    CTX_ANALYZER_FUNC5(QuerySurface, EGLOutput, EGL_OUT_BUF_SIZE, dpy, surface, attribute, value, ret);
}

void ApiSwapBuffers(EGLDisplay dpy, EGLSurface surface, EGLBoolean ret)
{
    CTX_ANALYZER_FUNC3(SwapBuffers, Output, OUT_BUF_SIZE, dpy, surface, ret);
}

void ApiWaitGL(EGLBoolean ret)
{
    CTX_ANALYZER_FUNC1(WaitGL, EGLOutput, EGL_OUT_BUF_SIZE, ret);
}

void ApiWaitNative(EGLint engine, EGLBoolean ret)
{
    CTX_ANALYZER_FUNC2(WaitNative, EGLOutput, EGL_OUT_BUF_SIZE, engine, ret);
}

void ApiBindAPI(EGLenum api, EGLBoolean ret)
{
    CTX_ANALYZER_FUNC2(BindAPI, EGLOutput, EGL_OUT_BUF_SIZE, api, ret);
}

void ApiQueryAPI(EGLenum ret)
{
    CTX_ANALYZER_FUNC1(QueryAPI, EGLOutput, EGL_OUT_BUF_SIZE, ret);
}

void ApiCreatePbufferFromClientBuffer(EGLDisplay dpy, EGLenum buftype, EGLClientBuffer buffer, EGLConfig config, const EGLint *attrib_list, EGLSurface ret)
{
    CTX_ANALYZER_FUNC6(CreatePbufferFromClientBuffer, EGLOutput, EGL_OUT_BUF_SIZE, dpy, buftype, buffer, config, attrib_list, ret);
}

void ApiReleaseThread(EGLBoolean ret)
{
    CTX_ANALYZER_FUNC1(ReleaseThread, EGLOutput, EGL_OUT_BUF_SIZE, ret);
}

void ApiWaitClient(EGLBoolean ret)
{
    CTX_ANALYZER_FUNC1(WaitClient, EGLOutput, EGL_OUT_BUF_SIZE, ret);
}

void ApiBindTexImage(EGLDisplay dpy, EGLSurface surface, EGLint buffer, EGLBoolean ret)
{
    CTX_ANALYZER_FUNC4(BindTexImage, EGLOutput, EGL_OUT_BUF_SIZE, dpy, surface, buffer, ret);
}

void ApiReleaseTexImage(EGLDisplay dpy, EGLSurface surface, EGLint buffer, EGLBoolean ret)
{
    CTX_ANALYZER_FUNC4(ReleaseTexImage, EGLOutput, EGL_OUT_BUF_SIZE, dpy, surface, buffer, ret);
}

void ApiSurfaceAttrib(EGLDisplay dpy, EGLSurface surface, EGLint attribute, EGLint value, EGLBoolean ret)
{
    CTX_ANALYZER_FUNC5(SurfaceAttrib, EGLOutput, EGL_OUT_BUF_SIZE, dpy, surface, attribute, value, ret);
}

void ApiSwapInterval(EGLDisplay dpy, EGLint interval, EGLBoolean ret)
{
    CTX_ANALYZER_FUNC3(SwapInterval, EGLOutput, EGL_OUT_BUF_SIZE, dpy, interval, ret);
}

void ApiGetCurrentContext(EGLContext ret)
{
    CTX_ANALYZER_FUNC1(GetCurrentContext, EGLOutput, EGL_OUT_BUF_SIZE, ret);
}
