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


#ifndef __PNG_INTERFACE_H__
#define __PNG_INTERFACE_H__


typedef struct _pic_data pic_data;
struct _pic_data
{
 int width, height;
 int bit_depth; 
 int flag;  
 unsigned char **rgba;
};

#define PNG_BYTES_TO_CHECK 4
#define HAVE_ALPHA 1
#define NO_ALPHA     0

int write_png_file(const char *file_name , pic_data *graph);

#endif
