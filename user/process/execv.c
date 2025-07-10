#include <my_header.h>

/* Usage:  */
int main(int argc, char *argv[]){                                  
    if(fork() == 0){
        char * args[] = {"./test001", "2", "3", (char *)0 };
        int ret = execv("/home/user/process/test001", args);
        ERROR_CHECK(ret,-1,"execv failed");
        //execv("./05_test", args);
        //execv("05_test", args);
    }else{
        printf("main process \n");
    }
    return 0;
}

