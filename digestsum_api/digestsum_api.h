/*
 *  digestsum_api.h
 * 
 *  Created on: Oct 18, 2020
 *      Author: Chenaniah
 */

#ifndef DIGESTSUM_API_H
#define DIGESTSUM_API_H

typedef enum {
    DEFAULT = 0,
    MD5,
    SHA1,
    SHA256,
    SHA512
}HashType;

typedef struct t_HashDigest {
    int (*calcDigestFromFile)(struct t_HashDigest* self, char* file, unsigned char** out, int* out_len, void (*p)(int));
    int (*calcDigestFromMemory)(struct t_HashDigest* self, char* in, size_t in_len, unsigned char** out, int* out_len);
    void (*destroySelf)(struct t_HashDigest* self);
}HashDigest;

HashDigest* createDigestHandler(HashType in);

#endif