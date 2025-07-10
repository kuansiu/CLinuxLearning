#include <my_header.h>

/* Usage:  */
int main(int argc, char *argv[]){                                  
    ARGS_CHECK(argc,3);

    mode_t mode;
    sscanf(argv[2],"%o",&mode);

    int ret = mkdir(argv[1],mode);
    ERROR_CHECK(ret,-1,"mkdir");
    return 0;
}

