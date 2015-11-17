/******************************************************************************\
|* Copyright (c) 2014- by Tencent Corporation.  All Rights Reserved.          *|
|*                                                                            *|
|* The material in this file is confidential and contains trade secrets of    *|
|* of Tencent Corporation.  This is proprietary information owned by Tencent  *|
|* Corporation.  No part of this work may be disclosed, reproduced, copied,   *|
|* transmitted, or used in any way for any purpose, without the express       *|
|* written permission of Tencent Corporation.                                 *|
|*                                                                            *|
\******************************************************************************/

#include "APIOS.h"

#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0500
#endif

#include <cstdio>
#include <cassert>
#include <conio.h>

const LIB_HANDLE INVALID_HANDLE = NULL;

GLuint apiOS_GetCurrentThreadID(void)
{
    return (GLuint)GetCurrentThreadId();
}

GLvoid apiOS_Sleep(GLint millisecond)
{
    Sleep(millisecond);
}

GLuint apiOS_GetTicks(void)
{
    /* Return the OS tick count. */
    return GetTickCount();
}

GLuint apiOS_GetMicroSeconds(void)
{
    LARGE_INTEGER   t1;
    LARGE_INTEGER   tc;
    GLuint         time;

    QueryPerformanceFrequency(&tc);
    QueryPerformanceCounter(&t1);

    time = (GLuint)(((double)t1.QuadPart / (double)tc.QuadPart) * 1000000);

    return time;
}

LIB_HANDLE apiOS_Dlopen(const GLchar* library)
{
    return LoadLibrary(library);
}

void apiOS_Dlclose(LIB_HANDLE lib)
{
    assert(lib != INVALID_HANDLE);

    BOOL ret = FreeLibrary(lib);
    assert( ret );
}

void* apiOS_Dlresolve(LIB_HANDLE lib, const GLchar* symbol)
{
    assert( lib != INVALID_HANDLE );

    return GetProcAddress(lib, symbol);
}

unsigned long apiOS_Ticks()
{
    return GetTickCount();
}

GLboolean apiOS_TerminalHasInput()
{
    HANDLE in = GetStdHandle(STD_INPUT_HANDLE);

    if (WaitForSingleObject(in, 0) == WAIT_OBJECT_0)
    {
        INPUT_RECORD    rec = {};
        DWORD           count = 0;

        PeekConsoleInput(in, &rec, 1, &count);
        if (rec.EventType == KEY_EVENT && _kbhit())
            return GL_TRUE;

        FlushConsoleInputBuffer(in);
    }

    return GL_FALSE;
}

GLint apiOS_TerminalReadKey()
{
    int key = _getch();
    if (key == 0x0 || key == 0xE0)
        _getch();
    return key;
}

GLvoid apiOS_TerminalInit()
{
    HANDLE in  = GetStdHandle(STD_INPUT_HANDLE);
    DWORD mode = 0;
    GetConsoleMode(in, &mode);
    //mode &= ~(ENABLE_ECHO_INPUT | ENABLE_LINE_INPUT | ENABLE_PROCESSED_INPUT);
    mode &= ~(ENABLE_ECHO_INPUT | ENABLE_LINE_INPUT);
    SetConsoleMode(in, mode);
}

GLvoid apiOS_TerminalUninit()
{
    HANDLE in  = GetStdHandle(STD_INPUT_HANDLE);
    DWORD mode = 0;
    GetConsoleMode(in, &mode);
    mode |= ENABLE_ECHO_INPUT | ENABLE_LINE_INPUT | ENABLE_PROCESSED_INPUT;
    SetConsoleMode(in, mode);
}

GLvoid apiOS_Sync()
{
    fflush(stdout);
    HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
    FlushFileBuffers(out);
}

GLvoid* apiOS_Malloc(GLuint size)
{
    GLvoid *p = malloc(size);
    return p;
}

GLvoid apiOS_Free(GLvoid *p)
{
    if (p)
    {
        free(p);
    }
}


BOOL apiOS_DumpFindFirstFileExists(LPCTSTR lpPath, DWORD dwFilter)
{
    WIN32_FIND_DATA fd;  
    HANDLE hFind = FindFirstFile(lpPath, &fd);  
    BOOL bFilter = (FALSE == dwFilter) ? TRUE : fd.dwFileAttributes & dwFilter;  
    BOOL RetValue = ((hFind != INVALID_HANDLE_VALUE) && bFilter) ? TRUE : FALSE;  
    FindClose(hFind);  
    return RetValue;  
}

/* 
 * 检查一个  路径 是否存在（绝对路径、相对路径，文件或文件夹均可） 
 * 存在则返回 1 (TRUE) 
 */  
BOOL apiOS_DumpFilePathExists(LPCTSTR lpPath)
{
    return apiOS_DumpFindFirstFileExists(lpPath, FALSE);
}

BOOL apiOS_DumpCreateDirectory(LPCTSTR lpPath)
{
    return CreateDirectory(lpPath, NULL);
}

void apiOS_SyncEnter(void *mutex)
{
    WaitForSingleObject(mutex, INFINITE);
}

void apiOS_SyncExit(void *mutex)
{
    ReleaseMutex(mutex);
}

void* apiOS_CreateSync()
{
    HANDLE mutex = CreateMutex(NULL, FALSE, NULL);
    return mutex;
}

void apiOS_ReleaseSync(void *mutex)
{
    CloseHandle(mutex);
}