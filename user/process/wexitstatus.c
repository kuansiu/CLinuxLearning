#include <my_header.h>

/* Usage:  */
int main(int argc, char *argv[]){                                  
    pid_t pid = fork();
    int status;
    ERROR_CHECK(pid,-1,"fork failed");
    if(pid == 0){
        printf("child process %d : caliing exit(-1)\n",getpid());
        exit(-1);
    }else{
        ERROR_CHECK(wait(&status),-1,"wait");
        if(WIFEXITED(status)){
            int code = WEXITSTATUS(status);
            printf("Father %d : child %d exited normally,exit status = %d\n",
                   getpid(),pid,code);
            
        }else if (WIFSIGNALED(status)){
            printf("Father %d: child %d was terminated by signal %d \n",
                   getpid(),pid,WTERMSIG(status));
        }

    }
    return 0;
}

