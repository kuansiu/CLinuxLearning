#include "head.h"

int initTcp(char *ip, char *port)
{   
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(ip);
    addr.sin_port = htons(atoi(port));
    int opt = 1;
    int sockfd = socket(AF_INET,SOCK_STREAM,0);
    setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));
    bind(sockfd,(struct sockaddr*)(&addr),sizeof(addr));
    listen(sockfd,50);
    return sockfd;
}

