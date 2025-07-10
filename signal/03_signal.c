#include <my_header.h>

void handler(int signum){
    printf("before signum = %d\n",signum);
    sleep(5);
    printf("after signum = %d\n",signum);
}

int main(int argc, char *argv[]){                                  
    signal(SIGINT,handler);
    
    signal(SIGQUIT,handler);
    
    while(1){
        sleep(5);
    }
    
    return 0;
}

