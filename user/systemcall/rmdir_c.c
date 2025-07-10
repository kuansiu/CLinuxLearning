#include <my_header.h>

/* Usage:  */
int main(int argc, char *argv[]){                                  
    ARGS_CHECK(argc,2);

    int ret = rmdir(argv[1]);
    ERROR_CHECK(ret,-1,"rmdir");
    return 0;
}

