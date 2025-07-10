#include "head.h"

typedef struct train_s{
    int len;
    char data[1000];
}train_t;

int transFile(int netfd){
    train_t train;
    char filename[] = "file1";
    train.len = 5;

    memcpy(train.data,filename,train.len);
    send(netfd,&train.len,sizeof(train.len),0);
    send(netfd,train.data,train.len,0);
    
    int fd = open(filename,O_RDWR);
    ssize_t sret = read(fd,train.data,sizeof(train.data));
    train.len = sret;
    send(netfd,&train.len,sizeof(train.len),0);
    send(netfd,train.data,train.len,0);

    close(fd);
    return 0;
}
