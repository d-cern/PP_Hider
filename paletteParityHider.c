#include "paletteParityHider.h"

BYTE gBitMasks[8] = {
                                0x01,   // 0000 0001
                                0x02,   // 0000 0010
                                0x04,   // 0000 0100
                                0x08,   // 0000 1000
                                0x10,   // 0001 0000
                                0x20,   // 0010 0000
                                0x40,   // 0100 0000
                                0x80    // 1000 0000
                            };


BYTE *hideMessage(BYTE *msgData, BYTE *pixelData)
{
    /* ---------------------------------------
    // closest colors of each parity (0 and 1) 
    // ---------------------------------------
    //  double array: for each color, store the 2 closest colors with differing parities
    //  ccParities[n][0]: closest color to palette index n with parity of 0 
    //
    //  NOTE: might be better to make it a single array since closest color of
    //        equal parity will always be itself (at least for this application)
    */
    int ccParities[256][2];

    BYTE curMsgByte, curMsgBit;
    //BYTE curCoverByte, curCoverBit;
    //BYTE tmp;
    BYTE *bytesToHide,          // msg data with its size prepended
            *modCover;          // cover data stream with hidden message
    unsigned int curPixel = 0;

    // ccParities[][]: initialize all values to -1
    memset(ccParities, -1, sizeof(unsigned int) * 256 * 2);
    /*
    for(int i = 0; i < 256; i++)
    {
        for(int j = 0; j < 2; j++)
        {
            ccParities[i][j] = -1;
        }
    } */

    // allocate memory for output stream
    modCover = (BYTE *) malloc(gpCoverFileInfoHdr->biSizeImage);
    // copy pixel data
    memcpy(modCover, pixelData, gpCoverFileInfoHdr->biSizeImage);

    // allocate memory for msg data with msg size
    bytesToHide = (BYTE *) malloc(gMsgFileSize + sizeof(unsigned int));
    // set first 4 bytes to msg size : little endian
    unsigned int tmp = gMsgFileSize;
    for(int i = 0; i < sizeof(unsigned int); i++)
    {
        memset(bytesToHide+i, tmp, 1);
        tmp >>= 8;
    }
    // copy message into bytesToHide
    memcpy(bytesToHide+sizeof(unsigned int), msgData, gMsgFileSize);


    // loop through msg bytes
    for(unsigned int i = 0; i < gMsgFileSize+sizeof(unsigned int); i++)
    {
        curMsgByte = bytesToHide[i];

        // loop through bits in Byte
        for(int j = 0; j < 8; j++)
        {
        //              |         select bit          |   set curMsgBit to value of bit   |
            curMsgBit = (curMsgByte & gBitMasks[7 - j]) == 0 ? 0 : 1;
            
            // check if bit to hide is the same as parity of this pixel's color
            if(curMsgBit != getPixelColorParity(modCover[curPixel]))
            {
                getClosestColor(modCover[curPixel], ccParities);
                modCover[curPixel] = ccParities[ modCover[curPixel] ][curMsgBit];
            }
            //else { modCover[curPixel] = modCover[curPixel]; }

            curPixel++;
        }

        if(curPixel >= gpCoverFileInfoHdr->biSizeImage)
        {
            printf("WARNING: hiding stopped at %u bytes\n", i);
            break;
        }
    }
    free(bytesToHide);
/*
    for(int i = 0; i < 256; i++)
    {
        printf("i %0x: ", i);
        for(int j = 0; j < 2; j++)
        {
            printf("%0x ", ccParities[i][j]);
        }
        printf("\n");
    }*/

    return modCover;
}

BYTE *extractMessage(BYTE *pixelData)
{
    int colorParities[256];

    BYTE *outMsgData;
    BYTE btmp, *bytesMsg;
    unsigned int curPixel = 0;

    bytesMsg = (BYTE *) malloc(sizeof(unsigned int) * sizeof(BYTE));

    // set array to -1
    memset(colorParities, -1, 256 * sizeof(int));

    // read message size
    for(int i = 0; i < sizeof(unsigned int); i++)
    {
        btmp = 0;

        for(int j = 0; j < 8; j++)
        {
            if(colorParities[pixelData[curPixel]] == -1)
            {
                colorParities[pixelData[curPixel]] = getPixelColorParity(pixelData[curPixel]);
            }

            // lshift 1
            btmp <<= 1;
            // add parity bit
            btmp += colorParities[pixelData[curPixel]];
            curPixel++;
        }

        *(bytesMsg+i) = btmp;
    }

    gMsgFileSize = *((unsigned int *) bytesMsg);
    free(bytesMsg);
    //printf("msg size = %u\n", gMsgFileSize);

    // allocate memory for output message byte stream
    outMsgData = (BYTE *) malloc(gMsgFileSize * sizeof(BYTE));

    for(int i = 0; i < gMsgFileSize; i++)
    {
        btmp = 0;

        for(int j = 0; j < 8; j++)
        {
            
            if(colorParities[pixelData[curPixel]] == -1)
            {
                colorParities[pixelData[curPixel]] = getPixelColorParity(pixelData[curPixel]);
            }

            // lshift 1
            btmp <<= 1;
            // add parity bit
            btmp += colorParities[pixelData[curPixel]];
            curPixel++;
        }

        *(outMsgData + i) = btmp;
    }

    return outMsgData;
}

void getClosestColor(BYTE pIdx, int ccParities[256][2])
{
    BYTE parity;
    BYTE ccIdx;     // closest color palette index
    double closestDistance, curDistance;
    //printf("getClosestColor\n");

    // check if closest colors have already been calculated
    if(ccParities[pIdx][0] != -1)
    {
        return;
    }


    parity = getPixelColorParity(pIdx);
    if(parity != 0 && parity != 1) // debug check
    {
        fprintf(stderr, "Error: getClosestColor() - Color parity not 0 or 1\n");
        exit(-1);
    }

    // closest color of same parity is itself
    ccParities[pIdx][parity] = pIdx;

    // initialize at palette idx 0 (or 1 if pIdx == 0)
    ccIdx = pIdx == 0 ? 1: 0;
    closestDistance = getColorDistance(gpCoverPalette[pIdx], gpCoverPalette[ccIdx]);

    // find closest color with opposite parity
    for (int i = 0; i < 0x100; i++)
    {
        if(i == pIdx) continue; // skip self comparison

        if(getPixelColorParity(i) == parity) continue; // skip colors of equal parity

        // check if i-th color is closer than current closest color
        curDistance = getColorDistance(gpCoverPalette[pIdx], gpCoverPalette[i]);
        if(curDistance < closestDistance)
        {
            ccIdx = i;
            closestDistance = curDistance;
        }
    }

    // set value in ccParities[][]
    if (parity == 0)
    {
        ccParities[pIdx][1] = ccIdx;
    }
    else
    {
        ccParities[pIdx][0] = ccIdx;
    }
}

// calculates the distance between 2 colors
double getColorDistance(RGBQUAD c1, RGBQUAD c2)
{
    return sqrt( pow((c1.rgbRed - c2.rgbRed), 2) + pow((c1.rgbGreen - c2.rgbGreen), 2) + pow((c1.rgbBlue - c2.rgbBlue), 2) );
}

// given a palette index, find the parity of its color
// (r + g + b) % 2
BYTE getPixelColorParity(BYTE pixelByte)
{
    if(gAction == ACTION_HIDE)
        return (gpCoverPalette[pixelByte].rgbRed + gpCoverPalette[pixelByte].rgbGreen + gpCoverPalette[pixelByte].rgbBlue) % 2;
    else
        return (gpStegoPalette[pixelByte].rgbRed + gpStegoPalette[pixelByte].rgbGreen + gpStegoPalette[pixelByte].rgbBlue) % 2;
}