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

#ifndef TCG_ANDROID_WINDOW_H
#define TCG_ANDROID_WINDOW_H

#include "TcgWindow.h"
#include <jni.h>
#include <errno.h>
#include <android_native_app_glue.h>
#include <android/native_window.h>
#include "TcgMacros.h"

namespace TCG
{
    /**
     * Our saved state data.
     */
    struct SavedState
    {
        float angle;
        int32_t x;
        int32_t y;
        int32_t width;
        int32_t height;
    };

    /*
     * Shared state for our app.
     */
    struct Engine
    {
        struct android_app* app;

        ASensorManager      *sensorManager;
        const ASensor       *accelerometerSensor;
        ASensorEventQueue   *sensorEventQueue;

        int                 animating;
        EGLDisplay          display;
        EGLSurface          *surface;
        EGLContext          *context;
        int32_t             width;
        int32_t             height;
        struct SavedState   state;
    };

    template<typename Controller>
    class BasicWindow : public window
    {
    public:
        BasicWindow(Controller& cont) :
          m_controller(cont),
          m_display(NULL)
        {
            for (int i=0; i<RENDER_THREAD_NUM; i++)
            {
                m_windowHandle[i]   = 0;
                m_width[i]          = 0;
                m_height[i]         = 0;
                m_ratio[i]          = 1.0;
                m_zoom[i]           = false;
            }
        }

        ~BasicWindow()
        {
        }

        void Create(TCGint threadId, const std::string& title, int visualid, int width, int height, struct Engine *eng)
        {
            if (m_windowHandle[threadId])
                return;

            m_engine = eng;

            m_windowHandle[threadId] = m_engine->app->window;

            m_display               = &m_engine->display;
            m_width[threadId]   = width;
            m_height[threadId]  = height;
            m_width[threadId]   = ANativeWindow_getWidth(m_windowHandle[threadId]);
            height              = ANativeWindow_getHeight(m_windowHandle[threadId]);

            m_engine->state.width    = m_width[threadId];
            m_engine->state.height   = m_height[threadId];
            m_engine->animating      = 1;
        }

        void Create(TCGint threadId, const std::string& title, int visulaid, int width, int height)
        {
            if (m_width[threadId] > width && width > 256)
                m_width[threadId] = width;

            if (m_height[threadId] > height && width > 256)
                m_height[threadId] = height;

            if (m_windowHandle[threadId] == m_engine->app->window)
                return;

            m_windowHandle[threadId] = m_engine->app->window;
            ANativeWindow_setBuffersGeometry(m_windowHandle[threadId], 0, 0, visulaid);

            m_width[threadId] = ANativeWindow_getWidth(m_windowHandle[threadId]);
            m_height[threadId] = ANativeWindow_getHeight(m_windowHandle[threadId]);
            m_engine->state.width = m_width[threadId];
            m_engine->state.height = m_height[threadId];

            if (m_width[threadId] > width && width > 128)
                m_width[threadId] = width;

            if (m_height[threadId] > height && height > 128)
                m_height[threadId] = height;
        }

        int Width(TCGint threadId) const
        {
            return m_width[threadId];
        }

        int Height(TCGint threadId) const
        {
            return m_height[threadId];
        }

        bool Exists(TCGint threadId) const
        {
            return m_windowHandle[threadId] != 0;
        }

        NativeWindowType Handle(TCGint threadId)
        {
            return m_windowHandle[threadId];
        }

        bool HasEvent(TCGint threadId)
        {
            m_engine->animating = 1;
            m_ident = ALooper_pollAll(0, NULL, &m_events, (void**)&m_source);

            if (m_ident >= 0)
            {
                return true;
            }
            else
            {
                return false;
            }
        }

        void ReadEvent(TCGint threadId)
        {
            // Process this event.
            if (m_source != NULL)
            {
                m_source->process(m_engine->app, m_source);
            }

            // If a sensor has data, process it now.
            if (m_ident == LOOPER_ID_USER)
            {
                if (m_engine->accelerometerSensor != NULL)
                {
                    ASensorEvent event;
                    while (ASensorEventQueue_getEvents(m_engine->sensorEventQueue, &event, 1) > 0)
                    {
                    }
                }
            }
        }

        void SetEngine(struct Engine* eng)
        {
            m_engine = eng;
        }

        void Zoom(TCGint threadId, int &x, int &y, int &w, int &h)
        {
            if (m_zoom[threadId])
            {
                int width = ANativeWindow_getWidth(m_windowHandle[threadId]);
                int height = ANativeWindow_getHeight(m_windowHandle[threadId]);
                if (height < (h+y) || width < (w+x))
                {
                    float ratioX = (float)(w+x) / width;
                    float ratioY = (float)(h+y) / height;
                    m_ratio[threadId] = (ratioX > ratioY) ? ratioX: ratioY;
                }

                if (m_ratio[threadId] != 1.0)
                {
                    x = (int)(x / m_ratio[threadId]);
                    y = (int)(y / m_ratio[threadId]);
                    w = (int)(w / m_ratio[threadId]);
                    h = (int)(h / m_ratio[threadId]);
                }
            }
        }

        void SetZoom(TCGint threadId, bool z)
        {
            m_zoom[threadId] = z;
        }

    private:
        BasicWindow(BasicWindow&);
        BasicWindow& operator=(BasicWindow&);

        Controller&                 m_controller;
        EGLDisplay*                 m_display;
        NativeWindowType            m_windowHandle[RENDER_THREAD_NUM];
        int                         m_width[RENDER_THREAD_NUM];
        int                         m_height[RENDER_THREAD_NUM];
        struct Engine*              m_engine;
        int                         m_ident;
        int                         m_events;
        struct android_poll_source* m_source;
        float                       m_ratio[RENDER_THREAD_NUM];
        bool                        m_zoom[RENDER_THREAD_NUM];
    };

} // app

#endif // TCG_ANDROID_WINDOW_H
