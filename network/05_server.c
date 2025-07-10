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
    
    int ret= bind(sockfd,(struct sockaddr*)&addr,sizeof(addr));
    ERROR_CHECK(ret,-1,"bindret");
    
    listen(sockfd,50);

    struct sockaddr_in clienAddr;
    socklen_t clienAddrLen = sizeof(clienAddr);
    int netfd = accept(sockfd,(struct sockaddr*)&clienAddr,&clienAddrLen);

    printf("netfd = %d,clien ip = %s,clien port = %d\n",
           netfd,inet_ntoa(clienAddr.sin_addr),ntohs(clienAddr.sin_port));


    printf("netfd = %d,clien ip = %u,clien port = %d\n",
           netfd,ntohl(clienAddr.sin_addr.s_addr),ntohs(clienAddr.sin_port));

    char buf[4096];
    while(1){
        memset(buf,0,sizeof(buf));
        int ret = recv(netfd,buf,sizeof(buf),0);
        if(ret==0){
            printf("client quit\n");
            break;
        }

        printf("client : %s",buf);
        
        printf("server: ");
        fgets(buf,sizeof(buf),stdin);
        send(netfd,buf,strlen(buf),0);


    }
    close(sockfd);
    close(netfd);

    return 0;
}

