#ifndef __TASK_QUEUE__
#define __TASK_QUEUE__
#include<my_header.h>

typedef struct node_s{
    int netfd;
    struct node_s* next;
}node_t;

typedef struct taskQueue_s
{
    node_t* front;
    node_t* rear;
    int queueSize;    
}taskQueue_t;

int taskQueueInit(taskQueue_t* taskQueue);

int enQueue(taskQueue_t* taskQueue,int netfd);

int deQueue(taskQueue_t *taskQueue);

int printQueue(taskQueue_t *taskQueue);

#endif