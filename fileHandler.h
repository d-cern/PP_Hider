#ifndef FILEFUNC_H
#define FILEFUNC_H

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
    #define WORD unsigned short
    #define LONG int
    #define MAX_PATH 260

    // the following structure information is taken from wingdi.h
    /* constants for the biCompression field */
    #define BI_RGB        0L	// (no compression)
    #define BI_RLE8       1L	// (run-length encoding, 8 bits)
    #define BI_RLE4       2L	// (run-length encoding, 4 bits)
    #define BI_BITFIELDS  3L
    #define BI_JPEG       4L
    #define BI_PNG        5L
    
    // this structure defines the file header format for a bitmap file
    #pragma pack(1)                                     // linux compatibility
    typedef struct tagBITMAPFILEHEADER // (14 bytes)
    {
        WORD    bfType;					// ASCII "BM"
        unsigned int bfSize;                            // total length of bitmap file
        WORD bfReserved1;                               // reserved
        WORD bfReserved2;                               // reserved
        unsigned int bfOffBits;                         // offset to start of actual pixel data
    } BITMAPFILEHEADER;
    
    // this structure defines the header which describes the bitmap itself
    typedef struct tagBITMAPINFOHEADER // (40 bytes)
    {
        unsigned int biSize;         // size of BITMAPINFOHEADER
        LONG biWidth;                // width in pixels
        LONG biHeight;               // height in pixels
        WORD biPlanes;               // always 1
        WORD biBitCount;             // color bits per pixel
        unsigned int biCompression;  // BI_RGB, BI_RLE8, BI_RLE4
        unsigned int biSizeImage;    // total bytes in image
        LONG biXPelsPerMeter;        // 0, or optional horizontal resolution
        LONG biYPelsPerMeter;        // 0, or optional vertical resolution
        unsigned int biClrUsed;      // colors actually used (normally zero, can be lower than biBitCount)
        unsigned int biClrImportant; // important colors actualy used (normally zero)
    } BITMAPINFOHEADER;	
    
    typedef struct tagRGBQUAD
    {
        BYTE rgbBlue;
        BYTE rgbGreen;
        BYTE rgbRed;
        BYTE rgbReserved;
    } RGBQUAD;

#endif

#define ACTION_HIDE     1
#define ACTION_EXTRACT  2

extern BITMAPFILEHEADER *gpCoverFileHdr, *gpStegoFileHdr;
extern BITMAPINFOHEADER *gpCoverFileInfoHdr, *gpStegoFileInfoHdr;
extern RGBQUAD *gpCoverPalette, *gpStegoPalette;
extern unsigned int gCoverFileSize, gMsgFileSize, gStegoFileSize;
extern char gAction;

extern BITMAPFILEHEADER **gpInFileHdr;
extern BITMAPINFOHEADER **gpInFileInfoHdr;
extern RGBQUAD **gpInFilePalette;
extern unsigned int *gpInFileSize;
extern char (*gpInFileName)[MAX_PATH]; // shut up gcc


BYTE *readFile(char *filename, unsigned int *fileSize, char mode);
int writeFile(char *filename, unsigned int filesize, BYTE *dataStream);

void displayBitmapInfo(     char *pFileName,
                            BITMAPFILEHEADER *pFileHdr,
                            BITMAPINFOHEADER *pFileInfo,
                            RGBQUAD *ptrPalette,
                            BYTE *pixelData        );
int isValidBitmap(BYTE *fileData);


#


#endif