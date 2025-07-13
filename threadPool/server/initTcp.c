#include<my_header.h>
int initTcp(char *ip,char *port){
    int sockfd = socket(AF_INET,SOCK_STREAM,0);
    int opt = 1;
    setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));
    struct sockaddr_in addr;
    addr.sin_addr.s_addr = inet_addr(ip);
    addr.sin_family = AF_INET;
    addr.sin_port = htons(atoi(port));
    bind(sockfd,(struct sockaddr*)&addr,sizeof(addr));
    listen(sockfd,50);
    return sockfd;
}