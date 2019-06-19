#ifndef _UDP_API_H
#define _UDP_API_H


#define _IN_
#define _OU_
#define NETWORK_ADDR_LEN        (16)   
typedef struct _network_addr_t {
    unsigned char
    addr[NETWORK_ADDR_LEN];         
    unsigned short  port;           
} NetworkAddr;


intptr_t   UDP_create(char *host, unsigned short port);
void UDP_close(intptr_t p_socket);
int UDP_write(intptr_t p_socket,const unsigned char *p_data,unsigned int datalen);
int UDP_read(intptr_t p_socket,unsigned char *p_data,unsigned int datalen);
int UDP_readTimeout(intptr_t p_socket,unsigned char *p_data,unsigned int datalen,unsigned int timeout);
intptr_t UDP_create_without_connect(_IN_ const char *host, _IN_ unsigned short port);
int UDP_connect(_IN_ intptr_t sockfd,_IN_ const char *host,_IN_ unsigned short port);
int UDP_close_without_connect(_IN_ intptr_t sockfd);
int UDP_joinmulticast(_IN_ intptr_t sockfd  ,  _IN_ char *p_group);
int UDP_recv(_IN_ intptr_t sockfd,_OU_ unsigned char *p_data,_IN_ unsigned int datalen,_IN_ unsigned int timeout_ms);
int UDP_recvfrom(_IN_ intptr_t sockfd,_OU_ NetworkAddr *p_remote,_OU_ unsigned char *p_data,
					_IN_ unsigned int datalen,_IN_ unsigned int timeout_ms  );

int UDP_send(  _IN_ intptr_t sockfd,_IN_ const unsigned char *p_data,
			_IN_ unsigned int datalen,_IN_ unsigned int timeout_ms);

int UDP_sendto(  _IN_ intptr_t sockfd,_IN_ const NetworkAddr *p_remote,
				_IN_ const unsigned char *p_data,_IN_ unsigned int datalen,_IN_ unsigned int timeout_ms);


#endif
