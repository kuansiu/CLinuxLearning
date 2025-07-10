#include <my_header.h>

void* threadFunc(void* arg){
    int fd = *(int *)arg;
    char buf[100];

    printf("fd = %d\n",fd);
    ssize_t byte = read(fd,buf,50);
    buf[byte] = '\0';
    printf("child thread: %s\n",buf);

    return NULL;
}


/* Usage:  */
int main(int argc, char *argv[]){                                  
    int fd = open("test.txt",O_RDONLY,0666);
    ERROR_CHECK(fd,-1,"open");
    
    printf("main thread open file,fd = %d\n",fd);

    pthread_t tid;
    pthread_create(&tid,NULL,threadFunc,&fd);
    pthread_join(tid,NULL);
    
    lseek(fd, 0, SEEK_SET);
    char buffer[100];
    ssize_t byte = read(fd,buffer,50);
    buffer[byte]='\0';
    printf("main read:%s\n",buffer);
    close(fd);
    
    return 0;
}

