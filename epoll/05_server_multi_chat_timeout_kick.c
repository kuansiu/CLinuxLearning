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
    struct epoll_event readySet[1024];
    int netfds[1024];
    time_t last_Active[1024];
    int fd_to_idx[65536];

    for(int i = -1;i<1024;i++){
        netfds[i] = -1;
        last_Active[i] = 0;
    }

    for(int i =0;i<65536;i++){
        fd_to_idx[i] = -1;//初始化反向索引
    }

    int currIdx = 0;

    printf("Chat server started, waiting for connections...\n");



    while(1){

        int readyNUm =  epoll_wait(epfd,readySet,1024,1000);    

        time_t currTime = time(NULL);

        for(int i = 0;i<readyNUm;i++){
            int fd = readySet[i].data.fd;
            if(fd == sockfd){
                netfds[currIdx] = accept(sockfd,NULL,NULL);
                printf("CLIENT[%d] connected!\n",currIdx);
                event.events = EPOLLIN;
                event.data.fd = netfds[currIdx];
                epoll_ctl(epfd,EPOLL_CTL_ADD,netfds[currIdx],&event);
                last_Active[currIdx] = time(NULL);
                fd_to_idx[netfds[currIdx]] = currIdx;
                currIdx++;
            }else{
                //使用反向索引快速找到对应客户端索引
                int client_idx = fd_to_idx[fd];
                if(client_idx != -1&&netfds[client_idx] == fd){

                    bzero(buf,sizeof(buf));
                    ssize_t sret = recv(netfds[client_idx],buf,sizeof(buf),0);
                    if(sret == 0){
                        printf("Client[%d] disconnected\n",client_idx);
                        epoll_ctl(epfd,EPOLL_CTL_DEL,netfds[client_idx],NULL);
                        close(netfds[client_idx]);
                        fd_to_idx[client_idx] = -1;
                        netfds[client_idx] = -1;

                    }
                    char msg[8192];
                    sprintf(msg,"CLIENT[%d]:%s",client_idx,buf);
                    last_Active[client_idx] = time(NULL);
                    for(int j=0;j<currIdx;j++){
                        if(netfds[j]!= -1&&j!=client_idx){
                            send(netfds[j],msg,strlen(msg),0);
                        }
                    }
                }
            }
        }
        for(int i = 0;i<currIdx;i++){
            if(netfds[i]!=-1&&(currTime-last_Active[i]>10)){
                printf("CLIENT[%d] timeout kicked!\n",i);
                epoll_ctl(epfd,EPOLL_CTL_DEL,netfds[i],NULL);
                close(netfds[i]);
                fd_to_idx[netfds[i]] = -1;
                netfds[i] = -1;
            }
        }
    }




    close(sockfd);
    close(epfd);
    return 0;
}

