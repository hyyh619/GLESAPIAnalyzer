#include "ApiCore.h"
#include "EGLAPI.h"
#include "ES30API.h"
#include "ApiGLES3Context.h"
#include "ApiEGLContext.h"

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

#ifndef API_DUMP

EGLAPI EGLint EGLAPIENTRY
eglGetError(
            void
            )
{
    EGLenum ret = g_opengl->eglGetError();
    ApiEglGetError(ret);
    return ret;
}

EGLAPI EGLDisplay EGLAPIENTRY
eglGetDisplay(
              EGLNativeDisplayType display_id
              )
{
    _SyncEnter();
    EGLDisplay  display = g_opengl->eglGetDisplay(display_id);
    ApiGetDisplay(display_id, display);
    _SyncExit();

    return (EGLDisplay)display;
}

EGLAPI EGLBoolean EGLAPIENTRY
eglInitialize(
              EGLDisplay dpy,
              EGLint *major,
              EGLint *minor
              )
{
    EGLBoolean ret = g_opengl->eglInitialize(dpy, major, minor);
    ApiInitialize(dpy, major, minor, ret);
    return ret;
}

EGLAPI EGLBoolean EGLAPIENTRY
eglTerminate(
             EGLDisplay dpy
             )
{
    EGLBoolean ret = g_opengl->eglTerminate(dpy);
    ApiTerminate(dpy, ret);
    return ret;
}


EGLAPI const char *EGLAPIENTRY
eglQueryString(
               EGLDisplay dpy,
               EGLint name
               )
{
    const char *ret = g_opengl->eglQueryString(dpy, name);
    ApiQueryString(dpy, name, ret);
    return ret;
}

EGLAPI EGLBoolean EGLAPIENTRY
eglGetConfigs(
              EGLDisplay dpy,
              EGLConfig *configs,
              EGLint config_size,
              EGLint *num_config
              )
{
    EGLBoolean ret = g_opengl->eglGetConfigs(dpy, configs, config_size, num_config);
    ApiGetConfigs(dpy, configs, config_size, num_config, ret);
    return ret;
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
    EGLBoolean ret = g_opengl->eglChooseConfig(dpy, attrib_list, configs, config_size, num_config);
    ApiChooseConfig(dpy, attrib_list, configs, config_size, num_config, ret);
    return ret;
}


EGLAPI EGLBoolean EGLAPIENTRY
eglGetConfigAttrib(
                   EGLDisplay dpy,
                   EGLConfig config,
                   EGLint attribute,
                   EGLint *value
                   )
{
    EGLBoolean ret = g_opengl->eglGetConfigAttrib(dpy, config, attribute, value);
    ApiGetConfigAttrib(dpy, config, attribute, value, ret);
    return ret;
}

EGLAPI EGLSurface EGLAPIENTRY
eglCreateWindowSurface(
                       EGLDisplay dpy,
                       EGLConfig config,
                       EGLNativeWindowType win,
                       const EGLint *attrib_list
                       )
{
    EGLSurface  ret = g_opengl->eglCreateWindowSurface(dpy, config, win, attrib_list);
    ApiCreateWindowSurface(dpy, config, win, attrib_list, ret);
    return ret;
}

EGLAPI EGLSurface EGLAPIENTRY
eglCreatePbufferSurface(EGLDisplay dpy,
                        EGLConfig config,
                        const EGLint *attrib_list)
{
    EGLSurface  ret = g_opengl->eglCreatePbufferSurface(dpy, config, attrib_list);
    ApiCreatePbufferSurface(dpy, config, attrib_list, ret);
    return ret;
}

EGLAPI EGLSurface EGLAPIENTRY
eglCreatePixmapSurface(EGLDisplay dpy,
                       EGLConfig config,
                       EGLNativePixmapType pixmap,
                       const EGLint *attrib_list)
{
    EGLSurface ret = g_opengl->eglCreatePixmapSurface(dpy, config, pixmap, attrib_list);
    ApiCreatePixmapSurface(dpy, config, pixmap, attrib_list, ret);
    return ret;
}

EGLAPI EGLBoolean EGLAPIENTRY
eglDestroySurface(
                  EGLDisplay dpy,
                  EGLSurface surface
                  )
{
    EGLBoolean  ret = g_opengl->eglDestroyContext(dpy, surface);
    ApiDestroySurface(dpy, surface, ret);
    return ret;
}

EGLAPI EGLBoolean EGLAPIENTRY
eglBindAPI(
           EGLenum api
           )
{
    EGLBoolean  ret = g_opengl->eglBindAPI(api);
    ApiBindAPI(api, ret);
    return ret;
}

EGLAPI EGLenum EGLAPIENTRY
eglQueryAPI(
            void
            )
{
    EGLenum ret = g_opengl->eglQueryAPI();
    ApiQueryAPI(ret);
    return ret;
}

EGLAPI EGLBoolean EGLAPIENTRY
eglWaitClient(
              void
              )
{
    EGLBoolean ret = g_opengl->eglWaitClient();
    ApiWaitClient(ret);
    return ret;
}

EGLAPI EGLBoolean EGLAPIENTRY
eglReleaseThread(
                 void
                 )
{
    EGLBoolean ret = g_opengl->eglReleaseThread();
    ApiReleaseThread(ret);
    return ret;
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
    EGLSurface ret = g_opengl->eglCreatePbufferFromClientBuffer(dpy, buftype, buffer, config, attrib_list);
    ApiCreatePbufferFromClientBuffer(dpy, buftype, buffer, config, attrib_list, ret);
    return ret;
}

EGLAPI EGLBoolean EGLAPIENTRY
eglQuerySurface(
                EGLDisplay dpy,
                EGLSurface surface,
                EGLint attribute,
                EGLint *value
                )
{
    EGLBoolean ret = g_opengl->eglQuerySurface(dpy, surface, attribute, value);
    ApiQuerySurface(dpy, surface, attribute, value, ret);
    return ret;
}

EGLAPI EGLBoolean EGLAPIENTRY
eglSurfaceAttrib(
                 EGLDisplay dpy,
                 EGLSurface Surface,
                 EGLint attribute,
                 EGLint value
                 )
{
    EGLBoolean ret = g_opengl->eglSurfaceAttrib(dpy, Surface, attribute, value);
    ApiSurfaceAttrib(dpy, Surface, attribute, value, ret);
    return ret;
}

EGLAPI EGLBoolean EGLAPIENTRY
eglBindTexImage(
                EGLDisplay dpy,
                EGLSurface surface,
                EGLint buffer
                )
{
    EGLBoolean ret = g_opengl->eglBindTexImage(dpy, surface, buffer);
    ApiBindTexImage(dpy, surface, buffer, ret);
    return ret;
}

EGLAPI EGLBoolean EGLAPIENTRY
eglReleaseTexImage(
                   EGLDisplay dpy,
                   EGLSurface Surface,
                   EGLint buffer
                   )
{
    EGLBoolean ret = g_opengl->eglReleaseTexImage(dpy, Surface, buffer);
    ApiReleaseTexImage(dpy, Surface, buffer, ret);
    return ret;
}

EGLAPI EGLBoolean EGLAPIENTRY
eglSwapInterval(
                EGLDisplay dpy,
                EGLint Interval
                )
{
    EGLBoolean ret = g_opengl->eglSwapInterval(dpy, Interval);
    ApiSwapInterval(dpy, Interval, ret);
    return ret;
}

EGLAPI EGLContext EGLAPIENTRY
eglCreateContext(EGLDisplay dpy,
                 EGLConfig config,
                 EGLContext share_context,
                 const EGLint *attrib_list)
{
    EGLContext ctx = g_opengl->eglCreateContext(dpy, config, share_context, attrib_list);
    ApiCreateContext(dpy, config, share_context, attrib_list, ctx);
    return ctx;
}

EGLAPI EGLBoolean EGLAPIENTRY
eglDestroyContext(EGLDisplay dpy,
                  EGLContext ctx)
{
    EGLBoolean ret = g_opengl->eglDestroyContext(dpy, ctx);
    ApiDestroyContext(dpy, ctx, ret);
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
    EGLBoolean ret = g_opengl->eglMakeCurrent(dpy, draw, read, ctx);
    ApiMakeCurrent(dpy, draw, read, ctx, ret);
    return ret;
}

EGLAPI EGLContext EGLAPIENTRY
eglGetCurrentContext(
                     void
                     )
{
    EGLContext ctx = g_opengl->eglGetCurrentContext();
    ApiGetCurrentContext(ctx);
    return ctx;
}

EGLAPI EGLSurface EGLAPIENTRY
eglGetCurrentSurface(
                     EGLint readdraw
                     )
{
    EGLSurface ret = g_opengl->eglGetCurrentSurface(readdraw);
    ApiGetCurrentSurface(readdraw, ret);
    return ret;
}

EGLAPI EGLDisplay EGLAPIENTRY
eglGetCurrentDisplay(
                     void
                     )
{
    EGLDisplay ret = g_opengl->eglGetCurrentDisplay();
    ApiGetCurrentDisplay(ret);
    return ret;
}

EGLAPI EGLBoolean EGLAPIENTRY
eglQueryContext(
                EGLDisplay dpy,
                EGLContext ctx,
                EGLint attribute,
                EGLint *value
                )
{
    EGLBoolean ret = g_opengl->eglQueryContext(dpy, ctx, attribute, value);
    ApiQueryContext(dpy, ctx, attribute, value, ret);
    return ret;
}

EGLAPI EGLBoolean EGLAPIENTRY
eglWaitGL(
          void
          )
{
    EGLBoolean ret = g_opengl->eglWaitGL();
    ApiWaitGL(ret);
    return ret;
}

EGLAPI EGLBoolean EGLAPIENTRY
eglWaitNative(
              EGLint engine
              )
{
    EGLBoolean ret = g_opengl->eglWaitNative(engine);
    ApiWaitNative(engine, ret);
    return ret;
}

EGLAPI EGLBoolean EGLAPIENTRY
eglSwapBuffers(
               EGLDisplay dpy,
               EGLSurface surface
               )
{
    EGLBoolean ret = g_opengl->eglSwapBuffers(dpy, surface);
    ApiSwapBuffers(dpy, surface, ret);
    return ret;
}

EGLAPI EGLBoolean EGLAPIENTRY
eglCopyBuffers(
               EGLDisplay dpy,
               EGLSurface surface,
               EGLNativePixmapType target
               )
{
    EGLBoolean ret = g_opengl->eglCopyBuffers(dpy, surface, target);
    ApiCopyBuffers(dpy, surface, target, ret);
    return ret;
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
    Abort("%s(%d)\n", __FUNCTION__, __LINE__);
    return NULL;
}

EGLAPI EGLBoolean EGLAPIENTRY
eglDestroyImageKHR (
                    EGLDisplay dpy,
                    EGLImageKHR image
                    )
{
    Abort("%s(%d)\n", __FUNCTION__, __LINE__);
    return EGL_FALSE;
}

EGLAPI EGLSyncKHR EGLAPIENTRY
eglCreateSyncKHR(
                 EGLDisplay dpy,
                 EGLenum type,
                 const EGLint *attrib_list
                 )
{
    Abort("%s(%d)\n", __FUNCTION__, __LINE__);
    return NULL;
}

EGLAPI EGLBoolean EGLAPIENTRY
eglDestroySyncKHR(
                  EGLDisplay dpy,
                  EGLSyncKHR syncKHR
                  )
{
    Abort("%s(%d)\n", __FUNCTION__, __LINE__);
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
    Abort("%s(%d)\n", __FUNCTION__, __LINE__);
    return 1;
}


EGLAPI EGLBoolean EGLAPIENTRY
eglGetSyncAttribKHR(
                    EGLDisplay display,
                    EGLSyncKHR syncKHR,
                    EGLint attribute,
                    EGLint *value)
{
    Abort("%s(%d)\n", __FUNCTION__, __LINE__);
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
        /* Loop while there are entries in the lookup table. */
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
    PEglThreadData  thread  = _GetThreadData();
    void*           func    = NULL;

    if (procname[0] == '\0')
    {
        /* Handle empty string. */
        return NULL;
    }

    func = _Lookup(_veglLookup, procname, NULL);
    if (NULL != func)
        return (__eglMustCastToProperFunctionPointerType)func;

    if (thread->context)
    {
        func = (__eglMustCastToProperFunctionPointerType)GetFuncAddress(thread->context->client, procname);
    }
    else
    {
        func = (__eglMustCastToProperFunctionPointerType)GetFuncAddress(3, procname);
    }

    return (__eglMustCastToProperFunctionPointerType)func;
}

#endif /* API_DUMP */