#include <my_header.h>

// ./04_client 192.168.1.1 8080
int main(int argc, char *argv[])
{
    ARGS_CHECK(argc,3);
    struct sockaddr_in addr;
    addr.sin_addr.s_addr = inet_addr(argv[1]);
    addr.sin_port = htons(atoi(argv[2]));
    addr.sin_family = AF_INET;
    int sockfd = socket(AF_INET,SOCK_STREAM,0);
    
    int ret = connect(sockfd,(struct sockaddr*)&addr,sizeof(addr));
    ERROR_CHECK(ret,-1,"connect");

    char buf[4096]={0};
    
    while(1){
        printf("client : ");
        fgets(buf,sizeof(buf),stdin);
        send(sockfd,buf,strlen(buf),0);

        memset(buf,0,sizeof(buf));
        ret = recv(sockfd,buf,sizeof(buf),0);
        if(ret == 0){
            printf("server quit\n");
            break;
        }

        printf("server : %s",buf);
        
    }
    close(sockfd);
    
    return 0;

}

