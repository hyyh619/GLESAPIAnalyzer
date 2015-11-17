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

#ifndef TCG_VDK_WINDOW_H
#define TCG_VDK_WINDOW_H

#if !(defined(LINUX) || defined(__QNXNTO__))
#  error this implementation is for linux/qnx only
#endif
#include "VDK/gc_vdk.h"
#include <stdexcept>
#include "TCGWindow.h"

/* Call this libGAL API to force player linking with libGAL.so */
extern "C" int gcoHAL_SetHardwareType(int, int);

namespace TCG
{
    template<typename Controller>
    class BasicWindow : public window
    {
    public:
        BasicWindow(Controller& cont) :
          controller_(cont),
          disp_(NULL),
          window_(0),
          width_(0),
          height_(0),
          ratio_(1.0),
          zoom_(false)
        {
            memset(&egl, 0, sizeof(vdkEGL));
            hgal_ = dlopen("libGAL.so");
            if (!hgal_) 
                throw std::runtime_error("libGAL.so library canot find");
            hvdk_ = dlopen("libVDK.so");
            if (!hvdk_) 
                throw std::runtime_error("libVDK.so library canot find");

            func_vdkinit=(fun_vdkInitialize)dlresolve( hvdk_, "vdkInitialize");
            func_vdkhidewin=(fun_vdkHideWindow)dlresolve( hvdk_, "vdkHideWindow");
            func_vdkdeswin=(fun_vdkDestroyWindow)dlresolve( hvdk_, "vdkDestroyWindow");
            func_vdkdesdisp=(fun_vdkDestroyDisplay)dlresolve( hvdk_, "vdkDestroyDisplay");
            func_vdkgetdisp=(fun_vdkGetDisplay)dlresolve( hvdk_, "vdkGetDisplay");
            func_vdkcrewin=(fun_vdkCreateWindow)dlresolve( hvdk_, "vdkCreateWindow");
            func_vdksetwintitle=(fun_vdkSetWindowTitle)dlresolve( hvdk_, "vdkSetWindowTitle");  
            func_vdkshowwin=(fun_vdkShowWindow)dlresolve( hvdk_, "vdkShowWindow");
            func_vdkgetevent=(fun_vdkGetEvent)dlresolve( hvdk_, "vdkGetEvent");
            func_vdkgetdispinfo=(fun_vdkGetDisplayInfo)dlresolve( hvdk_, "vdkGetDisplayInfo");
            func_sethwtype=(fun_HAL_SetHardwareType)dlresolve( hgal_, "gcoHAL_SetHardwareType");

            egl.vdk = func_vdkinit();
            func_sethwtype(0, 1);
        }
       ~BasicWindow()
        {
            if(egl.window != NULL)
            {
                func_vdkhidewin(egl.window);
                func_vdkdeswin(egl.window);
                window_ = egl.window = NULL;
            }
            if(egl.display != NULL)
            {
                func_vdkdesdisp(egl.display);
                disp_ = egl.display = NULL;
            }
            if (hvdk_)
                dlclose(hvdk_);
            if (hgal_)
                dlclose(hgal_);
        }
        void create(const std::string& title, int visualid, int width, int height)
        {
        
            if (window_)
            {
                if (width_ < width) width_ = width;
                if (height_ < height) height_ = height;
                return;
            }

            egl.display = func_vdkgetdisp(egl.vdk);
            egl.window = func_vdkcrewin(egl.display, -1, -1, width, height);

            // Set window title and show the window.
            func_vdksetwintitle(egl.window, "Tracer player");
            func_vdkshowwin(egl.window);

            window_ = egl.window;
            disp_   = egl.display;
            width_  = width;
            height_ = height;
        }
        int width() const
        {
            return width_;
        }
        int height() const
        {
            return height_;
        }
        bool Exists(TCGint threadId) const
        {
            return window_ != 0;
        }
        NativeWindowType Handle(TCGint threadId)
        {
            return window_;
        }

        bool has_event()
        {
            return func_vdkgetevent(egl.window, &event);
        }

        void read_event()
        {
            // Test for Keyboard event.
            if ((event.type == VDK_KEYBOARD) && event.data.keyboard.pressed)
            {
                WindowEventKeyboard keyb = {0};

                // Test for key.
                switch (event.data.keyboard.scancode)
                {
                    case VDK_F1: keyb.val = KEY_F1; break;
                    case VDK_F2: keyb.val = KEY_F2; break;
                    case VDK_F3: keyb.val = KEY_F3; break;
                    case VDK_F4: keyb.val = KEY_F4; break;
                    case VDK_F5: keyb.val = KEY_F5; break;
                default:
                    keyb.val = VDK_UNKNOWN;
                    break;
                }
                controller_.event(keyb);
            }
            else if (event.type == VDK_CLOSE)
            {
                window_close_event close;
                controller_.event(close);
            }
            else if (event.type == VDK_WINDOW_UPDATE)
            {
                WindowEventRepaint paint;
                controller_.event(paint);
            }
        }
        void zoom(int &x, int &y, int &w, int &h)
        {
            if (zoom_)
            {
                int window_width = 0;
                int window_height = 0;
                func_vdkgetdispinfo(disp_,&window_width, &window_height,0,0,0);
                if (window_height == 0 || window_width == 0) return;

                if (window_height < (h+y) || window_width < (w+x))
                {
                    float ratioX = (float)(w+x) / window_width;
                    float ratioY = (float)(h+y) / window_height;
                    ratio_ = (ratioX > ratioY) ? ratioX: ratioY;
                 }

                if (ratio_ != 1.0)
                {
                    x = (int)(x / ratio_);
                    y = (int)(y / ratio_);
                    w = (int)(w / ratio_);
                    h = (int)(h / ratio_);
                }
            }
        }
        void set_zoom(bool z) 
        {
            zoom_ = z;
        }

    private:
        BasicWindow(BasicWindow&);
        BasicWindow& operator=(BasicWindow&);

        Controller& controller_;
        vdkDisplay  disp_;
        vdkWindow   window_;
        int         width_;
        int         height_;
        LIB_HANDLE  hvdk_;
        LIB_HANDLE  hgal_;
		typedef  vdkPrivate (VDKLANG *fun_vdkInitialize)();
		typedef  int (VDKLANG *fun_vdkHideWindow)(vdkWindow);
		typedef  void (VDKLANG *fun_vdkDestroyWindow)(vdkWindow);
		typedef  void (VDKLANG *fun_vdkDestroyDisplay)(vdkDisplay);
		typedef  vdkDisplay (VDKLANG *fun_vdkGetDisplay)(vdkPrivate);
		typedef  vdkWindow (VDKLANG *fun_vdkCreateWindow)(vdkDisplay, int, int, int, int);
		typedef  void (VDKLANG *fun_vdkSetWindowTitle)(vdkWindow, const char*);
		typedef  int (VDKLANG *fun_vdkShowWindow)(vdkWindow);
		typedef  int (VDKLANG *fun_vdkGetEvent)(vdkWindow, vdkEvent*);
		typedef  int (VDKLANG *fun_vdkGetDisplayInfo)(vdkDisplay, int*, int*, unsigned long*, int*, int*);
        typedef  int (* fun_HAL_SetHardwareType)(int, int);

        fun_vdkInitialize       func_vdkinit;
        fun_vdkHideWindow       func_vdkhidewin;
        fun_vdkDestroyWindow    func_vdkdeswin;
        fun_vdkDestroyDisplay   func_vdkdesdisp;
        fun_vdkGetDisplay       func_vdkgetdisp;
        fun_vdkCreateWindow     func_vdkcrewin;
        fun_vdkSetWindowTitle   func_vdksetwintitle;
        fun_vdkShowWindow       func_vdkshowwin;
        fun_vdkGetEvent         func_vdkgetevent;
        fun_vdkGetDisplayInfo   func_vdkgetdispinfo;
        fun_HAL_SetHardwareType func_sethwtype;

        // to hold vdk information.
        vdkEGL      egl;
        vdkEvent    event;
        float          ratio_;
        bool           zoom_;

    };

} // app

#endif // TCG_VDK_WINDOW_H
