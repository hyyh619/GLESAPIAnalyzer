#ifndef _UTILS_H
#define _UTILS_H

#include <stdio.h>
#include <string.h>
#include <memory.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <map>
#include <string>

#define GL_GLEXT_PROTOTYPES
#include <GLES3/gl3.h>
#include <GLES3/gl3ext.h>

#include "types.h"

typedef std::map<GLuint, std::string>   STRING_MAP;

#define PROCESS_INPUT(name) \
    do \
    { \
        if (!strncmp(input, #name, strlen(#name)))\
        { \
            Parse##name(input, output, outputSize); \
            return GL_TRUE; \
        } \
    } \
    while (0)

#define  PROCESS_EGL_INPUT(name) \
    do \
    { \
        if (!strncmp(&input[9], #name, strlen(#name)))\
        { \
            Parse##name(input, output, outputSize); \
            return GL_TRUE; \
        } \
    } \
    while (0)

#define GLES_API_EGL       0
#define GLES_API_GLES1     2
#define GLES_API_GLES2     1
#define GLES_API_GLES3     GLES_API_GLES2
#define GLES_API_MAX       4

#define RESOURCE_PATH                   "resource"
#define RESULT_PATH                     "result"

#define LINE_BUF_SIZE                   1024
#define FILE_NAME_BUF_SIZE              1024
#define OUT_BUF_SIZE                    1024
#define UNIFORM_BUF_SIZE                1024*20
#define DRAW_BUF_SIZE                   1024*1024*4
#define TMP5_BUF_SIZE                   256*40
#define TMP_BUF_SIZE                    256
#define EGL_OUT_BUF_SIZE                512
#define GL_OUT_BUF_SIZE                 256

#define READ_GL_UINT                    1
#define READ_GL_UCHAR                   2

#ifdef WIN32
#define API_ABORT(fmt, ...) \
    do \
    { \
        __asm int 3; \
        printf(fmt, ##__VA_ARGS__); \
    } \
    while (0)
#else
#define API_ABORT(fmt, ...) \
    do \
    { \
         printf(fmt, ##__VA_ARGS__); \
    } while (0)
#endif

#define STRLEN(name) \
    (name?strlen(name):0)

#define STRCMP(str1, str2) \
    ((str1 && str2)?strcmp(str1, str2):1)

#define ALLOC(p, n) \
    p = malloc(n); if (p == NULL) return NULL;

#define FREE(p) \
    if ((p) != NULL) \
    {\
        free((p));\
        (p) = NULL;\
    }

#define MAX(a,b)    (((a) > (b)) ? (a) : (b))
#define MIN(a,b)    (((a) < (b)) ? (a) : (b))

typedef enum _enLogLevel
{
    LOG_VERBOSE = 0,
    LOG_WARNING,
    LOG_ERROR
} enLogLevel;

typedef enum _NODE_COLOR
{
    BLACK,
    RED
} NODE_COLOR;

typedef enum _NAME_CODE
{
    eglSwapBuffers1,
    eglSwapBuffers2,
    eglSwapBuffers3,
    eglSwapBuffers4,

    // dump DrawElements
    glDrawElements1,
    glDrawElements2,
    glDrawElements3,
    glDrawElements4,
    glDrawElements5,
    glDrawElements6,
    glDrawElements7,
    glDrawElements8,
    glDrawElements9,
    glDrawElements10,
    glDrawElements11,
    glDrawElements12,
    glDrawElements13,
    glDrawElements14,
    glDrawElements15,
    glDrawElements16,
    glDrawElements17,
    glDrawElements18,
    glDrawElements19,
    glDrawElements20,

    // dump DrawElements
    glDrawArrays1,
    glDrawArrays2,
    glDrawArrays3,
    glDrawArrays4,
    glDrawArrays5,
    glDrawArrays6,
    glDrawArrays7,
    glDrawArrays8,
    glDrawArrays9,
    glDrawArrays10,
    glDrawArrays11,
    glDrawArrays12,
    glDrawArrays13,
    glDrawArrays14,
    glDrawArrays15,
    glDrawArrays16,
    glDrawArrays17,
    glDrawArrays18,
    glDrawArrays19,
    glDrawArrays20,

    // dump draw states
    drawStates1,
    drawStates2,
    drawStates3,
    drawStates4,
    drawStates5,
    drawStates6,
    drawStates7,
    drawStates8,
    drawStates9,
    drawStates10,
    drawStates11,
    drawStates12,
    drawStates13,
    drawStates14,
    drawStates15,
    drawStates16,
    drawStates17,
    drawStates18,
    drawStates19,
    drawStates20,
    drawStates21,
    drawStates22,
    drawStates23,
    drawStates24,
    drawStates25,
    drawStates26,
    drawStates27,
    drawStates28,
    drawStates29,
    drawStates30,
    drawStates31,
    drawStates32,
    drawStates33,
    drawStates34,
    drawStates35,
    drawStates36,
    drawStates37,

    // dump uniform
    dumpUniform1,
    dumpUniform2,
    dumpUniform3,
    dumpUniform4,
    dumpUniform5,
    dumpUniform6,
    dumpUniform7,
    dumpUniform8,
    dumpUniform9,
    dumpUniform10,
    dumpUniform11,
    dumpUniform12,

    // dump matrix
    dumpMatrix1,
    dumpMatrix2,
    dumpMatrix3,

    // dump data
    dumpData1,

    // dump vertex data
    dumpVertex1,
    dumpVertex2,
    dumpVertex3,
    dumpVertex4,
    dumpVertex5,
    dumpVertex6,
    dumpVertex7,
    dumpVertex8,
    dumpVertex9,
    dumpVertex10,

    // gles2Disassembly
    gles2Disassemble1,
    gles2Disassemble2,
    gles2Disassemble3,
    gles2Disassemble4,
    gles2Disassemble5,
    gles2Disassemble6,
    gles2Disassemble7,
    gles2Disassemble8,
    gles2Disassemble9,
    gles2Disassemble10,
    gles2Disassemble11,
    gles2Disassemble12,
    gles2Disassemble13,
    gles2Disassemble14,
    gles2Disassemble15,
    gles2Disassemble16,
    gles2Disassemble17,
    gles2Disassemble18,
    gles2Disassemble19,
    gles2Disassemble20,
    gles2Disassemble21,
    gles2Disassemble22,
    gles2Disassemble23,
    gles2Disassemble24,
    gles2Disassemble25,
    gles2Disassemble26,
    gles2Disassemble27,
    gles2Disassemble28,
    gles2Disassemble29,
    gles2Disassemble30,

    // shader
    printShader1,
    printShader2,
    printShader3,
    printShader4,
    printShader5,
    printShader6,
    printShader7,
    printShader8,
    printShader9,

    // return code
    oneChar,
    twoChar,
    threeChar,
    fourChar,
} NAME_CODE;

typedef enum _OUTPUT_TARGET
{
    OUTPUT_TO_FILE = 0,
    OUTPUT_TO_FRAME_STRING,
    OUTPUT_TO_DRAW_STRING,
    OUTPUT_TO_TOTAL_STRING,
    OUTPUT_TO_DRAW_TOTAL_STRING,
    OUTPUT_INVALID
} OUTPUT_TARGET;

typedef struct _DictionaryNode
{
    GLchar                  *name;
    GLint                   nameLen;
    GLint                   outputSize;
    GLint                   nameCode;

    struct _DictionaryNode  *left;
    struct _DictionaryNode  *right;
    struct _DictionaryNode  *parent;

    NODE_COLOR              color;
} DictionaryNode;

GLboolean       OutputToFile(GLboolean    bEventSeq,
                             GLuint       eventSeq,
                             const GLchar *thread,
                             const GLchar *output,
                             FILE         *pFile);
GLboolean       OutputToTarget(OUTPUT_TARGET  target,
                               GLboolean      bEventSeq,
                               GLuint         nEventSeq,
                               const GLchar   *pThread,
                               const GLchar   *pOutput,
                               GLvoid         *pTarget);
GLvoid          OutputToShaderFile(GLchar *output);
GLvoid          OutputStrcat(GLchar* outBuf, GLint outputSize, const GLchar* string, ...);
GLchar*         readLineFast();
GLchar*         readUChar(GLint count);
GLchar*         readUCharFast(GLint count);
GLchar*         readLine();
GLuint*         readUInt(GLint count);
GLuint*         readUIntCount(GLint * count);
GLuint*         readUIntCountFast(GLint * count);
DictionaryNode* DictCreateNode(GLint nameCode, GLint outputSize, GLchar *name);
DictionaryNode* DictCreateNode(GLint nameCode, GLint outputSize);
DictionaryNode* DictFindSpecificNode(DictionaryNode *node, GLint code);
GLvoid          InsertNode(DictionaryNode **pRoot, DictionaryNode **pParent, DictionaryNode *node);
GLvoid          OutputStringFast(GLint nameCode, GLint *pos, GLchar *output, GLint outputSize, const GLchar *fmt, ...);
GLvoid          Float2String(GLfloat f, GLchar *buf);
GLchar*         GenJsonArray(GLuint *v, GLint count, GLchar *key);
GLint           WritePNGFile(GLchar *file_name, GLint width, GLint height, GLint bit_depth, GLchar *pixel, GLboolean hasAlpha);
GLvoid*         ReadData(GLuint type, GLuint handle, GLuint count);
GLvoid*         ReadTexData(GLuint type, GLuint handle, GLuint count);
GLvoid*         ReadCompressedTex(GLuint type, GLuint handle, GLuint count);
GLvoid          FreeData(GLvoid *ptr);
GLuint          ReadUInt(GLuint handle);
GLboolean       IsFileExist(GLchar *fileName);
GLvoid          EnableOutputToFile(GLboolean bEnable);
GLvoid          ClearStringMap(STRING_MAP *p);
GLvoid          ClearAllStringMap();
GLvoid          ClearDrawStringMap();
GLvoid          OutputDebugInfoToFile(const GLchar *str, ...);
GLvoid          Abort(const GLchar *msg, ...);
GLvoid          Print(enLogLevel level, const char *msg, ...);
GLboolean       OpenDumpFile();
GLvoid          SetDumpFile(GLint location);
GLvoid          CloseDumpFile();
const GLchar*   GetAPIName(GLESAPIIndex name);
GLvoid          APIInitNameMap();
GLint           GetAPIIndex(GLESAPIIndex name);

extern GLuint           nLineNum;
extern GLuint           g_nFrames;
extern GLuint           nStartFrame;
extern GLuint           nEndFrame;
extern GLboolean        bPrintEGL;
extern GLboolean        bPrintES20;
extern GLboolean        bPrintES11;
extern GLboolean        bPrintDrawShader;
extern GLboolean        bPrintVertexAttrib;
extern GLboolean        g_bSaveTexture;
extern GLboolean        g_bSaveShader;
extern GLboolean        bDumpFrameFile;
extern GLboolean        bPrintMatrix;
extern GLboolean        bDatabase;
extern GLboolean        bForceFinish;
extern GLboolean        bPrintUniform;
extern GLboolean        bThread;
extern GLboolean        bPrintLine;
extern GLboolean        bPrintContent;
extern GLboolean        bExitApp;
extern GLboolean        bUsePng;
extern GLboolean        bFlushFile;
extern GLchar           tmp6[TMP5_BUF_SIZE];
extern FILE             *g_pDrawFile;
extern FILE             *g_pOutputFile;
extern FILE             *g_pFrameFile;
extern FILE             *g_pApiDumpFile;
extern FILE             *g_pShaderFile;
extern FILE             *g_pTextureFile;
extern GLchar           strFileName[FILE_NAME_BUF_SIZE];
extern GLchar           strOnlyFileName[FILE_NAME_BUF_SIZE];
extern GLchar           strOnlySourcePath[FILE_NAME_BUF_SIZE];
extern GLchar           Line[LINE_BUF_SIZE];
extern GLchar           Draw[DRAW_BUF_SIZE];
extern GLchar           Output[OUT_BUF_SIZE];
extern GLchar           Uniform[UNIFORM_BUF_SIZE];
extern GLint            eglWidth;
extern GLint            eglHeight;
extern GLenum           eglFormat;
extern enLogLevel       printLevel;
extern GLchar           thread[64];
extern const GLchar     *strHalDumpFileName;
extern GLchar           strOnlyFrameFileName[FILE_NAME_BUF_SIZE];
extern GLchar           EGLOutput[EGL_OUT_BUF_SIZE];
extern GLchar           GLOutput[GL_OUT_BUF_SIZE];
extern DictionaryNode   *dictRoot;
extern GLfloat          totalSccanf;
extern STRING_MAP       g_frameStringMap;
extern STRING_MAP       g_drawStringMap;
extern STRING_MAP       g_totalStringMap;
extern GLuint           g_frameDrawCount;
extern GLuint           g_drawStringCount;
extern GLuint           g_totalStringCount;


#endif // _UTILS_H
