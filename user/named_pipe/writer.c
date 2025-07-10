#include <my_header.h>

/* Usage:  */
int main(int argc, char *argv[]){                                  
    int pipe_fd = open("1.pipe", O_WRONLY);
    while(1){
        write(pipe_fd, "hello", 5);
        printf("write once \n");
        sleep(5);
    }

    return 0;
}

