#include <my_header.h>
int main(int argc, char *argv[]){
    ARGS_CHECK(argc,2);
    // 先向终端输出一句话
    printf("两情若是长久时!\n");
    close(STDOUT_FILENO); // 关闭文件描述符1
    // 以读写模式打开一个文件
    int fd = open(argv[1],O_RDWR);
    ERROR_CHECK(fd,-1,"open");
    // 再利用printf函数向标准输出中输出
    printf("fd = %d\n", fd);
    printf("又岂在朝朝暮暮!\n");
    return 0;
}
