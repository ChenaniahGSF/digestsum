/*
 * utility.c
 *
 *  Created on: Oct 18, 2020
 *      Author: Chenaniah
 */
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

unsigned long long getFileSize(char* file) {
    struct stat64 buf;
    if(stat64(file, &buf) < 0) {
        return 0;
    } else {
        return buf.st_size;
    }
}

int convertHexToString(unsigned char* in, int in_len, char* out, int out_size) {
   int ret = -1;
   int i=0;
   char a[3]={0};
   if(out_size>=in_len*2)
   {
      for(i=0;i<in_len;i++)
      {
         memset(a,0,sizeof(a));
         snprintf(a,sizeof(a),"%02x",(unsigned char)in[i]);
         strncat(out,a,out_size - strlen(out) -1);
      }
      ret=0;
   }
   else
   {
      printf("ERROR: Out buffer len is too samll outlen[%d] inlen[%d]\n", out_size, in_len);
   }
   return ret;
}
