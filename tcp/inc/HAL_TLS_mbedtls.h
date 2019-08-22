#ifndef _HAL_TLS_MEBDTLS_H
#define _HAL_TLS_MBEDTLS_H


#if defined(__cplusplus)
extern "C" {
#endif
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdarg.h>
#include <inttypes.h>
#include "iot_log.h"


#define hal_warning(...) 	log_warning("tcp_ssl",__VA_ARGS__)
#define hal_info(...) 		log_info("tcp_ssl",__VA_ARGS__)
#define hal_err(...) 		log_err("tcp_ssl",__VA_ARGS__)
#define hal_debug(...) 		log_debug("tcp_ssl",__VA_ARGS__)

#ifndef CONFIG_HTTP_AUTH_TIMEOUT
    #define CONFIG_HTTP_AUTH_TIMEOUT        (5 * 1000)
#endif
#ifndef CONFIG_MID_HTTP_TIMEOUT
    #define CONFIG_MID_HTTP_TIMEOUT         (5 * 1000)
#endif
#ifndef CONFIG_GUIDER_AUTH_TIMEOUT
    #define CONFIG_GUIDER_AUTH_TIMEOUT      (10 * 1000)
#endif
#ifndef CONFIG_COAP_AUTH_TIMEOUT
    #define CONFIG_COAP_AUTH_TIMEOUT        (10 * 1000)
#endif
#ifndef CONFIG_MQTT_TX_MAXLEN
    #define CONFIG_MQTT_TX_MAXLEN           (1024)
#endif
#ifndef CONFIG_MQTT_RX_MAXLEN
    #define CONFIG_MQTT_RX_MAXLEN           (1024)
#endif
#ifndef CONFIG_MQTT_SUBTOPIC_MAXNUM
    #define CONFIG_MQTT_SUBTOPIC_MAXNUM     (100)
#endif
#ifndef CONFIG_SDK_THREAD_COST
    #define CONFIG_SDK_THREAD_COST          (0)
#endif
#ifndef CONFIG_MBEDTLS_DEBUG_LEVEL
    #define CONFIG_MBEDTLS_DEBUG_LEVEL      (0)
#endif


#define MBEDTLS_ERR_NET_SOCKET_FAILED                     -0x0042  /**< Failed to open a socket. */
#define MBEDTLS_ERR_NET_CONNECT_FAILED                    -0x0044  /**< The connection to the given server / port failed. */
#define MBEDTLS_ERR_NET_BIND_FAILED                       -0x0046  /**< Binding of the socket failed. */
#define MBEDTLS_ERR_NET_LISTEN_FAILED                     -0x0048  /**< Could not listen on the socket. */
#define MBEDTLS_ERR_NET_ACCEPT_FAILED                     -0x004A  /**< Could not accept the incoming connection. */
#define MBEDTLS_ERR_NET_RECV_FAILED                       -0x004C  /**< Reading information from the socket failed. */
#define MBEDTLS_ERR_NET_SEND_FAILED                       -0x004E  /**< Sending information through the socket failed. */
#define MBEDTLS_ERR_NET_CONN_RESET                        -0x0050  /**< Connection was reset by peer. */
#define MBEDTLS_ERR_NET_UNKNOWN_HOST                      -0x0052  /**< Failed to get an IP address for the given hostname. */
#define MBEDTLS_ERR_NET_BUFFER_TOO_SMALL                  -0x0043  /**< Buffer is too small to hold the data. */
#define MBEDTLS_ERR_NET_INVALID_CONTEXT                   -0x0045  /**< The context is invalid, eg because it was free()ed. */
#define MBEDTLS_NET_LISTEN_BACKLOG         10 /**< The backlog that listen() should use. */
#define MBEDTLS_NET_PROTO_TCP 0 /**< The TCP transport protocol */
#define MBEDTLS_NET_PROTO_UDP 1 /**< The UDP transport protocol */
/**
 * @brief Establish a SSL connection.
 *
 * @param [in] host: @n Specify the hostname(IP) of the SSL server
 * @param [in] port: @n Specify the SSL port of SSL server
 * @param [in] ca_crt @n Specify the root certificate which is PEM format.
 * @param [in] ca_crt_len @n Length of root certificate, in bytes.
 * @return SSL handle.
 * @see None.
 * @note None.
 */
uintptr_t HAL_SSL_Establish(
            const char *host,
            uint16_t port,
            const char *ca_crt,
            size_t ca_crt_len);

/**
 * @brief Destroy the specific SSL connection.
 *
 * @param[in] handle: @n Handle of the specific connection.
 *
 * @return The result of destroy ssl
 *
 * @retval < 0 : Fail.
 * @retval   0 : Success.
 */
int32_t HAL_SSL_Destroy( uintptr_t handle);

/**
 * @brief Write data into the specific SSL connection.
 *        The API will return immediately if 'len' be written into the specific SSL connection.
 *
 * @param [in] handle @n A descriptor identifying a connection.
 * @param [in] buf @n A pointer to a buffer containing the data to be transmitted.
 * @param [in] len @n The length, in bytes, of the data pointed to by the 'buf' parameter.
 * @param [in] timeout_ms @n Specify the timeout value in millisecond. In other words, the API block 'timeout_ms' millisecond maximumly.
 * @retval      < 0 : SSL connection error occur..
 * @retval        0 : No any data be write into the SSL connection in 'timeout_ms' timeout period.
 * @retval (0, len] : The total number of bytes be written in 'timeout_ms' timeout period.
 * @see None.
 */
int32_t HAL_SSL_Write( uintptr_t handle, const char *buf, int len, int timeout_ms);

/**
 * @brief Read data from the specific SSL connection with timeout parameter.
 *        The API will return immediately if 'len' be received from the specific SSL connection.
 *
 * @param [in] handle @n A descriptor identifying a SSL connection.
 * @param [out] buf @n A pointer to a buffer to receive incoming data.
 * @param [out] len @n The length, in bytes, of the data pointed to by the 'buf' parameter.
 * @param [in] timeout_ms @n Specify the timeout value in millisecond. In other words, the API block 'timeout_ms' millisecond maximumly.
 *
 * @retval       -2 : SSL connection error occur.
 * @retval       -1 : SSL connection be closed by remote server.
 * @retval        0 : No any data be received in 'timeout_ms' timeout period.
 * @retval (0, len] : The total number of bytes be received in 'timeout_ms' timeout period.
 * @see None.
 */
int32_t HAL_SSL_Read(uintptr_t handle, char *buf, int len, int timeout_ms);



#if defined(__cplusplus)
}
#endif

#endif
