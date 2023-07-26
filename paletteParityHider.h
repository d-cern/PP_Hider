#ifndef PALETTEPARITYHIDER_H
#define PALETTEPARITYHIDER_H
#include <stdio.h>
#include <math.h>
#include <windows.h>

extern BITMAPFILEHEADER *gpCoverFileHdr, *gpStegoFileHdr;
extern BITMAPINFOHEADER *gpCoverFileInfoHdr, *gpStegoFileInfoHdr;
extern RGBQUAD *gpCoverPalette, *gpStegoPalette;
extern unsigned int gCoverFileSize, gMsgFileSize, gStegoFileSize;

BYTE *hideMessage(unsigned char *msgData, unsigned char *coverData);
BYTE *extractMessage(unsigned char *coverData);

BYTE getClosestColor(unsigned char pIdx, int **ccParities);
BYTE getPixelColorParity(unsigned char pixelByte)

#endif