#include <my_header.h>
void handler(int sig){
    printf("收到信号 %d\n",sig);
}
/* Usage:  */
int main(int argc, char *argv[]){                                  
    signal(SIGINT,handler);
    signal(SIGQUIT,handler);
    while(1){
        printf("running...\n");
        sleep(1);
    }

    return 0;
}

