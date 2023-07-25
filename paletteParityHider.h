#ifndef PALETTEPARITYHIDER_H
#define PALETTEPARITYHIDER_H
#include <stdio.h>

unsigned char *hideMessage(unsigned char *msgData, unsigned char *coverData);
unsigned char *extractMessage(unsigned char *coverData);

#endif