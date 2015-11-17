#ifndef ENGINEOCL_H
#define ENGINEOCL_H


#ifdef __cplusplus
extern "C" {
#endif

#include <windows.h>


typedef signed   __int8         cl_char;
typedef unsigned __int8         cl_uchar;
typedef signed   __int16        cl_short;
typedef unsigned __int16        cl_ushort;
typedef signed   __int32        cl_int;
typedef unsigned __int32        cl_uint;
typedef signed   __int64        cl_long;
typedef unsigned __int64        cl_ulong;

typedef unsigned __int16        cl_half;
typedef float                   cl_float;
typedef double                  cl_double;

typedef struct _cl_platform_id *    cl_platform_id;
typedef struct _cl_device_id *      cl_device_id;
typedef struct _cl_context *        cl_context;
typedef struct _cl_command_queue *  cl_command_queue;
typedef struct _cl_mem *            cl_mem;
typedef struct _cl_program *        cl_program;
typedef struct _cl_kernel *         cl_kernel;
typedef struct _cl_event *          cl_event;
typedef struct _cl_sampler *        cl_sampler;

typedef cl_uint             cl_bool;
typedef cl_ulong            cl_bitfield;
typedef cl_bitfield         cl_device_type;
typedef cl_uint             cl_platform_info;
typedef cl_uint             cl_device_info;
typedef cl_bitfield         cl_device_fp_config;
typedef cl_uint             cl_device_mem_cache_type;
typedef cl_uint             cl_device_local_mem_type;
typedef cl_bitfield         cl_device_exec_capabilities;
typedef cl_bitfield         cl_command_queue_properties;

typedef intptr_t            cl_context_properties;
typedef cl_uint             cl_context_info;
typedef cl_uint             cl_command_queue_info;
typedef cl_uint             cl_channel_order;
typedef cl_uint             cl_channel_type;
typedef cl_bitfield         cl_mem_flags;
typedef cl_uint             cl_mem_object_type;
typedef cl_uint             cl_mem_info;
typedef cl_uint             cl_image_info;
typedef cl_uint             cl_buffer_create_type;
typedef cl_uint             cl_addressing_mode;
typedef cl_uint             cl_filter_mode;
typedef cl_uint             cl_sampler_info;
typedef cl_bitfield         cl_map_flags;
typedef cl_uint             cl_program_info;
typedef cl_uint             cl_program_build_info;
typedef cl_int              cl_build_status;
typedef cl_uint             cl_kernel_info;
typedef cl_uint             cl_kernel_work_group_info;
typedef cl_uint             cl_event_info;
typedef cl_uint             cl_command_type;
typedef cl_uint             cl_profiling_info;

typedef struct _cl_image_format cl_image_format;
typedef struct _cl_buffer_region cl_buffer_region;






/*
 * Native OpenCL Engine.
 */
typedef struct _EngineOCL EngineOCL;


/* Reference desktop OpenCL Engine. */
const EngineOCL *
ConstructOpenCLEngine(
	void
	);


/* De-reference desktop OpenCL Engine. */
void
DestroyOpenCLEngine(
	IN const EngineOCL * Eng
	);


struct _EngineOCL
{
    /* Platform API */
    cl_int (APIENTRY * clGetPlatformIDs) (cl_uint num_entries, cl_platform_id *platforms, cl_uint *num_platforms);
    
    cl_int (APIENTRY * clGetPlatformInfo) (
        cl_platform_id platform, 
        cl_platform_info param_name, 
        size_t param_value_size, 
        void *param_value, 
        size_t *param_value_size_ret);

    /* Device APIs */
    cl_int (APIENTRY * clGetDeviceIDs) (
        cl_platform_id platform, 
        cl_device_type device_type, 
        cl_uint num_entries, 
        cl_device_id *devices, 
        cl_uint *num_devices);
    
    cl_int (APIENTRY * clGetDeviceInfo) (
        cl_device_id device, 
        cl_device_info param_name, 
        size_t param_value_size, 
        void *param_value, 
        size_t *param_value_size_ret);

    /* Context APIs  */
    cl_context (APIENTRY * clCreateContext) (
        const cl_context_properties *properties, 
        cl_uint num_devices, 
        const cl_device_id *devices, 
        void (APIENTRY * pfn_notify)(const char *, const void *, size_t, void *), 
        void *user_data, 
        cl_int *errcode_ret);
    
    cl_context (APIENTRY * clCreateContextFromType) (
        const cl_context_properties *properties, 
        cl_device_type device_type, 
        void (APIENTRY * pfn_notify)(const char *, const void *, size_t, void *), 
        void *user_data, 
        cl_int *errcode_ret);
    
    cl_int (APIENTRY * clRetainContext) (cl_context context);
    cl_int (APIENTRY * clReleaseContext) (cl_context context);
    
    cl_int (APIENTRY * clGetContextInfo) (
        cl_context context, 
        cl_context_info param_name, 
        size_t param_value_size, 
        void *param_value, 
        size_t *param_value_size_ret);

    /* Command Queue APIs */
    cl_command_queue (APIENTRY * clCreateCommandQueue) (
        cl_context context, 
        cl_device_id device, 
        cl_command_queue_properties properties, 
        cl_int *errcode_ret);
    
    cl_int (APIENTRY * clRetainCommandQueue) (cl_command_queue command_queue);
    cl_int (APIENTRY * clReleaseCommandQueue) (cl_command_queue command_queue);
    
    cl_int (APIENTRY * clGetCommandQueueInfo) (
        cl_command_queue command_queue, 
        cl_command_queue_info param_name, 
        size_t param_value_size, 
        void *param_value, 
        size_t *param_value_size_ret);

    cl_int (APIENTRY * clSetCommandQueueProperty)(
        cl_command_queue               command_queue ,
        cl_command_queue_properties    properties ,
        cl_bool                         enable ,
        cl_command_queue_properties *  old_properties );

    /* Memory Object APIs */
    cl_mem (APIENTRY * clCreateBuffer) (cl_context context, cl_mem_flags flags, size_t size, void *host_ptr, cl_int *errcode_ret);
    
    cl_mem (APIENTRY * clCreateSubBuffer) (
        cl_mem buffer, 
        cl_mem_flags flags, 
        cl_buffer_create_type buffer_create_type, 
        const void *buffer_create_info, 
        cl_int *errcode_ret);
    
    cl_mem (APIENTRY * clCreateImage2D) (
        cl_context context, 
        cl_mem_flags flags, 
        const cl_image_format *image_format,
        size_t image_width, 
        size_t image_height, 
        size_t image_row_pitch, 
        void *host_ptr, 
        cl_int *errcode_ret);
    
    cl_mem (APIENTRY * clCreateImage3D) (
        cl_context context, 
        cl_mem_flags flags, 
        const cl_image_format *image_format, 
        size_t image_width, 
        size_t image_height, 
        size_t image_depth, 
        size_t image_row_pitch, 
        size_t image_slice_pitch, 
        void *host_ptr, 
        cl_int *errcode_ret);
    
    cl_int (APIENTRY * clRetainMemObject) (cl_mem memobj);
    cl_int (APIENTRY * clReleaseMemObject) (cl_mem memobj);
    
    cl_int (APIENTRY * clGetSupportedImageFormats) (
        cl_context context, 
        cl_mem_flags flags, 
        cl_mem_object_type image_type, 
        cl_uint num_entries, 
        cl_image_format *image_formats, 
        cl_uint *num_image_formats);
    
    cl_int (APIENTRY * clGetMemObjectInfo) (
        cl_mem memobj, 
        cl_mem_info param_name, 
        size_t param_value_size,
        void *param_value, 
        size_t *param_value_size_ret);
    
    cl_int (APIENTRY * clGetImageInfo) (
        cl_mem image, 
        cl_image_info param_name, 
        size_t param_value_size, 
        void *param_value, 
        size_t *param_value_size_ret);
    
    cl_int (APIENTRY * clSetMemObjectDestructorCallback) (
        cl_mem memobj, 
        void (APIENTRY *pfn_notify)( cl_mem memobj, void* user_data), 
        void *user_data);

    /* Sampler APIs  */
    cl_sampler (APIENTRY * clCreateSampler) (
        cl_context context, 
        cl_bool normalized_coords, 
        cl_addressing_mode addressing_mode, 
        cl_filter_mode filter_mode, 
        cl_int *errcode_ret);
    
    cl_int (APIENTRY * clRetainSampler) (cl_sampler sampler);
    cl_int (APIENTRY * clReleaseSampler) (cl_sampler sampler);
    
    cl_int (APIENTRY * clGetSamplerInfo) (
        cl_sampler sampler, 
        cl_sampler_info param_name, 
        size_t param_value_size, 
        void *param_value, 
        size_t *param_value_size_ret);

    /* Program Object APIs  */
    cl_program (APIENTRY * clCreateProgramWithSource) (
        cl_context context, 
        cl_uint count, 
        const char **strings, 
        const size_t *lengths, 
        cl_int *errcode_ret);
    
    cl_program (APIENTRY * clCreateProgramWithBinary) (
        cl_context context, 
        cl_uint num_devices, 
        const cl_device_id *device_list, 
        const size_t *lengths, 
        const unsigned char **binaries, 
        cl_int *binary_status, 
        cl_int *errcode_ret);
    
    cl_int (APIENTRY * clRetainProgram) (cl_program program);
    cl_int (APIENTRY * clReleaseProgram) (cl_program program);
    
    cl_int (APIENTRY * clBuildProgram) (
        cl_program program, 
        cl_uint num_devices, 
        const cl_device_id *device_list, 
        const char *options, 
        void (APIENTRY * pfn_notify)(cl_program program, void *user_data), 
        void *user_data);
    
    cl_int (APIENTRY * clUnloadCompiler) (void);
    
    cl_int (APIENTRY * clGetProgramInfo) (
        cl_program program, 
        cl_program_info param_name, 
        size_t param_value_size, 
        void *param_value, 
        size_t *param_value_size_ret);
    
    cl_int (APIENTRY * clGetProgramBuildInfo) (
        cl_program program, 
        cl_device_id device, 
        cl_program_build_info param_name, 
        size_t param_value_size, 
        void *param_value, 
        size_t *param_value_size_ret);

    /* Kernel Object APIs */
    cl_kernel (APIENTRY * clCreateKernel) (cl_program program, const char *kernel_name, cl_int *errcode_ret);
    cl_int (APIENTRY * clCreateKernelsInProgram) (cl_program program, cl_uint num_kernels, cl_kernel *kernels, cl_uint *num_kernels_ret);
    cl_int (APIENTRY * clRetainKernel) (cl_kernel kernel);
    cl_int (APIENTRY * clReleaseKernel) (cl_kernel kernel);
    cl_int (APIENTRY * clSetKernelArg) (cl_kernel kernel, cl_uint arg_index, size_t arg_size, const void *arg_value);
    
    cl_int (APIENTRY * clGetKernelInfo) (
        cl_kernel kernel, 
        cl_kernel_info param_name, 
        size_t param_value_size, 
        void *param_value, 
        size_t *param_value_size_ret);
    
    cl_int (APIENTRY * clGetKernelWorkGroupInfo) (
        cl_kernel kernel, 
        cl_device_id device, 
        cl_kernel_work_group_info param_name, 
        size_t param_value_size, 
        void *param_value, 
        size_t *param_value_size_ret);

    /* Event Object APIs  */
    cl_int (APIENTRY * clWaitForEvents) (cl_uint num_events, const cl_event *event_list);
    
    cl_int (APIENTRY * clGetEventInfo) (
        cl_event event, 
        cl_event_info param_name, 
        size_t param_value_size, 
        void *param_value, 
        size_t *param_value_size_ret);
    
    cl_event (APIENTRY * clCreateUserEvent) (cl_context context, cl_int *errcode_ret);
    cl_int (APIENTRY * clRetainEvent) (cl_event event);
    cl_int (APIENTRY * clReleaseEvent) (cl_event event);
    cl_int (APIENTRY * clSetUserEventStatus) (cl_event event, cl_int execution_status);
    
    cl_int (APIENTRY * clSetEventCallback) (
        cl_event event, 
        cl_int command_exec_callback_type, 
        void (APIENTRY * pfn_notify)(cl_event, cl_int, void *), 
        void *user_data);

    /* Profiling APIs  */
    cl_int (APIENTRY * clGetEventProfilingInfo) (
        cl_event event, 
        cl_profiling_info param_name, 
        size_t param_value_size, 
        void *param_value, 
        size_t *param_value_size_ret);

    /* Flush and Finish APIs */
    cl_int (APIENTRY * clFlush) (cl_command_queue command_queue);
    cl_int (APIENTRY * clFinish) (cl_command_queue command_queue);

    /* Enqueued Commands APIs */
    cl_int (APIENTRY * clEnqueueReadBuffer) (
        cl_command_queue command_queue, 
        cl_mem buffer, 
        cl_bool blocking_read, 
        size_t offset, 
        size_t cb, 
        void *ptr, 
        cl_uint num_events_in_wait_list, 
        const cl_event *event_wait_list, 
        cl_event *event);
    
    cl_int (APIENTRY * clEnqueueReadBufferRect) (
        cl_command_queue command_queue, 
        cl_mem buffer, 
        cl_bool blocking_read, 
        const size_t *buffer_origin, 
        const size_t *host_origin, 
        const size_t *region, 
        size_t buffer_row_pitch, 
        size_t buffer_slice_pitch,
        size_t host_row_pitch, 
        size_t host_slice_pitch, 
        void *ptr, 
        cl_uint num_events_in_wait_list, 
        const cl_event *event_wait_list, 
        cl_event *event);
    
    cl_int (APIENTRY * clEnqueueWriteBuffer) (
        cl_command_queue command_queue, 
        cl_mem buffer, 
        cl_bool blocking_write, 
        size_t offset, 
        size_t cb, 
        const void *ptr, 
        cl_uint num_events_in_wait_list, 
        const cl_event *event_wait_list, 
        cl_event *event);
    
    cl_int (APIENTRY * clEnqueueWriteBufferRect) (
        cl_command_queue command_queue, 
        cl_mem buffer, 
        cl_bool blocking_write, 
        const size_t *buffer_origin, 
        const size_t *host_origin, 
        const size_t *region, 
        size_t buffer_row_pitch, 
        size_t buffer_slice_pitch, 
        size_t host_row_pitch, 
        size_t host_slice_pitch, 
        const void *ptr, 
        cl_uint num_events_in_wait_list, 
        const cl_event *event_wait_list, 
        cl_event *event);
    
    cl_int (APIENTRY * clEnqueueCopyBuffer) (
        cl_command_queue command_queue, 
        cl_mem src_buffer, 
        cl_mem dst_buffer, 
        size_t src_offset, 
        size_t dst_offset, 
        size_t cb, 
        cl_uint num_events_in_wait_list, 
        const cl_event *event_wait_list, 
        cl_event *event);
    
    cl_int (APIENTRY * clEnqueueCopyBufferRect) (
        cl_command_queue command_queue, 
        cl_mem src_buffer, 
        cl_mem dst_buffer, 
        const size_t *src_origin, 
        const size_t *dst_origin, 
        const size_t *region, 
        size_t src_row_pitch, 
        size_t src_slice_pitch, 
        size_t dst_row_pitch, 
        size_t dst_slice_pitch, 
        cl_uint num_events_in_wait_list, 
        const cl_event *event_wait_list, 
        cl_event *event);

    cl_int (APIENTRY * clEnqueueReadImage) (
        cl_command_queue     command_queue,
        cl_mem               image,
        cl_bool              blocking_read,
        const size_t *       origin,
        const size_t *       region,
        size_t               row_pitch,
        size_t               slice_pitch,
        void *               ptr,
        cl_uint              num_events_in_wait_list,
        const cl_event *     event_wait_list,
        cl_event *           event);

    cl_int (APIENTRY * clEnqueueWriteImage)(
        cl_command_queue    command_queue,
        cl_mem              image,
        cl_bool             blocking_write,
        const size_t *      origin,
        const size_t *      region,
        size_t              input_row_pitch,
        size_t              input_slice_pitch,
        const void *        ptr,
        cl_uint             num_events_in_wait_list,
        const cl_event *    event_wait_list,
        cl_event *          event);

    cl_int (APIENTRY * clEnqueueCopyImage)(
        cl_command_queue     command_queue,
        cl_mem               src_image,
        cl_mem               dst_image,
        const size_t *       src_origin,
        const size_t *       dst_origin,
        const size_t *       region,
        cl_uint              num_events_in_wait_list,
        const cl_event *     event_wait_list,
        cl_event *           event);

    cl_int (APIENTRY * clEnqueueCopyImageToBuffer) (
        cl_command_queue command_queue,
        cl_mem           src_image,
        cl_mem           dst_buffer,
        const size_t *   src_origin,
        const size_t *   region,
        size_t           dst_offset,
        cl_uint          num_events_in_wait_list,
        const cl_event * event_wait_list,
        cl_event *       event);

    cl_int (APIENTRY * clEnqueueCopyBufferToImage) (
        cl_command_queue command_queue,
        cl_mem           src_buffer,
        cl_mem           dst_image,
        size_t           src_offset,
        const size_t *   dst_origin,
        const size_t *   region,
        cl_uint          num_events_in_wait_list,
        const cl_event * event_wait_list,
        cl_event *       event);

    void * (APIENTRY * clEnqueueMapBuffer) (
        cl_command_queue command_queue,
        cl_mem           buffer,
        cl_bool          blocking_map,
        cl_map_flags     map_flags,
        size_t           offset,
        size_t           cb,
        cl_uint          num_events_in_wait_list,
        const cl_event * event_wait_list,
        cl_event *       event,
        cl_int *         errcode_ret);

    void * (APIENTRY * clEnqueueMapImage) (
        cl_command_queue  command_queue,
        cl_mem            image,
        cl_bool           blocking_map,
        cl_map_flags      map_flags,
        const size_t *    origin,
        const size_t *    region,
        size_t *          image_row_pitch,
        size_t *          image_slice_pitch,
        cl_uint           num_events_in_wait_list,
        const cl_event *  event_wait_list,
        cl_event *        event,
        cl_int *          errcode_ret);

    cl_int (APIENTRY * clEnqueueUnmapMemObject) (
        cl_command_queue command_queue,
        cl_mem           memobj,
        void *           mapped_ptr,
        cl_uint          num_events_in_wait_list,
        const cl_event *  event_wait_list,
        cl_event *        event);

    cl_int (APIENTRY * clEnqueueNDRangeKernel) (
        cl_command_queue command_queue,
        cl_kernel        kernel,
        cl_uint          work_dim,
        const size_t *   global_work_offset,
        const size_t *   global_work_size,
        const size_t *   local_work_size,
        cl_uint          num_events_in_wait_list,
        const cl_event * event_wait_list,
        cl_event *       event);

    cl_int (APIENTRY * clEnqueueTask) (
        cl_command_queue  command_queue,
        cl_kernel         kernel,
        cl_uint           num_events_in_wait_list,
        const cl_event *  event_wait_list,
        cl_event *        event);

    cl_int (APIENTRY * clEnqueueNativeKernel) (
        cl_command_queue  command_queue,
        void (*user_func)(void *),
        void *            args,
        size_t            cb_args,
        cl_uint           num_mem_objects,
        const cl_mem *    mem_list,
        const void **     args_mem_loc,
        cl_uint           num_events_in_wait_list,
        const cl_event *  event_wait_list,
        cl_event *        event);

    cl_int (APIENTRY * clEnqueueMarker) (cl_command_queue command_queue, cl_event *event);

    cl_int (APIENTRY * clEnqueueWaitForEvents) (
        cl_command_queue command_queue,
        cl_uint          num_events,
        const cl_event * event_list);

    cl_int (APIENTRY * clEnqueueBarrier) (cl_command_queue command_queue);

    /* Extension function access
    *
    * Returns the extension function address for the given function name,
    * or NULL if a valid function can not be found.  The client must
    * check to make sure the address is not NULL, before using or
    * calling the returned function address.
    */
    void * (APIENTRY * clGetExtensionFunctionAddress) (const char * func_name);


};


#ifdef __cplusplus
}
#endif

#endif
