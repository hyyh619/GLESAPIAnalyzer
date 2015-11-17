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

#ifndef  __TCGCLIENT_H__
#define  __TCGCLIENT_H__

#include <cstdio>
#include <fstream>
#include <exception>
#include <string>
#include <sstream>
#include "TcgGlobal.h"
#include "TcgPlayer.h"
#include "TcgParser.h"
#include "TcgTokenizer.h"
#include "TcgWindow.h"
#include "TcgPlatform.h"
#include "TcgLoader.h"
#include "TcgPlayback.h"
#include "TcgUtility.h"
#include "TcgFrameSync.h"
#ifdef API_DUMP
#include "ApiAnalyzer.h"
#endif
#include "SingletonClass.h"

#include <signal.h>
#include "TcgAndroidWindow.h"

using namespace std;
using namespace TCG;

typedef tokenizer TokenizerType;

#define PLAY_CONF_FILE          "/sdcard/Tencent/TCGPlayer.conf"
#define ARG_ITEMS               40
#define CMD_LEN                 512

class CPlaybackAdapter
{
public:
    CPlaybackAdapter(CPlayer& pl, TokenizerType& tok, Parser& pars, struct android_app *pState);

    TCGvoid                     PlayFrame();
    TCGvoid                     SkipRendering(TCGbool val);

public:
    TCG::CPlayer&               m_player;
    TokenizerType&              m_tokenizer;
    Parser&                     m_parser;
    TCGint                      m_frames;
    struct android_app          *m_pState;
};

class CTCGClient : public ManualDeleteSingleton< CTCGClient >
{
    friend class ManualDeleteSingleton< CTCGClient >;

public:
    CTCGClient();
    virtual ~CTCGClient();

public:
    TCGbool                     Init();
    TCGbool                     UnInit();
    TCGvoid                     Main(struct android_app* state);
    TCGvoid                     MainSDK(EGLNativeWindowType window); // the function used by SDK (add by SUSON)
    TCGvoid                     InitAndroid(struct android_app* state);
    TCGvoid                     SetThreadQuitFlag();
    TCGvoid                     SetMainThreadQuitFlag();
    TCGvoid                     SetNetThreadQuitFlag();
    TCGvoid                     SendEndTraceEvent();
    TCGvoid                     SetReturnBackFlag(TCGint flag);
    TCGint                      GetReturnBackFlag();
	TCGvoid						SetWin(EGLNativeWindowType window);
	TCGvoid						GetDevice();
	TCGvoid						LostDevice();
	TCGbool						IsDeviceLost();
private:
    CStream*                    m_pStream;
    loader                      *m_pLibs;
    Playback<CPlaybackAdapter>  m_play;
    TokHeader                   m_tokHeader;
    struct android_app          *m_pState;
    struct Engine               m_engineObj;

    CLock                       m_lockThreadQuit;
    CLock                       m_lockReturnBack;
    CLock                       m_locksurface;
    volatile TCGint             m_iReturnBackFlag;

    std::string                 m_sTraceProcName;             

public:
#ifndef USE_FILE
    CTCGNet*                    m_pTcgNet;
    volatile TCGbool            m_bQuitFlag;
#endif //USE_FILE
    CLock                       m_lockDevice;
};

// 获得对象
#ifndef g_GetClient
#define g_GetClient()           (CTCGClient::Instance())
#endif // g_GetClient

// 清理对象
#ifndef g_ReleaseClient
#define g_ReleaseClient()       (CTCGClient::Destroy())
#endif

#endif // __TCGCLIENT_H__
