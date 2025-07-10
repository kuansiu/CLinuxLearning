#include <my_header.h>

/* Usage:  */
int main(int argc, char *argv[]){                                  
    int pipefd[2];
    pipe(pipefd);
    if(fork()==0){
        close(pipefd[1]);
        char buf[10];
        /* char * buf = NULL; */
        /* size_t bufcap = 0; */
        /* ssize_t n = read(pipefd[0],buf,0); */
        int ret = read(pipefd[0],buf,10);
        printf("child process output:");
        fflush(stdout);
        write(STDOUT_FILENO,buf,10);
    }else{
        close(pipefd[0]);
        /* char buf1[10]; */
        char *buf1 =NULL;
        size_t bufcap = 0;
        ssize_t n = getline(&buf1,&bufcap,stdin);
        /* read(STDIN_FILENO,buf1,10); */
        
        printf("write before\n");
        write(pipefd[1],buf1,n);
        free(buf1);
        wait(NULL);
        printf("child process over\n");
    }
    return 0;
}

