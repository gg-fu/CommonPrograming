#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include "queue.h"

queue *init_queue(int max_size)
{
    queue *q = (queue *)malloc(sizeof(queue));
    if(q == NULL)
    {
	perror("malloc failed \n");
	return NULL;
    }
    q->data = (void **)malloc(max_size*sizeof(void *));
    if(q->data == NULL)
    {
	perror("malloc failed \n");
	return NULL;
    }

    q->_head = 0;
    q->_tail = 0;
    q->max_size = max_size;

    pthread_mutex_init(&q->lock, NULL);  
    pthread_cond_init(&q->notempty, NULL);  
    pthread_cond_init(&q->notfull, NULL);  
    return q;
}

void enqueue(queue* q,void *data)
{
     pthread_mutex_lock(&q->lock);  
    /* 等待缓冲区未满*/  
    if ((q->_tail + 1) % QUEUE_SIZE == q->_head)  
    {  
        pthread_cond_wait(&q->notfull, &q->lock);  
    }  
    /* 写数据,并移动指针 */  
    q->data[q->_tail] = data;  
    q->_tail++;  
    if (q->_tail >= QUEUE_SIZE)  
        q->_tail = 0;  
    /* 设置缓冲区非空的条件变量*/  
    pthread_cond_signal(&q->notempty);  
    pthread_mutex_unlock(&q->lock);
}

void dequeue(queue* q,void **data)
{
    pthread_mutex_lock(&q->lock);
    /* 等待缓冲区未满*/
    if (q->_tail == q->_head)      
    {
        pthread_cond_wait(&q->notempty, &q->lock);
    }
    /* 写数据,并移动指针 */
    *data = q->data[q->_head];     
    q->_head++;   
    if (q->_head >= QUEUE_SIZE)     
        q->_head = 0;    
    /* 设置缓冲区非空的条件变量*/
    pthread_cond_signal(&q->notfull);
    pthread_mutex_unlock(&q->lock);
}
