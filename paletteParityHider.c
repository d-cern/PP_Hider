#include "paletteParityHider.h"

BYTE gBitMasks[8] = {
                                0xFE,   // 1111 1110
                                0xFD,   // 1111 1101
                                0xFC,   // 1111 1011
                                0xFB,   // 1111 0111
                                0xFA,   // 1110 1111
                                0xF9,   // 1101 1111
                                0xF8,   // 1011 1111
                                0xF7    // 0111 1111
                            };

BYTE *hideMessage(BYTE *msgData, BYTE *coverData)
{
    /* ---------------------------------------
    // closest colors of each parity (0 and 1) 
    // ---------------------------------------
    //  double array: for each color, store the 2 closest colors with differing parities
    //  ccParities[n][0]: closest color to palette index n with parity of 0 
    //
    //  NOTE: might be better to make it a single array since closest color of
    //        equal parity will always be itself (at least for this technique)
    */
    int ccParities[256][2] = {-1};

    BYTE curMsgByte, curMsgBit;
    BYTE curCoverByte, curCoverBit;
    BYTE tmp;
    int curPixel = 0;

    // loop through msg bytes
    for(unsigned int i = 0; i < gMsgFileSize; i++)
    {
        curMsgByte = msgData[i];

        // loop through bits in Byte
        for(int j = 0; i < 8; j++)
        {
        //              |         select bit          |   set curMsgBit to value of bit   |
            curMsgBit = (curMsgByte ^ gBitMasks[8 - j]) == 0 ? 0 : 1;
            
            if(curMsgBit != getPixelColorParity(coverData[curPixel]))
            {
                getClosestColor(coverData[curPixel], ccParities);
            }
            //curCoverByte = coverData[curPixel];
        }
        
    }
}

BYTE *extractMessage(BYTE *coverData)
{
    return 0;
}

void getClosestColor(BYTE pIdx, int **ccParities)
{
    BYTE parity;
    int curDistance;

    parity = getPixelColorParity(pIdx);

    if(ccParities[pIdx][0] != -1)
    {
        return;
    }

    if(parity == 0)
    {
        ccParities[pIdx][0] = pIdx;

        ccParities[pIdx][1] = 0;
        curDistance = getColorDistance(gpCoverPalette[pIdx], gpCoverPalette[0]);

        for(BYTE i = 0; i < 0x100; i++)
        {
            if(i == pIdx) continue;

            if(getPixelColorParity(i) != 1) continue;


        }
    }
}

// calculates the distance between 2 colors
int getColorDistance(RGBQUAD c1, RGBQUAD c2)
{
    return sqrt( pow((c1.rgbRed - c2.rgbRed), 2) + pow((c1.rgbGreen - c2.rgbGreen), 2) + pow((c1.rgbBlue - c2.rgbBlue), 2) );
}

// (r + g + b) % 2
BYTE getPixelColorParity(BYTE pixelByte)
{
    return (gpCoverPalette[pixelByte].rgbRed + gpCoverPalette[pixelByte].rgbGreen + gpCoverPalette[pixelByte].rgbBlue) % 2;
}