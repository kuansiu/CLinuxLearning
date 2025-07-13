#ifndef __THREAD_POOL__
#define __THREAD_POOL__
#include <my_header.h>
#include "tidArr.h"
#include "taskQueue.h"

typedef struct ThreadPool_s
{
    //子线程信息
    tidArr_t tidArr;
    //任务队列
    taskQueue_t taskQueue;
    //锁
    pthread_mutex_t mutex;
    //条件变量
    pthread_cond_t empty;
    pthread_cond_t full;
    //退出标志
    int exitFlag;
    //退出管道
    int exitPipe[2];
}ThreadPool_t;

int ThreadPoolInit(ThreadPool_t* ThreadPool,int workerNum);

int makeWorker(ThreadPool_t *ThreadPool);

int epollAdd(int epfd,int fd);

int epollDel(int epfd,int fd);

int transFile(int netfd);

int initTcp(char *ip,char* port);

#endif