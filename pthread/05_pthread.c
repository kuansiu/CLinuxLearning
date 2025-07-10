#include <my_header.h>
void *threadFunc(void* arg){
    int *count = (int*)arg;
    for(int i=0;i<10000000;i++){
        (*count)++;
    }
    printf("count = %d\n",*count);
    return NULL;
}
int main(int argc, char *argv[]){                                  
    pthread_t tid1;
    pthread_t tid2;
    
    int *count = (int*)malloc(sizeof(int));
    *count = 0;
    
    pthread_create(&tid1,NULL,threadFunc,count);
    pthread_create(&tid2,NULL,threadFunc,count);
    
    sleep(10);
    printf("i am main thread\n");
    free(count);
    
    
    return 0;
}

