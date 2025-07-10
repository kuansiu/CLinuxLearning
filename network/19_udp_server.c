#include <my_header.h>


int main(int argc, char *argv[])
{
    ARGS_CHECK(argc,3);
    int sockfd = socket(AF_INET,SOCK_DGRAM,0);
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(atoi(argv[2]));
    addr.sin_addr.s_addr = inet_addr(argv[1]);

    bind(sockfd,(struct sockaddr*)&addr,sizeof(addr));
    struct sockaddr_in clientAddr;
    socklen_t clientAddrLen = sizeof(clientAddr);
    fd_set readSet;
    int client_connected = 0;
    char buf[4096] = {0};

    while(1){
        FD_ZERO(&readSet);
        FD_SET(STDIN_FILENO,&readSet);
        FD_SET(sockfd,&readSet);

        select(1024,&readSet,NULL,NULL,NULL);


        if(FD_ISSET(sockfd,&readSet)){
            bzero(buf,sizeof(buf));
            ssize_t recv_len = recvfrom(sockfd,buf,sizeof(buf),0,(struct sockaddr*)&clientAddr,&clientAddrLen);

            if(recv_len>0){

                if(!client_connected){
                    printf("client connected :%s :%d\n",
                           inet_ntoa(clientAddr.sin_addr),
                           ntohs(clientAddr.sin_port));
                    client_connected = 1;
                }

                printf("client :%s\n",buf);

                if(strncmp(buf,"quit",4)==0){
                    printf("client disconnected...\n");
                    client_connected = 0;
                    
                }
            }
        }

        if(FD_ISSET(STDIN_FILENO,&readSet)){
            if(client_connected){
                bzero(buf,sizeof(buf));
                if(fgets(buf,sizeof(buf),stdin)!=NULL){
                    sendto(sockfd,buf,strlen(buf),0,
                           (struct sockaddr*)&clientAddr,clientAddrLen);
                
                }
            
    
                if(strncmp(buf,"quit",4)==0){
                    printf("server disconnected...\n");
                    break;
                }

            }else{
            char tmp[4096];
            fgets(tmp,sizeof(tmp),stdin);
            printf("no client connecte, waiting...\n");
        }
    }}
    close(sockfd);
    return 0;
}

