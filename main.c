/* 
    Palette Parity Hider using Jiri Fridrich Technique 
    Compiled with MinGW

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
	unsigned char *coverData, *pixelData;
	unsigned char *msgData;
	unsigned char *outputStream;

	// get the number of bits to use for data hiding or data extracting
	// if not specified, default to one
	initGlobals();
	printf("test\n");
	parseCommandLine(argc, argv);

	if(0 == 0)
	{
		// read bmp
		coverData = readFile(gCoverFileName, &gCoverFileSize);
		// get header
		gpCoverFileHdr = (BITMAPFILEHEADER *) coverData;
		// get info header
		gpCoverFileInfoHdr = (BITMAPINFOHEADER *) (coverData + sizeof(BITMAPFILEHEADER) );

		//	There are no palettes
		if(gpCoverFileInfoHdr->biBitCount > 16 || gpCoverFileInfoHdr->biBitCount == -1)
		{
			printf("\nError: Cover image does not have a palette\n\n");
			if(coverData) free(coverData);
			return 1;
		}
		gpCoverPalette = (RGBQUAD *) ( (char *) coverData + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) );

		pixelData = coverData + gpCoverFileHdr->bfOffBits;


		printf("c=%02X, %02x\n", *coverData, *(coverData+1) );

		displayBitmapInfo(gCoverFileName, gpCoverFileHdr, gpCoverFileInfoHdr, gpCoverPalette, pixelData);
	}
	
	// hide
	if(gAction == 1)
	{
		msgData = readFile(gMsgFileName, &gMsgFileSize);
		if(msgData == NULL)
		{
			free(coverData);
			return 1;
		}

		outputStream = hideMessage(msgData, coverData);
	}

	return 0;
} // main


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
	int cnt;

	if(argc < 2) usage();

	// RESET Parameters to make error checking easier
	gAction = 0;
	//gCoverPathFileName[0] = 0;
	//gMsgPathFileName[0] = 0;
	//gStegoPathFileName[0] = 0;
	cnt = 1;
	while(cnt < argc)	// argv[0] = program name
	{
		printf("cnt = %d; %s\n", cnt, argv[cnt]);
		if(_stricmp(argv[cnt], "-c") == 0)	// cover file
		{
			cnt++;
			printf("cnt = %d; %s\n", cnt, argv[cnt]);
			if(cnt == argc)
			{
				fprintf(stderr, "\n\nError - no file name following <%s> parameter.\n\n", argv[cnt-1]);
				exit(-1);
			}

			/*
			if(gCoverPathFileName[0] != 0)
			{
				fprintf(stderr, "\n\nError - cover file <%s> already specified.\n\n", gCoverPathFileName);
				exit(-2);
			}
			*/
			printf("in -c\n");

			// Note: function doesn't seem to work with GCC
			//GetFullPathName(argv[cnt], MAX_PATH, gCoverPathFileName, &gCoverFileName);
			strncpy(gCoverFileName, argv[cnt], MAX_PATH);
			printf("in -c\n");
		}
		else if(_stricmp(argv[cnt], "-m") == 0)	// msg file
		{
			cnt++;
			if(cnt == argc)
			{
				fprintf(stderr, "\n\nError - no file name following <%s> parameter.\n\n", argv[cnt-1]);
				exit(-1);
			}

			/*
			if(gMsgPathFileName[0] != 0)
			{
				fprintf(stderr, "\n\nError - message file <%s> already specified.\n\n", gMsgPathFileName);
				exit(-2);
			}
			*/

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

			/*
			if(gStegoPathFileName[0] != 0)
			{
				fprintf(stderr, "\n\nError - stego file <%s> already specified.\n\n", gStegoPathFileName);
				exit(-2);
			}
			*/

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
		printf("cnt = %d\n", cnt);

		cnt++;
	} // end while loop

	return;
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