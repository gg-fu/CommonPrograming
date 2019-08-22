#ifndef _HAL_OS_LINUX_H
#define _HAL_OS_LINUX_H


#if defined(__cplusplus)
extern "C" {
#endif

#define _IN_
#define _OUT_
#define PLATFORM_WAIT_INFINITE          (~0)


typedef enum {
    os_thread_priority_idle = -3,        /* priority: idle (lowest) */
    os_thread_priority_low = -2,         /* priority: low */
    os_thread_priority_belowNormal = -1, /* priority: below normal */
    os_thread_priority_normal = 0,       /* priority: normal (default) */
    os_thread_priority_aboveNormal = 1,  /* priority: above normal */
    os_thread_priority_high = 2,         /* priority: high */
    os_thread_priority_realtime = 3,     /* priority: realtime (highest) */
    os_thread_priority_error = 0x84,     /* system cannot determine priority or thread has illegal priority */
} hal_os_thread_priority_t;
typedef struct _hal_os_thread {
    hal_os_thread_priority_t priority;     /*initial thread priority */
    void                    *stack_addr;   /* thread stack address malloced by caller, use system stack by . */
    size_t                   stack_size;   /* stack size requirements in bytes; 0 is default stack size */
    int                      detach_state; /* 0: not detached state; otherwise: detached state. */
    char                    *name;         /* thread name. */
} hal_os_thread_param_t;


void *HAL_MutexCreate(void);
void HAL_MutexDestroy(_IN_ void *mutex);
void HAL_MutexLock(_IN_ void *mutex);
void HAL_MutexUnlock(_IN_ void *mutex);
void *HAL_Malloc(_IN_ uint32_t size);
void HAL_Free(_IN_ void *ptr);
uint64_t HAL_UptimeMs(void);
char *HAL_GetTimeStr(_IN_ char *buf, _IN_ int len);
void HAL_SleepMs(_IN_ uint32_t ms);
void HAL_Srandom(uint32_t seed);
uint32_t HAL_Random(uint32_t region);
void *HAL_SemaphoreCreate(void);
void HAL_SemaphoreDestroy(_IN_ void *sem);
void HAL_SemaphorePost(_IN_ void *sem);
int HAL_SemaphoreWait(_IN_ void *sem, _IN_ uint32_t timeout_ms);
int HAL_ThreadCreate(\
            void **thread_handle,\
            void *(*work_routine)(void *),\
            void *arg,\
            hal_os_thread_param_t *hal_os_thread_param,\
            int *stack_used);
void HAL_ThreadDetach(_IN_ void *thread_handle);
void HAL_ThreadDelete(_IN_ void *thread_handle);
void HAL_Sys_reboot(void);
void HAL_UTC_Set(long long ms);
long long HAL_UTC_Get(void);
void *HAL_Timer_Create(const char *name, void (*func)(void *), void *user_data);
int HAL_Timer_Start(void *timer, int ms);
int HAL_Timer_Stop(void *timer);
int HAL_Timer_Delete(void *timer);
void HAL_Reboot(void);	
#if defined(__cplusplus)
}
#endif

#endif
