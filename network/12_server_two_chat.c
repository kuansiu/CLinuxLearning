#include <my_header.h>


int main(int argc, char *argv[])
{
    ARGS_CHECK(argc,3);
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(argv[1]);
    addr.sin_port = htons(atoi(argv[2]));
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    ERROR_CHECK(sockfd, -1, "socket");

    int opt = 1;
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    int ret = bind(sockfd, (struct sockaddr*)&addr, sizeof(addr));
    ERROR_CHECK(ret, -1, "bind");

    ret = listen(sockfd, 50);
    ERROR_CHECK(ret, -1, "listen");

    printf("Chat server started on %s:%s\n", argv[1], argv[2]);
    
    fd_set monitorSet,readySet;
    FD_ZERO(&monitorSet);
    FD_SET(sockfd,&monitorSet);

    int netfd1 = -1;
    int netfd2 = -1;
    char buf[4096] = {0};
    while(1){
        memcpy(&readySet,&monitorSet,sizeof(fd_set));
        select(1024,&readySet,NULL,NULL,NULL);

        if(FD_ISSET(sockfd,&readySet)){
            if(netfd1 == -1){
                netfd1 = accept(sockfd,NULL,NULL);
                printf("netfd1 = %d\n",netfd1);
            }else{
                netfd2 = accept(sockfd,NULL,NULL);
                printf("netfd2 = %d\n",netfd2);
                FD_CLR(sockfd,&monitorSet);
                FD_SET(netfd1,&monitorSet);
                FD_SET(netfd2,&monitorSet);

            }
        }

        if(netfd1!=-1&&FD_ISSET(netfd1,&readySet)){
            bzero(buf,sizeof(buf));
            int ret = recv(netfd1,buf,sizeof(buf),0);
            if(ret == 0){
                FD_CLR(netfd1,&monitorSet);
                FD_SET(sockfd,&monitorSet);
                printf("client1 disconnected\n");
                close(netfd1);
                netfd1 = -1;
                continue;
            }
            send(netfd2,buf,strlen(buf),0);
        }


        if(netfd2!=-1&&FD_ISSET(netfd2,&readySet)){
            bzero(buf,sizeof(buf));
            int ret = recv(netfd2,buf,sizeof(buf),0);
            if(ret == 0){
                FD_CLR(netfd2,&monitorSet);
                FD_SET(sockfd,&monitorSet);
                printf("client2 disconnected\n");
                close(netfd2);
                netfd2 = -1;
                continue;
            }
            send(netfd1,buf,strlen(buf),0);
        }
    }
    close(sockfd);
    return 0;
}

