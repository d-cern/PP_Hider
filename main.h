#ifndef MAIN_H
#define MAIN_H

#ifdef _WIN32
    #include <windows.h>
    #include <stdio.h>
    #include <math.h>
#else
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include <math.h>
#endif

#include "fileHandler.h"
#include "paletteParityHider.h"

void parseCommandLine(int argc, char *argv[]);
void initGlobals();
void usage();

#endif