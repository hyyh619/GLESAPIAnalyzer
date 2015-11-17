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
#include "global.h"
#include "TCGPlayer.h"
#include "TCGParser.h"
#include "TCGTokenizer.h"
#include "TCGWindow.h"
#include "TCGPlatform.h"
#include "TCGLoader.h"
#include "TCGPlayback.h"
#include "TCGUtility.h"
#include "TCGFrameSync.h"
#ifdef API_DUMP
#include "ApiAnalyzer.h"
#endif
#include "SingletonClass.h"

#if defined(LINUX) || defined(__APPLE__) || defined(ANDROID)
#   include <signal.h>
#   if defined(ANDROID)
#       include "TCGAndroidWindow.h"
#   elif defined(APPLE_WITH_VIVANTE)
#       include "TCGAppleWithVivanteWindow.h"
#   else
#       include "TCGLinuxWindow.h"
#   endif
#else
#   ifndef _WIN32_WINNT
#       define _WIN32_WINNT 0x0500
#   endif
#   include <windows.h>
#   include <dbghelp.h> // for minidump
#   pragma comment(lib, "dbghelp.lib")
#   include "TCGWindowsWindow.h"
#endif

using namespace std;
using namespace TCG;

typedef tokenizer TokenizerType;

#define PLAY_CONF_FILE          "/sdcard/Tencent/TCGPlayer.conf"
#define ARG_ITEMS               40
#define CMD_LEN                 512

class CPlaybackAdapter
{
public:
#ifdef ANDROID
    CPlaybackAdapter(CPlayer& pl, TokenizerType& tok, Parser& pars, struct android_app *pState);
#else
    CPlaybackAdapter(CPlayer& pl, TokenizerType& tok, Parser& pars);
#endif

    TCGvoid     PlayFrame();
    TCGvoid     SkipRendering(TCGbool val);

public:
    TCG::CPlayer&                           m_player;
    TokenizerType&                          m_tokenizer;
    Parser&                                 m_parser;
    TCGint                                  m_frames;

#ifdef ANDROID
    struct android_app                      *m_pState;
#endif
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

#if defined(WIN32)
    TCGbool                     Update(TCGint argc, TCGchar **argv);
#elif defined(ANDROID)
    TCGbool                     ParseArgFile(const TCGchar* fname, stCmdArgs &args);
    TCGvoid                     Update(struct android_app* state);
    TCGvoid                     UpdateSDK(EGLNativeWindowType window); // the function used by SDK (add by SUSON)
#endif

private:
    TCGbool                     ParseArgv(TCGint argc, TCGchar* argv[], stCmdArgs& args);
    TCGvoid                     AddThreadPriority();
    TCGvoid                     ConnectServer();

#if defined ANDROID
    TCGbool                     Init(TCGint argc, TCGchar **argv, const TCGchar* fname);
    TCGvoid                     InitAndroid(struct android_app* state);
#else
    TCGvoid                     PrintHelp();
    TCGbool                     Init(TCGint argc, TCGchar **argv, const TCGchar* fname);
#endif

private:
    CStream                     m_stream;
    loader                      *m_pLibs;
    Playback<CPlaybackAdapter>  m_play;
    TokHeader                   m_tokHeader;

#ifdef ANDROID
    struct android_app          *m_pState;
    struct Engine               m_engineObj;
#endif
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
