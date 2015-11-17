#include <pthread.h>
#include <unistd.h>
#include <memory>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/keysym.h>
#include <dlfcn.h>
#include <fcntl.h>
#include <cstdio>
#include <cassert>
#include <termios.h>

#include <sys/time.h>
#include <sys/select.h>
#include <sys/types.h>
#include <sys/syscall.h>

#include "TCGWindow.h"
#include "TCGPlatform.h"

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
    ms_t ms = tv.tv_sec * 1000;
    ms += tv.tv_usec / 1000;
    return ms;
}

TCGbool tcgOS_TerminalHasInput()
{
    fd_set read;
    FD_ZERO(&read);
    FD_SET(STDIN_FILENO, &read);

    struct timeval tv = {};
    int ret = select(STDIN_FILENO + 1, &read, NULL, NULL, &tv);
    assert( ret != -1 );
    return ret != 0;
}

TCGint tcgOS_TerminalReadKey()
{
    fd_set read;
    FD_ZERO(&read);
    FD_SET(STDIN_FILENO, &read);
    select(STDIN_FILENO + 1, &read, NULL, NULL, NULL);

    return getchar();
}

TCGvoid tcgOS_TerminalInit()
{
    termios term = {0};
    tcgetattr(STDIN_FILENO, &term);
    term.c_lflag &= ~(ECHO | ECHONL | ICANON /* | ISIG */ | IEXTEN);
    tcsetattr(STDIN_FILENO, TCSANOW, &term);

    fcntl(STDIN_FILENO, F_SETFL, O_NONBLOCK);
}

TCGvoid tcgOS_TerminalUninit()
{
    termios term = {0};
    tcgetattr(STDIN_FILENO, &term);
    term.c_lflag |= (ECHO | ECHONL | ICANON /* | ISIG */ | IEXTEN);
    tcsetattr(STDIN_FILENO, TCSANOW, &term);

    fcntl(STDIN_FILENO, F_SETFL, 0);
}

TCGvoid tcgOS_Sync()
{
    fflush(stdout);
    fsync(STDOUT_FILENO);
}