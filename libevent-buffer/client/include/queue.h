#ifndef _QUEUE_H
#define _QUEUE_H
#include <stdlib.h>
#include <pthread.h>
#define QUEUE_SIZE 20

typedef struct {
    void **data;
    int _head;
    int _tail;
     int max_size;
    pthread_mutex_t lock; /* 互斥体lock 用于对缓冲区的互斥操作 */
    pthread_cond_t notempty; /* 缓冲区非空的条件变量 */
    pthread_cond_t notfull; /* 缓冲区未满的条件变量 */
}queue;

queue *init_queue(int max_size);
void enqueue(queue* q,void *data);
void dequeue(queue* q,void **data);
#endif
