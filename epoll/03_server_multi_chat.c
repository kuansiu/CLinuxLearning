#include <my_header.h>


int main(int argc, char *argv[])
{
    ARGS_CHECK(argc,3);
    struct sockaddr_in addr;
    addr.sin_addr.s_addr = inet_addr(argv[1]);
    addr.sin_family = AF_INET;
    addr.sin_port = htons(atoi(argv[2]));
    int sockfd = socket(AF_INET,SOCK_STREAM,0);
    int opt = 1;
    setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));
    int ret = bind(sockfd,(struct sockaddr*)&addr,sizeof(addr));
    ERROR_CHECK(ret,-1,"bind");
    listen(sockfd,50);


    int epfd = epoll_create(1);
    struct epoll_event event;

    event.events = EPOLLIN;
    event.data.fd = sockfd;
    epoll_ctl(epfd,EPOLL_CTL_ADD,sockfd,&event);

    char buf[4096];
    int netfd1 = -1;
    int netfd2 = -1;

    struct epoll_event evs[10];
    printf("Chat server started, waiting for connections...\n");

    while(1){
        int readyNum = epoll_wait(epfd,evs,10,-1);

        for(int i = 0;i<readyNum;i++){
            int fd = evs[i].data.fd;

            if(fd == sockfd){
                if(netfd1 == -1){
                    netfd1 = accept(sockfd,NULL,NULL);
                    printf("Client 1 connected\n");
                    event.events = EPOLLIN;
                    event.data.fd = netfd1;
                    epoll_ctl(epfd,EPOLL_CTL_ADD,netfd1,&event);
                }else{
                    netfd2 = accept(sockfd,NULL,NULL);
                    printf("Client 2 connected\n");
                    event.events = EPOLLIN;
                    event.data.fd = netfd2;
                    epoll_ctl(epfd,EPOLL_CTL_ADD,netfd2,&event);

                    epoll_ctl(epfd,EPOLL_CTL_DEL,sockfd,NULL);
                }
            }else if (fd == netfd1){
                bzero(buf,sizeof(buf));
                int ret = recv(netfd1,buf,sizeof(buf),0);
                if(ret == 0){
                    printf("client 1 disconnected!\n");
                    epoll_ctl(epfd,EPOLL_CTL_DEL,netfd1,NULL);
                    close(netfd1);
                    netfd1 = -1;

                    event.events = EPOLLIN;
                    event.data.fd = sockfd;
                    epoll_ctl(epfd,EPOLL_CTL_ADD,sockfd,&event);
                    continue;
                }
                if(netfd2 != -1){
                    char msg[6000];
                    sprintf(msg,"client[%d]:%s",netfd1,buf);
                    send(netfd2,msg,strlen(msg),0);
                }
            }
            else if(fd == netfd2){
                bzero(buf,sizeof(buf));
                int ret = recv(netfd2,buf,sizeof(buf),0);
                if(ret == 0){
                    printf("client 2 disconnected!\n");
                    epoll_ctl(epfd,EPOLL_CTL_DEL,netfd2,NULL);
                    close(netfd2);
                    netfd2 = -1;

                    event.events = EPOLLIN;
                    event.data.fd = sockfd;
                    epoll_ctl(epfd,EPOLL_CTL_DEL,netfd2,NULL);
                    continue;
                }
                if(netfd1 != -1){
                    char msg[6000];
                    sprintf(msg,"client[%d]:%s",netfd2,buf);
                    send(netfd1,msg,strlen(msg),0);
                }
            }
        }
    }
    close(epfd);
    close(sockfd);
    return 0;
}

