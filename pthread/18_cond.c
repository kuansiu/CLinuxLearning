#include <my_header.h>
typedef struct shareRes_s{
    pthread_mutex_t mutex;
    int flag;//0 A undone ,1 A done ,2 B done
    pthread_cond_t cond;
}shareRes_t;

void B(){
    printf("B before\n");
    sleep(5);
    printf("B inside\n");
}

void* threadFun(void* arg){
    shareRes_t* pshareRes = (shareRes_t*) arg;
    pthread_mutex_lock(&pshareRes->mutex);
    while(pshareRes->flag != 1){
        pthread_cond_wait(&pshareRes->cond,&pshareRes->mutex);
    }
    pthread_mutex_unlock(&pshareRes->mutex);
    
    B();
    pthread_mutex_lock(&pshareRes.mutex);
    while(pshareRes->flag !=2){
        pthread_cond_wait(&pshareRes->cond,&pshareRes->mutex);
    }
    pthread_mutex_unlock(&pshareRes->mutex);


    pthread_exit(NULL);
}

void A(){
    printf("A before\n");
    sleep(5);
    printf("A inside\n");
}

void C(){
    printf("C bedore\n");
    sleep(5);
    printf("C inside\n");
}

/* Usage:  */
int main(int argc, char *argv[]){                                  
    shareRes_t shareRes;
    pthread_t tidB;
    shareRes.flag = 0;
    pthread_mutex_init(&shareRes.mutex,NULL);
    pthread_cond_init(&shareRes.cond,NULL);
    pthread_create(&tidB,NULL,threadFun,&shareRes);
    A();
    pthread_mutex_lock(&shareRes.mutex);
    shareRes.flag = 1;
    pthread_cond_signal(&shareRes.cond);
    pthread_mutex_unlock(&shareRes.mutex);
    
    pthread_mutex_lock(&shareRes.mutex);
    pthread_cond_
    C();
    
    pthread_mutex_destroy(&shareRes.mutex);
    pthread_cond_destroy(&shareRes.cond);
    
    return 0;
}

