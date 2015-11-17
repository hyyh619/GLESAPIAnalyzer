#include "TCGPlayerUtils.h"
#include <pthread.h>
#include <unistd.h>
#include <memory>
#include <dlfcn.h>
#include <fcntl.h>
#include <cstdio>
#include <cassert>
#include <termios.h>

#include <sys/time.h>
#include <sys/select.h>
#include <sys/types.h>
#include <sys/syscall.h>

#include "TcgWindow.h"
#include "TcgPlatform.h"

using namespace std;

TCGhandle tcgOS_GetCurrentThreadID(void)
{
    return (TCGhandle) pthread_self();
}

TCGvoid tcgOS_Sleep(TCGint millisecond)
{
    usleep(1000*millisecond);
}

TCGuint tcgOS_GetTicks(void)
{
    struct timeval tv;

    /* Return the time of day in milliseconds. */
    gettimeofday(&tv, 0);
    return (tv.tv_sec * 1000) + (tv.tv_usec / 1000);
}

TCGuint tcgOS_GetMicroSeconds(void)
{
    struct  timeval tv;
    TCGuint time;

    /* Return the time of day in microseconds. */
    gettimeofday(&tv, 0);
    time = (tv.tv_sec * 1000 * 1000) + (tv.tv_usec);
    return time;
}

LIB_HANDLE tcgOS_Dlopen(const char* library)
{
    return ::dlopen(library, RTLD_LAZY);
}

TCGvoid tcgOS_Dlclose(LIB_HANDLE lib)
{
    assert( lib != INVALID_HANDLE );

    ::dlclose(lib);
}

TCGvoid* tcgOS_Dlresolve(LIB_HANDLE lib, const char* symbol)
{
    assert(lib != INVALID_HANDLE);

    return ::dlsym(lib, symbol);
}

TCGms_t tcgOS_Ticks()
{
    struct timeval tv = {};
    gettimeofday(&tv, NULL);
    TCGms_t ms = tv.tv_sec * 1000;
    ms += tv.tv_usec / 1000;
    return ms;
}

TCGint scr_touch = 0;
TCGbool tcgOS_TerminalHasInput()
{
    return (scr_touch != 0);
}

TCGint tcgOS_TerminalReadKey()
{
    int rt = 0;
    if(scr_touch == 1)
    {
        rt = KEY_SPACE;
    }
    else if(scr_touch == 2)
    {
        rt = KEY_M;
    }
    scr_touch = 0;
    return rt;
}

TCGvoid tcgOS_TerminalInit()
{

}

TCGvoid tcgOS_TerminalUninit()
{

}

TCGvoid tcgOS_Sync()
{
    fflush(stdout);
    fsync(STDOUT_FILENO);
}