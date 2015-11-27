#ifndef __API_OS_H
#define __API_OS_H

#include <Windows.h>
#include <GLES3/gl31.h>

#if defined(LINUX) || defined(__APPLE__) || defined(__QNXNTO__)
    typedef void* LIB_HANDLE;
#elif _WIN32 || WINDOWS
    typedef HMODULE LIB_HANDLE;
#endif
#ifdef ANDROID
    extern int scr_touch;
#endif

GLuint      apiOS_GetCurrentThreadID(void);
GLvoid      apiOS_Sleep(GLint millisecond);
GLuint      apiOS_GetTicks(void);
GLuint      apiOS_GetMicroSeconds(void);
GLvoid*     apiOS_Malloc(GLuint size);
GLvoid      apiOS_Free(GLvoid *p);
LIB_HANDLE  apiOS_Dlopen(const GLchar* library);
void        apiOS_Dlclose(LIB_HANDLE lib);
void*       apiOS_Dlresolve(LIB_HANDLE lib, const GLchar* symbol);
void        apiOS_SyncEnter(void *mutex);
void        apiOS_SyncExit(void *mutex);
void*       apiOS_CreateSync();
void        apiOS_ReleaseSync(void *mutex);

#ifdef WIN32
BOOL apiOS_DumpFindFirstFileExists(LPCTSTR lpPath, DWORD dwFilter);
BOOL apiOS_DumpFilePathExists(LPCTSTR lpPath);
BOOL apiOS_DumpCreateDirectory(LPCTSTR lpPath);
#endif

#endif /* __API_OS_H */