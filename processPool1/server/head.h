#include "my_header.h"

enum{
    BUSY,
    FREE
};

typedef struct workerData_s{
    int status;
    int pipefd;
    pid_t pid;
}workerData_t;

int makeWorker(int num,workerData_t* workerArr);

int initTcp(char* ip,char *port);

int epollAdd(int epfd,int fd);

int epollDel(int epfd,int fd);

int recvfd(int sockfd,int *pfdtorecv);

int sendfd(int sockfd,int fdtosend);

int transFile(int netfd);
