/*
 * utility.h
 *
 *  Created on: Oct 18, 2020
 *      Author: Chenaniah
 */

#ifndef UTILITY_H_
#define UTILITY_H_

#define ERROR_NO_ERROR              0x00000000
#define ERROR_MALLOC_FAILED         0x0FFF0001
#define ERROR_FILE_NOT_EXIST        0x0FFF0002
#define ERROR_FILE_OPEN_FAILED      0x0FFF0003

#define DEBUG       1

#ifdef DEBUG
#include <stdio.h>
#define LOGI(fmt, x...) \
do \
{ \
    printf("%s %s(Line %d): "fmt,__FILE__,__FUNCTION__,__LINE__, ##x); \
    printf("\n"); \
}while(0)
#else
#define debug(fmt, x...)
#endif

unsigned long long getFileSize(char* file);
int convertHexToString(unsigned char* in, int in_len, char* out, int out_size);


#endif