# PP_Hider - Closest Color Palette Parity Hider
Based on the technique outlined by Jiri Fridrich and Rui Wu.

Compiled with GCC using [MSYS2 MinGW](https://www.mingw-w64.org/downloads/).

Pre-compiled x86-64 Windows binary included in bin/

## Usage
To Hide:
        .\PP_Hider.exe -hide -c \<cover file\> -m \<msg file\> [-s \<stego file\>]

To Extract:
        .\PP_Hider.exe -extract -s \<stego file\> [-m \<message file\>]

## Non-GNU Standard .h Files
- windows.h