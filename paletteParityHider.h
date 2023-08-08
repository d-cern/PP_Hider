#ifndef PALETTEPARITYHIDER_H
#define PALETTEPARITYHIDER_H

#ifdef _WIN32
    #include <windows.h>
    #include <stdio.h>
    #include <math.h>
#else
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include <math.h>

    #define BYTE unsigned char
#endif

#define ACTION_HIDE     1
#define ACTION_EXTRACT  2

extern BITMAPFILEHEADER *gpCoverFileHdr, *gpStegoFileHdr;
extern BITMAPINFOHEADER *gpCoverFileInfoHdr, *gpStegoFileInfoHdr;
extern RGBQUAD *gpCoverPalette, *gpStegoPalette;
extern unsigned int gCoverFileSize, gMsgFileSize, gStegoFileSize;
extern char gAction;

BYTE *hideMessage(BYTE *msgData, BYTE *pixelData);
BYTE *extractMessage(BYTE *pixelData);

void getClosestColor(BYTE pIdx, int ccParities[256][2]);
double getColorDistance(RGBQUAD c1, RGBQUAD c2);
BYTE getPixelColorParity(BYTE pixelByte);

#endif