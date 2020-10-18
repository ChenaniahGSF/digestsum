/*
 * calcdigest.c
 *
 *  Created on: Oct 18, 2020
 *      Author: Chenaniah
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "digestsum_api.h"
#include "md5.h"
#include "sha1.h"
#include "sha256.h"
#include "sha512.h"
#include "utility.h"

#define BLOCK_SIZE          (64*1024)

typedef struct t_calcDigest {
    int (*calcDigestFromFile)(HashDigest* self, char* file, unsigned char** out, int* out_len, void (*p)(int));
    int (*calcDigestFromMemory)(HashDigest* self, char* in, size_t in_len, unsigned char** out, int* out_len);
    void (*destroySelf)(HashDigest* self);
    void (*hash_init)(void* ctx);
    void (*hash_update)(void* ctx, const unsigned char* in, size_t in_len);
    void (*hash_final)(void* ctx, unsigned char* md);
    unsigned char* md;
    int mdLen;
    void* ctx;
}calcDigest;

static int m_calcDigestFromFile(HashDigest* self, char* file, unsigned char** out, int* out_len, void (*p)(int)) {
    int ret = 0;
    unsigned long long file_size = 0;
    unsigned long long current_size = 0;
    int persent = 0;
    int cur_persent = 0;
    size_t len = 0;
    FILE* fp = NULL;
    unsigned char* block = NULL;
    calcDigest* _this = (calcDigest*)self;
    block = (unsigned char*)malloc(BLOCK_SIZE*sizeof(unsigned char));
    if(!block) {
        ret = ERROR_MALLOC_FAILED;
        return ret;
    }
    memset(block, 0, BLOCK_SIZE*sizeof(unsigned char));
    file_size = getFileSize(file);
    if(0 == file_size) {
        free(block);
        block = NULL;
        LOGI("maybe file not exist");
        ret = ERROR_FILE_NOT_EXIST;
        return ret;
    }
    if(!(fp = fopen(file, "rb"))) {
        LOGI("open file failed");
        free(block);
        block = NULL;
        ret = ERROR_FILE_OPEN_FAILED;
        return ret;
    } else {
        _this->hash_init(_this->ctx);
        while((len = fread(block, 1, BLOCK_SIZE, fp)) > 0) {
            _this->hash_update(_this->ctx, block, len);
           if(p) {
                current_size += len;
                cur_persent = current_size*100/file_size;
                if(cur_persent != persent) {
                    persent = cur_persent;
                    p(persent);
                }
            }
        }
        _this->hash_final(_this->ctx, _this->md);
        *out = _this->md;
        *out_len = _this->mdLen;
    }

    if(block) {
        free(block);
        block = NULL;
    }
    if(fp) {
        fclose(fp);
        fp = NULL;
    }
    ret = ERROR_NO_ERROR;
    return ret;
}

static int m_calcDigestFromMemory(HashDigest* self, char* in, size_t in_len, unsigned char** out, int* out_len) {
    int ret = 0;
    int index = 0;
    int block_num = in_len/BLOCK_SIZE;
    int update_size = 0;
    calcDigest* _this = (calcDigest*)self;
    if(in_len%BLOCK_SIZE) {
        block_num++;
    }
    _this->hash_init(_this->ctx);
    update_size = (((int)in_len - BLOCK_SIZE) < 0) ? in_len : BLOCK_SIZE;
    for(index = 0; index < block_num; index++) {
        _this->hash_update(_this->ctx, in, update_size);
        in += update_size;
        in_len -= update_size;
        update_size = ((int)in_len - BLOCK_SIZE) < 0 ? in_len : BLOCK_SIZE;
    }
    _this->hash_final(_this->ctx, _this->md);
    *out = _this->md;
    *out_len = _this->mdLen;
    ret = ERROR_NO_ERROR;
    return ret;
}

static void m_destroySelf(HashDigest* self) {
    if(((calcDigest*)self)->md) {
        free(((calcDigest*)self)->md);
        ((calcDigest*)self)->md = NULL;
    }
    if(((calcDigest*)self)->ctx) {
        free(((calcDigest*)self)->ctx);
        ((calcDigest*)self)->ctx = NULL;
    }
    if((calcDigest*)self) {
        free((calcDigest*)self);
        self = NULL;
    }
    //LOGI("Destory Self Handler");
}

HashDigest* createDigestHandler(HashType in) {
    if(in == MD5) {
        calcDigest* digestHandler = (calcDigest*)malloc(sizeof(calcDigest));
        digestHandler->md = (unsigned char*)malloc(MD5_DIGEST_SIZE*sizeof(unsigned char));
        digestHandler->ctx = (MD5_CTX*)malloc(sizeof(MD5_CTX));
        digestHandler->mdLen = MD5_DIGEST_SIZE;
        digestHandler->calcDigestFromFile = m_calcDigestFromFile;
        digestHandler->calcDigestFromMemory = m_calcDigestFromMemory;
        digestHandler->destroySelf = m_destroySelf;
        digestHandler->hash_init = md5_init;
        digestHandler->hash_update = md5_update;
        digestHandler->hash_final = md5_final;
        //LOGI("Create MD5 handler success");
        return (HashDigest*)digestHandler;
    } else if (in == SHA1){
        calcDigest* digestHandler = (calcDigest*)malloc(sizeof(calcDigest));
        digestHandler->md = (unsigned char*)malloc(SHA1_DIGEST_SIZE*sizeof(unsigned char));
        digestHandler->ctx = (SHA1_CTX*)malloc(sizeof(SHA1_CTX));
        digestHandler->mdLen = SHA1_DIGEST_SIZE;
        digestHandler->calcDigestFromFile = m_calcDigestFromFile;
        digestHandler->calcDigestFromMemory = m_calcDigestFromMemory;
        digestHandler->destroySelf = m_destroySelf;
        digestHandler->hash_init = sha1_init;
        digestHandler->hash_update = sha1_update;
        digestHandler->hash_final = sha1_final;
        //LOGI("Create SHA1 handler success");
        return (HashDigest*)digestHandler;
    } else if (in == SHA256) {
        calcDigest* digestHandler = (calcDigest*)malloc(sizeof(calcDigest));
        digestHandler->md = (unsigned char*)malloc(SHA256_DIGEST_LENGTH*sizeof(unsigned char));
        digestHandler->ctx = (SHA256_CTX*)malloc(sizeof(SHA256_CTX));
        digestHandler->mdLen = SHA256_DIGEST_LENGTH;
        digestHandler->calcDigestFromFile = m_calcDigestFromFile;
        digestHandler->calcDigestFromMemory = m_calcDigestFromMemory;
        digestHandler->destroySelf = m_destroySelf;
        digestHandler->hash_init = sha256_init;
        digestHandler->hash_update = sha256_update;
        digestHandler->hash_final = sha256_final;
       //LOGI("Create SHA256 handler success");
        return (HashDigest*)digestHandler;
    } else if (in == SHA512) {
        calcDigest* digestHandler = (calcDigest*)malloc(sizeof(calcDigest));
        digestHandler->md = (unsigned char*)malloc(SHA512_DIGEST_SIZE*sizeof(unsigned char));
        digestHandler->ctx = (SHA512_CTX*)malloc(sizeof(SHA512_CTX));
        digestHandler->mdLen = SHA512_DIGEST_SIZE;
        digestHandler->calcDigestFromFile = m_calcDigestFromFile;
        digestHandler->calcDigestFromMemory = m_calcDigestFromMemory;
        digestHandler->destroySelf = m_destroySelf;
        digestHandler->hash_init = sha512_init;
        digestHandler->hash_update = sha512_update;
        digestHandler->hash_final = sha512_final;
       //LOGI("Create SHA512 handler success");
        return (HashDigest*)digestHandler;
    } else {
        LOGI("HashType error");
        return NULL;
    }
}


