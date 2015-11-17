#include "egl.h"
#include "ConvertApiDump.h"
#include "utils.h"
#include "MathAPI.h"
#include "ApiAnalyzer.h"
#include "ParseEGL.h"
#include "ApiEGLContext.h"

EGLDisplay      eglDisplay      = NULL;
EGLContext      eglContext      = NULL;
EGLSurface      eglSurface      = NULL;
GLvoid          *wnd            = NULL;


#ifdef WIN32
/* Register class. */
static ATOM     regClass;

LRESULT CALLBACK
_WindowProc(
    HWND Window,
    UINT Message,
    WPARAM ParameterW,
    LPARAM ParameterL
    )
{
    /* We do nothing here - just return the default method for the message. */
    return DefWindowProc(Window, Message, ParameterW, ParameterL);
}
#endif

GLvoid EGLRelease()
{
    CEGLContext *pTmp = NULL;

    while (g_eglCtxStack)
    {
        pTmp = g_eglCtxStack;
        g_eglCtxStack = g_eglCtxStack->next;
        delete pTmp;
    }
}

GLvoid EGLInit()
{
}

stEglSurface* eglGetFreeSurfaceObject(stEglSurface *surfaceArray, EGLint *surfaceCount)
{
    stEglSurface    *p      = NULL;
    EGLint          used    = *surfaceCount;

    for (GLint i=used; i<SURFACE_OBJECT_NUM; i++)
    {
        p = &surfaceArray[i];
        if (p->bUsed == EGL_FALSE)
        {
            *surfaceCount += 1;
            return p;
        }
    }

    for (GLint i=0; i<used; i++)
    {
        p = &surfaceArray[i];
        if (p->bUsed == EGL_FALSE)
        {
            *surfaceCount += 1;
            return p;
        }
    }

    return p;
}

stEglSurface* eglGetSurfaceObject(stEglSurface *surfaceArray, EGLint surfaceCount, EGLint addr)
{
    stEglSurface    *p      = NULL;
    EGLint          used    = surfaceCount;

    for (GLint i=used; i>=0; i--)
    {
        p = &surfaceArray[i];
        if (p->bUsed == EGL_TRUE && p->addr == addr)
        {
            return p;
        }
    }

    for (GLint i=used; i<SURFACE_OBJECT_NUM; i++)
    {
        p = &surfaceArray[i];
        if (p->bUsed == EGL_TRUE && p->addr == addr)
        {
            return p;
        }
    }

    return NULL;
}

CEGLContext* eglGetFreeEGLContext(CEGLContext *contexts, EGLint *contextNum)
{
    CEGLContext *p      = NULL;
    EGLint      used    = *contextNum;

    for (GLint i=used; i<EGL_CONTEXT_NUM; i++)
    {
        p = &contexts[i];
        if (p->bUsed == EGL_FALSE)
        {
            *contextNum += 1;
            return p;
        }
    }

    for (GLint i=0; i<used; i++)
    {
        p = &contexts[i];
        if (p->bUsed == EGL_FALSE)
        {
            *contextNum += 1;
            return p;
        }
    }

    return p;
}

CEGLContext* eglGetEGLContext(CEGLContext *contexts, EGLint contextNum, EGLint addr)
{
    stEglContext    *p      = NULL;
    EGLint          used    = contextNum;

    for (GLint i=used; i>=0; i--)
    {
        p = &contexts[i];
        if (p->bUsed == EGL_TRUE && p->addr == addr)
        {
            return p;
        }
    }

    for (GLint i=used; i<EGL_CONTEXT_NUM; i++)
    {
        p = &contexts[i];
        if (p->bUsed == EGL_TRUE && p->addr == addr)
        {
            return p;
        }
    }

    return NULL;
}

EGL_CONFIG* eglGetFreeEGLConfig(EGL_CONFIG *configs, EGLint *configNum)
{
    EGL_CONFIG *p       = NULL;
    EGLint      used    = *configNum;

    for (GLint i=used; i<EGL_CONFIG_NUM; i++)
    {
        p = &configs[i];
        if (p->bUsed == EGL_FALSE)
        {
            *configNum += 1;
            return p;
        }
    }

    for (GLint i=0; i<used; i++)
    {
        p = &configs[i];
        if (p->bUsed == EGL_FALSE)
        {
            *configNum += 1;
            return p;
        }
    }

    return p;
}

EGL_CONFIG* eglGetEGLConfig(EGL_CONFIG *configs, EGLint configNum, EGLint addr)
{
    EGL_CONFIG *p       = NULL;
    EGLint      used    = configNum;

    for (GLint i=used; i>=0; i--)
    {
        p = &configs[i];
        if (p->bUsed == EGL_TRUE && p->addr == addr)
        {
            return p;
        }
    }

    for (GLint i=used; i<EGL_CONFIG_NUM; i++)
    {
        p = &configs[i];
        if (p->bUsed == EGL_TRUE && p->addr == addr)
        {
            return p;
        }
    }

    return NULL;
}

GLboolean eglDisassemble(GLchar *input, GLchar *output, GLint outputSize)
{
    memset(output, 0, outputSize);

    switch (input[9])
    {
    case 'B':
        PROCESS_EGL_INPUT(BindAPI);
        PROCESS_EGL_INPUT(BindTexImage);
        break;

    case 'C':
        PROCESS_EGL_INPUT(CreateContext);
        PROCESS_EGL_INPUT(ChooseConfig);
        PROCESS_EGL_INPUT(CreateImageKHR);
        PROCESS_EGL_INPUT(CreateSyncKHR);
        PROCESS_EGL_INPUT(ClientWaitSyncKHR);
        PROCESS_EGL_INPUT(CopyBuffers);
        PROCESS_EGL_INPUT(CreateWindowSurface);
        PROCESS_EGL_INPUT(CreatePbufferSurface);
        PROCESS_EGL_INPUT(CreatePbufferFromClientBuffer);
        PROCESS_EGL_INPUT(CreatePixmapSurface);
        break;

    case 'D':
        PROCESS_EGL_INPUT(DestroySurface);
        PROCESS_EGL_INPUT(DestroyContext);
        PROCESS_EGL_INPUT(DestroyImageKHR);
        PROCESS_EGL_INPUT(DestroySyncKHR);
        break;

    case 'G':
        PROCESS_EGL_INPUT(GetConfigs);
        PROCESS_EGL_INPUT(GetConfigAttrib);
        PROCESS_EGL_INPUT(EGLGetError);
        PROCESS_EGL_INPUT(GetDisplay);
        PROCESS_EGL_INPUT(GetCurrentContext);
        PROCESS_EGL_INPUT(GetCurrentSurface);
        PROCESS_EGL_INPUT(GetCurrentDisplay);
        PROCESS_EGL_INPUT(GetSyncAttribKHR);
        PROCESS_EGL_INPUT(GetProcAddress);
        break;

    case 'I':
        PROCESS_EGL_INPUT(Initialize);
        break;

    case 'L':
        PROCESS_EGL_INPUT(LockSurfaceKHR);
        break;

    case 'M':
        PROCESS_EGL_INPUT(MakeCurrent);
        break;

    case 'Q':
        PROCESS_EGL_INPUT(QueryAPI);
        PROCESS_EGL_INPUT(QueryString);
        PROCESS_EGL_INPUT(QueryContext);
        PROCESS_EGL_INPUT(QuerySurface);
        break;

    case 'R':
        PROCESS_EGL_INPUT(ReleaseThread);
        PROCESS_EGL_INPUT(ReleaseTexImage);
        break;

    case 'S':
        PROCESS_EGL_INPUT(SwapBuffersRegionEXT);
        PROCESS_EGL_INPUT(SwapRectangleANDROID);
        PROCESS_EGL_INPUT(SwapBuffers);
        PROCESS_EGL_INPUT(SwapInterval);
        PROCESS_EGL_INPUT(SurfaceAttrib);
        break;

    case 'T':
        PROCESS_EGL_INPUT(Terminate);
        break;

    case 'U':
        PROCESS_EGL_INPUT(UnlockSurfaceKHR);
        break;

    case 'W':
        PROCESS_EGL_INPUT(WaitNative);
        PROCESS_EGL_INPUT(WaitClient);
        break;

    default:
        break;
    }

    return true;
}

GLvoid TranslateEGLEnum(EGLenum value, GLchar *output)
{
    memset(output, 0, TMP_BUF_SIZE);

    switch (value)
    {
    case EGL_SYNC_REUSABLE_KHR:
        sprintf(output, "EGL_SYNC_REUSABLE_KHR");
        break;

    case EGL_SYNC_FENCE_KHR:
        sprintf(output, "EGL_SYNC_FENCE_KHR");
        break;

    case EGL_SYNC_NATIVE_FENCE_ANDROID:
        sprintf(output, "EGL_SYNC_NATIVE_FENCE_ANDROID");
        break;

    case EGL_SYNC_FLUSH_COMMANDS_BIT_KHR:
        sprintf(output, "EGL_SYNC_FLUSH_COMMANDS_BIT_KHR");
        break;

    case EGL_CONDITION_SATISFIED_KHR:
        sprintf(output, "EGL_CONDITION_SATISFIED_KHR");
        break;

    case EGL_TIMEOUT_EXPIRED_KHR:
        sprintf(output, "EGL_TIMEOUT_EXPIRED_KHR");
        break;

    default:
        sprintf(output, "EGL_NO_SYNC_KHR");
        break;
    }
}

GLvoid TranslateEGLAttrib(EGLenum attr, GLchar *output)
{
    const GLchar *ptr;

    switch (attr)
    {
    case EGL_CONTEXT_PRIORITY_LEVEL_IMG:
        ptr = "EGL_CONTEXT_PRIORITY_LEVEL_IMG";
        break;

    case EGL_CONTEXT_CLIENT_VERSION:
        ptr = "EGL_CONTEXT_CLIENT_VERSION";
        break;

    case EGL_BUFFER_SIZE:
        ptr = "EGL_BUFFER_SIZE";
        break;
    
    case EGL_ALPHA_SIZE:
        ptr = "EGL_ALPHA_SIZE";
        break;
    
    case EGL_BLUE_SIZE:
        ptr = "EGL_BLUE_SIZE";
        break;
    
    case EGL_GREEN_SIZE:
        ptr = "EGL_GREEN_SIZE";
        break;
    
    case EGL_RED_SIZE:
        ptr = "EGL_RED_SIZE";
        break;
    
    case EGL_DEPTH_SIZE:
        ptr = "EGL_DEPTH_SIZE";
        break;
    
    case EGL_STENCIL_SIZE:
        ptr = "EGL_STENCIL_SIZE";
        break;
    
    case EGL_CONFIG_CAVEAT:
        ptr = "EGL_CONFIG_CAVEAT";
        break;
    
    case EGL_CONFIG_ID:
        ptr = "EGL_CONFIG_ID";
        break;
    
    case EGL_LEVEL:
        ptr = "EGL_LEVEL";
        break;
    
    case EGL_MAX_PBUFFER_WIDTH:
        ptr = "EGL_MAX_PBUFFER_WIDTH";
        break;
    
    case EGL_MAX_PBUFFER_HEIGHT:
        ptr = "EGL_MAX_PBUFFER_HEIGHT";
        break;
    
    case EGL_MAX_PBUFFER_PIXELS:
        ptr = "EGL_MAX_PBUFFER_PIXELS";
        break;
    
    case EGL_NATIVE_RENDERABLE:
        ptr = "EGL_NATIVE_RENDERABLE";
        break;
    
    case EGL_NATIVE_VISUAL_ID:
        ptr = "EGL_NATIVE_VISUAL_ID";
        break;
    
    case EGL_NATIVE_VISUAL_TYPE:
        ptr = "EGL_NATIVE_VISUAL_TYPE";
        break;
    
    //case EGL_PRESERVED_RESOURCES:
    //    ptr = "EGL_PRESERVED_RESOURCES";
    //    break;
    
    case EGL_SAMPLES:
        ptr = "EGL_SAMPLES";
        break;
    
    case EGL_SAMPLE_BUFFERS:
        ptr = "EGL_SAMPLE_BUFFERS";
        break;
    
    case EGL_SURFACE_TYPE:
        ptr = "EGL_SURFACE_TYPE";
        break;
    
    case EGL_TRANSPARENT_TYPE:
        ptr = "EGL_TRANSPARENT_TYPE";
        break;
    
    case EGL_TRANSPARENT_BLUE_VALUE:
        ptr = "EGL_TRANSPARENT_BLUE_VALUE";
        break;
    
    case EGL_TRANSPARENT_GREEN_VALUE:
        ptr = "EGL_TRANSPARENT_GREEN_VALUE";
        break;
    
    case EGL_TRANSPARENT_RED_VALUE:
        ptr = "EGL_SAMPLE_BUFFERS";
        break;
    
    case EGL_BIND_TO_TEXTURE_RGB:
        ptr = "EGL_BIND_TO_TEXTURE_RGB";
        break;
    
    case EGL_BIND_TO_TEXTURE_RGBA:
        ptr = "EGL_BIND_TO_TEXTURE_RGBA";
        break;
    
    case EGL_MIN_SWAP_INTERVAL:
        ptr = "EGL_MIN_SWAP_INTERVAL";
        break;
    
    case EGL_MAX_SWAP_INTERVAL:
        ptr = "EGL_MAX_SWAP_INTERVAL";
        break;
    
    case EGL_LUMINANCE_SIZE:
        ptr = "EGL_LUMINANCE_SIZE";
        break;
    
    case EGL_ALPHA_MASK_SIZE:
        ptr = "EGL_ALPHA_MASK_SIZE";
        break;
    
    case EGL_COLOR_BUFFER_TYPE:
        ptr = "EGL_COLOR_BUFFER_TYPE";
        break;
    
    case EGL_RENDERABLE_TYPE:
        ptr = "EGL_RENDERABLE_TYPE";
        break;
    
    case EGL_CONFORMANT:
        ptr = "EGL_CONFORMANT";
        break;
    
    case EGL_MATCH_NATIVE_PIXMAP:
        ptr = "EGL_MATCH_NATIVE_PIXMAP";
        break;
    
    case EGL_RECORDABLE_ANDROID:
        ptr = "EGL_RECORDABLE_ANDROID";
        break;

    case EGL_RENDER_BUFFER:
        ptr = "EGL_RENDER_BUFFER";
        break;

    case EGL_BACK_BUFFER:
        ptr = "EGL_BACK_BUFFER";
        break;

    case EGL_SINGLE_BUFFER:
        ptr = "EGL_SINGLE_BUFFER";
        break;

    default:
        /* Dereference the EGLDisplay. */
        ptr = "Invalid";
        break;
    }

    if (output)
    {
        strcpy(output, ptr);
    }
}

GLvoid TranslateEglError(GLint error, GLchar *output)
{
    memset(output, 0, TMP_BUF_SIZE);

    switch (error)
    {
    case EGL_SUCCESS:
        sprintf(output, "EGL_SUCCESS");
        break;

    case EGL_NOT_INITIALIZED:
        sprintf(output, "EGL_NOT_INITIALIZED");
        break;

    case EGL_BAD_ACCESS:
        sprintf(output, "EGL_BAD_ACCESS");
        break;

    case EGL_BAD_ALLOC:
        sprintf(output, "EGL_BAD_ALLOC");
        break;

    case EGL_BAD_ATTRIBUTE:
        sprintf(output, "EGL_BAD_ATTRIBUTE");
        break;

    case EGL_BAD_CONFIG:
        sprintf(output, "EGL_BAD_CONFIG");
        break;

    case EGL_BAD_CONTEXT:
        sprintf(output, "EGL_BAD_CONTEXT");
        break;

    case EGL_BAD_CURRENT_SURFACE:
        sprintf(output, "EGL_BAD_CURRENT_SURFACE");
        break;

    case EGL_BAD_DISPLAY:
        sprintf(output, "EGL_BAD_DISPLAY");
        break;

    case EGL_BAD_MATCH:
        sprintf(output, "EGL_BAD_MATCH");
        break;

    case EGL_BAD_NATIVE_PIXMAP:
        sprintf(output, "EGL_BAD_NATIVE_PIXMAP");
        break;

    case EGL_BAD_NATIVE_WINDOW:
        sprintf(output, "EGL_BAD_NATIVE_WINDOW");
        break;

    case EGL_BAD_PARAMETER:
        sprintf(output, "EGL_BAD_PARAMETER");
        break;

    case EGL_BAD_SURFACE:
        sprintf(output, "EGL_BAD_SURFACE");
        break;

    case EGL_CONTEXT_LOST:
        sprintf(output, "EGL_CONTEXT_LOST");
        break;

    default:
        sprintf(output, "Unknow error: %x", error);
        break;
    }
}

GLvoid RunEglGetDisplay(GLint arg[10])
{
#ifdef OPENGL_W
    HDC hDC;

    wglCreateContext(hDC);
#elif VIVANTE_CMODEL

#endif
}