#include <my_header.h>

/* Usage:  */
int main(int argc, char *argv[]){                                  
    ARGS_CHECK(argc,2);
    close(STDIN_FILENO);
    int fd = open(argv[1],O_RDWR);
    ERROR_CHECK(fd,-1,"open");

    printf("fd = %d ",fd);

    int a;
    scanf("%d",&a);
    printf("a = %d\n",a);


    return 0;
}

