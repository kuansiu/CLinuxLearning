#include <my_header.h>
typedef struct shareRes_s{
    pthread_mutex_t mutex;
    int flag;
    pthread_cond_t cond;
}shareRes_t;

void B(){
    printf("B func inside\n");
    sleep(5);
}

void* threadFun(void* arg){
    shareRes_t* pshareRes = (shareRes_t*) arg;
    pthread_mutex_lock(&pshareRes->mutex);
    while(pshareRes->flag != 1){
        pthread_cond_wait(&pshareRes->cond,&pshareRes->mutex);
    }
    pthread_mutex_unlock(&pshareRes->mutex);
    printf("B before\n");
    B();
    pthread_exit(NULL);
}

void A(){
    printf("A inside\n");
    sleep(5);
}

void C(){
    printf("C inside\n");
    sleep(5);
}

/* Usage:  */
int main(int argc, char *argv[]){                                  
    shareRes_t shareRes;
    pthread_t tidB;
    shareRes.flag = 0;
    pthread_mutex_init(&shareRes.mutex,NULL);
    pthread_cond_init(&shareRes.cond,NULL);
    pthread_create(&tidB,NULL,threadFun,&shareRes);
    printf("A before\n");
    A();
    pthread_mutex_lock(&shareRes.mutex);
    shareRes.flag = 1;
    pthread_cond_signal(&shareRes.cond);
    pthread_mutex_unlock(&shareRes.mutex);

    pthread_join(tidB,NULL);
    printf("C bedore\n");
    C();
    
    pthread_mutex_destroy(&shareRes.mutex);
    pthread_cond_destroy(&shareRes.cond);
    
    return 0;
}

