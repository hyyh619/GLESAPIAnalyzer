#ifndef __API_DEBUG_H__
#define __API_DEBUG_H__

#ifndef API_DUMP_ENABLE
#define API_DUMP_ENABLE                 0
#endif

#ifndef API_DUMP_DRAW_INFO_START
#define API_DUMP_DRAW_INFO_START        4973
#endif

#ifndef API_DUMP_DRAW_INFO_END
#define API_DUMP_DRAW_INFO_END          5000
#endif

#ifndef API_WRITE_DRAW_BIN
#define API_WRITE_DRAW_BIN              1
#endif

#ifndef API_WRITE_INDEX_BUFFER
#define API_WRITE_INDEX_BUFFER          0
#endif

#ifndef API_WRITE_DRAW_VBO_BUFFER
#define API_WRITE_DRAW_VBO_BUFFER       0
#endif

#ifndef API_DUMP_SINGLE_DRAW_INFO
#define API_DUMP_SINGLE_DRAW_INFO       1
#endif

#ifndef API_DUMP_TIME_INFO_ONLY
#define API_DUMP_TIME_INFO_ONLY         0
#endif

#ifdef WIN32
#define API_DUMP_OUTPUT_PATH            "./"
#elif defined(ANDROID)
#define API_DUMP_OUTPUT_PATH            "/sdcard/tcg/"
#elif defined(__APPLE__)
#define API_DUMP_OUTPUT_PATH            "/Users/yinghuang/development/hy_code/tencent/API_CL1117/tcg_client/apple_project/"
#else
#define API_DUMP_OUTPUT_PATH            ""
#endif


#endif /* __API_DEBUG_H__ */