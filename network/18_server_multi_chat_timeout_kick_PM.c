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
    time_t last_active[1024];// 添加：记录每个客户端最后活跃时间
    int netfd[1024];
    int count = 0;
    while(count<1024){
        netfd[count]=-1;
        last_active[count] = 0;
        count++;
    }
    int curidx = 0;
    char buf[4096] = {0};

    struct timeval timeout;// 添加：设置超时时间
    timeout.tv_sec = 1;// 添加：1秒超时
    timeout.tv_usec = 0;


    while(1){
        memcpy(&readySet,&monitorSet,sizeof(fd_set));
        select(1024,&readySet,NULL,NULL,&timeout);// 添加：获取当前时间

        time_t currTime = time(NULL);// 添加：获取当前时间

        if(FD_ISSET(sockfd,&readySet)){
            netfd[curidx]=accept(sockfd,NULL,NULL);
            printf("curidx = %d,netfd = %d\n",curidx,netfd[curidx]);
            FD_SET(netfd[curidx],&monitorSet);
            last_active[curidx] = currTime;// 添加：设置新客户端活跃时间

            // 添加：发送使用说明给新客户端
            char welcome[512];
            sprintf(welcome, "Welcome! You are client[%d]. Use '@id message' for private chat. You will KICK OUT CHATROOM in 120s IF YOU NOT COMMUNICATE\n", curidx);
            send(netfd[curidx], welcome, strlen(welcome), 0);

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
                last_active[i] = currTime;// 添加：更新活跃时间
                if(buf[0] == '@'&&strlen(buf)>1){// 添加：检查是否是私聊消息
                    int target_id = atoi(&buf[1]);// 私聊格式：@target_id message
                    char* space = strchr(buf,' ');
                    if(space != NULL&&target_id>=0&&target_id<curidx&&netfd[target_id]!=-1){
                        char private_msg[8192];
                        sprintf(private_msg,"Private from client[%d]: %s",i,space+1);
                        send(netfd[target_id],private_msg,strlen(private_msg),0);

                        // 给发送者确认消息
                        char confirm[512];
                        sprintf(confirm,"Private message sent to client[%d]\n",target_id);
                        send(netfd[i],confirm,strlen(confirm),0);
                    }else{
                        // 私聊格式错误或目标不存在
                        char error_msg[512];
                        sprintf(error_msg,"Private message failed: invalid target or format\n");
                        send(netfd[i],error_msg,strlen(error_msg),0);
                    }


                }else{
                    char msg[8192];
                    sprintf(msg,"client[%d]:%s",i,buf);
                    for(int j=0;j<curidx;++j){
                        if(netfd[j]!=-1&&j!=i){
                            send(netfd[j],msg,strlen(msg),0);
                        }}}

                for(int t = 0;t<curidx;++t){
                    if(netfd[t] != -1&&(currTime - last_active[t])>120){
                        printf("Client %d timeout, kicking out! netfd = %d\n", t, netfd[t]);
                        FD_CLR(netfd[t],&monitorSet);
                        close(netfd[t]);
                        netfd[t]=-1;
                    }
                }
            }}}
            close(sockfd);
            return 0;
        }

