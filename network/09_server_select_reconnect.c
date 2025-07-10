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
    
    fd_set readySet,monitorSet;
    FD_ZERO(&monitorSet);
    FD_SET(STDIN_FILENO,&readySet);
    FD_SET(sockfd,&monitorSet);
    int netfd = -1;
    
    char buf[4096] = {0};
    while(1){
        memcpy(&readySet,&monitorSet,sizeof(fd_set));
        select(1024,&readySet,NULL,NULL,NULL);
        if(netfd == -1&&FD_ISSET(sockfd,&readySet)){
            netfd = accept(sockfd,NULL,NULL);
            printf("client connected\n");
            
            FD_CLR(sockfd,&monitorSet);
            FD_SET(netfd,&monitorSet);
            FD_SET(STDIN_FILENO,&monitorSet);
        }

        if(netfd!=-1&&FD_ISSET(netfd,&readySet)){
            bzero(buf,sizeof(buf));
            int ret = recv(netfd,buf,sizeof(buf),0);
            if(ret == 0){
                FD_CLR(netfd,&monitorSet);
                FD_CLR(STDIN_FILENO,&monitorSet);
                FD_SET(sockfd,&monitorSet);
                close(netfd);
                netfd = -1;
                printf("client disconnected\n");
                continue;
            }
            printf("client:%s\n",buf);
        }
        if(netfd!=-1&&FD_ISSET(STDIN_FILENO,&readySet)){
            bzero(buf,sizeof(buf));
            read(STDIN_FILENO,buf,sizeof(buf));
            send(netfd,buf,strlen(buf),0);
        }

    }
    return 0;
}

