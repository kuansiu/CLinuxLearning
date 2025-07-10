#include <my_header.h>

/* Usage:  */
int main(int argc, char *argv[]){                                  
    int fd = open("1.pipe",O_RDONLY);
    
    int name_len = 0;
    read(fd,&name_len,sizeof(int));
    char file_name[20]={0};
    read(fd,file_name,name_len);

    char str_buf[1024]={0};
    int conten_len = read(fd,str_buf,sizeof(str_buf));
    mkdir("test",0666);
    chdir("test");
    int new_fd = open(file_name,O_RDWR|O_CREAT|O_TRUNC,0777);

    write(new_fd,str_buf,conten_len);

    return 0;
}

