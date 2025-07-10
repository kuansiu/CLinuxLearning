#include <my_header.h>

typedef struct Info_s{
    int netfd;
    int is_Alive;
    time_t last_Active;
}Info;

int main(int argc, char *argv[])
{
    ARGS_CHECK(argc,3);
    struct sockaddr_in addr;
    addr.sin_addr.s_addr = inet_addr(argv[1]);
    addr.sin_port = htons(atoi(argv[2]));
    addr.sin_family = AF_INET;
    int sockfd = socket(AF_INET,SOCK_STREAM,0);
    int opt = 1;
    setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));
    int ret = bind(sockfd,(struct sockaddr*)(&addr),sizeof(addr));
    ERROR_CHECK(ret,-1,"bind");
    listen(sockfd,50);

    printf("Chat server started on %s:%s\n", argv[1], argv[2]);


    fd_set readySet,monitorSet;
    FD_ZERO(&monitorSet);
    FD_SET(sockfd,&monitorSet);

    struct timeval timeout;

    char buf[4096];

    Info info[1024];
    for(int i = 0;i<1024;i++){
        info[i].is_Alive = -1;
    }
    int currIdx = 0;
    while(1){

        timeout.tv_sec = 1;
        timeout.tv_usec = 0;
        memcpy(&readySet,&monitorSet,sizeof(fd_set)); 
        select(1024,&readySet,NULL,NULL,&timeout);
        time_t currTime = time(NULL);
        /* printf("current time is = %s\n",ctime(&currTime)); */

        if(FD_ISSET(sockfd,&readySet)){
            info[currIdx].netfd = accept(sockfd,NULL,NULL);
            info[currIdx].last_Active = currTime;
            info[currIdx].is_Alive = 1;

            printf("new client connect,netfd = %d,currIdx = %d\n",info[currIdx].netfd,currIdx);

            FD_SET(info[currIdx].netfd,&monitorSet);
            currIdx++;
        }
        for(int i =0;i<currIdx;i++){
            if(info[i].is_Alive == 1&&FD_ISSET(info[i].netfd,&readySet)){
                bzero(buf,sizeof(buf));
                ssize_t sret = recv(info[i].netfd,buf,sizeof(buf),0);
                if(sret == 0){
                    printf("client[%d] disconnect\n",info[i].netfd);
                    FD_CLR(info[i].netfd,&monitorSet);
                    
                    close(info[i].netfd);
                    info[i].is_Alive = -1;
                    continue;
                }
                char msg[4096];

                sprintf(msg,"client[%d]:%s",info[i].netfd,buf);
                /* fflush(stdout); */
                //更新时间
                info[i].last_Active = time(NULL);
                //群发
                for(int j = 0;j<currIdx;j++){
                    if(j != i&&info[j].is_Alive == 1){
                        send(info[j].netfd,msg,strlen(msg),0);
                    }

                }


            }


        }

        for(int k = 0;k<currIdx;k++){
            if(info[k].is_Alive == 1&&(currTime - info[k].last_Active > 30)){
                printf("timeout 30sec KICK OUT client[%d]\n",info[k].netfd);
                FD_CLR(info[k].netfd,&monitorSet);
                close(info[k].netfd);
                info[k].is_Alive = -1;
            }
        }
    }

    return 0;
}

