#include <my_header.h>

/* Usage:  */
int main(int argc, char *argv[]){                                  
    int shmid = shmget(ftok("./makefile",1),1024,0600|IPC_CREAT);
    char *p = (char*)shmat(shmid,NULL,0);

    printf("read sharemem :%s",p);

    return 0;
}

