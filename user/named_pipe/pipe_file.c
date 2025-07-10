#include <my_header.h>

/* Usage:  */
int main(int argc, char *argv[]){                                  
    ARGS_CHECK(argc,2);
    int pipe_fd = open("1.pipe",O_WRONLY);
    ERROR_CHECK(pipe_fd,-1,"open pipe");


    int file_fd = open(argv[1],O_RDONLY);
    ERROR_CHECK(file_fd,-1,"open file");

    char conten[1024];
    int byte_read = read(file_fd,conten,sizeof(conten)); 
    ERROR_CHECK(byte_read,-1,"read file");

    char file_name[20]={0};
    int  name_len =  strlen(argv[1]);
    strcpy(file_name,argv[1]);

    write(pipe_fd,&name_len,sizeof(int));
    write(pipe_fd,file_name,name_len);
    write(pipe_fd,conten,byte_read);    


    close(file_fd);
    close(pipe_fd);

    return 0;

}

