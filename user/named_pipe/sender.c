#include <my_header.h>

/* Usage:  */
int main(int argc, char *argv[]){                                  
    int pipe_fd = open("file_transfer.pipe",O_WRONLY);

    int file_fd = open(argv[1],O_RDONLY);
    char name[50] ={0};
    strcpy(name,argv[1]);
    int len = strlen(argv[1]);
    char conten[1024] ={0};
    int conten_len = read(file_fd,conten,sizeof(conten));
    printf("transfering\n");
    write(pipe_fd,&len,sizeof(int));
    write(pipe_fd,name,len);
    write(pipe_fd,&conten_len,sizeof(int));
    write(pipe_fd,conten,conten_len);
    printf("transfered\n");
    return 0;
}

