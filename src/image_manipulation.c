#include <stdio.h>
#include <stdlib.h>
#include <png.h>
#include <jpeglib.h>
#include <string.h>
#include "image_manipulation.h"

/*
simple function implementations for high level usage of
8 bit depth 3 channels PNG and JPEG files with set arguments
*/

//PNG specific functions
int init_png(FILE* fptr, int height, int width, png_structp* pngptr, png_infop* infoptr)
{

    *pngptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (*pngptr == NULL)
    {
        return -1;
    }

    *infoptr = png_create_info_struct(*pngptr);
    if (*infoptr == NULL)
    {
        return -2;
    }

    if(setjmp(png_jmpbuf(*pngptr)))
    {
        png_destroy_write_struct (pngptr, infoptr);
        return -3;
    }

    png_set_IHDR(*pngptr, *infoptr, width, height, 8,
                PNG_COLOR_TYPE_RGB, PNG_INTERLACE_NONE,
                PNG_COMPRESSION_TYPE_DEFAULT,PNG_FILTER_TYPE_DEFAULT);

    png_init_io(*pngptr, fptr);
    png_set_invert_alpha(*pngptr);

    png_write_info(*pngptr, *infoptr);
    return 0;
}

void destroy_png(png_structp pngptr, png_infop infoptr)
{
    png_write_end(pngptr, infoptr);
    png_destroy_write_struct (&pngptr, &infoptr);
}

//jpeg specific functions
int init_jpeg(FILE* fptr, struct jpeg_compress_struct** cinfo, struct jpeg_error_mgr* jerr, int height, int width)
{
    *cinfo = malloc(sizeof(struct jpeg_compress_struct));
    if(*cinfo == NULL)
    {
        return -1;
    }

    jpeg_create_compress(*cinfo);
    (*cinfo)->err = jpeg_std_error(jerr);

    jpeg_stdio_dest(*cinfo, fptr);


    (*cinfo)->image_width      = width;
    (*cinfo)->image_height     = height;
    (*cinfo)->input_components = 3;
    (*cinfo)->in_color_space   = JCS_RGB;
    jpeg_set_defaults(*cinfo);

    jpeg_start_compress(*cinfo, TRUE);
    return 0;
}

void destroy_jpeg(struct jpeg_compress_struct* cinfo)
{
    jpeg_finish_compress(cinfo);
    jpeg_destroy_compress(cinfo);
    free(cinfo);
}

//generalized (exported) image functions

/*
note: this function discovers the file format based on the extension
defaulting for jpeg
*/
image* init_image(char* file, int width, int height)
{
    FILE* fptr = fopen(file, "wb");
    if(fptr == NULL)
    {
        return NULL;
    }

    image* imptr = malloc(sizeof(image));
    if(imptr == NULL)
    {
        fclose(fptr);
        return NULL;
    }

    imptr->fptr = fptr;
    if((strcmp(file+strlen(file)-3, "png") == 0))
    {
        imptr->type     = IMAGE_IS_PNG;
        imptr->cinfoptr = NULL;

        if(init_png(fptr, height, width, &(imptr->pngptr), &(imptr->infoptr)))
        {
            free(imptr);
            fclose(fptr);
            return NULL;
        }
    }
    else
    {
        imptr->type    = IMAGE_IS_JPEG;
        imptr->pngptr  = NULL;
        imptr->infoptr = NULL;

        struct jpeg_error_mgr jerr;
        if(init_jpeg(fptr, &(imptr->cinfoptr), &jerr, height, width))
        {
            free(imptr);
            fclose(fptr);
            return NULL;
        }
    }

    return imptr;
}

void write_image(image* imptr, unsigned char** buf, int lines)
{
    if(imptr->type == IMAGE_IS_JPEG)
    {
        jpeg_write_scanlines(imptr->cinfoptr, buf, lines);
    }
    else if(imptr->type == IMAGE_IS_PNG)
    {
        png_write_rows(imptr->pngptr, buf, lines);
    }
    return;
}

void destroy_image(image* imptr)
{
    if(imptr->type == IMAGE_IS_JPEG)
    {
        destroy_jpeg(imptr->cinfoptr);
    }
    else if(imptr->type == IMAGE_IS_PNG)
    {
        destroy_png(imptr->pngptr, imptr->infoptr);
    }

    fclose(imptr->fptr);
    free(imptr);
    return;
}