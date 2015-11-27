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

#endif /* __EGL_API_H */