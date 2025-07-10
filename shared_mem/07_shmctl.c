#include <my_header.h>

/* Usage:  */
int main(int argc, char *argv[]){                                  
    int shmid = shmget(ftok("./makefile",123),1024,0600|IPC_CREAT);
    char *p = (char*)shmat(shmid,NULL,0);
    struct shmid_ds stat;
    int ret = shmctl(shmid,IPC_STAT,&stat);

    printf("cuid = %d perm = %o size = %ld nattch = %ld\n",
           stat.shm_perm.cuid,stat.shm_perm.mode,stat.shm_segsz,stat.shm_nattch);
    return 0;
}

