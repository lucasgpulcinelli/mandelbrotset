#include <stdio.h>
#include <stdlib.h>
#include <jpeglib.h>
#include <string.h>
#include "complex.h"
#include "mandelbrot.h"
#include "image_manipulation.h"

/*
thinking about a future multi threaded implementation for small video making
the below definition seemed reasonable for each thread,
as an 8 thread computer with 8 Gb of RAM could handle about
a one million pixel width image, a reasonable amount
*/  
#define SCANS_PER_WRITE 256


/*
remaps the maximum and minimum value for the mandelbrot set, enabling zooming in
the possible sections are in a numpad-like grid, slightly change to make the property below
note: this function always divide the area between mandmin and mandmax by four,
always preserving the aspect ratio between them
*/
void remap_mands(complex* mandmin, complex* mandmax, int section)
{   
    complex tmp = {mandmax->re - mandmin->re, mandmax->im - mandmin->im};

    switch(section)
    {
    case 0:
        break;
    case 1:
        mandmin->im = (mandmin->im + mandmax->im)/2;
        mandmax->re = (mandmin->re + mandmax->re)/2;
        break;
    case 2:
        mandmin->re+= 1*(tmp.re)/4;
        mandmin->im =   (mandmax->im + mandmin->im)/2;
        mandmax->re-= 1*(tmp.re)/4;
        break;
    case 3:
        mandmin->re = (mandmin->re + mandmax->re)/2;
        mandmin->im = (mandmin->im + mandmax->im)/2;
        break;
    case 4:
        mandmin->im+= 1*(tmp.im)/4;
        mandmax->re =   (mandmax->re + mandmin->re)/2;
        mandmax->im-= 1*(tmp.im)/4;
        break;
    case 5:
        mandmin->re+= 1*(tmp.re)/4;
        mandmin->im+= 1*(tmp.im)/4;
        mandmax->re-= 1*(tmp.re)/4;
        mandmax->im-= 1*(tmp.im)/4;
        break;
    case 6:
        mandmin->im+= 1*(tmp.im)/4;
        mandmin->re =   (mandmax->re + mandmin->re)/2;
        mandmax->im-= 1*(tmp.im)/4;
        break;
    case 7:
        mandmax->re = (mandmin->re + mandmax->re)/2;
        mandmax->im = (mandmin->im + mandmax->im)/2;
        break;
    case 8:
        mandmin->re+= 1*(tmp.re)/4;
        mandmax->im =   (mandmax->im + mandmin->im)/2;
        mandmax->re-= 1*(tmp.re)/4;
        break;
    case 9:
        mandmin->re = (mandmin->re + mandmax->re)/2;
        mandmax->im = (mandmin->im + mandmax->im)/2;
        break;
    }

    return;
}


//simple function to allocate a two dimensional buffer with NULL checking
unsigned char** alloc_charpp(int rows, int cols)
{
    unsigned char** ret = malloc(sizeof(unsigned char*) * rows);
    if(ret == NULL)
    {
        return NULL;
    }

    int i, got_null = 0;
    for(i = 0; i < rows; i++)
    {
        ret[i] = malloc(sizeof(unsigned char) * cols);
        if(ret[i] == NULL)
        {
            got_null = 1;
            break;
        }
    }

    if(got_null)
    {
        for(int j = 0; j < i; j++)
        {
            free(ret[j]);
        }
        free(ret);
        return NULL;
    }

    return ret;
}

void free_charpp(unsigned char** buf, int rows)
{
    for(int i = 0; i < rows; i++)
    {
        free(buf[i]);
    }
    free(buf);
    
    return;
}

//calculate the amount of steps for diversion for complex number c in the mandelbrot set
//unless it reaches steps_for_inf and did not diverge, in that case return 0, meaning it converges
int mandelbrot(complex c, int steps_for_inf)
{
    complex z = {0, 0};

    int i = 0;
    while(i <= steps_for_inf)
    {
        if(z.re*z.re + z.im*z.im >= 4)
        {
            return i;
        }
        z = complex_sum(complex_square(z), c);
        i++;
    }

    return 0;
}

/*
create the image file of height imheight and width imwidth of the mandelbrot set
between the complex numbers mandmin and mandmax,
considering that a number in the mandelbrot set converges after steps_for_inf steps 
*/
int makemandel(char* file,
            int imheight, int imwidth,
            complex mandmin, complex mandmax,
            int steps_for_inf)
{
    image* imptr = init_image(file, imwidth, imheight);
    if(imptr == NULL)
    {
        return -1;
    }

    unsigned char** buf = alloc_charpp(SCANS_PER_WRITE, imwidth*3);
    if(buf == NULL)
    {
        destroy_image(imptr);
        return -2;
    }

    long double r_slope = (mandmax.re - mandmin.re)/(imwidth-1);
    long double i_slope = (mandmin.im - mandmax.im)/(imheight-1);
    int scans = (imheight < SCANS_PER_WRITE)? imheight:SCANS_PER_WRITE;

    for(int line = 0; line < imheight; line+=scans)
    {
        for(int i = 0; i < scans; i++)
        {
            for(int pix = 0; pix < imwidth; pix++)
            {
                complex c = {(r_slope*pix+mandmin.re), ((i_slope*(i+line)+mandmax.im))};

                long double n = ((long double) mandelbrot(c, steps_for_inf))/steps_for_inf;

                buf[i][pix*3+0] = (int)(9*(1-n)*n*n*n*255);
                buf[i][pix*3+1] = (int)(15*(1-n)*(1-n)*n*n*255);
                buf[i][pix*3+2] = (int)(8.5*(1-n)*(1-n)*(1-n)*n*255);
            }
        }
        write_image(imptr, buf, scans);
    }


    free_charpp(buf, SCANS_PER_WRITE);
    destroy_image(imptr);
    return 0;
}

