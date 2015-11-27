#include "utils.h"
#include "GLESShare.h"
#include "GLSLShader.h"
#include "ApiGLES3Context.h"
#include "ApiEGLContext.h"
#include "gles3.h"

int CharNumInString(GLchar *str, GLuint srcLen, GLchar c)
{
    GLuint  num  = 0;

    for (GLuint i=0; i<srcLen; i++)
    {
        if (str[i] == c)
        {
            num ++;
        }
    }

    return num;
}

GLvoid AddTabToString(GLchar **str, GLuint nTab)
{
    for (GLuint i=0; i<nTab; i++)
    {
        **str = (GLchar)'\t';
        (*str)++;
    }
}

GLvoid IndentShaderCode(GLchar* src, GLuint srcLen, GLchar** dst)
{
    static GLuint   nLastTab    = 0;
    GLuint          nTab        = 0;
    GLuint          nLine       = 0;
    GLchar          *indent     = NULL;
    GLboolean       bNewLine    = GL_TRUE;
    GLchar          *indentOrig = NULL;

    // Get the line number of src
    nLine += CharNumInString(src, srcLen, (GLchar)'\n');
    nLine += CharNumInString(src, srcLen, (GLchar)'{');
    nLine += CharNumInString(src, srcLen, (GLchar)'}');
    nLine += CharNumInString(src, srcLen, (GLchar)';');

    // Get the tab number needed by indent
    for (GLuint i=0; i<srcLen; i++)
    {
        if (src[i] == '\n')
            nTab += nLastTab;

        if (src[i] == '{')
        {
            nTab += nLastTab;
            nLastTab ++;
        }

        if (src[i] == '}')
        {
            nLastTab --;
            nTab += nLastTab;
        }

        if (src[i] == ';')
        {
            nTab += nLastTab;
        }
    }

    // Allocate dst memory
    // size = TABs + RETURNs + source string length + terminator('\0')
    indent      = (GLchar*)malloc(nTab+nLine+srcLen+1);
    indentOrig  = indent;

    for (GLuint i=0; i<srcLen; i++)
    {
        if (bNewLine)
        {
            if (src[i] == '\t' || src[i] == ' ')
            {
                continue;
            }

            if (src[i] == '\n')
            {
                continue;
            }
            else if (src[i] == 0xD && src[i+1] == 0xA) // UNIX \r\n
            {
                i ++;
                continue;
            }
            else if (src[i] == '{')
            {
                AddTabToString(&indent, nLastTab);

                *indent++ = src[i];
                *indent++ = (GLchar)'\n';

                nLastTab ++;
                bNewLine = GL_TRUE;
            }
            else if ((src[i] == '/') &&
                     (src[i+1] == '/'))
            {
                while (src[i] != '\n')
                {
                    if (src[i] == 0xD) // unix /r
                    {
                        i ++;
                        continue;
                    }

                    *indent++ = src[i];
                    i ++;
                }
                *indent++ = src[i];
            }
            else if (src[i] == '}')
            {
                nLastTab --;
                bNewLine = GL_TRUE;

                AddTabToString(&indent, nLastTab);
                *indent++ = src[i];
                *indent++ = (GLchar)'\n';
            }
            else if (src[i] == ';')
            {
                bNewLine = GL_TRUE;
                *indent++ = src[i];
                *indent++ = (GLchar)'\n';
            }
            else if (!memcmp(&src[i], "#define", 7))
            {
                while (src[i] != '\n')
                {
                    if (src[i] == 0xD) // unix /r
                    {
                        i ++;
                        continue;
                    }

                    *indent++ = src[i];
                    i ++;
                }
                *indent++ = src[i];
            }
            else
            {
                AddTabToString(&indent, nLastTab);
                *indent++ = src[i];
                bNewLine = GL_FALSE;
            }
        }
        else
        {
            if (src[i] == (GLchar)'\n')
            {
                *indent++ = (GLchar)'\n';
                bNewLine = GL_TRUE;
                continue;
            }
            else if (src[i] == 0xD && src[i+1] == 0xA) // UNIX \r\n
            {
                i ++;
                *indent++ = (GLchar)'\n';
                bNewLine = GL_TRUE;
                continue;
            }
            else if (src[i] == '{')
            {
                *indent++ = (GLchar)'\n';
                AddTabToString(&indent, nLastTab);

                *indent++ = src[i];
                *indent++ = (GLchar)'\n';

                nLastTab ++;
                bNewLine = GL_TRUE;
            }
            else if (src[i] == '}')
            {
                nLastTab --;
                bNewLine = GL_TRUE;

                AddTabToString(&indent, nLastTab);
                *indent++ = src[i];
                *indent++ = (GLchar)'\n';
            }
            else if (src[i] == ';' && src[i+1] != (GLchar)'\n')
            {
                bNewLine = GL_TRUE;
                *indent++ = src[i];
                *indent++ = (GLchar)'\n';
            }
            else
            {
                *indent++ = src[i];
            }
        }
    }

    *indent++ = (GLchar)'\0';
    *dst = indentOrig;
}

GLuint GetShaderDataSizeByType(GLenum shaderType)
{
    GLuint dataSize    = 0;

    switch (shaderType)
    {
    case GL_FLOAT:
        dataSize    = 4;
        break;

    case GL_FLOAT_VEC2:
        dataSize    = 8;
        break;

    case GL_FLOAT_VEC3:
        dataSize    = 12;
        break;

    case GL_FLOAT_VEC4:
        dataSize    = 16;
        break;

    case GL_INT:
        dataSize    = 4;
        break;

    case GL_INT_VEC2:
        dataSize    = 8;
        break;

    case GL_INT_VEC3:
        dataSize    = 12;
        break;

    case GL_INT_VEC4:
        dataSize    = 16;
        break;

    case GL_BOOL:
        dataSize    = 4;
        break;

    case GL_BOOL_VEC2:
        dataSize    = 8;
        break;

    case GL_BOOL_VEC3:
        dataSize    = 12;
        break;

    case GL_BOOL_VEC4:
        dataSize    = 16;
        break;

    case GL_FLOAT_MAT2:
        dataSize    = 4*4;
        break;

    case GL_FLOAT_MAT3:
        dataSize    = 9*4;
        break;

    case GL_FLOAT_MAT4:
        dataSize    = 16*4;
        break;

    case GL_SAMPLER_2D:
        dataSize    = 4;
        break;

    case GL_SAMPLER_CUBE:
        dataSize    = 4;
        break;

    default:
        break;
    }

    return dataSize;
}

GLvoid CGLES3Context::DeleteShaderObject(GLuint shaderId)
{
    CShaderObj  *pShader = FindShader(shaderId);

    if (pShader)
    {
        shaderMap.erase(shaderId);
        delete (pShader);
    }
}

GLvoid CGLES3Context::CreateShaderObject(GLenum type, GLuint shaderId)
{
    CShaderObj *pShader = FindShader(shaderId);

    if (pShader)
    {
        pShader->m_type = type;
        return;
    }

    pShader = new CShaderObj(shaderId, type);
    if (pShader)
    {
        if (type == GL_VERTEX_SHADER)
        {
            pShader->m_type = GL_VERTEX_SHADER;
        }
        else
        {
            pShader->m_type = GL_FRAGMENT_SHADER;
        }

        shaderMap.insert(std::make_pair(shaderId, pShader));
    }
    else
    {
        Abort("Cannot allocate shader object\n");
    }
}

GLvoid CGLES3Context::ClearProgram(CProgramObj *pProgram)
{
    GLint   j           = 0;
    GLint   size        = (GLint)pProgram->m_uniforms.size();
    GLint   attrSize    = (GLint)pProgram->m_attributes.size();

    pProgram->m_bLinked   = GL_FALSE;

    for (j=0; j<size; j++)
    {
        CUniformObj *pUniform = pProgram->m_uniforms[j];
        delete (pUniform);
    }
    pProgram->m_uniforms.clear();

    for (j=0; j<attrSize; j++)
    {
        CAttribObj *pAttr = pProgram->m_attributes[j];
        delete pAttr;
    }
    pProgram->m_attributes.clear();

    if (pProgram->m_name)
    {
        g_opengl->glDeleteProgram(pProgram->m_name);
        g_opengl->glDeleteShader(pProgram->m_pVertex->m_name);
        g_opengl->glDeleteShader(pProgram->m_pFragment->m_name);

        pProgram->m_name                = 0;
        pProgram->m_pVertex->m_name     = 0;
        pProgram->m_pFragment->m_name   = 0;
    }
}

GLvoid CGLES3Context::CreateProgramObject(GLuint progId)
{
    CProgramObj *pProgram = FindProgram(progId);

    if (pProgram)
    {
        ClearProgram(pProgram);
        return;
    }

    pProgram = new CProgramObj(progId);
    if (pProgram)
    {
        programMap.insert(std::make_pair(progId, pProgram));
    }
    else
    {
        Abort("Cannot allocate program object.\n");
    }
}

GLvoid CGLES3Context::DeleteProgramObject(GLuint progId)
{
    CProgramObj *p          = FindProgram(progId);
    GLint       j           = 0;
    GLint       size        = (GLint)(p?p->m_uniforms.size():0);
    GLint       attrSize    = (GLint)(p?p->m_attributes.size():0);
    CUniformObj *pUniform   = NULL;
    CAttribObj  *pAttr      = NULL;

    if (!p)
        return;

    programMap.erase(progId);
    delete p;
}


GLint GetShaderDataType(GLchar *typeName, eShaderType *dataType)
{
    GLuint      isType = 1;
    eShaderType type = DATA_TYPE_INVALID;

    if (typeName == NULL)
    {
        isType = 0;
        goto _End;
    }

    if (!strcmp(typeName, "sampler2D"))
    {
        type = DATA_TYPE_SAMPLER_2D;
    }
    else if (!strcmp(typeName, "samplerCube"))
    {
        type = DATA_TYPE_SAMPLER_CUBIC;
    }
    else if (!strcmp(typeName, "mat2"))
    {
        type = DATA_TYPE_FLOAT_2X2;
    }
    else if (!strcmp(typeName, "mat3"))
    {
        type = DATA_TYPE_FLOAT_3X3;
    }
    else if (!strcmp(typeName, "mat4"))
    {
        type = DATA_TYPE_FLOAT_4X4;
    }
    else if (!strcmp(typeName, "vec2"))
    {
        type = DATA_TYPE_FLOAT_X2;
    }
    else if (!strcmp(typeName, "vec3"))
    {
        type = DATA_TYPE_FLOAT_X3;
    }
    else if (!strcmp(typeName, "vec4"))
    {
        type = DATA_TYPE_FLOAT_X4;
    }
    else if (!strcmp(typeName, "float"))
    {
        type = DATA_TYPE_FLOAT_X1;
    }
    else if (!strcmp(typeName, "int"))
    {
        type = DATA_TYPE_INTEGER_X1;
    }
    else if (!strcmp(typeName, "bool"))
    {
        type = DATA_TYPE_BOOLEAN_X1;
    }
    else if (!strcmp(typeName, "ivec2"))
    {
        type = DATA_TYPE_INTEGER_X2;
    }
    else if (!strcmp(typeName, "ivec3"))
    {
        type = DATA_TYPE_INTEGER_X3;
    }
    else if (!strcmp(typeName, "ivec4"))
    {
        type = DATA_TYPE_INTEGER_X4;
    }
    else if (!strcmp(typeName, "bvec2"))
    {
        type = DATA_TYPE_BOOLEAN_X2;
    }
    else if (!strcmp(typeName, "bvec3"))
    {
        type = DATA_TYPE_BOOLEAN_X3;
    }
    else if (!strcmp(typeName, "bvec4"))
    {
        type = DATA_TYPE_BOOLEAN_X4;
    }
    else
    {
        isType = 0;
    }

_End:
    if (dataType)
    {
        *dataType = type;
    }

    return isType;
}

GLchar* GetVariableNameFromShader(GLchar *source, GLchar *type, GLuint *pLen, eShaderType *varType)
{
    GLchar      buf[256];
    GLuint      len         = *pLen;
    GLuint      nameLen     = 0;
    GLchar      *name       = NULL;
    GLchar      *pSrc       = (GLchar*)(source + len);
    GLuint      stack       = 0;
    GLuint      bFound      = 0;
    eShaderType dataType    = DATA_TYPE_INVALID;

    memset(buf, 0, 256);
    while (*pSrc != '\0')
    {
        if (*pSrc != '\n'   &&
            *pSrc != ' '    &&
            *pSrc != '\t'   &&
            *pSrc != ';'    &&
            *pSrc != '{'    &&
            *pSrc != '}'    &&
            *pSrc != '('    &&
            *pSrc != ')')
        {
            buf[stack++] = *pSrc;
        }
        else
        {
            if (*pSrc == '\n'   ||
                *pSrc == ' '    ||
                *pSrc == '\t')
            {
                memset(buf, 0, 256); stack = 0;
            }
        }

        // Find uniform
        if (!strcmp(buf, type))
        {
            memset(buf, 0, 256); stack = 0;
            bFound = 1;
        }

        if (bFound)
        {
            if (!strcmp(buf, "highp") ||
                !strcmp(buf, "mediump") ||
                !strcmp(buf, "mediump"))
            {
                memset(buf, 0, 256); stack = 0;
            }

            if (GetShaderDataType(buf, &dataType))
            {
                memset(buf, 0, 256); stack = 0;
                *varType = dataType;
            }

            if (*pSrc == ';')
            {
                GLuint  k = 0;

                nameLen = (GLuint)strlen(buf) + 1;
                name = (GLchar*)malloc(nameLen);
                memset(name, 0, nameLen);

                for (GLuint j=0; j<nameLen; j++)
                {
                    if (buf[j] == '[')
                    {
                        break;
                    }

                    if (buf[j] != ' ' && buf[j] != '\t' && buf[j] != '\n')
                    {
                        name[k++] = buf[j];
                    }
                }

                break;
            }
        }

        pSrc ++;
        len ++;
    }

    *pLen = len;

    return name;
}

GLvoid CProgramObj::AnalyzeUniform()
{
    CUniformObj *pUniform       = NULL;
    GLchar      *pUniformName   = NULL;
    GLuint      progName        = m_name;
    GLint       activeUniforms  = 0;
    GLint       maxUniformLen   = 0;

    g_opengl->glGetProgramiv(progName, GL_ACTIVE_UNIFORMS, &activeUniforms);
    g_opengl->glGetProgramiv(progName, GL_ACTIVE_UNIFORM_MAX_LENGTH, &maxUniformLen);

    for (GLint i=0; i<activeUniforms; i++)
    {
        GLsizei len     = 0;
        GLint   size    = 0;
        GLenum  type    = 0;
        GLint   loc     = -1;

        pUniformName = (GLchar*)malloc(maxUniformLen);
        memset(pUniformName, 0, maxUniformLen);
        g_opengl->glGetActiveUniform(progName, i, maxUniformLen, &len, &size, &type, pUniformName);
        loc = g_opengl->glGetUniformLocation(progName, pUniformName);

        pUniform = new CUniformObj;
        pUniform->type      = type;
        pUniform->size      = size;
        pUniform->nOldLoc   = loc;
        pUniform->nLoc      = loc;
        pUniform->name      = pUniformName;

        m_uniforms.push_back(pUniform);
    }
}

GLvoid CProgramObj::AnalyzeAttribute()
{
    GLchar      *name               = NULL;
    CAttribObj  *pAttr              = NULL;
    GLint       activeAttributes    = 0;
    GLint       maxAttributeLen     = 0;
    GLuint      progName            = m_name;

    if (!m_bLinked)
    {
        return;
    }

    g_opengl->glGetProgramiv(progName, GL_ACTIVE_ATTRIBUTES, &activeAttributes);
    g_opengl->glGetProgramiv(progName, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &maxAttributeLen);

    for (GLint i=0; i<activeAttributes; i++)
    {
        GLsizei len         = 0;
        GLint   size        = 0;
        GLenum  type        = 0;
        GLint   loc         = -1;
        GLint   norm        = -1;
        GLint   stride      = 0;
        GLint   vertexType  = 0;

        name = (GLchar*)malloc(maxAttributeLen);
        memset(name, 0, maxAttributeLen);
        g_opengl->glGetActiveAttrib(progName, i, maxAttributeLen, &len, &size, &type, name);
        loc = g_opengl->glGetAttribLocation(progName, name);

        g_opengl->glGetVertexAttribiv(loc, GL_VERTEX_ATTRIB_ARRAY_TYPE, &vertexType);
        g_opengl->glGetVertexAttribiv(loc, GL_VERTEX_ATTRIB_ARRAY_STRIDE, &stride);
        g_opengl->glGetVertexAttribiv(loc, GL_VERTEX_ATTRIB_ARRAY_NORMALIZED, &norm);
        g_opengl->glGetVertexAttribiv(loc, GL_VERTEX_ATTRIB_ARRAY_SIZE, &size);

        pAttr = FindAttributeByName(name);
        if (pAttr == NULL)
        {
            pAttr = new CAttribObj;
            pAttr->name = name;
            m_attributes.push_back(pAttr);
        }
        else
        {
            free(name);
        }

        pAttr->type         = type;
        pAttr->size         = size;
        pAttr->stride       = stride;
        pAttr->normalized   = norm;
        pAttr->nLoc         = loc;
    }
}

GLboolean CGLES3Context::LinkProgram(CProgramObj *pProgram)
{
    CShaderObj  *vertex     = pProgram->m_pVertex;
    CShaderObj  *fragment   = pProgram->m_pFragment;
    GLint       status      = 0;
    GLint       len         = 0;
    GLuint      size        = (GLuint)pProgram->m_attributes.size();
    GLchar      infoLog[256];

    if (vertex == NULL || fragment == NULL)
    {
        return GL_FALSE;
    }

    if (pProgram->m_bLinked == GL_TRUE)
    {
        return GL_TRUE;
    }

    // Bind attribute location
    for (GLuint i=0; i<size; i++)
    {
        CAttribObj *pAttr = pProgram->m_attributes[i];
        g_opengl->glBindAttribLocation(pProgram->m_name, pAttr->nLoc, pAttr->name);
    }

    g_opengl->glAttachShader(pProgram->m_name, pProgram->m_pVertex->m_name);
    g_opengl->glAttachShader(pProgram->m_name, pProgram->m_pFragment->m_name);
    g_opengl->glLinkProgram(pProgram->m_name);
    g_opengl->glGetProgramiv(pProgram->m_name, GL_LINK_STATUS, &status);
    if (status == 0)
    {
        memset(infoLog, 0, 256);
        g_opengl->glGetProgramInfoLog(pProgram->m_name, 256, &len, infoLog);
        printf("Link failure on program(%d)", pProgram->m_name);
        printf("Info: %s", infoLog);
        pProgram->m_bLinked = GL_FALSE;

        g_opengl->glDeleteProgram(pProgram->m_name);
        g_opengl->glDeleteShader(pProgram->m_pVertex->m_name);
        g_opengl->glDeleteShader(pProgram->m_pFragment->m_name);
        return GL_FALSE;
    }

    pProgram->m_bLinked = GL_TRUE;

    pProgram->AnalyzeUniform();
    pProgram->AnalyzeAttribute();

    return GL_TRUE;
}

eShaderType ConvertGLSLDataTypeToInternType(GLenum type)
{
    switch (type)
    {
    case GL_FLOAT_MAT4:
        return DATA_TYPE_FLOAT_4X4;

    case GL_FLOAT_MAT3:
        return DATA_TYPE_FLOAT_3X3;

    case GL_FLOAT_MAT2:
        return DATA_TYPE_FLOAT_2X2;

    case GL_FLOAT_VEC4:
        return DATA_TYPE_FLOAT_X4;

    case GL_FLOAT_VEC3:
        return DATA_TYPE_FLOAT_X3;

    case GL_FLOAT_VEC2:
        return DATA_TYPE_FLOAT_X2;

    case GL_INT_VEC4:
        return DATA_TYPE_INTEGER_X4;

    case GL_INT_VEC3:
        return DATA_TYPE_INTEGER_X3;

    case GL_INT_VEC2:
        return DATA_TYPE_INTEGER_X2;

    case GL_BOOL_VEC4:
        return DATA_TYPE_BOOLEAN_X4;

    case GL_BOOL_VEC3:
        return DATA_TYPE_BOOLEAN_X3;

    case GL_BOOL_VEC2:
        return DATA_TYPE_BOOLEAN_X2;

    case GL_SAMPLER_2D:
        return DATA_TYPE_SAMPLER_2D;

    case GL_SAMPLER_CUBE:
        return DATA_TYPE_SAMPLER_CUBIC;

    case GL_FLOAT:
        return DATA_TYPE_FLOAT_X1;

    case GL_INT:
        return DATA_TYPE_INTEGER_X1;

    default:
        return DATA_TYPE_INVALID;
    }
}

GLuint GetSizeFromShaderDataType(eShaderType type)
{
    GLuint size = 0;

    switch (type)
    {
    case DATA_TYPE_FLOAT_X1: size = 4; break;
    case DATA_TYPE_FLOAT_X2: size = 8; break;
    case DATA_TYPE_FLOAT_X3: size = 12; break;
    case DATA_TYPE_FLOAT_X4: size = 16; break;
    case DATA_TYPE_FLOAT_2X2: size = 16; break;
    case DATA_TYPE_FLOAT_3X3: size = 36; break;
    case DATA_TYPE_FLOAT_4X4: size = 64; break;
    case DATA_TYPE_BOOLEAN_X1: size = 4; break;
    case DATA_TYPE_BOOLEAN_X2: size = 8; break;
    case DATA_TYPE_BOOLEAN_X3: size = 12; break;
    case DATA_TYPE_BOOLEAN_X4: size = 16; break;
    case DATA_TYPE_INTEGER_X1: size = 4; break;
    case DATA_TYPE_INTEGER_X2: size = 8; break;
    case DATA_TYPE_INTEGER_X3: size = 12; break;
    case DATA_TYPE_INTEGER_X4: size = 16; break;
    case DATA_TYPE_SAMPLER_1D: size = 4; break;
    case DATA_TYPE_SAMPLER_2D: size = 8; break;
    case DATA_TYPE_SAMPLER_3D: size = 12; break;
    case DATA_TYPE_SAMPLER_CUBIC: size = 4; break;
    case DATA_TYPE_FIXED_X1: size = 4; break;
    case DATA_TYPE_FIXED_X2: size = 8; break;
    case DATA_TYPE_FIXED_X3: size = 12; break;
    case DATA_TYPE_FIXED_X4: size = 16; break;
    case DATA_TYPE_IMAGE_2D: size = 8; break;
    case DATA_TYPE_IMAGE_3D: size = 12; break;
    case DATA_TYPE_SAMPLER: size = 4; break;
    case DATA_TYPE_FLOAT_2X3: size = 24; break;
    case DATA_TYPE_FLOAT_2X4: size = 32; break;
    case DATA_TYPE_FLOAT_3X2: size = 24; break;
    case DATA_TYPE_FLOAT_3X4: size = 48; break;
    case DATA_TYPE_FLOAT_4X2: size = 32; break;
    case DATA_TYPE_FLOAT_4X3: size = 48; break;
    case DATA_TYPE_ISAMPLER_2D: size = 8; break;
    case DATA_TYPE_ISAMPLER_3D: size = 12; break;
    case DATA_TYPE_ISAMPLER_CUBIC: size = 4; break;
    case DATA_TYPE_USAMPLER_2D: size = 8; break;
    case DATA_TYPE_USAMPLER_3D: size = 12; break;
    case DATA_TYPE_USAMPLER_CUBIC: size = 4; break;
    case DATA_TYPE_SAMPLER_EXTERNAL_OES: size = 4; break;
    case DATA_TYPE_INVALID: size = 0; break;
    default: size = 0; break;
    }

    return size;
}

CAttribObj* CProgramObj::FindAttributeByName(const GLchar *name)
{
    CAttribObj*                 attribute = NULL;
    AttributeArray::iterator    it;
    GLuint                      size = (GLuint)m_attributes.size();

    for (GLuint i=0; i<size; i++)
    {
        attribute = m_attributes[i];
        if (attribute->name && !STRCMP(attribute->name, name))
        {
            return attribute;
        }
    }

    return NULL;
}

CAttribObj* CProgramObj::FindAttribObjByIndex(GLuint index)
{
    for (GLuint i=0; i<m_attributes.size(); i++)
    {
        if (m_attributes[i]->nOldLoc == index)
            return m_attributes[i];
    }

    return NULL;
}

GLvoid PrintShader(int *pos, GLuint progID, GLuint vertex, GLuint fragment, GLchar *output, int outputSize)
{
    CShaderObj  *vertShader = CURRENT_CONTEXT1()->FindShader(vertex);
    CShaderObj  *fragShader = CURRENT_CONTEXT1()->FindShader(fragment);
    GLuint      vertexCnt   = vertShader?1:0;
    GLuint      fragmentCnt = fragShader?1:0;

    OutputStringFast(printShader1, pos, output, outputSize, "\n\nProgram: %08d, Vertex: %08d, Fragment: %08d\n", progID, vertex, fragment);

    if (bPrintDrawShader)
    {
        OutputStringFast(printShader2, pos, output, outputSize, "\n--------------------vertex begin-------------------\n");
        for (unsigned int i=0; i<vertexCnt; i++)
        {
            OutputStringFast(printShader3, pos, output, outputSize, "%s", vertShader->m_source);
        }
        OutputStringFast(printShader4, pos, output, outputSize, "\n---------------------vertex end---------------------\n");

        OutputStringFast(printShader5, pos, output, outputSize, "\n-------------------fragment begin------------------\n");
        for (unsigned int i=0; i<fragmentCnt; i++)
        {
            OutputStringFast(printShader6, pos, output, outputSize, "%s", fragShader->m_source);
        }
        OutputStringFast(printShader7, pos, output, outputSize, "\n---------------------fragment end---------------------\n");
    }
}

GLvoid CGLES3Context::ApiBindAttribLocation(GLuint program, GLint loc, const GLchar *name)
{
    if ((loc >= 0) && (loc < API_VERTEX_ATTRIBUTES_NUM))
    {
        CProgramObj *pProg  = FindProgram(program);
        CAttribObj  *pAttr  = NULL;
        GLint       len     = (int)strlen(name) + 1;

        if (pProg->m_bLinked)
        {
            pAttr = pProg->FindAttributeByName(name);
        }
        else
        {
            pAttr = new CAttribObj;

            pAttr->name = (GLchar*)malloc(len);
            memset(pAttr->name, 0, len);
            strcpy(pAttr->name, name);

            pProg->m_attributes.push_back(pAttr);
        }

        pAttr->nLoc     = loc;
        pAttr->nOldLoc  = loc;
    }

    CTX_ANALYZER_FUNC3(BindAttribLocation, GLOutput, GL_OUT_BUF_SIZE, program, loc, name);
}

CProgramObj* CGLES3Context::FindProgram(GLuint program)
{
    ProgramMap::iterator it = programMap.find(program);

    if (it != programMap.end())
    {
        return it->second;
    }
    else
    {
        return NULL;
    }
}

CShaderObj* CGLES3Context::FindShader(GLuint shader)
{
    ShaderMap::iterator it = shaderMap.find(shader);

    if (it != shaderMap.end())
    {
        return it->second;
    }
    else
    {
        return NULL;
    }
}

GLvoid CGLES3Context::ApiAttachShader(GLuint program, GLuint shader)
{
    CProgramObj *pProgram   = FindProgram(program);
    CShaderObj  *pShader    = FindShader(shader);
    GLenum      shaderType  = pShader?pShader->m_type:GL_FRAGMENT_SHADER;

    if (shaderType == GL_FRAGMENT_SHADER)
    {
        pProgram->m_pFragment = pShader;
    }
    else
    {
        pProgram->m_pVertex = pShader;
    }

    CTX_ANALYZER_FUNC2(AttachShader, GLOutput, GL_OUT_BUF_SIZE, program, shader);
}

GLint CGLES3Context::ApiGetUniformLocation(GLuint program, const GLchar *name, GLint oldLoc)
{
    CProgramObj *prog       = CURRENT_CONTEXT1()->FindProgram(program);
    CUniformObj *uniform    = prog->GetUniformByName(name);

    if (uniform == NULL)
    {
        goto _End;
    }

    uniform->nOldLoc = oldLoc;
    uniform->nLoc    = g_opengl->glGetUniformLocation(prog->m_name, name);

_End:
    CTX_ANALYZER_FUNC3(GetUniformLocation, GLOutput, GL_OUT_BUF_SIZE, program, name, oldLoc);
    return oldLoc;
}

GLvoid CGLES3Context::ApiLinkProgram(GLuint program)
{
    CProgramObj *p = FindProgram(program);
    LinkProgram(p);
    CTX_ANALYZER_FUNC1(LinkProgram, GLOutput, GL_OUT_BUF_SIZE, program);
}

GLvoid CGLES3Context::ApiCompileShader(GLuint shader)
{
    CShaderObj *p = FindShader(shader);
    CompileShader(p);
    CTX_ANALYZER_FUNC1(CompileShader, GLOutput, GL_OUT_BUF_SIZE, shader);
}

GLuint CGLES3Context::CompileShader(CShaderObj *pShader)
{
    GLuint  name    = pShader->m_name;
    GLint   status  = 0;
    GLint   len     = 0;
    GLchar  infoLog[256];

    // GL_VERTEX_SHADER: 0x8B31
    // GL_FRAGMENT_SHADER: 0x8B30
    g_opengl->glShaderSource(name, 1, (const GLchar**)&pShader->m_source, NULL);
    g_opengl->glCompileShader(name);
    g_opengl->glGetShaderiv(name, GL_COMPILE_STATUS, &status);
    if (status == 0)
    {
        memset(infoLog, 0, 256);
        g_opengl->glGetShaderInfoLog(name, 256, &len, infoLog);
        printf("Compile failure on shader(%d)\n", name);
        printf("Info: %s\n", infoLog);
        pShader->m_bCompiled = GL_FALSE;
    }

    pShader->m_bCompiled = GL_TRUE;

    return name;
}

GLuint CGLES3Context::ApiCreateShader(GLenum type, GLuint shader)
{
    CreateShaderObject(type, shader);
    CTX_ANALYZER_FUNC2(CreateShader, GLOutput, GL_OUT_BUF_SIZE, type, shader);
    return shader;
}

GLuint CGLES3Context::ApiCreateProgram(GLuint program)
{
    CreateProgramObject(program);
    CTX_ANALYZER_FUNC1(CreateProgram, GLOutput, GL_OUT_BUF_SIZE, program);
    return program;
}

GLvoid CGLES3Context::ApiDetachShader(GLuint program, GLuint shader)
{
    CProgramObj *pProg = FindProgram(program);
    CShaderObj  *pShader = FindShader(shader);

    if (!pProg || !pShader)
        return;

    if (pProg->m_pVertex == pShader)
    {
        pProg->m_pVertex = NULL;
    }
    else if (pProg->m_pFragment == pShader)
    {
        pProg->m_pFragment = NULL;
    }

    CTX_ANALYZER_FUNC2(DetachShader, GLOutput, GL_OUT_BUF_SIZE, program, shader);
}

GLvoid CGLES3Context::ApiDeleteShader(GLuint shader)
{
    DeleteShaderObject(shader);
    CTX_ANALYZER_FUNC1(DeleteShader, GLOutput, GL_OUT_BUF_SIZE, shader);
}

GLvoid CGLES3Context::ApiDeleteProgram(GLuint program)
{
    DeleteProgramObject(program);
    CTX_ANALYZER_FUNC1(DeleteProgram, GLOutput, GL_OUT_BUF_SIZE, program);
}

GLvoid CGLES3Context::ApiGetAttribLocation(GLuint program, const GLchar *name, GLint oldLoc)
{
    CProgramObj *pProg  = FindProgram(program);
    CAttribObj  *pAttr  = pProg->FindAttributeByName(name);

    if (pAttr)
    {
        pAttr->nLoc = g_opengl->glGetAttribLocation(pProg->m_name, name);
        pAttr->nOldLoc = oldLoc;
    }

    CTX_ANALYZER_FUNC3(GetAttribLocation, GLOutput, GL_OUT_BUF_SIZE, program, name, oldLoc);
}

GLvoid CGLES3Context::ApiProgramBinary(GLuint program, GLenum binaryFormat, const void *binary, GLsizei length)
{
    CProgramObj *p = FindProgram(program);

    if (p->m_pVertex && p->m_pFragment)
    {
        LinkProgram(p);
    }

    CTX_ANALYZER_FUNC4(ProgramBinary, GLOutput, GL_OUT_BUF_SIZE, program, binaryFormat, binary, length);
}

GLvoid CGLES3Context::ApiShaderSource(GLuint shader, GLsizei count, const GLchar *const*string, const GLint *length)
{
    GLsizei     i       = 0;
    GLsizei     n       = 0;
    CShaderObj  *p      = CURRENT_CONTEXT1()->shaderMap[shader];
    GLchar      *tmp    = NULL;
    GLuint      total   = 1;

    if (string == NULL)
    {
        return;
    }

    curShaderSrc    = (const GLchar**)string;
    curShaderCount  = count;
    curShader       = shader;

    for (i = 0; i < count; ++i)
    {
        if ((length != NULL) && (length[i] > 0))
        {
            total += length[i];
            n = length[i];
        }
        else
        {
            total += STRLEN(string[i]);
        }
    }

    p->m_source = (GLchar*)malloc(total);
    p->m_len    = total - 1;
    memset(p->m_source, 0, total);

    total = 0;
    for (i = 0; i < count; ++i)
    {
        if ((length != NULL) && (length[i] > 0))
        {
            n = length[i];
        }
        else if (string[i])
        {
            n = STRLEN(string[i]);
        }

        if (string[i] == NULL)
        {
            continue;
        }

        memcpy(p->m_source+total, string[i], n);
        total += n;
    }

    tmp = p->m_source;
    IndentShaderCode(tmp, n, &p->m_source);
    free(tmp);

    CTX_ANALYZER_FUNC4(ShaderSource, Uniform, UNIFORM_BUF_SIZE, shader, count, string, length);
}

CUniformObj* CProgramObj::GetUniformByName(const GLchar *name)
{
    CUniformObj *p = NULL;
    GLuint      uniformSize = m_uniforms.size();

    for (GLuint i=0; i<uniformSize; i++)
    {
        p = m_uniforms[i];
        if (p->name &&
            strlen(name) == strlen(p->name) &&
            !strncmp(name, p->name, strlen(name)))
        {
            if (strstr(name, "[0]"))
            {
                return p;
            }
            else if (!strstr(name, "["))
            {
                return p;
            }
        }
    }

    return NULL;
}

CUniformObj* CProgramObj::FindUniformByOldLoc(GLint loc)
{
    CUniformObj *p = NULL;
    GLuint      size = m_uniforms.size();

    if (loc < 0)
        return NULL;

    for (GLuint i=0; i<size; i++)
    {
        p = m_uniforms[i];
        if (p->nOldLoc == loc)
        {
            return p;
        }
    }

    return NULL;
}

CUniformObj* CProgramObj::FindUniformByLoc(GLint loc)
{
    CUniformObj *p = NULL;
    GLuint      size = m_uniforms.size();

    if (loc < 0)
        return NULL;

    for (GLuint i=0; i<size; i++)
    {
        p = m_uniforms[i];
        if (p->nLoc == loc)
        {
            return p;
        }
    }

    return NULL;
}

GLvoid* CProgramObj::GetUniformValueByLoc(GLint loc)
{
    CUniformObj *p = FindUniformByLoc(loc);
    return p->data;
}

GLvoid CProgramObj::SetUniform1234if(GLint location, eShaderType type, GLvoid *value)
{
    CUniformObj *uniform  = FindUniformByOldLoc(location);
    GLuint      totalSize = GetSizeFromShaderDataType(type);

    if (location >= 0 && uniform != NULL)
    {
        FREE(uniform->data);

        uniform->data    = malloc(totalSize);
        uniform->count   = 1;
        memcpy(uniform->data, value, totalSize);
    }
}

GLvoid  CProgramObj::SetUniform1234ifv(GLuint location, eShaderType type, GLuint count, const GLvoid *pData)
{
    CUniformObj *pUniform   = FindUniformByOldLoc(location);
    GLuint      totalSize   = GetSizeFromShaderDataType(type)*count;
    GLvoid      *pNewData   = (GLvoid*)malloc(totalSize);

    if (pNewData == NULL || pData == NULL)
        return;

    if (location >= 0 && pUniform != NULL)
    {
        GLchar  *p = (GLchar*)pNewData;

        memcpy(pNewData, pData, totalSize);
        FREE(pUniform->data);

        pUniform->count = count;
        pUniform->data  = pNewData;
    }
    else
    {
        FREE(pNewData);
    }
}

GLvoid CGLES3Context::ApiUniform1f(GLint location, GLfloat v0)
{
    CProgramObj *p = FindProgram(curProgram);
    p->SetUniform1234if(location, DATA_TYPE_FLOAT_X1, &v0);
    CTX_ANALYZER_FUNC2(Uniform1f, GLOutput, GL_OUT_BUF_SIZE, location, v0);
}

GLvoid CGLES3Context::ApiUniform1fv(GLint location, GLsizei count, const GLfloat *value)
{
    CProgramObj *p = FindProgram(curProgram);
    p->SetUniform1234ifv(location, DATA_TYPE_FLOAT_X1, count, value);
    CTX_ANALYZER_FUNC3(Uniform1fv, Uniform, UNIFORM_BUF_SIZE, location, count, value);
}

GLvoid CGLES3Context::ApiUniform1i(GLint location, GLint v0)
{
    CProgramObj *p = FindProgram(curProgram);
    p->SetUniform1234if(location, DATA_TYPE_INTEGER_X1, &v0);
    CTX_ANALYZER_FUNC2(Uniform1i, GLOutput, GL_OUT_BUF_SIZE, location, v0);
}

GLvoid CGLES3Context::ApiUniform1iv(GLint location, GLsizei count, const GLint *value)
{
    CProgramObj *p = FindProgram(curProgram);
    p->SetUniform1234ifv(location, DATA_TYPE_INTEGER_X1, count, value);
    CTX_ANALYZER_FUNC3(Uniform1iv, Uniform, UNIFORM_BUF_SIZE, location, count, value);
}

GLvoid CGLES3Context::ApiUniform2f(GLint location, GLfloat v0, GLfloat v1)
{
    CProgramObj *p      = FindProgram(curProgram);
    GLfloat     v[2]    = {v0, v1};
    p->SetUniform1234if(location, DATA_TYPE_FLOAT_X2, v);
    CTX_ANALYZER_FUNC3(Uniform2f, GLOutput, GL_OUT_BUF_SIZE, location, v0, v1);
}

GLvoid CGLES3Context::ApiUniform2fv(GLint location, GLsizei count, const GLfloat *value)
{
    CProgramObj *p = FindProgram(curProgram);
    p->SetUniform1234ifv(location, DATA_TYPE_FLOAT_X2, count, value);
    CTX_ANALYZER_FUNC3(Uniform2fv, Uniform, UNIFORM_BUF_SIZE, location, count, value);
}

GLvoid CGLES3Context::ApiUniform2i(GLint location, GLint v0, GLint v1)
{
    CProgramObj *p      = FindProgram(curProgram);
    GLint       v[2]    = {v0, v1};

    p->SetUniform1234if(location, DATA_TYPE_INTEGER_X2, v);
    CTX_ANALYZER_FUNC3(Uniform2i, GLOutput, GL_OUT_BUF_SIZE, location, v0, v1);
}

GLvoid CGLES3Context::ApiUniform2iv(GLint location, GLsizei count, const GLint *value)
{
    CProgramObj *p = FindProgram(curProgram);
    p->SetUniform1234ifv(location, DATA_TYPE_INTEGER_X2, count, value);
    CTX_ANALYZER_FUNC3(Uniform2iv, Uniform, UNIFORM_BUF_SIZE, location, count, value);
}

GLvoid CGLES3Context::ApiUniform3f(GLint location, GLfloat v0, GLfloat v1, GLfloat v2)
{
    CProgramObj *p      = FindProgram(curProgram);
    GLfloat     v[3]    = {v0, v1, v2};
    p->SetUniform1234if(location, DATA_TYPE_FLOAT_X3, v);
    CTX_ANALYZER_FUNC4(Uniform3f, GLOutput, GL_OUT_BUF_SIZE, location, v0, v1, v2);
}

GLvoid CGLES3Context::ApiUniform3fv(GLint location, GLsizei count, const GLfloat *value)
{
    CProgramObj *p = FindProgram(curProgram);
    p->SetUniform1234ifv(location, DATA_TYPE_FLOAT_X3, count, value);
    CTX_ANALYZER_FUNC3(Uniform3fv, Uniform, UNIFORM_BUF_SIZE, location, count, value);
}

GLvoid CGLES3Context::ApiUniform3i(GLint location, GLint v0, GLint v1, GLint v2)
{
    CProgramObj *p      = FindProgram(curProgram);
    GLint       v[3]    = {v0, v1, v2};

    p->SetUniform1234if(location, DATA_TYPE_INTEGER_X3, v);
    CTX_ANALYZER_FUNC4(Uniform3i, GLOutput, GL_OUT_BUF_SIZE, location, v0, v1, v2);
}

GLvoid CGLES3Context::ApiUniform3iv(GLint location, GLsizei count, const GLint *value)
{
    CProgramObj *p = FindProgram(curProgram);
    p->SetUniform1234ifv(location, DATA_TYPE_INTEGER_X3, count, value);
    CTX_ANALYZER_FUNC3(Uniform3iv, Uniform, UNIFORM_BUF_SIZE, location, count, value);
}

GLvoid CGLES3Context::ApiUniform4f(GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3)
{
    CProgramObj *p      = FindProgram(curProgram);
    GLfloat     v[4]    = {v0, v1, v2, v3};
    p->SetUniform1234if(location, DATA_TYPE_FLOAT_X4, v);
    CTX_ANALYZER_FUNC5(Uniform4f, GLOutput, GL_OUT_BUF_SIZE, location, v0, v1, v2, v3);
}

GLvoid CGLES3Context::ApiUniform4fv(GLint location, GLsizei count, const GLfloat *value)
{
    CProgramObj *p = FindProgram(curProgram);
    p->SetUniform1234ifv(location, DATA_TYPE_FLOAT_X4, count, value);
    CTX_ANALYZER_FUNC3(Uniform4fv, Uniform, UNIFORM_BUF_SIZE, location, count, value);
}

GLvoid CGLES3Context::ApiUniform4i(GLint location, GLint v0, GLint v1, GLint v2, GLint v3)
{
    CProgramObj *p      = FindProgram(curProgram);
    GLint       v[4]    = {v0, v1, v2, v3};

    p->SetUniform1234if(location, DATA_TYPE_INTEGER_X4, v);
    CTX_ANALYZER_FUNC5(Uniform4i, GLOutput, GL_OUT_BUF_SIZE, location, v0, v1, v2, v3);
}

GLvoid CGLES3Context::ApiUniform4iv(GLint location, GLsizei count, const GLint *value)
{
    CProgramObj *p = FindProgram(curProgram);
    p->SetUniform1234ifv(location, DATA_TYPE_INTEGER_X4, count, value);
    CTX_ANALYZER_FUNC3(Uniform4iv, Uniform, UNIFORM_BUF_SIZE, location, count, value);
}

GLvoid CProgramObj::SetUniformMatrix234fv(GLuint location, eShaderType type, GLuint count, GLuint transpose, const GLfloat *pData)
{
    GLvoid      *pNewData   = NULL;
    CUniformObj *pUniform   = FindUniformByOldLoc(location);
    GLuint      totalSize   = GetSizeFromShaderDataType(type)*count;

    if (!pUniform || !pData)
        return;

    pNewData = malloc(totalSize);
    memcpy(pNewData, pData, totalSize);
    FREE(pUniform->data);

    pUniform->data        = (GLvoid*)pNewData;
    pUniform->transpose   = transpose;
    pUniform->count       = count;
}

GLvoid CGLES3Context::ApiUniformMatrix2fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value)
{
    CProgramObj *p = FindProgram(curProgram);
    p->SetUniformMatrix234fv(location, DATA_TYPE_FLOAT_2X2, count, transpose, value);
    CTX_ANALYZER_FUNC4(UniformMatrix2fv, Uniform, UNIFORM_BUF_SIZE, location, count, transpose, value);
}

GLvoid CGLES3Context::ApiUniformMatrix3fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value)
{
    CProgramObj *p = FindProgram(curProgram);
    p->SetUniformMatrix234fv(location, DATA_TYPE_FLOAT_3X3, count, transpose, value);
    CTX_ANALYZER_FUNC4(UniformMatrix3fv, Uniform, UNIFORM_BUF_SIZE, location, count, transpose, value);
}

GLvoid CGLES3Context::ApiUniformMatrix4fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value)
{
    CProgramObj *p = FindProgram(curProgram);
    p->SetUniformMatrix234fv(location, DATA_TYPE_FLOAT_4X4, count, transpose, value);
    CTX_ANALYZER_FUNC4(UniformMatrix4fv, Uniform, UNIFORM_BUF_SIZE, location, count, transpose, value);
}

GLvoid CGLES3Context::ApiUseProgram(GLuint program)
{
    curProgram = program;
    CTX_ANALYZER_FUNC1(UseProgram, GLOutput, GL_OUT_BUF_SIZE, program);
}

GLvoid CGLES3Context::ApiGetActiveAttrib(GLuint program, GLuint index, GLsizei bufSize, GLsizei *length, GLint *size, GLenum *type, GLchar *name)
{
    CTX_ANALYZER_FUNC7(GetActiveAttrib, GLOutput, GL_OUT_BUF_SIZE, program, index, bufSize, length, size, type, name);
}

GLvoid CGLES3Context::ApiGetActiveUniform(GLuint program, GLuint index, GLsizei bufSize, GLsizei *length, GLint *size, GLenum *type, GLchar *name)
{
    CTX_ANALYZER_FUNC7(GetActiveUniform, GLOutput, GL_OUT_BUF_SIZE, program, index, bufSize, length, size, type, name);
}

GLvoid CGLES3Context::ApiGetAttachedShaders(GLuint program, GLsizei maxCount, GLsizei *count, GLuint *shaders)
{
    CTX_ANALYZER_FUNC4(GetAttachedShaders, GLOutput, GL_OUT_BUF_SIZE, program, maxCount, count, shaders);
}

GLvoid CGLES3Context::ApiGetProgramiv(GLuint program, GLenum pname, GLint *params)
{
    CTX_ANALYZER_FUNC3(GetProgramiv, GLOutput, GL_OUT_BUF_SIZE, program, pname, params);
}

GLvoid CGLES3Context::ApiGetProgramInfoLog(GLuint program, GLsizei bufSize, GLsizei *length, GLchar *infoLog)
{
    CTX_ANALYZER_FUNC4(GetProgramInfoLog, GLOutput, GL_OUT_BUF_SIZE, program, bufSize, length, infoLog);
}

GLvoid CGLES3Context::ApiGetShaderiv(GLuint shader, GLenum pname, GLint *params)
{
    CTX_ANALYZER_FUNC3(GetShaderiv, GLOutput, GL_OUT_BUF_SIZE, shader, pname, params);
}

GLvoid CGLES3Context::ApiGetShaderInfoLog(GLuint shader, GLsizei bufSize, GLsizei *length, GLchar *infoLog)
{
    CTX_ANALYZER_FUNC4(GetShaderInfoLog, GLOutput, GL_OUT_BUF_SIZE, shader, bufSize, length, infoLog);
}

GLvoid CGLES3Context::ApiGetShaderPrecisionFormat(GLenum shadertype, GLenum precisiontype, GLint *range, GLint *precision)
{
    CTX_ANALYZER_FUNC4(GetShaderPrecisionFormat, GLOutput, GL_OUT_BUF_SIZE, shadertype, precisiontype, range, precision);
}

GLvoid CGLES3Context::ApiGetShaderSource(GLuint shader, GLsizei bufSize, GLsizei *length, GLchar *source)
{
    CTX_ANALYZER_FUNC4(GetShaderSource, GLOutput, GL_OUT_BUF_SIZE, shader, bufSize, length, source);
}

GLvoid CGLES3Context::ApiGetUniformfv(GLuint program, GLint location, GLfloat *params)
{
    CTX_ANALYZER_FUNC3(GetUniformfv, GLOutput, GL_OUT_BUF_SIZE, program, location, params);
}

GLvoid CGLES3Context::ApiGetUniformiv(GLuint program, GLint location, GLint *params)
{
    CTX_ANALYZER_FUNC3(GetUniformiv, GLOutput, GL_OUT_BUF_SIZE, program, location, params);
}

GLvoid CGLES3Context::ApiShaderBinary(GLsizei count, const GLuint *shaders, GLenum binaryformat, const void *binary, GLsizei length)
{
    CTX_ANALYZER_FUNC5(ShaderBinary, GLOutput, GL_OUT_BUF_SIZE, count, shaders, binaryformat, binary, length);
}

GLvoid CGLES3Context::ApiValidateProgram(GLuint program)
{
    CTX_ANALYZER_FUNC1(ValidateProgram, GLOutput, GL_OUT_BUF_SIZE, program);
}

GLvoid CGLES3Context::ApiGetProgramBinary(GLuint program, GLsizei bufSize, GLsizei *length, GLenum *binaryFormat, void *binary)
{
    CTX_ANALYZER_FUNC5(GetProgramBinary, GLOutput, GL_OUT_BUF_SIZE, program, bufSize, length, binaryFormat, binary);
}