#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <unistd.h>
#include <fcntl.h>
#include <netinet/tcp.h>
#include <stdlib.h>
#include <netdb.h>
#include <errno.h>
#include "HAL_TCP_linux.h"
static uint64_t tcp_get_time_ms(void)
{
    struct timeval tv = { 0 };
    uint64_t time_ms;
    gettimeofday(&tv, NULL);
    time_ms = tv.tv_sec * 1000 + tv.tv_usec / 1000;
    return time_ms;
}
static uint64_t tcp_time_left(uint64_t t_end, uint64_t t_now)
{
    uint64_t t_left;
    if (t_end > t_now) {
        t_left = t_end - t_now;
    } else {
        t_left = 0;
    }
    return t_left;
}

uintptr_t HAL_TCP_Establish(const char *host, uint16_t port)
{
    struct addrinfo hints;
    struct addrinfo *addrInfoList = NULL;
    struct addrinfo *cur = NULL;
    int fd = 0;
    int rc = 0;
    char service[6];
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET; /* only IPv4 */
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    sprintf(service, "%u", port);
    if ((rc = getaddrinfo(host, service, &hints, &addrInfoList)) != 0) {
        tcp_err("getaddrinfo error");
        return 0;
    }
    for (cur = addrInfoList; cur != NULL; cur = cur->ai_next) {
        if (cur->ai_family != AF_INET) {
            tcp_err("socket type error");
            rc = 0;
            continue;
        }
        fd = socket(cur->ai_family, cur->ai_socktype, cur->ai_protocol);
        if (fd < 0) {
            tcp_err("create socket error");
            rc = 0;
            continue;
        }
        if (connect(fd, cur->ai_addr, cur->ai_addrlen) == 0) {
            rc = fd;
            break;
        }
        close(fd);
        tcp_err("connect error");
        rc = 0;
    }
    if (0 == rc) {
        tcp_err("fail to establish tcp");
    } else {
        tcp_err("success to establish tcp");
    }
    freeaddrinfo(addrInfoList);
    return (uintptr_t)rc;
}


int HAL_TCP_Destroy(uintptr_t fd)
{
    int rc;
    /* Shutdown both send and receive operations. */
    rc = shutdown((int) fd, 2);
    if (0 != rc) {
        tcp_err("shutdown error");
        return -1;
    }
    rc = close((int) fd);
    if (0 != rc) {
        tcp_err("closesocket error");
        return -1;
    }
    return 0;
}

int32_t HAL_TCP_Write(uintptr_t fd, const char *buf, uint32_t len, uint32_t timeout_ms)
{
    int ret;
    uint32_t len_sent;
    uint64_t t_end, t_left;
    fd_set sets;
    t_end = tcp_get_time_ms() + timeout_ms;
    len_sent = 0;
    ret = 1; /* send one time if timeout_ms is value 0 */
    do {
	t_left = tcp_time_left(t_end, tcp_get_time_ms());
        if (0 != t_left) {
            struct timeval timeout;
            FD_ZERO(&sets);
            FD_SET(fd, &sets);
            timeout.tv_sec = t_left / 1000;
            timeout.tv_usec = (t_left % 1000) * 1000;
            ret = select(fd + 1, NULL, &sets, NULL, &timeout);
            if (ret > 0) {
                if (0 == FD_ISSET(fd, &sets)) {
                    tcp_err("Should NOT arrive");
                    /* If timeout in next loop, it will not sent any data */
                    ret = 0;
                    continue;
                }
            } else if (0 == ret) {
                tcp_err("select-write timeout ");
                break;
            } else {
                if (EINTR == errno) {
                    tcp_err("EINTR be caught");
                    continue;
                }
                tcp_err("select-write fail");
                break;
            }
        }
        if (ret > 0) {
            ret = send(fd, buf + len_sent, len - len_sent, 0);
            if (ret > 0) {
                len_sent += ret;
            } else if (0 == ret) {
                tcp_err("No data be sent");
            } else {
                if (EINTR == errno) {
                    tcp_err("EINTR be caught");
                    continue;
                }
                tcp_err("send fail");
                break;
            }
        }
    } while ((len_sent < len) && (tcp_time_left(t_end, tcp_get_time_ms()) > 0));
    return len_sent;
}



int32_t HAL_TCP_Read(uintptr_t fd, char *buf, uint32_t len, uint32_t timeout_ms)
{
    int ret, err_code;
    uint32_t len_recv;
    uint64_t t_end, t_left;
    fd_set sets;
    struct timeval timeout;
    t_end = tcp_get_time_ms() + timeout_ms;
    len_recv = 0;
    err_code = 0;
    do {
        t_left = tcp_time_left(t_end, tcp_get_time_ms());
        if (0 == t_left) {
            break;
        }
        FD_ZERO(&sets);
        FD_SET(fd, &sets);
        timeout.tv_sec = t_left / 1000;
        timeout.tv_usec = (t_left % 1000) * 1000;
        ret = select(fd + 1, &sets, NULL, NULL, &timeout);
        if (ret > 0) {
            ret = recv(fd, buf + len_recv, len - len_recv, 0);
            if (ret > 0) {
                len_recv += ret;
            } else if (0 == ret) {
                tcp_err("connection is closed");
                err_code = -1;
                break;
            } else {
                if (EINTR == errno) {
                    tcp_err("EINTR be caught");
                    continue;
                }
                tcp_err("recv fail");
                err_code = -2;
                break;
            }
        } else if (0 == ret) {
            break;
        } else {
            tcp_err("select-recv fail");
            err_code = -2;
            break;
        }
    } while ((len_recv < len));
    /* priority to return data bytes if any data be received from TCP connection. */
    /* It will get error code on next calling */
    return (0 != len_recv) ? len_recv : err_code;
}


int Send_Message(uintptr_t fd, const char *buf, uint32_t length, uint32_t timeout_ms)
{
    int sendbuflen = 4 + length; 
    unsigned char* sendbuf = (unsigned char *)malloc(sendbuflen);
    if (!sendbuf){
        tcp_err("GWTcpClient::SendMsg error \n");
        return -1;
    }

    memset(sendbuf, 0, sendbuflen);
    //len
    sendbuf[0] = (length>>24)&0xff;
    sendbuf[1] = (length>>16)&0xff;
    sendbuf[2] = (length>>8)&0xff;
    sendbuf[3] = length&0xff;
   
    memcpy(sendbuf + 4, buf, length);
    int ret = HAL_TCP_Write(fd, sendbuf, sendbuflen, timeout_ms);
	
    free(sendbuf);
    if(ret == sendbuflen)
	return 0;
    else
	return -1;
} 

int32_t Read_Message(uintptr_t fd, char *buf, uint32_t len, uint32_t timeout_ms)
{
    unsigned char packetlen_buf[PREREAD_LEN+1] = {0};
    int byte_num = recv(fd, packetlen_buf, PREREAD_LEN, 0);
    if (byte_num <= 0){
    	tcp_err("RECV MSG ERROR !!!\n");
        return -1;
    }

    if (byte_num != PREREAD_LEN){
        tcp_err("read packet length,RECV MSG ERROR !!!\n");
        return -1;
    }
	
    unsigned int packetlen = (packetlen_buf[0]<<24)
                            |(packetlen_buf[1]<<16)
                            |(packetlen_buf[2]<<8)
                            |packetlen_buf[3];
    tcp_info("packet length = %u ......\n", packetlen);
    if(packetlen > len)
    {
 	tcp_info("packetlen great than input length\n");
	packetlen = len;
    }
    int ret = HAL_TCP_Read(fd, buf, packetlen, timeout_ms); 
    return ret;
}
