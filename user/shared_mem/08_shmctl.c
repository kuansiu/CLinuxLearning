#include <my_header.h>

/* Usage:  */
int main(int argc, char *argv[]){                                  
    int shmid = shmget(1000,1024,0600|IPC_CREAT);
    char *p = (char *)shmat(shmid,NULL,0);
    int ret = shmctl(shmid,IPC_RMID,NULL);
    return 0;
}

