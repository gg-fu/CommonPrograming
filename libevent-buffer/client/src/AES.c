#include "AES.h"
/* 将十六进制数(两位)转化为待解密的数据 */
char *Hex2Int(const char in[],char out[],int size)
{
	int i,Shi=0,Ge=0,number=0,index=0;
	for (i=0;i<size;index++,i+=2){
		if (in[i] >= 'a') 
			Shi = in[i] -'a' +10;
		else
			Shi = in[i] -'0';
		
		if (in[i+1] >= 'a')
			Ge = in[i+1] -'a' +10;
		else
			Ge = in[i+1] -'0';
		
		number = Shi*16+Ge;
		if (number >= 128)
			out[index] = number-256;
		else
			out[index] = number;
	}
	out[index]='\0';
	return out;
}
 
/* 将十进制数(小于256)转化为十六进制数 */
char *Int2Hex(char in[],char out[],int size)
{
    int j,aa=0,Shi=0,Ge=0,i=0;
    for (j=0;j<size;j++,i++) {
		if ( (int)in[j] < 0 )
			aa = 256+(int)in[j];
		else
			aa = (int)in[j];
		if (aa < 16) {
			out[i] = '0';
			if (aa < 10)       
				out[++i] = aa +'0';
			else
				out[++i] = aa -10 +'a';
		}
		else {
			Shi = aa/16,Ge = aa%16;
			if (Shi < 10)
				out[i] = Shi +'0';
			else
				out[i] = Shi - 10 +'a';
			
			if (Ge < 10)
				out[++i] = Ge +'0';
			else
				out[++i] = Ge -10 +'a';
		}
	}
	out[2*j]='\0';
    return out;
}
/*AES 128 ecb模式 */
int aes_encrypt_ecb(const uchar *aeskey,const uchar *data,char *encrypt_result,int size)
{
	unsigned char buffer[size+1];
	int length = strlen((char *)data);
	int num,outl=0,outltmp=0;
	
	EVP_CIPHER_CTX ctx;
    EVP_CIPHER_CTX_init(&ctx);
    int rv= EVP_EncryptInit_ex(&ctx,EVP_aes_128_ecb(),NULL,aeskey,NULL);
	if(size>=2*length)
	{
		if(length%AES_BLOCK_SIZE==0&&length/AES_BLOCK_SIZE!=0)
			num=length/AES_BLOCK_SIZE;
		else	
			num=length/AES_BLOCK_SIZE+1;
		rv=EVP_EncryptUpdate(&ctx,buffer,&outl,data,length);
		rv=EVP_EncryptFinal_ex(&ctx,buffer+outl,&outltmp);
		char *q = Int2Hex((char *)buffer,encrypt_result,num*AES_BLOCK_SIZE);
		if(q==NULL){
			EVP_CIPHER_CTX_cleanup(&ctx);
			return -1;
		}
	}
	else{
		EVP_CIPHER_CTX_cleanup(&ctx);
		printf("the output array space is too small\n");
		return -1;
	}
	EVP_CIPHER_CTX_cleanup(&ctx);
	return 0;
}
int aes_decrypt_ebc(const uchar *aeskey,const char *data,int length,uchar *decrypt_result,int size)
{
  	int outl=0,i;
	int outltmp=0;
	uchar buf[2*size+1];
	
	EVP_CIPHER_CTX ctx;
	EVP_CIPHER_CTX_init(&ctx);
	int rv= EVP_DecryptInit_ex(&ctx,EVP_aes_128_ecb(),NULL,aeskey,NULL);
	if(size*2>=length)
	{
		char *p = Hex2Int(data,(char *)buf,length);
		if(p==NULL){
			EVP_CIPHER_CTX_cleanup(&ctx);
			return -1;
		}
		rv=EVP_DecryptUpdate(&ctx,decrypt_result,&outl,buf,length/2);
		rv=EVP_DecryptFinal_ex(&ctx,decrypt_result+outl,&outltmp);
		int decrypt_len = strlen((char *)decrypt_result);
		if((outltmp+outl)&&decrypt_len%1024!=0)
			decrypt_result[outltmp+outl]='\0';
	}
	else{
		EVP_CIPHER_CTX_cleanup(&ctx);
		fprintf(stderr,"Array size is too samll!\n");
		return -1;
	}
	EVP_CIPHER_CTX_cleanup(&ctx);
	return 0;
	
}
/* AES 128 cbc模式 */
int aes_encrypt_cbc(uchar *in_buf, int in_len, uchar *key,uchar *iv, uchar *out_buf,int out_len)
{
    EVP_CIPHER_CTX ctx;
 
    int isSuccess = 0;
    unsigned char in[AES_BLOCK_SIZE];  
	char tmp[2*in_len+AES_BLOCK_SIZE];
    int outl = 0;   
    int outl_total = 0; 
    EVP_CIPHER_CTX_init(&ctx);  
    isSuccess = EVP_EncryptInit_ex(&ctx, EVP_aes_128_cbc(), NULL, key, iv);
	if(!isSuccess)  
	{  
		printf("EVP_DecryptInit_ex() failed");  
		EVP_CIPHER_CTX_cleanup(&ctx);  
		return 0; 
	} 
	if(out_len < 2*(in_len+in_len%AES_BLOCK_SIZE))
	{
		printf("the size of output size is too samll\n ");
		return 0;
	}
	//循环加密，每次加密AES_BLOCK_SIZE字节大小
    while(in_len >=AES_BLOCK_SIZE)
    {
        memcpy(in, in_buf, AES_BLOCK_SIZE);  
        in_len -= AES_BLOCK_SIZE;  
        in_buf += AES_BLOCK_SIZE;  
        isSuccess = EVP_EncryptUpdate(&ctx, out_buf + outl_total, &outl, in, AES_BLOCK_SIZE);  
		if(!isSuccess)  
		{  
			printf("EVP_EncryptUpdate() failed");  
			EVP_CIPHER_CTX_cleanup(&ctx);  
			return 0;  
		}  
		outl_total += outl;  
	}
	//加密剩余的数据
	if(in_len > 0)  
	{  
		memcpy(in, in_buf, in_len); 
		isSuccess = EVP_EncryptUpdate(&ctx,out_buf + outl_total, &outl, in, in_len);  
		outl_total += outl;  
 
		isSuccess = EVP_EncryptFinal_ex(&ctx,out_buf + outl_total,&outl);  
		if(!isSuccess)  
		{  
			printf("EVP_EncryptFinal_ex() failed");  
			EVP_CIPHER_CTX_cleanup(&ctx);  
			return 0;  
		}  
		outl_total += outl;  
	} 
	//将加密数据转化成十六进制数
	Int2Hex((char *)out_buf,tmp,outl_total);
	memcpy(out_buf,tmp,2*outl_total);   
    EVP_CIPHER_CTX_cleanup(&ctx); 
    return 2*outl_total;
}
 
 
int aes_decrypt_cbc(uchar* in_buf, int in_len, uchar *key,uchar *iv,uchar *out_buf,int out_len)
{
	unsigned char in[AES_BLOCK_SIZE]; 
	char *tmp = (char *)malloc(in_len);
	int outl = 0;  
	int outl_total = 0;  
	int isSuccess;  
 
	EVP_CIPHER_CTX ctx;     
	//初始化ctx，加密算法初始化  
	EVP_CIPHER_CTX_init(&ctx);  
	isSuccess = EVP_DecryptInit_ex(&ctx,EVP_aes_128_cbc(),NULL,key,iv);  
	if(!isSuccess)  
	{  
		printf("EVP_DecryptInit_ex() failed");  
		EVP_CIPHER_CTX_cleanup(&ctx);  
		return 0; 
	}  
	if(out_len < in_len/2)
	{
		printf("the size of output array is too small\n");
		return 0;
	}
	//将十六进制数转化为带符号的十进制数
	Hex2Int((char *)in_buf,tmp,2*in_len);
	//循环解密，每次解密AES_BLOCK_SIZE大小  
	while(in_len >AES_BLOCK_SIZE)  
	{  
		memcpy(in, tmp, AES_BLOCK_SIZE);  
		in_len -= AES_BLOCK_SIZE;  
		tmp += AES_BLOCK_SIZE;  
 
		isSuccess = EVP_DecryptUpdate(&ctx, out_buf + outl_total, &outl, in, AES_BLOCK_SIZE);  
		if(!isSuccess)  
		{  
			printf("EVP_DecryptUpdate() failed");  
			EVP_CIPHER_CTX_cleanup(&ctx);  
			return 0;  
		}  
		outl_total += outl;  
	}
 
	if(in_len > 0)  
	{  
		memcpy(in, tmp, in_len);  
		isSuccess = EVP_DecryptUpdate(&ctx, out_buf + outl_total, &outl, in, in_len);  
		outl_total += outl;  
	} 
 
	/*解密数据块不为16整数倍时执行 */
	if(in_len % AES_BLOCK_SIZE != 0)  
	{  
		isSuccess = EVP_DecryptFinal_ex(&ctx, out_buf + outl_total, &outl);  
		if(!isSuccess)  
		{  
			printf("EVP_DecryptFinal_ex() failed\n");  
			EVP_CIPHER_CTX_cleanup(&ctx);  
			return 0;  
		}
		outl_total += outl;  
	}  
	EVP_CIPHER_CTX_cleanup(&ctx);
    free(tmp);
	return outl_total;  
}
 

