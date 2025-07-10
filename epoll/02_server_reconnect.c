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
    
    int netfd;
    int epfd = epoll_create(1);

    struct epoll_event evs;
    evs.events = EPOLLIN;
    evs.data.fd = sockfd;
    epoll_ctl(epfd,EPOLL_CTL_ADD,sockfd,&evs);
    
    struct epoll_event readySet[1024];

    char buf[4096];
    int readyNum = 0;
    while(1){
        readyNum = epoll_wait(epfd,readySet,1024,-1);
        for(int i = 0;i<readyNum;i++){
            if(readySet[i].data.fd == sockfd){
                printf("client connect!\n");
                netfd = accept(sockfd,NULL,NULL);
                //移除socket监听
                epoll_ctl(epfd,EPOLL_CTL_DEL,sockfd,NULL);
                
                evs.events = EPOLLIN;
                evs.data.fd = netfd;
                //增加netfd监听
                epoll_ctl(epfd,EPOLL_CTL_ADD,netfd,&evs);

                evs.events = EPOLLIN;
                evs.data.fd = STDIN_FILENO;
                //增加stdin监听
                epoll_ctl(epfd,EPOLL_CTL_ADD,STDIN_FILENO,&evs);
            }else if (readySet[i].data.fd == STDIN_FILENO){
                //服务端输入发送
                bzero(buf,sizeof(buf));
                read(STDIN_FILENO,buf,sizeof(buf));
                send(netfd,buf,strlen(buf),0);
            }
            else if (readySet[i].data.fd == netfd){
                bzero(buf,sizeof(buf));
                //服务端接收客户端信息
                ssize_t sret = recv(netfd,buf,sizeof(buf),0);
                if(sret == 0){//客户端可能是退出了,检测一下
                    printf("client disconnect!\n");
                    evs.events = EPOLLIN;
                    evs.data.fd = sockfd;
                    //客户端退出后增加socket的监控
                    epoll_ctl(epfd,EPOLL_CTL_ADD,sockfd,&evs);
                    //删除netfd,stdin监控
                    epoll_ctl(epfd,EPOLL_CTL_DEL,netfd,NULL);
                    epoll_ctl(epfd,EPOLL_CTL_DEL,STDIN_FILENO,NULL);
                    close(netfd);
                    break;
                }
                printf("messege:%s",buf);
                fflush(stdout);
            }
        }
    }
    return 0;
}

