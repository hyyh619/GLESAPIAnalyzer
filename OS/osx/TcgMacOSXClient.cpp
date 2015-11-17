#include "TCGClient.h"
#include "TCGOS.h"
#include "TcgAudioPlay.h"
#ifndef USE_FILE
#include "TCGNet.h"
#endif

#ifdef WIN32
#include "TcpNet.h"
#endif

#ifndef USE_FILE
extern CNet* g_pNet;
#endif

CTCGClient::CTCGClient()
{
}


CTCGClient::~CTCGClient()
{
}

TCGbool CTCGClient::Init()
{
#ifndef USE_FILE

#ifdef WIN32
    g_pNet = TCG_NEW CTcpNet();
    if (g_pNet->Init(10086) < 0)
    {
        return TCG_FALSE;
    }
#endif // WIN32

    if (g_pNet == TCG_NULL)
    {
        TCGLOG(TCG_ERROR, TCG_NET, "Net Init Failed!");
    }

    TCGbool bFlag = g_GetNet()->Init(g_pNet);

    return bFlag;
#else
    return TCG_TRUE;
#endif
}

/*
  changed by SUSON
  主动退出TCGPlayer时，先向TCGTracer发送EndTrace的消息
  确认EndTrace消息已经发送出去，再调用g_GetNet()->UnInit();
*/
TCGbool CTCGClient::UnInit()
{
#ifndef USE_FILE
    stNetEndTraceEvent tEndTraceEvent;
    tEndTraceEvent.byEventType      = eNET_EVENT_CS_FRAME_ENDTRACE;
    tEndTraceEvent.unEventSize      = sizeof(tEndTraceEvent.unEndMsg);
    tEndTraceEvent.unOrgSize        = tEndTraceEvent.unEventSize;
    tEndTraceEvent.unEndMsg         = 1;

    g_GetNet()->SendFrameEvent(&tEndTraceEvent);

    g_GetNet()->UnInit();
    AudioFree();
    g_ReleaseNet();
#endif

    return TCG_TRUE; 
}

#ifdef WIN32
TCGbool CTCGClient::Update(TCGint argc, TCGchar **argv)
{
    return TCG_TRUE;
}
#endif

/******************************************************************************\
|* Copyright (c) 2014- by Tencent Corporation.  All Rights Reserved.      *|
|*                                                                            *|
|* The material in this file is confidential and contains trade secrets of    *|
|* of Tencent Corporation.  This is proprietary information owned by Tencent  *|
|* Corporation.  No part of this work may be disclosed, reproduced, copied,   *|
|* transmitted, or used in any way for any purpose, without the express       *|
|* written permission of Tencent Corporation.                                 *|
|*                                                                            *|
\******************************************************************************/

#include "TCGUtility.h"
#include "TCGPlayerMain.h"
#include "Lock.h"
#include "TCGDebug.h"
#include "TCGPlayerUtils.h"

LIB_HANDLE      g_handle        = NULL;
CPlayer         *g_pPlayer      = NULL;
GLfloat         g_fWRatio       = 1.0f;
GLfloat         g_fHRatio       = 1.0f;
GLboolean       g_bSetRatio     = GL_FALSE;
stCmdArgs        g_args;

#ifdef PERFORMANCE_DEBUG
#include    "TCGDumpInfo.h"
#endif

#if defined(LINUX) || defined(__APPLE__) || defined(__QNXNTO__)
#  ifdef __APPLE__
#       ifdef APPLE_WITH_VIVANTE
#       include "gc_vdk.h"
        vdkEGL g_Egl;
        const char* const LIB_GLES1 = "/Users/yinghuang/development/hy_code/P4/SW/4.6.9/production.dev/build/Debug/libGLESv1_CM.dylib";
        const char* const LIB_GLES2 = "/Users/yinghuang/development/hy_code/P4/SW/4.6.9/production.dev/build/Debug/libGLESv2.dylib";
        const char* const LIB_EGL   = "/Users/yinghuang/development/hy_code/P4/SW/4.6.9/production.dev/build/Debug/libEGL.dylib";
#       else
        const char* const LIB_GLES1 = "libGLESv1_CM.dylib";
        const char* const LIB_GLES2 = "libGLESv2.dylib";
        const char* const LIB_EGL   = "libEGL.dylib";
#  endif
#  endif
#  ifdef LINUX
        const char* const LIB_GLES1 = "libGLESv1_CM.so";
        const char* const LIB_GLES2 = "libGLESv2.so";
        const char* const LIB_EGL   = "libEGL.so";
#  endif
#  ifdef __QNXNTO__
        const char* const LIB_GLES1 = "libGLESv1_CM.so.1";
        const char* const LIB_GLES2 = "libGLESv2.so.1";
        const char* const LIB_EGL   = "libEGL.so.1";
#  endif
#else
#  ifdef WITH_TRACER
        const char* const LIB_GLES1 = "libGLES_TCGTracer.dll";
        const char* const LIB_GLES2 = "libGLES_TCGTracer.dll";
        const char* const LIB_EGL   = "libGLES_TCGTracer.dll";
#  else
        const char* const LIB_GLES1 = "libGLESv1_CM.dll";
        const char* const LIB_GLES2 = "libGLESv2.dll";
        const char* const LIB_EGL   = "libEGL.dll";
#  endif
#endif

#if defined(LINUX)
void sighandler(int signum)
{
    TCGLOG(TCG_INFO, TCG_CLIENT_OS, "Received signal %d\n", signum);
    tcgOS_TerminalUninit();
    tcgOS_Sync();
    exit(-1);
}
#endif

TCGvoid TCGClient::PrintHelp()
{
    const char *help = 
           "Options:\n"
           "  --config N         Use the specified EGL config\n"
           "  --default-config   Force using hard coded default config\n"
           "  --default-context  Use defalut window surface, config and context\n"
           "  --showFPS          Show FPS on log\n"
           "  --showFrameTime    Show Frame execution time cost\n"
           "  --dumpDraw         Dump each draw into a bmp file\n"
           "  --dumpFrame        Dump each frame into a png file\n"
           "  --runFrames N      Run frames\n"
           "  --end N            End frame for rendering\n"
           "  --error            Enable error check after each API call\n"
           "  --finish           Insert glFinish after each OpenGL API call\n"
           "  --fps Interval     Output average FPS and interval FPS information\n"
           "  --help             Print this help\n\n"
           "  --rendertime       Print the render time of each frame\n"
           "  --swapInterval     If enabling swapbuf, it will sleep some time after executing eglSwapBuffers.\n"
           "  --start N          Start frame for rendering\n"
           "  --swapbuf          Insert eglSwapBuffers after each OpenGL API call\n"
           "  --targetkey        Read input key presses from target device\n"
           "  --verbose          Print data parsed from the file\n"
           "  --zoom             Enable zoom to fit the target device screen.\n"
           "  --no-compress      Use uncompressed dump file.\n"
           "  --compress         Use compressed dump file.\n"
           "  --retrace          Running dump file is retraced from another dump file.\n"
           "  --dumpPath         Specify the path that dump pictures are saved in.\n"
           "  --runFile          Specify the running binary files";

#ifdef ANDROID
    const char *usage = "Usage: echo [options] tracefilename > /sdcard/Tencent/TCGPlayer.conf\n\n";
#else
    const char *usage = "Usage: TCGPlayer [options] tracefilename\n\n";
#endif
    
    TCGLOG(TCG_ERROR, TCG_CLIENT_OS, "%s", usage);
    TCGLOG(TCG_ERROR, TCG_CLIENT_OS, "%s", help);
}

TCGbool TCGClient::ParseArgv(TCGint argc, TCGchar* argv[], stCmdArgs& args)
{
    for (TCGint i=1; i<argc; ++i)
    {
        const TCGchar* arg = argv[i];
        if (!strcmp(arg, "--verbose"))
        {
            args.verbose = TCG_TRUE;
        }
        else if (!strcmp(arg, "--no-compress"))
        {
            args.compressedFile = TCG_FALSE;
        }
        else if (!strcmp(arg, "--compress"))
        {
            args.compressedFile = TCG_TRUE;
        }
        else if (!strcmp(arg, "--dumpPath"))
        {
            sprintf(args.strDumpPath, "%s", argv[++i]);
        }
        else if (!strcmp(arg, "--runFile"))
        {
            sprintf(args.strRunFile, "%s", argv[++i]);
        }
        else if (!strcmp(arg, "--retrace"))
        {
            args.retracedFile = TCG_TRUE;
        }
        else if (!strcmp(arg, "--showFPS"))
        {
            args.showFPS = TCG_TRUE;
        }
        else if (!strcmp(arg, "--showFrameTime"))
        {
            args.showFrameTime = TCG_TRUE;
        }
        else if (!strcmp(arg, "--targetkey"))
        {
            args.readterminal = TCG_TRUE;
        }
        else if (!strcmp(arg, "--width"))
        {
            args.width = atoi(argv[++i]);
        }
        else if (!strcmp(arg, "--height"))
        {
            args.height = atoi(argv[++i]);
        }
        else if (!strcmp(arg, "--swapInterval"))
        {
            args.swapInterval = atoi(argv[++i]);
        }
        else if (!strcmp(arg, "--start"))
        {
            args.start = atoi(argv[++i]);
        }
        else if (!strcmp(arg, "--end"))
        {
            args.end = atoi(argv[++i]);
        }
        else if (!strcmp(arg, "--runFrames"))
        {
            args.runFrames = atoi(argv[++i]);
        }
        else if (!strcmp(arg, "--dumpDraw"))
        {
            args.dumpDraw = TCG_TRUE;
        }
        else if (!strcmp(arg, "--dumpFrame"))
        {
            args.dumpFrame = TCG_TRUE;
        }
        else if (!strcmp(arg, "--finish"))
        {
            args.finish = TCG_TRUE;
        }
        else if (!strcmp(arg, "--swapbuf"))
        {
            args.swapbuf= TCG_TRUE;
        }
        else if (!strcmp(arg, "--rendertime"))
        {
            args.renderingtime= TCG_TRUE;
        }
        else if (!strcmp(arg, "--zoom"))
        {
            args.zoom= TCG_TRUE;
        }
        else if (!strcmp(arg, "--analyze"))
        {
            args.analyze = TCG_TRUE;
        }
        else if (!strcmp(arg, "--default-context"))
        {
            args.default_context= TCG_TRUE;
        }
        else if (!strcmp(arg, "--fps"))
        {
            if (!(i + 1 < argc))
            {
                PrintHelp();
                return TCG_FALSE;
            }
            if (!from_string<TCGint>(args.fpsinterval, argv[i+1]) || args.fpsinterval < 0)
            {
                TCGLOG(TCG_ERROR, TCG_CLIENT_OS, "%s is not a valid fps interval\n\n", argv[i+1]);
                return TCG_FALSE;
            }
            ++i;
        }
        else if (!strcmp(arg, "--config"))
        {
            if (!(i + 1 < argc))
            {
                PrintHelp();
                return TCG_FALSE;
            }
            if (!from_string<EGLint>(args.configid, argv[i+1]))
            {
                TCGLOG(TCG_ERROR, TCG_CLIENT_OS, "'%s' is not a valid config id\n\n.", argv[i+1]);
                return TCG_FALSE;
            }
            ++i;
        }
        else if (!strcmp(arg, "--patchid"))
        {
            if (!(i + 1 < argc))
            {
                PrintHelp();
                return TCG_FALSE;
            }
            if (!from_string<EGLenum>(args.patchid, argv[i+1]))
            {
                TCGLOG(TCG_ERROR, TCG_CLIENT_OS, "'%s' is not a valid patch id\n\n.", argv[i+1]);
                return TCG_FALSE;
            }
            ++i;
        }
        else if (!strcmp(arg, "--help")||!strcmp(arg, "-h")||(arg[0] == '-'))
        {
            PrintHelp();
            return TCG_FALSE;
        }
    }

    return TCG_TRUE;
}

TCGClient::TCGClient()
{
    CreateEventMemPool();

    m_pLibs = TCG_NEW loader(LIB_GLES1, LIB_GLES2, LIB_EGL);
    if (m_pLibs == NULL)
    {
        TCGLOG(TCG_ERROR, TCG_CLIENT_PLAYER, "%s(%d)", __FUNCTION__, __LINE__);
        throw std::runtime_error("Cannot load egl/gl librarys");
    }

#ifdef ANDROID
    memset(&m_engineObj, 0, sizeof(m_engineObj));
#endif
}

TCGClient::~TCGClient()
{
    DestroyEventMemPool();

    if (m_pLibs)
    {
        TCG_DELETE m_pLibs;
    }
}

TCGvoid TCGClient::AddThreadPriority()
{
#ifdef ANDROID
#if 0    /* Add thread priority */
    pthread_attr_t  attr;
    TCGbool         policy;
    TCGbool         rs;
    struct          sched_param param;
    TCGbool         maxThreadPriority;

    rs = pthread_attr_init(&attr);
    pthread_attr_getschedpolicy(&attr, &policy);

    if (policy != SCHED_RR)
    {
        pthread_attr_setschedpolicy(&attr, SCHED_RR);
    }

    maxThreadPriority = sched_get_priority_max(SCHED_RR);

    rs = pthread_attr_getschedparam(&attr, &param);
    policy =  param.sched_priority;
    if (policy < maxThreadPriority)
    {
        param.sched_priority = maxThreadPriority;
        pthread_attr_setschedparam(&attr, &param);

        rs = pthread_attr_getschedparam(&attr, &param);
    }
#endif
#endif
}

TCGvoid TCGClient::GetScreenResolution()
{
#ifdef ANDROID
    char szRead[9];

    FILE *pFile = fopen("/data/data/com.tencent.tcgplayer/files/ratio.txt","rb");
    if (pFile == 0)
    {
        TCGLOG(TCG_INFO, TCG_CLIENT_OS, "Open file fail\n");
    }

    if ((fread(szRead, 1, 9, pFile)) <= 0)
    {
        g_tWindowPos.nWidth    = 1920;
        g_tWindowPos.nHeight   = 1080;
    }
    else
    {
        TCGbool Len = strlen(szRead);
        TCGbool nCount;
        for (nCount = 0; nCount < 9; nCount++)
        {
            if (szRead[nCount] == '*')
            {
                break;
            }
        }

        TCGchar* strWidth;
        strWidth = (TCGchar*)TCG_MALLOC(nCount + 1, eTCG_MAIN);
        memcpy(strWidth, szRead, nCount);
        strWidth[nCount] = '\0';
        g_tWindowPos.nWidth = atoi(strWidth);

        TCGchar* strHeight;
        strHeight = (TCGchar*)TCG_MALLOC(Len - nCount, eTCG_MAIN);
        memcpy(strHeight, szRead + nCount + 1, Len - nCount - 1);
        strHeight[Len - nCount - 1] = '\0';
        g_tWindowPos.nHeight = atoi(strHeight);
    }

    fclose(pFile);
#else
    g_tWindowPos.nWidth    = g_args.width;
    g_tWindowPos.nHeight   = g_args.height;
#endif
}

TCGvoid TCGClient::GetScreenPosition()
{
#ifdef ANDROID
    char szPosRead[9];

    FILE *pPosFile = fopen("/data/data/com.tencent.tcgplayer/files/pos.txt","rb");
    if (pPosFile == 0)
    {
        TCGLOG(TCG_INFO, TCG_CLIENT_OS, "Open file fail\n");
    }

    if ((fread(szPosRead, 1, 9, pPosFile)) <= 0)
    {
        g_tWindowPos.nPosX = 0;
        g_tWindowPos.nPosY = 0;
    }
    else
    {
        TCGbool Len = strlen(szPosRead);
        TCGbool nCount;
        for (nCount = 0; nCount < Len; nCount++)
        {
            if (szPosRead[nCount] == '*')
            {
                break;
            }
        }
        TCGchar* strPosX;
        strPosX = (TCGchar*)TCG_MALLOC(nCount + 1, eTCG_MAIN);
        memcpy(strPosX, szPosRead, nCount);
        strPosX[nCount] = '\0';
        g_tWindowPos.nPosX = atoi(strPosX);
        
        TCGchar* strPosY;
        strPosY = (TCGchar*)TCG_MALLOC(Len - nCount, eTCG_MAIN);
        memcpy(strPosY, szPosRead + nCount + 1, Len - nCount - 1);
        strPosY[Len - nCount - 1] = '\0';
        g_tWindowPos.nPosY = atoi(strPosY);
    }

    fclose(pPosFile);
#endif
}

TCGvoid TCGClient::ConnectServer()
{
#ifndef USE_FILE
    // Network initialize
    g_GetClient()->Init();
#else
    InitGlobal();
#endif
}

TCGbool TCGClient::Init(TCGint argc, TCGchar **argv, const TCGchar* fname)
{
#ifdef Android
    if (!ParseArgFile(PLAY_CONF_FILE, g_args))
    {
        return ;
    }
#elif defined(WIN32) || defined(__APPLE__)
    if (!ParseArgv(argc, argv, g_args))
    {
        return TCG_FALSE;
    }
#endif

    GetScreenResolution();
    GetScreenPosition();
    ConnectServer();

    m_stream.SetCompressFlag(g_args.compressedFile);

    return TCG_TRUE;
}

TCGvoid TCGClient::UnInit()
{
	g_GetClient()->UnInit();
}

#if defined ANDROID
TCGbool TCGClient::Init_sdk(TCGint argc, TCGchar **argv, const TCGchar* fname)
{
    if (!ParseArgFile(PLAY_CONF_FILE, g_args))
    {
        return TCG_FALSE;
    }

    ConnectServer();

    return TCG_TRUE;
}
#endif

#if defined(WIN32) || defined(__APPLE__)
TCGbool TCGClient::main(TCGint argc, TCGchar **argv)
{
    Init(argc, argv, NULL);

    try
    {
        BasicWindow<Playback<CPlaybackAdapter>>     win(m_play);
        TokenizerType                               tok(m_tokHeader, &m_stream);
#ifdef API_DUMP
        ApiAnalyzer                                 apiAnalyzer(g_args.analyze, TCG_DEBUG_OUTPUT_PATH, NULL, g_args);
        CPlayer                                     pl(win, *m_pLibs, tok.fileformatversion(), &apiAnalyzer, &m_stream);
#else
        CPlayer                                     pl(win, *m_pLibs, tok.fileformatversion(), &m_stream);
#endif
        Parser                                      parser(pl, tok);

        g_pPlayer = &pl;

#ifdef API_DUMP
        apiAnalyzer.SetPlayer(&pl);
#endif

        TCGLOG(TCG_INFO, TCG_CLIENT_OS, "Start cloud render playback...\n");
        parser.ParseAll();
        TCGLOG(TCG_INFO, TCG_CLIENT_OS, "\ncloud render playback is completed.\n\n");
    }
    catch (const exception& e)
    {
        std::string err = e.what();
        TCGLOG(TCG_ERROR, TCG_CLIENT_OS, "\n\nException: %s\n\n", err.c_str());
        tcgOS_TerminalUninit();
        tcgOS_Sync();
    }
    catch (...)
    {
        TCGLOG(TCG_ERROR, TCG_CLIENT_OS, "Unexpected Exception\n");
        tcgOS_TerminalUninit();
        tcgOS_Sync();
    }

    return TCG_TRUE;
}
#elif defined(ANDROID)
/**
 * Process the next input event.
 */
static int32_t EngineHandleInput(struct android_app* app, AInputEvent* event)
{
    struct Engine* engine = (struct Engine*)app->userData;

    if (AInputEvent_getType(event) == AINPUT_EVENT_TYPE_MOTION)
    {
        int32_t x = AMotionEvent_getX(event, 0);
        int32_t y = AMotionEvent_getY(event, 0);

        if(abs((engine->state.x -x) + (engine->state.y - y)) > 4)
        {
            engine->state.x = x;
            engine->state.y = y;
            if(engine->state.x < engine->state.width/2 )
            {
                scr_touch = 1;
            }
            else
            {
                scr_touch = 2;
            }
        }

        return 1;
    }

    return 0;
}

/**
 * Process the next main command.
 */
CLock _Lock;
static TCGvoid EngineHandleCmd(struct android_app* app, TCGint cmd)
{
    struct Engine* engine = (struct Engine*)app->userData;
    switch (cmd)
    {
    case APP_CMD_SAVE_STATE:
        // The system has asked us to save our current state.  Do so.
        engine->app->savedState = TCG_MALLOC(sizeof(struct SavedState), eTCG_MAIN);
        *((struct SavedState*)engine->app->savedState) = engine->state;
        engine->app->savedStateSize = sizeof(struct SavedState);
        break;

    case APP_CMD_INIT_WINDOW:
        //窗口初始化
        _Lock.Lock();
        g_bWindowMax = TCG_TRUE;
        _Lock.UnLock();
        
        if (engine->app->window != NULL)
        {
            g_WindowType = engine->app->window;
        }
        break;

    case APP_CMD_TERM_WINDOW:
        //窗口销毁
        _Lock.Lock();
        g_bWindowMax = TCG_FALSE;
        _Lock.UnLock();
        break;

    case APP_CMD_GAINED_FOCUS:
        // When our app gains focus, we start monitoring the accelerometer.
        if (engine->accelerometerSensor != NULL)
        {
            ASensorEventQueue_enableSensor(engine->sensorEventQueue,
                    engine->accelerometerSensor);
            // We'd like to get 60 events per second (in us).
            ASensorEventQueue_setEventRate(engine->sensorEventQueue,
                    engine->accelerometerSensor, (1000L/60)*1000);
        }
        break;

    case APP_CMD_LOST_FOCUS:
        // When our app loses focus, we stop monitoring the accelerometer.
        // This is to avoid consuming battery while not being used.
        if (engine->accelerometerSensor != NULL)
        {
            ASensorEventQueue_disableSensor(engine->sensorEventQueue,
                    engine->accelerometerSensor);
        }
        // Also stop animating.
        engine->animating = 0;
        break;

    default:
        break;
    }
}

TCGbool TCGClient::ParseArgFile(const TCGchar* fname, stCmdArgs &args)
{
    auto_file   f(fopen(fname, "r"));
    TCGchar     line[CMD_LEN] = {0};
    TCGchar*    argv[ARG_ITEMS]={NULL};
    TCGbool     loop = 1;

    if (!f.file_)
    {
        TCGLOG(TCG_INFO, TCG_CLIENT_OS, "No config file TCGPlayer.conf @ /sdcard/Tencent/, use Default config replay");
        return TCG_TRUE;  //use default
    }

    fgets(line,CMD_LEN,f.file_);
    if(line == NULL)
    {
        PrintHelp();
        return TCG_FALSE;
    }

    //Handle use echo xxx > TCGPlayer.conf method generate config file
    if (line[strlen(line)-1] == 10)
    {
        line[strlen(line)-1] = '\0';
    }

    argv[1] = strtok(line," ");
    if(argv[1] == NULL)
    {
        argv[1] = line;
    }

    loop++;
    while(loop < ARG_ITEMS)
    {
        argv[loop] = strtok(NULL," ");
        if(argv[loop] == NULL) break;
        loop++;
    }

    return ParseArgv(loop,argv,args);
}

TCGvoid TCGClient::AndroidInit(struct android_app* state)
{
    state->userData     = &m_engineObj;
    state->onAppCmd     = EngineHandleCmd;
    state->onInputEvent = EngineHandleInput;
    m_engineObj.app     = state;
    
    // Prepare to monitor accelerometer
    m_engineObj.sensorManager         = ASensorManager_getInstance();
    m_engineObj.accelerometerSensor   = ASensorManager_getDefaultSensor(m_engineObj.sensorManager, ASENSOR_TYPE_ACCELEROMETER);
    m_engineObj.sensorEventQueue      = ASensorManager_createEventQueue(m_engineObj.sensorManager, state->looper, LOOPER_ID_USER, NULL, NULL);
    
    if (state->savedState != NULL)
    {
        // We are starting with a previous saved state; restore from it.
        m_engineObj.state = *(struct SavedState*)state->savedState;
    }
}

TCGvoid TCGClient::main(struct android_app* state)
{
    Init(0, NULL, PLAY_CONF_FILE);
    AndroidInit(state);

    try
    {
        g_handle = tcgOS_Dlopen("/data/data/com.tencent.tcgplayer/lib/libAndroidPlayerUtil.so");

        BasicWindow<Playback<CPlaybackAdapter> >     win(m_play);
        win.SetEngine(&m_engineObj);

        TCGLOG(TCG_ERROR, TCG_CLIENT_OS, "%s(%d): Create window object.\n", __FUNCTION__, __LINE__);

        TokenizerType                   tok(m_tokHeader, &m_stream);
        CPlayer                         pl(win, *m_pLibs, tok.fileformatversion(), &m_stream);
        Parser                          p(pl, tok);
        CPlaybackAdapter                adapter(pl, tok, p, state);

        TCGLOG(TCG_ERROR, TCG_CLIENT_OS, "%s(%d): Create playback adapter.\n", __FUNCTION__, __LINE__);

        // Wait for window ready on OS side
        while (m_engineObj.app->window == 0)
        {
            while(win.HasEvent(0))
                win.ReadEvent(0);

            tcgOS_Sleep(1);
        }

        AddThreadPriority();
        TCGLOG(TCG_ERROR, TCG_CLIENT_OS, "Start GL API Trace playback\n");
        m_play.play(win, adapter, g_args.readterminal, g_args.start, g_args.end);
    }
    catch (const exception& e)
    {
        std::string err = e.what();
        TCGLOG(TCG_ERROR, TCG_CLIENT_OS, "\nException: %s\n", err.c_str());
        tcgOS_TerminalUninit();
        tcgOS_Sync();
        if(g_handle != NULL)
        {
            tcgOS_Dlclose(g_handle);
            g_handle = NULL;
        }
    }
    catch (...)
    {
        TCGLOG(TCG_ERROR, TCG_CLIENT_OS, "\nUnexpected Exception\n");
        tcgOS_TerminalUninit();
        tcgOS_Sync();
        if(g_handle != NULL)
        {
            tcgOS_Dlclose(g_handle);
            g_handle = NULL;
        }
    }

    if (g_handle != NULL)
    {
        tcgOS_Dlclose(g_handle);
        g_handle = NULL;
    }
}

// the function used by SDK (add by SUSON)
TCGvoid TCGClient::main_sdk(EGLNativeWindowType window)
{
    g_bWindowMax = true;
    Init_sdk(0, NULL, PLAY_CONF_FILE);
    m_engineObj.app = new android_app();
    m_engineObj.app->window = window;
    try
    {
        g_handle = tcgOS_Dlopen("/data/data/com.tencent.tcgplayer/lib/libAndroidPlayerUtil.so");

        BasicWindow<Playback<CPlaybackAdapter> >     win(m_play);
        win.SetEngine(&m_engineObj);

        TCGLOG(TCG_ERROR, TCG_CLIENT_OS, "%s(%d): Create window object.\n", __FUNCTION__, __LINE__);

        TokenizerType                   tok(m_tokHeader, &m_stream);
        CPlayer                         pl(win, *m_pLibs, tok.fileformatversion(), &m_stream);
        Parser                          p(pl, tok);
        CPlaybackAdapter                adapter(pl, tok, p, NULL);

        TCGLOG(TCG_ERROR, TCG_CLIENT_OS, "%s(%d): Create playback adapter.\n", __FUNCTION__, __LINE__);

        // Wait for window ready on OS side
        while (m_engineObj.app->window == 0)
        {
            while(win.HasEvent(0))
                win.ReadEvent(0);

            tcgOS_Sleep(1);
        }

        AddThreadPriority();
        TCGLOG(TCG_ERROR, TCG_CLIENT_OS, "Start GL API Trace playback\n");
        m_play.play(win, adapter, g_args.readterminal, g_args.start, g_args.end);
    }
    catch (const exception& e)
    {
        std::string err = e.what();
        TCGLOG(TCG_ERROR, TCG_CLIENT_OS, "\nException: %s\n", err.c_str());
        tcgOS_TerminalUninit();
        tcgOS_Sync();
        if(g_handle != NULL)
        {
            tcgOS_Dlclose(g_handle);
            g_handle = NULL;
        }
    }
    catch (...)
    {
        TCGLOG(TCG_ERROR, TCG_CLIENT_OS, "\nUnexpected Exception\n");
        tcgOS_TerminalUninit();
        tcgOS_Sync();
        if(g_handle != NULL)
        {
            tcgOS_Dlclose(g_handle);
            g_handle = NULL;
        }
    }

    if (g_handle != NULL)
    {
        tcgOS_Dlclose(g_handle);
        g_handle = NULL;
    }
}
#endif

#ifdef ANDROID
CPlaybackAdapter::CPlaybackAdapter(CPlayer& pl,
                                   TokenizerType& tok,
                                   Parser& pars,
                                   struct android_app *pState) :
  m_player(pl),
  m_tokenizer(tok),
  m_parser(pars),
  m_frames(0)
{
}
#else
CPlaybackAdapter::CPlaybackAdapter(CPlayer& pl,
                                   TokenizerType& tok,
                                   Parser& pars) :
  m_player(pl),
  m_tokenizer(tok),
  m_parser(pars),
  m_frames(0)
{
}
#endif

TCGvoid CPlaybackAdapter::PlayFrame()
{
    /* For multithread rendering, we will parse event always. */
    while (TCG_TRUE)
    {
#ifdef ANDROID
        _Lock.Lock();
        TCGbool bWindowMax = g_bWindowMax;
        _Lock.UnLock();

        if (m_parser.IsEglSwapEvent() || !bWindowMax)
        {
            m_parser.ClearEglSwapFlag();
        }
        if (!bWindowMax)
        {
            continue;
        }
#endif
        m_parser.ParseSingleEvent();
        m_player.SyncLog();

#if defined(USE_FILE) || TCG_AUTO_TEST
        if (g_bParserFinish && IsRenderFinish())
        {
            break;
        }
#endif
    }

    m_player.ResetFrame();
    m_player.SyncLog();
}

TCGvoid CPlaybackAdapter::SkipRendering(TCGbool val)
{
    m_player.SkipRendering(val);
}

#if !defined(ANDROID)
TCGint main(TCGint argc, char* argv[])
{
    g_GetClient()->Init();
    g_GetClient()->Update();
    g_GetClient()->UnInit();
    return 0;
}
#else
/**
 * This is the main entry point of a native application that is using
 * android_native_app_glue.  It runs in its own thread, with its own
 * event loop for receiving input events and doing other things.
 */
void android_main(struct android_app* state)
{
    TCGClient client;

    client.main(state);

    return ;
}

#endif

