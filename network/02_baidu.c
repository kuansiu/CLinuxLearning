#include <my_header.h>
#include <netdb.h>

int main(int argc, char *argv[])
{
    const char query[] ="GET / HTTP/1.0\r\n"
        "Host: www.baidu.com\r\n"
        "\r\n";

    const char hostname[] = "www.baidu.com";
    
    struct hostent* Host = gethostbyname(hostname);
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(80);
    memcpy(&addr.sin_addr,Host->h_addr_list[0],sizeof(addr.sin_addr));
    
    int sockfd = socket(AF_INET,SOCK_STREAM,0);
    int cret = connect(sockfd,(struct sockaddr*)&addr,sizeof(addr));
    ERROR_CHECK(cret,-1,"connect");
    
    send(sockfd,query,strlen(query),0);
    char buf[4096];
    while(1){
        memset(buf,0,sizeof(buf));
        ssize_t ret = recv(sockfd,buf,sizeof(buf),0);
        ERROR_CHECK(ret,0,"recv");
        printf("ret = %ld\n,buf = %s\n",ret,buf);
    }

    return 0;
}

