#include "head.h"

typedef struct train_s{
    int len;
    char data[1000];
}train_t;
//v4.1 传输大文件 小火车协议 显示进度条
int transFile(int netfd){
    train_t train;
    char filename[] = "file1";
    train.len = 5;

    memcpy(train.data,filename,train.len);
    send(netfd,&train.len,sizeof(train.len),MSG_NOSIGNAL);
    send(netfd,train.data,train.len,MSG_NOSIGNAL);
    int fd = open(filename,O_RDWR);

    struct stat statbuf;
    fstat(fd,&statbuf);
    off_t filesize = statbuf.st_size;
    printf("filesize = %ld\n",filesize);
    train.len = sizeof(filesize);
    memcpy(train.data,&filesize,train.len);
    send(netfd,&train.len,sizeof(train.len),MSG_NOSIGNAL);
    send(netfd,train.data,train.len,MSG_NOSIGNAL);
    
    char* p = (char*)mmap(NULL,filesize,PROT_WRITE|PROT_READ,MAP_SHARED,fd,0);
    // p[0] ~ p[filesize - 1]
    off_t offset = 0;
    while (offset<filesize)
    {
        if(filesize-offset>1000){
            train.len = 1000;
        }else{
            train.len = filesize-offset;
        }
        //先发头
        send(netfd,&train.len,sizeof(train.len),MSG_NOSIGNAL);
        //文件内容从映射区到网卡 零拷贝
        send(netfd,p+offset,train.len,MSG_NOSIGNAL);
        offset+=train.len;
    }
    //发送空包 避免粘包
    train.len = 0;
    send(netfd,&train.len,sizeof(train.len),MSG_NOSIGNAL);
    munmap(p,filesize);

    close(fd);
    return 0;
}
