#include <my_header.h>

int main(int argc, char *argv[]){
   ARGS_CHECK(argc, 2);
    int fd=open(argv[1],O_WRONLY|O_CREAT|O_TRUNC,0664);
    ERROR_CHECK(fd,-1,"open");

    // 先printf输出一句话
    printf("start.\n");
    //先备份标准输出的文件描述符
    int temp=dup(STDOUT_FILENO);
    //将标准输出重定向指向文件
    dup2(fd, STDOUT_FILENO);
    printf("How are you?\n");
    //将标准输出重新终端
    dup2(temp, STDOUT_FILENO);
    printf("I'm fine, and you?\n");
    //再将标准输出重定向到文件
    dup2(fd, STDOUT_FILENO);
    printf("Me too.\n");

    close(fd);
    return 0;
}
