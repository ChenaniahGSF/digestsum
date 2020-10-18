/*
 * sha256.h
 *
 *  Created on: Oct 18, 2020
 *      Author: Chenaniah
 */

#ifndef SHA256_H_
#define SHA256_H_

#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

#define SHA256_DIGEST_LENGTH 32

/**
 * @brief Context for ciper operatins based on sha256
 */
typedef struct {
    /** global state */
    uint32_t state[8];
    /** processed bytes counter */
    uint32_t count[2];
    /** data buffer */
    unsigned char buf[64];
} sha256_context_t;

typedef sha256_context_t SHA256_CTX;

/**
 * @brief SHA-256 initialization.  Begins a SHA-256 operation.
 *
 * @param ctx  sha256_context_t handle to init
 */
void sha256_init(void *ctx);

/**
 * @brief Add bytes into the hash
 *
 * @param ctx  sha256_context_t handle to use
 * @param in   pointer to the input buffer
 * @param len  length of the buffer
 */
void sha256_update(void *ctx, const unsigned char *in, size_t len);

/**
 * @brief SHA-256 finalization.  Pads the input data, exports the hash value,
 * and clears the context state.
 *
 * @param digest resulting digest, this is the hash of all the bytes
 * @param ctx    sha256_context_t handle to use
 */
void sha256_final(void *ctx, unsigned char digest[32]);

/**
 * @brief A wrapper function to simplify the generation of a hash, this is
 * usefull for generating sha256 for one buffer
 *
 * @param d pointer to the buffer to generate hash from
 * @param n length of the buffer
 * @param md optional pointer to an array for the result, length must be
 *           SHA256_DIGEST_LENGTH
 *           if md == NULL, one static buffer is used
 */
unsigned char *sha256(unsigned char *d, size_t n, unsigned char *md);

#ifdef __cplusplus
}
#endif

#endif