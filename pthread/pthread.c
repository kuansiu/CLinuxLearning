#include <my_header.h>
void *threadFunc(void *arg){
    printf("i am child thread,tid = %lu\n",
           pthread_self());
    return NULL;
}
int main(int argc, char *argv[]){                                  
    pthread_t tid;
    int ret = pthread_create(&tid,NULL,threadFunc,NULL); 
    THREAD_ERROR_CHECK(ret,"pthread_create");
    printf("i am main thread,tid = %lu\n",
           pthread_self());
    usleep(20);
    /* sleep(1); */
    return 0;
}

