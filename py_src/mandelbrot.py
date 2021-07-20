import ctypes
import os

class c_complex(ctypes.Structure):
    _fields_ = [("re", ctypes.c_longdouble),
                ("im", ctypes.c_longdouble)]

    def __init__(self, pycomplex):
        self.re = pycomplex.real
        self.im = pycomplex.imag


libmandel = ctypes.CDLL(os.getcwd() + "/../build/libmandel.so")

libmandel.makemandel.argtypes = [ctypes.c_char_p,
                                ctypes.c_int, ctypes.c_int, 
                                c_complex, c_complex, ctypes.c_int]

libmandel.remap_mands.argtypes = [ctypes.POINTER(c_complex), 
                                ctypes.POINTER(c_complex),
                                ctypes.c_int]



def makemandel(file, height, width, mandmin, mandmax, steps_for_inf):
    libmandel.makemandel(
        file.encode("utf-8"), 
        height, width, c_complex(mandmin), c_complex(mandmax), steps_for_inf
    )

def remap_mands(mandmin, mandmax, section):
    cmandmin = c_complex(mandmin)
    cmandmax = c_complex(mandmax)

    libmandel.remap_mands(
        ctypes.byref(cmandmin), ctypes.byref(cmandmax), section
    )

    mandmin = complex(cmandmin.re, cmandmin.im)
    mandmax = complex(cmandmax.re, cmandmax.im)

    return (mandmin, mandmax)


if __name__ == "__main__":
    mandmin = -2-1j
    mandmax =  1+1j
    mandmin, mandmax = remap_mands(mandmin, mandmax, 2)
    print(mandmin, mandmax)
    makemandel("test.png", 600, 800, mandmin, mandmax, 255)

    