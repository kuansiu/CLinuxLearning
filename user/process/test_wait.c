#include <my_header.h>

/* Usage:  */
int main(int argc, char *argv[]){                                  
    if(fork()==0){
        printf("child process\n");
        sleep(20);
        return 100;
    }else{
        int status;
        int s_pid =wait(&status);
        printf("wait child: child pid = %d\n",s_pid);
        if(WIFEXITED(status)){
            printf("wait child: child pid = %d\n",WEXITSTATUS(status));
        }else if(WIFSIGNALED(status)){
            printf("child signed:%d\n",WTERMSIG(status));
        }
    }
    return 0;
}

