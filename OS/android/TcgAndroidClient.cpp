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

#include "TcgAndroidClient.h"
#include "TCGOS.h"
#include "TcgAudioPlay.h"
#include "TcgProtocolProc.h"
#include "TcgTexLocalCache.h"
#include "TCGUtility.h"
#include "TcgLock.h"
#include "TcgDebug.h"
#include "TcgPlayerUtils.h"

#ifdef PERFORMANCE_DEBUG
#include "TcgDumpInfo.h"
#endif //PERFORMANCE_DEBUG

#ifndef USE_FILE
#include "TcgNet.h"
extern CNet* g_pNet;
#endif //USE_FILE

const char* const   LIB_GLES1   = "libGLESv1_CM.so";
const char* const   LIB_GLES2   = "libGLESv2.so";
const char* const   LIB_EGL     = "libEGL.so";

LIB_HANDLE          g_handle;
CPlayer*            g_pPlayer;
GLfloat             g_fWRatio;
GLfloat             g_fHRatio;
GLboolean           g_bSetRatio;
stCmdArgs           g_args;

extern   stWindowPos  g_tWindowPos;

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
 *  Process the next main command.
 */
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
        g_GetClient()->m_lockDevice.Lock();
        g_bWindowMax = TCG_TRUE;
        g_GetClient()->m_lockDevice.UnLock();
        
        if (engine->app->window != NULL)
        {
            g_WindowType = engine->app->window;
        }
        break;

    case APP_CMD_TERM_WINDOW:
        //窗口销毁
        g_GetClient()->m_lockDevice.Lock();
        g_bWindowMax = TCG_FALSE;
        g_GetClient()->m_lockDevice.UnLock();
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

CTCGClient::CTCGClient()
{
#ifndef USE_FILE
    m_pTcgNet = TCG_NULL;
#endif //USE_FILE
    m_pStream = TCG_NULL;
    m_iReturnBackFlag = 0;
    memset(&m_engineObj, 0, sizeof(m_engineObj));
}

CTCGClient::~CTCGClient()
{
}

TCGbool CTCGClient::Init()
{
    g_handle    = NULL;
    g_pPlayer   = NULL;
    g_fWRatio   = 1.0f;
    g_fHRatio   = 1.0f;
    g_bSetRatio = GL_FALSE;
    g_tWindowPos.nPosX = 0;
    g_tWindowPos.nPosY = 0;
    g_tWindowPos.nWidth = 640;
    g_tWindowPos.nHeight = 360;

    CreateEventMemPool();

    m_pLibs = TCG_NEW loader(LIB_GLES1, LIB_GLES2, LIB_EGL);
    if (m_pLibs == NULL)
    {
        TCGLOG(TCG_ERROR, TCG_CLIENT_PLAYER, "%s(%d)", __FUNCTION__, __LINE__);
        throw std::runtime_error("Cannot load egl/gl librarys");
    }

    memset(&m_engineObj, 0, sizeof(m_engineObj));
    memset(&m_tokHeader, 0, sizeof(m_tokHeader));
    m_sTraceProcName = "";
    SetReturnBackFlag(0);

    m_pStream = new CStream();
    if(m_pStream == TCG_NULL)
    {
        TCGLOG(TCG_ERROR, TCG_NET, "%s(%d)", __FUNCTION__, __LINE__);
        throw std::runtime_error("Cannot Create Stream");
    }
    m_pStream->Init();

    g_GetFrameSyn()->Init();

#ifndef USE_FILE
    m_bQuitFlag = TCG_FALSE;

    if (g_pNet == TCG_NULL)
    {
        TCGLOG(TCG_ERROR, TCG_NET, "Net Init Failed!");
    }
    m_pTcgNet = new CTCGNet();
    if(m_pTcgNet == TCG_NULL)
    {
        TCGLOG(TCG_ERROR, TCG_NET, "%s(%d)", __FUNCTION__, __LINE__);
        throw std::runtime_error("Cannot Create Net");
    }
    TCGbool bFlag = m_pTcgNet->Init(g_pNet);

    return bFlag;
#else
    InitGlobal();
    return TCG_TRUE;
#endif //USE_FILE
}

/*
  changed by SUSON
  主动退出TCGPlayer时，先向TCGTracer发送EndTrace的消息
  确认EndTrace消息已经发送出去，再调用UnInit();
*/
TCGbool CTCGClient::UnInit()
{
#ifndef USE_FILE
    if(m_pTcgNet)
    {
        m_pTcgNet->UnInit();
        delete m_pTcgNet;
        m_pTcgNet = TCG_NULL;
    }
#endif //USE_FILE

    if(m_pStream)
    {
        m_pStream->UnInit();
        delete m_pStream;
        m_pStream = TCG_NULL;
    }

    DestroyEventMemPool();

    memset(&m_engineObj, 0, sizeof(m_engineObj));
    memset(&m_tokHeader, 0, sizeof(m_tokHeader));
    m_sTraceProcName = "";

    if (m_pLibs)
    {
        TCG_DELETE m_pLibs;
        m_pLibs = TCG_NULL;
    }

    g_GetFrameSyn()->UnInit();

#if defined(TCG_SOUND)
	AudioUnInit();
#endif

    SetReturnBackFlag(1);

    return TCG_TRUE; 
}

TCGvoid  CTCGClient::SetReturnBackFlag(TCGint flag)
{
    m_lockReturnBack.Lock();
    m_iReturnBackFlag = flag;
    m_lockReturnBack.UnLock();
}

TCGint CTCGClient::GetReturnBackFlag()
{
    return m_iReturnBackFlag;
}

TCGvoid CTCGClient::SendEndTraceEvent()
{
#ifndef USE_FILE
    stNetEndTraceEvent tEndTraceEvent;
    tEndTraceEvent.byEventType      = eNET_EVENT_CS_FRAME_ENDTRACE;
    tEndTraceEvent.unEventSize      = sizeof(tEndTraceEvent.unEndMsg);
    tEndTraceEvent.unOrgSize        = tEndTraceEvent.unEventSize;
    tEndTraceEvent.unEndMsg         = 1;

    m_pTcgNet->SendFrameEvent(&tEndTraceEvent);
#endif //USE_FILE
}

TCGvoid CTCGClient::InitAndroid(struct android_app* state)
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

TCGvoid CTCGClient::Main(struct android_app* state)
{
    try
    {
        g_handle = tcgOS_Dlopen("/data/data/com.tencent.tcgplayer/lib/libAndroidPlayerUtil.so");

        BasicWindow<Playback<CPlaybackAdapter> >     win(m_play);
        win.SetEngine(&m_engineObj);

        TCGLOG(TCG_ERROR, TCG_CLIENT_OS, "%s(%d): Create window object.\n", __FUNCTION__, __LINE__);

        TokenizerType                   tok(m_tokHeader, m_pStream, m_sTraceProcName);
        CPlayer                         pl(win, *m_pLibs, tok.fileformatversion(), m_pStream, m_sTraceProcName);
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

TCGvoid CTCGClient::GetDevice()
{
	m_lockDevice.Lock();
	g_bWindowMax = TCG_TRUE;
	m_lockDevice.UnLock();
}

TCGvoid CTCGClient::LostDevice()
{
	m_lockDevice.Lock();
	g_bWindowMax = TCG_FALSE;
	m_lockDevice.UnLock();
}

TCGbool CTCGClient::IsDeviceLost()
{
	return !g_bWindowMax;
}

TCGvoid CTCGClient::SetWin(EGLNativeWindowType window)
{
	m_locksurface.Lock();
	g_WindowType = window;
	if (!m_engineObj.app)
		m_engineObj.app = new android_app();
	m_engineObj.app->window = window;
	m_locksurface.UnLock();
}

// the function used by SDK (add by SUSON)
TCGvoid CTCGClient::MainSDK(EGLNativeWindowType window)
{
    m_lockDevice.Lock();
    g_bWindowMax = true;
    m_lockDevice.UnLock();

    m_locksurface.Lock();
	if (!m_engineObj.app)
		m_engineObj.app = new android_app();
    m_engineObj.app->window = window;
    m_locksurface.UnLock();

    try
    {
        g_handle = tcgOS_Dlopen("/data/data/com.tencent.tcgplayer/lib/libAndroidPlayerUtil.so");

        BasicWindow<Playback<CPlaybackAdapter> >     win(m_play);
        win.SetEngine(&m_engineObj);

        TCGLOG(TCG_ERROR, TCG_CLIENT_OS, "%s(%d): Create window object.\n", __FUNCTION__, __LINE__);

        TokenizerType                   tok(m_tokHeader, m_pStream, m_sTraceProcName);
        CPlayer                         pl(win, *m_pLibs, tok.fileformatversion(), m_pStream, m_sTraceProcName);
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

        TCGLOG(TCG_ERROR, TCG_RENDER, "Start GL API Trace playback\n");
        m_play.play(win, adapter, g_args.readterminal, g_args.start, g_args.end);
        //等待网络线程结束再结束主线程
#ifndef USE_FILE       
        for(TCGuint i = 0; i < eNET_THREAD_SIZE; ++i)
        {
            pthread_join(m_pTcgNet->GetProtocalProc()->m_atNetThread[i].handle, TCG_NULL);
        }
#endif //USE_FILE
        TCGLOG(TCG_ERROR, TCG_RENDER, "Finish GL API Trace playback\n");
    }
    catch (const exception& e)
    {
        std::string err = e.what();
        TCGLOG(TCG_ERROR, TCG_RENDER, "\nException: %s\n", err.c_str());
        tcgOS_TerminalUninit();
        tcgOS_Sync();
        if(g_handle != NULL)
        {
            tcgOS_Dlclose(g_handle);
            g_handle = NULL;
        }
        //捕获到异常，直接退出进程
#if defined(TCG_SOUND)
	    AudioUnInit();
#endif
        exit(0);
    }
    catch (...)
    {
        TCGLOG(TCG_ERROR, TCG_RENDER, "\nUnexpected Exception\n");
        tcgOS_TerminalUninit();
        tcgOS_Sync();
        if(g_handle != NULL)
        {
            tcgOS_Dlclose(g_handle);
            g_handle = NULL;
        }
        //捕获到异常，直接退出进程
#if defined(TCG_SOUND)
	    AudioUnInit();
#endif
        exit(0);
    }

    if (g_handle != NULL)
    {
        tcgOS_Dlclose(g_handle);
        g_handle = NULL;
    }

    m_locksurface.Lock();
    if (m_engineObj.app)
    {
        delete m_engineObj.app;
        m_engineObj.app = TCG_NULL;
    }
    m_locksurface.UnLock();
}

TCGvoid CTCGClient::SetThreadQuitFlag()
{
    m_lockThreadQuit.Lock();

    SetMainThreadQuitFlag();
    SetNetThreadQuitFlag();

    m_lockThreadQuit.UnLock();
}

TCGvoid CTCGClient::SetMainThreadQuitFlag()
{
#ifndef USE_FILE
    m_bQuitFlag = TCG_TRUE;
#endif //USE_FILE
}

TCGvoid CTCGClient::SetNetThreadQuitFlag()
{
#ifndef USE_FILE
    m_pTcgNet->SetNetThreadQuit();
#endif //USE_FILE
}

CPlaybackAdapter::CPlaybackAdapter(CPlayer& pl, TokenizerType& tok, Parser& pars, struct android_app *pState) :
    m_player(pl),
    m_tokenizer(tok),
    m_parser(pars),
    m_frames(0)
{
}

TCGvoid CPlaybackAdapter::PlayFrame()
{
    /* For multithread rendering, we will parse event always. */
    while (TCG_TRUE)
    {
        g_GetClient()->m_lockDevice.Lock();
        TCGbool bWindowMax = g_bWindowMax;
        g_GetClient()->m_lockDevice.UnLock();

        if (m_parser.IsEglSwapEvent() || !bWindowMax)
        {
            m_parser.ClearEglSwapFlag();
        }
        if (!bWindowMax)
        {
            continue;
        }
        
        m_parser.ParseSingleEvent();
        m_player.SyncLog();

#if defined(USE_FILE) || TCG_AUTO_TEST
        if (g_bParserFinish && IsRenderFinish())
        {
            break;
        }
#else
        if(g_GetClient()->m_bQuitFlag == TCG_TRUE)
        {
            TCGLOG(TCG_ERROR, TCG_RENDER, "Close Render Thread");

            m_player.UnInitRenderThread();
            
            //等待渲染线程结束
            for(TCGuint i = 0; i < RENDER_THREAD_NUM; ++i)
            {
                pthread_join(m_player.m_atRenderThread[i].handle, TCG_NULL);
            }

#ifdef TCG_TEXTURE_LOCAL_CACHE
            TCGLOG(TCG_ERROR, TCG_RENDER, "Close Texture Local Cache Thread");

            m_player.m_pDataManager->m_pTexLocalCache->UnInitThread();

            pthread_join(m_player.m_pDataManager->m_pTexLocalCache->m_tTexLocalCacheThread.handle, TCG_NULL);
#endif //TCG_TEXTURE_LOCAL_CACHE
            break;
        }
#endif //USE_FILE || TCG_AUTO_TEST
    }

    m_player.ResetFrame();
    m_player.SyncLog();
}

TCGvoid CPlaybackAdapter::SkipRendering(TCGbool val)
{
    m_player.SkipRendering(val);
}

/**
 * This is the main entry point of a native application that is using
 * android_native_app_glue.  It runs in its own thread, with its own
 * event loop for receiving input events and doing other things.
 */
void android_main(struct android_app* state)
{
    g_GetClient()->Init();
    g_GetClient()->InitAndroid(state);
    g_GetClient()->Main(state);
    g_GetClient()->UnInit();

    return ;
}
