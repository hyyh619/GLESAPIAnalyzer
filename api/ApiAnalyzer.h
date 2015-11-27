#ifndef _API_ANALYZER_H
#define _API_ANALYZER_H

#ifdef VTRACER_USE
#include "APIGLES3Tracer.h"
#elif defined(TCG_USE)
#include "ApiTcg.h"
#else
#include <map>
#include <GLES3/gl31.h>
#endif

using namespace std;

class CApiDumpDraw;

typedef std::map<GLuint, std::string*>   CommandMap;

class CApiAnalyzer
{
public:
#ifdef API_DUMP
    CApiAnalyzer(GLboolean bAnalyzed, const GLchar *filePath, stCmdArgs& options);

    GLvoid          ParseEGLEvent(const stEvent &ev);
    GLvoid          ParseES3Event(const stEvent &ev);
    GLvoid          UpdateContext(const stEvent &ev);
    GLvoid          ShaderSource(GLuint shaderIndex, GLsizei count, GLuint stringAddr, GLuint lenAddr);
    GLvoid          BeginEvent(const stEvent& ev);
    GLvoid          EndEvent(const stEvent& ev);
    GLvoid          SetPlayer(CPlayer *p);
#endif

    CApiAnalyzer();
    ~CApiAnalyzer();

    GLvoid          Release();

private:
#ifdef API_DUMP
    stCmdArgs       m_options;
#endif

    GLboolean       m_bAnalyzeApiDump;
};

GLvoid* GetCurrentDataPtr(GLuint handle);
GLuint  GetCurrentDataLen(GLuint handle);
GLvoid* GetCurrentTexDataPtr(GLuint handle);
GLvoid* GetCurrentVAPDataPtr(GLuint handle);
GLuint  GetCurrentVAPDataLen(GLuint handle);
GLvoid  ReleaseShaderSource(GLchar *p);

#endif