#ifndef GWTCPCLIENT_H
#define GWTCPCLIENT_H

#include <stdint.h>
#include <event.h>
#include <openssl/crypto.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <openssl/rand.h>

#define CERT_PATH "certificate/clientcert.pem"
#define KEY_PATH  "certificate/Clientkey.pem"
#define CAFILE "certificate/cacert.pem"

class CTcpClient {
public:
    CTcpClient();
    ~CTcpClient();

    bool CloseSocket();
    int ConnectServer(const char* domain, uint16_t port);

    int SendMsg(struct bufferevent * bev,unsigned char *buffer,int length);
	
	//int FullWrite(SSL *ssl,unsigned char *buffer,int length);
	
	int FullRead(struct bufferevent * bev,char *buffer,int length);
	
	int RecvDataFromServer(struct bufferevent * bev,char* recv_msg);
	
	
	SSL *SSL_init();
	
protected:
    int m_nSockfd;
	SSL *m_pSSL;

};

#endif 

