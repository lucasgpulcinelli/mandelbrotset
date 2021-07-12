#ifndef __IMAGE_MANUPULATION_H__
#define __IMAGE_MANUPULATION_H__

#include <png.h>
#include <jpeglib.h>

#define IMAGE_IS_JPEG 0
#define IMAGE_IS_PNG  1

typedef struct
{
    int type;
    png_structp pngptr;
    png_infop infoptr;
    struct jpeg_compress_struct* cinfoptr;
    FILE* fptr;
} image;

image* init_image(char* file, int width, int height);
void write_image(image* imptr, unsigned char** buf, int lines);
void destroy_image(image* imptr);


#endif