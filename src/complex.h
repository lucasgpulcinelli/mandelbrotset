#ifndef __COMPLEX_H__
#define __COMPLEX_H__
typedef struct
{
    long double re;
    long double im;
} complex;

complex complex_square(complex z);
complex complex_sum(complex z1, complex z2);
#endif