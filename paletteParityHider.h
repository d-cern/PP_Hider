#ifndef PALETTEPARITYHIDER_H
#define PALETTEPARITYHIDER_H

#include <stdio.h>
#include <math.h>
#include <windows.h>

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