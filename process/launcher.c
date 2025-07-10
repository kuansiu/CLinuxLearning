#include <my_header.h>

/* Usage:  */
int main(int argc, char *argv[]){                     
    pid_t pid = fork();
    if(pid==0){
        execl("./listdir","listdir",(char*)0);
        printf("已执行execl\n");
    }else{
        int status;
        waitpid(pid,&status,0);
        printf("子进程退出,%d\n",WEXITSTATUS(status));
    }
    return 0;
}

