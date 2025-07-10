#include <my_header.h>

/* Usage:  */
int main(int argc, char *argv[]){                                  
    ARGS_CHECK(argc,2);
    int fd = open(argv[1],O_RDWR);
    ERROR_CHECK(fd,-1,"open");

    int ret = ftruncate(fd,20);
    ERROR_CHECK(ret,-1,"ftruncate");
    
    char buf[20] = {0};

    ssize_t rret = read(fd,buf,20);
    ERROR_CHECK(rret,-1,"read");

    for(ssize_t i=0;i<20;i++){
        if(isupper(buf[i])){
            buf[i] = tolower(buf[i]);
        }
    }

    lseek(fd,0,SEEK_SET);

    ssize_t sret = write(fd,buf,20);
    ERROR_CHECK(sret,-1,"write");

    close(fd);

    return 0;
}

