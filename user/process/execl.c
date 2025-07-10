#include <my_header.h>

/* Usage:  */
int main(int argc, char *argv[]){                                  
    if(fork()==0){
        printf("1 execl\n");
        execl("/home/user/process/test001","./test001","1","2",(char*)0);
        /* printf("2 execl\n"); */
        /* execl("./test001","./test001","1","2",(char*)0); */
        /* printf("3 execl\n"); */
        /* execl("test001","./test001","2","4",(char*)0); */
        printf("child over\n");
        exit(EXIT_SUCCESS);
    }else{
        printf("main process\n");
        sleep(2);
    }
    return 0;
}

