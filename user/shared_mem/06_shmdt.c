#include <my_header.h>

/* Usage:  */
int main(int argc, char *argv[]){                                  
    int shmid = shmget(ftok("./makefile",1),1024,0600|IPC_CREAT);
    ERROR_CHECK(shmid,-1,"shmget");
    char *p = (char*)shmat(shmid,NULL,0);
    ERROR_CHECK(p,NULL,"shmat");
    strcpy(p,"12jkl");
    shmdt(p);

    while(1){
        strcpy(p,"456");
    }
    return 0;
}

