#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <string.h>

#include "MathAPI.h"
#include "TranslateGL.h"
#include "gles3.h"
#include "ConvertApiDump.h"
#include "utils.h"
#include "egl.h"
#include "MemoryPool.h"
#include "ParseGLES3.h"
#include "ApiGLES3Context.h"

#define FAKE_FRAGMENT_SHADER    0
#define FAKE_VERTEX_SHADER      0

GLenum UniformIsTextureSampler(const GLchar *name, GLenum *shaderType);
GLvoid OutputVertexAttribToFrameFile(GLuint index);

int draws[] =
{
    886 //26954
};
int nDraw = sizeof(draws)/sizeof(draws[0]);

GLboolean IsDrawNeedDump(GLuint nDrawTotal)
{
    for (int i=0; i<nDraw; i++)
    {
        if (draws[i] == nDrawTotal)
        {
            return GL_TRUE;
        }
    }

    return GL_FALSE;
}

GLvoid GLES3Init()
{
    ApiCreateTexMemPool();
}

GLvoid GLES3Release()
{
    ApiDestroyTexMemPool();
}

GLboolean GLES3Disassemble(GLchar *input, GLchar *output, GLint outputSize)
{
    GLint           arg[10];
    const GLchar    *ptr    = NULL;
    GLclampf        *p1     = (GLclampf*)&arg[0];
    GLclampf        *p2     = (GLclampf*)&arg[1];
    GLclampf        *p3     = (GLclampf*)&arg[2];
    GLclampf        *p4     = (GLclampf*)&arg[3];
    GLint           pos     = 0;

    if (outputSize != DRAW_BUF_SIZE)
    {
        memset(output, 0, outputSize);
    }

    switch (*input)
    {
    case 'A':
        {
            PROCESS_INPUT(ActiveTexture);
            PROCESS_INPUT(AttachShader);
        }
        break;

    case 'B':
        {
            PROCESS_INPUT(BindTexture);
            PROCESS_INPUT(BindBufferBase);
            PROCESS_INPUT(BindBufferRange);
            PROCESS_INPUT(BindBuffer);
            PROCESS_INPUT(BufferData);
            PROCESS_INPUT(BufferSubData);
            PROCESS_INPUT(BindVertexArray);
            PROCESS_INPUT(BindTransformFeedback);
            PROCESS_INPUT(BeginTransformFeedback);
            PROCESS_INPUT(BindAttribLocation);
            PROCESS_INPUT(BindFramebuffer);
            PROCESS_INPUT(BindRenderbuffer);
            PROCESS_INPUT(BlendFuncSeparate);
            PROCESS_INPUT(BlendFunc);
            PROCESS_INPUT(BlendColor);
            PROCESS_INPUT(BlendEquationSeparate);
            PROCESS_INPUT(BlendEquation);
        }
        break;

    case 'C':
        {
            PROCESS_INPUT(ClearColor);
            PROCESS_INPUT(ClearDepthf);
            PROCESS_INPUT(ClearStencil);
            PROCESS_INPUT(Clear);
            PROCESS_INPUT(ColorMask);
            PROCESS_INPUT(CullFace);
            PROCESS_INPUT(CreateShader);
            PROCESS_INPUT(CompileShader);
            PROCESS_INPUT(CreateProgram);
            PROCESS_INPUT(CopyTexImage2D);
            PROCESS_INPUT(CopyTexSubImage2D);
            PROCESS_INPUT(CompressedTexImage2D);
            PROCESS_INPUT(CompressedTexSubImage2D);
            PROCESS_INPUT(CheckFramebufferStatus);
        }
        break;

    case 'D':
        {
            PROCESS_INPUT(DrawBuffers);
            PROCESS_INPUT(DisableVertexAttribArray);
            PROCESS_INPUT(Disable);
            PROCESS_INPUT(DrawArrays);
            PROCESS_INPUT(DrawElements);
            PROCESS_INPUT(DetachShader);
            PROCESS_INPUT(DepthFunc);
            PROCESS_INPUT(DepthMask);
            PROCESS_INPUT(DepthRangef);
            PROCESS_INPUT(DeleteShader);
            PROCESS_INPUT(DeleteTextures);
            PROCESS_INPUT(DeleteBuffers);
            PROCESS_INPUT(DeleteVertexArrays);
            PROCESS_INPUT(DeleteTransformFeedback);
            PROCESS_INPUT(DeleteProgram);
            PROCESS_INPUT(DeleteRenderbuffers);
            PROCESS_INPUT(DeleteFramebuffers);
            PROCESS_INPUT(DiscardFramebufferEXT);
        }
        break;

    case 'E':
        {
            PROCESS_INPUT(EnableVertexAttribArray);
            PROCESS_INPUT(Enable);
            PROCESS_INPUT(EndTransformFeedback);
            PROCESS_INPUT(EGLImageTargetRenderbufferStorageOES);
            PROCESS_INPUT(EGLImageTargetTexture2DOES);
        }
        break;

    case 'F':
        {
            PROCESS_INPUT(FramebufferTexture2D);
            PROCESS_INPUT(FramebufferRenderbuffer);
            PROCESS_INPUT(FlushMappedBufferRange);
            PROCESS_INPUT(Finish);
            PROCESS_INPUT(Flush);
            PROCESS_INPUT(FrontFace);
        }
        break;

    case 'G':
        {
            PROCESS_INPUT(GenBuffers);
            PROCESS_INPUT(GenerateMipmap);
            PROCESS_INPUT(GenFramebuffers);
            PROCESS_INPUT(GenRenderbuffers);
            PROCESS_INPUT(GenTextures);
            PROCESS_INPUT(GenTransformFeedbacks);
            PROCESS_INPUT(GenVertexArrays);
            PROCESS_INPUT(GetProgramBinary);
            PROCESS_INPUT(GetError);
            PROCESS_INPUT(GetUniformLocation);
            PROCESS_INPUT(GetShaderiv);
            PROCESS_INPUT(GetShaderInfoLog);
            PROCESS_INPUT(GetShaderSource);
            PROCESS_INPUT(GetProgramiv);
            PROCESS_INPUT(GetActiveAttrib);
            PROCESS_INPUT(GetAttribLocation);
            PROCESS_INPUT(GetAttachedShaders);
            PROCESS_INPUT(GetProgramInfoLog);
            PROCESS_INPUT(GetVertexAttribfv);
            PROCESS_INPUT(GetVertexAttribPointerv);
            PROCESS_INPUT(GetVertexAttribiv);
            PROCESS_INPUT(GetBufferParameteriv);
            PROCESS_INPUT(GetString);
            PROCESS_INPUT(GetIntegerv);
            PROCESS_INPUT(GetBooleanv);
            PROCESS_INPUT(GetFloatv);
            PROCESS_INPUT(GetShaderPrecisionFormat);
            PROCESS_INPUT(GetActiveUniform);
            PROCESS_INPUT(GetRenderbufferParameteriv);
            PROCESS_INPUT(GetTexParameteriv);
            PROCESS_INPUT(GetTexParameterfv);
            PROCESS_INPUT(GetUniformfv);
            PROCESS_INPUT(GetUniformiv);
            PROCESS_INPUT(GetFramebufferAttachmentParameteriv);
        }
        break;

    case 'H':
        {
            PROCESS_INPUT(Hint);
        }
        break;

    case 'I':
        {
            PROCESS_INPUT(InvalidateFramebuffer);
            PROCESS_INPUT(IsBuffer);
            PROCESS_INPUT(IsShader);
            PROCESS_INPUT(IsProgram);
            PROCESS_INPUT(IsRenderbuffer);
            PROCESS_INPUT(IsEnabled);
            PROCESS_INPUT(IsTexture);
            PROCESS_INPUT(IsFramebuffer);
        }
    break;

    case 'L':
        {
            PROCESS_INPUT(LinkProgram);
            PROCESS_INPUT(LineWidth);
        }
        break;

    case 'M':
        {
            PROCESS_INPUT(MapBufferRange);
        }
        break;

    case 'P':
        {
            PROCESS_INPUT(ProgramBinaryOES);
            PROCESS_INPUT(ProgramBinary);
            PROCESS_INPUT(PauseTransformFeedback);
            PROCESS_INPUT(PixelStorei);
            PROCESS_INPUT(PolygonOffset);
        }
        break;

    case 'R':
        {
            PROCESS_INPUT(ResumeTransformFeedback);
            PROCESS_INPUT(ReadPixels);
            PROCESS_INPUT(RenderbufferStorage);
        }
        break;

    case 'S':
        {
            PROCESS_INPUT(Scissor);
            PROCESS_INPUT(StencilFuncSeparate);
            PROCESS_INPUT(StencilFunc);
            PROCESS_INPUT(StencilMaskSeparate);
            PROCESS_INPUT(StencilMask);
            PROCESS_INPUT(StencilOpSeparate);
            PROCESS_INPUT(StencilOp);
            PROCESS_INPUT(ShaderSource);
            PROCESS_INPUT(SampleCoverage);
        }
        break;

    case 'U':
        {
            PROCESS_INPUT(UnmapBuffer);

            PROCESS_INPUT(Uniform1fv);
            PROCESS_INPUT(Uniform1f);
            PROCESS_INPUT(Uniform1iv);
            PROCESS_INPUT(Uniform1i);

            PROCESS_INPUT(Uniform2fv);
            PROCESS_INPUT(Uniform2f);
            PROCESS_INPUT(Uniform2iv);
            PROCESS_INPUT(Uniform2i);

            PROCESS_INPUT(Uniform3fv);
            PROCESS_INPUT(Uniform3f);
            PROCESS_INPUT(Uniform3iv);
            PROCESS_INPUT(Uniform3i);

            PROCESS_INPUT(Uniform4fv);
            PROCESS_INPUT(Uniform4f);
            PROCESS_INPUT(Uniform4iv);
            PROCESS_INPUT(Uniform4i);

            PROCESS_INPUT(UniformMatrix2fv);
            PROCESS_INPUT(UniformMatrix3fv);
            PROCESS_INPUT(UniformMatrix4fv);

            PROCESS_INPUT(UseProgram);
        }
    break;

    case 'T':
        {
            PROCESS_INPUT(TransformFeedbackVaryings);

            PROCESS_INPUT(TexParameteriv);
            PROCESS_INPUT(TexParameteri);
            PROCESS_INPUT(TexParameterfv);
            PROCESS_INPUT(TexParameterf);

            PROCESS_INPUT(TexStorage2D);
            PROCESS_INPUT(TexStorage3D);
            PROCESS_INPUT(TexImage2D);
            PROCESS_INPUT(TexSubImage2D);
            PROCESS_INPUT(TexImage3D);
            PROCESS_INPUT(TexSubImage3D);
        }
    break;

    case 'V':
        {
            PROCESS_INPUT(VertexAttribPointer);
            PROCESS_INPUT(VertexAttribIPointer);

            PROCESS_INPUT(VertexAttrib1fv);
            PROCESS_INPUT(VertexAttrib2fv);
            PROCESS_INPUT(VertexAttrib3fv);
            PROCESS_INPUT(VertexAttrib4fv);

            PROCESS_INPUT(VertexAttrib1f);
            PROCESS_INPUT(VertexAttrib2f);
            PROCESS_INPUT(VertexAttrib3f);
            PROCESS_INPUT(VertexAttrib4f);

            PROCESS_INPUT(Viewport);
            PROCESS_INPUT(ValidateProgram);
        }
        break;

    default:
        sprintf(output, "gl%s\n", input);
        break;
    }

    return true;
}

GLvoid OutputMaskToFrameFile(GLchar *str)
{
    OutputToTarget(OUTPUT_TO_FILE, GL_FALSE, 0, NULL, str, g_pFrameFile);
}

GLvoid OutputClearToFrameFile(GLint mask)
{
    GLchar buf[1024];

    memset(buf, 0, 1024);
    TranslateClearMask(mask, tmp);
    sprintf(buf, "glClear(%s)", tmp);

    if (mask & GL_COLOR_BUFFER_BIT)
    {
        OutputStrcat(buf, 1024, "  RGBA(%f, %f, %f, %f) Mask(%01d%01d%01d%01d)", 
            CURRENT_CONTEXT1().clearRed, CURRENT_CONTEXT1().clearGreen, CURRENT_CONTEXT1().clearBlue, CURRENT_CONTEXT1().clearAlpha,
            CURRENT_CONTEXT1().colorRedMask, CURRENT_CONTEXT1().colorGreenMask, CURRENT_CONTEXT1().colorBlueMask, CURRENT_CONTEXT1().colorAlphaMask);
    }

    if (mask & GL_STENCIL_BUFFER_BIT)
    {
        OutputStrcat(buf, 1024, "  Stencil(0x%X)", CURRENT_CONTEXT1().clearStencil);
    }

    if (mask & GL_DEPTH_BUFFER_BIT)
    {
        OutputStrcat(buf, 1024, "  Depth(%1.1f)", CURRENT_CONTEXT1().clearDepth);
    }

    OutputStrcat(buf, 1024, "\n");
    OutputToTarget(OUTPUT_TO_FRAME_STRING, GL_FALSE, ++g_frameDrawCount, NULL, buf, g_pFrameFile);
}

GLvoid OutputDrawArraysToFrameFile_es20(GLenum mode, GLuint first, GLint count, GLuint historyID)
{
    GLuint      progID      = CURRENT_CONTEXT1().curProgram;
    CProgramObj *prog       = CURRENT_CONTEXT1().FindProgram(progID);
    GLuint      uniformSize = prog->m_uniforms.size();
    GLchar      buf[1024];

    memset(buf, 0, 1024);
    TranslateDrawMode(mode, tmp1);

    if (bThread)
    {
        sprintf(buf, "%s glDrawArrays(%s, %d, %d)", thread, tmp1, first, count);
    }
    else
    {
        sprintf(buf, "glDrawArrays(%s, %d, %d)", tmp1, first, count);
    }

    OutputStrcat(buf, 1024, "  prog(%d)", CURRENT_CONTEXT1().curProgram);

    if (CURRENT_CONTEXT1().currentFBO)
    {
        OutputStrcat(buf, 1024, "  FBO(%d)", CURRENT_CONTEXT1().currentFBO);
    }

    if (CURRENT_CONTEXT1().scissorEnable)
    {
        OutputStrcat(buf, 1024, ",  s(%d, %d, %d, %d)",
            CURRENT_CONTEXT1().scissorX, CURRENT_CONTEXT1().scissorY,
            CURRENT_CONTEXT1().scissorWidth, CURRENT_CONTEXT1().scissorHeight);
    }

    for (unsigned int i=0; i<uniformSize; i++)
    {
        CUniformObj *pUniform   = prog->m_uniforms[i];
        GLuint      unit        = 0; 
        GLenum      dataType    = pUniform->type;
        GLuint      texId       = 0;

        if (dataType != GL_SAMPLER_2D &&
            dataType != GL_SAMPLER_CUBE)
            continue;

        if (!pUniform->data)
        {
            unit = 0;
        }
        else
        {
            unit = *((GLuint*)pUniform->data);
        }

        switch (dataType)
        {
        case GL_SAMPLER_2D:
            texId = CURRENT_CONTEXT1().textureSampler[unit][TEXTURE_TYPE_2D];
            break;

        case GL_SAMPLER_CUBE:
            texId = CURRENT_CONTEXT1().textureSampler[unit][TEXTURE_TYPE_CUBEMAP];
            break;

        default:
            break;
        }

        OutputStrcat(buf, 1024, ",  t(%04d)", texId);
    }

    //OutputStrcat(buf, 1024, " <url:./%s.hy#tn=DrawArrays: %d>\n", strOnlyFileName, CURRENT_CONTEXT1().nDrawArrays);
    OutputStrcat(buf, 1024, " #%d\n", CURRENT_CONTEXT1().nDrawTotal);
    OutputToTarget(OUTPUT_TO_FRAME_STRING, GL_FALSE, ++g_frameDrawCount, NULL, buf, g_pFrameFile);
}

GLvoid OutputDrawElementsToFrameFile_es20(GLenum mode, GLint count, GLenum indexType)
{
    GLuint      progID      = CURRENT_CONTEXT1().curProgram;
    CProgramObj *prog       = CURRENT_CONTEXT1().FindProgram(progID);
    GLuint      uniformSize = prog->m_uniforms.size();
    GLchar      buf[1024];

    memset(buf, 0, 1024);
    TranslateDrawMode(mode, tmp1);
    TranslateDataType(indexType, tmp3);

    if (bThread)
    {
        OutputStrcat(buf, 1024, "%s glDrawElements(%s, %d, %s)", thread, tmp1, count, tmp3);
    }
    else
    {
        OutputStrcat(buf, 1024, "glDrawElements(%s, %d, %s)", tmp1, count, tmp3);
    }

    OutputStrcat(buf, 1024, "  prog(%04d)", CURRENT_CONTEXT1().curProgram);

    if (CURRENT_CONTEXT1().currentFBO)
    {
        OutputStrcat(buf, 1024, "  FBO(%d)", CURRENT_CONTEXT1().currentFBO);
    }

    if (CURRENT_CONTEXT1().scissorEnable)
    {
        OutputStrcat(buf, 1024, ",  s(%d, %d, %d, %d)",
            CURRENT_CONTEXT1().scissorX, CURRENT_CONTEXT1().scissorY,
            CURRENT_CONTEXT1().scissorWidth, CURRENT_CONTEXT1().scissorHeight);
    }

    for (unsigned int i=0; i<uniformSize; i++)
    {
        CUniformObj *uniform    = prog->m_uniforms[i];
        GLuint      unit        = 0;
        GLenum      dataType    = uniform->type;
        GLuint      texId       = 0;

        if (dataType != GL_SAMPLER_2D &&
            dataType != GL_SAMPLER_CUBE)
            continue;

        if (uniform->data)
        {
            unit = *((GLuint*)uniform->data);
        }
        else
        {
            unit = 0;
        }

        switch (dataType)
        {
        case GL_SAMPLER_2D:
            texId = CURRENT_CONTEXT1().textureSampler[unit][TEXTURE_TYPE_2D];
            break;

        case GL_SAMPLER_CUBE:
            texId = CURRENT_CONTEXT1().textureSampler[unit][TEXTURE_TYPE_CUBEMAP];
            break;

        default:
            break;
        }

        OutputStrcat(buf, 1024, ",  t(%04d)", texId);
    }

    // OutputStrcat(buf, 1024, " <url:./%s.hy#tn=DrawElements: %d>\n", strOnlyFileName, CURRENT_CONTEXT1().nDrawElements);
    OutputStrcat(buf, 1024, " #%d\n", CURRENT_CONTEXT1().nDrawTotal);
    OutputToTarget(OUTPUT_TO_FRAME_STRING, GL_FALSE, ++g_frameDrawCount, NULL, buf, g_pFrameFile);
}

GLvoid OutputDrawElements_es20(GLenum mode, GLint indexCount, GLenum indexType, GLuint indexAddr, GLchar *output, int outputSize)
{
    GLuint      progID          = CURRENT_CONTEXT1().curProgram;
    CProgramObj *program        = CURRENT_CONTEXT1().FindProgram(progID);
    GLuint      PrimitiveCnt    = 0;
    GLint       pos             = 0;
    GLuint      vertexMax       = 0;
    GLuint      vertexMin       = 0xFFFFFFFF;

    CURRENT_CONTEXT1().nDrawElements ++;
    CURRENT_CONTEXT1().nDrawTotal ++;

    if (bDumpFrameFile)
    {
        OutputDrawElementsToFrameFile_es20(mode, indexCount, indexType);
    }

    TranslateDrawMode(mode, tmp1);
    TranslateDataType(indexType, tmp2);
    //OutputStringFast(glDrawElements1, &pos, output, outputSize, "<url:./%s#tn=drawElements=%08d>\n", strHalDumpFileName, CURRENT_CONTEXT1().nDrawElements);
    //OutputStringFast(glDrawElements2, &pos, output, outputSize, "<url:./%s#tn=DrawElements: %08d>", strOnlyFrameFileName, CURRENT_CONTEXT1().nDrawElements);
    OutputStringFast(glDrawElements3, &pos, output, outputSize, "\n**********************************************************************************\n");
    OutputStringFast(glDrawElements4, &pos, output, outputSize, "  %s glDrawElements: %08d,   Totals: %08d\n", thread, CURRENT_CONTEXT1().nDrawElements, CURRENT_CONTEXT1().nDrawTotal);
    OutputStringFast(glDrawElements5, &pos, output, outputSize, "   %s, indexCount=%08d, %s, prog=%08d\n", tmp1, indexCount, tmp2, CURRENT_CONTEXT1().curProgram);
    
    GetPrimitiveCount(mode, indexCount, &PrimitiveCnt);
    OutputStates(&pos, ES20_VERSION, output, outputSize);
    OutputStringFast(glDrawElements7, &pos, output, outputSize, "Frames: %04d\n", g_nFrames);

    OutputToTarget(OUTPUT_TO_DRAW_TOTAL_STRING, GL_FALSE, 0, NULL, output, g_pOutputFile);
    memset(tmp1, 0, TMP_BUF_SIZE);
    sprintf(tmp1, "\n**********************************************************************************\n\n");
    OutputToTarget(OUTPUT_TO_TOTAL_STRING, GL_FALSE, 0, NULL, tmp1, g_pOutputFile);

    if (IsDrawNeedDump(CURRENT_CONTEXT1().nDrawTotal) == GL_FALSE)
    {
        return;
    }

    // Dump uniform
    DumpUniforms(&pos, output, outputSize);

    if (program)
    {
        GLuint  vertex      = program->m_pVertex->m_oldName;
        GLuint  fragment    = program->m_pFragment->m_oldName;

        PrintShader(&pos, progID, vertex, fragment, output, outputSize);

        if (bPrintVertexAttrib)
        {
            GLuint      indexBufferID   = CURRENT_CONTEXT1().elementArrayBuffer;
            CBufObj     *buffer         = NULL;
            GLchar      *p              = NULL;
            GLuint      pointer         = indexAddr;
            GLuint      lineNum         = indexCount/8;
            GLuint      columnNum       = 8;
            GLint       counter         = 0;
            GLuint      Index           = 0;
            GLuint      indexStride     = 0;
            GLvoid      *v              = NULL;

            if (indexType == GL_UNSIGNED_BYTE)
            {
                indexStride = 1;
            }
            else if (indexType == GL_UNSIGNED_SHORT)
            {
                indexStride = 2;
            }
            else if (indexType == GL_UNSIGNED_INT)
            {
                indexStride = 4;
            }
            else
            {
                OutputStringFast(glDrawElements8, &pos, output, outputSize, "\nglDrawElements: wrong index data type: 0x%X\n", indexType);
                goto _DrawElementsEnd;
            }

            if (indexBufferID)
            {
                buffer = CURRENT_CONTEXT1().bufferMap[indexBufferID];

#ifdef LINUX
                p = (GLchar*)((long)(buffer->m_pData) + (long)pointer);
#else
                p = (GLchar*)((GLuint)(buffer->m_pData) + (GLuint)pointer);
#endif

                if (buffer->m_pData == NULL)
                {
                    OutputStringFast(glDrawElements9, &pos, output, outputSize, "cannot get index buffer from buffer object!!!\n");
                    goto _DrawElementsEnd;
                }
            }
            else
            {
                //v = (GLvoid*)readUCharFast(0);
                v = (GLvoid*)ReadData(READ_GL_UCHAR, indexAddr, 0);
                p = (GLchar*)v;

                if (p == NULL)
                {
                    OutputStringFast(glDrawElements10, &pos, output, outputSize, "cannot read index buffer from dump file!!!\n");
                    goto _DrawElementsEnd;
                }
            }

            OutputStringFast(glDrawElements11, &pos, output, outputSize, "\n++++++++++++++++++++++++Index begin+++++++++++++++++++++++\n");
            OutputStringFast(glDrawElements12, &pos, output, outputSize, "indexCount: %08d\n", indexCount);
            OutputStringFast(glDrawElements13, &pos, output, outputSize, "indexType: %d\n", indexType);
            for (GLuint i=0; i<=lineNum; i++)
            {
                OutputStringFast(glDrawElements14, &pos, output, outputSize, "#%08d: ", i+1);
                for (GLuint j=0; j<columnNum; j++)
                {
                    OutputData(&pos, indexType, output, outputSize, p);

                    if (indexType == GL_UNSIGNED_BYTE)
                    {
                        Index = *(GLubyte*)p;
                    }
                    else if (indexType == GL_UNSIGNED_SHORT)
                    {
                        Index = *(unsigned short*)p;
                    }
                    else if (indexType == GL_UNSIGNED_INT)
                    {
                        Index = *(unsigned int*)p;
                    }
                    else
                    {
                        Index = 0;
                    }

                    if (Index > vertexMax)
                    {
                        vertexMax = Index;
                    }

                    if (Index < vertexMin)
                    {
                        vertexMin = Index;
                    }

                    counter ++;
                    p += indexStride;

                    if (counter == indexCount)
                    {
                        OutputStringFast(oneChar, &pos, output, outputSize, "\n");
                        goto _IndexEnd;
                    }

                    OutputStringFast(twoChar, &pos, output, outputSize, ", ");
                }
                OutputStringFast(oneChar, &pos, output, outputSize, "\n");
            }
_IndexEnd:
            OutputStringFast(glDrawElements15, &pos, output, outputSize, "Index: %d - %d\n", vertexMin, vertexMax);
            OutputStringFast(glDrawElements16, &pos, output, outputSize, "\n++++++++++++++++++++++++Index end+++++++++++++++++++++++\n");

            vertexMax ++;

            DumpVertexAttr(&pos, vertexMin, vertexMax, 0, output, outputSize);
        }
    }

_DrawElementsEnd:
    OutputStringFast(glDrawElements17, &pos, output, outputSize, "\n**********************************************************************************\n\n");
    OutputToTarget(OUTPUT_TO_DRAW_STRING, GL_FALSE, 0, NULL, output, g_pDrawFile);
}

GLvoid OutputDrawArrays_es20(GLenum mode, GLuint first, GLint count, GLchar *output, int outputSize)
{
    GLuint      progID          = CURRENT_CONTEXT1().curProgram;
    CProgramObj *program        = CURRENT_CONTEXT1().FindProgram(progID);
    GLuint      PrimitiveCnt    = 0;
    GLuint      historyID       = 0xFFFFFFF;
    GLint       pos             = 0;
    GLint       pos1            = 0;

    CURRENT_CONTEXT1().nDrawArrays ++;
    CURRENT_CONTEXT1().nDrawTotal ++;

    if (bDumpFrameFile)
    {
        OutputDrawArraysToFrameFile_es20(mode, first, count, historyID);
    }

    TranslateDrawMode(mode, tmp1);
    OutputStringFast(glDrawArrays3, &pos, output, outputSize, "\n**********************************************************************************\n");
    OutputStringFast(glDrawArrays4, &pos, output, outputSize, "  %s glDrawArrays: %08d,   Totals: %08d\n", thread, CURRENT_CONTEXT1().nDrawArrays, CURRENT_CONTEXT1().nDrawTotal);
    GetPrimitiveCount(mode, count, &PrimitiveCnt);
    OutputStringFast(glDrawArrays5, &pos, output, outputSize, "   %s, first=%04d, count=%08d, prog=%08d\n", tmp1, first, count, CURRENT_CONTEXT1().curProgram);
    OutputStates(&pos, ES20_VERSION, output, outputSize);

    OutputStringFast(glDrawArrays7, &pos, output, outputSize, "Frames: %04d\n", g_nFrames);

    OutputToTarget(OUTPUT_TO_DRAW_TOTAL_STRING, GL_FALSE, 0, NULL, output, g_pOutputFile);

    // Dump uniform
    DumpUniforms(&pos, output, outputSize);

    memset(tmp1, 0, TMP_BUF_SIZE);
    OutputStringFast(glDrawArrays8, &pos1, tmp1, TMP_BUF_SIZE, "\n**********************************************************************************\n\n");
    OutputToTarget(OUTPUT_TO_TOTAL_STRING, GL_FALSE, 0, NULL, tmp1, g_pOutputFile);

    if (IsDrawNeedDump(CURRENT_CONTEXT1().nDrawTotal) == GL_FALSE)
    {
        return;
    }

    if (program)
    {
        GLuint  vertex      = program->m_pVertex->m_oldName;
        GLuint  fragment    = program->m_pFragment->m_oldName;

        PrintShader(&pos, progID, vertex, fragment, output, outputSize);
        DumpVertexAttr(&pos, 0, count, first, output, outputSize);
    }

    OutputStringFast(glDrawArrays9, &pos, output, outputSize, "\n**********************************************************************************\n\n");
    OutputToTarget(OUTPUT_TO_DRAW_STRING, GL_FALSE, 0, NULL, output, g_pDrawFile);
}

GLvoid DumpVertexAttr(int *pos, int vertexMin, int vertexMax, int first, GLchar *output, int outputSize)
{
    GLuint          progID          = CURRENT_CONTEXT1().curProgram;
    CProgramObj     *program        = CURRENT_CONTEXT1().FindProgram(progID);
    CBufObj         *buffer         = NULL;
    GLchar          *v              = NULL;
    GLuint          addressUsed     = 0;
    CAttribObj      *attribute      = NULL;
    stVertAttrPtr   *vertex         = NULL;
    GLchar          *address[16]    = {0};

    OutputStringFast(dumpVertex1, pos, output, outputSize, "\n+++++++++++++++++++++++++Attribute begin+++++++++++++++++++++++++\n");

    for (GLuint i=0; i<program->m_attributes.size(); i++)
    {
        GLchar          *p              = NULL;
        GLchar          *vertexPointer  = NULL;
        GLint           vertexSize      = 0;
        GLenum          vertexType      = 0;
        GLboolean       normalized      = GL_FALSE;
        GLsizei         stride          = 0;
        GLint           pointer         = 0;
        GLuint          dataTypeSize    = 0;
        GLuint          lineNum         = 0;
        GLuint          columnNum       = 8;
        GLint           counter         = 1;
        GLuint          count           = (vertexMax-vertexMin+1);

        attribute   = program->m_attributes[i];
        vertex      = &CURRENT_CONTEXT1().vertexAttribPointer[attribute->nLoc];

        if (!(CURRENT_CONTEXT1().vertexAttribBits & (1 << attribute->nLoc)))
            continue;

        if (vertex->size == 0)
            continue;

        if (!vertex->vbo)
        {
            v = (GLchar*)ReadData(READ_GL_UCHAR, vertex->pointer, 0);
            if (v == NULL)
                continue;

            address[i] = v;
            addressUsed ++;
        }

        vertexSize      = vertex->size;
        vertexType      = vertex->type;
        normalized      = vertex->normalized;
        stride          = vertex->stride;
        pointer         = vertex->pointer;
        lineNum         = count/4;
        columnNum       = 4;
        counter         = vertexMin;
        dataTypeSize    = GetDataTypeSize(vertexType);

        // Use VBO
        if (vertex->vbo)
        {
            buffer = CURRENT_CONTEXT1().bufferMap[vertex->vbo];

            if (buffer->m_pData && (GLuint)pointer < buffer->m_nSize)
            {
#ifdef LINUX
                vertexPointer = (GLchar*)((long)(buffer->data) + (long)pointer + first * stride);
                p = (GLchar*)((long)vertexPointer + vertexMin * stride);
#else
                vertexPointer = (GLchar*)((GLuint)(buffer->m_pData) + (GLuint)pointer + first * stride);
                p = (GLchar*)((GLuint)vertexPointer + vertexMin * stride);
#endif
            }
            else
            {
                p = address[i];
                vertexPointer = p;
            }
        }
        else
        {
            p = address[i];
            vertexPointer = p;

            // do not use vbo. we should caculate data size based on glVertexAttribPointer and draw.
            GLuint dataSize = GetDataTypeSize(vertexType);

            dataSize *= (vertexSize*(vertexMax-vertexMin));
        }

        OutputStringFast(dumpVertex2, pos, output, outputSize, "\n+++++++++++++++++++++++++%s+++++++++++++++++++++++++\n", attribute->name);
        OutputStringFast(dumpVertex3, pos, output, outputSize, "Count: %08d\n", vertexMax);
        OutputStringFast(dumpVertex4, pos, output, outputSize, "Type: %d\n", vertexType);
        OutputStringFast(dumpVertex5, pos, output, outputSize, "Size: %d\n", vertexSize);

        if (p == NULL)
        {
            OutputStringFast(dumpVertex6, pos, output, outputSize, "Cannot get data\n");
            goto _VertexEnd;
        }

        for (GLuint j=0; j<=lineNum; j++)
        {
            OutputStringFast(dumpVertex7, pos, output, outputSize, "#%08d: ", j+1);
            for (GLuint k=0; k<columnNum; k++)
            {
                OutputStringFast(oneChar, pos, output, outputSize, "(");
                OutputData(pos, vertexType, output, outputSize, p);
                for (GLint l=1; l<vertexSize; l++)
                {
                    OutputStringFast(twoChar, pos, output, outputSize, ", ");
                    OutputData(pos, vertexType, output, outputSize, p+dataTypeSize*l);
                }
                OutputStringFast(threeChar, pos, output, outputSize, ")  ");
                p += stride;
                counter ++;

                if (counter >= vertexMax)
                {
                    OutputStringFast(oneChar, pos, output, outputSize, "\n");
                    goto _VertexEnd;
                }
            }
            OutputStringFast(oneChar, pos, output, outputSize, "\n");
        }

_VertexEnd:
        OutputStringFast(dumpVertex8, pos, output, outputSize, "+++++++++++++++++++++++++%s+++++++++++++++++++++++++\n\n", attribute->name);
    }

    OutputStringFast(dumpVertex9, pos, output, outputSize, "\n+++++++++++++++++++++++++Attribute end+++++++++++++++++++++++++\n");
}

GLboolean IsTextureSampler(const GLchar *src, const GLchar *textFunc, const GLchar *uniName)
{
    GLchar var[256];
    GLchar *pVar = NULL;

    while (src)
    {
        src = strstr(src, textFunc);
        if (src)
        {
            src += STRLEN(textFunc);
            memset(var, 0, 256);
            pVar = var;

            do
            {
                if (*src != ' ' && *src != '(')
                    break;

                src ++;
            }
            while (1);

            do
            {
                if (*src == ' ' || *src == ',')
                    break;

                *pVar++ = *src++;
            }
            while (1);

            if (uniName)
            {
                if (!strcmp(var, uniName))
                {
                    return true;
                }
            }
            else
            {
                return false;
            }
        }
    }

    return false;
}

GLenum UniformIsTextureSampler(const GLchar *name, GLenum *shaderType)
{
    GLuint      progID          = CURRENT_CONTEXT1().curProgram;
    CProgramObj *programArray   = CURRENT_CONTEXT1().FindProgram(progID);
    CShaderObj  *vertexShader   = programArray->m_pVertex;
    CShaderObj  *fragmentShader = programArray->m_pFragment;
    GLuint      i               = 0;

    if (g_bSaveShader == false)
    {
        *shaderType = 0;
        return 0;
    }

    if (vertexShader)
    {
        for (i=0; i<1; i++)
        {
            if (IsTextureSampler(vertexShader->m_source, "texture2D", name))
            {
                *shaderType = GL_VERTEX_SHADER;
                return GL_TEXTURE_2D;
            }

            if (IsTextureSampler(vertexShader->m_source, "textureCube", name))
            {
                *shaderType = GL_VERTEX_SHADER;
                return GL_TEXTURE_CUBE_MAP;
            }
        }
    }

    if (fragmentShader)
    {
        for (i=0; i<1; i++)
        {
            if (IsTextureSampler(fragmentShader->m_source, "texture2D", name))
            {
                *shaderType = GL_FRAGMENT_SHADER;
                return GL_TEXTURE_2D;
            }

            if (IsTextureSampler(fragmentShader->m_source, "textureCube", name))
            {
                *shaderType = GL_FRAGMENT_SHADER;
                return GL_TEXTURE_CUBE_MAP;
            }
        }
    }

    return 0;
}

GLboolean DumpUniform(int *pos, GLint location, GLuint programId, CUniformObj *uniform, GLchar *output, int outputSize)
{
    GLenum          dataType    = -1;
    GLuint          dataSize    = 2;
    GLboolean       bMatrix     = GL_FALSE;
    GLenum          texTarget   = 0;
    GLuint          matrixSize  = 0;
    GLuint          typeSize    = 0;
    const GLchar    *typeStr    = "DATA_TYPE_INVALID";
    GLenum          shaderType  = 0;

    if (!bPrintUniform)
    {
        return GL_FALSE;
    }

    if (!uniform || !uniform->name)
    {
        return GL_FALSE;
    }

    if (!uniform->data)
    {
        return GL_FALSE;
    }

    switch (uniform->type)
    {
    case GL_FLOAT_MAT4:
        dataType    = GL_FLOAT;
        dataSize    = 4;
        matrixSize  = 4*4;
        typeSize    = sizeof(GLfloat);
        bMatrix     = GL_TRUE;
        typeStr     = "GL_FLOAT_MAT4 ";
        break;

    case GL_FLOAT_MAT3:
        dataType    = GL_FLOAT;
        dataSize    = 3;
        matrixSize  = 3*3;
        typeSize    = sizeof(GLfloat);
        bMatrix     = GL_TRUE;
        typeStr     = "GL_FLOAT_MAT3 ";
        break;

    case GL_INT:
        dataSize    = 1;
        dataType    = GL_INT;
        typeSize    = sizeof(GLint);
        typeStr     = "GL_INT";
        break;

    case GL_FLOAT:
        dataType    = GL_FLOAT;
        dataSize    = 1;
        typeSize    = sizeof(GLfloat);
        typeStr     = "GL_FLOAT  ";
        break;

    case GL_FLOAT_VEC2:
        dataSize    = 2;
        dataType    = GL_FLOAT;
        typeSize    = sizeof(GLfloat);
        typeStr     = "GL_FLOAT_VEC2  ";
        break;

    case GL_FLOAT_VEC3:
        dataSize    = 3;
        dataType    = GL_FLOAT;
        typeSize    = sizeof(GLfloat);
        typeStr     = "GL_FLOAT_VEC3  ";
        break;

    case GL_FLOAT_VEC4:
        dataType    = GL_FLOAT;
        dataSize    = 4;
        typeSize    = sizeof(GLfloat);
        typeStr     = "GL_FLOAT_VEC4  ";
        break;

    default:
        break;
    }

    OutputStringFast(dumpUniform4, pos, output, outputSize, "\n--------------------%s-------------------\n", uniform->name);
    OutputStringFast(dumpUniform5, pos, output, outputSize, "Type:  %s\n", typeStr);
    OutputStringFast(dumpUniform6, pos, output, outputSize, "Count: %03d\n", uniform->count);
    if (bMatrix)
    {
        GLvoid  *data = uniform->data;

        for (GLuint i=0; i<uniform->count; i++)
        {
            OutputStringFast(dumpUniform7, pos, output, outputSize, "   %03d:\n", i);
            DumpMatrix(pos, dataType, dataSize, dataSize, data, output, outputSize);
#ifdef LINUX
            data = (GLvoid*)((long)data + matrixSize*typeSize);
#else
            data = (GLvoid*)((GLuint)data + matrixSize*typeSize);
#endif
        }
    }
    else
    {
        GLvoid  *data = uniform->data;

        for (GLuint i=0; i<uniform->count; i++)
        {
            OutputStringFast(dumpUniform8, pos, output, outputSize, "   %03d:\n", i);
            DumpVector(pos, dataType, dataSize, data, output, outputSize);
#ifdef LINUX
            data = (GLvoid*)((long)data + dataSize*typeSize);
#else
            data = (GLvoid*)((GLuint)data + dataSize*typeSize);
#endif
        }
    }

    // Check if uniform variable is texture sampler
    texTarget = UniformIsTextureSampler(uniform->name, &shaderType);
    if (texTarget != 0)
    {
        GLint       unit        = *(int*)uniform->data;
        CTexObj     *pTexObj    = CURRENT_CONTEXT1().GetTexObjByUnit(unit+GL_TEXTURE0, texTarget);
        stTexImage  *pTexImage  = pTexObj->GetTexImageByLevel(texTarget, 0);

        if (unit < API_TEX_UNIT_NUM && pTexImage)
        {
            OutputStringFast(dumpUniform9, pos, output, outputSize, "   tex: %s\n", pTexImage->texName);
        }
        else
        {
            OutputStringFast(dumpUniform9, pos, output, outputSize, "   tex: null\n");
        }
    }

    OutputStringFast(dumpUniform11, pos, output, outputSize, "--------------------%s-------------------\n", uniform->name);

    return GL_TRUE;
}

GLvoid DumpUniforms(int *pos, GLchar *output, int outputSize)
{
    GLuint      progID      = CURRENT_CONTEXT1().curProgram;
    CProgramObj *program    = CURRENT_CONTEXT1().FindProgram(progID);
    GLuint      uniformSize = program->m_uniforms.size();

    if (!bPrintUniform)
    {
        return;
    }

    OutputStringFast(dumpUniform1, pos, output, outputSize, "\n--------------------begin--------------------\n");
    OutputStringFast(dumpUniform2, pos, output, outputSize, "Uniform: %d\n", program->m_uniforms.size());
    for (GLuint i=0; i<uniformSize; i++)
    {
        CUniformObj *uniform = program->m_uniforms[i];

        if (!DumpUniform(pos, i, progID, uniform, output, outputSize))
        {
        }
    }
    OutputStringFast(dumpUniform3, pos, output, outputSize, "\n--------------------end----------------------\n");
}

GLvoid SaveShaderToFile(GLuint shaderIndex, GLenum shaderType, GLuint shaderLine, GLchar **shaderSource)
{
    const GLchar    *suffix     = NULL;
    FILE            *pFile      = NULL;
    CShaderObj      *pShader    = CURRENT_CONTEXT1().FindShader(shaderIndex);
    GLchar          shaderName[128];

    memset(shaderName, 0, 128);

    if (shaderType == GL_VERTEX_SHADER)
    {
        suffix = "vert";
    }
    else
    {
        suffix = "frag";
    }

    sprintf(shaderName, "%s%s/shader%d.%s", strOnlySourcePath, RESOURCE_PATH, shaderIndex, suffix);
    memcpy(pShader->m_strFileName, shaderName, 128);

    if (IsFileExist(shaderName))
    {
        return;
    }

    pFile = fopen(shaderName, "w");
    if (pFile == NULL)
    {
        printf("DumpShaderFile: cannot open %s\n", shaderName);
    }

    for (GLuint i=0; i<shaderLine; i++)
    {
        OutputToTarget(OUTPUT_TO_FILE, GL_FALSE, 0, NULL, shaderSource[i], pFile);
        OutputToTarget(OUTPUT_TO_FILE, GL_FALSE, 0, NULL, "\n", pFile);
    }

    if (pFile)
    {
        fclose(pFile);
    }
}

GLvoid OutputVertexAttribToFrameFile(GLuint index)
{
    stVertAttrPtr *vertex = &(CURRENT_CONTEXT1().vertexAttribPointer[index]);
    GLchar buf[256];

    memset(buf, 0, 256);
    TranslateDataType(vertex->type, tmp);
    sprintf(buf, "glVertexAttribPointer(index=%02d, stride=%06d, pointer=0x%08X, buffer=%04d, type=%s\n",
        index, vertex->stride, vertex->pointer, CURRENT_CONTEXT1().arrayBuffer, tmp);
    OutputToTarget(OUTPUT_TO_FILE, GL_FALSE, 0, NULL, buf, g_pFrameFile);
}

GLvoid OutputFBOAttachment(GLenum type, GLuint name, const GLchar *attachName, GLchar *output, GLint outputSize)
{
    const GLchar        *targetName = NULL;
    GLboolean           bTex        = GL_TRUE;
    CTexObj             *pTex       = NULL;
    CRenderbufferObj    *pRbo       = NULL;
    GLenum              format      = 0;
    GLsizei             width       = 0;
    GLsizei             height      = 0;

    switch (type)
    {
    case GL_TEXTURE_2D:
        targetName = "tex2d";
        break;

    case GL_TEXTURE_CUBE_MAP_POSITIVE_X:
        targetName = "texCube_px";
        break;

    case GL_TEXTURE_CUBE_MAP_NEGATIVE_X:
        targetName = "texCube_nx";
        break;

    case GL_TEXTURE_CUBE_MAP_POSITIVE_Y:
        targetName = "texCube_py";
        break;

    case GL_TEXTURE_CUBE_MAP_NEGATIVE_Y:
        targetName = "texCube_ny";
        break;

    case GL_TEXTURE_CUBE_MAP_POSITIVE_Z:
        targetName = "texCube_pz";
        break;

    case GL_TEXTURE_CUBE_MAP_NEGATIVE_Z:
        targetName = "texCube_nz";
        break;

    case GL_RENDERBUFFER:
        targetName = "rbo";
        bTex = GL_FALSE;
        break;

    default:
        targetName = "Invalid";
        bTex = GL_FALSE;
        break;
    }

    if (bTex)
    {
        pTex    = CURRENT_CONTEXT1().GetTexObjByName(name);
        format  = pTex->GetTexFormat();
        width   = pTex->GetWidth(0);
        height  = pTex->GetHeight(0);

        TranslateTexFormat(format, tmp1);
        OutputStrcat(output, outputSize, "%s: %s(%04d, %dx%d, %s)  ", attachName, targetName, name, width, height, tmp1);
    }
    else
    {
        pRbo = CURRENT_CONTEXT1().GetRBOByName(name);
        format = pRbo->GetFormat();
        TranslateTexFormat(format, tmp1);
        OutputStrcat(output, outputSize, "%s: %s(%04d, %s)  ", attachName, targetName, name, tmp1);
    }
}

GLvoid OutputCurrentFBO(GLint *pos, GLchar *output, GLint outputSize)
{
    CFramebufferObj *pFBO = CURRENT_CONTEXT1().GetCurrentFBO();
    GLchar buf[128];

    if (pFBO)
    {
        OutputStringFast(drawStates34, pos, output, outputSize, "   FBO: %04d\n", pFBO->name);
        if (pFBO->color0 != 0)
        {
            memset(buf, 0, 128);
            OutputFBOAttachment(pFBO->color0Type, pFBO->color0, "        color  ", buf, 128);
            OutputStringFast(drawStates35, pos, output, outputSize, "%s\n", buf);
        }

        if (pFBO->depth != 0)
        {
            memset(buf, 0, 128);
            OutputFBOAttachment(pFBO->depthType, pFBO->depth, "        depth  ", buf, 128);
            OutputStringFast(drawStates36, pos, output, outputSize, "%s\n", buf);
        }

        if (pFBO->stencil != 0)
        {
            memset(buf, 0, 128);
            OutputFBOAttachment(pFBO->stencilType, pFBO->stencil, "        stencil", buf, 128);
            OutputStringFast(drawStates37, pos, output, outputSize, "%s\n", buf);
        }
    }
    else // Using RT
    {
        OutputStringFast(drawStates34, pos, output, outputSize, "   FBO: 0000  \n");
    }
}

GLvoid OutputFBO(GLuint currentFBO, GLchar *output, GLint outputSize)
{
    CFramebufferObj *pFBO = CURRENT_CONTEXT1().GetCurrentFBO();

    sprintf(output, "glBindFramebuffer(fbo=%04d)  ", currentFBO);

    // Using FBO
    if (pFBO)
    {
        if (pFBO->color0 != 0)
        {
            OutputFBOAttachment(pFBO->color0Type, pFBO->color0, "color", output, outputSize);
        }

        if (pFBO->depth != 0)
        {
            OutputFBOAttachment(pFBO->depthType, pFBO->depth, "depth", output, outputSize);
        }

        if (pFBO->stencil != 0)
        {
            OutputFBOAttachment(pFBO->stencilType, pFBO->stencil, "stencil", output, outputSize);;
        }
    }
    else // Using RT
    {

    }

    OutputStrcat(output, outputSize, "\n");
}