#include "head.h"
int main(int argc, char *argv[])
{
    // ./server 192.168.72.128 12345 3
    ARGS_CHECK(argc,4);
    int workerNum = atoi(argv[3]);
    workerData_t * workerArr = (workerData_t *)calloc(workerNum,sizeof(workerData_t));
    // 创建了很多个工人进程
    makeWorker(workerNum,workerArr);
    // 服务端准备工作做好了
    int sockfd = initTcp(argv[1],argv[2]);

    // epoll相关的事情
    int epfd = epoll_create(1);
    epollAdd(epfd,sockfd);

    struct epoll_event readySet[1024];
    while(1){
        int readyNum = epoll_wait(epfd,readySet,1024,-1);
        for(int i = 0; i < readyNum; ++i){
            if(readySet[i].data.fd == sockfd){
                int netfd = accept(sockfd,NULL,NULL);
                printf("1 client connect, netfd = %d\n", netfd);
                for(int j = 0; j < workerNum; ++j){
                    if(workerArr[j].status == FREE){
                        sendfd(workerArr[j].pipefd, netfd);
                        workerArr[j].status = BUSY;
                        break;
                    }
                }
            }
        }
    }
    return 0;
}


