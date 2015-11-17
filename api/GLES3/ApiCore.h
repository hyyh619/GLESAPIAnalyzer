#ifndef __API_CORE_H
#define __API_CORE_H

#define TARGET_CAPS_MAX_WIDTH               2048
#define TARGET_CAPS_MAX_HEIGHT              2048
#define TARGET_CAPS_MULTI_TARGET_COUNT      1
#define TARGET_CAPS_MAX_SAMPLES             4

#define STREAM_CAPS_MAX_ATTRIBUTES          10
#define STREAM_CAPS_MAX_STREAM_SIZE         256
#define STREAM_CAPS_MAX_NUM_OF_STREAMS      1

#define TEXTURE_CAPS_MAX_WIDTH              2048
#define TEXTURE_CAPS_MAX_HEIGHT             2048
#define TEXTURE_CAPS_MAX_DEPTH              1
#define TEXTURE_CAPS_CUBIC                  1
#define TEXTURE_CAPS_NPOT                   0
#define TEXTURE_CAPS_VERTEX_SAMPLIERS       8
#define TEXTURE_CAPS_PIXEL_SAMPLIERS        8

#define SHADER_CAPS_VERTEX_UNIFORMS         160
#define SHADER_CAPS_FRAGMENT_UNIFORMS       64
#define SHADER_CAPS_VARYINGS                12

#define DRAW_BUFFERS                        S1


#endif /* __API_CORE_H */