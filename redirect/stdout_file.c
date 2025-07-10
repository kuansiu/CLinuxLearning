#include <my_header.h>

/* Usage:  */
int main(int argc, char *argv[]){                                  
    ARGS_CHECK(argc,2);

    int fd = open(argv[1],O_RDWR);
    ERROR_CHECK(fd,-1,"open");

    int stdou_bakup = 7;

    dup2(STDOUT_FILENO,stdou_bakup);//backup
    printf("output on screen1\n");

    dup2(fd,STDOUT_FILENO);
    printf("output in file1\n");
    
    dup2(stdou_bakup,STDOUT_FILENO);;
    printf("output on screen 2\n");

    dup2(fd,STDOUT_FILENO);
    printf("output in file2\n");
    
    close(stdou_bakup);
    close(fd);

    return 0;
}

