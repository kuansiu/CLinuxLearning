#include <my_header.h>

/* Usage:  */
int main(int argc, char *argv[]){                                  
    int fd_write = open("1.pipe",O_WRONLY);
    int fd_read = open("2.pipe",O_RDONLY);

    while(1){
        char buf[100]={0};
        int read_stdin = read(STDIN_FILENO,buf,sizeof(buf));
        if(read_stdin == 0){
            break;
        }

        write(fd_write,buf,sizeof(buf));

        bzero(buf,sizeof(buf));

        int read_num = read(fd_read,buf,sizeof(buf));
        if(read_num == 0){
            printf("user disconnet\n");
            break;
        }
        printf("UserB: %s ",buf);
    }
    close(fd_write);
    close(fd_read);
    return 0;
}

