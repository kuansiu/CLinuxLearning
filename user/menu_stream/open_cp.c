#include <my_header.h>

/* Usage:  */
int main(int argc, char *argv[]){                                  
    ARGS_CHECK(argc,3);
    int fdr =open(argv[1],O_RDONLY,0666);
    ERROR_CHECK(fdr,-1,"open read  failed");
    int fdw = open(argv[2],O_WRONLY|O_CREAT|O_TRUNC,0666);
    ERROR_CHECK(fdw,-1,"open write failed");
    char buf[1024]={0};
    
    ssize_t ret;
    while((ret = read(fdr,buf,sizeof(buf)-1))>0){
        write(fdw,buf,ret);
    }
    close(fdr);
    close(fdw);

    return 0;
}

