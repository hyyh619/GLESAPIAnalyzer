#ifndef __API_DUMPINFO_H
#define __API_DUMPINFO_H

#include <stdio.h>
#include <map>
#include <GLES3/gl31.h>

#include "utils.h"
#include "GLESGlobal.h"
#include "GLES3VertexBuffer.h"
#include "GLSLShader.h"
#include "GLES3Tex.h"

#define DRAW_BIN_HEADER                 "DRAW_BIN"
#define API_PERFORMANCE_FRAME_NUM       500
#define API_GL_NAME_MAP_SIZE            0x10000
#define API_GL_NAME_KEY(type, old)      (((old >> 16) + (old & 0xFFFF)) | (type << 4))

typedef std::map<GLuint, CShaderObj*>       ApiShaderMap;
typedef std::map<GLuint, CProgramObj*>      ApiProgramMap;

class CGLES3Context;

// these go into the next 8 bits
enum
{
    API_GL_SAMPLER       = 0x100,
    API_GL_PROGRAM       = 0x200,
    API_GL_SHADER        = 0x300,
    API_GL_FRAMEBUFFER   = 0x400,
    API_GL_RENDERBUFFER  = 0x500,
    API_GL_VARRAY        = 0x600,
    API_GL_QUERY         = 0x700,
    API_GL_TFBACK        = 0x800,
    API_GL_SYNC          = 0x900,
    API_GL_TRUE          = 0xa00,
    API_GL_FALSE         = 0xb00,
    API_GL_ARRAY_RECLAIM = 0xc00,
    API_GL_PIPELINE      = 0xd00,
    API_GL_UNIFORM_BLOCK = 0xe00
};

typedef enum _DumpCommand
{
    Dump_DrawElements = 1,
    Dump_DrawArrays,
    Dump_ShaderSource,
    Dump_BindAttribLocation,
    Dump_Uniform,
    Dump_Texture,
    Dump_ActiveTexture,
    Dump_BindTexture,
    Dump_TexParameter,
    Dump_TexImage2D,
    Dump_VertexAttribPointer,
    Dump_ColorMask,
    Dump_Viewport,
    Dump_Enable,
    Dump_Disable,
    Dump_Clear,
    Dump_Invalid
} DumpCommand;

class CFrameInfo
{
public:
    CFrameInfo(GLuint nFrame);
    ~CFrameInfo();

public:
    GLuint          m_nFrame;                       // frame sequence
    GLuint          m_nFrameTimeBeforeEglSwap;      // current frame ticks before eglSwapBuffers
    GLuint          m_nFrameTimeAfterEglSwap;       // current frame ticks after eglSwapBuffers
    GLuint          m_nEventStartTime;              // current start event ticks
    GLuint          m_nFramePlayEventTimeCost;      // only PlayEvent cost time
    GLfloat         m_fFramePlayEventTimeCost;
    GLfloat         m_fFrameTimeCost;               // current frame cost between each eglSwapBuffers
    GLfloat         m_fFrameRenderTimeCost;         // current frame render cost on GLES driver

    GLuint          m_nTotalEvent;                  // all event in one frame.

    /* Draw Info */
    GLuint          m_nDrawCount;
    GLuint          m_nProcessedVertexCount;
    GLuint          m_nPrimitiveCount;
    GLuint          m_nPixelsCount;

    /* Texture Info */
    GLuint          m_nTexImageTotalCount;          // Includes glTexImage2D, glTexSubImage2D, glCompressedTexImage2D, glCompressedTexSubImage2D
    GLuint          m_nTexImageTotalDataSize;       // Includes glTexImage2D, glTexSubImage2D, glCompressedTexImage2D, glCompressedTexSubImage2D
    GLuint          m_nTexImageCompressedCount;
    GLuint          m_nTexImageCompressedDataSize;  // Compressed texture image only
    GLuint          m_nTexImageCount;
    GLuint          m_nTexImageDataSize;            // no compressed texture image.

    /* vertex data info */
    GLuint          m_nBufferDataCount;             // Includes all calls of glBufferData and glBufferSubData
    GLuint          m_nBufferDataSize;              // Includes vertex data size of all glBufferData and glBufferSubData
    GLuint          m_nVertexAttribData;            // Includes glVertexAttribPointer data.
};

typedef std::map<GLuint, CFrameInfo*>       FrameInfoMap;

class CDumpInfo
{
public:
    CDumpInfo(const GLchar *path, CGLES3Context *pContext);
    ~CDumpInfo();

    GLvoid              Release();
    GLvoid              OutputFunCallNum();
    GLvoid              OutputStatistic();
    GLvoid              AddFrame();
    GLvoid              BeginEvent(GLESAPIIndex func);
    GLvoid              EndEvent(GLESAPIIndex func);
    GLvoid              BeginRender();
    GLvoid              EndRender();
    GLvoid              BeginFrameInfo(GLESAPIIndex func);
    GLvoid              EndFrameInfo(GLESAPIIndex func);
    CFrameInfo*         FindFrameInfo(GLuint nFrame);
    GLvoid              InsertFrameInfo(CFrameInfo *pFrame);
    GLboolean           IsDrawCommand(GLESAPIIndex name);
    GLboolean           IsDrawCommandWithoutClear(GLESAPIIndex name);
    GLboolean           IsDrawNeedDump();
    GLvoid              DumpColorBuffer(GLchar *fileName);
    GLvoid              DumpDraw(GLESAPIIndex func);
    GLvoid              DumpDrawStates();
    GLvoid              DumpDrawElements(GLESAPIIndex func);
    GLvoid              DumpDrawArrays(GLESAPIIndex func);
    GLvoid              DumpVertexData(GLESAPIIndex func, CFrameInfo *pFrame, GLuint vertexCount, GLuint start);
    GLvoid              DumpShader(GLESAPIIndex func, CFrameInfo *pFrame);
    GLvoid              GetVertexAttribData(GLESAPIIndex func, CFrameInfo *pFrame, GLuint vertexCount);
    GLvoid              DumpTexture(GLint loc, GLenum type, GLuint size);
    GLvoid              DumpTextureImage(GLuint unit, stTexImage *pTexImage);
    GLvoid              DumpTextureParameter(GLenum target, GLuint unit);
    GLvoid              DumpUniformValue(GLint loc, GLenum type, GLuint size);
    GLvoid              DumpToFile(FILE *pFile, const GLchar *fmt, ...);
    GLvoid              DumpToSingleFile(GLchar *pInfo, GLuint len);
    GLvoid              SaveIndexFile(GLESAPIIndex func);
    GLvoid              SaveVBO(GLuint vbo, GLuint vertexCount, GLuint strideo, GLuint start);
    GLvoid              SaveVAP(GLuint pointer, GLuint offset, GLuint vertexCount, GLuint stride);
    GLvoid              SaveShaderSource(GLESAPIIndex func, CFrameInfo *pFrame);

    GLvoid              WriteDraw(GLESAPIIndex func);
    GLvoid              WriteDrawElements(GLESAPIIndex func);
    GLvoid              WriteDrawArrays(GLESAPIIndex func);
    GLvoid              WriteShader(GLESAPIIndex func);
    GLvoid              WriteUniformValue(GLint loc, GLenum type, GLuint size);
    GLvoid              WriteTexture(GLint loc, GLenum type, GLuint size);
    GLvoid              WriteTextureParameter(GLenum target, GLuint unit);
    GLvoid              WriteTextureImage(GLuint unit, stTexImage *pTexImage);
    GLvoid              WriteVertexData(GLESAPIIndex func);
    GLvoid              WriteDrawStates();
    GLvoid              WriteClear();
    GLvoid              CompressFile(const GLchar *fileName);

    CShaderObj*         FindShaderObject(GLuint name);
    CProgramObj*        FindProgramObject(GLuint name);

    CAttribObj*         FindAttribObjByIndex(GLuint index);
    CAttribObj*         FindAttributeByName(CProgramObj *program, GLchar *name);
    CUniformObj*        FindUniformByOldLoc(GLuint program, GLint location);
    GLvoid              SaveProgram();
    GLvoid              LoadProgram();

public:
    CGLES3Context       *m_pContext;

    GLboolean           m_bWriteDrawBin;
    GLboolean           m_bTimeOnly;
    GLboolean           m_bCaculateRenderPixels;
    GLboolean           m_bDumpPng;
    GLuint              m_nCurrentFrame;            // Record current frame
    GLuint              m_nSampleFrame;             // we will sample render info after running each m_nSampleFrame
    GLfloat             m_fCurrentFrameCost;        // It's time between eglSwapBuffers.
    GLuint              m_nLastFrameTicks;
    GLuint              m_nEventBeginTicks;
    GLfloat             m_fCurrentFrameEventCost;   // It's time of PlayEvent cost between eglSwapBuffers.
    GLuint              m_nRenderStartTime;
    GLuint              m_nRenderEndTime;
    GLuint              m_nRenderTotalTime;
    GLfloat             m_fTotalRenderTime;
    GLfloat             m_fTotalPlayEventTime;
    GLfloat             m_fTotalRunTime;

    FrameInfoMap        m_frameMap;

    GLboolean           m_bDumpEnable;
    FILE                *m_pDumpFile;
    GLchar              *m_strDumpFilePath;
    FILE                *m_pDumpDrawFile;
    GLchar              *m_strDumpDrawFilePath;
    GLchar              *m_strPath;
    GLchar              m_strDrawBinary[256];
    FILE                *m_pDrawBinFile;
    FILE                *m_pDrawSingleFile;

    GLchar              *m_pPixelBuffers;
    GLchar              *m_pLastPixelBuffers;
    GLuint              m_nPixelBuffersSize;
    GLuint              m_nCurrentPixels;
    GLuint              m_nWidth;
    GLuint              m_nHeight;

    GLuint              m_nDrawTotal;

    GLuint              m_nCurrentProgram;
    GLuint              m_nOrigProgram;

    GLclampf            m_clearColorR;
    GLclampf            m_clearColorG;
    GLclampf            m_clearColorB;
    GLclampf            m_clearColorA;
    GLclampf            m_clearDepth;
    GLint               m_clearStencil;
    GLint               m_clearBits;

    GLuint              m_frameCount;
    GLuint              m_startTime;
    GLuint              m_totalFrames;
    GLuint              m_totalTime;

    GLboolean           m_bOutputFunCallNum;

    GLint               m_nSavedProgram;
};

#endif /*__API_DUMPINFO_H*/
