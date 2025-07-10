#include <my_header.h>

/* Usage:  */
int main(int argc, char *argv[]){                                  
    int shmid = shmget(ftok("./makefile",100),1024,0600|IPC_CREAT);
    ERROR_CHECK(shmid,-1,"shmget");
    char *p = (char*)shmat(shmid,NULL,0);
    
    strcpy(p,"How are you");

    return 0;
}

