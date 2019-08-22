#ifndef _HAL_TCP_LINUX_H
#define _HAL_TCP_LINUX_H

#if defined(__cplusplus)
extern "C" {
#endif
#include <stdint.h>
#include "iot_log.h"
#define PREREAD_LEN   4
#define tcp_warning(...) 	log_warning("tcp",__VA_ARGS__)
#define tcp_info(...) 	log_info("tcp",__VA_ARGS__)
#define tcp_err(...) 		log_err("tcp",__VA_ARGS__)
#define tcp_debug(...) 	log_debug("tcp",__VA_ARGS__)

uintptr_t HAL_TCP_Establish(const char *host, uint16_t port);
int HAL_TCP_Destroy(uintptr_t fd);
int32_t HAL_TCP_Write(uintptr_t fd, const char *buf, uint32_t len, uint32_t timeout_ms);
int32_t HAL_TCP_Read(uintptr_t fd, char *buf, uint32_t len, uint32_t timeout_ms);
int32_t Read_Message(uintptr_t fd, char *buf, uint32_t len, uint32_t timeout_ms);
int Send_Message(uintptr_t fd, const char *buf, uint32_t len, uint32_t timeout_ms);

#if defined(__cplusplus)
}
#endif

#endif
