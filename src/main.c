#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "complex.h"
#include "mandelbrot.h"

#define TRUE  1
#define FALSE 0


/*
remaps the maximum and minimum value for the mandelbrot set, enabling zooming in
the possible sections are explained in the end of help()
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

void help(void)
{
    printf(
    "\n\
command line arguments:\n\
    \n\
    width: indicates the pixel width of all produces images\n\
    \n\
    height: indicates the pixel height of all produced images\n\
    \n\
    file: base filename for the images, can be .png or .jpeg\n\
    (for instance: a file argument of \"this.png\" would create the\n\
    series of files starting from this000.png, then this001.png and so on)\n\
    \n\
    steps_for_inifiny: the amount of steps in the mandelbrot function\n\
    before an input is considered divergent\
    \n\
    \n\
runtime arguments:\n\
    \n\
a number between zero and nine indicates a zoom in a numpad-like grid\
    \n\
the word \"steps\" followed by a space and a number sets\n\
steps_for_infinity to that number, userful when very deep in the mandelbrot set\n\
    \n\
    \n\
representation of the zoom grid:\n\
    \n\
    \n\
              -         \n\
              -         \n\
        1     -    3    \n\
              -         \n\
              -         \n\
    --------------------\n\
              -         \n\
              -         \n\
        7     -    9    \n\
              -         \n\
              -         \n\
    \n\
                        \n\
        ------------    \n\
        -          -    \n\
        -          -    \n\
        -          -    \n\
        -     5    -    \n\
        -          -    \n\
        -          -    \n\
        ------------    \n\
                        \n\
    \n\
        -          -    \n\
        -          -    \n\
        -     2    -    \n\
        -          -    \n\
        ------------    \n\
        ------------    \n\
        -          -    \n\
        -     8    -    \n\
        -          -    \n\
        -          -    \n\
    \n\
                        \n\
    --------------------\n\
             --         \n\
             --         \n\
             --         \n\
        4    --     6   \n\
             --         \n\
             --         \n\
    --------------------\n\
                        \n\
    \n\
    ");
}

int main(int argc, char** argv)
{
    if(argc == 2 && strcmp("--help", argv[1]) == 0)
    {
        help();
        exit(0);
    }
    if(argc < 5)
    {
        fprintf(stderr, "usage: \"%s <width> <height> <file> <steps_for_infinity>\"\n", argv[0]);
        fprintf(stderr, "use \"%s --help\" for a more details\n", argv[0]);
        exit(-1);
    }

    int width  = atoi(argv[1]);
    int height = atoi(argv[2]);
    int steps_for_inf = atoi(argv[4]);

    //this file variable will be the real filename for the file that will be written
    //as it will have three numbers following it, the +3 is necessary 
    char* file = malloc(sizeof(char) * (strlen(argv[3]) + 1 + 3));
    strcpy(file, argv[3]);

    //for the trailing numbers we need no find the . in the filename
    //and make a copy of the end to preserve the extension
    char* i_of_dot = strchr(file, '.');
    char* copy_of_end = malloc(sizeof(char) * (strlen(i_of_dot) + 1));
    strcpy(copy_of_end, i_of_dot);

    strcpy(i_of_dot, "000");
    i_of_dot+=3;
    strcat(file, copy_of_end);


    //the whole interesting part of the mandelbrot set is here
    complex mandmin = {-2, -1};
    complex mandmax = { 1,  1};

    int iteration = 0;
    while(TRUE)
    {
        //create the mandelbrot set file for this step
        int ret = makemandel(file, height, width, mandmin, mandmax, steps_for_inf);
        if(ret)
        {
            fprintf(stderr, "Error in makemandel function, code %d\n", ret);
            exit(-3);
        }


        //change the max and min for zooming
        int section;
        if(scanf("%d", &section) != 1)
        {
            //or the steps for infinity
            if(scanf("steps %d", &steps_for_inf) != 1)
            {
                break;
            }
            section = 0;
        }
        if(section < 0 && section > 9)
        {
            fprintf(stderr, "%d Not valid, use a number between 0 and 9 for the section to zoom\n", section);
            fprintf(stderr, "or \"steps <a_number>\" to indicate how many steps to use in mandelbrot\n");
            continue;
        }
        remap_mands(&mandmin, &mandmax, section);


        //increment the number in the filename
        snprintf(i_of_dot-3, strlen(i_of_dot-3)+1, "%03d%s", ++iteration, copy_of_end);
    }

    free(file);
    free(copy_of_end);

    return 0;
}