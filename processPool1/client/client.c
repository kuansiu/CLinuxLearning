#include <my_header.h>

typedef struct train_s{
    int len;
    char data[1000];

}train_t;

int recvn(int sockfd,void *buf,int len){
    int total = 0;
    char *p = (char *)buf;
    while (total < len)
    {
        ssize_t sret = recv(sockfd,p+total,len-total,0);
        total+=sret;
    }
    return total;
}
//v4.0
int recvFile(int netfd){
    train_t train;
    char filename[4096] = {0};
    recv(netfd,&train.len,sizeof(train.len),0);
    recv(netfd,train.data,train.len,0);
    memcpy(filename,train.data,train.len);//传文件名

    int fd = open(filename,O_RDWR|O_CREAT|O_TRUNC,0666);

    off_t filesize;
    recvn(netfd,&train.len,sizeof(train.len));
    recvn(netfd,train.data,train.len);
    memcpy(&filesize,train.data,train.len);//传文件大小
    printf("client recvFile filesize = %ld\n",filesize);

    off_t cursize = 0;
    off_t lastsize = 0;
    off_t slice = filesize/10000;

    while(1){
        recvn(netfd,&train.len,sizeof(train.len));//车头
        if(train.len==0){
            break;
        }
        cursize += train.len;
        if(cursize - lastsize > slice){
            printf("%5.2lf%%\r",100.0*cursize/filesize);
            fflush(stdout);
            lastsize = cursize;
        }
        recvn(netfd,train.data,train.len);//车厢
        write(fd,train.data,train.len);//写入文件
    }
    printf("100.00%%\n");
    close(fd);
    return 0;
}
// // ./client 192.168.72.128 12345
int main(int argc, char *argv[])
{   
    ARGS_CHECK(argc,3);
    int sockfd = socket(AF_INET,SOCK_STREAM,0);
    struct sockaddr_in addr;
    addr.sin_addr.s_addr = inet_addr(argv[1]);
    addr.sin_family = AF_INET;
    addr.sin_port = htons(atoi(argv[2]));
    int ret = connect(sockfd,(struct sockaddr*)&addr,sizeof(addr));
    ERROR_CHECK(ret,-1,"connect");
    recvFile(sockfd);
    close(sockfd);
    return 0;
}

