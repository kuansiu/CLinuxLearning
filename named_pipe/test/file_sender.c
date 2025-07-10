#include <my_header.h>

/* Usage:  */
int main(int argc, char *argv[]){                                  
    int pipe_fd = open("file_transfer.pipe",O_WRONLY);
    //打开管道
    //复制文件名到name_length
    char name[50] = {0};
    strcpy(name,argv[1]);
    //打开文件
    int file_fd = open(argv[1],O_RDONLY);
    char conten[1024]={0};
    //文件内容长度
    int conten_len = read(file_fd,conten,sizeof(conten));
    int len = strlen(argv[1]);//名字长度

    write(pipe_fd,&len,sizeof(int));//先发送名字的长度
    write(pipe_fd,name,len);//发送名字内容
    write(pipe_fd,&conten_len,sizeof(int));//发送内容长度
    write(pipe_fd,conten,conten_len);//发送内容

    close(pipe_fd);
    close(file_fd);


    return 0;
}

