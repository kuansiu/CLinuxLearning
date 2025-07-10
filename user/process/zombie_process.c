#include <my_header.h>
void create_zombie_process(){
    pid_t pid = fork();
    ERROR_CHECK(pid,-1,"fork failed");

    if(pid==0){
        printf("child process running pid = %d,father process pid = %d\n",getpid(),getppid());
        sleep(2);
        printf("child process will exit\n");
        exit(EXIT_SUCCESS);
    }else{
        printf("father process create child process,pid = %d\n",pid);
        sleep(1);
        printf("child process is exited and become zombie process\n");
        printf("father process continuing running 10s,child process is become zombie process\n");
        sleep(10);


    }
}
/* Usage:  */
int main(int argc, char *argv[]){                                  
    create_zombie_process();
    return 0;
}

