#include <stdio.h>
#include <stdlib.h>
#ifdef WIN32
#include <io.h>
#endif

#include "zlib.h"
#include "types.h"
#include "PngInterface.h"
#include "bitmap.h"
#include "APIOS.h"
#include "utils.h"
#include "GLESGlobal.h"
#include "TranslateGL.h"
#include "ApiDebug.h"
#include "ApiDumpUtils.h"
#include "ApiDumpInfo.h"
#include "ApiGLES3Context.h"
#include "ApiEGLContext.h"

GLuint      g_startDraw = API_DUMP_DRAW_INFO_START;
GLuint      g_endDraw   = API_DUMP_DRAW_INFO_END;
GLuint      g_funcCallNum[GLES_FUNCTION_TOTAL_NUMBER];

GLboolean CDumpInfo::IsDrawNeedDump()
{
    if ((g_startDraw != 0 && m_nDrawTotal >= g_startDraw) &&
        (g_endDraw != 0 && m_nDrawTotal <= g_endDraw))
    {
        return GL_TRUE;
    }

    return GL_FALSE;
}

CDumpInfo::CDumpInfo(const GLchar *path, CGLES3Context *pContext)
{
    GLchar  *pFileName      = (GLchar*)malloc(256);
    GLchar  *pDrawFileName  = (GLchar*)malloc(256);
    GLchar  *pPath          = (GLchar*)malloc(256);

    m_pContext = pContext;

    if (pFileName == NULL || pDrawFileName == NULL || pPath == NULL)
    {
        if (pDrawFileName)
        {
            free(pDrawFileName);
        }

        if (pFileName)
        {
            free(pFileName);
        }

        if (pPath)
        {
            free(pPath);
        }

        return;
    }

    memset(pFileName, 0, 256);
    memset(pDrawFileName, 0, 256);
    memset(pPath, 0, 256);

    m_bWriteDrawBin         = API_WRITE_DRAW_BIN;
    m_bDumpPng              = GL_TRUE;
    m_bTimeOnly             = API_DUMP_TIME_INFO_ONLY;
    m_bCaculateRenderPixels = GL_FALSE;
    if (m_bTimeOnly)
    {
        sprintf(pFileName, "%sdraw/api_dump_info_time_only.csv", path);
    }
    else
    {
        sprintf(pFileName, "%sdraw/api_dump_info_all.csv", path);
    }

    sprintf(pDrawFileName, "%sdraw/api_dump_info.hy", path);
    sprintf(pPath, "%s", path);

    m_nPixelBuffersSize             = 0;
    m_pPixelBuffers                 = NULL;
    m_nWidth                        = 0;
    m_nHeight                       = 0;

    m_nCurrentFrame                 = 0;
    m_nSampleFrame                  = 10;
    m_fCurrentFrameCost             = 0.0f;
    m_nLastFrameTicks               = 0;
    m_fCurrentFrameEventCost        = 0.0f;
    m_bDumpEnable                   = API_DUMP_ENABLE;
    m_strDumpFilePath               = pFileName;
    m_strDumpDrawFilePath           = pDrawFileName;
    m_strPath                       = pPath;

    m_nDrawTotal                    = 0;

    m_clearColorR                   = 0.0f;
    m_clearColorG                   = 0.0f;
    m_clearColorB                   = 0.0f;
    m_clearColorA                   = 0.0f;
    m_clearDepth                    = 1.0f;
    m_clearStencil                  = 0;

    m_pDrawSingleFile   = NULL;
    m_pDumpFile         = fopen(m_strDumpFilePath, "w");
    m_pDumpDrawFile     = fopen(m_strDumpDrawFilePath, "w");
    if (m_pDumpFile == NULL)
    {
        m_bDumpEnable = GL_FALSE;
        return;
    }

    CFrameInfo *pFrame = NULL;
    pFrame = new CFrameInfo(++m_nCurrentFrame);
    if (pFrame == NULL)
    {
        m_bDumpEnable = GL_FALSE;
        return;
    }
    InsertFrameInfo(pFrame);

    m_frameCount    = 0;
    m_startTime     = 0;
    m_totalFrames   = 0;
    m_totalTime     = 0;

    m_nRenderStartTime      = 0;
    m_nRenderEndTime        = 0;
    m_nRenderTotalTime      = 0;

    m_fTotalRenderTime      = 0.0f;
    m_fTotalPlayEventTime   = 0.0f;
    m_fTotalRunTime         = 0.0f;

    memset(g_funcCallNum, 0, sizeof(GLuint) * GLES_FUNCTION_TOTAL_NUMBER);
    m_bOutputFunCallNum     = API_DUMP_ENABLE;
}

GLvoid CDumpInfo::OutputFunCallNum()
{
    GLchar  buf[512];
    FILE    *pFile = NULL;

    memset(buf, 0, 512);
    sprintf(buf, "%stcg_client_dump_info_call_num.csv", m_strPath);

    pFile = fopen(buf, "w");
    if (pFile == NULL)
    {
        return;
    }

    memset(buf, 0, 512);
    sprintf(buf, "Name,Count\n");
    fwrite(buf, 1, strlen(buf), pFile);

    for (GLuint i=0; i<GLES_FUNCTION_TOTAL_NUMBER; i++)
    {
        if (g_funcCallNum[i] > 0)
        {
            memset(buf, 0, 128);
            sprintf(buf, "%s,%d\n", GetAPIName((GLESAPIIndex)i), g_funcCallNum[i]);
            fwrite(buf, 1, strlen(buf), pFile);
        }
    }

    fclose(pFile);
}

CDumpInfo::~CDumpInfo()
{
}

GLvoid CDumpInfo::OutputStatistic()
{
    FrameInfoMap::iterator  it;
    GLchar buf[512];

    memset(buf, 0, 512);
    if (m_bTimeOnly)
    {
        sprintf(buf, "Frame,eglSwapTime,PlayEventTime,RenderTime\n");
    }
    else
    {
        sprintf(buf, "Frame,eglSwapTime,PlayEventTime,RenderTime,totalEvent,totalDraw,primitiveCount,pixels,texImageSize,bufferSize,vertexDataSize\n");
    }

    fwrite(buf, 1, strlen(buf), m_pDumpFile);

    sprintf(buf, "1,%8.3f,%8.3f,%8.3f\n",
            m_fTotalRunTime,
            m_fTotalPlayEventTime,
            m_fTotalRenderTime);
    fwrite(buf, 1, strlen(buf), m_pDumpFile);

    it = m_frameMap.begin();
    if (it != m_frameMap.end())
    {
        it ++;
        for (; it != m_frameMap.end(); it ++)
        {
            CFrameInfo  *p = it->second;

            p->m_nTexImageTotalDataSize = p->m_nTexImageCompressedDataSize + p->m_nTexImageDataSize;
            p->m_nTexImageTotalCount    = p->m_nTexImageCompressedCount + p->m_nTexImageCount;

            /*
             *  Output csv format:
             *  The colunms are the following:
             *  1.  current frame number
             *  2.  time cost of current frame, including GPU, Driver, App and sleep time.
             *  3.  time cost of only PlayEvent function.
             *  4.  time cost of render function.
             *  5.  total event of current frame.
             *  6.  draw call count of current frame
             *  7.  primitive count of current frame
             *  8.  pixel count of current frame
             *  9.  total image data of current frame
             *  10. total vertex buffer size
             *  11. total vertex attribute data which comes from glVertexAttribPointer.
             */
            memset(buf, 0, 512);
            if (m_bTimeOnly)
            {
                sprintf(buf, "%d,%8.3f,%8.3f,%8.3f\n",
                        p->m_nFrame,
                        p->m_fFrameTimeCost,
                        p->m_fFramePlayEventTimeCost,
                        p->m_fFrameRenderTimeCost);
            }
            else
            {
                sprintf(buf, "%d,%8.03f,%8.03f,%8.3f,%d,%d,%d,%d,%d,%d,%d\n",
                        p->m_nFrame,
                        p->m_fFrameTimeCost,
                        p->m_fFramePlayEventTimeCost,
                        p->m_fFrameRenderTimeCost,
                        p->m_nTotalEvent,
                        p->m_nDrawCount,
                        p->m_nPrimitiveCount,
                        p->m_nPixelsCount,
                        p->m_nTexImageTotalDataSize,
                        p->m_nBufferDataSize,
                        p->m_nVertexAttribData);
            }
            fwrite(buf, 1, strlen(buf), m_pDumpFile);
        }

        for (it = m_frameMap.begin(); it != m_frameMap.end();)
        {
            CFrameInfo  *p = it->second;
            m_frameMap.erase(it++);
            delete p;
        }
    }
}

GLvoid CDumpInfo::Release()
{
    FrameInfoMap::iterator  it;

    if (m_bOutputFunCallNum)
    {
        OutputFunCallNum();
    }

    if (m_bDumpEnable)
    {
        OutputStatistic();
    }

    if (m_pDumpFile)
    {
        fclose(m_pDumpFile);
    }

    if (m_strDumpFilePath)
    {
        free(m_strDumpFilePath);
    }

    if (m_pDumpDrawFile)
    {
        fclose(m_pDumpDrawFile);
    }

    if (m_strDumpDrawFilePath)
    {
        free(m_strDumpDrawFilePath);
    }

    if (m_strPath)
    {
        free(m_strPath);
    }
}

GLvoid CDumpInfo::DumpToSingleFile(GLchar *pInfo, GLuint len)
{
    GLuint  writeBytes;

    if (m_pDrawSingleFile == NULL)
    {
        GLchar  fileName[256];
        GLchar  path[256];
        FILE    *pFile = NULL;

        memset(fileName, 0, 256);

#ifdef WIN32
        memset(path, 0, 256);
        sprintf(path, "%sdraw/frame%d", m_strPath, m_nCurrentFrame);
        BOOL bRes = apiOS_DumpFilePathExists(path);
        if (!bRes)
        {
            bRes = apiOS_DumpCreateDirectory(path);
            if (!bRes)
            {
                printf("%s(%d): execute %s.", __FUNCTION__, __LINE__, path);
                return;
            }
        }
#endif

        sprintf(fileName, "%sdraw/frame%d/frame%d_glDrawElements%d.hy", m_strPath, m_nCurrentFrame, m_nCurrentFrame, m_nDrawTotal);

        pFile = fopen(fileName, "w");
        if (pFile == NULL)
        {
            printf("%s(%d): dump draw file is null.", __FUNCTION__, __LINE__);
            return;
        }

        m_pDrawSingleFile = pFile;
    }

    writeBytes = fwrite(pInfo, 1, len, m_pDrawSingleFile);
    if (len != writeBytes)
    {
        printf("%s(%d): cannot write to dump draw file.", __FUNCTION__, __LINE__);
        return;
    }
}

GLvoid CDumpInfo::DumpToFile(FILE *pFile, const GLchar *fmt, ...)
{
    GLchar  buf[2048];
    va_list arguments;
    GLuint  writeBytes;
    GLuint  len = 0;

    if (pFile == NULL)
    {
        printf("%s(%d): dump draw file is null.", __FUNCTION__, __LINE__);
        return;
    }

    memset(buf, 0, 2048);
    va_start(arguments, fmt);
    vsnprintf(buf, 2048, fmt, arguments);
    va_end(arguments);

    len = strlen(buf);
    writeBytes = fwrite(buf, 1, len, pFile);
    if (len != writeBytes)
    {
        printf("%s(%d): cannot write to dump draw file.", __FUNCTION__, __LINE__);
        return;
    }

#if API_DUMP_SINGLE_DRAW_INFO
    DumpToSingleFile(buf, len);
#endif
}

GLvoid CDumpInfo::AddFrame()
{
    m_nCurrentFrame ++;
}

GLvoid CDumpInfo::BeginRender()
{
    m_nRenderStartTime = apiOS_GetMicroSeconds();
}

GLvoid CDumpInfo::EndRender()
{
    m_nRenderEndTime = apiOS_GetMicroSeconds();
    m_nRenderTotalTime += (m_nRenderEndTime - m_nRenderStartTime);
}

GLvoid CDumpInfo::BeginEvent(GLESAPIIndex func)
{
    if (m_frameCount == 0)
    {
        m_startTime = apiOS_GetMicroSeconds();
    }

    if (func == EGL_API_eglSwapBuffers)
    {
        m_frameCount ++;
    }

    if (!m_bDumpEnable)
        return;

    g_funcCallNum[func] ++;

    BeginFrameInfo(func);
}

GLvoid CDumpInfo::EndEvent(GLESAPIIndex func)
{
    if (m_frameCount == API_PERFORMANCE_FRAME_NUM)
    {
        GLuint     endTime     = apiOS_GetMicroSeconds();
        GLfloat    costTime    = 0.0f;
        GLfloat    totalTime   = 0.0f;
        GLfloat    fps         = 0.0f;

        m_totalFrames   += m_frameCount;
        m_totalTime     += (endTime - m_startTime);

        costTime    = ((GLfloat)(endTime - m_startTime)) / (1000000.0f);
        totalTime   = ((GLfloat)m_totalTime) / (1000000.0f);
        fps         = ((GLfloat)m_totalFrames) / totalTime;

        printf("cost for %d frames: %.3fs, total: %.3fs, %.3fFPS, totalFrame: %d\n",
               API_PERFORMANCE_FRAME_NUM, costTime, totalTime, fps, m_totalFrames);

        m_startTime     = 0;
        m_frameCount    = 0;
    }

    if (!m_bDumpEnable)
        return;

    EndFrameInfo(func);
}

CFrameInfo* CDumpInfo::FindFrameInfo(GLuint nFrame)
{
    FrameInfoMap::iterator  it = m_frameMap.find(m_nCurrentFrame);

    if (it != m_frameMap.end())
    {
        return it->second;
    }

    return NULL;
}

GLvoid CDumpInfo::InsertFrameInfo(CFrameInfo *pFrame)
{
    m_frameMap.insert(std::make_pair(pFrame->m_nFrame, pFrame));
}

GLvoid CDumpInfo::GetVertexAttribData(GLESAPIIndex func, CFrameInfo *pFrame, GLuint vertexCount)
{
    // Get vertex data size
    for (GLuint i=0; i<API_VERTEX_ATTRIBUTES_NUM; i++)
    {
        if (!(VERTEX_ATTRIB_BITS & (1 << i)))
            continue;

        stVertAttrPtr   *p          = VERTEX_ATTRIB_PTR(i);
        GLenum          vertexType  = p->type;
        GLuint          vertexSize  = p->size;
        GLuint          size        = 0;

        if (p->size == 0 && p->type == 0)
            continue;

        if (p->bUseVBO)
            continue;

        switch (vertexType)
        {
            case GL_UNSIGNED_BYTE:
                size = vertexSize * vertexCount;
                break;

            case GL_UNSIGNED_SHORT:
            case GL_SHORT:
                size = vertexSize * vertexCount * 2;
                break;

            case GL_UNSIGNED_INT:
            case GL_INT:
            case GL_FLOAT:
            case GL_FIXED:
                size = vertexSize * vertexCount * 4;
                break;

            default:
                printf("%s(%d): cannot get vertex type(0x%04X)", __FUNCTION__, __LINE__, vertexType);
                break;
        }

        pFrame->m_nVertexAttribData += size;
    }
}

GLvoid CDumpInfo::DumpUniformValue(GLint loc, GLenum type, GLuint size)
{
    GLuint      typeSize    = 0;
    GLuint      total       = 0;
    CProgramObj *pProg      = FindProgramObject(m_nOrigProgram);
    GLvoid      *pData      = pProg->GetUniformValueByLoc(loc);

    typeSize    = DumpGetShaderDataSize(type);
    total       = typeSize * size;

    for (GLuint k=0; k<size; k++)
    {
        GLfloat *pSrc = (GLfloat*)((GLuint)pData + typeSize*k);
        GLint   *pInt = (GLint*)((GLuint)pData + typeSize*k);

        if (type == GL_FLOAT_MAT4)
        {
            DumpToFile(m_pDumpDrawFile, "   mat%02d= %f, %f, %f, %f\n", k, pSrc[0], pSrc[1], pSrc[2], pSrc[3]);
            DumpToFile(m_pDumpDrawFile, "          %f, %f, %f, %f\n", pSrc[4], pSrc[5], pSrc[6], pSrc[7]);
            DumpToFile(m_pDumpDrawFile, "          %f, %f, %f, %f\n", pSrc[8], pSrc[9], pSrc[10], pSrc[11]);
            DumpToFile(m_pDumpDrawFile, "          %f, %f, %f, %f\n", pSrc[12], pSrc[13], pSrc[14], pSrc[15]);
        }
        else if (type == GL_FLOAT_MAT3)
        {
            DumpToFile(m_pDumpDrawFile, "   mat%02d= %f, %f, %f\n", k, pSrc[0], pSrc[1], pSrc[2]);
            DumpToFile(m_pDumpDrawFile, "          %f, %f, %f\n", pSrc[3], pSrc[4], pSrc[5]);
            DumpToFile(m_pDumpDrawFile, "          %f, %f, %f\n", pSrc[6], pSrc[7], pSrc[8]);
        }
        else if (type == GL_FLOAT_MAT2)
        {
            DumpToFile(m_pDumpDrawFile, "   mat%02d= %f, %f\n", k, pSrc[0], pSrc[1]);
            DumpToFile(m_pDumpDrawFile, "          %f, %f\n", pSrc[2], pSrc[3]);
        }
        else if (type == GL_FLOAT)
        {
            DumpToFile(m_pDumpDrawFile, "   float%02d= %f\n", k, pSrc[0]);
        }
        else if (type == GL_FLOAT_VEC2)
        {
            DumpToFile(m_pDumpDrawFile, "   vec2_%02d= %f, %f\n", k, pSrc[0], pSrc[1]);
        }
        else if (type == GL_FLOAT_VEC3)
        {
            DumpToFile(m_pDumpDrawFile, "   vec3_%02d= %f, %f, %f\n", k, pSrc[0], pSrc[1], pSrc[2]);
        }
        else if (type == GL_FLOAT_VEC4)
        {
            DumpToFile(m_pDumpDrawFile, "   vec4_%02d= %f, %f, %f, %f\n", k, pSrc[0], pSrc[1], pSrc[2], pSrc[3]);
        }
        else if (type == GL_INT)
        {
            DumpToFile(m_pDumpDrawFile, "   int%02d= %f\n", k, pInt[0]);
        }
        else if (type == GL_INT_VEC2)
        {
            DumpToFile(m_pDumpDrawFile, "   ivec2_%02d= %f, %f\n", k, pInt[0], pInt[1]);
        }
        else if (type == GL_INT_VEC3)
        {
            DumpToFile(m_pDumpDrawFile, "   ivec3_%02d= %f, %f, %f\n", k, pInt[0], pInt[1], pInt[2]);
        }
        else if (type == GL_INT_VEC4)
        {
            DumpToFile(m_pDumpDrawFile, "   ivec4_%02d= %f, %f, %f, %f\n", k, pInt[0], pInt[1], pInt[2], pInt[3]);
        }
    }
    DumpToFile(m_pDumpDrawFile, "uniform_end\n");
}

GLvoid CDumpInfo::DumpShader(GLESAPIIndex func, CFrameInfo *pFrame)
{
    GLuint  shader[2];
    GLsizei count               = 0;
    GLint   activeAttributes    = 0;
    GLint   activeUniforms      = 0;
    GLint   maxAttributeLen     = 0;
    GLint   maxUniformLen       = 0;
    GLchar  *pAttributeName     = NULL;
    GLchar  *pUniformName       = NULL;

    g_opengl->glGetAttachedShaders(m_nCurrentProgram, 2, &count, shader);

    DumpToFile(m_pDumpDrawFile, "**********************************program**********************************\n");
    for (GLuint i=0; i<2; i++)
    {
        GLenum  type;
        GLint   shaderLen   = 0;
        GLchar  *pSrc       = NULL;
        GLsizei size        = 0;

        g_opengl->glGetShaderiv(shader[i], GL_SHADER_TYPE, (GLint*)&type);
        if (type == GL_VERTEX_SHADER)
        {
            DumpToFile(m_pDumpDrawFile, "VertexShader:\n");
        }
        else if (type == GL_FRAGMENT_SHADER)
        {
            DumpToFile(m_pDumpDrawFile, "FragmentShader:\n");
        }

        g_opengl->glGetShaderiv(shader[i], GL_SHADER_SOURCE_LENGTH, &shaderLen);
        pSrc = (GLchar*)malloc(shaderLen + 1);
        if (pSrc == NULL)
        {
            printf("%s(%d): cannot allocte for shader source\n", __FUNCTION__, __LINE__);
            continue;
        }
        memset(pSrc, 0, shaderLen+1);
        g_opengl->glGetShaderSource(shader[i], shaderLen, &size, pSrc);
        if (size > 0)
        {
            DumpToFile(m_pDumpDrawFile, pSrc);
            DumpShaderToSingleFile(m_strPath, shader[i], type, pSrc);
        }
        else
        {
            CProgramObj *p = FindProgramObject(m_nOrigProgram);

            if (type == GL_VERTEX_SHADER)
            {
                DumpToFile(m_pDumpDrawFile, p->m_pVertex->m_source);
                DumpShaderToSingleFile(m_strPath, shader[i], type, p->m_pVertex->m_source);
            }
            else if (type == GL_FRAGMENT_SHADER)
            {
                DumpToFile(m_pDumpDrawFile, p->m_pFragment->m_source);
                DumpShaderToSingleFile(m_strPath, shader[i], type, p->m_pVertex->m_source);
            }
        }

        FREE(pSrc);
    }
    DumpToFile(m_pDumpDrawFile, "\nAttributes:\n");

    g_opengl->glGetProgramiv(m_nCurrentProgram, GL_ACTIVE_ATTRIBUTES, &activeAttributes);
    g_opengl->glGetProgramiv(m_nCurrentProgram, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &maxAttributeLen);
    g_opengl->glGetProgramiv(m_nCurrentProgram, GL_ACTIVE_UNIFORMS, &activeUniforms);
    g_opengl->glGetProgramiv(m_nCurrentProgram, GL_ACTIVE_UNIFORM_MAX_LENGTH, &maxUniformLen);
    pAttributeName  = (GLchar*)malloc(maxAttributeLen);
    pUniformName    = (GLchar*)malloc(maxUniformLen);

    for (GLint i=0; i<activeAttributes; i++)
    {
        GLsizei len         = 0;
        GLint   size        = 0;
        GLenum  type        = 0;
        GLint   loc         = -1;
        GLint   norm        = -1;
        GLint   stride      = 0;
        GLint   bufBind     = 0;
        GLint   vertexType  = 0;
        GLint   pointer     = 0;

        memset(pAttributeName, 0, maxAttributeLen);
        g_opengl->glGetActiveAttrib(m_nCurrentProgram, i, maxAttributeLen, &len, &size, &type, pAttributeName);
        loc = g_opengl->glGetAttribLocation(m_nCurrentProgram, pAttributeName);
        TranslateShaderDataType(type, dumpTmp1);
        DumpToFile(m_pDumpDrawFile, "%s: type=%s, ", pAttributeName, dumpTmp1);

        g_opengl->glGetVertexAttribiv(loc, GL_VERTEX_ATTRIB_ARRAY_TYPE, &vertexType);
        g_opengl->glGetVertexAttribiv(loc, GL_VERTEX_ATTRIB_ARRAY_STRIDE, &stride);
        g_opengl->glGetVertexAttribiv(loc, GL_VERTEX_ATTRIB_ARRAY_BUFFER_BINDING, &bufBind);
        g_opengl->glGetVertexAttribiv(loc, GL_VERTEX_ATTRIB_ARRAY_NORMALIZED, &norm);
        g_opengl->glGetVertexAttribiv(loc, GL_VERTEX_ATTRIB_ARRAY_SIZE, &size);
        g_opengl->glGetVertexAttribiv(loc, GL_VERTEX_ATTRIB_ARRAY_POINTER, &pointer);
        TranslateDataType(vertexType, dumpTmp1);
        DumpToFile(m_pDumpDrawFile, "vertex%d, %s, size=%d, stride=%d, bufBind=%d, pointer=%d, norm=%d\n",
            loc, dumpTmp1, size, stride, bufBind, pointer, norm);
    }
    DumpToFile(m_pDumpDrawFile, "\nUniforms:\n");

    for (GLint i=0; i<activeUniforms; i++)
    {
        GLsizei len     = 0;
        GLint   size    = 0;
        GLenum  type    = 0;
        GLint   loc     = -1;

        memset(pUniformName, 0, maxUniformLen);
        g_opengl->glGetActiveUniform(m_nCurrentProgram, i, maxUniformLen, &len, &size, &type, pUniformName);
        loc = g_opengl->glGetUniformLocation(m_nCurrentProgram, pUniformName);
        TranslateShaderDataType(type, dumpTmp1);
        DumpToFile(m_pDumpDrawFile, "%s: type=%s, size=%d\n", pUniformName, dumpTmp1, size);

        if (type == GL_SAMPLER_2D ||
            type == GL_SAMPLER_CUBE)
        {
            DumpTexture(loc, type, size);
            DumpToFile(m_pDumpDrawFile, "uniform_end\n");
        }
        else
        {
            DumpUniformValue(loc, type, size);
        }
        DumpToFile(m_pDumpDrawFile, "\n");
    }

    FREE(pAttributeName);
    FREE(pUniformName);

    DumpToFile(m_pDumpDrawFile, "**********************************program**********************************\n\n");
}

GLvoid CDumpInfo::SaveVBO(GLuint vbo, GLuint vertexCount, GLuint stride, GLuint start)
{
    GLchar fileName[256];

    memset(fileName, 0, 256);
    sprintf(fileName, "frame%d_glDrawElements%d", m_nCurrentFrame, m_nDrawTotal);

    CBufObj *pBuffer = GET_VBO(vbo);
    if (pBuffer)
    {
        GLchar  name[256];
        FILE    *pFile      = NULL;
        GLuint  writeBytes  = 0;
        GLuint  count       = (vertexCount+1) * stride;
        GLchar  *pSrc       = (GLchar*)pBuffer->GetCurrentDataPointer() + start * stride;

        memset(name, 0, 256);
        sprintf(name, "%sdraw/frame%d/%s_VBO_%d.dat", m_strPath, m_nCurrentFrame, fileName, vbo);
        pFile = fopen(name, "wb");
        if (pFile != NULL)
        {
            writeBytes = fwrite(pBuffer->GetCurrentDataPointer(), 1, count, pFile);
            if (count != writeBytes)
            {
                printf("%s(%d): Cannot write vbo file.", __FUNCTION__, __LINE__);
            }

            fclose(pFile);
        }
    }
}

GLvoid CDumpInfo::SaveVAP(GLuint pointer, GLuint offset, GLuint vertexCount, GLuint stride)
{
    GLchar  fileName[256];
    GLvoid  *pSrc = (GLvoid*)((GLuint)(pointer) + offset);

    memset(fileName, 0, 256);
    sprintf(fileName, "frame%d_glDrawElements%d", m_nCurrentFrame, m_nDrawTotal);

    GLchar  name[256];
    FILE    *pFile      = NULL;
    GLuint  count       = (vertexCount+1) * stride;
    GLuint  writeBytes  = 0;

    memset(name, 0, 256);
    sprintf(name, "%sdraw/frame%d/%s_VAP_%X.dat", m_strPath, m_nCurrentFrame, fileName, pointer);
    pFile = fopen(name, "wb");
    if (pFile != NULL)
    {
        writeBytes = fwrite(pSrc, 1, count, pFile);
        if (count != writeBytes)
        {
            printf("%s(%d): Cannot write vap file.", __FUNCTION__, __LINE__);
        }

        fclose(pFile);
    }
}

GLvoid CDumpInfo::DumpVertexData(GLESAPIIndex func, CFrameInfo *pFrame, GLuint vertexCount, GLuint start)
{
    stVertAttrPtr   *p          = NULL;
    GLvoid          *pData      = NULL;
    GLvoid          *pSrc       = NULL;
    GLuint          stride      = 0;
    GLuint          pointer     = 0;
    CAttribObj      *pCurrent   = NULL;

    DumpToFile(m_pDumpDrawFile, "**********************************attribute**********************************\n");
    for (GLuint j=0; j<API_VERTEX_ATTRIBUTES_NUM; j++)
    {
        if ((VERTEX_ATTRIB_BITS & (0x1 << j)) == 0)
            continue;

        p = VERTEX_ATTRIB_PTR(j);
        if (p->size == 0)
            continue;

        pCurrent = FindAttribObjByIndex(j);
        if (pCurrent == NULL)
            continue;

        p->realIndex = pCurrent->nLoc;
        TranslateDataType(p->type, dumpTmp1);

        if (p->bUseVBO)
        {
            pointer = p->pointer;

            // Save vbo file
#if API_WRITE_DRAW_VBO_BUFFER
            SaveVBO(p->vbo, vertexCount, p->stride, start);
#endif
        }
        else
        {
            pointer = ((p->pointer << 12)+p->offset);

#if API_WRITE_DRAW_VBO_BUFFER
            SaveVAP(p->pointer, p->offset, vertexCount, p->stride);
#endif
        }

        DumpToFile(m_pDumpDrawFile, "glVertexAttribPointer(index=%d, size=%d, n=%d, stride=%d, %s, p=0x%08X)\n",
                  p->realIndex, p->size, p->normalized, p->stride, dumpTmp1, pointer);

        p->bytesPerPixel = p->size * DumpGetDataSize(p->type);
    }
    DumpToFile(m_pDumpDrawFile, "\n");

    for (GLuint i=0; i<vertexCount; i++)
    {
        DumpToFile(m_pDumpDrawFile, "#%d: ", i);
        for (GLuint j=0; j<API_VERTEX_ATTRIBUTES_NUM; j++)
        {
            if ((VERTEX_ATTRIB_BITS & (0x1 << j)) == 0)
                continue;

            p = VERTEX_ATTRIB_PTR(j);
            if (p->size == 0)
                continue;

            if (p->bUseVBO)
            {
                CBufObj *pBuffer = GET_VBO(p->vbo);

                if (pBuffer->GetCurrentDataPointer())
                {
                    pSrc = (GLvoid*)((GLuint)pBuffer->GetCurrentDataPointer() + p->pointer);
                }
                else
                {
                    pSrc = NULL;
                }
            }
            else
            {
                GLvoid *pData = (GLvoid*)(p->pointer);
                pSrc = (GLvoid*)((GLuint)pData + p->offset);
            }

            if (!pSrc)
                continue;

            if (p->stride)
            {
                stride = p->stride;
            }
            else
            {
                stride = p->bytesPerPixel;
            }

            pSrc = (GLvoid*)((GLuint)pSrc + i * stride);
            DumpToFile(m_pDumpDrawFile, "(");
            pData = pSrc;
            for (GLint k=0; k<p->size-1; k++)
            {
                switch (p->type)
                {
                case GL_FLOAT:
                    DumpToFile(m_pDumpDrawFile, "%f, ", *(GLfloat*)pData);
                    //DumpToFile(m_pDumpDrawFile, "0x%08X, ", *(GLuint*)pData);
                    pData = (GLvoid*)((GLuint)pData + 4);
                    break;

                case GL_UNSIGNED_BYTE:
                    DumpToFile(m_pDumpDrawFile, "%03d, ", *(GLubyte*)pData);
                    pData = (GLvoid*)((GLuint)pData + 1);
                    break;

                case GL_SHORT:
                    DumpToFile(m_pDumpDrawFile, "%04d, ", *(GLshort*)pData);
                    pData = (GLvoid*)((GLuint)pData + 2);
                    break;

                case GL_UNSIGNED_SHORT:
                    DumpToFile(m_pDumpDrawFile, "%04d, ", *(GLushort*)pData);
                    pData = (GLvoid*)((GLuint)pData + 2);
                    break;

                case GL_INT:
                    DumpToFile(m_pDumpDrawFile, "%08d, ", *(GLint*)pData);
                    pData = (GLvoid*)((GLuint)pData + 4);
                    break;

                case GL_HALF_FLOAT_OES:
                case GL_HALF_FLOAT:
                    {
                        U32 float1 = DumpAQFloat16ToFloat(*(U16*)pData);
                        DumpToFile(m_pDumpDrawFile, "%f, ", *(GLfloat*)&float1);
                        pData = (GLvoid*)((GLuint)pData + 2);
                    }
                    break;

                default:
                    Abort("%s(%d): Cannot parse data.\n", __FUNCTION__, __LINE__);
                    break;
                }
            }

            switch (p->type)
            {
            case GL_FLOAT:
                DumpToFile(m_pDumpDrawFile, "%f", *(GLfloat*)pData);
                //DumpToFile(m_pDumpDrawFile, "0x%08X, ", *(GLuint*)pData);
                pData = (GLvoid*)((GLuint)pData + 4);
                break;

            case GL_UNSIGNED_BYTE:
                DumpToFile(m_pDumpDrawFile, "%03d", *(GLubyte*)pData);
                pData = (GLvoid*)((GLuint)pData + 1);
                break;

            case GL_SHORT:
                DumpToFile(m_pDumpDrawFile, "%04d, ", *(GLshort*)pData);
                pData = (GLvoid*)((GLuint)pData + 2);
                break;

            case GL_UNSIGNED_SHORT:
                DumpToFile(m_pDumpDrawFile, "%04d, ", *(GLushort*)pData);
                pData = (GLvoid*)((GLuint)pData + 2);
                break;

            case GL_INT:
                DumpToFile(m_pDumpDrawFile, "%08d, ", *(GLint*)pData);
                pData = (GLvoid*)((GLuint)pData + 4);
                break;

            case GL_HALF_FLOAT_OES:
            case GL_HALF_FLOAT:
                {
                    U32 float1 = DumpAQFloat16ToFloat(*(U16*)pData);
                    DumpToFile(m_pDumpDrawFile, "%f, ", *(GLfloat*)&float1);
                    pData = (GLvoid*)((GLuint)pData + 2);
                }
                break;

            default:
                Abort("%s(%d): Cannot parse data.", __FUNCTION__, __LINE__);
                break;
            }
            DumpToFile(m_pDumpDrawFile, ") ");
        }
        DumpToFile(m_pDumpDrawFile, "\n");
    }
    DumpToFile(m_pDumpDrawFile, "**********************************attribute**********************************\n\n");
}

GLvoid CDumpInfo::DumpDrawStates()
{
    GLint value[4];

    // Write mask
    {
        GLboolean   colorMask[4];
        g_opengl->glGetBooleanv(GL_COLOR_WRITEMASK, colorMask);
        DumpToFile(m_pDumpDrawFile, "****ColorMask: (%d, %d, %d, %d)\n",
            colorMask[0], colorMask[1], colorMask[2], colorMask[3]);
    }

    // View port
    {
        g_opengl->glGetIntegerv(GL_VIEWPORT, value);
        DumpToFile(m_pDumpDrawFile, "****Viewport: (%04d, %04d, %04d, %04d)\n", value[0], value[1], value[2], value[3]);
    }

    // Scissor
    if (glIsEnabled(GL_SCISSOR_TEST))
    {
        g_opengl->glGetIntegerv(GL_SCISSOR_BOX, value);
        DumpToFile(m_pDumpDrawFile, "****Scissor: (%04d, %04d, %04d, %04d)\n", value[0], value[1], value[2], value[3]);
    }

    // Dither
    if (glIsEnabled(GL_DITHER))
    {
        DumpToFile(m_pDumpDrawFile, "****Dither: on\n");
    }

    // Depth test
    if (glIsEnabled(GL_DEPTH_TEST))
    {
        GLfloat     depthRange[2];
        GLint       depthFunc;
        GLboolean   depthMask;

        g_opengl->glGetFloatv(GL_DEPTH_RANGE, depthRange);
        g_opengl->glGetIntegerv(GL_DEPTH_FUNC, &depthFunc);
        g_opengl->glGetBooleanv(GL_DEPTH_WRITEMASK, &depthMask);

        TranslateFunc(depthFunc, dumpTmp1);
        DumpToFile(m_pDumpDrawFile, "****Depth: on %s (%f, %f) WriteMask: %s\n",
            dumpTmp1, depthRange[0], depthRange[1], depthMask?"on":"off");
    }

    // Blend
    if (glIsEnabled(GL_BLEND))
    {
        GLint       srcRGB;
        GLint       srcAlpha;
        GLint       dstRGB;
        GLint       dstAlpha;
        GLint       equationRGB;
        GLint       equationAlpha;

        g_opengl->glGetIntegerv(GL_BLEND_SRC_RGB, &srcRGB);
        g_opengl->glGetIntegerv(GL_BLEND_SRC_ALPHA, &srcAlpha);
        g_opengl->glGetIntegerv(GL_BLEND_DST_RGB, &dstRGB);
        g_opengl->glGetIntegerv(GL_BLEND_DST_ALPHA, &dstAlpha);
        g_opengl->glGetIntegerv(GL_BLEND_EQUATION_RGB, &equationRGB);
        g_opengl->glGetIntegerv(GL_BLEND_EQUATION_ALPHA, &equationAlpha);

        TranslateBlendFunc(srcRGB, dumpTmp1);
        TranslateBlendFunc(srcAlpha, dumpTmp2);
        TranslateBlendFunc(dstRGB, dumpTmp3);
        TranslateBlendFunc(dstAlpha, dumpTmp4);
        TranslateBlendMode(equationRGB, dumpTmp5);
        TranslateBlendMode(equationAlpha, dumpTmp6);

        DumpToFile(m_pDumpDrawFile, "****Blend: on\tR=%1.3f, G=%1.3f, B=%1.3f, A=%1.3f\n",
            m_pContext->blendColorRed,
            m_pContext->blendColorGreen,
            m_pContext->blendColorBlue,
            m_pContext->blendColorAlpha);

        /* func */
        DumpToFile(m_pDumpDrawFile, "****   srcRGB: %s\tsrcAlpha: %s\n", dumpTmp1, dumpTmp2);
        DumpToFile(m_pDumpDrawFile, "****   dstRGB: %s\tdstAlpha: %s\n", dumpTmp3, dumpTmp4);

        /* mode */
        DumpToFile(m_pDumpDrawFile, "****   modeRGB: %smodeAlpha: %s\n", dumpTmp5, dumpTmp6);
    }

    // Cull Face
    if (glIsEnabled(GL_CULL_FACE))
    {
        GLint   cullFaceMode;
        GLint   frontFace;
        const GLchar    *pStrMode;
        const GLchar    *pStrFace;

        g_opengl->glGetIntegerv(GL_CULL_FACE_MODE, &cullFaceMode);
        g_opengl->glGetIntegerv(GL_FRONT_FACE, &frontFace);

        switch (cullFaceMode)
        {
        case GL_FRONT:
            pStrMode = "GL_FRONT         ";
            break;

        case GL_BACK:
            pStrMode = "GL_BACK          ";
            break;

        case GL_FRONT_AND_BACK:
            pStrMode = "GL_FRONT_BACK    ";
            break;

        default:
            pStrMode = "Invalid          ";
            break;
        }

        switch (frontFace)
        {
        case GL_CW:
            pStrFace = "GL_CW    ";
            break;

        case GL_CCW:
            pStrFace = "GL_CCW   ";
            break;

        default:
            pStrFace = "Invalid  ";
            break;
        }

        DumpToFile(m_pDumpDrawFile, "****CullFace: on\t%s\t%s\n", pStrMode, pStrFace);
    }

    // Polygon offset
    if (glIsEnabled(GL_POLYGON_OFFSET_FILL))
    {
        GLfloat offsetFactor;
        GLfloat offsetUnit;

        g_opengl->glGetFloatv(GL_POLYGON_OFFSET_FACTOR, &offsetFactor);
        g_opengl->glGetFloatv(GL_POLYGON_OFFSET_UNITS, &offsetUnit);

        DumpToFile(m_pDumpDrawFile, "****PolygonOffset: on\tfactor: %08.3f, units: %08.3f\n", offsetFactor, offsetUnit);
    }

    // Sample alpha to coverage
    if (glIsEnabled(GL_SAMPLE_ALPHA_TO_COVERAGE))
    {
        DumpToFile(m_pDumpDrawFile, "****SampleAlphaToCoverage: on\n");
    }

    // Sample coverage
    if (glIsEnabled(GL_SAMPLE_COVERAGE))
    {
        GLfloat     value;
        GLboolean   invert;

        g_opengl->glGetFloatv(GL_SAMPLE_COVERAGE_VALUE, &value);
        g_opengl->glGetBooleanv(GL_SAMPLE_COVERAGE_INVERT, &invert);

        DumpToFile(m_pDumpDrawFile, "****SampleCoverage: on, value: %08.3f, invert: %s\n",
            value, invert?"on":"off");
    }

    // Stencil test
    if (glIsEnabled(GL_STENCIL_TEST))
    {
        GLint   frontFunc;
        GLint   frontValueMask;
        GLint   frontRef;
        GLint   frontFail;
        GLint   frontPassDepthFail;
        GLint   frontPassDepthPass;
        GLint   frontWriteMask;
        GLint   backFunc;
        GLint   backValueMask;
        GLint   backRef;
        GLint   backFail;
        GLint   backPassDepthFail;
        GLint   backPassDepthPass;
        GLint   backWriteMask;

        g_opengl->glGetIntegerv(GL_STENCIL_FUNC, &frontFunc);
        g_opengl->glGetIntegerv(GL_STENCIL_VALUE_MASK, &frontValueMask);
        g_opengl->glGetIntegerv(GL_STENCIL_REF, &frontRef);
        g_opengl->glGetIntegerv(GL_STENCIL_FAIL, &frontFail);
        g_opengl->glGetIntegerv(GL_STENCIL_PASS_DEPTH_FAIL, &frontPassDepthFail);
        g_opengl->glGetIntegerv(GL_STENCIL_PASS_DEPTH_PASS, &frontPassDepthPass);
        g_opengl->glGetIntegerv(GL_STENCIL_WRITEMASK, &frontWriteMask);

        g_opengl->glGetIntegerv(GL_STENCIL_BACK_FUNC, &backFunc);
        g_opengl->glGetIntegerv(GL_STENCIL_BACK_VALUE_MASK, &backValueMask);
        g_opengl->glGetIntegerv(GL_STENCIL_BACK_REF, &backRef);
        g_opengl->glGetIntegerv(GL_STENCIL_BACK_FAIL, &backFail);
        g_opengl->glGetIntegerv(GL_STENCIL_BACK_PASS_DEPTH_FAIL, &backPassDepthFail);
        g_opengl->glGetIntegerv(GL_STENCIL_BACK_PASS_DEPTH_PASS, &backPassDepthPass);
        g_opengl->glGetIntegerv(GL_STENCIL_BACK_WRITEMASK, &backWriteMask);

        DumpToFile(m_pDumpDrawFile, "****Stencil: on\n");

        TranslateFunc(frontFunc, dumpTmp1);
        DumpToFile(m_pDumpDrawFile, "****   Front: %s, Mask: 0x%08X, WriteMask: 0x%08X, Ref: 0x%08X\n",
            dumpTmp1, frontValueMask, frontWriteMask, frontRef);
        TranslateFunc(backFunc, dumpTmp1);
        DumpToFile(m_pDumpDrawFile, "****   Back : %s, Mask: 0x%08X, WriteMask: 0x%08X, Ref: 0x%08X\n",
            dumpTmp1, backValueMask, backWriteMask, backRef);

        TranslateOperation(frontFail, dumpTmp1);
        TranslateOperation(frontPassDepthFail, dumpTmp2);
        TranslateOperation(frontPassDepthPass, dumpTmp3);
        DumpToFile(m_pDumpDrawFile, "****   Front: OpFail: %s, OpDepthFail: %s, OpDepthPass: %s\n", dumpTmp1, dumpTmp2, dumpTmp3);

        TranslateOperation(backFail, dumpTmp1);
        TranslateOperation(backPassDepthFail, dumpTmp2);
        TranslateOperation(backPassDepthPass, dumpTmp3);
        DumpToFile(m_pDumpDrawFile, "****   Back : OpFail: %s, OpDepthFail: %s, OpDepthPass: %s\n", dumpTmp1, dumpTmp2, dumpTmp3);
    }
}

GLvoid CDumpInfo::DumpColorBuffer(GLchar *fileName)
{
    GLint   viewport[4];
    GLubyte *pData  = NULL;
    GLint   width   = 0;
    GLint   height  = 0;

    g_opengl->glGetIntegerv(GL_VIEWPORT, viewport);
    width   = viewport[2];
    height  = viewport[3];

    pData = (GLubyte*)malloc(width*height*4);
    if (pData == NULL)
    {
        printf("%s(%d): cannot allocate memory.\n", __FUNCTION__, __LINE__);
        return;
    }

    g_opengl->glReadPixels(0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, pData);
    DumpSavePicture(fileName, width, height, pData, m_bDumpPng);
    free(pData);
}

GLvoid CDumpInfo::WriteUniformValue(GLint loc, GLenum type, GLuint size)
{
    GLuint      typeSize    = 0;
    GLuint      total       = 0;
    CProgramObj *pProg      = FindProgramObject(m_nOrigProgram);
    GLvoid      *pData      = pProg->GetUniformValueByLoc(loc);

    typeSize    = DumpGetShaderDataSize(type);
    total       = typeSize * size;

    DumpWriteBytes(m_pDrawBinFile, pData, total);
}

GLvoid CDumpInfo::WriteTextureParameter(GLenum target, GLuint unit)
{
    GLint value = 0;

    //DumpWriteInt(m_pDrawBinFile, Dump_TexParameter);

    g_opengl->glActiveTexture(unit+GL_TEXTURE0);

    g_opengl->glGetTexParameteriv(target, GL_TEXTURE_WRAP_S, &value);
    //DumpWriteInt(m_pDrawBinFile, GL_TEXTURE_WRAP_S);
    DumpWriteInt(m_pDrawBinFile, value);

    g_opengl->glGetTexParameteriv(target, GL_TEXTURE_WRAP_T, &value);
    //DumpWriteInt(m_pDrawBinFile, GL_TEXTURE_WRAP_T);
    DumpWriteInt(m_pDrawBinFile, value);

    g_opengl->glGetTexParameteriv(target, GL_TEXTURE_MIN_FILTER, &value);
    //DumpWriteInt(m_pDrawBinFile, GL_TEXTURE_MIN_FILTER);
    DumpWriteInt(m_pDrawBinFile, value);

    g_opengl->glGetTexParameteriv(target, GL_TEXTURE_MAG_FILTER, &value);
    //DumpWriteInt(m_pDrawBinFile, GL_TEXTURE_MAG_FILTER);
    DumpWriteInt(m_pDrawBinFile, value);

    g_opengl->glGetTexParameteriv(target, GL_TEXTURE_MAX_ANISOTROPY_EXT, &value);
    //DumpWriteInt(m_pDrawBinFile, GL_TEXTURE_MAX_ANISOTROPY_EXT);
    DumpWriteInt(m_pDrawBinFile, value);

    // Restore current texture unit
    g_opengl->glActiveTexture(m_pContext->activeTexUnit);
}

GLvoid CDumpInfo::WriteTextureImage(GLuint unit, stTexImage *pTexImage)
{
    GLuint pixelSize    = DumpGetPixelSize(pTexImage->format, pTexImage->type);
    GLuint size         = pixelSize * pTexImage->width * pTexImage->height;

    DumpWriteInt(m_pDrawBinFile, Dump_TexImage2D);
    DumpWriteInt(m_pDrawBinFile, pTexImage->target);
    DumpWriteInt(m_pDrawBinFile, pTexImage->level);
    DumpWriteInt(m_pDrawBinFile, pTexImage->format);
    DumpWriteInt(m_pDrawBinFile, pTexImage->width);
    DumpWriteInt(m_pDrawBinFile, pTexImage->height);
    DumpWriteInt(m_pDrawBinFile, 0);
    DumpWriteInt(m_pDrawBinFile, pTexImage->format);
    DumpWriteInt(m_pDrawBinFile, pTexImage->type);
    DumpWriteInt(m_pDrawBinFile, pTexImage->imageSize);
    if (pTexImage->format == GL_ETC1_RGB8_OES)
    {
        DumpWriteBytes(m_pDrawBinFile, pTexImage->pixels, pTexImage->imageSize);
    }
    else
    {
        DumpWriteBytes(m_pDrawBinFile, pTexImage->pixels, size);
    }
    DumpWriteInt(m_pDrawBinFile, pTexImage->nUnpackAlignment);
}

GLvoid CDumpInfo::WriteTexture(GLint loc, GLenum type, GLuint size)
{
    GLint       *pValue = (GLint*)malloc(size*4);
    GLuint      name    = 0;
    CTexObj     *pTex   = NULL;
    stTexImage  *pImg   = NULL;

    g_opengl->glGetUniformiv(m_nCurrentProgram, loc, pValue);

    DumpWriteInt(m_pDrawBinFile, Dump_Texture);
    DumpWriteInt(m_pDrawBinFile, size);
    if (type == GL_SAMPLER_2D)
    {
        for (GLuint j=0; j<size; j++)
        {
            GLenum unit = pValue[j];
            pTex = m_pContext->GetTexObjByUnit(unit+GL_TEXTURE0, GL_TEXTURE_2D);
            pImg = pTex->pTex2D;

            //DumpWriteInt(m_pDrawBinFile, Dump_ActiveTexture);
            DumpWriteInt(m_pDrawBinFile, unit);

            //DumpWriteInt(m_pDrawBinFile, Dump_BindTexture);
            DumpWriteInt(m_pDrawBinFile, pTex->target);
            DumpWriteInt(m_pDrawBinFile, name);

            WriteTextureParameter(GL_TEXTURE_2D, pValue[j]);

            DumpWriteInt(m_pDrawBinFile, pTex->GetImageCount(GL_TEXTURE_2D));
            while (pImg)
            {
                WriteTextureImage(pValue[j], pImg);
                pImg = pImg->next;
            }
        }
    }
    else if (type == GL_SAMPLER_CUBE)
    {
        for (GLuint j=0; j<size; j++)
        {
            GLenum unit = pValue[j];
            pTex = m_pContext->GetTexObjByUnit(unit+GL_TEXTURE0, GL_TEXTURE_2D);

            //DumpWriteInt(m_pDrawBinFile, Dump_ActiveTexture);
            DumpWriteInt(m_pDrawBinFile, unit);

            //DumpWriteInt(m_pDrawBinFile, Dump_BindTexture);
            DumpWriteInt(m_pDrawBinFile, pTex->target);
            DumpWriteInt(m_pDrawBinFile, name);

            WriteTextureParameter(GL_TEXTURE_CUBE_MAP, unit);
            for (GLuint k=0; k<6; k++)
            {
                DumpWriteInt(m_pDrawBinFile, pTex->GetImageCount(k+GL_TEXTURE_CUBE_MAP_POSITIVE_X));
                pImg = pTex->pTexCube[k];
                while (pImg)
                {
                    WriteTextureImage(pValue[j], pImg);
                    pImg = pImg->next;
                }
            }
        }
    }

    free(pValue);
}

GLvoid CDumpInfo::WriteShader(GLESAPIIndex func)
{
    GLuint  shader[2];
    GLsizei count               = 0;
    GLint   activeAttributes    = 0;
    GLint   activeUniforms      = 0;
    GLint   maxAttributeLen     = 0;
    GLint   maxUniformLen       = 0;
    GLchar  *pAttributeName     = NULL;
    GLchar  *pUniformName       = NULL;

    if (!glIsProgram(m_nCurrentProgram))
    {
        printf("ID(%d) is not program.\n", m_nCurrentProgram);
        return;
    }

    g_opengl->glGetError();
    g_opengl->glGetAttachedShaders(m_nCurrentProgram, 2, &count, shader);

    // Write shader
    DumpWriteInt(m_pDrawBinFile, Dump_ShaderSource);
    for (GLsizei i=0; i<count; i++)
    {
        GLenum  type;
        GLsizei shaderLen   = 0;
        GLchar  *pSrc       = NULL;
        GLsizei size        = 0;

        if (!glIsShader(shader[i]))
        {
            printf("ID(%d): is not shader.\n", shader[i]);
            continue;
        }

        g_opengl->glGetShaderiv(shader[i], GL_SHADER_TYPE, (GLint*)&type);
        g_opengl->glGetShaderiv(shader[i], GL_SHADER_SOURCE_LENGTH, &shaderLen);
        pSrc = (GLchar*)malloc(shaderLen + 1);
        if (pSrc == NULL)
        {
            printf("%s(%d): cannot allocate for shader source", __FUNCTION__, __LINE__);
            continue;
        }
        memset(pSrc, 0, shaderLen+1);
        g_opengl->glGetShaderSource(shader[i], shaderLen, &size, pSrc);
        DumpWriteInt(m_pDrawBinFile, type);
        if (size > 0)
        {
            DumpWriteBytes(m_pDrawBinFile, pSrc, shaderLen+1);
        }
        else
        {
            CProgramObj *p = FindProgramObject(m_nOrigProgram);

            if (type == GL_VERTEX_SHADER)
            {
                DumpWriteBytes(m_pDrawBinFile, p->m_pVertex->m_source, p->m_pVertex->m_len);
            }
            else if (type == GL_FRAGMENT_SHADER)
            {
                DumpWriteBytes(m_pDrawBinFile, p->m_pFragment->m_source, p->m_pFragment->m_len);
            }
        }

        free(pSrc);
    }

    g_opengl->glGetProgramiv(m_nCurrentProgram, GL_ACTIVE_ATTRIBUTES, &activeAttributes);
    g_opengl->glGetProgramiv(m_nCurrentProgram, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &maxAttributeLen);
    g_opengl->glGetProgramiv(m_nCurrentProgram, GL_ACTIVE_UNIFORMS, &activeUniforms);
    g_opengl->glGetProgramiv(m_nCurrentProgram, GL_ACTIVE_UNIFORM_MAX_LENGTH, &maxUniformLen);
    pAttributeName  = (GLchar*)malloc(maxAttributeLen);
    pUniformName    = (GLchar*)malloc(maxUniformLen);

    DumpWriteInt(m_pDrawBinFile, Dump_BindAttribLocation);
    DumpWriteInt(m_pDrawBinFile, activeAttributes);
    for (GLint i=0; i<activeAttributes; i++)
    {
        GLsizei len         = 0;
        GLint   size        = 0;
        GLenum  type        = 0;
        GLint   loc         = -1;

        memset(pAttributeName, 0, maxAttributeLen);
        g_opengl->glGetActiveAttrib(m_nCurrentProgram, i, maxAttributeLen, &len, &size, &type, pAttributeName);
        loc = g_opengl->glGetAttribLocation(m_nCurrentProgram, pAttributeName);

        // Write binary
        DumpWriteInt(m_pDrawBinFile, loc);
        DumpWriteBytes(m_pDrawBinFile, pAttributeName, maxAttributeLen);
    }

    DumpWriteInt(m_pDrawBinFile, Dump_Uniform);
    DumpWriteInt(m_pDrawBinFile, activeUniforms);
    for (GLint i=0; i<activeUniforms; i++)
    {
        GLsizei len     = 0;
        GLint   size    = 0;
        GLenum  type    = 0;
        GLint   loc     = -1;

        memset(pUniformName, 0, maxUniformLen);
        g_opengl->glGetActiveUniform(m_nCurrentProgram, i, maxUniformLen, &len, &size, &type, pUniformName);
        loc = g_opengl->glGetUniformLocation(m_nCurrentProgram, pUniformName);

        DumpWriteInt(m_pDrawBinFile, size);
        DumpWriteInt(m_pDrawBinFile, type);
        DumpWriteBytes(m_pDrawBinFile, pUniformName, maxUniformLen);
        WriteUniformValue(loc, type, size);

        if (type == GL_SAMPLER_2D ||
            type == GL_SAMPLER_CUBE)
        {
            WriteTexture(loc, type, size);
        }
    }

    free(pAttributeName);
    free(pUniformName);
}

GLvoid CDumpInfo::WriteVertexData(GLESAPIIndex func)
{
    stVertAttrPtr   *p          = NULL;
    GLvoid          *pSrc       = NULL;
    GLuint          arraySize   = 0;
    GLuint          size        = 0;
    CAttribObj      *pCurrent   = NULL;
    GLuint          stride      = 0;

    for (GLuint j=0; j<API_VERTEX_ATTRIBUTES_NUM; j++)
    {
        if ((VERTEX_ATTRIB_BITS & (0x1 << j)) == 0)
            continue;

        p = VERTEX_ATTRIB_PTR(j);
        if (p->size == 0)
            continue;

        pCurrent = FindAttribObjByIndex(j);
        if (pCurrent == NULL)
            continue;

        stride = p->stride;
        if (stride == 0)
        {
            GLuint dataSize = GetDataTypeSize(p->type);
            stride = dataSize * p->size;
        }

        p->realIndex = pCurrent->nLoc;

        if (p->bUseVBO)
        {
            CBufObj *pBuffer = GET_VBO(p->vbo);
            pSrc = (GLvoid*)((GLuint)pBuffer->GetCurrentDataPointer() + p->pointer);
            size = pBuffer->GetSize();
        }
        else
        {
            GLvoid *pData = (GLvoid*)(p->pointer);
            pSrc = (GLvoid*)((GLuint)pData + p->offset);

            if (func == GL3_API_glDrawArrays)
            {
                size = stride * m_pContext->drawCount;
            }
            else if (func == GL3_API_glDrawElements)
            {
                size = stride * m_pContext->drawVertexCount;
            }
            else
            {
                Abort("%s(%d): todo.\n", __FUNCTION__, __LINE__);
            }
        }

        arraySize = size - p->offset;

        DumpWriteInt(m_pDrawBinFile, Dump_VertexAttribPointer);
        DumpWriteInt(m_pDrawBinFile, p->realIndex);
        DumpWriteInt(m_pDrawBinFile, p->size);
        DumpWriteInt(m_pDrawBinFile, p->type);
        DumpWriteInt(m_pDrawBinFile, p->normalized);
        DumpWriteInt(m_pDrawBinFile, p->stride);
        DumpWriteBytes(m_pDrawBinFile, pSrc, arraySize);
    }
}

GLvoid CDumpInfo::WriteDrawStates()
{
    GLint value[4];

    // Write mask
    {
        GLboolean   colorMask[4];
        g_opengl->glGetBooleanv(GL_COLOR_WRITEMASK, colorMask);
        DumpWriteInt(m_pDrawBinFile, Dump_ColorMask);
        DumpWriteInt(m_pDrawBinFile, colorMask[0]);
        DumpWriteInt(m_pDrawBinFile, colorMask[1]);
        DumpWriteInt(m_pDrawBinFile, colorMask[2]);
        DumpWriteInt(m_pDrawBinFile, colorMask[3]);
    }

    // View port
    {
        g_opengl->glGetIntegerv(GL_VIEWPORT, value);
        DumpWriteInt(m_pDrawBinFile, Dump_Viewport);
        DumpWriteInt(m_pDrawBinFile, value[0]);
        DumpWriteInt(m_pDrawBinFile, value[1]);
        DumpWriteInt(m_pDrawBinFile, value[2]);
        DumpWriteInt(m_pDrawBinFile, value[3]);
    }

    // Scissor
    if (glIsEnabled(GL_SCISSOR_TEST))
    {
        DumpWriteInt(m_pDrawBinFile, Dump_Enable);
        DumpWriteInt(m_pDrawBinFile, GL_SCISSOR_TEST);

        g_opengl->glGetIntegerv(GL_SCISSOR_BOX, value);
        DumpWriteInt(m_pDrawBinFile, value[0]);
        DumpWriteInt(m_pDrawBinFile, value[1]);
        DumpWriteInt(m_pDrawBinFile, value[2]);
        DumpWriteInt(m_pDrawBinFile, value[3]);
    }

    // Dither
    if (glIsEnabled(GL_DITHER))
    {
        DumpWriteInt(m_pDrawBinFile, Dump_Enable);
        DumpWriteInt(m_pDrawBinFile, GL_DITHER);
    }

    // Depth test
    if (glIsEnabled(GL_DEPTH_TEST))
    {
        GLfloat     depthRange[2];
        GLint       depthFunc;
        GLboolean   depthMask;

        g_opengl->glGetFloatv(GL_DEPTH_RANGE, depthRange);
        g_opengl->glGetIntegerv(GL_DEPTH_FUNC, &depthFunc);
        g_opengl->glGetBooleanv(GL_DEPTH_WRITEMASK, &depthMask);

        DumpWriteInt(m_pDrawBinFile, Dump_Enable);
        DumpWriteInt(m_pDrawBinFile, GL_DEPTH_TEST);
        DumpWriteInt(m_pDrawBinFile, depthMask);
        DumpWriteInt(m_pDrawBinFile, depthFunc);
        DumpWriteFloat(m_pDrawBinFile, depthRange[0]);
        DumpWriteFloat(m_pDrawBinFile, depthRange[1]);
    }

    // Blend
    if (glIsEnabled(GL_BLEND))
    {
        GLint   srcRGB;
        GLint   srcAlpha;
        GLint   dstRGB;
        GLint   dstAlpha;
        GLint   equationRGB;
        GLint   equationAlpha;

        g_opengl->glGetIntegerv(GL_BLEND_SRC_RGB, &srcRGB);
        g_opengl->glGetIntegerv(GL_BLEND_SRC_ALPHA, &srcAlpha);
        g_opengl->glGetIntegerv(GL_BLEND_DST_RGB, &dstRGB);
        g_opengl->glGetIntegerv(GL_BLEND_DST_ALPHA, &dstAlpha);
        g_opengl->glGetIntegerv(GL_BLEND_EQUATION_RGB, &equationRGB);
        g_opengl->glGetIntegerv(GL_BLEND_EQUATION_ALPHA, &equationAlpha);

        DumpWriteInt(m_pDrawBinFile, Dump_Enable);
        DumpWriteInt(m_pDrawBinFile, GL_BLEND);
        DumpWriteInt(m_pDrawBinFile, srcRGB);
        DumpWriteInt(m_pDrawBinFile, srcAlpha);
        DumpWriteInt(m_pDrawBinFile, dstRGB);
        DumpWriteInt(m_pDrawBinFile, dstAlpha);
        DumpWriteInt(m_pDrawBinFile, equationRGB);
        DumpWriteInt(m_pDrawBinFile, equationAlpha);
        DumpWriteFloat(m_pDrawBinFile, m_pContext->blendColorRed);
        DumpWriteFloat(m_pDrawBinFile, m_pContext->blendColorGreen);
        DumpWriteFloat(m_pDrawBinFile, m_pContext->blendColorBlue);
        DumpWriteFloat(m_pDrawBinFile, m_pContext->blendColorAlpha);
    }

    // Cull Face
    if (glIsEnabled(GL_CULL_FACE))
    {
        GLint   cullFaceMode;
        GLint   frontFace;

        g_opengl->glGetIntegerv(GL_CULL_FACE_MODE, &cullFaceMode);
        g_opengl->glGetIntegerv(GL_FRONT_FACE, &frontFace);

        DumpWriteInt(m_pDrawBinFile, Dump_Enable);
        DumpWriteInt(m_pDrawBinFile, GL_CULL_FACE);
        DumpWriteInt(m_pDrawBinFile, cullFaceMode);
        DumpWriteInt(m_pDrawBinFile, frontFace);
    }

    // Polygon offset
    if (glIsEnabled(GL_POLYGON_OFFSET_FILL))
    {
        GLfloat offsetFactor;
        GLfloat offsetUnit;

        g_opengl->glGetFloatv(GL_POLYGON_OFFSET_FACTOR, &offsetFactor);
        g_opengl->glGetFloatv(GL_POLYGON_OFFSET_UNITS, &offsetUnit);

        DumpWriteInt(m_pDrawBinFile, Dump_Enable);
        DumpWriteInt(m_pDrawBinFile, GL_POLYGON_OFFSET_FILL);
        DumpWriteFloat(m_pDrawBinFile, offsetFactor);
        DumpWriteFloat(m_pDrawBinFile, offsetUnit);
    }

    // Sample alpha to coverage
    if (glIsEnabled(GL_SAMPLE_ALPHA_TO_COVERAGE))
    {
        DumpWriteInt(m_pDrawBinFile, Dump_Enable);
        DumpWriteInt(m_pDrawBinFile, GL_SAMPLE_ALPHA_TO_COVERAGE);
    }

    // Sample coverage
    if (glIsEnabled(GL_SAMPLE_COVERAGE))
    {
        GLfloat     value;
        GLboolean   invert;

        g_opengl->glGetFloatv(GL_SAMPLE_COVERAGE_VALUE, &value);
        g_opengl->glGetBooleanv(GL_SAMPLE_COVERAGE_INVERT, &invert);

        DumpWriteInt(m_pDrawBinFile, Dump_Enable);
        DumpWriteInt(m_pDrawBinFile, GL_SAMPLE_COVERAGE);
        DumpWriteFloat(m_pDrawBinFile, value);
        DumpWriteInt(m_pDrawBinFile, invert);
    }

    // Stencil test
    if (glIsEnabled(GL_STENCIL_TEST))
    {
        GLint   frontFunc;
        GLint   frontValueMask;
        GLint   frontRef;
        GLint   frontFail;
        GLint   frontPassDepthFail;
        GLint   frontPassDepthPass;
        GLint   frontWriteMask;
        GLint   backFunc;
        GLint   backValueMask;
        GLint   backRef;
        GLint   backFail;
        GLint   backPassDepthFail;
        GLint   backPassDepthPass;
        GLint   backWriteMask;

        g_opengl->glGetIntegerv(GL_STENCIL_FUNC, &frontFunc);
        g_opengl->glGetIntegerv(GL_STENCIL_VALUE_MASK, &frontValueMask);
        g_opengl->glGetIntegerv(GL_STENCIL_REF, &frontRef);
        g_opengl->glGetIntegerv(GL_STENCIL_FAIL, &frontFail);
        g_opengl->glGetIntegerv(GL_STENCIL_PASS_DEPTH_FAIL, &frontPassDepthFail);
        g_opengl->glGetIntegerv(GL_STENCIL_PASS_DEPTH_PASS, &frontPassDepthPass);
        g_opengl->glGetIntegerv(GL_STENCIL_WRITEMASK, &frontWriteMask);

        g_opengl->glGetIntegerv(GL_STENCIL_BACK_FUNC, &backFunc);
        g_opengl->glGetIntegerv(GL_STENCIL_BACK_VALUE_MASK, &backValueMask);
        g_opengl->glGetIntegerv(GL_STENCIL_BACK_REF, &backRef);
        g_opengl->glGetIntegerv(GL_STENCIL_BACK_FAIL, &backFail);
        g_opengl->glGetIntegerv(GL_STENCIL_BACK_PASS_DEPTH_FAIL, &backPassDepthFail);
        g_opengl->glGetIntegerv(GL_STENCIL_BACK_PASS_DEPTH_PASS, &backPassDepthPass);
        g_opengl->glGetIntegerv(GL_STENCIL_BACK_WRITEMASK, &backWriteMask);

        DumpWriteInt(m_pDrawBinFile, Dump_Enable);
        DumpWriteInt(m_pDrawBinFile, GL_STENCIL_TEST);
        DumpWriteInt(m_pDrawBinFile, frontFunc);
        DumpWriteInt(m_pDrawBinFile, frontValueMask);
        DumpWriteInt(m_pDrawBinFile, frontRef);
        DumpWriteInt(m_pDrawBinFile, frontFail);
        DumpWriteInt(m_pDrawBinFile, frontPassDepthFail);
        DumpWriteInt(m_pDrawBinFile, frontPassDepthPass);
        DumpWriteInt(m_pDrawBinFile, frontWriteMask);
        DumpWriteInt(m_pDrawBinFile, backFunc);
        DumpWriteInt(m_pDrawBinFile, backValueMask);
        DumpWriteInt(m_pDrawBinFile, backRef);
        DumpWriteInt(m_pDrawBinFile, backFail);
        DumpWriteInt(m_pDrawBinFile, backPassDepthFail);
        DumpWriteInt(m_pDrawBinFile, backPassDepthPass);
        DumpWriteInt(m_pDrawBinFile, backWriteMask);
    }
}

GLvoid CDumpInfo::WriteDrawElements(GLESAPIIndex func)
{
    GLenum  mode        = m_pContext->drawMode;
    GLsizei indexCount  = m_pContext->drawCount;
    GLenum  indexType   = m_pContext->drawIndicesType;
    GLvoid* indices     = m_pContext->drawIndices;
    GLuint  indexOffset = 0;
    GLuint  indexSize   = DumpGetDataSize(indexType) * indexCount;
    GLchar  *pIndices   = NULL;

    if (ELEMENT_ARRAY_BUFFER)
    {
        CBufObj *pIndex = GET_VBO(ELEMENT_ARRAY_BUFFER);
        pIndices = (GLchar*)pIndex->GetCurrentDataPointer();
        indexOffset = (GLuint)indices;
    }
    else
    {
        pIndices = (GLchar*)indices;
    }

    DumpWriteInt(m_pDrawBinFile, Dump_DrawElements);
    DumpWriteInt(m_pDrawBinFile, mode);
    DumpWriteInt(m_pDrawBinFile, indexCount);
    DumpWriteInt(m_pDrawBinFile, indexType);
    DumpWriteBytes(m_pDrawBinFile, (GLvoid*)((GLuint)pIndices+indexOffset), indexSize);
}

GLvoid CDumpInfo::WriteDrawArrays(GLESAPIIndex func)
{
    GLenum      mode    = m_pContext->drawMode;
    GLint       first   = m_pContext->drawFirst;
    GLsizei     count   = m_pContext->drawCount;

    DumpWriteInt(m_pDrawBinFile, Dump_DrawArrays);
    DumpWriteInt(m_pDrawBinFile, mode);
    DumpWriteInt(m_pDrawBinFile, first);
    DumpWriteInt(m_pDrawBinFile, count);
}

GLvoid CDumpInfo::WriteClear()
{
    DumpWriteInt(m_pDrawBinFile, Dump_Clear);
    DumpWriteInt(m_pDrawBinFile, m_clearBits);
    m_clearBits = 0;
    DumpWriteFloat(m_pDrawBinFile, m_clearColorR);
    DumpWriteFloat(m_pDrawBinFile, m_clearColorG);
    DumpWriteFloat(m_pDrawBinFile, m_clearColorB);
    DumpWriteFloat(m_pDrawBinFile, m_clearColorA);
    DumpWriteFloat(m_pDrawBinFile, m_clearDepth);
    DumpWriteInt(m_pDrawBinFile, m_clearStencil);
}

GLvoid CDumpInfo::CompressFile(const GLchar *fileName)
{
    FILE    *pFile      = fopen(fileName, "rb");
    GLuint  fileSize    = 0;
    GLchar  *pSrc       = NULL;
    GLchar  *pDst       = NULL;
    GLuint  readBytes   = 0;
    GLuint  dstLen      = 0;
    GLint   result      = Z_OK;

    do
    {
        if (pFile == NULL)
        {
            printf("%s(%d): cannot open file(%s).\n", __FUNCTION__, __LINE__, fileName);
            break;
        }

        fseek(pFile, 0, SEEK_END);
        fileSize = ftell(pFile);
        fseek(pFile, 0, SEEK_SET);

        pSrc = (GLchar*)malloc(fileSize);
        pDst = (GLchar*)malloc(fileSize);
        if (pSrc == NULL || pDst == NULL)
        {
            printf("%s(%d): cannot allocate memory.\n", __FUNCTION__, __LINE__);
            break;
        }

        readBytes = fread(pSrc, 1, fileSize, pFile);
        if (readBytes != fileSize)
        {
            printf("%s(%d): cannot read file(%s).\n", __FUNCTION__, __LINE__, fileName);
            break;
        }

        dstLen = fileSize;
        result = compress2((Bytef*)pDst, (uLong*)&dstLen, (Bytef*)pSrc, (uLong)fileSize, 2);
        if (result != Z_OK)
        {
            printf("%s(%d): cannot compress with error(%d).\n", __FUNCTION__, __LINE__, result);
            break;
        }

        fclose(pFile);
        pFile = fopen(fileName, "wb");
        if (pFile == NULL)
        {
            printf("%s(%d): cannot open file(%s).\n", __FUNCTION__, __LINE__, fileName);
            break;
        }

        readBytes = fwrite((GLchar*)&fileSize, 1, sizeof(fileSize), pFile);
        if (readBytes != sizeof(fileSize))
        {
            printf("%s(%d): cannot write file(%s).\n", __FUNCTION__, __LINE__, fileName);
            break;
        }

        readBytes = fwrite(pDst, 1, dstLen, pFile);
        if (readBytes != dstLen)
        {
            printf("%s(%d): cannot write file(%s).\n", __FUNCTION__, __LINE__, fileName);
            break;
        }
    }
    while (GL_FALSE);

    if (pSrc)
    {
        free(pSrc);
    }

    if (pDst)
    {
        free(pDst);
    }

    if (pFile)
    {
        fclose(pFile);
    }
}

GLvoid CDumpInfo::WriteDraw(GLESAPIIndex func)
{
    memset(m_strDrawBinary, 0, 256);
    if (func == GL3_API_glDrawElements)
    {
        sprintf(m_strDrawBinary, "%sdraw/glDrawElements%d.bin", m_strPath, m_nDrawTotal);
    }
    else if (func == GL3_API_glDrawArrays)
    {
        sprintf(m_strDrawBinary, "%sdraw/glDrawArrays%d.bin", m_strPath, m_nDrawTotal);
    }

    m_pDrawBinFile = fopen(m_strDrawBinary, "wb");
    if (m_pDrawBinFile == NULL)
    {
        printf("%s(%d): cannot open file(%s).", __FUNCTION__, __LINE__, m_strDrawBinary);
        return;
    }

    DumpWriteBytes(m_pDrawBinFile, (GLvoid*)DRAW_BIN_HEADER, strlen(DRAW_BIN_HEADER));

    WriteClear();
    WriteDrawStates();
    WriteShader(func);
    WriteVertexData(func);

    if (func == GL3_API_glDrawElements)
    {
        WriteDrawElements(func);
    }
    else if (func == GL3_API_glDrawArrays)
    {
        WriteDrawArrays(func);
    }

    DumpFlushFile(m_pDrawBinFile);
    fclose(m_pDrawBinFile);
    CompressFile(m_strDrawBinary);
}

GLvoid CDumpInfo::SaveIndexFile(GLESAPIIndex func)
{
    GLchar  fileName[256];
    GLchar  indexFileName[256];
    FILE    *pFile      = NULL;
    GLsizei indexCount  = m_pContext->drawCount;
    GLenum  indexType   = m_pContext->drawIndicesType;
    GLvoid  *indices    = m_pContext->drawIndices;
    GLuint  indexOffset = 0;
    GLchar  *pIndices   = NULL;

    if (ELEMENT_ARRAY_BUFFER)
    {
        CBufObj *pIndex = GET_VBO(ELEMENT_ARRAY_BUFFER);
        pIndices = (GLchar*)pIndex->GetCurrentDataPointer();
        indexOffset = (GLuint)indices;
    }
    else
    {
        pIndices = (GLchar*)indices;
    }

    memset(fileName, 0, 256);
    sprintf(fileName, "frame%d_glDrawElements%d", m_nCurrentFrame, m_nDrawTotal);

    memset(indexFileName, 0, 256);
    sprintf(indexFileName, "%sdraw/frame%d/%s_index.dat", fileName, m_nCurrentFrame, m_strPath);
    pFile = fopen(indexFileName, "wb");
    if (pFile)
    {
        GLuint size = DumpGetDataSize(indexType)*indexCount;
        GLvoid *p   = (GLvoid*)((GLuint)pIndices + indexOffset);
        fwrite(p, 1, size, pFile);;
        fclose(pFile);
    }
}

GLvoid CDumpInfo::DumpDrawElements(GLESAPIIndex func)
{
    CFrameInfo  *pFrame     = FindFrameInfo(m_nCurrentFrame);
    GLenum      mode        = m_pContext->drawMode;
    GLsizei     indexCount  = m_pContext->drawCount;
    GLenum      indexType   = m_pContext->drawIndicesType;
    GLvoid*     indices     = m_pContext->drawIndices;
    GLuint      indexOffset = 0;
    GLchar      *pIndices   = NULL;
    GLuint      vertexMin   = 0xFFFFFFFF;
    GLuint      vertexMax   = 0;
    GLuint      index       = 0;
    GLvoid      *pIndexData = NULL;
    GLuint      fmtCount    = 0;
    GLuint      vertexCount = 0;
    GLboolean   bNeedDump   = IsDrawNeedDump();

    if (!bNeedDump)
    {
        return;
    }

    pFrame->m_nDrawCount ++;
    pFrame->m_nPrimitiveCount += DumpGetPrimitiveCountPerMode(mode, indexCount);

    if (ELEMENT_ARRAY_BUFFER)
    {
        CBufObj *pIndex = GET_VBO(ELEMENT_ARRAY_BUFFER);
        pIndices = (GLchar*)pIndex->GetCurrentDataPointer();
        indexOffset = (GLuint)indices;
    }
    else
    {
        pIndices = (GLchar*)indices;
    }

    DumpToFile(m_pDumpDrawFile, "////////////////////////////////////////////////////////////////////////////////////////////////////////\n");
    DumpToFile(m_pDumpDrawFile, "//                                                draw                                                //\n");
    DumpToFile(m_pDumpDrawFile, "////////////////////////////////////////////////////////////////////////////////////////////////////////\n");
    TranslateDrawMode(mode, dumpTmp1);
    TranslateDataType(indexType, dumpTmp2);
    DumpToFile(m_pDumpDrawFile, "#%d glDrawElements(%s, indexCount=%d, %s, indices=0x%08X) prog=%08d\n\n",
        m_nDrawTotal, dumpTmp1, indexCount, dumpTmp2, indices, m_nOrigProgram);
    DumpDrawStates();

    DumpToFile(m_pDumpDrawFile, "*************************index*************************\n");

    // Save Index file
#if API_WRITE_INDEX_BUFFER
    SaveIndexFile(func);
#endif

    pIndexData = (GLvoid*)((GLuint)pIndices + indexOffset);
    for (GLint i=0; i<indexCount; i++)
    {
        switch (indexType)
        {
        case GL_UNSIGNED_BYTE:
            index = *(GLubyte*)pIndexData;
            pIndexData = (GLvoid*)((GLuint)pIndexData + 1);
            if (bNeedDump)
            {
                DumpToFile(m_pDumpDrawFile, "%02d ", index);
            }
            break;

        case GL_UNSIGNED_SHORT:
            index = *(GLushort*)pIndexData;
            pIndexData = (GLvoid*)((GLuint)pIndexData + 2);
            if (bNeedDump)
            {
                DumpToFile(m_pDumpDrawFile, "%04d ", index);
            }
            break;

        case GL_UNSIGNED_INT:
            index = *(GLuint*)pIndexData;
            pIndexData = (GLvoid*)((GLuint)pIndexData + 4);
            if (bNeedDump)
            {
                DumpToFile(m_pDumpDrawFile, "%08d ", index);
            }
            break;

        default:
            break;
        }

        fmtCount ++;
        if (bNeedDump && ((fmtCount % 8) == 0))
        {
            DumpToFile(m_pDumpDrawFile, "\n");
        }

        if (index > vertexMax)
        {
            vertexMax = index;
        }

        if (index < vertexMin)
        {
            vertexMin = index;
        }
    }

    vertexCount = vertexMax - vertexMin + 1;
    m_pContext->drawVertexCount = vertexCount;

    if ((fmtCount % 8) != 0)
    {
        DumpToFile(m_pDumpDrawFile, "\n");
    }
    DumpToFile(m_pDumpDrawFile, "*************************index*************************\n\n");

    GetVertexAttribData(func, pFrame, vertexCount);

    DumpShader(func, pFrame);
    DumpVertexData(func, pFrame, vertexCount, vertexMin);
    DumpFlushFile(m_pDumpDrawFile);

    if (m_bWriteDrawBin)
    {
        WriteDraw(func);
    }
}

GLvoid CDumpInfo::DumpDrawArrays(GLESAPIIndex func)
{
    CFrameInfo  *pFrame     = FindFrameInfo(m_nCurrentFrame);
    GLenum      mode        = m_pContext->drawMode;
    GLint       first       = m_pContext->drawFirst;
    GLsizei     count       = m_pContext->drawCount;
    GLboolean   bNeedDump   = IsDrawNeedDump();

    pFrame->m_nDrawCount ++;
    pFrame->m_nPrimitiveCount += DumpGetPrimitiveCountPerMode(mode, count);

    GetVertexAttribData(func, pFrame, count);

    if (!bNeedDump)
    {
        return;
    }

    DumpToFile(m_pDumpDrawFile, "////////////////////////////////////////////////////////////////////////////////////////////////////////\n");
    DumpToFile(m_pDumpDrawFile, "//                                                draw                                                //\n");
    DumpToFile(m_pDumpDrawFile, "////////////////////////////////////////////////////////////////////////////////////////////////////////\n");
    TranslateDrawMode(mode, dumpTmp1);
    DumpToFile(m_pDumpDrawFile, "#%d glDrawArrays(%s, first=%d, count=%d) prog=%08d\n\n",
        m_nDrawTotal, dumpTmp1, first, count, m_nOrigProgram);

    DumpDrawStates();
    DumpShader(func, pFrame);
    DumpVertexData(func, pFrame, count, first);
    DumpFlushFile(m_pDumpDrawFile);

    if (m_bWriteDrawBin)
    {
        WriteDraw(func);
    }
}

GLvoid CDumpInfo::DumpDraw(GLESAPIIndex func)
{
    m_nDrawTotal ++;

    if (m_bDumpEnable == GL_FALSE || m_pDumpDrawFile == NULL)
    {
        return;
    }

    switch (func)
    {
    case GL3_API_glDrawElements:
        DumpDrawElements(func);
        break;

    case GL3_API_glDrawArrays:
        DumpDrawArrays(func);
        break;

    default:
        break;
    }

#if API_DUMP_SINGLE_DRAW_INFO
    if (m_pDrawSingleFile)
    {
        fclose(m_pDrawSingleFile);
        m_pDrawSingleFile = NULL;
    }
#endif

    if (IsDrawNeedDump())
    {
        GLchar buf[256];
        GLchar path[256];

        memset(path, 0, 256);
        sprintf(path, "%sdraw/result", m_strPath);

#ifdef WIN32
        {
            BOOL bRes = apiOS_DumpFilePathExists(path);
            if (!bRes)
            {
                bRes = apiOS_DumpCreateDirectory(path);
                if (!bRes)
                {
                    printf("%s(%d): execute %s.", __FUNCTION__, __LINE__, path);
                    return;
                }
            }
        }
#endif

        memset(buf, 0, 256);
        sprintf(buf, "%s/%s_%d", path,
            (func == GL3_API_glDrawElements)?"glDrawElements":"glDrawArrays", m_nDrawTotal);
        DumpColorBuffer(buf);
    }
}

GLvoid CDumpInfo::DumpTextureImage(GLuint unit, stTexImage *pTexImage)
{
    GLchar  buf[256];
    GLint   format;
    FILE    *pFile = NULL;
    GLchar  path[256];

    memset(path, 0, 256);
    sprintf(path, "%sdraw/texture", m_strPath);

#ifdef WIN32
    {
        BOOL bRes = apiOS_DumpFilePathExists(path);
        if (!bRes)
        {
            bRes = apiOS_DumpCreateDirectory(path);
            if (!bRes)
            {
                printf("%s(%d): execute %s.", __FUNCTION__, __LINE__, path);
                return;
            }
        }
    }
#endif

    DumpToFile(m_pDumpDrawFile, "   textureUnit: %d fileName: %s\n", unit, pTexImage->texName);
    memset(buf, 0, 256);
    sprintf(buf, "%s/%s", path, pTexImage->texName);

    pFile = fopen(buf, "rb");
    if (pFile == NULL)
    {
        format = DumpConvertFormat(pTexImage->format, pTexImage->type);
        switch (pTexImage->format)
        {
        case GL_COMPRESSED_RGB8_ETC2:
            break;

        case GL_ETC1_RGB8_OES:
            {
                GLvoid *pDecompress = DumpDecompressETC1(pTexImage->width,
                                                         pTexImage->height,
                                                         pTexImage->imageSize,
                                                         pTexImage->pixels,
                                                         &format);
                ApiWriteBitmap(buf, pDecompress, pTexImage->width, pTexImage->height, 0, format);
                free(pDecompress);
            }
            break;

        default:
            ApiWriteBitmap(buf, pTexImage->pixels, pTexImage->width, pTexImage->height, 0, format);
            break;
        }
    }
    else
    {
        fclose(pFile);
    }
}

GLvoid CDumpInfo::DumpTextureParameter(GLenum target, GLuint unit)
{
    GLint value = 0;

    g_opengl->glActiveTexture(unit+GL_TEXTURE0);

    g_opengl->glGetTexParameteriv(target, GL_TEXTURE_WRAP_S, &value);
    TranslateTexValue(value, dumpTmp1);
    DumpToFile(m_pDumpDrawFile, "   wrap_s: %s\n", dumpTmp1);

    g_opengl->glGetTexParameteriv(target, GL_TEXTURE_WRAP_T, &value);
    TranslateTexValue(value, dumpTmp1);
    DumpToFile(m_pDumpDrawFile, "   wrap_t: %s\n", dumpTmp1);

    g_opengl->glGetTexParameteriv(target, GL_TEXTURE_MIN_FILTER, &value);
    TranslateTexValue(value, dumpTmp1);
    DumpToFile(m_pDumpDrawFile, "   min_filter: %s\n", dumpTmp1);

    g_opengl->glGetTexParameteriv(target, GL_TEXTURE_MAG_FILTER, &value);
    TranslateTexValue(value, dumpTmp1);
    DumpToFile(m_pDumpDrawFile, "   mag_filter: %s\n", dumpTmp1);

    g_opengl->glGetTexParameteriv(target, GL_TEXTURE_MAX_ANISOTROPY_EXT, &value);
    DumpToFile(m_pDumpDrawFile, "   anisotropy: %d\n", value);

    // Restore current texture unit
    g_opengl->glActiveTexture(m_pContext->activeTexUnit);
}

GLvoid CDumpInfo::DumpTexture(GLint loc, GLenum type, GLuint size)
{
    GLint       *pValue = (GLint*)malloc(size*4);
    CTexObj     *pTex   = NULL;
    stTexImage  *pImg   = NULL;

    g_opengl->glGetUniformiv(m_nCurrentProgram, loc, pValue);

    if (type == GL_SAMPLER_2D)
    {
        for (GLuint j=0; j<size; j++)
        {
            GLenum unit = pValue[j];
            pTex = m_pContext->GetTexObjByUnit(unit+GL_TEXTURE0, GL_TEXTURE_2D);
            pImg = pTex->pTex2D;

            DumpTextureParameter(GL_TEXTURE_2D, unit);
            while (pImg)
            {
                DumpTextureImage(pValue[j], pImg);
                pImg = pImg->next;
            }
        }
    }
    else if (type == GL_SAMPLER_CUBE)
    {
        for (GLuint j=0; j<size; j++)
        {
            GLenum unit = pValue[j];
            pTex = m_pContext->GetTexObjByUnit(unit+GL_TEXTURE0, GL_TEXTURE_2D);

            DumpTextureParameter(GL_TEXTURE_CUBE_MAP, unit);
            for (GLuint k=0; k<6; k++)
            {
                pImg = pTex->pTexCube[k];
                while (pImg)
                {
                    DumpTextureImage(pValue[j], pImg);
                    pImg = pImg->next;
                }
            }
        }
    }

    free(pValue);
}

GLvoid CDumpInfo::SaveShaderSource(GLESAPIIndex func, CFrameInfo *pFrame)
{
    GLuint      shader      = m_pContext->curShader;
    GLsizei     count       = m_pContext->curShaderCount;
    GLuint      strLen      = 0;
    CShaderObj  *pShader    = FindShaderObject(shader);
    GLchar      *pSrcArray  = pShader->m_source;
    GLchar      *pSrc       = NULL;

    if (pShader == NULL)
    {
        printf("%s(%d): cannot find shader object.\n", __FUNCTION__, __LINE__);
        return;
    }

    pShader->m_source   = (GLchar*)malloc(strLen);
    pShader->m_len      = strLen;
    pSrc                = pShader->m_source;

    memset(pSrc, 0, strLen);
    memcpy(pSrc, pSrcArray, strLen-1);

    g_opengl->glShaderSource(pShader->m_name, 1, &pShader->m_source, NULL);
}

CAttribObj* CDumpInfo::FindAttribObjByIndex(GLuint index)
{
    CProgramObj *p = FindProgramObject(m_nOrigProgram);
    return p->FindAttribObjByIndex(index);
}

CUniformObj* CDumpInfo::FindUniformByOldLoc(GLuint program, GLint location)
{
    CProgramObj *pProg  = FindProgramObject(program);
    return pProg->FindUniformByOldLoc(location);
}

CAttribObj* CDumpInfo::FindAttributeByName(CProgramObj *program, GLchar *name)
{
    if (program == NULL)
    {
        return NULL;
    }

    return program->FindAttributeByName(name);
}

GLvoid CDumpInfo::SaveProgram()
{
    g_opengl->glGetIntegerv(GL_CURRENT_PROGRAM, &m_nSavedProgram);
    g_opengl->glUseProgram(m_nCurrentProgram);
}

GLvoid CDumpInfo::LoadProgram()
{
    g_opengl->glUseProgram(m_nSavedProgram);
}

GLvoid CDumpInfo::BeginFrameInfo(GLESAPIIndex func)
{
    GLuint          current = apiOS_GetMicroSeconds();
    CFrameInfo      *pFrame = FindFrameInfo(m_nCurrentFrame);

    if (pFrame == NULL)
    {
        printf("BeginFrameInfo: Cannot find frame (%d)\n", m_nCurrentFrame);
        return;
    }

    if (!m_bTimeOnly)
    {
        // Caculate total event per frame.
        pFrame->m_nTotalEvent ++;

        switch (func)
        {
        case GL3_API_glBindBuffer:
            break;

        case GL3_API_glVertexAttribPointer:
            break;

        case GL3_API_glCreateProgram:
            break;

        case GL3_API_glCreateShader:
            break;

        case GL3_API_glAttachShader:
            break;

        case GL3_API_glCompileShader:
            break;

        case GL3_API_glLinkProgram:
            break;

        case GL3_API_glUseProgram:
            {
                m_nOrigProgram = m_pContext->curProgram;

                if (m_nOrigProgram != 0)
                {
                    CProgramObj *p = FindProgramObject(m_nOrigProgram);
                    m_nCurrentProgram = p->m_name;
                }
                else
                {
                    m_nCurrentProgram = 0;
                }
            }
            break;

        case GL3_API_glBindAttribLocation:
            break;

        case GL3_API_glGetAttribLocation:
            break;

        case GL3_API_glGetUniformLocation:
            break;

        case GL3_API_glUniform1f:
        case GL3_API_glUniform1i:
        case GL3_API_glUniform2f:
        case GL3_API_glUniform2i:
        case GL3_API_glUniform3f:
        case GL3_API_glUniform3i:
        case GL3_API_glUniform4f:
        case GL3_API_glUniform4i:
            break;

        case GL3_API_glUniformMatrix2fv:
        case GL3_API_glUniformMatrix3fv:
        case GL3_API_glUniformMatrix4fv:
            break;

        case GL3_API_glUniform1fv:
        case GL3_API_glUniform1iv:
        case GL3_API_glUniform2fv:
        case GL3_API_glUniform2iv:
        case GL3_API_glUniform3fv:
        case GL3_API_glUniform3iv:
        case GL3_API_glUniform4fv:
        case GL3_API_glUniform4iv:
            break;

        case GL3_API_glActiveTexture:
            break;

        case GL3_API_glBindTexture:
            break;

        case GL3_API_glPixelStorei:
            break;

        case GL3_API_glBlendColor:
            break;

        case GL3_API_glClear:
            {
                GLfloat     clearColor[4];
                GLfloat     depthClear;
                GLint       stencilClear;

                g_opengl->glGetFloatv(GL_COLOR_CLEAR_VALUE, clearColor);
                m_clearColorR = clearColor[0];
                m_clearColorG = clearColor[1];
                m_clearColorB = clearColor[2];
                m_clearColorA = clearColor[3];

                g_opengl->glGetFloatv(GL_DEPTH_CLEAR_VALUE, &depthClear);
                m_clearDepth = depthClear;

                g_opengl->glGetIntegerv(GL_STENCIL_CLEAR_VALUE, &stencilClear);
                m_clearStencil = stencilClear;

                m_clearBits = m_pContext->clearBitsMask;
            }
            break;

        default:
            break;
        }
    }

    m_nEventBeginTicks = current;

    if (func != EGL_API_eglSwapBuffers)
    {
        return;
    }

    pFrame->m_nFrameTimeBeforeEglSwap = current;
}

CProgramObj* CDumpInfo::FindProgramObject(GLuint name)
{
    return m_pContext->FindProgram(name);
}

CShaderObj* CDumpInfo::FindShaderObject(GLuint name)
{
    return m_pContext->FindShader(name);
}

GLvoid CDumpInfo::EndFrameInfo(GLESAPIIndex func)
{
    GLuint      current = apiOS_GetMicroSeconds();
    CFrameInfo  *pFrame = FindFrameInfo(m_nCurrentFrame);

    if (pFrame == NULL)
    {
        printf("EndFrameInfo: Cannot find frame (%d)\n", m_nCurrentFrame);
        return;
    }

    if (!m_bTimeOnly)
    {
        GLuint nCurrentSize = 0;

        switch (func)
        {
        case GL3_API_glBufferData:
            break;

        case GL3_API_glBufferSubData:
            break;

        case GL3_API_glTexImage2D:
        case GL3_API_glTexSubImage2D:
        case GL3_API_glCompressedTexImage2D:
        case GL3_API_glCompressedTexSubImage2D:
            break;

        case GL3_API_glTexStorage2D:
            break;

        case GL3_API_glDrawElements:
        case GL3_API_glDrawArrays:
            DumpDraw(func);
            break;

        case GL3_API_glViewport:
            {
                GLint nViewport[4];

                g_opengl->glGetIntegerv(GL_VIEWPORT, nViewport);
                m_nWidth    = nViewport[2];
                m_nHeight   = nViewport[3];
            }
            break;

        case GL3_API_glShaderSource:
            SaveShaderSource(func, pFrame);
            break;

        default:
            break;
        }

        if (m_bCaculateRenderPixels)
        {
            if (IsDrawCommand((GLESAPIIndex)func))
            {
                nCurrentSize = m_nWidth * m_nHeight * 4;
                if (m_nPixelBuffersSize != nCurrentSize)
                {
                    if (m_pPixelBuffers != NULL)
                    {
                        free(m_pPixelBuffers);
                    }

                    if (m_pLastPixelBuffers != NULL)
                    {
                        free(m_pLastPixelBuffers);
                    }

                    m_pPixelBuffers     = (GLchar*)malloc(nCurrentSize);
                    m_pLastPixelBuffers = (GLchar*)malloc(nCurrentSize);
                    m_nPixelBuffersSize = nCurrentSize;
                }

                g_opengl->glReadPixels(0, 0, m_nWidth, m_nHeight, GL_RGBA, GL_UNSIGNED_BYTE, m_pPixelBuffers);

                if (func == GL3_API_glClear)
                {
                    memcpy(m_pLastPixelBuffers, m_pPixelBuffers, m_nPixelBuffersSize);
                }
            }

            if (IsDrawCommandWithoutClear((GLESAPIIndex)func))
            {
                GLchar  *pSrc   = m_pLastPixelBuffers;
                GLchar  *pDst   = m_pPixelBuffers;
                GLchar  *pTmp   = m_pLastPixelBuffers;

                for (GLuint i=0; i<m_nHeight; i++)
                {
                    for (GLuint j=0; j<m_nWidth; j++)
                    {
                        if ((pSrc[0] != pDst[0]) ||
                            (pSrc[1] != pDst[1]) ||
                            (pSrc[2] != pDst[2]) ||
                            (pSrc[3] != pDst[3]))
                        {
                            m_nCurrentPixels ++;
                        }

                        pSrc += 4;
                        pDst += 4;
                    }
                }

                m_pLastPixelBuffers = m_pPixelBuffers;
                m_pPixelBuffers     = pTmp;
            }
        }
    }

    pFrame->m_nFramePlayEventTimeCost += (current - m_nEventBeginTicks);

    if (func != EGL_API_eglSwapBuffers)
    {
        return;
    }

    m_fCurrentFrameCost                 = (GLfloat)(current - m_nLastFrameTicks) / 1000.0f;
    m_nLastFrameTicks                   = current;
    pFrame->m_nFrameTimeAfterEglSwap    = current;
    pFrame->m_fFrameTimeCost            = m_fCurrentFrameCost;
    pFrame->m_fFramePlayEventTimeCost   = (GLfloat)(pFrame->m_nFramePlayEventTimeCost) / 1000.0f;
    pFrame->m_fFrameRenderTimeCost      = (GLfloat)(m_nRenderTotalTime) / 1000.0f;
    pFrame->m_nPixelsCount              = m_nCurrentPixels;
    m_nCurrentPixels                    = 0;
    m_nRenderTotalTime                  = 0;

    if (m_nCurrentFrame > 1)
    {
        m_fTotalRenderTime      += pFrame->m_fFrameRenderTimeCost;
        m_fTotalPlayEventTime   += pFrame->m_fFramePlayEventTimeCost;
        m_fTotalRunTime         += pFrame->m_fFrameTimeCost;
    }

    // Create next frame
    m_nCurrentFrame ++;
    pFrame = new CFrameInfo(m_nCurrentFrame);
    if (pFrame == NULL)
    {
        m_bDumpEnable = GL_FALSE;
        return;
    }
    InsertFrameInfo(pFrame);
}

GLboolean CDumpInfo::IsDrawCommand(GLESAPIIndex name)
{
    if(name == GL3_API_glDrawArrays ||
       name == GL3_API_glDrawElements ||
       name == GL3_API_glClear||
       name == GL3_API_glDrawArraysInstanced ||
       name == GL3_API_glDrawElementsInstanced ||
       name == GL3_API_glDrawRangeElements ||
       name == GL3_API_glMultiDrawArraysEXT ||
       name == GL3_API_glMultiDrawElementsEXT)
    {
        return GL_TRUE;
    }

    return GL_FALSE;
}

GLboolean CDumpInfo::IsDrawCommandWithoutClear(GLESAPIIndex name)
{
    if(name == GL3_API_glDrawArrays ||
       name == GL3_API_glDrawElements ||
       name == GL3_API_glDrawArraysInstanced ||
       name == GL3_API_glDrawElementsInstanced ||
       name == GL3_API_glDrawRangeElements ||
       name == GL3_API_glMultiDrawArraysEXT ||
       name == GL3_API_glMultiDrawElementsEXT)
    {
        return GL_TRUE;
    }

    return GL_FALSE;
}

CFrameInfo::CFrameInfo(GLuint nFrame)
{
    m_nFrame                        = nFrame;
    m_nFrameTimeBeforeEglSwap       = 0;
    m_nFrameTimeAfterEglSwap        = 0;
    m_fFrameTimeCost                = 0.0f;
    m_fFrameRenderTimeCost          = 0.0f;

    m_nEventStartTime               = 0;
    m_nFramePlayEventTimeCost       = 0;
    m_fFramePlayEventTimeCost       = 0.0f;

    m_nTotalEvent                   = 0;
    m_nDrawCount                    = 0;
    m_nProcessedVertexCount         = 0;
    m_nPrimitiveCount               = 0;
    m_nPixelsCount                  = 0;

    m_nTexImageTotalCount           = 0;
    m_nTexImageTotalDataSize        = 0;
    m_nTexImageCompressedCount      = 0;
    m_nTexImageCompressedDataSize   = 0;
    m_nTexImageCount                = 0;
    m_nTexImageDataSize             = 0;

    m_nBufferDataCount              = 0;
    m_nBufferDataSize               = 0;
    m_nVertexAttribData             = 0;
}

CFrameInfo::~CFrameInfo()
{
}
