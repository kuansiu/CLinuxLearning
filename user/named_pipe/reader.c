#include <my_header.h>

/* Usage:  */
int main(int argc, char *argv[]){                                  
    int pipe_fd = open("1.pipe",O_RDONLY);
    while(1){
        char buf[60]={0};
        read(pipe_fd,buf,sizeof(buf));
        printf("read :%s\n",buf);
    }
    return 0;
}

