#include <my_header.h>
void handler(int signum){
    printf("signum = %d\n",signum);
}
/* Usage:  */
int main(int argc, char *argv[]){                                  
    signal(SIGINT,handler);
    sigset_t set,oldset;
    sigemptyset(&set);
    sigaddset(&set,SIGINT);
    sigprocmask(SIG_BLOCK,&set,&oldset);
    sleep(10);
    printf("sleep over!\n");
    
    sigprocmask(SIG_SETMASK,&oldset,NULL);
    while(1){
        sleep(1);
    }
    
    return 0;
}

