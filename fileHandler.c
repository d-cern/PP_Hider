#include "fileHandler.h"

unsigned char *readFile(char *filename, unsigned int *fileSize)
{
	FILE *ptrFile;
	unsigned char *pFile;

	ptrFile = fopen(filename, "rb");	// specify read only and binary (no CR/LF added)

	if(ptrFile == NULL)
	{
		printf("Error: Could not open file: %s.\n\n", filename);
		return NULL;
	}

	fseek(ptrFile, 0, SEEK_END);
	*fileSize = ftell(ptrFile);
	fseek(ptrFile, 0, SEEK_SET);

	// malloc memory to hold the file, include room for the header and color table
	pFile = (unsigned char *) malloc(*fileSize);

	if(pFile == NULL)
	{
		printf("Error: Could not allocate %d bytes of memory for file %s\n\n", *fileSize, filename);
		return NULL;
	}

	// Read in complete file
	// buffer for data, size of each item, max # items, ptr to the file
	fread(pFile, sizeof(unsigned char), *fileSize, ptrFile);
	fclose(ptrFile);

	return(pFile);
}

// writes modified bitmap file to disk
// gMask used to determine the name of the file
int writeFile(char *filename, unsigned int fileSize, unsigned char *pFile)
{
	FILE *ptrFile;
	int ret;

	// open the new file, MUST set binary format (text format will add line feed characters)
	ptrFile = fopen(filename, "wb+");
	if(ptrFile == NULL)
	{
		printf("Error: Could not open file for writing: %s.\n\n", filename);
		return -1;
	}

	// write the file
	ret = (int) fwrite(pFile, sizeof(unsigned char), fileSize, ptrFile);

	// check for success
	if(ret != fileSize)
	{
		printf("Error writing file %s.\n\n", filename);
		return -1;
	}

	fclose(ptrFile); // close file
	return 0;
}

/* Bitmap Functions */

// Show the various bitmap header bytes primarily for debugging
void displayFileInfo(char *pFileName,
					 BITMAPFILEHEADER *pFileHdr, 
					 BITMAPINFOHEADER *pFileInfo,
					 RGBQUAD *ptrPalette,
					 unsigned char *pixelData)
{
	int numColors, i;

	printf("\nFile Information for %s: \n\n", pFileName);
	printf("File Header Info:\n");
	printf("File Type: %c%c\nFile Size:%d\nData Offset:%d\n\n", 
		(pFileHdr->bfType & 0xFF), (pFileHdr->bfType >> 8), pFileHdr->bfSize, pFileHdr->bfOffBits);

	switch(pFileInfo->biBitCount)
	{
	case 1:
		numColors = 2;
		break;
	case 4:
		numColors = 16;
		break;
	case 8:
		numColors = 256;
		break;
	case 16:
		numColors = 65536;
		break;
	case 24:
		numColors = 16777216;
		break;
	default:
		numColors = -1;
	}

	printf("Bit Map Image Info:\n\nSize Info Header:%d\nWidth:%d\nHeight:%d\nPlanes:%d\n"
		"Bits/Pixel:%d ==> %d colors\n"
		"Compression:%d\nImage Size:%d\nRes X:%d\nRes Y:%d\nColors:%d\nImportant Colors:%d\n\n",
		pFileInfo->biSize, 
		pFileInfo->biWidth, 
		pFileInfo->biHeight, 
		pFileInfo->biPlanes, 
		pFileInfo->biBitCount, numColors,
		pFileInfo->biCompression, 
		pFileInfo->biSizeImage, 
		pFileInfo->biXPelsPerMeter,
		pFileInfo->biYPelsPerMeter,
		pFileInfo->biClrUsed,
		pFileInfo->biClrImportant);

	//	There are no palettes
	if(pFileInfo->biBitCount > 16 || numColors == -1)
	{
		printf("\nNo Palette\n\n");
	}
	else
	{
		printf("Palette:\n\n");

		for(i = 0; i < numColors; i++)
		{
			printf("R:%02x   G:%02x   B:%02x\n", ptrPalette->rgbRed, ptrPalette->rgbGreen, ptrPalette->rgbBlue);
			ptrPalette++;
		}
	}

	// print first 24 bytes of pixel data
	printf("\n Pixel Data: \n\n");
	for(int i = 0; i < 24; i++)
	{
		printf("%02X ", *(pixelData + i) );
	}
	printf("\n\n");
	return;
}

// quick check for bitmap file validity - you may want to expand this or be more specfic for a particular bitmap type
int isValidBitmap(unsigned char *fileData)
{
	if( fileData[0] != 'B' || fileData[1] != 'M') return -1;

	return 0;
} // isValidBitMap