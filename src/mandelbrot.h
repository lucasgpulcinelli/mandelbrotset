#ifndef __MANDELBROT_H__
#define __MANDELBROT_H__
#include "complex.h"

int makemandel(char* file,
            int imheight, int imwidth,
            complex mandmin, complex mandmax,
            int steps_for_inf);

void remap_mands(complex* mandmin, complex* mandmax, int section);

#endif