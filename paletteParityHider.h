#ifndef PALETTEPARITYHIDER_H
#define PALETTEPARITYHIDER_H

#include <stdio.h>
#include <math.h>
#include <windows.h>

extern BITMAPFILEHEADER *gpCoverFileHdr, *gpStegoFileHdr;
extern BITMAPINFOHEADER *gpCoverFileInfoHdr, *gpStegoFileInfoHdr;
extern RGBQUAD *gpCoverPalette, *gpStegoPalette;
extern unsigned int gCoverFileSize, gMsgFileSize, gStegoFileSize;

BYTE *hideMessage(BYTE *msgData, BYTE *coverData);
BYTE *extractMessage(BYTE *coverData);

void getClosestColor(BYTE pIdx, int ccParities[256][2]);
double getColorDistance(RGBQUAD c1, RGBQUAD c2);
BYTE getPixelColorParity(BYTE pixelByte);

#endif