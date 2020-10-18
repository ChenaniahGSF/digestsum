/*
 * md5.h
 *
 *  Created on: Oct 18, 2020
 *      Author: Chenaniah
 */

#ifndef MD5_H_
#define MD5_H_
#include <stddef.h>

typedef struct
{
    unsigned int count[2];
    unsigned int state[4];
    unsigned char buffer[64];
} MD5_CTX;

#define MD5_DIGEST_SIZE     (16)

#define F(x,y,z) ((x & y) | (~x & z))
#define G(x,y,z) ((x & z) | (y & ~z))
#define H(x,y,z) (x^y^z)
#define I(x,y,z) (y ^ (x | ~z))
#define ROTATE_LEFT(x,n) ((x << n) | (x >> (32-n)))

#define FF(a,b,c,d,x,s,ac) \
{ \
    a += F(b,c,d) + x + ac; \
    a = ROTATE_LEFT(a,s); \
    a += b; \
}
#define GG(a,b,c,d,x,s,ac) \
{ \
    a += G(b,c,d) + x + ac; \
    a = ROTATE_LEFT(a,s); \
    a += b; \
}
#define HH(a,b,c,d,x,s,ac) \
{ \
    a += H(b,c,d) + x + ac; \
    a = ROTATE_LEFT(a,s); \
    a += b; \
}
#define II(a,b,c,d,x,s,ac) \
{ \
    a += I(b,c,d) + x + ac; \
    a = ROTATE_LEFT(a,s); \
    a += b; \
}
void md5_init(void *context);
void md5_update(void *context, const unsigned char *input, size_t inputlen);
void md5_final(void *context, unsigned char* digest);

#endif