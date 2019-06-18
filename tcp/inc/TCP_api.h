#ifndef _TCP_API_H
#define _TCP_API_H
#include <stdint.h>


uintptr_t TCP_Establish(const char *host, uint16_t port);
int TCP_Destroy(uintptr_t fd);
int32_t TCP_Write(uintptr_t fd, const char *buf, uint32_t len, uint32_t timeout_ms);
int32_t TCP_Read(uintptr_t fd, char *buf, uint32_t len, uint32_t timeout_ms);
int32_t Read_Message(uintptr_t fd, char *buf, uint32_t len, uint32_t timeout_ms);
int Send_Message(uintptr_t fd, const char *buf, uint32_t len, uint32_t timeout_ms);

#endif
