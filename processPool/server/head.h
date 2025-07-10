#include "my_header.h"

enum{
    FREE,
    BUSY
};

typedef struct workerData_s {
    pid_t pid;
    int status;
    int pipefd;
}workerData_t;

int initTcp(char* ip,char* port);

int makeWorker(int num,workerData_t arr[]);

int recvfd(int sockfd,int *pfdtorecv);

int sendfd(int sockfd,int fdtosend);

int epollAdd(int epfd,int fd);

int epollDel(int epfd,int fd);
