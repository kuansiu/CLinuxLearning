#include "head.h"

int makeWorker(int workerNum,workerData_t* workerArr){
    for(int i = 0;i<workerNum;i++){
        int fds[2];
        socketpair(AF_LOCAL,SOCK_STREAM,0,fds);
        pid_t pid = fork();
        if(pid == 0){
            //子进程
            while(1){
                close(fds[0]);
                int netfd;
                recvfd(fds[1],&netfd);
                printf("netfd = %d\n",netfd);
            }
        }
        close(fds[1]);
        workerArr[i].pid = pid;
        workerArr[i].status = FREE;
        workerArr[i].pipefd = fds[0];
        printf("i = %d, pid = %d, pipefd = %d\n", i, pid, fds[0]);
    }
    return 0;
}
