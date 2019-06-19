#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <unistd.h>
#include "UDP_api.h"
#define HOST "127.0.0.1"
#define PORT 8888
#define DATA "hello world"

void test()
{
	char buf[128]={0};
	/***********************TEST 1**************************************/
	int sockfd = -1;

        if(-1 == (sockfd=socket(AF_INET, SOCK_DGRAM,0)))
        {
            perror("create socket failed");
            exit (1);
        }
        NetworkAddr *server_addr= (NetworkAddr *)malloc(sizeof(NetworkAddr));
        memset(server_addr,0,sizeof(NetworkAddr));

        strncpy(server_addr->addr,HOST,strlen(HOST));
        server_addr->port = PORT;
        UDP_sendto(sockfd,server_addr,DATA,strlen(DATA),100);
	UDP_recvfrom(sockfd,server_addr,buf,sizeof(buf),100);
	printf("recv from ip:%s,port:%d,data:%s\n",server_addr->addr,server_addr->port,buf);
        free(server_addr);

	UDP_close(sockfd);
	/***********************TEST 2**************************************/
	memset(buf,0,sizeof(buf));
	sockfd = UDP_create(HOST, PORT);
	UDP_send(sockfd,DATA,strlen(DATA),100);
	UDP_recv(sockfd,buf,sizeof(buf),100);
	printf("recv data:%s\n",buf);
	UDP_close(sockfd);
}

int main(int argc,char *argv[])
{
	test();
	return 0;
}
