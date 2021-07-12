#include "complex.h"

/*
this simplistic implementation was preferred
as opposed to using the C99 complex number because
passing a C99 complex number as a function argument
is not trivial and the performance gain is not extreme
*/

complex complex_square(complex z)
{
    complex res = {z.re*z.re - z.im*z.im, 2*z.re*z.im};
    return res;
}

complex complex_sum(complex z1, complex z2)
{
    complex res = {z1.re + z2.re, z1.im + z2.im};
    return res;
}