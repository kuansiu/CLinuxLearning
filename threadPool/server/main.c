#include"ThreadPool.h"

ThreadPool_t *globalThreadPool = NULL;

void handler(int signum) {
    printf("Received signal %d, initiating graceful shutdown...\n", signum);
    if (globalThreadPool) {
        globalThreadPool->exitFlag = 1;
        write(globalThreadPool->exitPipe[1], "1", 1);
    }
}

int main(int argc, char *argv[]) {
    ARGS_CHECK(argc, 4);
    
    char *ip = argv[1];
    char *port = argv[2];
    int workerNum = atoi(argv[3]);
    
    ThreadPool_t threadPool;
    globalThreadPool = &threadPool;
    
    ThreadPoolInit(&threadPool, workerNum);
    
    // 创建退出管道
    pipe(threadPool.exitPipe);
    
    // 注册信号处理函数
    signal(SIGUSR1, handler);
    signal(SIGINT, handler);
    
    makeWorker(&threadPool);
    
    int sockfd = initTcp(ip, port);
    printf("Server listening on %s:%s with %d worker threads\n", ip, port, workerNum);
    
    int epfd = epoll_create(1);
    epollAdd(epfd, sockfd);
    epollAdd(epfd, threadPool.exitPipe[0]);
    
    struct epoll_event readySet[1024];
    
    while (1) {
        int readyNum = epoll_wait(epfd, readySet, 1024, -1);
        
        for (int i = 0; i < readyNum; i++) {
            if (readySet[i].data.fd == sockfd) {
                // 新客户端连接
                int netfd = accept(sockfd, NULL, NULL);
                printf("New client connected, netfd = %d\n", netfd);
                
                pthread_mutex_lock(&threadPool.mutex);
                enQueue(&threadPool.taskQueue, netfd);
                pthread_cond_signal(&threadPool.empty);
                pthread_mutex_unlock(&threadPool.mutex);
                
            } else if (readySet[i].data.fd == threadPool.exitPipe[0]) {
                // 收到退出信号
                printf("Thread pool is going to exit gracefully!\n");
                
                // 通知所有工作线程退出
                pthread_mutex_lock(&threadPool.mutex);
                threadPool.exitFlag = 1;
                pthread_cond_broadcast(&threadPool.empty);
                pthread_mutex_unlock(&threadPool.mutex);
                
                // 等待所有线程结束
                for (int j = 0; j < threadPool.tidArr.tidArrSize; j++) {
                    pthread_join(threadPool.tidArr.tidArr[j], NULL);
                    printf("Thread %lu joined\n", threadPool.tidArr.tidArr[j]);
                }
                
                printf("All worker threads terminated!\n");
                close(sockfd);
                close(threadPool.exitPipe[0]);
                close(threadPool.exitPipe[1]);
                pthread_mutex_destroy(&threadPool.mutex);
                pthread_cond_destroy(&threadPool.empty);
                pthread_cond_destroy(&threadPool.full);
                exit(0);
            }
        }
    }
    
    return 0;
}
