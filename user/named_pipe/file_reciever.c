#include <my_header.h>

/* Usage:  */
int main(int argc, char *argv[]){                                  
    int pipe_fd = open("file_transfer.pipe",O_RDONLY);
    
    int len =0;
    read(pipe_fd,&len,sizeof(int));
    
    char name[50]={0};
    read(pipe_fd,name,len);

    int str_len = 0;
    read(pipe_fd,&str_len,sizeof(int));

    char conten[1024]={0};
    read(pipe_fd,conten,str_len);

    mkdir(argv[1],0755);
    chdir(argv[1]);

    int new_fd = open(name,O_RDWR|O_CREAT|O_TRUNC,0666);
    
    write(new_fd,conten,str_len);
    printf("recieve file in %s",argv[1]);
    close(pipe_fd);
    close(new_fd);

    return 0;
}

