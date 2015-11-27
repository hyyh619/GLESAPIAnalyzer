#ifndef __API_DUMP_DRAW_H
#define __API_DUMP_DRAW_H

#include <GLES3/gl31.h>
#include "ApiEngine.h"

#ifdef WIN32
#define API_DUMP_DRAW_PATH          "./result"
#elif defined(__APPLE__)
#define API_DUMP_DRAW_PATH          "./result"
#elif defined(ANDROID)
#define API_DUMP_DRAW_PATH          "./result"
#endif

class CAnalyzer;

struct stDumpDrawOptions
{
    GLboolean   dumpFrameBmp;
    GLboolean   dumpFramePng;
    GLboolean   dumpDraw;
    GLboolean   dumpDrawPng;
    GLuint      dumpDrawStart;
    GLuint      dumpDrawEnd;
    GLboolean   dumpDepth;
};

class CApiDumpDraw
{
public:
    CApiDumpDraw(CAnalyzer *pAnalyzer);
    ~CApiDumpDraw();

    GLvoid              DumpPng(GLchar *fileName);
    GLvoid              DumpBmp(GLchar *fileName);
    GLvoid              DumpFrame();

    GLvoid              DumpDraw(GLESAPIIndex func);
    GLboolean           GetDumpFileName(GLchar *fileName, const GLchar *dumpName, GLint loop, GLESAPIIndex func);
    GLvoid              DumpDepth(GLESAPIIndex func);

public:
    stDumpDrawOptions   m_options;
    CAnalyzer           *m_pAnalyzer;
    GLuint              m_nCurrentFrameNum;
    GLuint              m_nLoop;

    // Dump depth
    GLuint              m_dumpDepthVert;
    GLuint              m_dumpDepthFrag;
    GLuint              m_dumpDepthProg;

    // Draw engine
    CApiEngine          m_apiEngine;
};

#endif /* __API_DUMP_DRAW_H */