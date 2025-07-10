#include <my_header.h>
#define CHUNK_SIZE (16*1024*1024)//16MB
/* Usage:  */
int main(int argc, char *argv[]){                                  
    ARGS_CHECK(argc,3);
    int fd_r = open(argv[1],O_RDONLY);
    ERROR_CHECK(fd_r,-1,"open fdr");

    int fd_w = open(argv[2],O_RDWR|O_TRUNC|O_CREAT,0666);
    ERROR_CHECK(fd_w,-1,"open fdw");

    struct stat st;
    int ret = fstat(fd_r,&st);
    ERROR_CHECK(ret,-1,"fstat");
    off_t filesize = st.st_size;

    int fret = ftruncate(fd_w,filesize);
    ERROR_CHECK(fret,-1,"ftruncate");

    off_t offset = 0;
    while(offset<filesize){
        size_t len = ((filesize-offset)>CHUNK_SIZE)?CHUNK_SIZE:(filesize-offset);
        void* src_cmp = mmap(NULL,len,PROT_READ,MAP_SHARED,fd_r,offset);
        ERROR_CHECK(src_cmp,MAP_FAILED,"mmap src_cmp");

        void* dest_cmp = mmap(NULL,len,PROT_WRITE,MAP_SHARED,fd_w,offset);
        ERROR_CHECK(dest_cmp,MAP_FAILED,"mmap dest_cmp");
        
        memcpy(dest_cmp,src_cmp,len);

        munmap(src_cmp,len);
        munmap(dest_cmp,len);

        offset+=len;
    }
    close(fd_r);
    close(fd_w);
    return 0;
}

