#if 0
#include <windows.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <Engine.h>
#include "veEGL.h"

#ifdef __cplusplus
#   define gcvNULL              0
#else
#   define gcvNULL              ((void *) 0)
#endif

#define gcvFALSE                0
#define gcvTRUE                 1

#define gcmLOCKSECTION() \
    static HANDLE __lockHandle__; \
    \
    if (__lockHandle__ == gcvNULL) \
    { \
        __lockHandle__ = CreateMutex(gcvNULL, gcvFALSE, gcvNULL); \
    } \
    \
    WaitForSingleObject(__lockHandle__, INFINITE)

#define gcmUNLOCKSECTION() \
    ReleaseMutex(__lockHandle__)

FILE* g_debugFile;

void* setAPIDumpFile(const char* FileName)
{
    /* Close any existing file handle. */
    if (g_debugFile != gcvNULL)
    {
        fclose(g_debugFile);
        g_debugFile = gcvNULL;
    }

    fopen_s(&g_debugFile, FileName, "w");

    return (void*)g_debugFile;
}

void setAPIDumpFileHandle(void* FileHandle)
{
    /* Close any existing file handle. */
    if (g_debugFile != gcvNULL)
    {
        fclose(g_debugFile);
        g_debugFile = gcvNULL;
    }

    g_debugFile = FileHandle;
}

void* getAPIDumpFile()
{
    return g_debugFile;
}

void printDump (const char* Message) {
#if gcdDUMP_API
    gcmLOCKSECTION();
    if (g_debugFile == gcvNULL) {
        return;
    }

    if (g_debugFile != gcvNULL)
    {
        fprintf(g_debugFile, Message);
        fprintf(g_debugFile, "\n");
    }

    gcmUNLOCKSECTION();
#endif
}

EGLAPI void gcfDumpApi(const char* Message,...) {
#if gcdDUMP_API
    static char buffer[256];
    unsigned int offset = 0;
    va_list args;

    /* initialize input args */
    va_start(args, Message);

    /* Format the string. */
    offset += vsprintf_s(buffer + offset, sizeof (buffer) - offset, Message, args);

    va_end(args);

    printDump(buffer);
#endif
}

EGLAPI void gcfDumpArray(const void* Data, unsigned int Size){
#if gcdDUMP_API
    const unsigned int* data = (const unsigned int*) Data;
    if (Size > 0)
    {
        if (Data == gcvNULL)
        {
            gcfDumpApi("$$ <nil>");
        }
        else
        {
            unsigned int index;

            for (index = 0; index < Size;)
            {
                switch (Size - index)
                {
                case 1:
                    gcfDumpApi("$$ 0x%08X: 0x%08X", data + index, data[index]);
                    index += 1;
                    break;
                case 2:
                    gcfDumpApi("$$ 0x%08X: 0x%08X 0x%08X",
                               data + index, data[index], data[index + 1]);
                    index += 2;
                    break;
                case 3:
                    gcfDumpApi("$$ 0x%08X: 0x%08X 0x%08X 0x%08X",
                               data + index, data[index], data[index + 1],
                               data[index + 2]);
                    index += 3;
                    break;
                default:
                    gcfDumpApi("$$ 0x%08X: 0x%08X 0x%08X 0x%08X 0x%08X",
                               data + index, data[index], data[index + 1],
                               data[index + 2], data[index + 3]);
                    index += 4;
                    break;
                }
            }
        }

        gcfDumpApi("$$ **********");
    }
#endif
}

EGLAPI void gcfDumpArrayToken (const void* Data, unsigned int Termination){
#if gcdDUMP_API
    const unsigned int* data = (unsigned int*) Data;
    if (Data == gcvNULL)
    {
        gcfDumpApi("$$ <nil>");
    }
    else
    {
        unsigned int index;
        for (index = 0; data[index] != Termination; index += 2)
        {
            gcfDumpApi("$$ 0x%08X: 0x%08X 0x%08X", data + index, data[index], data[index + 1]);
        }
        gcfDumpApi("$$ 0x%08X: 0x%08X", data + index, Termination);
    }
    gcfDumpApi("$$ **********");
#endif
}

EGLAPI void gcfDumpApiData(const void* Data, unsigned Size){
#if gcdDUMP_API
    const unsigned char* data = (unsigned char*) Data;

    if (Data == gcvNULL)
    {
        gcfDumpApi("$$ <nil>");
    }
    else
    {
        unsigned long index;


        if (Size == 0)
        {
            Size = (unsigned long) strlen(Data);
            Size += 1;
        }
        for (index = 0; index < Size;)
        {
            switch (Size - index)
            {
            case 1:
                gcfDumpApi("$$ 0x%08X: 0x%02X", data + index, data[index]);
                index += 1;
                break;

            case 2:
                gcfDumpApi("$$ 0x%08X: 0x%02X 0x%02X",
                           data + index, data[index], data[index + 1]);
                index += 2;
                break;

            case 3:
                gcfDumpApi("$$ 0x%08X: 0x%02X 0x%02X 0x%02X",
                           data + index, data[index], data[index + 1],
                           data[index + 2]);
                index += 3;
                break;

            case 4:
                gcfDumpApi("$$ 0x%08X: 0x%02X 0x%02X 0x%02X 0x%02X",
                           data + index, data[index], data[index + 1],
                           data[index + 2], data[index + 3]);
                index += 4;
                break;

            case 5:
                gcfDumpApi("$$ 0x%08X: 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X",
                           data + index, data[index], data[index + 1],
                           data[index + 2], data[index + 3],
                           data[index + 4]);
                index += 5;
                break;

            case 6:
                gcfDumpApi("$$ 0x%08X: 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X "
                           "0x%02X",
                           data + index, data[index], data[index + 1],
                           data[index + 2], data[index + 3],
                           data[index + 4], data[index + 5]);
                index += 6;
                break;

            case 7:
                gcfDumpApi("$$ 0x%08X: 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X "
                           "0x%02X 0x%02X",
                           data + index, data[index], data[index + 1],
                           data[index + 2], data[index + 3],
                           data[index + 4], data[index + 5],
                           data[index + 6]);
                index += 7;
                break;

            default:
                gcfDumpApi("$$ 0x%08X: 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X "
                           "0x%02X 0x%02X 0x%02X",
                           data + index, data[index], data[index + 1],
                           data[index + 2], data[index + 3],
                           data[index + 4], data[index + 5],
                           data[index + 6], data[index + 7]);
                index += 8;
                break;
            }
        }
    }

    gcfDumpApi("$$ **********");
#endif
}
#endif /* 0 */
