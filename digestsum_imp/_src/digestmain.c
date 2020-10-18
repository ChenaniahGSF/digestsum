/*
 * digestmain.c
 *
 *  Created on: Oct 18, 2020
 *      Author: Chenaniah
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "digestsum_api.h"
#include "utility.h"

void print_persent(int persent) {
   printf("process = %d\n",persent);
}

void print_error_1() {
   printf("-a -b -c -d only choose one from four\n");
}

void print_error_2() {
   printf("-f -s only choose one from two\n");
}

void print_help() {
   printf("-a means choose algorithm: md5\n");
   printf("-b means choose algorithm: sha1\n");
   printf("-c means choose algorithm: sha256\n");
   printf("-d means choose algorithm: sha512\n");
   printf("-a -b -c -d can ignore, default algorithm: md5\n");
   printf("-f means calculate digest from a file\n");
   printf("-s means calculate digest form a string\n");
   printf("-h means help\n");
   printf("eg: digestsum -a -f filename\n");
   printf("eg: digestsum -a -s string\n");
   printf("eg: digestsum -f filename\n");
   printf("eg: digestsum -s string\n");
   printf("eg: digestsum -h\n");
}

int main( int argc, char **argv) {
   int ret = 0;
   int oc;
   int flag_1 = 0;
   int flag_2 = 0;
   HashDigest* digest = NULL;
   unsigned char* md = NULL;
   int md_len = 0;
   char mdString[256] = {0};

   if(argc == 1) {
       print_help();
   }

   opterr = 0;
   while((oc = getopt(argc, argv, "abcdf:s:h")) != -1) {
      switch(oc) {
         case 'a':
            flag_1++;
            if(flag_1 >= 2) {
               print_error_1();
               digest->destroySelf(digest);
               digest = NULL;
               return 0;
            }
            digest = createDigestHandler(MD5);
            break;
         case 'b':
             flag_1++;
             if(flag_1 >= 2) {
                print_error_1();
                digest->destroySelf(digest);
                digest = NULL;
                return 0;
             }
             digest = createDigestHandler(SHA1);
             break;
         case 'c':
            flag_1++;
            if(flag_1 >= 2) {
               print_error_1();
               digest->destroySelf(digest);
               digest = NULL;
               return 0;
            }
            digest = createDigestHandler(SHA256);
            break;
         case 'd':
             flag_1++;
             if(flag_1 >= 2) {
                print_error_1();
                digest->destroySelf(digest);
                digest = NULL;
                return 0;
             }
             digest = createDigestHandler(SHA512);
             break;
         case 'f':
            flag_2++;
            if(flag_2 >= 2) {
                print_error_2();
                return 0;
            }
            if(digest == NULL) {
               digest = createDigestHandler(MD5);
            }
            if((digest->calcDigestFromFile(digest, optarg, &md, &md_len, NULL)) != 0) {
               LOGI("calc digest from file failed");
               digest->destroySelf(digest);
               digest = NULL;
               return 0;
            }
            if(convertHexToString(md, md_len, mdString, sizeof(mdString)) == 0) {
               printf("%s\n", mdString);
            }
            digest->destroySelf(digest);
            digest = NULL;
            break;
         case 's':
            flag_2++;
            if(flag_2 >= 2) {
                print_error_2();
                return 0;
            }
            if(digest == NULL) {
               digest = createDigestHandler(MD5);
            }
            if((digest->calcDigestFromMemory(digest, optarg, strlen(optarg), &md, &md_len)) != 0) {
               LOGI("calc digest from memory failed");
               digest->destroySelf(digest);
               digest = NULL;
               return 0;
            }
            if(convertHexToString(md, md_len, mdString, sizeof(mdString)) == 0) {
               printf("%s\n", mdString);
            }
            digest->destroySelf(digest);
            digest = NULL;
            break;
         case 'h':
            print_help();
            break;
         case '?':
            printf("arguments error!\n");
            print_help();
            break;
         default:
            break;
      }
   }
   return 0;
}
