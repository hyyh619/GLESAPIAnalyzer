#ifndef _GLES2_H
#define _GLES2_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <memory.h>
#include <vector>

#define GL_GLEXT_PROTOTYPES
#include <GLES3/gl3ext.h>
#include <GLES3/gl31.h>
#include <GLES2/gl2ext.h>
#include <GLES2/gl2.h>
#include <EGL/egl.h>

#include "GLESShare.h"
#include "GLSLShader.h"
#include "ApiGLES3Context.h"

extern EGLDisplay       eglDisplay;
extern EGLContext       eglContext;
extern EGLSurface       eglSurface;
extern GLint            eglWidth;
extern GLint            eglHeight;
extern GLenum           eglFormat;

GLboolean   GLES3Disassemble(GLchar *input, GLchar *output, GLint outputBufSize);
GLvoid      GLES3Init();
GLvoid      GLES3Release();
GLvoid      SetTexParameter(CTexObj *Texture,
                            GLenum Target,
                            GLenum Parameter,
                            GLint  Value);
GLboolean   IsNeedDraw(GLuint indexCount, GLenum mode, GLuint count);

#endif /* _GLES2_H */
