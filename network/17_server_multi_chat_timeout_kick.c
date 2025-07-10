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
    
    time_t last_active[1024];
    int netfd[1024];
    int count = 0;

    while(count<1024){
        netfd[count]=-1;
        last_active[count] = 0;
        count++;
    }

    int curidx = 0;
    char buf[4096] = {0};
    
    struct timeval timeout;


    while(1){
        memcpy(&readySet,&monitorSet,sizeof(fd_set));
        timeout.tv_sec = 1;
        timeout.tv_usec = 0;
        select(1024,&readySet,NULL,NULL,&timeout);
        
        time_t currTime = time(NULL);

        if(FD_ISSET(sockfd,&readySet)){
            netfd[curidx]=accept(sockfd,NULL,NULL);
            printf("curidx = %d,netfd = %d\n",curidx,netfd[curidx]);
            FD_SET(netfd[curidx],&monitorSet);
            last_active[curidx] = currTime;
            ++curidx;
        }
        
        for(int i = 0;i<curidx;++i){
            if(netfd[i]!=-1&&FD_ISSET(netfd[i],&readySet)){
                bzero(buf,sizeof(buf));
                ssize_t ret = recv(netfd[i],buf,sizeof(buf),0);
                if(ret == 0){
                    printf("1 client disconnected! i = %d,netfd = %d\n",i,netfd[i]);
                    FD_CLR(netfd[i],&monitorSet);
                    close(netfd[i]);
                    netfd[i]=-1;
                    continue;
                }
                last_active[i] = time(NULL);
                char msg[8192];
                sprintf(msg,"client[%d]:%s",i,buf);
                for(int j=0;j<curidx;++j){
                    if(netfd[j]!=-1&&j!=i){
                        send(netfd[j],msg,strlen(msg),0);
                    }
                }
            }
        }
        
        for(int t = 0;t<curidx;++t){
            if(netfd[t] != -1&&(currTime - last_active[t])>30){
                printf("Client %d timeout, kicking out! netfd = %d\n", t, netfd[t]);
                FD_CLR(netfd[t],&monitorSet);
                close(netfd[t]);
                netfd[t]=-1;
            }
        }
    }
    close(sockfd);
    return 0;
}

