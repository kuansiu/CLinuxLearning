#include"ThreadPool.h"

int ThreadPoolInit(ThreadPool_t* ThreadPool,int workerNum){
   tidArrInit(workerNum,&ThreadPool->tidArr);
   taskQueueInit(&ThreadPool->taskQueue);
   pthread_mutex_init(&ThreadPool->mutex,NULL);
   pthread_cond_init(&ThreadPool->full,NULL);
   pthread_cond_init(&ThreadPool->empty,NULL);
   ThreadPool->exitFlag = 0;
   return 0;
}