#ifndef __AES_H
#define __AES_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <openssl/aes.h>
#include <openssl/evp.h>
#include <openssl/ssl.h>

#ifdef __cplusplus
extern "C" {
#endif
    typedef unsigned char uchar;
    char *Hex2Int(const char in[],char out[],int size);
    char *Int2Hex(char in[],char out[],int size);
    int aes_encrypt_ecb(const uchar *aeskey,const uchar *data,char *encrypt_result,int size);
    int aes_decrypt_ebc(const uchar *aeskey,const char *data,int length,uchar *decrypt_result,int size);
    int aes_encrypt_cbc(uchar *in_buf, int in_len, uchar *key,uchar *iv, uchar *out_buf,int out_len);
    int aes_decrypt_cbc(uchar* in_buf, int in_len, uchar *key,uchar *iv,uchar *out_buf,int out_len);
#ifdef __cplusplus
}
#endif

#endif
 
 

