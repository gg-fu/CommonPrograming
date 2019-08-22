/*
 * Copyright (c) 2014-2016 Alibaba Group. All rights reserved.
 * License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

#ifndef __LITE_LOG_H__
#define __LITE_LOG_H__
#if defined(__cplusplus)
extern "C" {
#endif
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>

#define NONE                 "\e[0m"
#define BLACK                "\e[0;30m"
#define L_BLACK              "\e[1;30m"
#define RED                  "\e[0;31m"
#define L_RED                "\e[1;31m"
#define GREEN                "\e[0;32m"
#define L_GREEN              "\e[1;32m"
#define BROWN                "\e[0;33m"
#define YELLOW               "\e[1;33m"
#define BLUE                 "\e[0;34m"
#define L_BLUE               "\e[1;34m"
#define PURPLE               "\e[0;35m"
#define L_PURPLE             "\e[1;35m"
#define CYAN                 "\e[0;36m"
#define L_CYAN               "\e[1;36m"
#define GRAY                 "\e[0;37m"
#define WHITE                "\e[1;37m"
 
#define BOLD                 "\e[1m"
#define UNDERLINE            "\e[4m"
#define BLINK                "\e[5m"
#define REVERSE              "\e[7m"
#define REDBACK              "\e[41m"
#define HIDE                 "\e[8m"
#define CLEAR                "\e[2J"
#define CLRLINE              "\r\e[K"

#define LITE_LOG_ENABLED
#define LOG_BUFFER_SIZE 256
#define LOG_MSG_MAXLEN                  (255)
#define LOG_MOD_NAME_LEN                (7)
#define LOG_PREFIX_FMT                  "%s [%s] %s(%d): "
#define LOG_PREFIX_FMT_ONLINE           "%ld %s %s "
#define HEXDUMP_SEP_LINE                "+" \
    "-----------------------" \
    "-----------------------" \
    "-----------------------"
#if defined(_PLATFORM_IS_LINUX_)
    #undef  LOG_MSG_MAXLEN
    #define LOG_MSG_MAXLEN              (512)
#endif

typedef struct {
    char            name[LOG_MOD_NAME_LEN + 1];
    int             priority;
    char            text_buf[LOG_MSG_MAXLEN + 1];
} log_client;

typedef enum _LOGLEVEL {
    LOG_EMERG_LEVEL = 0,    /* OS system is unavailable */
    LOG_CRIT_LEVEL,         /* current application aborting */
    LOG_ERR_LEVEL,          /* current app-module error */
    LOG_WARNING_LEVEL,      /* using default parameters */
    LOG_INFO_LEVEL,         /* running messages */
    LOG_DEBUG_LEVEL,        /* debugging messages */
} LOGLEVEL;

void    LITE_openlog(const char *ident);
void    LITE_closelog(void);
int     LITE_log_enabled(void);
char   *LITE_get_logname(void);
int     LITE_get_loglevel(void);
void    LITE_set_loglevel(int level);
int     LITE_hexdump(const char *title, const void *buf, const int len);
void    LITE_syslog_routine(char *m, const char *f, const int l, const int level, const char *fmt, va_list *params);
void    LITE_syslog(char *m, const char *f, const int l, const int level, const char *fmt, ...);

void LITE_printf(const char *fmt, ...);
int LITE_vsnprintf(char *str,const int len, const char *format, va_list ap);
int LITE_snprintf(char *str, const int len, const char *fmt, ...);
int LITE_hexdump(const char *title, const void *buff, const int len);
#ifdef SDK_DEBUG_LEVEL_NONE
#define log_warning(mod ,...)
#define log_info(mod ,...)
#define log_debug(mod ,...)
#else
#define log_warning(mod ,...)            LITE_syslog(mod, __FUNCTION__, __LINE__, LOG_WARNING_LEVEL, __VA_ARGS__)
#define log_info(mod ,...)               LITE_syslog(mod, __FUNCTION__, __LINE__, LOG_INFO_LEVEL, __VA_ARGS__)
#define log_debug(mod ,...)              LITE_syslog(mod, __FUNCTION__, __LINE__, LOG_DEBUG_LEVEL, __VA_ARGS__)
#endif
#define log_emerg(mod ,...)              LITE_syslog(mod, __FUNCTION__, __LINE__, LOG_EMERG_LEVEL, __VA_ARGS__)
#define log_crit(mod ,...)               LITE_syslog(mod, __FUNCTION__, __LINE__, LOG_CRIT_LEVEL, __VA_ARGS__)
#define log_err(mod ,...)                LITE_syslog(mod, __FUNCTION__, __LINE__, LOG_ERR_LEVEL, __VA_ARGS__)
#define log_err_online(mod, ...)         LITE_syslog_post(mod, LOG_ERR_LEVEL, __VA_ARGS__)
int     log_multi_line_internal(const char *f, const int l,
                                const char *title, int level, char *payload, const char *mark);
#define log_multi_line(level, title, fmt, payload, mark) \
    log_multi_line_internal(__func__, __LINE__, title, level, payload, mark)
void    LITE_rich_hexdump(const char *f, const int l,
                          const int level,
                          const char *buf_str,
                          const void *buf_ptr,
                          const int buf_len
                         );
#if defined(__GLIBC__)
#define HEXDUMP_DEBUG(buf, len) \
    LITE_rich_hexdump(__func__, __LINE__, LOG_DEBUG_LEVEL, #buf, (const void *)buf, (const int)len)
#define HEXDUMP_INFO(buf, len)      \
    LITE_rich_hexdump(__func__, __LINE__, LOG_INFO_LEVEL, #buf, (const void *)buf, (const int)len)
#else
#define HEXDUMP_DEBUG(buf, len) \
    LITE_rich_hexdump(__func__, __LINE__, LOG_DEBUG_LEVEL, #buf, buf, len)
#define HEXDUMP_INFO(buf, len)      \
    LITE_rich_hexdump(__func__, __LINE__, LOG_INFO_LEVEL, #buf, buf, len)
#endif
#if defined(__cplusplus)
}
#endif
#endif  /* __LITE_LOG_H__ */

