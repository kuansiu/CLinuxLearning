#include <my_header.h>


int main(int argc, char *argv[])
{
    ARGS_CHECK(argc,3);
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(atoi(argv[2]));
    addr.sin_addr.s_addr = inet_addr(argv[1]);
    
    int sockfd = socket(AF_INET,SOCK_STREAM,0);
    int ret = connect(sockfd,(struct sockaddr*)&addr,sizeof(addr));
    ERROR_CHECK(ret,-1,"connect");

    char buf[4096]={0};
    fd_set readSet;
    while(1){
        FD_ZERO(&readSet);
        FD_SET(STDIN_FILENO,&readSet);
        FD_SET(sockfd,&readSet);
        select(sockfd+1,&readSet,NULL,NULL,NULL);
        if(FD_ISSET(STDIN_FILENO,&readSet)){//client input
            bzero(buf,sizeof(buf));
            read(STDIN_FILENO,buf,sizeof(buf));
            send(sockfd,buf,strlen(buf),0);
        }
        if(FD_ISSET(sockfd,&readSet)){//server recieve
            bzero(buf,sizeof(buf));
            ret = recv(sockfd,buf,sizeof(buf),0);
            if(ret == 0){
                printf("server quit!\n");
                break;
            }
            printf("server:%s",buf);
        }
    }
    close(sockfd);
    return 0;
}

