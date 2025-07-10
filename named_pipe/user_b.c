#include <my_header.h>

/* Usage:  */
int main(int argc, char *argv[]){                                  
    int fd_read = open("1.pipe", O_RDONLY);
    int fd_write = open("2.pipe", O_WRONLY);

    while(1){
        char buf[60];
        bzero(buf, sizeof(buf));
        // 读取1.pipe: 打印到标准输出
        int read_num = read(fd_read, buf, sizeof(buf));
        if(read_num == 0){
            printf("对方断开链接 \n");
            break;
        }
        printf("UserA: %s ", buf);

        bzero(buf, sizeof(buf));
        // 读取标准输入: 通过2.pipe发送给User_A
        int read_stdin = read(STDIN_FILENO, buf, sizeof(buf));
        if(read_stdin == 0){
            // 用户按下ctrl+d; 输入文件终止符; 终止标准输入; read返回0
            break;
        }
        write(fd_write, buf, sizeof(buf));

    }

    close(fd_write);
    close(fd_read);
    return 0;
}

