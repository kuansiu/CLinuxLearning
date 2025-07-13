#include <my_header.h>

typedef struct train_s{
    int len;
    char data[1000];
}train_t;

int recvn(int sockfd, void *buf, int len){
    int total = 0;
    char *p = (char *)buf;
    while (total < len) {
        ssize_t sret = recv(sockfd, p + total, len - total, 0);
        if(sret <= 0) {
            return sret;
        }
        total += sret;
    }
    return total;
}

int sendFile(int netfd, char* filename) {
    train_t train;
    
    // 发送文件名
    train.len = strlen(filename);
    send(netfd, &train.len, sizeof(train.len), 0);
    send(netfd, filename, train.len, 0);
    printf("Requesting file: %s\n", filename);
    
    // 接收文件大小
    int ret = recvn(netfd, &train.len, sizeof(train.len));
    if(ret <= 0) {
        printf("Connection closed or error\n");
        return -1;
    }
    
    if(train.len == 0) {
        printf("File not found on server\n");
        return -1;
    }
    
    off_t filesize;
    recvn(netfd, train.data, train.len);
    memcpy(&filesize, train.data, train.len);
    printf("File size: %ld bytes\n", filesize);
    
    // 创建本地文件
    char local_filename[1024];
    snprintf(local_filename, sizeof(local_filename), "downloaded_%s", filename);
    int fd = open(local_filename, O_RDWR|O_CREAT|O_TRUNC, 0666);
    if(fd == -1) {
        perror("open local file");
        return -1;
    }
    
    // 接收文件内容
    off_t cursize = 0;
    off_t lastsize = 0;
    off_t slice = filesize > 0 ? filesize/100 : 1;
    
    while(1) {
        ret = recvn(netfd, &train.len, sizeof(train.len));
        if(ret <= 0 || train.len == 0) {
            break;
        }
        
        ret = recvn(netfd, train.data, train.len);
        if(ret <= 0) {
            break;
        }
        
        write(fd, train.data, train.len);
        cursize += train.len;
        
        if(cursize - lastsize > slice || cursize == filesize) {
            printf("Progress: %5.2f%%\r", 100.0 * cursize / filesize);
            fflush(stdout);
            lastsize = cursize;
        }
    }
    
    printf("\nFile download completed: %s\n", local_filename);
    close(fd);
    return 0;
}

// ./client <server_ip> <server_port> <filename>
int main(int argc, char *argv[]) {
    ARGS_CHECK(argc, 4);
    
    char *server_ip = argv[1];
    char *server_port = argv[2];
    char *filename = argv[3];
    
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    ERROR_CHECK(sockfd, -1, "socket");
    
    struct sockaddr_in addr;
    addr.sin_addr.s_addr = inet_addr(server_ip);
    addr.sin_family = AF_INET;
    addr.sin_port = htons(atoi(server_port));
    
    int ret = connect(sockfd, (struct sockaddr*)&addr, sizeof(addr));
    ERROR_CHECK(ret, -1, "connect");
    
    printf("Connected to server %s:%s\n", server_ip, server_port);
    
    sendFile(sockfd, filename);
    
    close(sockfd);
    return 0;
}