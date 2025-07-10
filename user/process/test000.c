#include <my_header.h>

/* Usage:  */
int main(int argc, char *argv[]){                                  
    printf("test000.c pid: %d;  group id: %d \n",getpid(), getpgrp());
    return 0;
}

