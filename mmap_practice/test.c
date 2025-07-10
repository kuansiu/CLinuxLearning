#include <my_header.h>

/* Usage:  */
int main(int argc, char *argv[]){                                  
    ARGS_CHECK(argc,2);
    int fd = open(argv[1],O_RDWR);
    ERROR_CHECK(fd,-1,"open failed");

    int ret = ftruncate(fd,20);
    ERROR_CHECK(ret,-1,"ftruncate failed");

    char* addr = (char*)mmap(NULL,20,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
    ERROR_CHECK(addr,MAP_FAILED,"mmap failed");

    for(size_t i=0;i<20;i++){
        addr[i] = tolower(addr[i]);
    }

    munmap(addr,20);
    close(fd);

    return 0;
}

