#include <my_header.h>

/* Usage:  */
int main(int argc, char *argv[]){                                  
    key_t keytag = ftok("./makefile",1);
    ERROR_CHECK(keytag,-1,"ftok");
    printf("keytag :%d\n",keytag);
    int shmid = shmget(keytag,1024,0600|IPC_CREAT);
    ERROR_CHECK(shmid,-1,"shmid");
    char *p = (char*)shmat(shmid,NULL,0);
    ERROR_CHECK(p,NULL,"p");
    strcpy(p,"1245678");

    return 0;
}

