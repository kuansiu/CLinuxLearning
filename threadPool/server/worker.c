#include"ThreadPool.h"

void* worker(void* arg) {
    ThreadPool_t* threadPool = (ThreadPool_t*)arg;
    
    while (1) {
        pthread_mutex_lock(&threadPool->mutex);
        
        // 检查退出标志
        if (threadPool->exitFlag) {
            pthread_mutex_unlock(&threadPool->mutex);
            printf("Worker thread %lu exiting gracefully\n", pthread_self());
            pthread_exit(NULL);
        }
        
        // 等待任务或退出信号
        while (threadPool->taskQueue.queueSize == 0 && !threadPool->exitFlag) {
            pthread_cond_wait(&threadPool->empty, &threadPool->mutex);
        }
        
        // 再次检查退出标志
        if (threadPool->exitFlag) {
            pthread_mutex_unlock(&threadPool->mutex);
            printf("Worker thread %lu exiting gracefully\n", pthread_self());
            pthread_exit(NULL);
        }
        
        // 获取任务
        int netfd = threadPool->taskQueue.front->netfd;
        deQueue(&threadPool->taskQueue);
        pthread_mutex_unlock(&threadPool->mutex);
        
        printf("Worker thread %lu processing client fd %d\n", pthread_self(), netfd);
        transFile(netfd);
    }
    
    return NULL;
}

int makeWorker(ThreadPool_t *ThreadPool) {
    for (int i = 0; i < ThreadPool->tidArr.tidArrSize; i++) {
        int ret = pthread_create(&ThreadPool->tidArr.tidArr[i], NULL, worker, ThreadPool);
        THREAD_ERROR_CHECK(ret, "pthread_create");
        printf("Created worker thread %d: %lu\n", i, ThreadPool->tidArr.tidArr[i]);
    }
    return 0;
}
