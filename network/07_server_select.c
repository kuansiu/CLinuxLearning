#include <my_header.h>


int main(int argc, char *argv[])
{
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(atoi(argv[2]));
    addr.sin_addr.s_addr = inet_addr(argv[1]);
    int sockfd = socket(AF_INET,SOCK_STREAM,0);
    int opt = 1;
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    int ret = bind(sockfd,(struct sockaddr*)&addr,sizeof(addr));
    ERROR_CHECK(ret,-1,"bind");
    ret = listen(sockfd,50);
    ERROR_CHECK(ret,-1,"listen");
    struct sockaddr_in clientAddr;
    socklen_t clientAddrLen = sizeof(clientAddr);
    
    int netfd = accept(sockfd,(struct sockaddr*)&clientAddr,&clientAddrLen);
    
    fd_set readSet;
    char buf[4096] = {0};
    while(1){
        FD_ZERO(&readSet);
        FD_SET(STDIN_FILENO,&readSet);
        FD_SET(netfd,&readSet);
        select(netfd+1,&readSet,NULL,NULL,NULL);

        if(FD_ISSET(STDIN_FILENO,&readSet)){
            bzero(buf,sizeof(buf));
            read(STDIN_FILENO,buf,sizeof(buf));
            send(netfd,buf,strlen(buf),0);
        }
        
        if(FD_ISSET(netfd,&readSet)){
            bzero(buf,sizeof(buf));
            ret = recv(netfd,buf,sizeof(buf),0);
            if(ret == 0){
                printf("client quit!\n");
                break;
            }
            printf("client:%s",buf);
        }
    }
    return 0;
}

