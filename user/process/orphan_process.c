#include <my_header.h>

void create_orphan_process(){
    pid_t pid = fork();

    ERROR_CHECK(pid,-1,"fork failed");
    if(pid==0){
        printf("child process running pid:%d,father process:%d\n",getpid(),getppid());
        sleep(3);
        printf("child process wakeup pid = %d,father process pid = %d\n",getpid(),getppid());
        sleep(5);
        printf("child process exit pid = %d,father process pid = %d\n",getpid(),getppid());
        exit(EXIT_SUCCESS);
    }else{
        printf("father process create child process pid = %d\n",pid);
        sleep(1);
        printf("father process exit,child process becoming orphan process\n");
        exit(EXIT_SUCCESS);
    }
}

/* Usage:  */
int main(int argc, char *argv[]){                                  
    create_orphan_process();

    return 0;
}

