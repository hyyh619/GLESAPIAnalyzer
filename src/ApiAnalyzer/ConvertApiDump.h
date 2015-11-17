#ifndef _CONVERT_API_DUMP_H
#define _CONVERT_API_DUMP_H

#include <stdio.h>
#include <string.h>
#include <memory.h>
#include <stdlib.h>
#include <stdarg.h>
#ifdef _WIN32
#include <windows.h>
#endif

#include <GLES2/gl2.h>

GLchar*     readUChar(GLint count);
GLchar*     readLine();
GLuint*     readUInt(GLint count);
GLuint*     readUIntCount(GLint * count);
GLvoid      FlushToOutputFile(GLchar *output, FILE *file);
GLboolean   GLES3Disassemble(GLchar *input, GLchar *output, GLint outputSize);
GLboolean   eglDisassemble(GLchar *input, GLchar *output, GLint outputSize);
GLvoid      GLES3Init();
GLvoid      GLES3Release();
GLvoid      ParseApiDumpFile(FILE *pFile);
GLboolean   ParseArguments1(GLint argc, const GLchar **argv);
GLvoid      EGLInit();
GLvoid      EGLRelease();

extern const GLchar  *parameter[16];

#endif /* _CONVERT_API_DUMP_H */