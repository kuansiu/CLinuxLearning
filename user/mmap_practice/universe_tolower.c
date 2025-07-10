#include <my_header.h>

/* Usage:  */
int main(int argc, char *argv[]){                                  
    ARGS_CHECK(argc,2);
    int fd = open(argv[1],O_RDWR);
    ERROR_CHECK(fd,-1,"open");

    struct stat sb;
    int ret = fstat(fd,&sb);
    ERROR_CHECK(ret,-1,"fstat");
    off_t filesize = sb.st_size;

    char *buf = malloc(filesize);
    ERROR_CHECK(buf,NULL,"malloc");

    ssize_t total = 0;
    while(total<filesize){
        ssize_t r =read(fd,buf+total,filesize-total);
        ERROR_CHECK(r,-1,"read");
        if(r==0){
            break;
        }

        total+=r;
    }

    for(off_t i=0;i<total;i++){
        if(isupper(buf[i])){
            buf[i]=tolower(buf[i]);
        }
    }

    lseek(fd,0,SEEK_SET);
    ssize_t written =0;
    while(written<total){
        ssize_t w = write(fd,buf+written,total-written);
        ERROR_CHECK(w,-1,"write");
        written+=w;
    }

    free(buf);
    close(fd);

    return 0;
}

