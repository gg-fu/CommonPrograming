#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <fstream>
#include <event.h>
#include <netinet/in.h>  
#include <arpa/inet.h>  
#include <openssl/ssl.h>
#include <event2/bufferevent_ssl.h>
#include "TcpClient.h"
#include "AES.h"
#include "queue.h"
#define IP "127.0.0.1"
#define PORT 6000

queue *q;
CTcpClient TcpClient;
static int g_ntime = 10;
struct bufferevent *g_sBev = NULL;
void tcp_reconnect(struct bufferevent* bev,struct bufferevent *socketEvent,bufferevent_data_cb Read);

// 读服务端发来的数据 
void ReadMes(struct bufferevent* bev, void* arg)
{
    char msg[1024] = {0};
    unsigned char buf[1024]={0};
    int ret = TcpClient.RecvDataFromServer(bev,msg);
    if(ret)
    {
	//aes_decrypt_ebc((unsigned char *)"aeskey",msg,ret,buf,1024);
	enqueue(q,msg);
	printf("recv data is :%s\n",msg);
    }
}

// 连接断开或者出错回调 
void event_callback(struct bufferevent *bev, short event, void *arg)
{
	printf("event_callback\n");
	if( BEV_EVENT_CONNECTED == event )
    {
		printf("connect ok\n");
		
        bufferevent_enable(bev, EV_READ);
    }
	else if (event & (EVBUFFER_EOF|EVBUFFER_ERROR))
	{
		if (event & BEV_EVENT_ERROR)
        {
            int err = bufferevent_socket_get_dns_error(bev);
            printf("DNS error:%s\n",evutil_gai_strerror(err));
        }
		printf("connection closed\n");
		sleep(3);
		bufferevent_data_cb read = (bufferevent_data_cb)arg;
		tcp_reconnect(bev,g_sBev,read);
	}
	else if(event & BEV_EVENT_TIMEOUT)
    {
		printf("心跳................\n");
        // 重新注册可读事件 
        bufferevent_enable(bev, EV_READ);
    }
  

}


void Write(struct bufferevent* bev, void* arg)
{
	printf("write function\n");
}
struct bufferevent *tcp_connect(struct event_base * base,
								struct bufferevent *socketEvent, 
								bufferevent_data_cb Read)
{
	printf("in function tcp_connect\n");
	socketEvent = NULL;
	SSL *ssl = TcpClient.SSL_init();
	struct bufferevent* bev = NULL;
	if (ssl == NULL)
	{
		printf("ssl is null\n");
	    bev = bufferevent_socket_new(base,-1,BEV_OPT_CLOSE_ON_FREE);
		return NULL;
	}
    else{
        bev = bufferevent_openssl_socket_new(base, -1, ssl,
															BUFFEREVENT_SSL_CONNECTING ,
															BEV_OPT_CLOSE_ON_FREE);
    }
    if(bev != NULL)
    {

		socketEvent = bev;
    }
    if(bufferevent_socket_connect_hostname(bev, NULL, AF_INET, IP, PORT) == -1)
    {
        printf("bufferevent_socket_connect_hostname failed!\n");
        return NULL;
    }
    struct timeval READ_TIMEOUT = {10, 0};
    bufferevent_setcb(bev, Read, Write, event_callback, (void *)Read);
    bufferevent_set_timeouts(bev, &READ_TIMEOUT, NULL);
	
    bufferevent_enable(bev, EV_READ|EV_WRITE);
	
    return socketEvent;
}

void tcp_reconnect(	struct bufferevent* bev,
					struct bufferevent *socketEvent,
					bufferevent_data_cb Read)
{
    if(bev == NULL)
    {
        printf("null bufferevent!\n");
        return;
    }
    struct event_base *base = bufferevent_get_base(bev);
    if(base == NULL)
    {
        printf("null event_base!\n");
        return;
    }
    bufferevent_free(bev);

    printf("try reconnect...\n");
    if (tcp_connect(base,socketEvent,Read)==NULL)
	{
		printf(" reconnect failed!\n");
	}
}
static void signal_cb(evutil_socket_t sig, short events, void *user_data)
{
    struct event_base *base = (struct event_base *)user_data;
    struct timeval delay = { 2, 0 };//设置延迟2s

    printf("Caught an interrupt signal; exiting cleanly in two seconds.\n");
    //如果有事件,处理完当前事件退出 
    //event_base_loopexit(base, &delay);
    event_base_loopexit(base, NULL);
}

void *connect_event(void *arg)
{
	struct event_base * pEventBase = NULL;
	
    // 创建事件驱动句柄 
    pEventBase = event_base_new();
	
	if ((g_sBev =tcp_connect(pEventBase,g_sBev,ReadMes)) == NULL)
	{
		printf("tcp_connect failed\n");
	}
	 //定义了 SIGINT信号事件,设置回调位signal_cb 
    struct event *signal_event = evsignal_new(pEventBase, SIGINT, signal_cb, (void *)pEventBase);
    if (!signal_event || event_add(signal_event, NULL)<0) 
	{
        fprintf(stderr, "Could not create/add a signal event!\n");
    }
	
	// 开始事件循环 
    event_base_dispatch(pEventBase);

    // 事件循环结束 资源清理 
    event_base_free(pEventBase);
}

void *read_queue(void *arg)
{
    void *data;
    while (1)  
    {  
        dequeue(q,&data);  
        printf("--->%s \n", (char *)data);  
    }  
    return NULL;
}

// 创建一个新线程，在新线程里初始化libevent读事件的相关设置，并开启event_base_dispatch
void thread_worker(void *(*func)(void *),void *arg)
{
    int ret;
    pthread_t thread;
    ret = pthread_create(&thread,NULL,func,arg);
    if(ret != 0)
    {
	perror("pthread_create failed\n");
	return;
    }
    
}
int main(int argc, char *argv[])
{
    q = init_queue(QUEUE_SIZE);

    thread_worker(read_queue,NULL);
    thread_worker(connect_event,NULL);
    while(1);
    return 0;
}
