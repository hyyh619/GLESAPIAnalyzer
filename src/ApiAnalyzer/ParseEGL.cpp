#include "MathAPI.h"
#include "ParseEGL.h"
#include "utils.h"
#include "bitmap.h"
#include "MemoryPool.h"
#include "TranslateGL.h"
#include "ParseGLES3.h"
#include "GLESShare.h"
#include "egl.h"
#include "Analyzer.h"


GLvoid ParseBindAPI(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint   arg[2];
    GLchar  *str;

    if (sscanf(input, "${EGL eglBindAPI 0x%08X}", &arg[0]) == 1)
    {
        ANALYZER_FUNC2(BindAPI, arg[0], arg[1]);
    }
}

GLvoid ParseBindTexImage(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint   arg[4];

    if (sscanf(input,
               "${EGL eglBindTexImage 0x%08X 0x%08X 0x%08X}",
               &arg[0], &arg[1], &arg[2]) == 3)
    {
        ANALYZER_FUNC4(BindTexImage, (EGLDisplay)arg[0], (EGLSurface)arg[1], arg[2], arg[3]);
    }
}

GLvoid ParseCreateContext(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint   arg[5];

    if (sscanf(input,
               "${EGL eglCreateContext 0x%08X 0x%08X 0x%08X (0x%08X) := 0x%08X",
               &arg[0], &arg[1], &arg[2], &arg[3], &arg[4]) == 5)
    {
        stEglContext    *p = NULL;
        static EGLint   eglContextCount = 0;
        EGLint          *pAttr  = (EGLint*)ReadData(READ_GL_UINT, arg[3], 0);


    }
}

GLvoid ParseChooseConfig(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint   arg[5];

    if (sscanf(input,
               "${EGL eglChooseConfig 0x%08X (0x%08X) (0x%08X) 0x%08X (0x%08X)",
               &arg[0], &arg[1], &arg[2], &arg[3], &arg[4]) == 5)
    {
        EGLint      *attribList = (EGLint*)ReadData(READ_GL_UCHAR, arg[1], 0);
        EGLint      *configAddr = (EGLint*)ReadData(READ_GL_UCHAR, arg[3], 0);
        EGLint      *configNum  = (EGLint*)ReadData(READ_GL_UCHAR, arg[4], 0);
        EGLint      *value      = attribList;
        EGL_CONFIG  *p          = NULL;
        EGLBoolean  bFinish     = EGL_FALSE;
        EGLint      num         = configNum ? *configNum : 0;

        if (configAddr == NULL)
        {
            return;
        }

        for (GLint i=0; i<num; i++)
        {
            if (configAddr)
            {
                p = eglGetEGLConfig(egl_context.config, egl_context.configCount, configAddr[i]);
                if (p == NULL)
                {
                    p = eglGetFreeEGLConfig(egl_context.config, &egl_context.configCount);
                }
            }

            if (p)
            {
                p->bUsed = EGL_TRUE;

                if (configAddr)
                {
                    p->addr = configAddr[i];
                }

                do
                {
                    switch (*value++)
                    {
                    case EGL_RENDERABLE_TYPE:   p->eglRenderableType    = *value++;     break;
                    case EGL_RED_SIZE       :   p->redSize              = *value++;     break;
                    case EGL_GREEN_SIZE     :   p->greenSize            = *value++;     break;
                    case EGL_BLUE_SIZE      :   p->blueSize             = *value++;     break;
                    case EGL_ALPHA_SIZE     :   p->alphaSize            = *value++;     break;
                    case EGL_DEPTH_SIZE     :   p->depthSize            = *value++;     break;
                    case EGL_STENCIL_SIZE   :   p->stencilSize          = *value++;     break;
                    case EGL_SURFACE_TYPE   :   p->surfaceType          = *value++;     break;
                    case EGL_SAMPLES        :   p->samples              = *value++;     break;
                    case EGL_NONE:
                        bFinish = EGL_TRUE;
                        break;
                    default:
                        break;
                    }
                }
                while (!bFinish);
            }

            bFinish = EGL_FALSE;
        }

        sprintf(output, "eglChooseConfig(display = 0x%08X, attrib_list = 0x%08X, configs = 0x%08X, config_size = 0x%08X, num_config = 0x%08X)\n",
                arg[0], arg[1], arg[2], arg[3], arg[4]);

        if (attribList)
        {
            FreeData(attribList);
        }

        if (configAddr)
        {
            FreeData(configAddr);
        }

        if (configNum)
        {
            FreeData(configNum);
        }
    }
}

GLvoid ParseCreateImageKHR(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint   arg[6];

    if (sscanf(input,
               "${EGL eglCreateImageKHR 0x%08X 0x%08X 0x%08X 0x%08X (0x%08X) := 0x%08X",
               &arg[0], &arg[1], &arg[2], &arg[3], &arg[4], &arg[5]) == 6)
    {
        EGLenum         target  = arg[2];
        GLuint          texture = arg[3];
        const GLchar    *image  = NULL;

        switch (target)
        {
        case EGL_GL_TEXTURE_2D_KHR:
            image = "EGL_GL_TEXTURE_2D_KHR";
            break;

        case EGL_GL_TEXTURE_CUBE_MAP_POSITIVE_X_KHR:
        case EGL_GL_TEXTURE_CUBE_MAP_NEGATIVE_X_KHR:
        case EGL_GL_TEXTURE_CUBE_MAP_POSITIVE_Y_KHR:
        case EGL_GL_TEXTURE_CUBE_MAP_NEGATIVE_Y_KHR:
        case EGL_GL_TEXTURE_CUBE_MAP_POSITIVE_Z_KHR:
        case EGL_GL_TEXTURE_CUBE_MAP_NEGATIVE_Z_KHR:
            image = "EGL_GL_TEXTURE_CUBE_MAP";
            break;

        case EGL_NATIVE_PIXMAP_KHR:
            image = "EGL_NATIVE_PIXMAP_KHR";
            break;

        case EGL_GL_RENDERBUFFER_KHR:
            image = "EGL_GL_RENDERBUFFER_KHR";
            break;

        case EGL_NATIVE_BUFFER_ANDROID:
            image = "EGL_NATIVE_BUFFER_ANDROID";
            break;

        case EGL_VG_PARENT_IMAGE_KHR:
            image = "EGL_VG_PARENT_IMAGE_KHR";
            break;

        default:
            image = "invalid";
            break;
        }

        sprintf(output, "eglCreateImageKHR(%s, texture=0x%08X) = 0x%08X\n", image, texture, arg[5]);
    }
}

GLvoid ParseCreateSyncKHR(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint   arg[4];

    if (sscanf(input,
               "${EGL eglCreateSyncKHR 0x%08X 0x%08X (0x%08X) := 0x%08X",
               &arg[0], &arg[1], &arg[2], &arg[3]) == 4)
    {
        TranslateEGLEnum(arg[1], tmp1);
        sprintf(output, "eglCreateSyncKHR(display = 0x%08X, type = %s, attribList = 0x%08X) = 0x%08X\n", arg[0], tmp1, arg[2], arg[3]);
    }
}

GLvoid ParseClientWaitSyncKHR(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint   arg[5];
    U64     arg64;

    if (sscanf(input,
               "${EGL eglClientWaitSyncKHR 0x%08X 0x%08X 0x%08X 0x%16llX := 0x%08X}",
               &arg[0], &arg[1], &arg[2], &arg64, &arg[4]) == 5)
    {
        TranslateEGLEnum(arg[2], tmp1);

        memset(tmp1, 0, TMP_BUF_SIZE);
        if (arg64 == EGL_FOREVER_KHR)
        {
            sprintf(tmp2, "EGL_FOREVER_KHR");
        }
        else
        {
            sprintf(tmp2, "%ums", (GLuint)MATH_DivideUInt64(arg64, 1000000ull));
        }

        memset(tmp3, 0, TMP_BUF_SIZE);
        if (arg[4] == EGL_FALSE)
        {
            sprintf(tmp3, "EGL_FALSE");
        }
        else
        {
            TranslateEGLEnum(arg[4], tmp3);
        }

        sprintf(output, "eglClientWaitSyncKHR(display = 0x%08X, sync = 0x%08X, flags = %s, timeout = %s) = %s\n",
            arg[0], arg[1], tmp1, tmp2, tmp3);
    }
}

GLvoid ParseCopyBuffers(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint   arg[3];

    if (sscanf(input,
               "${EGL eglCopyBuffers 0x%08X 0x%08X 0x%08X}",
               &arg[0], &arg[1], &arg[2]) == 3)
    {
        PrintParams3(output, outputSize, "eglCopyBuffers",
                    (GLuint)arg[0], "display", INT_04D,
                    (GLuint)arg[1], "surface", INT_0X08X,
                    (GLuint)arg[2], "target", INT_0X08X);
    }
}

GLvoid ParseCreateWindowSurface(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint   arg[7];

    if (sscanf(input,
               "${EGL eglCreateWindowSurface 0x%08X 0x%08X 0x%08X (0x%08X) := 0x%08X (%dx%d)",
               &arg[0], &arg[1], &arg[2], &arg[3], &arg[4], &arg[5], &arg[6]) == 7)
    {
        stEglSurface    *surfObj    = NULL;
        static EGLint   windowCount = 0;
        EGL_CONFIG      *config     = eglGetEGLConfig(egl_context.config, egl_context.configCount, arg[1]);
        EGLint          *pAttr      = (EGLint*)ReadData(READ_GL_UINT, arg[3], 0);

        surfObj             = eglGetFreeSurfaceObject(egl_context.surface, &egl_context.surfaceCount);
        surfObj->width      = arg[5];
        surfObj->height     = arg[6];
        surfObj->bUsed      = EGL_TRUE;
        surfObj->addr       = arg[4];
        surfObj->type       = SURFACE_TYPE_WINDOWSURFACE;
        surfObj->config     = config;
        memset(surfObj->name, 0, 32);
        sprintf(surfObj->name, "WindowSurface%d", ++windowCount);

        sprintf(output, "eglCreateWindowSurface(display = 0x%08X, config = 0x%08X, window = 0x%08X, attrib_list = 0x%08X) %dx%d, %s\n",
                arg[0], arg[1], arg[2], arg[3], surfObj->width, surfObj->height, surfObj->name);

        if (pAttr)
        {
            while (*pAttr != EGL_NONE)
            {
                EGLint name = pAttr[0];
                EGLint value = pAttr[1];

                TranslateEGLAttrib(name, tmp3);
                if (value == 0x3084)
                {
                    TranslateEGLAttrib(value, tmp4);
                    OutputStrcat(output, outputSize, "    %s: %s\n", tmp3, tmp4);
                }
                else
                {
                    OutputStrcat(output, outputSize, "    %s: %d\n", tmp3, value);
                }

                pAttr += 2;
            }
        }
    }
}

GLvoid ParseCreatePbufferSurface(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint   arg[7];

    if (sscanf(input,
               "${EGL eglCreatePbufferSurface 0x%08X 0x%08X (0x%08X) := 0x%08X (%dx%d)",
               &arg[0], &arg[1], &arg[2], &arg[3], &arg[4], &arg[5]) == 6)
    {
        stEglSurface    *surface        = eglGetFreeSurfaceObject(egl_context.surface, &egl_context.surfaceCount);
        static EGLint   pbufferCount    = 0;
        EGL_CONFIG      *config         = eglGetEGLConfig(egl_context.config, egl_context.configCount, arg[1]);

        surface->width  = arg[4];
        surface->height = arg[5];
        surface->bUsed  = EGL_TRUE;
        surface->addr   = arg[3];
        surface->type   = SURFACE_TYPE_PBUFFER;
        surface->config = config;
        memset(surface->name, 0, 32);
        sprintf(surface->name, "Pbuffer%d", ++pbufferCount);

        if (config)
        {
            sprintf(output, "eglCreatePbufferSurface(display = 0x%08X, config = 0x%08X, attrib_list = 0x%08X) %s (%dx%d) rgba(%d%d%d%d) depth(%d) stencil(%d)\n",
                    arg[0], arg[1], arg[2], surface->name, arg[4], arg[5],
                    config->redSize, config->greenSize, config->blueSize, config->alphaSize,
                    config->depthSize, config->stencilSize);
        }
        else
        {
            sprintf(output, "eglCreatePbufferSurface(display = 0x%08X, config = 0x%08X, attrib_list = 0x%08X) %s (%dx%d)\n",
                    arg[0], arg[1], arg[2], surface->name, arg[4], arg[5]);
        }
    }
}

GLvoid ParseCreatePbufferFromClientBuffer(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint   arg[8];

    if (sscanf(input,
               "${EGL eglCreatePbufferFromClientBuffer 0x%08X 0x%08X 0x%08X 0x%08X (0x%08X) := 0x%08X (%dx%d)",
               &arg[0], &arg[1], &arg[2], &arg[3], &arg[4], &arg[5], &arg[6], &arg[7]) == 8)
    {
        sprintf(output, "eglCreatePbufferFromClientBuffer() ****todo\n");
    }
}

GLvoid ParseCreatePixmapSurface(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint   arg[7];

    if (sscanf(input,
               "${EGL eglCreatePixmapSurface 0x%08X 0x%08X 0x%08X (0x%08X) := 0x%08X (%dx%d)",
               &arg[0], &arg[1], &arg[2], &arg[3], &arg[4], &arg[5], &arg[6]) == 7)
    {
        sprintf(output, "eglCreatePixmapSurface() ****todo\n");
    }
}

GLvoid ParseDestroySurface(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint   arg[2];

    if (sscanf(input,
               "${EGL eglDestroySurface 0x%08X 0x%08X}",
               &arg[0], &arg[1]) == 2)
    {
        PrintParams2(output, outputSize, "eglDestroySurface",
                    (GLuint)arg[0], "display", INT_04D,
                    (GLuint)arg[1], "surface", INT_0X08X);
    }
}

GLvoid ParseDestroyContext(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint   arg[2];

    if (sscanf(input,
               "${EGL eglDestroyContext 0x%08X 0x%08X}",
               &arg[0], &arg[1]) == 2)
    {
        PrintParams2(output, outputSize, "eglDestroyContext",
                    (GLuint)arg[0], "display", INT_04D,
                    (GLuint)arg[1], "context", INT_0X08X);
    }
}

GLvoid ParseDestroyImageKHR(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint   arg[2];

    if (sscanf(input,
               "${EGL eglDestroyImageKHR 0x%08X 0x%08X}",
               &arg[0], &arg[1]) == 2)
    {
        PrintParams2(output, outputSize, "eglDestroyImageKHR",
                    (GLuint)arg[0], "display", INT_04D,
                    (GLuint)arg[1], "image", INT_0X08X);
    }
}

GLvoid ParseDestroySyncKHR(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint   arg[2];

    if (sscanf(input,
               "${EGL eglDestroySyncKHR 0x%08X 0x%08X}",
               &arg[0], &arg[1]) == 2)
    {
        PrintParams2(output, outputSize, "eglDestroySyncKHR",
                    (GLuint)arg[0], "display", INT_04D,
                    (GLuint)arg[1], "sync", INT_0X08X);
    }
}

GLvoid ParseGetConfigs(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint   arg[4];

    if (sscanf(input,
               "${EGL eglGetConfigs 0x%08X (0x%08X) 0x%08X (0x%08X)",
               &arg[0], &arg[1], &arg[2], &arg[3]) == 4)
    {
        sprintf(output, "eglGetConfigs(display = 0x%08X, configs = 0x%08X, config_size = 0x%08X, num_config = 0x%08X)\n",
                arg[0], arg[1], arg[2], arg[3]);
    }
}

GLvoid ParseGetConfigAttrib(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint   arg[4];

    if (sscanf(input,
               "${EGL eglGetConfigAttrib 0x%08X 0x%08X 0x%08X := 0x%08X}",
               &arg[0], &arg[1], &arg[2], &arg[3]) == 4)
    {
        TranslateEGLAttrib(arg[2], tmp5);

        sprintf(output, "eglGetConfigAttrib(display = 0x%08X, config = 0x%08X, attrib = %s, value = 0x%08X)\n",
                arg[0], arg[1], tmp5, arg[3]);
    }
}

GLvoid ParseEGLGetError(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint   arg[1];

    if (sscanf(input,
               "${EGL eglGetError := 0x%08X}",
               &arg[0]) == 1)
    {
        if (arg[0] != EGL_SUCCESS)
        {
            TranslateEglError(arg[0], tmp1);
            sprintf(output, "eglGetError() = %s\n", tmp1);
        }
    }
}

GLvoid ParseGetDisplay(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint   arg[2];

    if (sscanf(input,
               "${EGL eglGetDisplay 0x%08X := 0x%08X}",
               &arg[0], &arg[1]) == 2)
    {
        sprintf(output, "eglGetDisplay(display_id = 0x%08X) display = 0x%08X\n", arg[0], arg[1]);
    }
}

GLvoid ParseGetCurrentContext(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint   arg[1];

    if (sscanf(input,
               "${EGL eglGetCurrentContext := 0x%08X}",
               &arg[0]) == 1)
    {
        sprintf(output, "eglGetCurrentContext() = 0x%08X\n", arg[0]);
    }
}

GLvoid ParseGetCurrentSurface(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint   arg[2];
    GLchar  *ptr;

    if (sscanf(input,
               "${EGL eglGetCurrentSurface 0x%08X := 0x%08X}",
               &arg[0], &arg[1]) == 2)
    {
        switch (arg[0])
        {
        case EGL_READ:
            ptr = "EGL_READ";
            break;

        case EGL_DRAW:
            ptr = "EGL_DRAW";
            break;

        default:
            ptr = "Invalid";
            break;
        }

        sprintf(output, "eglGetCurrentSurface(readdraw = %s) = 0x%08X\n", ptr, arg[1]);
    }
}

GLvoid ParseGetCurrentDisplay(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint   arg[1];

    if (sscanf(input,
               "${EGL eglGetCurrentDisplay := 0x%08X}",
               &arg[0]) == 1)
    {
        sprintf(output, "eglGetCurrentDisplay() = 0x%08X\n", arg[0]);
    }
}

GLvoid ParseGetSyncAttribKHR(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint   arg[4];

    if (sscanf(input,
               "${EGL eglGetSyncAttribKHR 0x%08X 0x%08X 0x%08X := 0x%08X}",
               &arg[0], &arg[1], &arg[2], &arg[3]) == 4)
    {
        sprintf(output, "eglGetSyncAttribKHR() ****todo\n");
    }
}

GLvoid ParseGetProcAddress(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint   arg[2];

    if (sscanf(input,
               "${EGL eglGetProcAddress (0x%08X) := 0x%08X",
               &arg[0], &arg[1]) == 2)
    {
        GLchar  *v = readUCharFast(0);
        sprintf(output, "eglGetProcAddress(name=%s) = 0x%08X\n", v, arg[1]);
        FREE(v);
    }
}

GLvoid ParseInitialize(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint   arg[1];

    if (sscanf(input,
               "${EGL eglInitialize 0x%08X}",
               &arg[0]) == 1)
    {
        PrintParams1(output, outputSize, "eglInitialize", (GLuint)arg[0], "display", INT_04D);
    }
}

GLvoid ParseLockSurfaceKHR(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint   arg[3];

    if (sscanf(input,
               "${EGL eglLockSurfaceKHR 0x%08X 0x%08X (0x%08X)",
               &arg[0], &arg[1], &arg[2]) == 3)
    {
        sprintf(output, "eglLockSurfaceKHR() ****todo\n");
    }
}

GLvoid ParseMakeCurrent(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint   arg[4];

    if (sscanf(input,
               "${EGL eglMakeCurrent 0x%08X 0x%08X 0x%08X 0x%08X}",
               &arg[0], &arg[1], &arg[2], &arg[3]) == 4)
    {
        stEglContext    *p      = NULL;
        stEglSurface    *draw   = NULL;
        stEglSurface    *read   = NULL;

        p       = eglGetEGLContext(egl_context.eglContext, egl_context.eglContextCount, arg[3]);
        draw    = eglGetSurfaceObject(egl_context.surface, egl_context.surfaceCount, arg[1]);
        read    = eglGetSurfaceObject(egl_context.surface, egl_context.surfaceCount, arg[2]);
        if (p)
        {
            p->draw = draw;
            p->read = read;
        }

        sprintf(output, "eglMakeCurrent(display = 0x%08X, Draw = 0x%08X, Read = 0x%08X, Context = 0x%08X) %s draw(%s) read(%s)\n",
                arg[0], arg[1], arg[2], arg[3],
                p?p->name:"null",
                draw?draw->name:"null",
                read?read->name:"null");
    }
}

GLvoid ParseQueryAPI(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint   arg[1];

    if (sscanf(input,
               "${EGL eglQueryAPI := 0x%08X}",
               &arg[0]) == 1)
    {
        sprintf(output, "eglQueryAPI() ****todo\n");
    }
}

GLvoid ParseQueryString(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint   arg[3];
    GLchar  *ptr;

    if (sscanf(input,
               "${EGL eglQueryString 0x%08X 0x%08X := 0x%08X",
               &arg[0], &arg[1], &arg[2]) == 3)
    {
        switch (arg[1])
        {
        case EGL_CLIENT_APIS:
            ptr = "EGL_CLIENT_APIS";
            break;

        case EGL_EXTENSIONS:
            ptr = "EGL_EXTENSIONS";
            break;

        case EGL_VENDOR:
            ptr = "EGL_VENDOR";
            break;

        case EGL_VERSION:
            ptr = "EGL_VERSION";
            break;

        default:
            /* Bad parameter. */
            ptr = "Invalid";
            break;
        }

        GLchar *p  = (GLchar*)ReadData(READ_GL_UCHAR, arg[2], 0);

        sprintf(output, "eglQueryString(display = 0x%X, name = %s)\n", arg[0], ptr);
        OutputStrcat(output, outputSize, "    %s\n", p);
    }
}

GLvoid ParseQueryContext(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint   arg[4];
    GLchar  *ptr;

    if (sscanf(input,
               "${EGL eglQueryContext 0x%08X 0x%08X 0x%08X := 0x%08X}",
               &arg[0], &arg[1], &arg[2], &arg[3]) == 4)
    {
        switch (arg[2])
        {
        case EGL_CONFIG_ID:
            ptr = "EGL_CONFIG_ID";
            break;

        case EGL_CONTEXT_CLIENT_TYPE:
            ptr = "EGL_CONTEXT_CLIENT_TYPE";
            break;

        case EGL_CONTEXT_CLIENT_VERSION:
            ptr = "EGL_CONTEXT_CLIENT_VERSION";
            break;

        case EGL_RENDER_BUFFER:
            ptr = "EGL_RENDER_BUFFER";
            break;

        default:
            ptr = "Invalid";
            break;
        }

        sprintf(output, "eglQueryContex(display = 0x%08X, context = 0x%08X, attrib = %s, value = 0x%08X)\n",
                arg[0], arg[1], ptr, arg[3]);
    }
}

GLvoid ParseQuerySurface(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint   arg[4];
    GLchar  *ptr;

    if (sscanf(input,
               "${EGL eglQuerySurface 0x%08X 0x%08X 0x%08X := 0x%08X}",
               &arg[0], &arg[1], &arg[2], &arg[3]) == 4)
    {
        switch (arg[2])
        {
        case EGL_CONFIG_ID:
            ptr = "EGL_CONFIG_ID";
            break;

        case EGL_WIDTH:
            ptr = "EGL_WIDTH";
            break;

        case EGL_HEIGHT:
            ptr = "EGL_HEIGHT";
            break;

        case EGL_MIPMAP_TEXTURE:
            ptr = "EGL_MIPMAP_TEXTURE";
            break;

        case EGL_MIPMAP_LEVEL:
            ptr = "EGL_MIPMAP_LEVEL";
            break;

        case EGL_TEXTURE_FORMAT:
            ptr = "EGL_TEXTURE_FORMAT";
            break;

        case EGL_TEXTURE_TARGET:
            ptr = "EGL_TEXTURE_TARGET";
            break;

        case EGL_LARGEST_PBUFFER:
            ptr = "EGL_LARGEST_PBUFFER";
            break;

        case EGL_HORIZONTAL_RESOLUTION:
        case EGL_VERTICAL_RESOLUTION:
        case EGL_PIXEL_ASPECT_RATIO:
            ptr = "EGL_HORIZONTAL_RESOLUTION";
            break;

        case EGL_RENDER_BUFFER:
            ptr = "EGL_RENDER_BUFFER";
            break;

        case EGL_SWAP_BEHAVIOR:
            ptr = "EGL_SWAP_BEHAVIOR";
            break;

        case EGL_VG_ALPHA_FORMAT:
        case EGL_VG_COLORSPACE:
            /* Not yet implemented. */
            ptr = "Not Implemented";
            break;

        case EGL_BITMAP_POINTER_KHR:
            ptr = "EGL_BITMAP_POINTER_KHR";
            break;

        case EGL_BITMAP_PITCH_KHR:
            ptr = "EGL_BITMAP_PITCH_KHR";
            break;

        case EGL_BITMAP_ORIGIN_KHR:
            ptr = "EGL_BITMAP_ORIGIN_KHR";
            break;

        case EGL_BITMAP_PIXEL_RED_OFFSET_KHR:
        case EGL_BITMAP_PIXEL_GREEN_OFFSET_KHR:
        case EGL_BITMAP_PIXEL_BLUE_OFFSET_KHR:
        case EGL_BITMAP_PIXEL_ALPHA_OFFSET_KHR:
        case EGL_BITMAP_PIXEL_LUMINANCE_OFFSET_KHR:
            ptr = "EGL_BITMAP_PIXEL_RED_OFFSET_KHR";
            break;

        default:
            /* Bad attribute. */
            ptr = "Invalid";
            break;
        }

        sprintf(output, "eglQuerySurface(display = 0x%08X, surface = 0x%08X, attrib = %s, value = 0x%08X)\n",
                arg[0], arg[1], ptr, arg[3]);
    }
}

GLvoid ParseReleaseThread(GLchar *input, GLchar *output, GLint outputSize)
{
    if (strncmp(input,
                "${EGL eglReleaseThread}",
                23) == 0)
    {
        PrintParams0(output, outputSize, "eglReleaseThread");
    }
}

GLvoid ParseReleaseTexImage(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint   arg[3];

    if (sscanf(input,
               "${EGL eglReleaseTexImage 0x%08X 0x%08X 0x%08X}",
               &arg[0], &arg[1], &arg[2]) == 3)
    {
        PrintParams3(output, outputSize, "eglReleaseTexImage",
                    (GLuint)arg[0], "display", INT_04D,
                    (GLuint)arg[1], "surface", INT_0X08X,
                    (GLuint)arg[2], "buffer", INT_0X08X);
    }
}

GLvoid ParseSwapBuffers(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint   arg[2];
    GLint   pos = 0;

    if (sscanf(input,
               "${EGL eglSwapBuffers 0x%08X 0x%08X}",
               &arg[0], &arg[1]) == 2)
    {
        OutputStringFast(eglSwapBuffers1, &pos, output, outputSize, "\n************************************************eglSwapBuffers************************************************\n");
        OutputStringFast(eglSwapBuffers2, &pos, output, outputSize, "****%s eglSwapBuffers(display=0x%08X, draw=0x%08X)\n", thread, arg[0], arg[1]);
        OutputStringFast(eglSwapBuffers3, &pos, output, outputSize, "**** Frames: %04d\n", g_nFrames++);
        OutputStringFast(eglSwapBuffers4, &pos, output, outputSize, "************************************************eglSwapBuffers************************************************\n");
    }
}

GLvoid ParseSwapBuffersRegionEXT(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint   arg[4];

    if (sscanf(input,
               "${EGL eglSwapBuffersRegionEXT 0x%08X 0x%08X 0x%08X (0x%08X)",
               &arg[0], &arg[1], &arg[2], &arg[3]) == 4)
    {
        sprintf(output, "eglSwapBuffersRegionEXT() ****todo\n");
    }
}

GLvoid ParseSwapRectangleANDROID(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint   arg[6];

    if (sscanf(input,
               "${EGL eglSwapRectangleANDROID 0x%08X 0x%08X 0x%08X 0x%08X 0x%08X 0x%08X}",
               &arg[0], &arg[1], &arg[2], &arg[3], &arg[4], &arg[5]) == 6)
    {
        sprintf(output, "eglSwapRectangleANDROID() ****todo\n");
    }
}

GLvoid ParseSwapInterval(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint   arg[2];

    if (sscanf(input,
               "${EGL eglSwapInterval 0x%08X 0x%08X}",
               &arg[0], &arg[1]) == 2)
    {
        PrintParams2(output, outputSize, "eglSwapInterval",
                    (GLuint)arg[0], "display", INT_04D,
                    (GLuint)arg[1], "interval", INT_0X08X);
    }
}

GLvoid ParseSurfaceAttrib(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint   arg[4];
    GLchar  *ptr;

    if (sscanf(input,
               "${EGL eglSurfaceAttrib 0x%08X 0x%08X 0x%08X 0x%08X}",
               &arg[0], &arg[1], &arg[2], &arg[3]) == 4)
    {
        switch (arg[2])
        {
        case EGL_MIPMAP_LEVEL:
            ptr = "EGL_MIPMAP_LEVEL";
            break;

        case EGL_SWAP_BEHAVIOR:
            ptr = "EGL_SWAP_BEHAVIOR";
            break;

        default:
            /* Invalid attribute. */
            ptr = "Invalid";
            break;
        }
        PrintParams4(output, outputSize, "eglSurfaceAttrib",
                    (GLuint)arg[0], "display", INT_04D,
                    (GLuint)arg[1], "interval", INT_0X08X,
                    (GLuint)ptr, "", STR_STR,
                    (GLuint)arg[3], "value", INT_0X08X);
    }
}

GLvoid ParseTerminate(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint   arg[1];

    if (sscanf(input,
               "${EGL eglTerminate 0x%08X}",
               &arg[0]) == 1)
    {
        PrintParams1(output, outputSize, "eglTerminate", (GLuint)arg[0], "display", INT_04D);
    }
}

GLvoid ParseUnlockSurfaceKHR(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint   arg[2];

    if (sscanf(input,
               "${EGL eglUnlockSurfaceKHR 0x%08X 0x%08X}",
               &arg[0], &arg[1]) == 2)
    {
        sprintf(output, "eglUnlockSurfaceKHR() ****todo\n");
    }
}

GLvoid ParseWaitNative(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint   arg[1];

    if (sscanf(input,
               "${EGL eglWaitNative 0x%08X}",
               &arg[0]) == 1)
    {
        sprintf(output, "eglWaitNative()\n");
    }
}

GLvoid ParseWaitClient(GLchar *input, GLchar *output, GLint outputSize)
{
    if (strncmp(input,
                "${EGL eglWaitClient}",
                20) == 0)
    {
        sprintf(output, "eglWaitClient()\n");
    }
}