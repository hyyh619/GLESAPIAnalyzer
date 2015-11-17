#include "ApiEGLContext.h"

CEGLContext     *g_eglCtxStack  = NULL;
stEglSurface    *g_eglSurfStack = NULL;

CEGLContext::CEGLContext()
{
    draw    = NULL;
    read    = NULL;
    addr    = 0;
    context = NULL;
    next    = NULL;

    memset(name, 0, 32);
}

CEGLContext::~CEGLContext()
{
}