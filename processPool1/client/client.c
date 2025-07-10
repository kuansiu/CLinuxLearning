#include <my_header.h>

typedef struct train_s{
    int len;
    char data[1000];

}train_t;

int recvFile(int netfd){
    train_t train;
    char filename[4096] = {0};
    recv(netfd,&train.len,sizeof(train.len),0);
    recv(netfd,train.data,train.len,0);
    memcpy(filename,train.data,train.len);//传文件名

    int fd = open(filename,O_RDWR|O_CREAT|O_TRUNC,0666);
    recv(netfd,&train.len,sizeof(train.len),0);
    recv(netfd,train.data,train.len,0);
    write(fd,train.data,train.len);//传内容

    close(fd);
    return 0;
}

int main(int argc, char *argv[])
{   
    ARGS_CHECK(argc,3);
    int sockfd = socket(AF_INET,SOCK_STREAM,0);
    struct sockaddr_in addr;
    addr.sin_addr.s_addr = inet_addr(argv[1]);
    addr.sin_family = AF_INET;
    addr.sin_port = htons(atoi(argv[2]));
    int ret = connect(sockfd,(struct sockaddr*)&addr,sizeof(addr));
    ERROR_CHECK(ret,-1,"bind");
    recvFile(sockfd);
    close(sockfd);
    return 0;
}

