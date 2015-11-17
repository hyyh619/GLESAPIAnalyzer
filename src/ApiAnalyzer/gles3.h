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
GLvoid      OutputDrawElements_es20(GLenum mode, GLint indexCount, GLenum indexType, GLuint indexAddr, char *output, GLint outputSize);
GLvoid      OutputDrawArrays_es20(GLenum mode, GLuint first, GLint count, char *output, GLint outputSize);
GLvoid      OutputDrawArraysToFrameFile_es20(GLenum mode, GLuint first, GLint count, GLuint historyID);
GLvoid      OutputClearToFrameFile(GLint mask);
GLvoid      OutputMaskToFrameFile(char *str);
GLvoid      OutputFBO(GLuint currentFBO, GLchar *output, GLint outputSize);
GLvoid      OutputCurrentFBO(GLint *pos, GLchar *output, GLint outputSize);
GLvoid      OutputDrawElementsToFrameFile_es20(GLenum mode, GLint count, GLenum indexType, GLuint historyID);
GLvoid      DumpVertexAttr(GLint *pos, GLint vertexMin, GLint vertexMax, GLint first, char *output, GLint outputSize);
GLboolean   IsNeedDraw(GLuint indexCount, GLenum mode, GLuint count);

#endif /* _GLES2_H */
