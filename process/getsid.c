#include <my_header.h>

/* Usage:  */
int main(int argc, char *argv[]){                                  
    if(fork()==0){
        printf("child session id = %d\n",getsid(0));
        int res = setsid();
        ERROR_CHECK(res,-1,"setsid");
        printf("new session id = %d\n",getsid(0));
        while(1){
            sleep(1);
        }
    }else{
            printf("main session id = %d\n",getsid(0));
        }
    return 0;
}

