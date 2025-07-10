#include <my_header.h>

int main(int argc, char *argv[])
{
    ARGS_CHECK(argc,3);
    
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(atoi(argv[2]));
    addr.sin_addr.s_addr = inet_addr(argv[1]);
    
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    ERROR_CHECK(sockfd, -1, "socket");
    
    int ret = connect(sockfd, (struct sockaddr*)&addr, sizeof(addr));
    ERROR_CHECK(ret, -1, "connect");
    
    fd_set readySet, monitorSet;
    FD_ZERO(&monitorSet);
    FD_SET(STDIN_FILENO, &monitorSet);
    FD_SET(sockfd, &monitorSet);
    
    char buf[4096] = {0};
    
    while(1) {
        memcpy(&readySet, &monitorSet, sizeof(fd_set));
        select(1024, &readySet, NULL, NULL, NULL);
        
        if(FD_ISSET(sockfd, &readySet)) {
            bzero(buf, sizeof(buf));
            int ret = recv(sockfd, buf, sizeof(buf), 0);
            if(ret == 0) {
                printf("Server disconnected\n");
                break;
            }
            printf("%s", buf);
        }
        
        if(FD_ISSET(STDIN_FILENO, &readySet)) {
            bzero(buf, sizeof(buf));
            read(STDIN_FILENO, buf, sizeof(buf));
            char msg[4096];
            sprintf(msg,"client2:%s",buf);
            send(sockfd, msg, strlen(msg), 0);  
        }
    }
    
    close(sockfd);
    return 0;
}
