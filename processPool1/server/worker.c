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
                int flag;
                recvfd(fds[1],&flag,&netfd);
                printf("woker flag = %d\n",flag);
                if(flag == 1){
                    printf("I am going to exit!\n");
                    exit(0);
                }
                printf("netfd = %d\n",netfd);
                transFile(netfd);
                close(netfd);
                pid_t pid = getpid();
                write(fds[1],&pid,sizeof(pid));
            }
        }
        close(fds[1]);
        workerArr[i].status = FREE;
        workerArr[i].pid = pid;
        workerArr[i].pipefd = fds[0];
        printf("i = %d, pid = %d, pipefd = %d\n", i, pid, fds[0]);
    }
    return 0;
}
