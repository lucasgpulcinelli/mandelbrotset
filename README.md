# Mandelbrot set implementation

## A simple implementation in C and python of a method for seeing and zooming in the mandelbrot set

The mandelbrot set is a set of all complex numbers C that converge with the repeated application of the function Z_n = (Z_n-1)^2 + C, yeilding a unique self repeating pattern
with interesting properties. As such there is an interest for making a program with the objective of understanding the set and learning about generalized image manipulation.

## Build instructions

### packages needed
libjpeg-lib libpng-dev make gcc (or another C compiler toolchain) and python3

### instructions
use "make" to build the project and, after changing the directory to build use "./main.py" to run the program

## Showcase

A simple exploratory run (after using imagemagik) created this gif:

<img src="./showcase.gif" width="375">

(it is a bit choppy as a smooth animation is not yet possible)

## TODO

1) As showed above, a smooth zooming (that does not take a long time and with a good resolution) is not possible

## Credits

This program was created by Lucas Eduardo Gulka Pulcinelli, with the help of [Arthur Grillo Queiroz Cabral](https://github.com/grillo-0) in some theoretical aspects of the mandelbrot set.
