#include <my_header.h>

/* Usage:  */
int main(int argc, char *argv[]){                                  
    int fd_w = open("a2b.pipe",O_WRONLY);
    ERROR_CHECK(fd_w,-1,"open a2b pipe");
    int fd_r = open("b2a.pipe",O_RDONLY);
    ERROR_CHECK(fd_r,-1,"open b2a pipe");

    fd_set set;

    while(1){
        FD_ZERO(&set);
        FD_SET(STDIN_FILENO,&set);
        FD_SET(fd_r,&set);
        int maxfd = (STDIN_FILENO>fd_r)?STDIN_FILENO:fd_r;
        select(maxfd+1,&set,NULL,NULL,NULL);
        char buf[1024]={0};
        if(FD_ISSET(STDIN_FILENO,&set)){
            bzero(buf,sizeof(buf));
            int read_stdin = read(STDIN_FILENO,buf,sizeof(buf));
            if(read_stdin>0){
                buf[read_stdin]='\0';
                if(buf[read_stdin-1]=='\n'){
                    buf[read_stdin-1]='\0';
                    read_stdin--;

                }
                int w = write(fd_w,buf,sizeof(buf));
                if(w<0){
                    perror("write to pipe");
                    break;
                }
                if(strcmp(buf,"1")==0&&strlen(buf)==1&&buf[0]=='1'){
                    printf("you exit\n");
                    break;
                }    
            }else if(read_stdin==0){
                printf("stdin closed\n");
                break;
            }else{
                perror("read stdin");
                break;
            }
        }


        if(FD_ISSET(fd_r,&set)){
            int read_fd = read(fd_r,buf,sizeof(buf));
            if(read_fd>0){
                buf[read_fd]='\0';
                printf("[Received]: ");
                fflush(stdout);
                write(STDOUT_FILENO,buf,read_fd);
                /* fflush(stdout); */
                printf("\n");
                if(strcmp(buf,"1")==0&&strlen(buf)==1&&buf[0]=='1'){
                    printf("user exit\n");
                    break;
                }
            }else if(read_fd==0){
                printf("pipe closed\n");
                break;
            }else{
                perror("read pipe");
                break;
            }
        }
    }
    close(fd_r);
    close(fd_w);
    return 0;
}

