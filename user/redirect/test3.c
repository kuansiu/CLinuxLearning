#include <my_header.h>

/* Usage:  */
int main(int argc, char *argv[]){                                  
    ARGS_CHECK(argc,2);
    close(STDIN_FILENO);
    close(STDERR_FILENO);
    int old_fd = open(argv[1],O_RDWR);
    ERROR_CHECK(old_fd,-1,"open");
    printf("old_fd = %d",old_fd);

    int new_fd = dup(old_fd);
    ERROR_CHECK(new_fd,-1,"dup");
    printf("new_fd = %d\n",new_fd);

    write(old_fd,"hello",5);
    write(new_fd,"12345",5);

    close(old_fd);
    close(new_fd);

    return 0;
}

