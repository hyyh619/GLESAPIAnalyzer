#ifndef __GLSL_SHADER_H
#define __GLSL_SHADER_H

#include <vector>
#include <map>
#include <GLES3/gl31.h>
#include "GLESGlobal.h"
#include "ApiOESEngine.h"

#define SHADER_SAMPLER_NUM      8

/* shader type enumeration. */
typedef enum _eShaderType
{
    DATA_TYPE_FLOAT_X1,                 /* 0x00 */
    DATA_TYPE_FLOAT_X2,                 /* 0x01 */
    DATA_TYPE_FLOAT_X3,                 /* 0x02 */
    DATA_TYPE_FLOAT_X4,                 /* 0x03 */
    DATA_TYPE_FLOAT_2X2,                /* 0x04 */
    DATA_TYPE_FLOAT_3X3,                /* 0x05 */
    DATA_TYPE_FLOAT_4X4,                /* 0x06 */
    DATA_TYPE_BOOLEAN_X1,               /* 0x07 */
    DATA_TYPE_BOOLEAN_X2,               /* 0x08 */
    DATA_TYPE_BOOLEAN_X3,               /* 0x09 */
    DATA_TYPE_BOOLEAN_X4,               /* 0x0A */
    DATA_TYPE_INTEGER_X1,               /* 0x0B */
    DATA_TYPE_INTEGER_X2,               /* 0x0C */
    DATA_TYPE_INTEGER_X3,               /* 0x0D */
    DATA_TYPE_INTEGER_X4,               /* 0x0E */
    DATA_TYPE_SAMPLER_1D,               /* 0x0F */
    DATA_TYPE_SAMPLER_2D,               /* 0x10 */
    DATA_TYPE_SAMPLER_3D,               /* 0x11 */
    DATA_TYPE_SAMPLER_CUBIC,            /* 0x12 */
    DATA_TYPE_FIXED_X1,                 /* 0x13 */
    DATA_TYPE_FIXED_X2,                 /* 0x14 */
    DATA_TYPE_FIXED_X3,                 /* 0x15 */
    DATA_TYPE_FIXED_X4,                 /* 0x16 */
    DATA_TYPE_IMAGE_2D,                 /* 0x17 */  /* For OCL. */
    DATA_TYPE_IMAGE_3D,                 /* 0x18 */  /* For OCL. */
    DATA_TYPE_SAMPLER,                  /* 0x19 */  /* For OCL. */
    DATA_TYPE_FLOAT_2X3,                /* 0x1A */
    DATA_TYPE_FLOAT_2X4,                /* 0x1B */
    DATA_TYPE_FLOAT_3X2,                /* 0x1C */
    DATA_TYPE_FLOAT_3X4,                /* 0x1D */
    DATA_TYPE_FLOAT_4X2,                /* 0x1E */
    DATA_TYPE_FLOAT_4X3,                /* 0x1F */
    DATA_TYPE_ISAMPLER_2D,              /* 0x20 */
    DATA_TYPE_ISAMPLER_3D,              /* 0x21 */
    DATA_TYPE_ISAMPLER_CUBIC,           /* 0x22 */
    DATA_TYPE_USAMPLER_2D,              /* 0x23 */
    DATA_TYPE_USAMPLER_3D,              /* 0x24 */
    DATA_TYPE_USAMPLER_CUBIC,           /* 0x25 */
    DATA_TYPE_SAMPLER_EXTERNAL_OES,     /* 0x26 */
    DATA_TYPE_INVALID
} eShaderType;

typedef struct _SAMPLER
{
    GLuint              textureUnit;
    GLchar              *uniformName;
    GLenum              textureType;
} SAMPLER;

class CAttribObj
{
public:
    CAttribObj()
    {
        size                = 0;
        type                = 0;
        normalized          = 0;
        stride              = 0;
        pointer             = 0;
        name                = 0;
        bufID               = 0;
        nLoc                = 0;
        nOldLoc             = 0;
    }

    ~CAttribObj()
    {
        if (name)
        {
            free(name);
        }
    }

public:
    GLint               size;
    GLenum              type;
    GLboolean           normalized;
    GLsizei             stride;
    GLint               pointer;
    GLchar              *name;
    GLuint              bufID;          // Binded buffer id
    GLint               nLoc;
    GLint               nOldLoc;
};

typedef std::vector<CAttribObj*> AttributeArray;

class CUniformObj
{
public:
    CUniformObj()
    {
        nameLen     = 0;
        name        = NULL;
        data        = NULL;
        count       = 0;
        transpose   = 0;
        type        = 0;
        nLoc        = 0;
        nOldLoc     = 0;
        size        = 0;
    }

    ~CUniformObj()
    {
        if (name)
        {
            free(name);
        }

        if (data)
        {
            free(data);
        }
    }

public:
    size_t              nameLen;        /* Length of the uniform name. */
    GLchar              *name;          /* The uniform name. */

    GLvoid              *data;
    GLuint              count;
    GLboolean           transpose;
    GLenum              type;
    GLint               nLoc;
    GLint               nOldLoc;
    GLuint              size;           /* uniform size, such as matrix[16], its size is 16 */
};

typedef std::vector<CUniformObj*>   UniformArray;

class CShaderObj
{
public:
    CShaderObj(GLuint oldName, GLenum type)
    {
        m_bCompiled = GL_FALSE;
        m_type      = type;
        m_oldName   = oldName;
        m_name      = g_opengl->glCreateShader(type);

        memset(m_strFileName, 0, 256);
    }

    ~CShaderObj()
    {
        Release();
    }

    GLvoid Release()
    {
        if (m_name)
        {
            g_opengl->glDeleteShader(m_name);
            m_name = 0;
        }

        if (m_source)
        {
            free(m_source);
            m_source = NULL;
        }
    }

public:
    GLenum      m_type;
    GLchar      *m_source;
    GLuint      m_len;
    GLuint      m_name;
    GLuint      m_oldName;
    GLboolean   m_bCompiled;
    GLchar      m_strFileName[256];
};

class CProgramObj
{
public:
    CProgramObj(GLuint oldName)
    {
        m_pVertex   = NULL;
        m_pFragment = NULL;

        m_bLinked   = GL_FALSE;
        m_oldName   = oldName;
        m_name      = g_opengl->glCreateProgram();

        m_attributes.clear();
        m_uniforms.clear();
    }

    ~CProgramObj()
    {
        GLuint      size, j;
        CUniformObj *pUniform;
        CAttribObj  *pAttr;

        size = m_uniforms.size();
        for (j=0; j<size; j++)
        {
            pUniform = m_uniforms[j];
            delete (pUniform);
        }
        m_uniforms.clear();

        size = m_attributes.size();
        for (j=0; j<size; j++)
        {
            pAttr = m_attributes[j];
            delete pAttr;
        }
        m_attributes.clear();

        // We cannot release GL object here, because there is no gl context.
        if (m_name)
        {
            g_opengl->glDeleteProgram(m_name);
        }
    }

    CAttribObj*         FindAttributeByName(const GLchar *name);
    CAttribObj*         FindAttribObjByIndex(GLuint index);
    CUniformObj*        FindUniformByOldLoc(GLint loc);
    CUniformObj*        FindUniformByLoc(GLint loc);
    CUniformObj*        GetUniformByName(const GLchar *name);
    GLvoid              SetUniform1234if(GLint location, eShaderType type, GLvoid *value);
    GLvoid              SetUniform1234ifv(GLuint location, eShaderType type, GLuint count, const GLvoid *pData);
    GLvoid              SetUniformMatrix234fv(GLuint location, eShaderType type, GLuint count, GLuint transpose, const GLfloat *pData);
    GLvoid              AnalyzeAttribute();
    GLvoid              AnalyzeUniform();
    GLvoid*             GetUniformValueByLoc(GLint loc);

public:
    GLuint              m_name;
    GLuint              m_oldName;
    GLboolean           m_bLinked;
    CShaderObj          *m_pVertex;
    CShaderObj          *m_pFragment;

    /* Attributes */
    AttributeArray      m_attributes;

    /* Uniforms */
    UniformArray        m_uniforms;
};

typedef std::map<GLuint, CShaderObj*>   ShaderMap;
typedef std::map<GLuint, CProgramObj*>  ProgramMap;

GLvoid      PrintShader(GLint *pos, GLuint progID, GLuint vertex, GLuint fragment, char *output, GLint outputSize);
eShaderType ConvertGLSLDataTypeToInternType(GLenum type);
GLuint      GetSizeFromShaderDataType(eShaderType type);
GLuint      GetShaderDataSizeByType(GLenum shaderType);

#endif /* __GLSL_SHADER_H */