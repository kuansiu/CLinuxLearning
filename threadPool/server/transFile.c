#include "ThreadPool.h"

typedef struct train_s{
    int len;
    char data[1000];
}train_t;

int transFile(int netfd){
    train_t train;
    
    // �6�7��B���
    int ret = recv(netfd, &train.len, sizeof(train.len), 0);
    if(ret <= 0){
        printf("client disconnected\n");
        return -1;
    }
    
    ret = recv(netfd, train.data, train.len, 0);
    if(ret <= 0){
        printf("recv filename failed\n");
        return -1;
    }
    
    train.data[train.len] = '\0';
    printf("client wants file: %s\n", train.data);
    
    // S ��
    int fd = open(train.data, O_RDONLY);
    if(fd == -1){
        printf("file %s not found\n", train.data);
        // ���
        train.len = 0;
        send(netfd, &train.len, sizeof(train.len), MSG_NOSIGNAL);
        return -1;
    }
    
    // �և�'v�
    struct stat statbuf;
    fstat(fd, &statbuf);
    off_t filesize = statbuf.st_size;
    printf("filesize = %ld\n", filesize);
    
    train.len = sizeof(filesize);
    memcpy(train.data, &filesize, train.len);
    send(netfd, &train.len, sizeof(train.len), MSG_NOSIGNAL);
    send(netfd, train.data, train.len, MSG_NOSIGNAL);
    
    // (mmap�L�� �
    char* p = (char*)mmap(NULL, filesize, PROT_READ, MAP_PRIVATE, fd, 0);
    if(p == MAP_FAILED){
        perror("mmap");
        close(fd);
        return -1;
    }
    
    off_t offset = 0;
    while(offset < filesize){
        if(filesize - offset > 1000){
            train.len = 1000;
        }else{
            train.len = filesize - offset;
        }
        
        // H�pn�
        send(netfd, &train.len, sizeof(train.len), MSG_NOSIGNAL);
        // �����(��)
        send(netfd, p + offset, train.len, MSG_NOSIGNAL);
        offset += train.len;
    }
    
    // �z� ��_
    train.len = 0;
    send(netfd, &train.len, sizeof(train.len), MSG_NOSIGNAL);
    
    munmap(p, filesize);
    close(fd);
    printf("file transfer completed\n");
    
    return 0;
}