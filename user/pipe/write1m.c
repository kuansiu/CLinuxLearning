#include <my_header.h>

/* Usage:  */
int main(int argc, char *argv[]){                                  
    int shmid = shmget(100,4096,0600|IPC_CREAT);
    int *p = (int *)shmat(shmid,NULL,0);
    p[0]=0;
    if(fork()==0){
        for(int i = 0;i<10000000;i++){
            p[0]++;
        }
    }else{
        for(int i = 0;i<10000000;i++){
            p[0]++;
        }
        wait(NULL);
        printf("%d \n",p[0]);
    }
    return 0;
}

