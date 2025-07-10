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
    int netfd = accept(sockfd,NULL,NULL);
    int epfd = epoll_create(1);

    struct epoll_event evs;
    evs.events = EPOLLIN;
    evs.data.fd = STDIN_FILENO;
    epoll_ctl(epfd,EPOLL_CTL_ADD,STDIN_FILENO,&evs);
    
    evs.events = EPOLLIN;
    evs.data.fd = netfd;
    epoll_ctl(epfd,EPOLL_CTL_ADD,netfd,&evs);
    
    struct epoll_event readySet[1024];

    char buf[4096];
    int readyNum = 0;
    while(1){
        readyNum = epoll_wait(epfd,readySet,1024,-1);
        for(int i = 0;i<readyNum;i++){
            if(readySet[i].data.fd == STDIN_FILENO){
                bzero(buf,sizeof(buf));
                read(STDIN_FILENO,buf,sizeof(buf));
                send(netfd,buf,strlen(buf),0);
            }
            else if (readySet[i].data.fd == netfd){
                bzero(buf,sizeof(buf));
                ssize_t sret = recv(netfd,buf,sizeof(buf),0);
                if(sret == 0){
                    printf("client quit!\n");
                    goto end;
                }
                printf("messege:%s",buf);
            }
        }
    }
    end:
    return 0;
}

