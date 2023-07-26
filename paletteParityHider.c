#include "paletteParityHider.h"

unsigned char gBitMasks[8] = {
                                0xFE,   // 1111 1110
                                0xFD,   // 1111 1101
                                0xFC,   // 1111 1011
                                0xFB,   // 1111 0111
                                0xFA,   // 1110 1111
                                0xF9,   // 1101 1111
                                0xF8,   // 1011 1111
                                0xF7    // 0111 1111
                            }

unsigned char *hideMessage(unsigned char *msgData, unsigned char *coverData)
{
    // closest color parities
    //  double array: for each color, store the 2 closest colors with differing parities
    //  ccParities[n][0]: closest color to palette index 1 with parity of 0 
    int ccParities[256][2];

    char curMsgBit;
    char curCoverBit;

    // loop through msg bytes
    for(unsigned int i = 0; i < gMsgFileSize; i++)
    {
        // loop through bits in Byte
        for(int j = 0; i < 8; j++)
        {
            curMsgBit = msgData[i] ^ gBitMasks[j];

        }
        
        // loop through bits in current msg byte
    }
}

unsigned char *extractMessage(unsigned char *coverData)
{

}