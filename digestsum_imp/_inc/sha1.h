/**
* @file re_sha.h  Interface to SHA (Secure Hash Standard) functions
*
* Copyright (C) 2010 Creytiv.com
*/

#ifndef SHA_H_
#define SHA_H_
#include <stdint.h>

#ifdef USE_OPENSSL
#include <openssl/sha.h>
#else

/* public api for steve reid's public domain SHA-1 implementation */
/* this file is in the public domain */

/** SHA-1 Context */
typedef struct {
    uint32_t state[5];
    /**< Context state */
    uint32_t count[2];
    /**< Counter       */
    uint8_t buffer[64]; /**< SHA-1 buffer  */
} SHA1_CTX;

/** SHA-1 Context (OpenSSL compat) */
typedef SHA1_CTX SHA_CTX;

/** SHA-1 Digest size in bytes */
#define SHA1_DIGEST_SIZE 20
/** SHA-1 Digest size in bytes (OpenSSL compat) */
#define SHA_DIGEST_LENGTH SHA1_DIGEST_SIZE

void sha1_init(void *ctx);

void sha1_update(void *ctx, const unsigned char *p, size_t len);

void sha1_final(void *ctx, unsigned char digest[SHA1_DIGEST_SIZE]);

#endif

#endif