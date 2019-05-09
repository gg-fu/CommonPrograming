/*******************************************************************************
 * Copyright (c) 2012, 2017 IBM Corp.
 *
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * and Eclipse Distribution License v1.0 which accompany this distribution. 
 *
 * The Eclipse Public License is available at 
 *   http://www.eclipse.org/legal/epl-v10.html
 * and the Eclipse Distribution License is available at 
 *   http://www.eclipse.org/org/documents/edl-v10.php.
 *
 * Contributors:
 *    Ian Craggs - initial contribution
 *******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <OsWrapper.h>
#include "Login.h"
#include "PubSub.h"
#include "AES.h"
#define SIZE 1024
#define CLIENTID    "APP_Client1"

#define prikey "prikey.pem"
#define Head "https://127.0.0.1:8888/cloud/Register?mac=00:0c:29:db:07:3f&version=1&versionName=123&sign="
#define PAYLOAD "where am I from?who am I?where am I going?"
			
char *const topics[]={"MQTT Examples","Download/#"};				 
DATA *data = NULL;
int count =0;


int msgarrvd(void *context, char *topicName, int topicLen, MQTTAsync_message *message)
{

    char* payloadptr;
	char decrypt_result[2*SIZE+1]={0};
	MQTTAsync client = (MQTTAsync)context;
	
	char iv[16];
	memset(iv,1,sizeof(iv));
    printf("Message arrived\n");
    printf("     topic: %s\n", topicName);
    printf("   message: ");

	/************************此处注意！！！***************************
	 *strlen(message->payload)不等于message->payloadlen **************
	 *原因是：云端传过来的数据后面添加了尾巴，导致比真实数据要长一些 */
    payloadptr = message->payload;
	aes_decrypt_cbc(payloadptr,message->payloadlen,"12345678",iv,decrypt_result,sizeof(decrypt_result));
	printf("dec:%lu,%s\n",strlen(decrypt_result),decrypt_result);
	
    MQTTAsync_freeMessage(&message);
    MQTTAsync_free(topicName);
    return 1;
}                                            

int main(int argc, char* argv[])
{
	MQTTAsync client;
	uchar buf[2048]={0};
	//（1）登录（2）连接（3）订阅（4）发布（5）加密（6）解密
	int ret = Register(buf,prikey,Head,sizeof(buf));
	if(ret == -1)
	{
		printf("请求失败\n");
		return -1;
	}
	printf("buf:%s\n",buf);
	data=parseJSON(strstr(buf,"{"));
	if(data == NULL )
	{
		printf("注册失败\n");
		return -1;
	}
	ret = Connect(&client,data->MQTTAddress,CLIENTID,data->Token,data->MQTTPass,msgarrvd);
	//int ret = Connect(&client,"127.0.0.1",CLIENTID,"username","password",msgarrvd);
	if(ret == -1)
	{
		printf("连接MQTT服务器失败\n");
		return -1;
	}
	char * const back[] = {"back"};
	ret = Subscribe_topic(client,back,1);
	if(ret)
	{
		printf("订阅主题失败\n");
		return -1;
	}
	
	char ch;
	char *key = "12345678";
	char encrypt_result[1024];
	char decrypt_result[1024];
	char iv[16];
	memset(iv,1,sizeof(iv));
	while(1)
	{
		ch=getchar();
		if(ch=='s'||ch=='S')
		{
			memset(encrypt_result,0,sizeof(encrypt_result));
			int ret = aes_encrypt_cbc(PAYLOAD,sizeof(PAYLOAD),key,iv,encrypt_result,sizeof(encrypt_result));
			if(ret == -1)
				continue;
			printf("%lu,%s\n",strlen(encrypt_result),encrypt_result);
			Publish_Message(client,"MQTT Examples",encrypt_result,strlen(encrypt_result));
			count++;
		}
		else if(ch=='q'||ch=='Q')
			break;
	}
	printf("count:%d\n",count);
	disConnect(client,topics,2);
	return 0;
}
