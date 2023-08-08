# PP_Hider - Closest Color Palette Parity Hider
Based on the technique outlined by Jiri Fridrich and Rui Wu.

Parity refers to the even/odd-ness of the sum of a pixel's color components, RGB.

Basic Algorithm:
```
for pixel in image:
        if msgBit == parity(pixel.color):
                continue
        else:
                pixel.color = getClosestColorOfParity(pixel.color, msgBit)
        endif
```
                
Compiled with GCC using [MSYS2 MinGW](https://www.mingw-w64.org/downloads/).

Pre-compiled x86-64 PE binary included in [bin/](https://github.com/d-cern/PP_Hider/tree/master/bin/).

## Usage
To Hide:
        .\PP_Hider.exe -hide -c \<cover file\> -m \<msg file\> [-s \<stego file\>]

To Extract:
        .\PP_Hider.exe -extract -s \<stego file\> [-m \<message file\>]

## Non-GNU Standard .h Files
- windows.h
