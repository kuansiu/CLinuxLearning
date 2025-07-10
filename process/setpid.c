#include <my_header.h>

/* Usage:  */
int main(int argc, char *argv[]){                                  
    if(fork()==0){
        printf("child pid = %d; group id = %d \n",getpid(),getpgrp());
        setpgid(0,0);
        printf("child pid = %d; group id = %d \n",getpid(),getpgrp());

    }else{
        printf("main pid = %d; group id = %d \n",getpid(),getpgrp());
    }
    return 0;
}

