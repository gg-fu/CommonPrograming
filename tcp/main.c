#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "TCP_api.h"
#define DATA "hello world"

int main(int argc,char *argv[])
{
    int fd = -1;
    if(argc != 5)
    {
	printf("please input -i IP -p port\n");
	return -1;
    }
    fd = TCP_Establish(argv[2], atoi(argv[4]));
    while(fd == 0){
	printf("reconnect \n");
	fd = TCP_Establish(argv[2], atoi(argv[4]));
	sleep(5);
    }
    printf("fd:%d\n",fd);
    int ret = TCP_Write(fd,DATA,strlen(DATA), 100);
    char buf[125]={0};
    while(1){
	memset(buf,0,sizeof(buf));
	ret = TCP_Read(fd, buf, strlen(DATA), 100);
	if(ret == -1)
	{
	    printf("connect close\n");
	    break;
	}
	else if(ret > 0)
	   printf("recv buf:%s\n",buf);
	else{
	    printf("recv data failed\n");
	    continue;
	}
    }
    TCP_Destroy(fd);
    return 0;
}

