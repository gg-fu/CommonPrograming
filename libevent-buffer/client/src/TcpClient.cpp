#include <sys/ioctl.h>
#include <netdb.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>

#include "TcpClient.h"

#define PREREAD_LEN 4

CTcpClient::CTcpClient() : m_nSockfd(-1)
{

}

CTcpClient::~CTcpClient()
{
    if (m_nSockfd > 0){
        shutdown(m_nSockfd,SHUT_RD);
        close(m_nSockfd);
    }
}

bool CTcpClient::CloseSocket()
{
    if (m_nSockfd > 0){
        shutdown(m_nSockfd,SHUT_RD);
        close(m_nSockfd);
    }
    return true;
}


/******************************************************************
***   函数功能：SSL初始化
***   描述： 加载openssl算法库，创建加载证书的ssl套接字
***   返回： ssl套接字
*******************************************************************/
SSL *CTcpClient::SSL_init()
{
	SSL_library_init();
    SSL_load_error_strings();
	OpenSSL_add_all_algorithms ();
    SSL_CTX *ssl_ctx = SSL_CTX_new(SSLv23_client_method());
	if (!ssl_ctx)
	{
		printf("SSL_CTX_NEW failed\n");
		return NULL;
	}
	//passwd is supplied to protect the private key,when you want to read key
	SSL_CTX_set_default_passwd_cb_userdata(ssl_ctx,(void *)"test@2017");
	
	//set cipher ,when handshake client will send the cipher list to server
	//SSL_CTX_set_cipher_list(ssl_ctx,"HIGH:MEDIA:LOW:!DH");
	//SSL_CTX_set_cipher_list(g_sslctx,"AES128-SHA");
	
	//set verify ,when recive the server certificate and verify it
	//and verify_cb function will deal the result of verification
	SSL_CTX_set_verify(ssl_ctx, SSL_VERIFY_PEER, NULL);
	
	//sets the maximum depth for the certificate chain verification that shall
	//be allowed for ctx
	SSL_CTX_set_verify_depth(ssl_ctx, 10);
 
	//load the certificate for verify server certificate, CA file usually load
    if(!access(CAFILE,F_OK))
	    SSL_CTX_load_verify_locations(ssl_ctx,CAFILE, NULL);
	
	//load user certificate,this cert will be send to server for server verify
    if(!access(CERT_PATH,F_OK)&&!access(KEY_PATH,F_OK)){
	    if(SSL_CTX_use_certificate_file(ssl_ctx,CERT_PATH,SSL_FILETYPE_PEM) <= 0){
		    printf("certificate file error!\n");
		    return NULL;
	    }
	    //load user private key
	    if(SSL_CTX_use_PrivateKey_file(ssl_ctx,KEY_PATH,SSL_FILETYPE_PEM) <= 0){
		    printf("privatekey file error!\n");
	    }
    }
	if(!SSL_CTX_check_private_key(ssl_ctx)){
		printf("Check private key failed!\n");
		return NULL;
	}

	m_pSSL = SSL_new(ssl_ctx);

	if (!m_pSSL)
	{
		printf("SSL_NEW failed\n");
		return NULL;
	}
	//SSL_CTX_free(ssl_ctx);
	return m_pSSL;
}

/******************************************************************
***   函数功能：连接服务器
***   描述： 通过原始的socket连接服务器
***   参数：domain -- 目标ip地址
*** 		port   -- 目标端口号
***   返回： socketfd
*******************************************************************/
int CTcpClient::ConnectServer(const char* domain, uint16_t port) {
    struct addrinfo hints;
    struct addrinfo *res;
    CloseSocket();
	
    // Specify TCP stream socket (don't mind if ipv4 or ipv6)
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET;//AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    // Specify host and port
    char portString[5];
    sprintf(portString, "%d", port);
    getaddrinfo(domain, portString, &hints, &res);

    // Create the socket
    if((m_nSockfd = socket(res->ai_family, res->ai_socktype, 0)) < 0) 
	{
        return -1;
    }

    // Connect the socket
    if(connect(m_nSockfd, res->ai_addr, res->ai_addrlen) < 0) 
	{
        return -1;
    }
    // Cleanup
    freeaddrinfo(res);

    return m_nSockfd;
}
/******************************************************************
***   函数功能：消息发送
***   描述： 通过libevent的结构体进行消息的发送
***   参数： bev  -- bufferevent 结构体
*** 		 buffer  -- 待发送的消息
***			 length  -- 消息长度
***   返回： 0 -- 消息发送成功
***			 -1 -- 消息发送失败
*******************************************************************/
int CTcpClient::SendMsg(struct bufferevent * bev,unsigned char *buffer,int length)
{
    //
    int sendbuflen = 4 + length;
    unsigned char* sendbuf = new unsigned char[length + 4];
    if (!sendbuf){
        printf("GWTcpClient::SendMsg error \n");
        return -1;
    }

    memset(sendbuf, 0, sendbuflen);
    //len
    sendbuf[0] = (length>>24)&0xff;
    sendbuf[1] = (length>>16)&0xff;
    sendbuf[2] = (length>>8)&0xff;
    sendbuf[3] = length&0xff;

	unsigned int packetlen = (sendbuf[0]<<24)
						 |(sendbuf[1]<<16)
						 |(sendbuf[2]<<8)
						 |sendbuf[3];
    printf("packet length = %u ......\n", packetlen);
    //body
    memcpy(sendbuf + 4, buffer, length);
	int ret = 0;
	int count = 0;
    while (-1 == (ret = bufferevent_write(bev, sendbuf, sendbuflen)))
	{
		count++;
		if (count >3) break;
	}

	free(sendbuf);
    return ret;
}

/*int CTcpClient::FullWrite(SSL *ssl,unsigned char *buffer,int length)
{
    int bytes_left;
    int written_bytes;

    char *ptr = (char*)buffer;
    bytes_left=length;
    while(bytes_left>0)
    {
		//开始写
		written_bytes = SSL_write(ssl,ptr,bytes_left);
		if(written_bytes <= 0) //出错了
		{
			if(errno == EINTR) // 中断错误 我们继续写
				written_bytes = 0;
			else             // 其他错误 没有办法,只好撤退了
				return(-1);
		}

		bytes_left-=written_bytes;
		ptr+=written_bytes;     //从剩下的地方继续写 
    }
    return(0);
}*/
/******************************************************************
***   函数功能：保证读数据完整
***   描述： 通过循环读的方式，处理丢包、分包的情况
***   参数： bev --  bufferevent 结构体
*** 		 buffer   --  缓存数据的数组
***          length   --  需要读取的数据长度
***   返回： 0  -- 成功
***          else  -- 失败
*******************************************************************/
int CTcpClient::FullRead(struct bufferevent* bev,char *buffer,int length)
{
    int bytes_left = 0;
    int bytes_read = 0;
    char *ptr = (char *)buffer;

    bytes_left = length;
    while (bytes_left > 0)
    {
		bytes_read = bufferevent_read(bev,ptr,bytes_left);
		if(bytes_read<0)
		{
			if(errno==EINTR)
				bytes_read=0;
			else
				return(-1);
		}
		else if(bytes_read==0)
			break;
		bytes_left-=bytes_read;
		ptr+=bytes_read;
    }
    return(length-bytes_left);
}

/******************************************************************
***   函数功能：接收服务器下发的数据
***   描述： 接收数据函数
***   参数： bev -- bufferevent 结构体
*** 		 recv_msg  -- 存储数据的数组
***   返回： 0 -- 成功
***  		 else  -- 失败
*******************************************************************/
int CTcpClient::RecvDataFromServer(struct bufferevent* bev, char* recv_msg)
{
	printf("recvdata\n");
    int byte_num = 0;
    //read packet len
	unsigned char packetlen_buf[4] = {0};
	byte_num = bufferevent_read(bev, packetlen_buf, PREREAD_LEN);
	if (byte_num <= 0){
		printf("RECV MSG ERROR !!!\n");
		return -1;
	}

	if (byte_num != PREREAD_LEN){
		printf("read packet length,RECV MSG ERROR !!!\n");
		return -1;
	}

	printf("read packet length : %d\n",byte_num);

	unsigned int packetlen = (packetlen_buf[0]<<24)
							 |(packetlen_buf[1]<<16)
							 |(packetlen_buf[2]<<8)
							 |packetlen_buf[3];
	printf("packet length = %u ......\n", packetlen);

	byte_num = FullRead(bev, recv_msg, packetlen);
	if (byte_num <= 0){
		printf("read body, RECV MSG ERROR !!!\n");
		return -1;
	}
    return byte_num;
}





