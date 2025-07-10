#include <my_header.h>

/* Usage:  */
int main(int argc, char *argv[]){                                  
    printf("pid = %d; group id = %d\n",getpid(),getpgrp());
    if(fork()==0){
        sleep(2);
        printf("child pid = %d; child group id = %d\n",getpid(),getpgrp());
        execl("./test000","./test000",(char *)0);
        printf("execl after\n");
    }else{
        printf("main pid = %d; group id = %d\n",getpid(),getpgrp());
        printf("main process over\n");
    }
    return 0;
}

