#include <my_header.h>

/* Usage:  */
int main(int argc, char *argv[]){                                  
    key_t keytag = ftok("./makefile",1);
    printf("key_t : %d\n",keytag);

    int shmid = shmget(keytag,100,0600|IPC_CREAT);

    ERROR_CHECK(shmid,-1,"shmget");
    printf("shmid:%d\n",shmid);

    return 0;
}

