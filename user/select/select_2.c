#include <my_header.h>

/* Usage:  */
int main(int argc, char *argv[]){                                  
    int fd = open("1.pipe",O_RDONLY);
    printf("open 1.pipe\n");
    int fd_w = open("2.pipe",O_WRONLY);
    printf("open 2.pipe\n");
    fd_set read_set;
    int maxfd = (fd>STDIN_FILENO)?fd:STDIN_FILENO;
    while(1){
        FD_ZERO(&read_set);
        FD_SET(fd,&read_set);
        FD_SET(STDIN_FILENO,&read_set);
        char buf[1024]={0};
        /* printf("select before\n"); */
        select(maxfd+1,&read_set,NULL,NULL,NULL);
        //处理键盘输入
        if(FD_ISSET(STDIN_FILENO,&read_set)){
            /* printf("keyboard available\n"); */
            //读取用户输入
            int ret = read(STDIN_FILENO,buf,sizeof(buf));
            if(ret>0){
                /* printf("stdin write before\n"); */
                //将读取到的内容写入管道写端
                int ret_w = write(fd_w,buf,ret);
                if(ret_w<0){
                    perror("write to pipe");
                    break;
                }
            }else if(ret ==0){
                printf("stdin closed\n");
                break;
            }else{
                perror("read stdin");
                break;
            }
        }

        //管道fd可读,表示对方发来数据
        if(FD_ISSET(fd,&read_set)){
            /* printf("pipe available\n"); */
            int ret_r_fd = read(fd,buf,sizeof(buf));
            if(ret_r_fd>0){
                /* printf("pipe write before\n"); */
                //将读取到的数据写入标准输出,显示到终端
                buf[ret_r_fd]='\0';
                printf("message recv:\n");
                write(STDOUT_FILENO,buf,ret_r_fd);        
            }else if(ret_r_fd == 0){
                printf("pipe closed\n");
                break;
            }else{
                perror("read pipe");
                break;
            }
        }
    }
    close(fd);
    close(fd_w);
    return 0;
}

