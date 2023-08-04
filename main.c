/* 
    Palette Parity Hider using Jiri Fridrich Technique 
    Compiled with GCC using MinGW

    Duarte Cernadas
*/

#include "main.h"


// Global Variables
BITMAPFILEHEADER *gpCoverFileHdr, *gpStegoFileHdr;
BITMAPINFOHEADER *gpCoverFileInfoHdr, *gpStegoFileInfoHdr;
RGBQUAD *gpCoverPalette, *gpStegoPalette;
unsigned int gCoverFileSize, gMsgFileSize, gStegoFileSize;

char gCoverFileName[MAX_PATH];
char gMsgFileName[MAX_PATH];
char gStegoFileName[MAX_PATH];
char gAction;						// typically hide (1), extract (2), wipe (3), randomize (4), but also specifies custom actions for specific programs


int main(int argc, char *argv[])
{
	BITMAPFILEHEADER **pInFileHdr;
	BITMAPINFOHEADER **pInFileInfoHdr;
	RGBQUAD **pInFilePalette;
	unsigned int *pInFileSize;
	char (*pInFileName)[MAX_PATH]; // shut up gcc

	BYTE *imgData, *pixelData;
	BYTE *msgData;
	BYTE *modCover; // modified cover data

	// get the number of bits to use for data hiding or data extracting
	// if not specified, default to one
	initGlobals();
	parseCommandLine(argc, argv);

	// set pointers
	if(gAction == ACTION_HIDE)
	{
		pInFileHdr = &gpCoverFileHdr;
		pInFileInfoHdr = &gpCoverFileInfoHdr;
		pInFilePalette = &gpCoverPalette;
		pInFileSize = &gCoverFileSize;
		pInFileName = &gCoverFileName;
	}
	else if(gAction == ACTION_EXTRACT)
	{
		pInFileHdr = &gpStegoFileHdr;
		pInFileInfoHdr = &gpStegoFileInfoHdr;
		pInFilePalette = &gpStegoPalette;
		pInFileSize = &gStegoFileSize;
		pInFileName = &gStegoFileName;
	}
	else
	{
		fprintf(stderr, "Error: main() - invalid action\n");
		return 1;
	}

	if(0 == 0)
	{
		// read bmp
		imgData = readFile(*pInFileName, pInFileSize);
		if(imgData == NULL) return 1;
		// get header
		*pInFileHdr = (BITMAPFILEHEADER *) imgData;
		// get info header
		*pInFileInfoHdr = (BITMAPINFOHEADER *) (imgData + sizeof(BITMAPFILEHEADER) );

		//	There are no palettes
		if((*pInFileInfoHdr)->biBitCount > 16 || (*pInFileInfoHdr)->biBitCount == -1)
		{
			printf("\nError: Input image does not have a palette\n\n");
			if(imgData) free(imgData);
			return 1;
		}
		*pInFilePalette = (RGBQUAD *) ( (char *) imgData + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) );

		pixelData = imgData + (*pInFileHdr)->bfOffBits;


		printf("c=%02X, %02x\n", *imgData, *(imgData+1) );

		//displayBitmapInfo(gCoverFileName, pInFileHdr, pInFileInfoHdr, pInFilePalette, pixelData);
	}

	// hide
	if(gAction == ACTION_HIDE)
	{
		msgData = readFile(gMsgFileName, &gMsgFileSize);
		//printf("msgfilesize = %u\n", gMsgFileSize);
		if(msgData == NULL)
		{
			free(imgData);
			return -1;
		}

		modCover = hideMessage(msgData, pixelData);

		if (writeFile(gStegoFileName, gCoverFileSize, modCover))
		{
			fprintf(stderr, "Error: writeFile() - failed to write file\n");
			free(imgData);
			free(msgData);
			free(modCover);

			return -1;
		}
	}
	// extract
	else if(gAction == ACTION_EXTRACT)
	{
		msgData = extractMessage(pixelData);
	}

	free(imgData);
	free(msgData);
	free(modCover);

	return 0;
}


void initGlobals()
{
	gpCoverFileHdr = NULL;
	gpStegoFileHdr = NULL;
	gpCoverFileInfoHdr = NULL;
	gpStegoFileInfoHdr = NULL;
	gpCoverPalette = NULL;
	gpStegoPalette = NULL;
	gCoverFileSize = gMsgFileSize = gStegoFileSize = 0;

	// Command Line Global Variables
	//gCoverPathFileName[0] = 0;
	//gCoverFileName = NULL;
	//gMsgPathFileName[0] = 0;
	//gMsgFileName = NULL;
	//gStegoPathFileName[0] = 0;
	//gStegoFileName = NULL;
	gAction = 0;						// typically hide (1), extract (2)

	return;
}

void parseCommandLine(int argc, char *argv[])
{
	const char extractFilePrefix[5] = "ext_\0";
	const char stegoFilePrefix[5] = "hid_\0";
	int cnt;

	if(argc < 2) usage();

	// RESET Parameters to make error checking easier
	gAction = 0;
	gCoverFileName[0] = 0;
	gMsgFileName[0] = 0;
	gStegoFileName[0] = 0;
	cnt = 1;
	while(cnt < argc)	// argv[0] = program name
	{
		if(_stricmp(argv[cnt], "-c") == 0)	// cover file
		{
			cnt++;
			if(cnt == argc)
			{
				fprintf(stderr, "\n\nError - no file name following <%s> parameter.\n\n", argv[cnt-1]);
				exit(-1);
			}

			if(gCoverFileName[0] != 0)
			{
				fprintf(stderr, "\n\nError - cover file <%s> already specified.\n\n", gCoverFileName);
				exit(-2);
			}

			// Note: function doesn't seem to work with GCC
			//GetFullPathName(argv[cnt], MAX_PATH, gCoverPathFileName, &gCoverFileName);
			strncpy(gCoverFileName, argv[cnt], MAX_PATH);
		}
		else if(_stricmp(argv[cnt], "-m") == 0)	// msg file
		{
			cnt++;
			if(cnt == argc)
			{
				fprintf(stderr, "\n\nError - no file name following <%s> parameter.\n\n", argv[cnt-1]);
				exit(-1);
			}

			if(gMsgFileName[0] != 0)
			{
				fprintf(stderr, "\n\nError - message file <%s> already specified.\n\n", gMsgFileName);
				exit(-2);
			}

			// Note: function doesn't seem to work with GCC
			//GetFullPathName(argv[cnt], MAX_PATH, gMsgPathFileName, &gMsgFileName);
			strncpy(gMsgFileName, argv[cnt], MAX_PATH);
		}
		else if(_stricmp(argv[cnt], "-s") == 0) // stego file
		{
			cnt++;
			if(cnt == argc)
			{
				fprintf(stderr, "\n\nError - no file name following '%s' parameter.\n\n", argv[cnt-1]);
				exit(-1);
			}

			if(gStegoFileName[0] != 0)
			{
				fprintf(stderr, "\n\nError - stego file <%s> already specified.\n\n", gStegoFileName);
				exit(-2);
			}

			// Note: function doesn't seem to work with GCC
			//GetFullPathName(argv[cnt], MAX_PATH, gStegoPathFileName, &gStegoFileName);
			strncpy(gStegoFileName, argv[cnt], MAX_PATH);
		}
		else if(_stricmp(argv[cnt], "-hide") == 0)	// hide
		{
			if(gAction)
			{
				fprintf(stderr, "\n\nError, an action has already been specified.\n\n");
				exit(-1);
			}

			gAction = ACTION_HIDE;
		}
		else if(_stricmp(argv[cnt], "-extract") == 0)	// extract
		{
			if(gAction)
			{
				fprintf(stderr, "\n\nError, an action has already been specified.\n\n");
				exit(-1);
			}

			gAction = ACTION_EXTRACT;
		}

		cnt++;
	} // end while loop

	// if not specified, set stego/msg file output name
	if(
		(gStegoFileName[0] == 0 && gAction == ACTION_HIDE) || 
		(gMsgFileName[0] == 0 && gAction == ACTION_EXTRACT)	)
	{
		const char *prefix;
		char *fileOut;
		int tmplen = strnlen(gCoverFileName, MAX_PATH);
		int fileNameStart;

		// set file type
		fileOut = gAction == ACTION_HIDE ? gStegoFileName : gMsgFileName;
		// set file name prefix
 		prefix = gAction == ACTION_HIDE ? stegoFilePrefix : extractFilePrefix;

		// find start of file name:
		// ./media/images/picture.bmp => ../|_|picture.bmp
		for(int i = tmplen-1; i >= 0; i--)
		{
			if(gCoverFileName[i] == '\\' || gCoverFileName[i] == '/')
			{
				fileNameStart = i + 1;

				strncpy(fileOut, prefix, 5); // ./media/images/
				strncat(fileOut, gCoverFileName+fileNameStart, MAX_PATH - (tmplen + 4 - fileNameStart));
				break;
			}
			else if(i == 0)
			{
				fileNameStart = 0;

				strncpy(fileOut, prefix, 5);
				strncat(fileOut, gCoverFileName, MAX_PATH - 4);
			}
		}

		printf("%s\n", fileOut);
	}
}

// prints help message to the screen
void usage()
{
	char *programName = ".\\PP_Hider.exe";
	fprintf(stdout, "\n");
	fprintf(stdout, "To Hide:\n");
	fprintf(stdout, "\t%s -hide -c <cover file> -m <msg file> [-s <stego file>]\n\n", programName);

	fprintf(stdout, "To Extract:\n");
	fprintf(stdout, "\t%s -extract -s <stego file> [-m <message file>]\n\n", programName);

	//fprintf(stdout, "Parameters:\n");
	//fprintf(stdout, "Set number of bits to hide per pixel:	-b ( 1 to 8 def:1 )\n\n");

	//fprintf(stdout, "\n\tNOTES:\n\t1.Order of parameters is irrelevant.\n\t2.All selections in \"[]\" are optional.\n\n");
	exit(0);
}