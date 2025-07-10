#include <my_header.h>


int main(int argc, char *argv[])
{
    ARGS_CHECK(argc,2);
    struct hostent* Host = gethostbyname(argv[1]);
    printf("real host name = %s\n",Host->h_name);
    for(int i =0;Host->h_aliases[i]!=NULL;i++){
        printf("aliases = %s\n",Host->h_aliases[i]);}
    char buf[1024]={0};
    for(int i=0;Host->h_addr_list[i]!=NULL;i++){
        memset(buf,0,sizeof(buf));
        inet_ntop(AF_INET,Host->h_addr_list[i],buf,sizeof(buf));
        printf("ip = %s\n",buf);}

    return 0;
}

