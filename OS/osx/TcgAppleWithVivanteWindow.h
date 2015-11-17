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

#ifndef APP_WITH_VIVANTE_H
#define APP_WITH_VIVANTE_H

#if !defined(LINUX) && !defined(__APPLE__) && !defined(__QNXNTO__)
#  error this implementation is for linux/apple/qnx only
#endif
#if 0
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/keysym.h>
#else
#include "gc_vdk.h"
extern vdkEGL g_Egl;
#endif
#include <stdexcept>
#include <sys/time.h>
#include "TCGWindow.h"

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
          height_(0)
        {
        }
        ~BasicWindow()
        {
#if 0
            if (window_) XDestroyWindow(disp_, window_);
            if (disp_)   XCloseDisplay(disp_);
#else
            if (window_)
            {
                vdkDestroyWindow(window_);
            }
            
            if (disp_)
            {
                vdkDestroyDisplay(disp_);
            }
#endif
        }
        void Create(TCGint threadId, const std::string& title, int visualid, int width, int height)
        {
            width   = g_tWindowPos.nWidth;
            height  = g_tWindowPos.nHeight;
            
            if (window_)
            {
                if (width_ < width) width_ = width;
                if (height_ < height) height_ = height;
                return;
            }

#if 0
            Display* dpy = XOpenDisplay(NULL);
            // select appropriate attributes for the window based on EGLDisplay
            XVisualInfo  vistemplate = {};
            vistemplate.visualid     = visualid;
            int num_visuals          = 0;

            XVisualInfo* visinfo = XGetVisualInfo(dpy, VisualIDMask, &vistemplate, &num_visuals);
            if (!visinfo)
                throw std::runtime_error("cannot get visual info");

            Window root = RootWindow(dpy, DefaultScreen(dpy));

            XSetWindowAttributes attr;
            attr.background_pixel = 0;
            attr.border_pixel     = 0;
            attr.colormap         = XCreateColormap(dpy, root, visinfo->visual, AllocNone);
            attr.event_mask       = StructureNotifyMask | ExposureMask | KeyPressMask;
            Window win            = XCreateWindow(dpy, root, 0, 0, width, height, 0, visinfo->depth, InputOutput, visinfo->visual,
                                                  CWBackPixel | CWBorderPixel | CWColormap | CWEventMask,
                                                  &attr);
            XFree(visinfo);
            if (!win)
                throw std::runtime_error("cannot create window");

            if (!title.empty())
                XSetStandardProperties(dpy, win, title.c_str(), title.c_str(), None, (char**)NULL, 0, NULL);

            XMapWindow(dpy, win);
            XSync(dpy, 1);
            window_ = win;
            disp_   = dpy;
#else
            g_Egl.window = vdkCreateWindow(g_Egl.display, 0, 0, width, height);
            window_ = g_Egl.window;
            disp_   = g_Egl.display;
#endif
            
            width_  = width;
            height_ = height;
        }
        int Width(TCGint threadId) const
        {
            return width_;
        }
        int Height(TCGint threadId) const
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

        bool HasEvent(TCGint threadId)
        {
#if 0
            if (!disp_)
                return false;
            XFlush(disp_);
            return XPending(disp_) != 0;
#else
            return true;
#endif
        }

        void ReadEvent(TCGint threadId)
        {
#if 0
            XEvent event = {};
            XNextEvent(disp_, &event);
            switch (event.type)
            {
                case Expose:
                {
                    WindowEventRepaint paint;
                    controller_.event(paint);
                }
                break;
                case KeyPress:
                {
                    WindowEventKeyboard keyb = {0};

                    switch (XLookupKeysym(&event.xkey, 0))
                    {
                        case XK_F1: keyb.val = KEY_F1; break;
                        case XK_F2: keyb.val = KEY_F2; break;
                        case XK_F3: keyb.val = KEY_F3; break;
                        case XK_F4: keyb.val = KEY_F4; break;
                        case XK_F5: keyb.val = KEY_F5; break;
                        default:
                        {
                            char buff[10] = {0};
                            XLookupString(&event.xkey, buff, sizeof(buff), NULL, NULL);
                            keyb.val = buff[0];
                        }
                        break;
                    }
                    controller_.event(keyb);
                    // todo: modifiers
                }
                break;

                case DestroyNotify:
                {
                    window_close_event close;
                    controller_.event(close);
                }
                default:
                    break;
            }
#endif
        }
        void Zoom(TCGint threadId, int &x, int &y, int &w, int &h)
        {
        }
        void SetZoom(TCGint threadId, bool z)
        {
        }

    private:
        BasicWindow(BasicWindow&);
        BasicWindow& operator=(BasicWindow&);

        Controller& controller_;
        Display*    disp_;
        Window      window_;
        int         width_;
        int         height_;

    };

} // TCG

#endif // APP_WITH_VIVANTE_H

