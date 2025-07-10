#include <my_header.h>

/* Usage:  */
int main(int argc, char *argv[]){                                  
    ARGS_CHECK(argc,2);
    printf("12345qwert\n");

    int fd = open(argv[1],O_RDWR);
    ERROR_CHECK(fd,-1,"open");

    close(STDOUT_FILENO);

    int fd_cp= dup(fd);

    printf("fd=%d\n",fd);
    printf("fd_cp=%d\n",fd_cp);
    printf("Aazxswedcvfr\n");

    fflush(stdout);
    close(fd);
    close(fd_cp);

    return 0;
}

