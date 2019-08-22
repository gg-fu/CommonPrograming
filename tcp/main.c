#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "utils_net.h"
#define DATA "hello world"
#define HOST "192.168.1.168"
#define PORT 8888
static utils_network_t pNetwork = {0};

static int ConnectToServer()
{
	int ret;
	ret = pNetwork.connect(&pNetwork);
	if( ret == -1){
		tcp_err("ConnectToServer failed\n");
		return -1;
	}
	return 0;	
}

int main(int argc,char *argv[])
{
    	int fd = -1;
	char buff[4096] = {0};
    	LITE_openlog("TCP-CLIENT");
    	LITE_set_loglevel(LOG_DEBUG_LEVEL);
    	tcp_info("start tcp client\n");
	
	int ret = iotx_net_init(&pNetwork , HOST , PORT , NULL, NULL);
	if(ret == -1){
		tcp_err("iotx_net_init failed\n");
		goto exit;
	}
	ret = ConnectToServer();
	if(ret == -1){
		tcp_err("iotx_net_connect failed\n");
		goto exit;
	}
	
	while(1){
		memset(buff,0,sizeof(buff));
		ret = pNetwork.read(&pNetwork,buff,sizeof(buff),200);
		if(ret == 0){
			ret = pNetwork.write(&pNetwork,buff,sizeof(buff),200);
		}	
		else
			sleep(1);	
	}
exit:
	pNetwork.disconnect(&pNetwork);
    	LITE_closelog();
    	return 0;
}

