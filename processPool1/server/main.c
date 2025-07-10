#include "head.h"

// ./server 192.168.72.128 12345 3
int main(int argc, char *argv[])
{
    ARGS_CHECK(argc,4);
    int workerNum = atoi(argv[3]);
    workerData_t *workerArr = (workerData_t *)calloc(workerNum,sizeof(workerData_t));
    
    makeWorker(workerNum,workerArr);

    int sockfd = initTcp(argv[1],argv[2]);

    int epfd = epoll_create(1);
    epollAdd(epfd,sockfd);
    
    for(int i = 0;i<workerNum;i++){
        epollAdd(epfd,workerArr[i].pipefd);
    }
    
    struct epoll_event readySet[1024];
    while(1){
        int readyNum = epoll_wait(epfd,readySet,1024,-1);
        for(int i = 0;i < readyNum;i++){
            if(readySet[i].data.fd == sockfd){
                int netfd = accept(sockfd,NULL,NULL);
                printf("1 client connect, netfd = %d\n", netfd);
                for(int j = 0;j<workerNum;j++){
                    if(workerArr[j].status == FREE){
                        sendfd(workerArr[j].pipefd,netfd);
                        workerArr[j].status = BUSY;
                        break;
                    }
                }
            }
        else{
            for(int i = 0;i<workerNum;i++){
                if(readySet[i].data.fd == workerArr[i].pipefd){
                    pid_t pid;
                    read(readySet[i].data.fd,&pid,sizeof(pid));
                    printf("worker %d DONE WORK\n",pid);
                    workerArr[i].status = FREE;
                    break;
                }
            }
        }
    }
    }
    return 0;
}

