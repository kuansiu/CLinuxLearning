#include "head.h"
int exitPipe[2];
void handler(int signum) {
    printf("signum = %d\n", signum);
    write(exitPipe[1], "1", 1);
}
// ./server 192.168.72.128 12345 3
int main(int argc, char *argv[]) {
    ARGS_CHECK(argc, 4);
    int workerNum = atoi(argv[3]);
    workerData_t *workerArr = (workerData_t *)calloc(workerNum, sizeof(workerData_t));

    makeWorker(workerNum, workerArr);

    pipe(exitPipe);

    signal(SIGUSR1, handler);

    int sockfd = initTcp(argv[1], argv[2]);

    int epfd = epoll_create(1);
    epollAdd(epfd, sockfd);

    for (int i = 0; i < workerNum; i++) {
        epollAdd(epfd, workerArr[i].pipefd);
    }
    epollAdd(epfd, exitPipe[0]);
    struct epoll_event readySet[1024];
    while (1) {
        int readyNum = epoll_wait(epfd, readySet, 1024, -1);

        for (int i = 0; i < readyNum; i++) {
            printf("[main.c] readyset[%d].data.fd = %d\n",i,readySet[i].data.fd);
            if (readySet[i].data.fd == sockfd) {
                int netfd = accept(sockfd, NULL, NULL);
                printf("[mian.c] 1 client connect, netfd = %d\n", netfd);
                for (int j = 0; j < workerNum; j++) {
                    if (workerArr[j].status == FREE) {
                        int flag = 0;
                        sendfd(workerArr[j].pipefd, flag,netfd);
                        workerArr[j].status = BUSY;
                        break;
                    }
                }
                //工人接收退出指令
            } else if (readySet[i].data.fd == exitPipe[0]) {
                printf("process pool is going to exit!\n");

                for (int j = 0; j < workerNum; ++j) {
                    int flag = 1;
                    sendfd(workerArr[j].pipefd, flag, 0);
                }
                for (int j = 0; j < workerNum; ++j) {
                    // 等待子进程终止并回收内核资源
                    wait(NULL);
                }

                printf("ALL WORKER KILLED!\n");
                free(workerArr);
                exit(EXIT_SUCCESS);
            } else {//工人目无工可做
                for (int i = 0; i < workerNum; i++) {
                    if (readySet[i].data.fd == workerArr[i].pipefd) {
                        pid_t pid;
                        read(readySet[i].data.fd, &pid, sizeof(pid));
                        printf("[else] readyset[%d].data.fd = %d\n",i,readySet[i].data.fd); 
                        printf("worker %d DONE WORK\n", pid);
                        workerArr[i].status = FREE;
                        break;
                    }
                }
            }
        }
    }
    return 0;
}
