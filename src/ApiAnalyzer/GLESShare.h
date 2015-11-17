#ifndef _GLES_CONVERT_H
#define _GLES_CONVERT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <map>
#include <vector>

#include "types.h"
#include "GLESGlobal.h"

#define TMP_BUF_SIZE            256
#define TMP5_BUF_SIZE           256*40
#define RENDER_BUFFER_MAP_NUM   1024
#define FRAME_BUFFER_MAP_NUM    1024
#define PROGRAM_MAP_NUM         1024
#define SHADER_MAP_NUM          2048
#define ATTRIB_MAP_NUM          64
#define UNIFORM_MAP_NUM         1024

#define TEXTURE_TYPE_2D         0
#define TEXTURE_TYPE_CUBEMAP    1
#define TEXTURE_TYPE_EXTERNAL   2
#define TEXTURE_TYPE_3D         3

#define ES11_VERSION            1
#define ES20_VERSION            2

#ifndef GLboolean
typedef unsigned char    GLboolean;
#endif

typedef GLuint  TEXTURE_MAPPING;
typedef GLuint  FRAME_BUFFER_MAPPING;
typedef GLuint  RENDER_BUFFER_MAPPING;
typedef GLuint  BUFFER_MAPPINT;
typedef GLuint  SHADER_MAPPING;
typedef GLuint  PROGRAM_MAPPING;

typedef enum _PARAM_FORMAT
{
    INT_04D,
    INT_04D_NO,   /* no description string */
    INT_08D,
    INT_0X04X,
    INT_0X08X,
    STR_STR,
    STR_STR_NAME,
    FLOAT_1_1,
    FLOAT_1_5,
    FLOAT_5_1,
} PARAM_FORMAT;


GLvoid GetPrimitiveCount(GLenum Mode, GLsizei Count, GLuint *PrimitiveCount);
GLvoid OutputBlend(int *pos, int esVersion, char *output, int outputSize);
GLvoid OutputCullFace(int *pos, int esVersion, char *output, int outputSize);
GLvoid OutputDepth(int *pos, int esVersion, char *output, int outputSize);
GLvoid OutputDither(int *pos, int esVersion, char *output, int outputSize);
GLvoid OutputScissor(int *pos, int esVersion, char *output, int outputSize);
GLvoid OutputStencil(int *pos, int esVersion, char *output, int outputSize);
GLvoid OutputPolygonOffset(int *pos, int esVersion, char *output, int outputSize);
GLvoid OutputSampleCoverage(int *pos, int esVersion, char *output, int outputSize);
GLvoid OutputViewPort(int *pos, int esVersion, char *output, int outputSize);
GLvoid OutputData(int *pos, GLenum type, char *output, int outputSize, char *input);
GLvoid OutputStrcatFast(int pos, char* outBuf, int outputSize, const char* string, ...);
GLvoid PrintMatrixf(GLfloat *matrix, char *output);
GLvoid OutputStates(int *pos, int esVersion, char *output, int outputSize);
GLvoid GetInternalFormatBppFromTextureFormat(GLenum Format, GLenum Type, gceSURF_FORMAT *ImageFormat, GLuint *Bpp, GLuint *fakeBpp);

void DumpMatrix(int *pos, GLenum type, int row, int column, GLvoid *data, char *output, int outputSize);
void DumpVector(int *pos, GLenum type, int num, GLvoid *data, char *output, int outputSize);

GLvoid SetCapability(GLenum cap, GLboolean flag);

void GenDelObjects(GLvoid *pContext,
                   GLchar *output,
                   GLuint outputSize,
                   GLuint count,
                   const GLuint *buffers,
                   const GLchar *funcName,
                   const GLchar *objName);
void PrintParams0(GLchar *output, GLuint outputSize, const GLchar *funcName);
void PrintParams1(GLchar *output, GLuint outputSize, const GLchar *funcName,
                  GLuint param, const GLchar *paramName, PARAM_FORMAT format, GLboolean bShader = GL_FALSE);
void PrintParams2(GLchar *output, GLuint outputSize, const GLchar *funcName,
                  GLuint param1, const GLchar *paramName1, PARAM_FORMAT format1,
                  GLuint param2, const GLchar *paramName2, PARAM_FORMAT format2, GLboolean bShader = GL_FALSE);
void PrintParams3(GLchar *output, GLuint outputSize, const GLchar *funcName,
                  GLuint param1, const GLchar *paramName1, PARAM_FORMAT format1,
                  GLuint param2, const GLchar *paramName2, PARAM_FORMAT format2,
                  GLuint param3, const GLchar *paramName3, PARAM_FORMAT format3);
void PrintParams4(GLchar *output, GLuint outputSize, const GLchar *funcName,
                  GLuint param1, const GLchar *paramName1, PARAM_FORMAT format1,
                  GLuint param2, const GLchar *paramName2, PARAM_FORMAT format2,
                  GLuint param3, const GLchar *paramName3, PARAM_FORMAT format3,
                  GLuint param4, const GLchar *paramName4, PARAM_FORMAT format4);
void PrintParams1Return(GLchar *output, GLuint outputSize, const GLchar *funcName,
                        GLuint param1, const GLchar *paramName1, PARAM_FORMAT format1,
                        GLuint ret, PARAM_FORMAT retFormat);

GLvoid SaveTextureToFile(GLenum target,
                         GLuint x,
                         GLuint y,
                         GLuint z,
                         GLuint width,
                         GLuint height,
                         GLuint texWidth,
                         GLuint texHeight,
                         GLuint texDepth,
                         GLenum format,
                         GLenum type,
                         GLuint level,
                         GLuint srcPixels);

GLvoid SaveCompressedTextureToFile(GLenum target,
                                   GLuint x,
                                   GLuint y,
                                   GLuint z,
                                   GLuint width,
                                   GLuint height,
                                   GLuint texWidth,
                                   GLuint texHeight,
                                   GLuint texDepth,
                                   GLenum format,
                                   GLenum type,
                                   GLuint level,
                                   GLuint srcPixels);

extern char                     tmp[TMP_BUF_SIZE];
extern char                     tmp1[TMP_BUF_SIZE];
extern char                     tmp2[TMP_BUF_SIZE];
extern char                     tmp3[TMP_BUF_SIZE];
extern char                     tmp4[TMP_BUF_SIZE];
extern char                     tmp5[TMP5_BUF_SIZE];
extern char                     tmp6[TMP5_BUF_SIZE];
extern FRAME_BUFFER_MAPPING     frameBufferMap[FRAME_BUFFER_MAP_NUM];
extern RENDER_BUFFER_MAPPING    renderBufferMap[RENDER_BUFFER_MAP_NUM];
extern GLboolean                bUseBufferFunc;
extern GLboolean                bUseUniformFunc;

#endif /* _GLES_CONVERT_H */
