#include "utils.h"
#include "png.h"
#include "bitmap.h"
#include "MemoryPool.h"
#include "ApiAnalyzer.h"

enLogLevel printLevel = LOG_ERROR;

const char  *strHalDumpFileName = "hal_Displacement_noData_21.log.hy";
char        strOnlyFileName[FILE_NAME_BUF_SIZE];
char        strOnlyFrameFileName[FILE_NAME_BUF_SIZE];
char        strFileName[FILE_NAME_BUF_SIZE];
char        strOnlySourcePath[FILE_NAME_BUF_SIZE];
char        Line[LINE_BUF_SIZE];
char        Uniform[UNIFORM_BUF_SIZE];
char        Draw[DRAW_BUF_SIZE];
char        Output[OUT_BUF_SIZE];
char        EGLOutput[EGL_OUT_BUF_SIZE];
char        GLOutput[GL_OUT_BUF_SIZE];

char        tmp[TMP_BUF_SIZE];
char        tmp1[TMP_BUF_SIZE];
char        tmp2[TMP_BUF_SIZE];
char        tmp3[TMP_BUF_SIZE];
char        tmp4[TMP_BUF_SIZE];
char        tmp5[TMP5_BUF_SIZE];
char        tmp6[TMP5_BUF_SIZE];

FILE        *g_pApiDumpFile = NULL;
FILE        *g_pOutputFile  = NULL;
FILE        *g_pDrawFile    = NULL;
FILE        *g_pFrameFile   = NULL;
FILE        *g_pShaderFile  = NULL;
FILE        *g_pTextureFile = NULL;
GLuint      nLineNum        = 0;
GLuint      nStartFrame     = 0;
GLuint      nEndFrame       = 0;
GLuint      g_nFrames       = 1;

GLboolean   bPrintEGL           = GL_FALSE;
GLboolean   bPrintES20          = GL_FALSE;
GLboolean   bPrintES11          = GL_FALSE;
GLboolean   bPrintDrawShader    = GL_FALSE;
GLboolean   bPrintVertexAttrib  = GL_FALSE;
GLboolean   g_bSaveTexture      = GL_FALSE;
GLboolean   bDumpFrameFile      = GL_FALSE;
GLboolean   bPrintMatrix        = GL_FALSE;
GLboolean   g_bSaveShader       = GL_FALSE;
GLboolean   bForceFinish        = GL_FALSE;
GLboolean   bPrintUniform       = GL_FALSE;
GLboolean   bThread             = GL_FALSE;
GLboolean   bPrintLine          = GL_FALSE;
GLboolean   bPrintContent       = GL_TRUE;
GLboolean   bExitApp            = GL_FALSE;
GLboolean   bDatabase           = GL_FALSE;
GLboolean   bUsePng             = GL_FALSE;
GLboolean   bCollectCloudSize   = GL_FALSE;
GLboolean   bFlushFile          = GL_TRUE;
GLboolean   bOutputToShaderFile = GL_TRUE;

GLint       eglWidth            = 1920;
GLint       eglHeight           = 1080;
GLenum      eglFormat           = GL_RGBA;

DictionaryNode  *dictRoot       = NULL;

/* collect cloud render data size for each frame */
int         totalCloudSize      = 0;
int         texCloudSize        = 0;
int         vboCloudSize        = 0;
int         uniformCloudSize    = 0;
int         funcCloudSize       = 0;
int         vertexCloudSize     = 0;

STRING_MAP  g_frameStringMap;
STRING_MAP  g_drawStringMap;
STRING_MAP  g_totalStringMap;
GLboolean   g_bOutputToFile     = GL_TRUE;
GLuint      g_frameDrawCount    = 0;
GLuint      g_drawStringCount   = 0;
GLuint      g_totalStringCount  = 0;

GLvoid Abort(const GLchar *msg, ...)
{
    GLchar  string[512];

    memset(string, 0, 512);

    va_list args;
    va_start(args, msg);
    vsnprintf(string, sizeof(string), msg, args);
    va_end(args);

    OutputDebugInfoToFile(string);
    printf("%s\n",  string);

#ifdef WIN32
    __asm int 3;
#endif
}

GLvoid ClearAllStringMap()
{
    ClearStringMap(&g_frameStringMap);
    ClearStringMap(&g_drawStringMap);
    ClearStringMap(&g_totalStringMap);

    g_frameDrawCount    = 0;
    g_drawStringCount   = 0;
    g_totalStringCount  = 0;
}

GLvoid ClearDrawStringMap()
{
    ClearStringMap(&g_drawStringMap);
    g_drawStringCount = 0;
}

GLvoid ClearStringMap(STRING_MAP *p)
{
    p->clear();
}

GLvoid AddStringToMap(GLuint id, const GLchar *string, STRING_MAP *p)
{
    std::string str;

    str.append(string);
    p->insert(std::make_pair(id, str));
}

GLvoid EnableOutputToFile(GLboolean bEnable)
{
    g_bOutputToFile = bEnable;
}

void* ReadCompressedTex(GLuint type, GLuint handle, GLuint count)
{
    void *data = NULL;

#ifdef API_DUMP
    if (handle)
    {
        data = GetCurrentTexDataPtr(handle);
    }
#else
    switch (type)
    {
    case READ_GL_UINT:
        data = readUInt(count);
        break;

    case READ_GL_UCHAR:
        data = readUCharFast(count);
        break;

    default:
        break;
    }
#endif

    return data;
}

void* ReadTexData(GLuint type, GLuint handle, GLuint count)
{
    void *data = NULL;

#ifdef API_DUMP
    if (handle)
    {
        data = GetCurrentTexDataPtr(handle);
    }
#else
    switch (type)
    {
    case READ_GL_UINT:
        data = readUInt(count);
        break;

    case READ_GL_UCHAR:
        data = readUCharFast(count);
        break;

    default:
        break;
    }
#endif

    return data;
}

GLvoid* ReadData(GLuint type, GLuint handle, GLuint count)
{
    GLvoid *data = NULL;

#ifdef API_DUMP
    if (handle)
    {
        data = GetCurrentDataPtr(handle);
    }
#else
    switch (type)
    {
    case READ_GL_UINT:
        data = readUInt(count);
        break;

    case READ_GL_UCHAR:
        data = readUCharFast(count);
        break;

    default:
        break;
    }
#endif

    return data;
}

GLuint ReadUInt(GLuint handle)
{
    GLuint *v = (GLuint*)ReadData(READ_GL_UINT, handle, 1);
    GLuint res  = v?*v:0;

    FreeData(v);
    return res;
}

void FreeData(void *ptr)
{
#ifndef API_DUMP
    FREE(ptr);
#endif
}

void FlushToOutputFile(char *output, FILE *file)
{
    int nWrittenBytes   = 0;
    int nLen            = strlen(output);

    if (file)
    {
        nWrittenBytes = fwrite(output, 1, nLen, file);
        if (nWrittenBytes != nLen)
        {
            printf("FlushToOutputFile: Write cmd buffer file error\n");
        }
    }

#ifdef _WIN32
    _flushall();
#endif

    return;
}

int char2intTable[] =
{
/*00*/  0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
/*10*/  0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
/*20*/  0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
/*30*/  0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
/*40*/  0, 1, 2, 3, 4, 0, 0, 0, 0, 1,   // 48: '0' ~ 51: '9'
/*50*/  2, 3, 4, 5, 6, 7, 8, 9, 8, 9,
/*60*/  0, 1, 2, 3, 4,10,11,12,13,14,   // 65: 'A' ~ 70: 'F'
/*70*/ 15, 1, 2, 3, 4,10,11,12,13,14,
/*80*/ 15, 1, 2, 3, 4, 5, 6, 7, 8, 9,
/*90*/  0, 1, 2, 3, 4, 5, 6,10,11,12,   // 97: 'a' ~ 102: 'f'
/*A0*/ 13,14,15, 3, 4, 5, 6,10,11,12,
/*B0*/ 13,14,15, 3, 4, 5, 6, 7, 8, 9,
/*C0*/  0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
/*D0*/  0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
};

static int Char2Int(char c)
{
    return char2intTable[c];
}

char* GenJsonArray(GLuint *v, int count, char *key)
{
    char buf1[16];
    char *json;
    int len = count * (strlen(key)+12) + 10;

    json = (char*)malloc(len);
    if (json == NULL)
        return NULL;

    memset(json, 0, len);
    sprintf(json, "{\"%s\": [", key);
    for (int i=0; i<count; i++)
    {
        memset(buf1, 0, 16);
        sprintf(buf1, "%d", v[i]);
        strcat(json, buf1);
        if (i < (count-1))
        {
            strcat(json, ",");
        }
    }
    strcat(json, "]}");

    return json;
}

GLboolean ParseArguments1(int argc, const char **argv)
{
    int i = 0;

    if (argc < 2)
    {
        printf("**** -file filename     input API dump file\n");
        printf("**** -egl               dump egl API\n");
        printf("**** -es11              dump es11 API\n");
        printf("**** -es20              dump es20 API\n");
        printf("**** -shader            dump shader source code\n");
        printf("**** -vertex            dump vertex attributes data\n");
        printf("**** -save_texture      dump texture bitmap file\n");
        printf("**** -start num         dump API from specified frames\n");
        printf("**** -end num           dump API until specified frames\n");
        printf("**** -frame_file        dump frame file\n");
        printf("**** -matrix            dump matrix, such as rotate, scale, translate and multiply\n");
        printf("**** -save_shader       save shader source code to a file\n");
        printf("**** -width             set EGL surface width\n");
        printf("**** -height            set EGL surface height\n");
        printf("**** -uniform           dump uniform variables\n");
        printf("**** -thread            dump thread id\n");
        printf("**** -line              print source file line number\n");
        printf("**** -png               save texture as png file\n");

        return GL_FALSE;
    }

    for (i=0; i<argc; i++, argv++)
    {
        if (!strncmp(*argv, "-file", 5))
        {
            char    *p = NULL;
            int     len = 0;

            argv++;
            i++;

            memset(strFileName, 0, FILE_NAME_BUF_SIZE);
            memset(strOnlyFileName, 0, FILE_NAME_BUF_SIZE);
            memset(strOnlyFrameFileName, 0, FILE_NAME_BUF_SIZE);
            strcpy(strFileName, *argv);

            len = strlen(strFileName);
            p = &strFileName[len-1];

#ifndef LINUX
            while (*p != '/')
                p --;
            p += 1;
#else
            while (*p-- != '/'); p += 2;
#endif
            strcpy(strOnlyFileName, p);
            sprintf(strOnlyFrameFileName, "%s_Frame.hy", p);
            len = strlen(strFileName) - strlen(p);
            memcpy(strOnlySourcePath, strFileName, len);
        }

        if (!strncmp(*argv, "-line", 5))
        {
            bPrintLine = GL_TRUE;
        }

        if (!strncmp(*argv, "-png", 5))
        {
            bUsePng = GL_TRUE;
        }

        if (!strncmp(*argv, "-thread", 7))
        {
            bThread = GL_TRUE;
        }

        if (!strncmp(*argv, "-uniform", 7))
        {
            bPrintUniform = GL_TRUE;
        }

        if (!strncmp(*argv, "-egl", 4))
        {
            bPrintEGL = GL_TRUE;
        }

        if (!strncmp(*argv, "-es11", 4))
        {
            bPrintES11 = GL_TRUE;
        }

        if (!strncmp(*argv, "-es20", 4))
        {
            bPrintES20 = GL_TRUE;
        }

        if (!strncmp(*argv, "-shader", 7))
        {
            bPrintDrawShader = GL_TRUE;
        }

        if (!strncmp(*argv, "-vertex", 7))
        {
            bPrintVertexAttrib = GL_TRUE;
        }

        if (!strncmp(*argv, "-start", 6))
        {
            argv ++;
            i ++;

            nStartFrame = atoi(*argv);
        }

        if (!strncmp(*argv, "-end", 4))
        {
            argv ++;
            i ++;

            nEndFrame = atoi(*argv);
        }

        if (!strncmp(*argv, "-save_texture", 13))
        {
            g_bSaveTexture = GL_TRUE;
        }

        if (!strncmp(*argv, "-frame_file", 11))
        {
            bDumpFrameFile = GL_TRUE;
        }

        if (!strncmp(*argv, "-matrix", 7))
        {
            bPrintMatrix = GL_TRUE;
        }

        if (!strncmp(*argv, "-save_shader", 12))
        {
            g_bSaveShader = GL_TRUE;
        }

        if (!strncmp(*argv, "-width", 5))
        {
            argv ++;
            i ++;

            eglWidth = atoi(*argv);
        }

        if (!strncmp(*argv, "-height", 6))
        {
            argv ++;
            i ++;

            eglHeight = atoi(*argv);
        }

        if (!strncmp(*argv, "-flush", 6))
        {
            bFlushFile = GL_TRUE;
        }
    }

    return GL_TRUE;
}

GLboolean OutputToTarget(OUTPUT_TARGET  target,
                         GLboolean      bEventSeq,
                         GLuint         nEventSeq,
                         const GLchar   *pThread,
                         const GLchar   *pOutput,
                         GLvoid         *pTarget)
{
    GLboolean bRes = GL_FALSE;

    if (g_bOutputToFile)
    {
        bRes = OutputToFile(bEventSeq, nEventSeq, pThread, pOutput, (FILE*)pTarget);
    }
    else
    {
        switch (target)
        {
        case OUTPUT_TO_FRAME_STRING:
            AddStringToMap(nEventSeq, pOutput, &g_frameStringMap);
            bRes = GL_TRUE;
            break;

        case OUTPUT_TO_DRAW_STRING:
            AddStringToMap(++g_drawStringCount, pOutput, &g_drawStringMap);
            bRes = GL_TRUE;
            break;

        case OUTPUT_TO_TOTAL_STRING:
            AddStringToMap(++g_totalStringCount, pOutput, &g_totalStringMap);
            bRes = GL_TRUE;
            break;

        case OUTPUT_TO_DRAW_TOTAL_STRING:
            AddStringToMap(++g_totalStringCount, pOutput, &g_totalStringMap);
            AddStringToMap(++g_drawStringCount, pOutput, &g_drawStringMap);
            bRes = GL_TRUE;
            break;

        case OUTPUT_TO_FILE:
            bRes = GL_TRUE;
            break;

        default:
            break;
        }
    }

    return bRes;
}

GLboolean OutputToFile(GLboolean    bEventSeq,
                       GLuint       eventSeq,
                       const GLchar *thread,
                       const GLchar *output,
                       FILE         *pFile)
{
    GLint   nWrittenBytes   = 0;
    GLint   nLen            = 0;
    GLchar  LineNum[16];

    if (pFile)
    {
        if (bEventSeq)
        {
            memset(LineNum, 0, 16);
            sprintf(LineNum, "[#%08d]", eventSeq);
            fwrite(LineNum, 1, strlen(LineNum), pFile);
        }
        else
        {
            bPrintContent = GL_TRUE;
        }

        nLen            = strlen(output);
        nWrittenBytes   = fwrite(output, 1, nLen, pFile);
        if (nWrittenBytes != nLen)
        {
            printf("Write cmd buffer file error\n");
            return GL_FALSE;
        }
    }

    if (bFlushFile)
        fflush(pFile);

    return GL_TRUE;
}

GLvoid OutputToShaderFile(GLchar *output)
{
    if (bOutputToShaderFile)
    {
        OutputToFile(GL_FALSE, 0, NULL, output, g_pShaderFile);
    }
}

GLuint * readUIntCount(GLint* count)
{
    const GLchar    *line;
    GLuint          *buffer;
    GLuint          current;
    GLuint          data[4];
    GLint           index;
    GLint           n;
    GLvoid          *temp;

    if (*count == 0)
    {
        buffer = NULL;
    }
    else
    {
        ALLOC(temp, *count * 4);
        buffer = (GLuint*) temp;
    }

    for (index = 0;;)
    {
        line = readLineFast();
        if (line == NULL)
        {
            return NULL;
        }

        n = sscanf(line, "$$ 0x%08X: 0x%08X 0x%08X 0x%08X 0x%08X",
                   &current, &data[0], &data[1], &data[2], &data[3]) - 1;

        if (n > 0)
        {
            if (*count == 0)
            {
                buffer = (GLuint *) realloc(buffer, 4 * (index + n));
                if (buffer == NULL)
                {
                    return NULL;
                }
                memcpy(buffer + index, data, n * 4);
                index += n;
            }
            else
            {
                if (index + n > *count)
                {
                    return NULL;
                }
                memcpy(buffer + index, data, 4 * n);
                index += n;
            }
        }

        else if (strncmp(line, "$$ <nil>", 8) == 0)
        {
            if (index == 0)
            {
                if (buffer != NULL)
                {
                    free(buffer);
                    buffer = NULL;
                }
            }
            else if (buffer != NULL)
            {
                return NULL;
            }
        }

        else if (strncmp(line, "$$ **********", 13) == 0)
        {
            if ((*count != 0) && (index != *count) && (buffer != NULL))
            {
                return NULL;
            }

            *count = index;
            return buffer;
        }

        else if (strncmp(line, "$}", 2) == 0)
        {
            if ((*count != 0) && (index != *count) && (buffer != NULL))
            {
                FREE(buffer);
                return NULL;
            }

            *count = index;
            return buffer;
        }
    }
}

GLuint * readUIntCountFast(int * count)
{
    const GLchar    *line;
    GLuint          *buffer;
    GLuint          data[4];
    GLint           index, n;
    GLvoid          *temp;
    GLchar          *p;
    GLint           dIndex;
    GLint           d;

    if (*count == 0)
    {
        buffer = NULL;
    }
    else
    {
        ALLOC(temp, *count * 4);
        buffer = (GLuint*) temp;
    }

    for (index = 0;;)
    {
        line = readLineFast();
        if (line == NULL)
        {
            return NULL;
        }

        //n = sscanf(line, "$$ 0x%08X: 0x%08X 0x%08X 0x%08X 0x%08X",
        //           &current, &data[0], &data[1], &data[2], &data[3]) - 1;
        n = 0;
        if (!strncmp(line, "$$ 0x", 5))
        {
            p       = (char*)&line[15];
            dIndex  = 0;
            while (*p != '\0')
            {
                if (p[0] == '0' && p[1] == 'x')
                {
                    d = 0;
                    for (int j=0; j<8; j++)
                    {
                        d <<= 4;
                        d += Char2Int(p[j+2]);
                    }
                    data[dIndex++] = d;
                    n ++;
                    p += 10;
                }
                else
                {
                    p ++;
                }
            }
        }

        if (n > 0)
        {
            if (*count == 0)
            {
                buffer = (GLuint *) realloc(buffer, 4 * (index + n));
                if (buffer == NULL)
                {
                    return NULL;
                }
                memcpy(buffer + index, data, n * 4);
                index += n;
            }
            else
            {
                if (index + n > *count)
                {
                    return NULL;
                }
                memcpy(buffer + index, data, 4 * n);
                index += n;
            }
        }

        else if (strncmp(line, "$$ <nil>", 8) == 0)
        {
            if (index == 0)
            {
                if (buffer != NULL)
                {
                    free(buffer);
                    buffer = NULL;
                }
            }
            else if (buffer != NULL)
            {
                return NULL;
            }
        }

        else if (strncmp(line, "$$ **********", 13) == 0)
        {
            if ((*count != 0) && (index != *count) && (buffer != NULL))
            {
                return NULL;
            }

            *count = index;
            return buffer;
        }

        else if (strncmp(line, "$}", 2) == 0)
        {
            if ((*count != 0) && (index != *count) && (buffer != NULL))
            {
                FREE(buffer);
                return NULL;
            }

            *count = index;
            return buffer;
        }
    }
}

GLuint * readUInt(int count)
{
    return readUIntCountFast(&count);
}

char* readLine(void)
{
    static char lineBuffer[256];
    char *p;

    while (fgets(lineBuffer, sizeof(lineBuffer), g_pApiDumpFile) != NULL)
    {
        nLineNum++;

        if ((p = strstr(lineBuffer, "${")) != NULL)
        {
             return p;
        }

        if ((p = strstr(lineBuffer, "$}")) != NULL)
        {
           return p;
        }

        if ((p = strstr(lineBuffer, "$$")) != NULL)
        {
           return p;
        }
    }

    return NULL;
}

char* readLineFast(void)
{
    static char lineBuffer[256];
    char *p;

    if (!g_pApiDumpFile)
    {
        return NULL;
    }

    while (fgets(lineBuffer, sizeof(lineBuffer), g_pApiDumpFile) != NULL)
    {
        nLineNum++;

        p = lineBuffer;
        while (*p != '\0')
        {
            if (p[0] == '$')
            {
                if (p[1] == '{' || p[1] == '}' || p[1] == '$')
                {
                    return p;
                }
            }

            p ++;
        }
    }

    return NULL;
}

GLchar* readUCharFast(GLint count)
{
    const GLchar    *line;
    GLchar          *buffer;
    GLuint          data[8];
    GLint           index;
    GLint           n;
    GLint           i;
    GLchar          *p;
    GLubyte         d;
    GLint           dIndex = 0;

    if (count == 0)
    {
        buffer = NULL;
    }
    else
    {
        buffer = (char *) malloc(count);
        if (buffer == NULL)
        {
            return NULL;
        }
    }

    for (index = 0;;)
    {
        line = readLineFast();
        if (line == NULL)
        {
            return NULL;
        }

        //n = sscanf(line, "$$ 0x%08X: 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X "
        //           "0x%02X 0x%02X",
        //           &current, &data[0], &data[1], &data[2], &data[3], &data[4],
        //           &data[5], &data[6], &data[7]) - 1;

        n = 0;
        if (strstr(line, "$$ 0x"))
        {
            p = (char*)&line[15];
            dIndex = 0;
            while (*p != '\0')
            {
                if (p[0] == '0' && p[1] == 'x')
                {
                    d = Char2Int(p[2]) * 16 + Char2Int(p[3]);
                    p += 4;
                    data[dIndex++] = d;
                    n ++;
                }
                else
                {
                    p ++;
                }
            }
        }

        if (n > 0)
        {
            if (count == 0)
            {
                buffer = (char *) realloc(buffer, index + n);
                if (buffer == NULL)
                {
                    return NULL;
                }
            }
            else if (index + n > count)
            {
                return NULL;
            }

            for (i = 0; i < n; ++i)
            {
                buffer[index++] = (unsigned char) data[i];
            }
        }
        else if (strncmp(line, "$$ <nil>", 8) == 0)
        {
            if (index == 0)
            {
                if (buffer != NULL)
                {
                    free(buffer);
                    buffer = NULL;
                }
            }
            else if (buffer != NULL)
            {
                return NULL;
            }
        }

        else if (strncmp(line, "$$ **********", 13) == 0)
        {
            if ((count != 0) && (index != count) && (buffer != NULL))
            {
                return NULL;
            }

            if (index != 0)
            {
                GLchar  *tmp = (GLchar*)malloc(index+1);

                memset(tmp, 0, index+1);
                memcpy(tmp, buffer, index);
                free(buffer);

                buffer = tmp;
            }

            return buffer;
        }
        else
        {
            return NULL;
        }
    }
}

GLchar* readUChar(GLint count)
{
    const GLchar    *line;
    GLchar          *buffer;
    GLuint          current;
    GLuint          data[8];
    GLint           index;
    GLint           n;
    GLint           i;

    if (count == 0)
    {
        buffer = NULL;
    }
    else
    {
        buffer = (char *) malloc(count);
        if (buffer == NULL)
        {
            return NULL;
        }
    }

    for (index = 0;;)
    {
        line = readLineFast();
        if (line == NULL)
        {
            return NULL;
        }

        n = sscanf(line, "$$ 0x%08X: 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X "
                   "0x%02X 0x%02X",
                   &current, &data[0], &data[1], &data[2], &data[3], &data[4],
                   &data[5], &data[6], &data[7]) - 1;

        if (n > 0)
        {
            if (count == 0)
            {
                buffer = (char *) realloc(buffer, index + n);
                if (buffer == NULL)
                {
                    return NULL;
                }
            }
            else if (index + n > count)
            {
                return NULL;
            }

            for (i = 0; i < n; ++i)
            {
                buffer[index++] = (unsigned char) data[i];
            }
        }
        else if (strncmp(line, "$$ <nil>", 8) == 0)
        {
            if (index == 0)
            {
                if (buffer != NULL)
                {
                    free(buffer);
                    buffer = NULL;
                }
            }
            else if (buffer != NULL)
            {
                return NULL;
            }
        }

        else if (strncmp(line, "$$ **********", 13) == 0)
        {
            if ((count != 0) && (index != count) && (buffer != NULL))
            {
                return NULL;
            }

            if (index != 0)
            {
                GLchar  *tmp = (GLchar*)malloc(index+1);

                memset(tmp, 0, index+1);
                memcpy(tmp, buffer, index);
                free(buffer);

                buffer = tmp;
            }

            return buffer;
        }
        else
        {
            return NULL;
        }
    }
}

GLvoid Print(enLogLevel level, const char *msg, ...)
{
    va_list arguments;
    GLchar  buf[1024];

    if (level < printLevel)
        return;

    va_start(arguments, msg);
    vsnprintf(buf, 1024, msg, arguments);
    va_end(arguments);

    printf("%s", buf);
}

GLvoid OutputStrcat(GLchar* outBuf, GLint outputSize, const GLchar* string, ...)
{
    va_list arguments;

    va_start(arguments, string);
    vsnprintf(tmp6, TMP5_BUF_SIZE, string, arguments);
    strcat(outBuf, tmp6);
    va_end(arguments);
}

// For performance, output to specific position.
void OutputStrcatFast(int pos, char* outBuf, int outputSize, const char* string, ...)
{
    va_list arguments;

    va_start(arguments, string);
    vsnprintf(tmp6, TMP5_BUF_SIZE, string, arguments);
    strcpy(&outBuf[pos], tmp6);
    va_end(arguments);
}

void SetDumpFile(int location)
{
    fseek(g_pApiDumpFile, 0, location);
}

GLboolean OpenDumpFile()
{
    char    strOutputFile[1024];
    char    strDrawFile[1024];
    char    strFrameFile[1024];
    char    strShaderFile[1024];
    char    strTextureFile[1024];

    memset(strOutputFile, 0, 1024);
    memset(strDrawFile, 0, 1024);
    memset(strFrameFile, 0, 1024);

    sprintf(strOutputFile, "%s.hy", strFileName);
    sprintf(strDrawFile, "%s_Draw.hy", strFileName);
    sprintf(strFrameFile, "%s_Frame.hy", strFileName);
    sprintf(strShaderFile, "%s_shader.hy", strFileName);
    sprintf(strTextureFile, "%s_texture.hy", strFileName);

    g_pOutputFile = fopen(strOutputFile, "w");
    if (g_pOutputFile == NULL)
    {
        printf("Cannot create output file: %s\n", strOutputFile);
        goto _Error;
    }

    g_pDrawFile = fopen(strDrawFile, "w");
    if (g_pDrawFile == NULL)
    {
        printf("Cannot create draw file: %s\n", strDrawFile);
        goto _Error;
    }

    g_pFrameFile = fopen(strFrameFile, "w");
    if (g_pFrameFile == NULL)
    {
        printf("Cannot create frame file: %s\n", strFrameFile);
        goto _Error;
    }

    g_pShaderFile = fopen(strShaderFile, "w");
    if (g_pShaderFile == NULL)
    {
        printf("Cannot create shader file: %s\n", strShaderFile);
        goto _Error;
    }

    g_pTextureFile = fopen(strTextureFile, "w");
    if (g_pTextureFile == NULL)
    {
        printf("Cannot create texture file: %s\n", strTextureFile);
        goto _Error;
    }

    APIInitNameMap();

    return GL_TRUE;

_Error:
    return GL_FALSE;
}

void CloseDumpFile()
{
    if (g_pApiDumpFile)
    {
        fclose(g_pApiDumpFile);
    }

    if (g_pOutputFile)
    {
        fclose(g_pOutputFile);
    }

    if (g_pDrawFile)
    {
        fclose(g_pDrawFile);
    }

    if (g_pFrameFile)
    {
        fclose(g_pFrameFile);
    }

    if (g_pShaderFile)
    {
        fclose(g_pShaderFile);
    }

    if (g_pTextureFile)
    {
        fclose(g_pTextureFile);
    }
}

/*
 * 'read_word()' - Read a 16-bit unsigned integer.
 */
#if 0
static GLushort read_word(FILE *fp)
{
    GLubyte b0, b1; /* Bytes from file */

    b0 = getc(fp);
    b1 = getc(fp);

    return ((b1 << 8) | b0);
}

/*
 * 'read_dword()' - Read a 32-bit unsigned integer.
 */
static GLuint read_dword(FILE *fp)
{
    GLubyte b0, b1, b2, b3; /* Bytes from file */

    b0 = getc(fp);
    b1 = getc(fp);
    b2 = getc(fp);
    b3 = getc(fp);

    return ((((((b3 << 8) | b2) << 8) | b1) << 8) | b0);
}

/*
 * 'read_long()' - Read a 32-bit signed integer.
 */
static int read_long(FILE *fp)
{
    unsigned char b0, b1, b2, b3; /* Bytes from file */

    b0 = getc(fp);
    b1 = getc(fp);
    b2 = getc(fp);
    b3 = getc(fp);

    return ((int)(((((b3 << 8) | b2) << 8) | b1) << 8) | b0);
}

#endif

/*
 * 'write_word()' - Write a 16-bit unsigned integer.
 */
static GLint write_word(FILE *fp, GLushort w)
{
    putc(w, fp);
    return (putc(w >> 8, fp));
}

/*
 * 'write_dword()' - Write a 32-bit unsigned integer.
 */
static GLint write_dword(FILE *fp, GLuint dw)
{
    putc(dw, fp);
    putc(dw >> 8, fp);
    putc(dw >> 16, fp);
    return (putc(dw >> 24, fp));
}

/*
 * 'write_long()' - Write a 32-bit signed integer.
 */
static GLint write_long(FILE *fp, GLint l)
{
    putc(l, fp);
    putc(l >> 8, fp);
    putc(l >> 16, fp);
    return (putc(l >> 24, fp));
}

int SaveDIBitmap(const GLchar *filename, BMPINFO *info, GLubyte *bits, GLint alignedWidth)
{
    FILE    *fp = NULL;                 /* Open file pointer */
    GLuint  size;                       /* Size of file */
    GLuint  infosize;                   /* Size of bitmap info */
    GLuint  bitsize;                    /* Size of bitmap pixels */
    GLint   i;
    GLint   srcStride;
    GLint   dstStride;

    fp = fopen(filename, "w");
    if (fp == NULL)
    {
        /* Failed to open the file. */
        printf("*ERROR*  Failed to open the file %s", filename);

        return -1;
    }

    /* Figure out the bitmap size */
    if (info->bmiHeader.biSizeImage == 0)
    {
        bitsize =  info->bmiHeader.biWidth *
                    ((info->bmiHeader.biBitCount + 7) / 8) *
                abs(info->bmiHeader.biHeight);
    }
    else
    {
        bitsize = info->bmiHeader.biSizeImage;
    }

    srcStride  =  info->bmiHeader.biWidth * ((info->bmiHeader.biBitCount + 7) / 8);
    dstStride  =  alignedWidth * ((info->bmiHeader.biBitCount + 7) / 8);

    /* Figure out the header size */
    infosize = sizeof(BMPINFOHEADER);
    switch (info->bmiHeader.biCompression)
    {
    case BIT_BITFIELDS :
        infosize += 12; /* Add 3 RGB doubleword masks */
        if (info->bmiHeader.biClrUsed == 0)
        break;
    case BIT_RGB :
        if (info->bmiHeader.biBitCount > 8 &&
        info->bmiHeader.biClrUsed == 0)
        break;
    case BIT_RLE8 :
    case BIT_RLE4 :
        if (info->bmiHeader.biClrUsed == 0)
            infosize += (1 << info->bmiHeader.biBitCount) * 4;
        else
            infosize += info->bmiHeader.biClrUsed * 4;
        break;
    }

    size = sizeof(BMPFILEHEADER) + infosize + bitsize;

    /* Write the file header, bitmap information, and bitmap pixel data... */
    write_word(fp, BF_TYPE);        /* bfType */
    write_dword(fp, size);          /* bfSize */
    write_word(fp, 0);              /* bfReserved1 */
    write_word(fp, 0);              /* bfReserved2 */
    write_dword(fp, 14 + infosize); /* bfOffBits */

    write_dword(fp, info->bmiHeader.biSize);
    write_long(fp, info->bmiHeader.biWidth);
    write_long(fp, abs(info->bmiHeader.biHeight)/*info->bmiHeader.biHeight*/); // bugbug: keep height positive
    write_word(fp, info->bmiHeader.biPlanes);
    write_word(fp, info->bmiHeader.biBitCount);
    write_dword(fp, info->bmiHeader.biCompression);
    write_dword(fp, info->bmiHeader.biSizeImage);
    write_long(fp, info->bmiHeader.biXPelsPerMeter);
    write_long(fp, info->bmiHeader.biYPelsPerMeter);
    write_dword(fp, info->bmiHeader.biClrUsed);
    write_dword(fp, info->bmiHeader.biClrImportant);

    if ((infosize > 40)
        && (fwrite(info->bmiColors, infosize - 40, 1, fp) < 1))
    {
        /* Couldn't write the bitmap header - return... */
        fclose(fp);
        return (-1);
    }

    for (i = 0; i < abs(info->bmiHeader.biHeight); i++)
    {
        if (info->bmiHeader.biHeight > 0)
            fwrite(bits + dstStride * i, 1, srcStride, fp);
        else
            fwrite(bits + dstStride * (abs(info->bmiHeader.biHeight) - 1 - i), 1, srcStride, fp);
    }

    /* OK, everything went fine - return... */
    fclose(fp);
    return (0);
}

GLboolean SaveTexture2DIB(GLint          format,
                          GLenum         type,
                          GLsizei        width,
                          GLsizei        height,
                          const GLchar   *bmpFileName,
                          GLvoid         *pixels)
{
    BMPINFO         bitmap;
    GLint           bitCount;
    gceSURF_FORMAT  imageFormat = gcvSURF_UNKNOWN;

    switch (type)
    {
        case GL_UNSIGNED_BYTE:
        case GL_UNSIGNED_SHORT:
        case GL_UNSIGNED_INT:
        case GL_UNSIGNED_SHORT_4_4_4_4:
        case GL_UNSIGNED_SHORT_5_5_5_1:
        case GL_UNSIGNED_SHORT_5_6_5:
        case GL_UNSIGNED_INT_2_10_10_10_REV:
        case GL_UNSIGNED_INT_24_8:
        case GL_HALF_FLOAT:
        case GL_FLOAT:
        case GL_DEPTH_COMPONENT24:
        case GL_DEPTH_COMPONENT32F:
            break;

        default:
            goto _Error;
    }

    /* Now we only support to dump GL_RGB and GL_RGBA */
    switch (format)
    {
    case GL_DEPTH_COMPONENT:
        goto _Error;

    case GL_ALPHA:
        goto _Error;

    case GL_RGB:
        switch (type)
        {
        case GL_UNSIGNED_SHORT_4_4_4_4:
            imageFormat = gcvSURF_X4R4G4B4;
            break;

        case GL_UNSIGNED_SHORT_5_5_5_1:
            imageFormat = gcvSURF_X1R5G5B5;
            break;

        case GL_UNSIGNED_SHORT_5_6_5:
            imageFormat = gcvSURF_R5G6B5;
            break;

        case GL_UNSIGNED_BYTE:
            imageFormat = gcvSURF_B8G8R8;
            break;

        case GL_UNSIGNED_INT_2_10_10_10_REV:
            imageFormat = gcvSURF_X2B10G10R10;
            break;

        case GL_HALF_FLOAT:
            imageFormat = gcvSURF_X16B16G16R16F;
            break;
        }
        break;

    case GL_RGBA:
        switch (type)
        {
        case GL_UNSIGNED_BYTE:
            imageFormat = gcvSURF_A8B8G8R8;
            break;

        case GL_UNSIGNED_SHORT_4_4_4_4:
            imageFormat = gcvSURF_R4G4B4A4;
            break;

        case GL_UNSIGNED_SHORT_5_5_5_1:
            imageFormat = gcvSURF_R5G5B5A1;
            break;

        case GL_UNSIGNED_INT_2_10_10_10_REV:
            goto _Error;

        case GL_HALF_FLOAT:
            goto _Error;
        }
        break;

    case GL_LUMINANCE:
        goto _Error;

    case GL_LUMINANCE_ALPHA:
        goto _Error;

    //case GL_BGRA_EXT:
    //    switch (type)
    //    {
    //    case GL_UNSIGNED_BYTE:
    //        imageFormat = gcvSURF_A8R8G8B8;
    //        break;
    //    }
    //    break;

    case GL_DEPTH_STENCIL:
        goto _Error;

    default:
        goto _Error;
    }

    /* Did we find a valid Format-Type combination?. */
    if (imageFormat == gcvSURF_UNKNOWN)
    {
        goto _Error;
    }

    switch (imageFormat)
    {
    case gcvSURF_B8G8R8: /* test */
        bitmap.mask[0] = 0x00FF0000;
        bitmap.mask[1] = 0x0000FF00;
        bitmap.mask[2] = 0x000000FF;
        bitCount = 24;
        break;

    case gcvSURF_A8R8G8B8:
    case gcvSURF_X8R8G8B8:
        bitmap.mask[0] = 0x00FF0000;
        bitmap.mask[1] = 0x0000FF00;
        bitmap.mask[2] = 0x000000FF;
        bitCount = 32;
        break;

    case gcvSURF_R8G8B8A8:
    case gcvSURF_R8G8B8X8:
        bitmap.mask[0] = 0xFF000000;
        bitmap.mask[1] = 0x00FF0000;
        bitmap.mask[2] = 0x0000FF00;
        bitCount = 32;
        break;

    case gcvSURF_X8B8G8R8:
    case gcvSURF_A8B8G8R8:
        bitmap.mask[0] = 0x000000FF;
        bitmap.mask[1] = 0x0000FF00;
        bitmap.mask[2] = 0x00FF0000;
        bitCount = 32;
        break;

    case gcvSURF_B8G8R8A8:
    case gcvSURF_B8G8R8X8:
        bitmap.mask[0] = 0x0000FF00;
        bitmap.mask[1] = 0x00FF0000;
        bitmap.mask[2] = 0xFF000000;
        bitCount = 32;
        break;

    case gcvSURF_X4R4G4B4:
    case gcvSURF_A4R4G4B4:
        bitmap.mask[0] = 0x00000F00;
        bitmap.mask[1] = 0x000000F0;
        bitmap.mask[2] = 0x0000000F;
        bitCount = 16;
        break;

    case gcvSURF_R4G4B4X4:
    case gcvSURF_R4G4B4A4:
        bitmap.mask[0] = 0x0000F000;
        bitmap.mask[1] = 0x00000F00;
        bitmap.mask[2] = 0x000000F0;
        bitCount = 16;
        break;

    case gcvSURF_X4B4G4R4:
    case gcvSURF_A4B4G4R4:
        bitmap.mask[0] = 0x0000000F;
        bitmap.mask[1] = 0x000000F0;
        bitmap.mask[2] = 0x00000F00;
        bitCount = 16;
        break;

    case gcvSURF_B4G4R4X4:
    case gcvSURF_B4G4R4A4:
        bitmap.mask[0] = 0x000000F0;
        bitmap.mask[1] = 0x00000F00;
        bitmap.mask[2] = 0x0000F000;
        bitCount = 16;
        break;

    case gcvSURF_R5G6B5:
        bitmap.mask[0] = 0x0000F800;
        bitmap.mask[1] = 0x000007E0;
        bitmap.mask[2] = 0x0000001F;
        bitCount = 16;
        break;

    case gcvSURF_B5G6R5:
        bitmap.mask[0] = 0x0000001F;
        bitmap.mask[1] = 0x000007E0;
        bitmap.mask[2] = 0x0000F800;
        bitCount = 16;
        break;

    case gcvSURF_A1R5G5B5:
    case gcvSURF_X1R5G5B5:
        bitmap.mask[0] = 0x00007C00;
        bitmap.mask[1] = 0x000003E0;
        bitmap.mask[2] = 0x0000001F;
        bitCount = 16;
        break;

    case gcvSURF_R5G5B5X1:
    case gcvSURF_R5G5B5A1:
        bitmap.mask[0] = 0x0000F800;
        bitmap.mask[1] = 0x000007C0;
        bitmap.mask[2] = 0x0000003E;
        bitCount = 16;
        break;

    case gcvSURF_B5G5R5X1:
    case gcvSURF_B5G5R5A1:
        bitmap.mask[0] = 0x0000003E;
        bitmap.mask[1] = 0x000007C0;
        bitmap.mask[2] = 0x0000F800;
        bitCount = 16;
        break;

    case gcvSURF_X1B5G5R5:
    case gcvSURF_A1B5G5R5:
        bitmap.mask[0] = 0x0000001F;
        bitmap.mask[1] = 0x000003E0;
        bitmap.mask[2] = 0x00007C00;
        bitCount = 16;
        break;

    default:
        // can not save and display
        goto _Error;
    }

    /* Fill in the BITMAPINFOHEADER information. */
    bitmap.bmiHeader.biSize             = sizeof(bitmap.bmiHeader);
    bitmap.bmiHeader.biWidth            = width;
    bitmap.bmiHeader.biHeight           = -(GLint)height;
    bitmap.bmiHeader.biPlanes           = 1;
    bitmap.bmiHeader.biBitCount         = bitCount;
    bitmap.bmiHeader.biCompression      = BIT_BITFIELDS;
    bitmap.bmiHeader.biSizeImage        = 0;
    bitmap.bmiHeader.biXPelsPerMeter    = 0;
    bitmap.bmiHeader.biYPelsPerMeter    = 0;
    bitmap.bmiHeader.biClrUsed          = 0;
    bitmap.bmiHeader.biClrImportant     = 0;

    if (SaveDIBitmap(bmpFileName, (BMPINFO *)&bitmap, (unsigned char *)pixels, width) == 0)
    {
        goto _Error;
    }

    return TRUE;

_Error:

    return FALSE;
}

/************************************************************************************************
 *                                                                                              *
 * red-black tree for dictionary                                                                *
 *                                                                                              *
 ************************************************************************************************/
void InsertCase5(DictionaryNode *node);
void InsertCase4(DictionaryNode *node);
void InsertCase3(DictionaryNode *node);
void InsertCase2(DictionaryNode *node);
void InsertCase1(DictionaryNode *node);

DictionaryNode* DictCreateNode(int nameCode, int outputSize)
{
    DictionaryNode  *p = (DictionaryNode*)malloc(sizeof(DictionaryNode));

    if (p == NULL)
    {
        printf("DictCreateNode: cannot allocate memory for node.\n");
    }

    p->nameLen      = 0;
    p->nameCode     = nameCode;
    p->outputSize   = outputSize;
    p->name         = NULL;

    return p;
}

DictionaryNode* DictCreateNode(int nameCode, int outputSize, char *name)
{
    DictionaryNode  *p = (DictionaryNode*)malloc(sizeof(DictionaryNode));

    if (p == NULL)
    {
        printf("DictCreateNode: cannot allocate memory for node.\n");
        return p;
    }

    p->nameLen      = strlen(name);
    p->nameCode     = nameCode;
    p->outputSize   = outputSize;
    p->name         = (char*)malloc(p->nameLen+1);
    memset(p->name, 0, p->nameLen+1);
    memcpy(p->name, name, p->nameLen);

    return p;
}

DictionaryNode* DictFindSpecificNode(DictionaryNode *node, int code)
{
    if (node && node->nameCode == code)
    {
        return node;
    }

    if (node && node->nameCode > code)
    {
        return DictFindSpecificNode(node->left, code);
    }

    if (node && node->nameCode < code)
    {
        return DictFindSpecificNode(node->right, code);
    }

    return NULL;
}

void LeftRotate(DictionaryNode *node)
{
    DictionaryNode  *parent     = NULL;
    DictionaryNode  *rightChild = NULL;
    DictionaryNode  *rightLeft  = NULL;

    if (!node->right)
    {
        printf("%s(%d): right is null.\n", __FUNCTION__, __LINE__);
        return;
    }

    parent      = node->parent;
    rightChild  = node->right;
    rightLeft   = rightChild->left;

    if (parent)
    {
        if (parent->left == node)
        {
            parent->left = rightChild;
        }
        else if (parent->right == node)
        {
            parent->right = rightChild;
        }

        rightChild->parent  = parent;
    }
    else
    {
        rightChild->parent = NULL;
    }

    rightChild->left = node;
    node->parent = rightChild;

    node->right = rightLeft;
    if (rightLeft)
    {
        rightLeft->parent = node;
    }
}

void RightRotate(DictionaryNode *node)
{
    DictionaryNode  *leftChild  = NULL;
    DictionaryNode  *parent     = NULL;
    DictionaryNode  *leftRight  = NULL;

    if (!node->left)
    {
        printf("%s(%d): left is null.\n", __FUNCTION__, __LINE__);
        return;
    }

    parent      = node->parent;
    leftChild   = node->left;
    leftRight   = leftChild->right;

    if (parent)
    {
        if (parent->right == node)
        {
            parent->right = leftChild;
        }
        else if (parent->left == node)
        {
            parent->left = leftChild;
        }

        leftChild->parent   = parent;
    }
    else
    {
        leftChild->parent = NULL;
    }

    leftChild->right = node;
    node->parent = leftChild;

    node->left = leftRight;
    if (leftRight)
    {
        leftRight->parent = node;
    }
}

void ReplaceRoot(DictionaryNode **pRoot)
{
    DictionaryNode   *root = *pRoot;

    while (root->parent)
    {
        root = root->parent;
    }

    *pRoot = root;
}

DictionaryNode* GrandParent(DictionaryNode* n)
{
    return n->parent->parent;
}

DictionaryNode* Uncle(DictionaryNode* n)
{
    if (GrandParent(n) && (n->parent == GrandParent(n)->left))
    {
        return GrandParent(n)->right;
    }
    else if (GrandParent(n))
    {
        return GrandParent(n)->left;
    }

    return NULL;
}

void InsertNode(DictionaryNode **pRoot, DictionaryNode **pParent, DictionaryNode *node)
{
    DictionaryNode  *root   = *pRoot;
    DictionaryNode  *parent = *pParent;

    // root is null
    if (!root)
    {
        *pRoot          = node;
        node->parent    = NULL;
        node->left      = NULL;
        node->right     = NULL;
        node->color     = BLACK;
        return;
    }

    if (node->nameCode < parent->nameCode)
    {
        if (parent->left == NULL)
        {
            parent->left    = node;
            node->parent    = parent;
            node->left      = NULL;
            node->right     = NULL;
            node->color     = RED;
            InsertCase1(node);

            if (*pParent == *pRoot)
            {
                ReplaceRoot(pRoot);
            }

            return;
        }

        InsertNode(pRoot, &parent->left, node);
    }

    if (node->nameCode > parent->nameCode)
    {
        if (parent->right == NULL)
        {
            parent->right   = node;
            node->parent    = parent;
            node->left      = NULL;
            node->right     = NULL;
            node->color     = RED;
            InsertCase1(node);

            if (*pParent == *pRoot)
            {
                ReplaceRoot(pRoot);
            }

            return;
        }

        InsertNode(pRoot, &parent->right, node);
    }

    if (*pParent == *pRoot)
    {
        ReplaceRoot(pRoot);
    }
}

void InsertCase1(DictionaryNode *node)
{
     if (node->parent == NULL)
     {
         node->color = BLACK;
     }
     else
     {
         InsertCase2(node);
     }
}

void InsertCase2(DictionaryNode *node)
{
     if (node->parent->color == BLACK)
     {
         return; /* 树仍旧有效 */
     }
     else
     {
         InsertCase3(node);
     }
}

void InsertCase3(DictionaryNode *node)
{
    if (Uncle(node) != NULL && Uncle(node)->color == RED)
    {
        node->parent->color         = BLACK;
        Uncle(node)->color          = BLACK;
        GrandParent(node)->color    = RED;
        InsertCase1(GrandParent(node));
    }
    else
    {
        InsertCase4(node);
    }
}

void InsertCase4(DictionaryNode *node)
{
    DictionaryNode   *grandParent = GrandParent(node);

    if (grandParent && (node == node->parent->right) && (node->parent == grandParent->left))
    {
        LeftRotate(node->parent);
        node = grandParent->left;
    }
    else if (grandParent && (node == node->parent->left) && (node->parent == grandParent->right))
    {
        RightRotate(node->parent);

        if (grandParent)
        {
            node = grandParent->right;
        }
    }

    InsertCase5(node);
}

void InsertCase5(DictionaryNode *node)
{
    node->parent->color         = BLACK;

    if (GrandParent(node))
    {
        GrandParent(node)->color    = RED;
    }

    if (GrandParent(node) && (node == node->parent->left) && (node->parent == GrandParent(node)->left))
    {
        RightRotate(GrandParent(node));
    }
    else if (GrandParent(node))
    {
        /* Here, n == n->parent->right && n->parent == grandparent(n)->right */
        LeftRotate(GrandParent(node));
    }
}

void OutputStringFast(int nameCode, int *pos, char *output, int outputSize, const char *fmt, ...)
{
    va_list         arguments;
    DictionaryNode  *pNode = NULL;

    va_start(arguments, fmt);
    vsnprintf(&output[*pos], outputSize-*pos, fmt, arguments);
    va_end(arguments);

    pNode = DictFindSpecificNode(dictRoot, nameCode);
    if (pNode == NULL)
    {
        pNode = DictCreateNode(nameCode, strlen(&output[*pos]));
        InsertNode(&dictRoot, &dictRoot, pNode);
    }

    switch (nameCode)
    {
    case dumpUniform4:
    case dumpUniform5:
    case dumpUniform9:
    case dumpUniform10:
    case dumpUniform11:
    case dumpVertex2:
    case dumpVertex8:
    case printShader3:
    case printShader6:
    case dumpData1:
    case gles2Disassemble2:
    case gles2Disassemble7:
    case gles2Disassemble5:
    case gles2Disassemble3:
    case gles2Disassemble1:
    case gles2Disassemble14:
    case gles2Disassemble11:
    case drawStates35:
    case drawStates36:
    case drawStates37:
        pNode->outputSize = strlen(&output[*pos]);
        break;
    }

    *pos += pNode->outputSize;
}

char int2charTable[] =
{
    '0', '1', '2', '3', '4', '5', '6', '7', '8', '9'
};

void Float2String(float f, char *buf)
{
    int         d       = *(int*)&f;
    int         sign    = (d & 0x80000000) ? 1 : 0;
    float       f1      = sign == 0 ? f : -f;
    int         f_int   = (int)f1;
    float       f_frag  = f1 - f_int;
    char        *p      = buf;
    GLboolean   bInt    = GL_FALSE;

    //if (f != 0 && f != 1.0 && f != -1.0)
    //    __asm int 3;

    if (sign == 1)
    {
        *p++ = '-';
    }

    if (f_int != 0)
    {
        for (int i=1000000000; i>0; i/=10)
        {
            d = f_int / i;
            if (d > 0 || bInt)
            {
                bInt = GL_TRUE;
                *p++ = int2charTable[d];
                f_int %= i;
            }
        }
    }
    else
    {
        *p++ = '0';
    }

    *p++ = '.';
    f_int = (int)(f_frag * 100000000);
    //if (f_int != 0)
    {
        for (int i=10000000; i>0; i/=10)
        {
            d = f_int / i;
            f_int %= i;
            *p++ = int2charTable[d];
        }
    }
    //else
    //{
    //    *p++ = int2charTable[0];
    //}
}

/* 将puc_data结构中的数据写入至png文件 */  
GLint WritePNGFile(GLchar *file_name, GLint width, GLint height, GLint bit_depth, GLchar *pixel, GLboolean hasAlpha)
{
    GLint       j;
    GLint       i;
    GLint       temp;
    GLint       pos;
    GLchar      *srcPixel = pixel;
    png_byte    color_type;
    png_structp png_ptr;
    png_infop   info_ptr;
    png_infop   write_end_info_ptr;
    png_bytep   *row_pointers;
    GLint       step = 0;
    GLint       res = 0;

    /* create file */
    FILE *fp = fopen(file_name, "wb");
    if (!fp)
    {
        //printf("[write_png_file] File %s could not be opened for writing\n", file_name);
        return -1;
    }

    /* initialize stuff */  
    png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);

    if (!png_ptr)
    {
        printf("[write_png_file] png_create_write_struct failed\n");
        return -1;
    }

    info_ptr = png_create_info_struct(png_ptr);
    write_end_info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr)
    {
        printf("[write_png_file] png_create_info_struct failed\n");  
        return -1;
    }

    if (setjmp(png_jmpbuf(png_ptr)))  
    {
        printf("[write_png_file] Error during init_io\n");  
        return -1;  
    }

    png_init_io(png_ptr, fp);

    /* write header */
    if (setjmp(png_jmpbuf(png_ptr)))
    {
        printf("[write_png_file] Error during writing header\n");  
        return -1;
    }

    if (hasAlpha)
    {
        color_type = PNG_COLOR_TYPE_RGB_ALPHA;
    }
    else
    {
        color_type = PNG_COLOR_TYPE_RGB;
    }

    png_set_IHDR(png_ptr, info_ptr, width, height,
                 bit_depth, color_type, PNG_INTERLACE_NONE,
                 PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);

    png_write_info(png_ptr, info_ptr);

    /* write bytes */  
    if (setjmp(png_jmpbuf(png_ptr)))
    {
        printf("[write_png_file] Error during writing bytes\n");
        return -1;
    }

    if (hasAlpha)
    {
        temp = (4 * width);
        step = 4;
    }
    else
    {
        temp = (3 * width);
        step = 3;
    }

    pos = 0;
    row_pointers = (png_bytep*)ApiAllocateTexMemory(height*sizeof(png_bytep));

    for (i = 0; i < height; i++)  
    {
        row_pointers[i] = (png_bytep)ApiAllocateTexMemory(sizeof(unsigned char)*temp);
        for (j = 0; j < temp; j += step)  
        {
            row_pointers[i][j]   = *srcPixel++;     // red
            row_pointers[i][j+1] = *srcPixel++;     // green
            row_pointers[i][j+2] = *srcPixel++;     // blue

            if (hasAlpha)
            {
                row_pointers[i][j+3] = *srcPixel++; // alpha
            }
            ++pos;
        }
    }

    png_write_image(png_ptr, row_pointers);

    /* end write */
    if (setjmp(png_jmpbuf(png_ptr)))
    {
        printf("[write_png_file] Error during end of write\n");
        res = -1;
        goto _Err;
    }

    png_write_end(png_ptr, NULL);

_Err:

    /* cleanup heap allocation */
    for (j=0; j<height; j++)
    {
        ApiReleaseTexMemory(row_pointers[j]);
    }
    ApiReleaseTexMemory(row_pointers);

    png_destroy_write_struct(&png_ptr, &info_ptr);

    if (fp)
    {
        fclose(fp);
    }

    return res;
}

GLboolean IsFileExist(GLchar *fileName)
{
    FILE        *p = NULL;
    GLboolean   res = GL_FALSE;

    p = fopen(fileName, "rb");
    if (p)
    {
        res = GL_TRUE;
        fclose(p);
    }

    return res;
}

GLvoid OutputDebugInfoToFile(const GLchar *str, ...)
{
    GLchar  buf[256];
    va_list arguments;

    memset(buf, 0, 256);
    va_start(arguments, str);
    vsnprintf(buf, TMP5_BUF_SIZE, str, arguments);
    va_end(arguments);
    OutputToFile(GL_FALSE, 0, "", buf, g_pOutputFile);
}

const GLchar* apiNameTable[GLES_FUNCTION_TOTAL_NUMBER];

GLvoid APIInitNameMap()
{
    apiNameTable[EGL_API_eglGetError]                     = "eglGetError";
    apiNameTable[EGL_API_eglGetDisplay]                   = "eglGetDisplay";
    apiNameTable[EGL_API_eglInitialize]                   = "eglInitialize";
    apiNameTable[EGL_API_eglTerminate]                    = "eglTerminate";
    apiNameTable[EGL_API_eglQueryString]                  = "eglQueryString";
    apiNameTable[EGL_API_eglGetConfigs]                   = "eglGetConfigs";
    apiNameTable[EGL_API_eglChooseConfig]                 = "eglChooseConfig";
    apiNameTable[EGL_API_eglGetConfigAttrib]              = "eglGetConfigAttrib";
    apiNameTable[EGL_API_eglCreateWindowSurface]          = "eglCreateWindowSurface";
    apiNameTable[EGL_API_eglCreatePbufferSurface]         = "eglCreatePbufferSurface";
    apiNameTable[EGL_API_eglCreatePixmapSurface]          = "eglCreatePixmapSurface";
    apiNameTable[EGL_API_eglDestroySurface]               = "eglDestroySurface";
    apiNameTable[EGL_API_eglQuerySurface]                 = "eglQuerySurface";
    apiNameTable[EGL_API_eglBindAPI]                      = "eglBindAPI";
    apiNameTable[EGL_API_eglQueryAPI]                     = "eglQueryAPI";
    apiNameTable[EGL_API_eglWaitClient]                   = "eglWaitClient";
    apiNameTable[EGL_API_eglReleaseThread]                = "eglReleaseThread";
    apiNameTable[EGL_API_eglCreatePbufferFromClientBuffer]= "eglCreatePbufferFromClientBuffer";
    apiNameTable[EGL_API_eglSurfaceAttrib]                = "eglSurfaceAttrib";
    apiNameTable[EGL_API_eglBindTexImage]                 = "eglBindTexImage";
    apiNameTable[EGL_API_eglReleaseTexImage]              = "eglReleaseTexImage";
    apiNameTable[EGL_API_eglSwapInterval]                 = "eglSwapInterval";
    apiNameTable[EGL_API_eglCreateContext]                = "eglCreateContext";
    apiNameTable[EGL_API_eglDestroyContext]               = "eglDestroyContext";
    apiNameTable[EGL_API_eglMakeCurrent]                  = "eglMakeCurrent";
    apiNameTable[EGL_API_eglGetCurrentContext]            = "eglGetCurrentContext";
    apiNameTable[EGL_API_eglGetCurrentSurface]            = "eglGetCurrentSurface";
    apiNameTable[EGL_API_eglGetCurrentDisplay]            = "eglGetCurrentDisplay";
    apiNameTable[EGL_API_eglQueryContext]                 = "eglQueryContext";
    apiNameTable[EGL_API_eglWaitGL]                       = "eglWaitGL";
    apiNameTable[EGL_API_eglWaitNative]                   = "eglWaitNative";
    apiNameTable[EGL_API_eglSwapBuffers]                  = "eglSwapBuffers";
    apiNameTable[EGL_API_eglCopyBuffers]                  = "eglCopyBuffers";
    apiNameTable[EGL_API_eglGetProcAddress]               = "eglGetProcAddress";
    apiNameTable[EGL_API_eglCreateImageKHR]               = "eglCreateImageKHR";
    apiNameTable[EGL_API_eglDestroyImageKHR]              = "eglDestroyImageKHR";
    apiNameTable[EGL_API_eglCreateSyncKHR]                = "eglCreateSyncKHR";
    apiNameTable[EGL_API_eglDestroySyncKHR]               = "eglDestroySyncKHR";
    apiNameTable[EGL_API_eglClientWaitSyncKHR]            = "eglClientWaitSyncKHR";
    apiNameTable[EGL_API_eglSignalSyncKHR]                = "eglSignalSyncKHR";
    apiNameTable[EGL_API_eglGetSyncAttribKHR]             = "eglGetSyncAttribKHR";
    apiNameTable[EGL_API_eglPatchID]                      = "eglPatchID";

    apiNameTable[GL1_API_glAlphaFunc]                     = "glAlphaFunc";
    apiNameTable[GL1_API_glClearColor]                    = "glClearColor";
    apiNameTable[GL1_API_glClearDepthf]                   = "glClearDepthf";
    apiNameTable[GL1_API_glClipPlanef]                    = "glClipPlanef";
    apiNameTable[GL1_API_glColor4f]                       = "glColor4f";
    apiNameTable[GL1_API_glDepthRangef]                   = "glDepthRangef";
    apiNameTable[GL1_API_glFogf]                          = "glFogf";
    apiNameTable[GL1_API_glFogfv]                         = "glFogfv";
    apiNameTable[GL1_API_glFrustumf]                      = "glFrustumf";
    apiNameTable[GL1_API_glGetClipPlanef]                 = "glGetClipPlanef";
    apiNameTable[GL1_API_glGetFloatv]                     = "glGetFloatv";
    apiNameTable[GL1_API_glGetLightfv]                    = "glGetLightfv";
    apiNameTable[GL1_API_glGetMaterialfv]                 = "glGetMaterialfv";
    apiNameTable[GL1_API_glGetTexEnvfv]                   = "glGetTexEnvfv";
    apiNameTable[GL1_API_glGetTexParameterfv]             = "glGetTexParameterfv";
    apiNameTable[GL1_API_glLightModelf]                   = "glLightModelf";
    apiNameTable[GL1_API_glLightModelfv]                  = "glLightModelfv";
    apiNameTable[GL1_API_glLightf]                        = "glLightf";
    apiNameTable[GL1_API_glLightfv]                       = "glLightfv";
    apiNameTable[GL1_API_glLineWidth]                     = "glLineWidth";
    apiNameTable[GL1_API_glLoadMatrixf]                   = "glLoadMatrixf";
    apiNameTable[GL1_API_glMaterialf]                     = "glMaterialf";
    apiNameTable[GL1_API_glMaterialfv]                    = "glMaterialfv";
    apiNameTable[GL1_API_glMultMatrixf]                   = "glMultMatrixf";
    apiNameTable[GL1_API_glMultiTexCoord4f]               = "glMultiTexCoord4f";
    apiNameTable[GL1_API_glNormal3f]                      = "glNormal3f";
    apiNameTable[GL1_API_glOrthof]                        = "glOrthof";
    apiNameTable[GL1_API_glPointParameterf]               = "glPointParameterf";
    apiNameTable[GL1_API_glPointParameterfv]              = "glPointParameterfv";
    apiNameTable[GL1_API_glPointSize]                     = "glPointSize";
    apiNameTable[GL1_API_glPolygonOffset]                 = "glPolygonOffset";
    apiNameTable[GL1_API_glRotatef]                       = "glRotatef";
    apiNameTable[GL1_API_glScalef]                        = "glScalef";
    apiNameTable[GL1_API_glTexEnvf]                       = "glTexEnvf";
    apiNameTable[GL1_API_glTexEnvfv]                      = "glTexEnvfv";
    apiNameTable[GL1_API_glTexParameterf]                 = "glTexParameterf";
    apiNameTable[GL1_API_glTexParameterfv]                = "glTexParameterfv";
    apiNameTable[GL1_API_glTranslatef]                    = "glTranslatef";
    apiNameTable[GL1_API_glActiveTexture]                 = "glActiveTexture";
    apiNameTable[GL1_API_glAlphaFuncx]                    = "glAlphaFuncx";
    apiNameTable[GL1_API_glBindBuffer]                    = "glBindBuffer";
    apiNameTable[GL1_API_glBindTexture]                   = "glBindTexture";
    apiNameTable[GL1_API_glBlendFunc]                     = "glBlendFunc";
    apiNameTable[GL1_API_glBufferData]                    = "glBufferData";
    apiNameTable[GL1_API_glBufferSubData]                 = "glBufferSubData";
    apiNameTable[GL1_API_glClear]                         = "glClear";
    apiNameTable[GL1_API_glClearColorx]                   = "glClearColorx";
    apiNameTable[GL1_API_glClearDepthx]                   = "glClearDepthx";
    apiNameTable[GL1_API_glClearStencil]                  = "glClearStencil";
    apiNameTable[GL1_API_glClientActiveTexture]           = "glClientActiveTexture";
    apiNameTable[GL1_API_glClipPlanex]                    = "glClipPlanex";
    apiNameTable[GL1_API_glColor4ub]                      = "glColor4ub";
    apiNameTable[GL1_API_glColor4x]                       = "glColor4x";
    apiNameTable[GL1_API_glColorMask]                     = "glColorMask";
    apiNameTable[GL1_API_glColorPointer]                  = "glColorPointer";
    apiNameTable[GL1_API_glCompressedTexImage2D]          = "glCompressedTexImage2D";
    apiNameTable[GL1_API_glCompressedTexSubImage2D]       = "glCompressedTexSubImage2D";
    apiNameTable[GL1_API_glCopyTexImage2D]                = "glCopyTexImage2D";
    apiNameTable[GL1_API_glCopyTexSubImage2D]             = "glCopyTexSubImage2D";
    apiNameTable[GL1_API_glCullFace]                      = "glCullFace";
    apiNameTable[GL1_API_glDeleteBuffers]                 = "glDeleteBuffers";
    apiNameTable[GL1_API_glDeleteTextures]                = "glDeleteTextures";
    apiNameTable[GL1_API_glDepthFunc]                     = "glDepthFunc";
    apiNameTable[GL1_API_glDepthMask]                     = "glDepthMask";
    apiNameTable[GL1_API_glDepthRangex]                   = "glDepthRangex";
    apiNameTable[GL1_API_glDisable]                       = "glDisable";
    apiNameTable[GL1_API_glDisableClientState]            = "glDisableClientState";
    apiNameTable[GL1_API_glDrawArrays]                    = "glDrawArrays";
    apiNameTable[GL1_API_glDrawElements]                  = "glDrawElements";
    apiNameTable[GL1_API_glEnable]                        = "glEnable";
    apiNameTable[GL1_API_glEnableClientState]             = "glEnableClientState";
    apiNameTable[GL1_API_glFinish]                        = "glFinish";
    apiNameTable[GL1_API_glFlush]                         = "glFlush";
    apiNameTable[GL1_API_glFogx]                          = "glFogx";
    apiNameTable[GL1_API_glFogxv]                         = "glFogxv";
    apiNameTable[GL1_API_glFrontFace]                     = "glFrontFace";
    apiNameTable[GL1_API_glFrustumx]                      = "glFrustumx";
    apiNameTable[GL1_API_glGetBooleanv]                   = "glGetBooleanv";
    apiNameTable[GL1_API_glGetBufferParameteriv]          = "glGetBufferParameteriv";
    apiNameTable[GL1_API_glGetClipPlanex]                 = "glGetClipPlanex";
    apiNameTable[GL1_API_glGenBuffers]                    = "glGenBuffers";
    apiNameTable[GL1_API_glGenTextures]                   = "glGenTextures";
    apiNameTable[GL1_API_glGetError]                      = "glGetError";
    apiNameTable[GL1_API_glGetFixedv]                     = "glGetFixedv";
    apiNameTable[GL1_API_glGetIntegerv]                   = "glGetIntegerv";
    apiNameTable[GL1_API_glGetLightxv]                    = "glGetLightxv";
    apiNameTable[GL1_API_glGetMaterialxv]                 = "glGetMaterialxv";
    apiNameTable[GL1_API_glGetPointerv]                   = "glGetPointerv";
    apiNameTable[GL1_API_glGetString]                     = "glGetString";
    apiNameTable[GL1_API_glGetTexEnviv]                   = "glGetTexEnviv";
    apiNameTable[GL1_API_glGetTexEnvxv]                   = "glGetTexEnvxv";
    apiNameTable[GL1_API_glGetTexParameteriv]             = "glGetTexParameteriv";
    apiNameTable[GL1_API_glGetTexParameterxv]             = "glGetTexParameterxv";
    apiNameTable[GL1_API_glHint]                          = "glHint";
    apiNameTable[GL1_API_glIsBuffer]                      = "glIsBuffer";
    apiNameTable[GL1_API_glIsEnabled]                     = "glIsEnabled";
    apiNameTable[GL1_API_glIsTexture]                     = "glIsTexture";
    apiNameTable[GL1_API_glLightModelx]                   = "glLightModelx";
    apiNameTable[GL1_API_glLightModelxv]                  = "glLightModelxv";
    apiNameTable[GL1_API_glLightx]                        = "glLightx";
    apiNameTable[GL1_API_glLightxv]                       = "glLightxv";
    apiNameTable[GL1_API_glLineWidthx]                    = "glLineWidthx";
    apiNameTable[GL1_API_glLoadIdentity]                  = "glLoadIdentity";
    apiNameTable[GL1_API_glLoadMatrixx]                   = "glLoadMatrixx";
    apiNameTable[GL1_API_glLogicOp]                       = "glLogicOp";
    apiNameTable[GL1_API_glMaterialx]                     = "glMaterialx";
    apiNameTable[GL1_API_glMaterialxv]                    = "glMaterialxv";
    apiNameTable[GL1_API_glMatrixMode]                    = "glMatrixMode";
    apiNameTable[GL1_API_glMultMatrixx]                   = "glMultMatrixx";
    apiNameTable[GL1_API_glMultiTexCoord4x]               = "glMultiTexCoord4x";
    apiNameTable[GL1_API_glNormal3x]                      = "glNormal3x";
    apiNameTable[GL1_API_glNormalPointer]                 = "glNormalPointer";
    apiNameTable[GL1_API_glOrthox]                        = "glOrthox";
    apiNameTable[GL1_API_glPixelStorei]                   = "glPixelStorei";
    apiNameTable[GL1_API_glPointParameterx]               = "glPointParameterx";
    apiNameTable[GL1_API_glPointParameterxv]              = "glPointParameterxv";
    apiNameTable[GL1_API_glPointSizex]                    = "glPointSizex";
    apiNameTable[GL1_API_glPolygonOffsetx]                = "glPolygonOffsetx";
    apiNameTable[GL1_API_glPopMatrix]                     = "glPopMatrix";
    apiNameTable[GL1_API_glPushMatrix]                    = "glPushMatrix";
    apiNameTable[GL1_API_glReadPixels]                    = "glReadPixels";
    apiNameTable[GL1_API_glRotatex]                       = "glRotatex";
    apiNameTable[GL1_API_glSampleCoverage]                = "glSampleCoverage";
    apiNameTable[GL1_API_glSampleCoveragex]               = "glSampleCoveragex";
    apiNameTable[GL1_API_glScalex]                        = "glScalex";
    apiNameTable[GL1_API_glScissor]                       = "glScissor";
    apiNameTable[GL1_API_glShadeModel]                    = "glShadeModel";
    apiNameTable[GL1_API_glStencilFunc]                   = "glStencilFunc";
    apiNameTable[GL1_API_glStencilMask]                   = "glStencilMask";
    apiNameTable[GL1_API_glStencilOp]                     = "glStencilOp";
    apiNameTable[GL1_API_glTexCoordPointer]               = "glTexCoordPointer";
    apiNameTable[GL1_API_glTexEnvi]                       = "glTexEnvi";
    apiNameTable[GL1_API_glTexEnvx]                       = "glTexEnvx";
    apiNameTable[GL1_API_glTexEnviv]                      = "glTexEnviv";
    apiNameTable[GL1_API_glTexEnvxv]                      = "glTexEnvxv";
    apiNameTable[GL1_API_glTexImage2D]                    = "glTexImage2D";
    apiNameTable[GL1_API_glTexParameteri]                 = "glTexParameteri";
    apiNameTable[GL1_API_glTexParameterx]                 = "glTexParameterx";
    apiNameTable[GL1_API_glTexParameteriv]                = "glTexParameteriv";
    apiNameTable[GL1_API_glTexParameterxv]                = "glTexParameterxv";
    apiNameTable[GL1_API_glTexSubImage2D]                 = "glTexSubImage2D";
    apiNameTable[GL1_API_glTranslatex]                    = "glTranslatex";
    apiNameTable[GL1_API_glVertexPointer]                 = "glVertexPointer";
    apiNameTable[GL1_API_glViewport]                      = "glViewport";
    apiNameTable[GL1_API_glEGLImageTargetTexture2DOES]    = "glEGLImageTargetTexture2DOES";
    apiNameTable[GL1_API_glMapBufferOES]                  = "glMapBufferOES";
    apiNameTable[GL1_API_glUnmapBufferOES]                = "glUnmapBufferOES";
    apiNameTable[GL1_API_glPointSizePointerOES]           = "glPointSizePointerOES";
    apiNameTable[GL1_API_glCurrentPaletteMatrixOES]       = "glCurrentPaletteMatrixOES";
    apiNameTable[GL1_API_glLoadPaletteFromModelViewMatrixOES] = "glLoadPaletteFromModelViewMatrixOES";
    apiNameTable[GL1_API_glMatrixIndexPointerOES]         = "glMatrixIndexPointerOES";
    apiNameTable[GL1_API_glWeightPointerOES]              = "glWeightPointerOES";
    apiNameTable[GL1_API_glBlendEquationSeparateOES]      = "glBlendEquationSeparateOES";
    apiNameTable[GL1_API_glBlendFuncSeparateOES]          = "glBlendFuncSeparateOES";
    apiNameTable[GL1_API_glBlendEquationOES]              = "glBlendEquationOES";
    apiNameTable[GL1_API_glGetBufferPointervOES]          = "glGetBufferPointervOES";
    apiNameTable[GL1_API_glDrawTexsOES]                   = "glDrawTexsOES";
    apiNameTable[GL1_API_glDrawTexiOES]                   = "glDrawTexiOES";
    apiNameTable[GL1_API_glDrawTexxOES]                   = "glDrawTexxOES";
    apiNameTable[GL1_API_glDrawTexsvOES]                  = "glDrawTexsvOES";
    apiNameTable[GL1_API_glDrawTexivOES]                  = "glDrawTexivOES";
    apiNameTable[GL1_API_glDrawTexxvOES]                  = "glDrawTexxvOES";
    apiNameTable[GL1_API_glDrawTexfOES]                   = "glDrawTexfOES";
    apiNameTable[GL1_API_glDrawTexfvOES]                  = "glDrawTexfvOES";
    apiNameTable[GL1_API_glIsRenderbufferOES]             = "glIsRenderbufferOES";
    apiNameTable[GL1_API_glBindRenderbufferOES]           = "glBindRenderbufferOES";
    apiNameTable[GL1_API_glDeleteRenderbuffersOES]        = "glDeleteRenderbuffersOES";
    apiNameTable[GL1_API_glGenRenderbuffersOES]           = "glGenRenderbuffersOES";
    apiNameTable[GL1_API_glRenderbufferStorageOES]        = "glRenderbufferStorageOES";
    apiNameTable[GL1_API_glGetRenderbufferParameterivOES] = "glGetRenderbufferParameterivOES";
    apiNameTable[GL1_API_glIsFramebufferOES]              = "glIsFramebufferOES";
    apiNameTable[GL1_API_glBindFramebufferOES]            = "glBindFramebufferOES";
    apiNameTable[GL1_API_glDeleteFramebuffersOES]         = "glDeleteFramebuffersOES";
    apiNameTable[GL1_API_glGenFramebuffersOES]            = "glGenFramebuffersOES";
    apiNameTable[GL1_API_glCheckFramebufferStatusOES]     = "glCheckFramebufferStatusOES";
    apiNameTable[GL1_API_glFramebufferRenderbufferOES]    = "glFramebufferRenderbufferOES";
    apiNameTable[GL1_API_glFramebufferTexture2DOES]       = "glFramebufferTexture2DOES";
    apiNameTable[GL1_API_glGetFramebufferAttachmentParameterivOES] = "glGetFramebufferAttachmentParameterivOES";
    apiNameTable[GL1_API_glGenerateMipmapOES]             = "glGenerateMipmapOES";
    apiNameTable[GL1_API_glEGLImageTargetRenderbufferStorageOES] = "glEGLImageTargetRenderbufferStorageOES";
    apiNameTable[GL1_API_glMultiDrawArraysEXT]            = "glMultiDrawArraysEXT";
    apiNameTable[GL1_API_glMultiDrawElementsEXT]          = "glMultiDrawElementsEXT";

    apiNameTable[GL3_API_glActiveTexture]                 = "glActiveTexture";
    apiNameTable[GL3_API_glAttachShader]                  = "glAttachShader";
    apiNameTable[GL3_API_glBindAttribLocation]            = "glBindAttribLocation";
    apiNameTable[GL3_API_glBindBuffer]                    = "glBindBuffer";
    apiNameTable[GL3_API_glBindFramebuffer]               = "glBindFramebuffer";
    apiNameTable[GL3_API_glBindRenderbuffer]              = "glBindRenderbuffer";
    apiNameTable[GL3_API_glBindTexture]                   = "glBindTexture";
    apiNameTable[GL3_API_glBlendColor]                    = "glBlendColor";
    apiNameTable[GL3_API_glBlendEquation]                 = "glBlendEquation";
    apiNameTable[GL3_API_glBlendEquationSeparate]         = "glBlendEquationSeparate";
    apiNameTable[GL3_API_glBlendFunc]                     = "glBlendFunc";
    apiNameTable[GL3_API_glBlendFuncSeparate]             = "glBlendFuncSeparate";
    apiNameTable[GL3_API_glBufferData]                    = "glBufferData";
    apiNameTable[GL3_API_glBufferSubData]                 = "glBufferSubData";
    apiNameTable[GL3_API_glCheckFramebufferStatus]        = "glCheckFramebufferStatus";
    apiNameTable[GL3_API_glClear]                         = "glClear";
    apiNameTable[GL3_API_glClearColor]                    = "glClearColor";
    apiNameTable[GL3_API_glClearDepthf]                   = "glClearDepthf";
    apiNameTable[GL3_API_glClearStencil]                  = "glClearStencil";
    apiNameTable[GL3_API_glColorMask]                     = "glColorMask";
    apiNameTable[GL3_API_glCompileShader]                 = "glCompileShader";
    apiNameTable[GL3_API_glCompressedTexImage2D]          = "glCompressedTexImage2D";
    apiNameTable[GL3_API_glCompressedTexSubImage2D]       = "glCompressedTexSubImage2D";
    apiNameTable[GL3_API_glCopyTexImage2D]                = "glCopyTexImage2D";
    apiNameTable[GL3_API_glCopyTexSubImage2D]             = "glCopyTexSubImage2D";
    apiNameTable[GL3_API_glCreateProgram]                 = "glCreateProgram";
    apiNameTable[GL3_API_glCreateShader]                  = "glCreateShader";
    apiNameTable[GL3_API_glCullFace]                      = "glCullFace";
    apiNameTable[GL3_API_glDeleteBuffers]                 = "glDeleteBuffers";
    apiNameTable[GL3_API_glDeleteFramebuffers]            = "glDeleteFramebuffers";
    apiNameTable[GL3_API_glDeleteTextures]                = "glDeleteTextures";
    apiNameTable[GL3_API_glDeleteProgram]                 = "glDeleteProgram";
    apiNameTable[GL3_API_glDeleteRenderbuffers]           = "glDeleteRenderbuffers";
    apiNameTable[GL3_API_glDeleteShader]                  = "glDeleteShader";
    apiNameTable[GL3_API_glDetachShader]                  = "glDetachShader";
    apiNameTable[GL3_API_glDepthFunc]                     = "glDepthFunc";
    apiNameTable[GL3_API_glDepthMask]                     = "glDepthMask";
    apiNameTable[GL3_API_glDepthRangef]                   = "glDepthRangef";
    apiNameTable[GL3_API_glDisable]                       = "glDisable";
    apiNameTable[GL3_API_glDisableVertexAttribArray]      = "glDisableVertexAttribArray";
    apiNameTable[GL3_API_glDrawArrays]                    = "glDrawArrays";
    apiNameTable[GL3_API_glDrawElements]                  = "glDrawElements";
    apiNameTable[GL3_API_glEnable]                        = "glEnable";
    apiNameTable[GL3_API_glEnableVertexAttribArray]       = "glEnableVertexAttribArray";
    apiNameTable[GL3_API_glFinish]                        = "glFinish";
    apiNameTable[GL3_API_glFlush]                         = "glFlush";
    apiNameTable[GL3_API_glFramebufferRenderbuffer]       = "glFramebufferRenderbuffer";
    apiNameTable[GL3_API_glFramebufferTexture2D]          = "glFramebufferTexture2D";
    apiNameTable[GL3_API_glFrontFace]                     = "glFrontFace";
    apiNameTable[GL3_API_glGenBuffers]                    = "glGenBuffers";
    apiNameTable[GL3_API_glGenerateMipmap]                = "glGenerateMipmap";
    apiNameTable[GL3_API_glGenFramebuffers]               = "glGenFramebuffers";
    apiNameTable[GL3_API_glGenRenderbuffers]              = "glGenRenderbuffers";
    apiNameTable[GL3_API_glGenTextures]                   = "glGenTextures";
    apiNameTable[GL3_API_glGetActiveAttrib]               = "glGetActiveAttrib";
    apiNameTable[GL3_API_glGetActiveUniform]              = "glGetActiveUniform";
    apiNameTable[GL3_API_glGetAttachedShaders]            = "glGetAttachedShaders";
    apiNameTable[GL3_API_glGetAttribLocation]             = "glGetAttribLocation";
    apiNameTable[GL3_API_glGetBooleanv]                   = "glGetBooleanv";
    apiNameTable[GL3_API_glGetBufferParameteriv]          = "glGetBufferParameteriv";
    apiNameTable[GL3_API_glGetError]                      = "glGetError";
    apiNameTable[GL3_API_glGetFloatv]                     = "glGetFloatv";
    apiNameTable[GL3_API_glGetFramebufferAttachmentParameteriv] = "glGetFramebufferAttachmentParameteriv";
    apiNameTable[GL3_API_glGetIntegerv]                   = "glGetIntegerv";
    apiNameTable[GL3_API_glGetProgramiv]                  = "glGetProgramiv";
    apiNameTable[GL3_API_glGetProgramInfoLog]             = "glGetProgramInfoLog";
    apiNameTable[GL3_API_glGetRenderbufferParameteriv]    = "glGetRenderbufferParameteriv";
    apiNameTable[GL3_API_glGetShaderiv]                   = "glGetShaderiv";
    apiNameTable[GL3_API_glGetShaderInfoLog]              = "glGetShaderInfoLog";
    apiNameTable[GL3_API_glGetShaderPrecisionFormat]      = "glGetShaderPrecisionFormat";
    apiNameTable[GL3_API_glGetShaderSource]               = "glGetShaderSource";
    apiNameTable[GL3_API_glGetString]                     = "glGetString";
    apiNameTable[GL3_API_glGetTexParameterfv]             = "glGetTexParameterfv";
    apiNameTable[GL3_API_glGetTexParameteriv]             = "glGetTexParameteriv";
    apiNameTable[GL3_API_glGetUniformfv]                  = "glGetUniformfv";
    apiNameTable[GL3_API_glGetUniformiv]                  = "glGetUniformiv";
    apiNameTable[GL3_API_glGetUniformLocation]            = "glGetUniformLocation";
    apiNameTable[GL3_API_glGetVertexAttribfv]             = "glGetVertexAttribfv";
    apiNameTable[GL3_API_glGetVertexAttribiv]             = "glGetVertexAttribiv";
    apiNameTable[GL3_API_glGetVertexAttribPointerv]       = "glGetVertexAttribPointerv";
    apiNameTable[GL3_API_glHint]                          = "glHint";
    apiNameTable[GL3_API_glIsBuffer]                      = "glIsBuffer";
    apiNameTable[GL3_API_glIsEnabled]                     = "glIsEnabled";
    apiNameTable[GL3_API_glIsFramebuffer]                 = "glIsFramebuffer";
    apiNameTable[GL3_API_glIsProgram]                     = "glIsProgram";
    apiNameTable[GL3_API_glIsRenderbuffer]                = "glIsRenderbuffer";
    apiNameTable[GL3_API_glIsShader]                      = "glIsShader";
    apiNameTable[GL3_API_glIsTexture]                     = "glIsTexture";
    apiNameTable[GL3_API_glLineWidth]                     = "glLineWidth";
    apiNameTable[GL3_API_glLinkProgram]                   = "glLinkProgram";
    apiNameTable[GL3_API_glPixelStorei]                   = "glPixelStorei";
    apiNameTable[GL3_API_glPolygonOffset]                 = "glPolygonOffset";
    apiNameTable[GL3_API_glReadPixels]                    = "glReadPixels";
    apiNameTable[GL3_API_glReleaseShaderCompiler]         = "glReleaseShaderCompiler";
    apiNameTable[GL3_API_glRenderbufferStorage]           = "glRenderbufferStorage";
    apiNameTable[GL3_API_glSampleCoverage]                = "glSampleCoverage";
    apiNameTable[GL3_API_glScissor]                       = "glScissor";
    apiNameTable[GL3_API_glShaderBinary]                  = "glShaderBinary";
    apiNameTable[GL3_API_glShaderSource]                  = "glShaderSource";
    apiNameTable[GL3_API_glStencilFunc]                   = "glStencilFunc";
    apiNameTable[GL3_API_glStencilFuncSeparate]           = "glStencilFuncSeparate";
    apiNameTable[GL3_API_glStencilMask]                   = "glStencilMask";
    apiNameTable[GL3_API_glStencilMaskSeparate]           = "glStencilMaskSeparate";
    apiNameTable[GL3_API_glStencilOp]                     = "glStencilOp";
    apiNameTable[GL3_API_glStencilOpSeparate]             = "glStencilOpSeparate";
    apiNameTable[GL3_API_glTexImage2D]                    = "glTexImage2D";
    apiNameTable[GL3_API_glTexParameterf]                 = "glTexParameterf";
    apiNameTable[GL3_API_glTexParameterfv]                = "glTexParameterfv";
    apiNameTable[GL3_API_glTexParameteri]                 = "glTexParameteri";
    apiNameTable[GL3_API_glTexParameteriv]                = "glTexParameteriv";
    apiNameTable[GL3_API_glTexSubImage2D]                 = "glTexSubImage2D";
    apiNameTable[GL3_API_glUniform1f]                     = "glUniform1f";
    apiNameTable[GL3_API_glUniform1fv]                    = "glUniform1fv";
    apiNameTable[GL3_API_glUniform1i]                     = "glUniform1i";
    apiNameTable[GL3_API_glUniform1iv]                    = "glUniform1iv";
    apiNameTable[GL3_API_glUniform2f]                     = "glUniform2f";
    apiNameTable[GL3_API_glUniform2fv]                    = "glUniform2fv";
    apiNameTable[GL3_API_glUniform2i]                     = "glUniform2i";
    apiNameTable[GL3_API_glUniform2iv]                    = "glUniform2iv";
    apiNameTable[GL3_API_glUniform3f]                     = "glUniform3f";
    apiNameTable[GL3_API_glUniform3fv]                    = "glUniform3fv";
    apiNameTable[GL3_API_glUniform3i]                     = "glUniform3i";
    apiNameTable[GL3_API_glUniform3iv]                    = "glUniform3iv";
    apiNameTable[GL3_API_glUniform4f]                     = "glUniform4f";
    apiNameTable[GL3_API_glUniform4fv]                    = "glUniform4fv";
    apiNameTable[GL3_API_glUniform4i]                     = "glUniform4i";
    apiNameTable[GL3_API_glUniform4iv]                    = "glUniform4iv";
    apiNameTable[GL3_API_glUniformMatrix2fv]              = "glUniformMatrix2fv";
    apiNameTable[GL3_API_glUniformMatrix3fv]              = "glUniformMatrix3fv";
    apiNameTable[GL3_API_glUniformMatrix4fv]              = "glUniformMatrix4fv";
    apiNameTable[GL3_API_glUseProgram]                    = "glUseProgram";
    apiNameTable[GL3_API_glValidateProgram]               = "glValidateProgram";
    apiNameTable[GL3_API_glVertexAttrib1f]                = "glVertexAttrib1f";
    apiNameTable[GL3_API_glVertexAttrib1fv]               = "glVertexAttrib1fv";
    apiNameTable[GL3_API_glVertexAttrib2f]                = "glVertexAttrib2f";
    apiNameTable[GL3_API_glVertexAttrib2fv]               = "glVertexAttrib2fv";
    apiNameTable[GL3_API_glVertexAttrib3f]                = "glVertexAttrib3f";
    apiNameTable[GL3_API_glVertexAttrib3fv]               = "glVertexAttrib3fv";
    apiNameTable[GL3_API_glVertexAttrib4f]                = "glVertexAttrib4f";
    apiNameTable[GL3_API_glVertexAttrib4fv]               = "glVertexAttrib4fv";
    apiNameTable[GL3_API_glVertexAttribPointer]           = "glVertexAttribPointer";
    apiNameTable[GL3_API_glViewport]                      = "glViewport";
    apiNameTable[GL3_API_glReadBuffer]                    = "glReadBuffer";
    apiNameTable[GL3_API_glDrawRangeElements]             = "glDrawRangeElements";
    apiNameTable[GL3_API_glTexImage3D]                    = "glTexImage3D";
    apiNameTable[GL3_API_glTexSubImage3D]                 = "glTexSubImage3D";
    apiNameTable[GL3_API_glCopyTexSubImage3D]             = "glCopyTexSubImage3D";
    apiNameTable[GL3_API_glCompressedTexImage3D]          = "glCompressedTexImage3D";
    apiNameTable[GL3_API_glCompressedTexSubImage3D]       = "glCompressedTexSubImage3D";
    apiNameTable[GL3_API_glGenQueries]                    = "glGenQueries";
    apiNameTable[GL3_API_glDeleteQueries]                 = "glDeleteQueries";
    apiNameTable[GL3_API_glIsQuery]                       = "glIsQuery";
    apiNameTable[GL3_API_glBeginQuery]                    = "glBeginQuery";
    apiNameTable[GL3_API_glEndQuery]                      = "glEndQuery";
    apiNameTable[GL3_API_glGetQueryiv]                    = "glGetQueryiv";
    apiNameTable[GL3_API_glGetQueryObjectuiv]             = "glGetQueryObjectuiv";
    apiNameTable[GL3_API_glUnmapBuffer]                   = "glUnmapBuffer";
    apiNameTable[GL3_API_glGetBufferPointerv]             = "glGetBufferPointerv";
    apiNameTable[GL3_API_glDrawBuffers]                   = "glDrawBuffers";
    apiNameTable[GL3_API_glUniformMatrix2x3fv]            = "glUniformMatrix2x3fv";
    apiNameTable[GL3_API_glUniformMatrix3x2fv]            = "glUniformMatrix3x2fv";
    apiNameTable[GL3_API_glUniformMatrix2x4fv]            = "glUniformMatrix2x4fv";
    apiNameTable[GL3_API_glUniformMatrix4x2fv]            = "glUniformMatrix4x2fv";
    apiNameTable[GL3_API_glUniformMatrix3x4fv]            = "glUniformMatrix3x4fv";
    apiNameTable[GL3_API_glUniformMatrix4x3fv]            = "glUniformMatrix4x3fv";
    apiNameTable[GL3_API_glBlitFramebuffer]               = "glBlitFramebuffer";
    apiNameTable[GL3_API_glRenderbufferStorageMultisample] = "glRenderbufferStorageMultisample";
    apiNameTable[GL3_API_glFramebufferTextureLayer]       = "glFramebufferTextureLayer";
    apiNameTable[GL3_API_glMapBufferRange]                = "glMapBufferRange";
    apiNameTable[GL3_API_glFlushMappedBufferRange]        = "glFlushMappedBufferRange";
    apiNameTable[GL3_API_glBindVertexArray]               = "glBindVertexArray";
    apiNameTable[GL3_API_glDeleteVertexArrays]            = "glDeleteVertexArrays";
    apiNameTable[GL3_API_glGenVertexArrays]               = "glGenVertexArrays";
    apiNameTable[GL3_API_glIsVertexArray]                 = "glIsVertexArray";
    apiNameTable[GL3_API_glGetIntegeri_v]                 = "glGetIntegeri_v";
    apiNameTable[GL3_API_glBeginTransformFeedback]        = "glBeginTransformFeedback";
    apiNameTable[GL3_API_glEndTransformFeedback]          = "glEndTransformFeedback";
    apiNameTable[GL3_API_glBindBufferRange]               = "glBindBufferRange";
    apiNameTable[GL3_API_glBindBufferBase]                = "glBindBufferBase";
    apiNameTable[GL3_API_glTransformFeedbackVaryings]     = "glTransformFeedbackVaryings";
    apiNameTable[GL3_API_glGetTransformFeedbackVarying]   = "glGetTransformFeedbackVarying";
    apiNameTable[GL3_API_glVertexAttribIPointer]          = "glVertexAttribIPointer";
    apiNameTable[GL3_API_glGetVertexAttribIiv]            = "glGetVertexAttribIiv";
    apiNameTable[GL3_API_glGetVertexAttribIuiv]           = "glGetVertexAttribIuiv";
    apiNameTable[GL3_API_glVertexAttribI4i]               = "glVertexAttribI4i";
    apiNameTable[GL3_API_glVertexAttribI4ui]              = "glVertexAttribI4ui";
    apiNameTable[GL3_API_glVertexAttribI4iv]              = "glVertexAttribI4iv";
    apiNameTable[GL3_API_glVertexAttribI4uiv]             = "glVertexAttribI4uiv";
    apiNameTable[GL3_API_glGetUniformuiv]                 = "glGetUniformuiv";
    apiNameTable[GL3_API_glGetFragDataLocation]           = "glGetFragDataLocation";
    apiNameTable[GL3_API_glUniform1ui]                    = "glUniform1ui";
    apiNameTable[GL3_API_glUniform2ui]                    = "glUniform2ui";
    apiNameTable[GL3_API_glUniform3ui]                    = "glUniform3ui";
    apiNameTable[GL3_API_glUniform4ui]                    = "glUniform4ui";
    apiNameTable[GL3_API_glUniform1uiv]                   = "glUniform1uiv";
    apiNameTable[GL3_API_glUniform2uiv]                   = "glUniform2uiv";
    apiNameTable[GL3_API_glUniform3uiv]                   = "glUniform3uiv";
    apiNameTable[GL3_API_glUniform4uiv]                   = "glUniform4uiv";
    apiNameTable[GL3_API_glClearBufferiv]                 = "glClearBufferiv";
    apiNameTable[GL3_API_glClearBufferuiv]                = "glClearBufferuiv";
    apiNameTable[GL3_API_glClearBufferfv]                 = "glClearBufferfv";
    apiNameTable[GL3_API_glClearBufferfi]                 = "glClearBufferfi";
    apiNameTable[GL3_API_glGetStringi]                    = "glGetStringi";
    apiNameTable[GL3_API_glCopyBufferSubData]             = "glCopyBufferSubData";
    apiNameTable[GL3_API_glGetUniformIndices]             = "glGetUniformIndices";
    apiNameTable[GL3_API_glGetActiveUniformsiv]           = "glGetActiveUniformsiv";
    apiNameTable[GL3_API_glGetUniformBlockIndex]          = "glGetUniformBlockIndex";
    apiNameTable[GL3_API_glGetActiveUniformBlockiv]       = "glGetActiveUniformBlockiv";
    apiNameTable[GL3_API_glGetActiveUniformBlockName]     = "glGetActiveUniformBlockName";
    apiNameTable[GL3_API_glUniformBlockBinding]           = "glUniformBlockBinding";
    apiNameTable[GL3_API_glDrawArraysInstanced]           = "glDrawArraysInstanced";
    apiNameTable[GL3_API_glDrawElementsInstanced]         = "glDrawElementsInstanced";
    apiNameTable[GL3_API_glFenceSync]                     = "glFenceSync";
    apiNameTable[GL3_API_glIsSync]                        = "glIsSync";
    apiNameTable[GL3_API_glDeleteSync]                    = "glDeleteSync";
    apiNameTable[GL3_API_glClientWaitSync]                = "glClientWaitSync";
    apiNameTable[GL3_API_glWaitSync]                      = "glWaitSync";
    apiNameTable[GL3_API_glGetInteger64v]                 = "glGetInteger64v";
    apiNameTable[GL3_API_glGetSynciv]                     = "glGetSynciv";
    apiNameTable[GL3_API_glGetInteger64i_v]               = "glGetInteger64i_v";
    apiNameTable[GL3_API_glGetBufferParameteri64v]        = "glGetBufferParameteri64v";
    apiNameTable[GL3_API_glGenSamplers]                   = "glGenSamplers";
    apiNameTable[GL3_API_glDeleteSamplers]                = "glDeleteSamplers";
    apiNameTable[GL3_API_glIsSampler]                     = "glIsSampler";
    apiNameTable[GL3_API_glBindSampler]                   = "glBindSampler";
    apiNameTable[GL3_API_glSamplerParameteri]             = "glSamplerParameteri";
    apiNameTable[GL3_API_glSamplerParameteriv]            = "glSamplerParameteriv";
    apiNameTable[GL3_API_glSamplerParameterf]             = "glSamplerParameterf";
    apiNameTable[GL3_API_glSamplerParameterfv]            = "glSamplerParameterfv";
    apiNameTable[GL3_API_glGetSamplerParameteriv]         = "glGetSamplerParameteriv";
    apiNameTable[GL3_API_glGetSamplerParameterfv]         = "glGetSamplerParameterfv";
    apiNameTable[GL3_API_glVertexAttribDivisor]           = "glVertexAttribDivisor";
    apiNameTable[GL3_API_glBindTransformFeedback]         = "glBindTransformFeedback";
    apiNameTable[GL3_API_glDeleteTransformFeedbacks]      = "glDeleteTransformFeedbacks";
    apiNameTable[GL3_API_glGenTransformFeedbacks]         = "glGenTransformFeedbacks";
    apiNameTable[GL3_API_glIsTransformFeedback]           = "glIsTransformFeedback";
    apiNameTable[GL3_API_glPauseTransformFeedback]        = "glPauseTransformFeedback";
    apiNameTable[GL3_API_glResumeTransformFeedback]       = "glResumeTransformFeedback";
    apiNameTable[GL3_API_glGetProgramBinary]              = "glGetProgramBinary";
    apiNameTable[GL3_API_glProgramBinary]                 = "glProgramBinary";
    apiNameTable[GL3_API_glProgramParameteri]             = "glProgramParameteri";
    apiNameTable[GL3_API_glInvalidateFramebuffer]         = "glInvalidateFramebuffer";
    apiNameTable[GL3_API_glInvalidateSubFramebuffer]      = "glInvalidateSubFramebuffer";
    apiNameTable[GL3_API_glTexStorage2D]                  = "glTexStorage2D";
    apiNameTable[GL3_API_glTexStorage3D]                  = "glTexStorage3D";
    apiNameTable[GL3_API_glGetInternalformativ]           = "glGetInternalformativ";
    apiNameTable[GL3_API_glEGLImageTargetTexture2DOES]    =                  "glEGLImageTargetTexture2DOES";
    apiNameTable[GL3_API_glEGLImageTargetRenderbufferStorageOES] = "glEGLImageTargetRenderbufferStorageOES";
    apiNameTable[GL3_API_glMultiDrawArraysEXT]            = "glMultiDrawArraysEXT";
    apiNameTable[GL3_API_glMultiDrawElementsEXT]          = "glMultiDrawElementsEXT";
    apiNameTable[GL3_API_glMapBufferOES]                  = "glMapBufferOES";
    apiNameTable[GL3_API_glUnmapBufferOES]                = "glUnmapBufferOES";
    apiNameTable[GL3_API_glDiscardFramebufferEXT]         = "glDiscardFramebufferEXT";
    apiNameTable[GL3_API_glFramebufferTexture2DMultisampleEXT] = "glFramebufferTexture2DMultisampleEXT";
    apiNameTable[GL3_API_glTexDirectVIV]                  = "glTexDirectVIV";
    apiNameTable[GL3_API_glTexDirectInvalidateVIV]        = "glTexDirectInvalidateVIV";
    apiNameTable[GL3_API_glTexDirectVIVMap]               = "glTexDirectVIVMap";
    apiNameTable[GL3_API_glTexDirectTiledMapVIV]          = "glTexDirectTiledMapVIV";
    apiNameTable[GL3_API_glGetBufferPointervOES]          = "glGetBufferPointervOES";
    apiNameTable[GL3_API_glGetProgramBinaryOES]           = "glGetProgramBinaryOES";
    apiNameTable[GL3_API_glProgramBinaryOES]              = "glProgramBinaryOES";
    apiNameTable[GL3_API_glGenQueriesEXT]                 = "glGenQueriesEXT";
    apiNameTable[GL3_API_glDeleteQueriesEXT]              = "glDeleteQueriesEXT";
    apiNameTable[GL3_API_glIsQueryEXT]                    = "glIsQueryEXT";
    apiNameTable[GL3_API_glBeginQueryEXT]                 = "glBeginQueryEXT";
    apiNameTable[GL3_API_glEndQueryEXT]                   = "glEndQueryEXT";
    apiNameTable[GL3_API_glGetQueryivEXT]                 = "glGetQueryivEXT";
    apiNameTable[GL3_API_glGetQueryObjectuivEXT]          = "glGetQueryObjectuivEXT";
    apiNameTable[GL3_API_glRenderbufferStorageMultisampleEXT] = "glRenderbufferStorageMultisampleEXT";
    apiNameTable[GL3_API_glGetGraphicsResetStatusEXT]     = "glGetGraphicsResetStatusEXT";
    apiNameTable[GL3_API_glReadnPixelsEXT]                = "glReadnPixelsEXT";
    apiNameTable[GL3_API_glGetnUniformfvEXT]              = "glGetnUniformfvEXT";
    apiNameTable[GL3_API_glGetnUniformivEXT]              = "glGetnUniformivEXT";
    apiNameTable[GL3_API_glDispatchCompute]               = "glDispatchCompute";
    apiNameTable[GL3_API_glDispatchComputeIndirect]       = "glDispatchComputeIndirect";
    apiNameTable[GL3_API_glDrawArraysIndirect]            = "glDrawArraysIndirect";
    apiNameTable[GL3_API_glDrawElementsIndirect]          = "glDrawElementsIndirect";
    apiNameTable[GL3_API_glFramebufferParameteri]         = "glFramebufferParameteri";
    apiNameTable[GL3_API_glGetFramebufferParameteriv]     = "glGetFramebufferParameteriv";
    apiNameTable[GL3_API_glGetProgramInterfaceiv]         = "glGetProgramInterfaceiv";
    apiNameTable[GL3_API_glGetProgramResourceIndex]       = "glGetProgramResourceIndex";
    apiNameTable[GL3_API_glGetProgramResourceName]        = "glGetProgramResourceName";
    apiNameTable[GL3_API_glGetProgramResourceiv]          = "glGetProgramResourceiv";
    apiNameTable[GL3_API_glGetProgramResourceLocation]    = "glGetProgramResourceLocation";
    apiNameTable[GL3_API_glUseProgramStages]              = "glUseProgramStages";
    apiNameTable[GL3_API_glActiveShaderProgram]           = "glActiveShaderProgram";
    apiNameTable[GL3_API_glCreateShaderProgramv]          = "glCreateShaderProgramv";
    apiNameTable[GL3_API_glBindProgramPipeline]           = "glBindProgramPipeline";
    apiNameTable[GL3_API_glDeleteProgramPipelines]        = "glDeleteProgramPipelines";
    apiNameTable[GL3_API_glGenProgramPipelines]           = "glGenProgramPipelines";
    apiNameTable[GL3_API_glIsProgramPipeline]             = "glIsProgramPipeline";
    apiNameTable[GL3_API_glGetProgramPipelineiv]          = "glGetProgramPipelineiv";
    apiNameTable[GL3_API_glProgramUniform1i]              = "glProgramUniform1i";
    apiNameTable[GL3_API_glProgramUniform2i]              = "glProgramUniform2i";
    apiNameTable[GL3_API_glProgramUniform3i]              = "glProgramUniform3i";
    apiNameTable[GL3_API_glProgramUniform4i]              = "glProgramUniform4i";
    apiNameTable[GL3_API_glProgramUniform1ui]             = "glProgramUniform1ui";
    apiNameTable[GL3_API_glProgramUniform2ui]             = "glProgramUniform2ui";
    apiNameTable[GL3_API_glProgramUniform3ui]             = "glProgramUniform3ui";
    apiNameTable[GL3_API_glProgramUniform4ui]             = "glProgramUniform4ui";
    apiNameTable[GL3_API_glProgramUniform1f]              = "glProgramUniform1f";
    apiNameTable[GL3_API_glProgramUniform2f]              = "glProgramUniform2f";
    apiNameTable[GL3_API_glProgramUniform3f]              = "glProgramUniform3f";
    apiNameTable[GL3_API_glProgramUniform4f]              = "glProgramUniform4f";
    apiNameTable[GL3_API_glProgramUniform1iv]             = "glProgramUniform1iv";
    apiNameTable[GL3_API_glProgramUniform2iv]             = "glProgramUniform2iv";
    apiNameTable[GL3_API_glProgramUniform3iv]             = "glProgramUniform3iv";
    apiNameTable[GL3_API_glProgramUniform4iv]             = "glProgramUniform4iv";
    apiNameTable[GL3_API_glProgramUniform1uiv]            = "glProgramUniform1uiv";
    apiNameTable[GL3_API_glProgramUniform2uiv]            = "glProgramUniform2uiv";
    apiNameTable[GL3_API_glProgramUniform3uiv]            = "glProgramUniform3uiv";
    apiNameTable[GL3_API_glProgramUniform4uiv]            = "glProgramUniform4uiv";
    apiNameTable[GL3_API_glProgramUniform1fv]             = "glProgramUniform1fv";
    apiNameTable[GL3_API_glProgramUniform2fv]             = "glProgramUniform2fv";
    apiNameTable[GL3_API_glProgramUniform3fv]             = "glProgramUniform3fv";
    apiNameTable[GL3_API_glProgramUniform4fv]             = "glProgramUniform4fv";
    apiNameTable[GL3_API_glProgramUniformMatrix2fv]       = "glProgramUniformMatrix2fv";
    apiNameTable[GL3_API_glProgramUniformMatrix3fv]       = "glProgramUniformMatrix3fv";
    apiNameTable[GL3_API_glProgramUniformMatrix4fv]       = "glProgramUniformMatrix4fv";
    apiNameTable[GL3_API_glProgramUniformMatrix2x3fv]     = "glProgramUniformMatrix2x3fv";
    apiNameTable[GL3_API_glProgramUniformMatrix3x2fv]     = "glProgramUniformMatrix3x2fv";
    apiNameTable[GL3_API_glProgramUniformMatrix2x4fv]     = "glProgramUniformMatrix2x4fv";
    apiNameTable[GL3_API_glProgramUniformMatrix4x2fv]     = "glProgramUniformMatrix4x2fv";
    apiNameTable[GL3_API_glProgramUniformMatrix3x4fv]     = "glProgramUniformMatrix3x4fv";
    apiNameTable[GL3_API_glProgramUniformMatrix4x3fv]     = "glProgramUniformMatrix4x3fv";
    apiNameTable[GL3_API_glValidateProgramPipeline]       = "glValidateProgramPipeline";
    apiNameTable[GL3_API_glGetProgramPipelineInfoLog]     = "glGetProgramPipelineInfoLog";
    apiNameTable[GL3_API_glBindImageTexture]              = "glBindImageTexture";
    apiNameTable[GL3_API_glGetBooleani_v]                 = "glGetBooleani_v";
    apiNameTable[GL3_API_glMemoryBarrier]                 = "glMemoryBarrier";
    apiNameTable[GL3_API_glMemoryBarrierByRegion]         = "glMemoryBarrierByRegion";
    apiNameTable[GL3_API_glTexStorage2DMultisample]       = "glTexStorage2DMultisample";
    apiNameTable[GL3_API_glGetMultisamplefv]              = "glGetMultisamplefv";
    apiNameTable[GL3_API_glSampleMaski]                   = "glSampleMaski";
    apiNameTable[GL3_API_glGetTexLevelParameteriv]        = "glGetTexLevelParameteriv";
    apiNameTable[GL3_API_glGetTexLevelParameterfv]        = "glGetTexLevelParameterfv";
    apiNameTable[GL3_API_glBindVertexBuffer]              = "glBindVertexBuffer";
    apiNameTable[GL3_API_glVertexAttribFormat]            = "glVertexAttribFormat";
    apiNameTable[GL3_API_glVertexAttribIFormat]           = "glVertexAttribIFormat";
    apiNameTable[GL3_API_glVertexAttribBinding]           = "glVertexAttribBinding";
    apiNameTable[GL3_API_glVertexBindingDivisor]          = "glVertexBindingDivisor";
}

const GLchar* GetAPIName(GLESAPIIndex name)
{
    return apiNameTable[name];
}

GLint GetAPIIndex(GLESAPIIndex name)
{
    if (name >= EGL_API_eglGetError && name <= EGL_API_eglPatchID)
        return GLES_API_EGL;
    else if (name >= GL1_API_glAlphaFunc && name <= GL1_API_glMultiDrawElementsEXT)
        return GLES_API_GLES1;
    else if (name >= GL3_API_glVertexAttribPointer && name <= GL3_API_glVertexBindingDivisor)
        return GLES_API_GLES3;

    return -1;
}