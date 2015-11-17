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


#include <stdio.h>
#include <png.h> 
#include <stdlib.h>
#include "PngInterface.h"

/**********************************************************************/
int write_png_file(const char *file_name , pic_data *graph)
{
    int         j;
    int         i;
    int         temp;
    int         pos;
    png_byte    color_type;

    png_structp png_ptr;
    png_infop   info_ptr;
    png_bytep   *row_pointers;

    /* create file */
    FILE *fp = fopen(file_name, "wb");
    if (!fp)
    {
        printf("[write_png_file] File %s could not be opened for writing", file_name);
        return -1;
    }

    /* initialize stuff */
    png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr)
    {
        printf("[write_png_file] png_create_write_struct failed");
        return -1;
    }

    info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr)
    {
        printf("[write_png_file] png_create_info_struct failed");
        return -1;
    }

    if (setjmp(png_jmpbuf(png_ptr)))
    {
        printf("[write_png_file] Error during init_io");
        return -1;
    }

    png_init_io(png_ptr, fp);

    /* write header */
    if (setjmp(png_jmpbuf(png_ptr)))
    {
        printf("[write_png_file] Error during writing header");
        return -1;
    }

    if(graph->flag == HAVE_ALPHA)
    {
        color_type = PNG_COLOR_TYPE_RGB_ALPHA;
    }
    else
    {
        color_type = PNG_COLOR_TYPE_RGB;
    }

    png_set_IHDR(png_ptr, info_ptr, graph->width, graph->height,
                 graph->bit_depth, color_type, PNG_INTERLACE_NONE,
                 PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);

    png_write_info(png_ptr, info_ptr);

    /* write bytes */
    if (setjmp(png_jmpbuf(png_ptr)))
    {
        printf("[write_png_file] Error during writing bytes");
        return -1;
    }

    if(graph->flag == HAVE_ALPHA)
    {
        temp = (4 * graph->width);
    }
    else
    {
        temp = (3 * graph->width);
    }

    pos = 0;
    row_pointers = (png_bytep*)malloc(graph->height*sizeof(png_bytep));
    for (i = graph->height - 1; i >= 0; i--)
    {
        row_pointers[i] = (png_bytep)malloc(sizeof(unsigned char)*temp);
        for(j = 0; j < temp; j += 4)
        {
            row_pointers[i][j]  = graph->rgba[0][pos]; // red
            row_pointers[i][j+1] = graph->rgba[1][pos]; // green
            row_pointers[i][j+2] = graph->rgba[2][pos];  // blue
            if(graph->flag == HAVE_ALPHA)
            {
                row_pointers[i][j+3] = graph->rgba[3][pos]; // alpha
            }

            ++pos;
        }
    }

    png_write_image(png_ptr, row_pointers);

    /* end write */
    if (setjmp(png_jmpbuf(png_ptr)))
    {
        printf("[write_png_file] Error during end of write");
        return -1;
    }

    png_write_end(png_ptr, NULL);

    /* cleanup heap allocation */
    for (j=0; j<graph->height; j++)
    {
        free(row_pointers[j]);
    }
    free(row_pointers);

    png_destroy_write_struct(&png_ptr, &info_ptr);

    fclose(fp);
    return 0;
}