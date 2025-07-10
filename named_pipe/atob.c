#include <my_header.h>
#define BUF_SIZE 512
#define FIFO_SEND "fifo_a2b"
#define FIFO_RECV "fifo_b2a"
#include<pthread.h>
int send_fd,recv_fd;
void* send_func(void* arg){
    char buf[BUF_SIZE];
    while(1){
        if(fgets(buf,BUF_SIZE,stdin)==NULL){ break;}
        printf("发送...");
        write(send_fd,buf,strlen(buf));
        if(strncmp(buf,"exit",4)==0){ break;}
    }
    return NULL;
}

void* recv_func(void* arg){
    char buf[BUF_SIZE];
    while(1){
        int read_num = read(recv_fd,buf,BUF_SIZE);
        if(read_num>0){
            buf[read_num] = '\0';
            printf("收到:%s",buf);
            fflush(stdout);
            if(strncmp(buf,"exit",4)==0){ break;}
        }
    }
    return NULL;
}

/* Usage:  */
int main(int argc, char *argv[]){                                  
    send_fd = open(FIFO_SEND,O_WRONLY);
    ERROR_CHECK(send_fd,-1,"open a2b");

    recv_fd = open(FIFO_RECV,O_RDONLY);
    ERROR_CHECK(recv_fd,-1,"open b2a");

    pthread_t t_send,t_recv;
    pthread_create(&t_send,NULL,send_func,NULL);
    pthread_create(&t_recv,NULL,recv_func,NULL);

    pthread_join(t_send,NULL);
    pthread_join(t_recv,NULL);

    close(send_fd);
    close(recv_fd);


    return 0;
}

