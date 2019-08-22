#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "TCP_api.h"
#include "iot_log.h"
#define DATA "hello world"

#define utils_warning(...) log_warning("tcp",__VA_ARGS__)
#define utils_info(...) log_info("tcp",__VA_ARGS__)
#define utils_err(...) log_err("tcp",__VA_ARGS__)
#define utils_debug(...) log_debug("tcp",__VA_ARGS__)
int main(int argc,char *argv[])
{
    int fd = -1;
    LITE_openlog("TCP-CLIENT");
    LITE_set_loglevel(LOG_DEBUG_LEVEL);
    utils_info("start tcp client\n");
    if(argc != 5)
    {
	utils_err("please input -i IP -p port\n");
	return -1;
    }
    fd = TCP_Establish(argv[2], atoi(argv[4]));
    while(fd == 0){
	utils_warning("reconnect \n");
	fd = TCP_Establish(argv[2], atoi(argv[4]));
	sleep(5);
    }
    utils_debug("fd:%d\n",fd);
    int ret = TCP_Write(fd,DATA,strlen(DATA), 100);
    char buf[125]={0};
    while(1){
	memset(buf,0,sizeof(buf));
	ret = TCP_Read(fd, buf, strlen(DATA), 100);
	if(ret == -1)
	{
	    utils_warning("connect close\n");
	    break;
	}
	else if(ret > 0)
	   utils_info("recv buf:%s\n",buf);
	else{
	    utils_warning("recv data failed\n");
	    continue;
	}
    }
    TCP_Destroy(fd);
    LITE_closelog();
    return 0;
}

