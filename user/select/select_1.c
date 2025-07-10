#include <my_header.h>

/* Usage:  */
int main(int argc, char *argv[]){                                  
    fd_set read_set;
    FD_ZERO(&read_set);
    FD_SET(STDIN_FILENO,&read_set);

    printf("select before.\n");
    int ret = select(10,&read_set,NULL,NULL,NULL);
    printf("select after.\n");

    char buf[100]={0};
    read(STDIN_FILENO,buf,sizeof(buf));
    printf("input msg:%s",buf);

    return 0;
}

