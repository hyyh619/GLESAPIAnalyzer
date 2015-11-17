#ifndef __GLES_GLOBAL_H
#define __GLES_GLOBAL_H

#include <map>
#include <vector>
#include <GLES3/gl31.h>
#include "types.h"

#define API_VERTEX_ATTRIBUTES_NUM   32
#define API_TEX_UNIT_NUM            16
#define API_TEXTURE_TYPE_NUM        4    /*GL_TEXTURE_2D, GL_TEXTURE_CUBE_MAP, GL_TEXTURE_EXTERNAL, GL_TEXTURE_3D*/

GLuint  GetDataTypeSize(GLenum type);
GLuint  GetPixelSizeByTexFormat(GLenum format, GLenum type);
GLuint  GetTexImageSize(GLenum format, GLenum type, GLsizei width, GLsizei height, GLsizei depth);

#ifndef API_DUMP
typedef enum _TRNameId
{
    TR_NAME_Unknown = 0,

    // class name id
    TR_NAME_EGLConfig = 1,
    TR_NAME_EGLNativeWindowType,
    TR_NAME_EGLSurface,
    TR_NAME_EGLNativeDisplayType,
    TR_NAME_EGLBuffer,
    TR_NAME_EGLDisplay,
    TR_NAME_EGLNativePixmapType,
    TR_NAME_EGLContext,
    TR_NAME_EGLImage,
    TR_NAME_EGLClientBuffer,
    TR_NAME_EGLSync,

    // egl
    TR_NAME_display,
    TR_NAME_dpy,
    TR_NAME_major,
    TR_NAME_minor,
    TR_NAME_configs,
    TR_NAME_config_size,
    TR_NAME_num_config,
    TR_NAME_attrib_list,
    TR_NAME_config,
    TR_NAME_attribute,
    TR_NAME_win,
    TR_NAME_pixmap,
    TR_NAME_surface,
    TR_NAME_api,
    TR_NAME_buftype,
    TR_NAME_interval,
    TR_NAME_share_context,
    TR_NAME_ctx,
    TR_NAME_draw,
    TR_NAME_read,
    TR_NAME_readdraw,
    TR_NAME_engine,
    TR_NAME_image,
    TR_NAME_PatchID,
    TR_NAME_Set,

    // name id
    TR_NAME_Error,
    TR_NAME_func,
    TR_NAME_ref,

    TR_NAME_red,
    TR_NAME_green,
    TR_NAME_blue,
    TR_NAME_alpha,

    TR_NAME_depth,

    TR_NAME_config_id,
    TR_NAME_buffer_size,
    TR_NAME_red_size,
    TR_NAME_green_size,
    TR_NAME_blue_size,
    TR_NAME_alpha_size,
    TR_NAME_bind_to_texture_rgb,
    TR_NAME_bind_to_texture_rgba,
    TR_NAME_config_caveat,
    TR_NAME_depth_size,
    TR_NAME_max_swap_interval,
    TR_NAME_min_swap_interval,
    TR_NAME_native_renderable,
    TR_NAME_sample_buffers,
    TR_NAME_samples,
    TR_NAME_stencil_size,
    TR_NAME_surface_type,
    TR_NAME_transparent_type,
    TR_NAME_transparent_red,
    TR_NAME_transparent_green,
    TR_NAME_transparent_blue,
    TR_NAME_luminance_size,
    TR_NAME_alpha_mask_size,
    TR_NAME_color_buffer_type,
    TR_NAME_conformant,
    TR_NAME_renderable_type,

    TR_NAME_zNear,
    TR_NAME_zFar,
    TR_NAME_left,
    TR_NAME_right,
    TR_NAME_bottom,
    TR_NAME_top,

    TR_NAME_x,
    TR_NAME_y,
    TR_NAME_z,
    TR_NAME_w,

    TR_NAME_s,
    TR_NAME_t,
    TR_NAME_r,
    TR_NAME_q,

    TR_NAME_nx,
    TR_NAME_ny,
    TR_NAME_nz,
    TR_NAME_nw,

    TR_NAME_width,
    TR_NAME_height,

    TR_NAME_level,
    TR_NAME_internalformat,
    TR_NAME_format,
    TR_NAME_border,
    TR_NAME_imageSize,
    TR_NAME_data,
    TR_NAME_texture,
    TR_NAME_textures,
    TR_NAME_xoffset,
    TR_NAME_yoffset,
    TR_NAME_zoffset,

    TR_NAME_pname,
    TR_NAME_param,
    TR_NAME_params,

    TR_NAME_eqn,

    TR_NAME_light,
    TR_NAME_face,
    TR_NAME_target,
    TR_NAME_env,
    TR_NAME_size,
    TR_NAME_angle,
    TR_NAME_m,
    TR_NAME_attribindex,

    TR_NAME_factor,
    TR_NAME_units,

    TR_NAME_buffer,
    TR_NAME_buffers,
    TR_NAME_offset,
    TR_NAME_usage,
    TR_NAME_bindingindex,

    TR_NAME_mask,
    TR_NAME_maskNumber,
    TR_NAME_type,
    TR_NAME_stride,
    TR_NAME_pointer,
    TR_NAME_n,
    TR_NAME_plane,
    TR_NAME_equation,
    TR_NAME_flag,
    TR_NAME_cap,
    TR_NAME_array,
    TR_NAME_name,
    TR_NAME_opcode,
    TR_NAME_pixels,
    TR_NAME_sfactor,
    TR_NAME_dfactor,
    TR_NAME_value,
    TR_NAME_invert,
    TR_NAME_fail,
    TR_NAME_zfail,
    TR_NAME_zpass,
    TR_NAME_access,
    TR_NAME_matrixpaletteindex,
    TR_NAME_modeRGB,
    TR_NAME_modeAlpha,
    TR_NAME_srcRGB,
    TR_NAME_dstRGB,
    TR_NAME_srcAlpha,
    TR_NAME_dstAlpha,
    TR_NAME_coords,
    TR_NAME_primcount,

    TR_NAME_renderbuffer,
    TR_NAME_renderbuffers,
    TR_NAME_framebuffer,
    TR_NAME_framebuffers,
    TR_NAME_attachment,
    TR_NAME_renderbuffertarget,
    TR_NAME_textarget,
    TR_NAME_bufSize,
    TR_NAME_length,

    TR_NAME_mode,
    TR_NAME_count,
    TR_NAME_first,
    TR_NAME_indices,
    TR_NAME_start,
    TR_NAME_end,
    TR_NAME_instanceCount,

    TR_NAME_src,
    TR_NAME_str,

    TR_NAME_program,
    TR_NAME_shader,
    TR_NAME_index,
    TR_NAME_location,
    TR_NAME_shaders,
    TR_NAME_binaryformat,
    TR_NAME_binary,
    TR_NAME_string,
    TR_NAME_maxcount,
    TR_NAME_bufsize,
    TR_NAME_infolog,
    TR_NAME_shadertype,
    TR_NAME_precisiontype,
    TR_NAME_range,
    TR_NAME_precision,
    TR_NAME_source,
    TR_NAME_v,
    TR_NAME_transpose,
    TR_NAME_values,
    TR_NAME_normalized,
    TR_NAME_v0,
    TR_NAME_v1,
    TR_NAME_v2,
    TR_NAME_v3,
    TR_NAME_uniformCount,
    TR_NAME_uniformNames,
    TR_NAME_uniformIndices,
    TR_NAME_uniformBlockName,
    TR_NAME_uniformBlockIndex,
    TR_NAME_uniformBlockBinding,
    TR_NAME_programInterface,
    TR_NAME_propCount,
    TR_NAME_props,
    TR_NAME_pipeline,
    TR_NAME_stages,
    TR_NAME_strings,
    TR_NAME_pipelines,
    TR_NAME_relativeoffset,
    TR_NAME_glShaderSourceLine,
    TR_NAME_glStringArray,

    TR_NAME_val,
    TR_NAME_fixedsamplelocations,
    TR_NAME_barriers,
    TR_NAME_layered,
    TR_NAME_arrays,
    TR_NAME_indirect,
    TR_NAME_num_groups_x,
    TR_NAME_num_groups_y,
    TR_NAME_num_groups_z,
    TR_NAME_num_groups_w,
    TR_NAME_logical,
    TR_NAME_physical,
    TR_NAME_tiled,
    TR_NAME_levels,
    TR_NAME_numAttachments,
    TR_NAME_attachments,
    TR_NAME_divisor,
    TR_NAME_unit,
    TR_NAME_sampler,
    TR_NAME_samplers,
    TR_NAME_timeout,
    TR_NAME_sync,
    TR_NAME_flags,
    TR_NAME_condition,
    TR_NAME_ids,
    TR_NAME_id,
    TR_NAME_bufs,
    TR_NAME_srcX0,
    TR_NAME_srcY0,
    TR_NAME_srcX1,
    TR_NAME_srcY1,
    TR_NAME_dstX0,
    TR_NAME_dstY0,
    TR_NAME_dstX1,
    TR_NAME_dstY1,
    TR_NAME_filter,
    TR_NAME_layer,
    TR_NAME_primitiveMode,
    TR_NAME_varyings,
    TR_NAME_bufferMode,
    TR_NAME_drawbuffer,
    TR_NAME_stencil,
    TR_NAME_readTarget,
    TR_NAME_writeTarget,
    TR_NAME_readOffset,
    TR_NAME_writeOffset,

    // player only name
    TR_NAME_pixelformat,
    TR_NAME_nativedata,

    TR_NAME_MyEGLConfig0,
    TR_NAME_MyEGLConfig1,
    TR_NAME_MyEGLConfig2,
    TR_NAME_MyEGLConfig3,
    TR_NAME_MyEGLConfig4,
    TR_NAME_MyEGLConfig5,
    TR_NAME_MyEGLConfig6,
    TR_NAME_MyEGLConfig7,
    TR_NAME_MyEGLConfig8,
    TR_NAME_MyEGLConfig9,
    TR_NAME_MyEGLConfig10,
    TR_NAME_MyEGLConfig11,
    TR_NAME_MyEGLConfig12,
    TR_NAME_MyEGLConfig13,
    TR_NAME_MyEGLConfig14,
    TR_NAME_MyEGLConfig15,
    TR_NAME_MyEGLConfig16,
    TR_NAME_MyEGLConfig17,
    TR_NAME_MyEGLConfig18,
    TR_NAME_MyEGLConfig19,
    TR_NAME_MyEGLConfig20,
    TR_NAME_MyEGLConfig21,
    TR_NAME_MyEGLConfig22,
    TR_NAME_MyEGLConfig23,
    TR_NAME_MyEGLConfig24,
    TR_NAME_MyEGLConfig25,
    TR_NAME_MyEGLConfig26,
    TR_NAME_MyEGLConfig27,
    TR_NAME_MyEGLConfig28,
    TR_NAME_MyEGLConfig29,
    TR_NAME_MyEGLConfig30,
    TR_NAME_MyEGLConfig31,
    TR_NAME_MyEGLConfig32,
    TR_NAME_MyEGLConfig33,
    TR_NAME_MyEGLConfig34,
    TR_NAME_MyEGLConfig35,
    TR_NAME_MyEGLConfig36,
    TR_NAME_MyEGLConfig37,
    TR_NAME_MyEGLConfig38,
    TR_NAME_MyEGLConfig39,
    TR_NAME_MyEGLConfig40,
    TR_NAME_MyEGLConfig41,
    TR_NAME_MyEGLConfig42,
    TR_NAME_MyEGLConfig43,
    TR_NAME_MyEGLConfig44,
    TR_NAME_MyEGLConfig_default,
    TR_NAME_MyWindow,
    TR_NAME_eglGetDisplay,
    TR_NAME_AndroidNativeBuffer,
    TR_NAME_eglCreateWindowSurface,
    TR_NAME_eglCreateContext,

    // Android only
    //TR_NAME_nativedata,
    TR_NAME_native_type,
    TR_NAME_native_handle,
    TR_NAME_updatednativedata,

    TR_NAME_MAX
} TRNameId;

union TRvalue
{
    GLbyte  b;
    GLshort s;
    GLint   i;
    long    l;
    GLfloat f;
    double  d;
    GLuint  h;
};

struct stValue
{
    TRNameId        nameid;
    TRNameId        classnameid;
    GLint           type;
    GLint           classid;
    TRvalue         val;
    GLESAPIIndex    funcName;

    stValue():  //default construct function
        nameid(TR_NAME_Unknown),
        classnameid(TR_NAME_Unknown),
        type(0),
        classid(0),
        funcName(GLES_FUNCTION_TOTAL_NUMBER)
    {
        val.l = 0;
    }
};

struct stEvent
{
    GLESAPIIndex    name;
    stValue         args[11];
    GLuint          argNum;
    GLint           api;

    stEvent():
        name(GLES_FUNCTION_TOTAL_NUMBER),
        argNum(0)
    {
    }
};
#endif

#endif /* __GLES_GLOBAL_H */