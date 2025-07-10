#include <my_header.h>

/* Usage:  */
int main(int argc, char *argv[]){                                  
    if(fork()==0){
        printf("child process %d\n",getpid());
        sleep(20);
        return 100;
    }else{
        int status;
        /* int s_pid = waitpid(-1,&status,0); */
        int s_pid = waitpid(-1,&status,0);
        if(s_pid == 0){
            printf("no child process end\n");
        }else{
            printf("wait child:child pid = %d\n",s_pid);
            if(WIFEXITED(status)){
                printf("child status:%d\n",WEXITSTATUS(status));

            }else if(WIFSIGNALED(status)){
                printf("child signed:%d\n",WTERMSIG(status));
            }
        }
    }
    return 0;
}

