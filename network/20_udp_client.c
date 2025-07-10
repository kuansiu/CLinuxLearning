#include <my_header.h>


int main(int argc, char *argv[])
{
    ARGS_CHECK(argc,3);
    
    int sockfd = socket(AF_INET,SOCK_DGRAM,0);
    struct sockaddr_in server_addr;
    server_addr.sin_addr.s_addr = inet_addr(argv[1]);
    server_addr.sin_port = htons(atoi(argv[2]));
    server_addr.sin_family = AF_INET;
    //只能客户端先发
    sendto(sockfd,"hello",5,0,(struct sockaddr*)&server_addr,sizeof(server_addr));
    
    socklen_t server_len = sizeof(server_addr);
    char buf[4096];
    
    fd_set readSet;
    

    while(1){
        FD_ZERO(&readSet);
        FD_SET(sockfd,&readSet);
        FD_SET(STDIN_FILENO,&readSet);


        select(1024,&readSet,NULL,NULL,NULL);

        if(FD_ISSET(sockfd,&readSet)){
            memset(buf,0,sizeof(buf));
            ssize_t recv_len = recvfrom(sockfd,buf,sizeof(buf),0,
                                        (struct sockaddr*)&server_addr,&server_len);
            if(recv_len>0){
                printf("server: %s\n",buf);
            }
        }
        

        if(FD_ISSET(STDIN_FILENO,&readSet)){
            bzero(buf,sizeof(buf));
            if(fgets(buf,sizeof(buf),stdin)){
                sendto(sockfd,buf,strlen(buf),0,(struct sockaddr*)&server_addr,server_len);


            }
        }
    }
    close(sockfd);
    return 0;
}

