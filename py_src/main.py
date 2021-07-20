#!/usr/bin/env python3
import mandelbrot
import sys

def main_help():
    print(
    "\n\
command line arguments:\n\
    \n\
    width: indicates the pixel width of all produces images\n\
    \n\
    height: indicates the pixel height of all produced images\n\
    \n\
    directory: directory for the output images\n\
    \n\
    format: can be either png or jpeg\n\
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
    ")
    return

if len(sys.argv) == 2 and sys.argv[1] == "--help":
    main_help()
    sys.exit(0)
elif len(sys.argv) < 5:
    print("usage: \"{} <width> <height> <directory> <format> <steps_for_infinity>\"".format(sys.argv[0]),
    file=sys.stderr)
    sys.exit(-1)

width  = int(sys.argv[1])
height = int(sys.argv[2])
outdir = sys.argv[3]
fileformat = sys.argv[4]
steps_for_inf = int(sys.argv[5])

mandmin = -2-1j
mandmax =  1+1j
filenumber = 0

while True:
    ret = mandelbrot.makemandel(outdir + f"/{filenumber:04}." + fileformat, 
        height, width, mandmin, mandmax, steps_for_inf)
    if ret != 0:
        print(f"Error in makemandel function, code {ret}", file=sys.stderr)
        sys.exit(ret)
    
    filenumber += 1
    
    try:
        inputstrs = input("next command: ").split()

        if inputstrs[0] == "steps":
            steps_for_inf = int(inputstrs[1])
        else:
            mandmin, mandmax = mandelbrot.remap_mands(mandmin, mandmax, int(inputstrs[0]))
    except EOFError:
        break