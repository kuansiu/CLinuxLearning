#include <my_header.h>
typedef struct shareRes_s{
    pthread_mutex_t mutex;
    int ticketCount;
}shareRes_t;
void* threadFun3(void* arg){
    shareRes_t* pshareRes = (shareRes_t*)arg;
    while(1){
        pthread_mutex_lock(&pshareRes->mutex);
        if(pshareRes->ticketCount<10){
            printf("add 10 ticket before ticketCount :%d\n",pshareRes->ticketCount);
            pshareRes->ticketCount+=10;
            printf("add 10 ticket after ticketCount :%d\n",pshareRes->ticketCount);
            pthread_mutex_unlock(&pshareRes->mutex);
            break;
        }else{
            pthread_mutex_unlock(&pshareRes->mutex);
            /* break; */
        }
    }
    printf("add thread exit before\n");
    pthread_exit(NULL);
}

void* threadFun1(void* arg){
    shareRes_t* pshareRes = (shareRes_t*)arg;
    while(1){
        pthread_mutex_lock(&pshareRes->mutex);
        if(pshareRes->ticketCount>0){
            printf("windows1:ready to sell ticket,remain ticket:%d\n",pshareRes->ticketCount);
            pshareRes->ticketCount--;
            printf("windows1:sold 1 ticket,remain ticket:%d\n",pshareRes->ticketCount);
            pthread_mutex_unlock(&pshareRes->mutex);
            sleep(1);
        }else{
            pthread_mutex_unlock(&pshareRes->mutex);
            break;
        }
    }
    printf("winodws1 exit before\n");
    pthread_exit(NULL);
}

void* threadFun2(void* arg){
    shareRes_t* pshareRes = (shareRes_t*)arg;
    while(1){
        pthread_mutex_lock(&pshareRes->mutex);
        if(pshareRes->ticketCount>0){
            printf("windows2:ready to sell ticket,remain ticket:%d\n",pshareRes->ticketCount);
            pshareRes->ticketCount--;
            printf("windows2::sold 1 ticket,remain ticket:%d\n",pshareRes->ticketCount);
            pthread_mutex_unlock(&pshareRes->mutex);
            sleep(1);
        }else{ 
            pthread_mutex_unlock(&pshareRes->mutex);
            break;
        }
    }
    printf("winodws2 exit before\n");
    pthread_exit(NULL);
}
int main(int argc, char *argv[]){                                  

    shareRes_t shareRes;
    shareRes.ticketCount = 20;
    pthread_t tid1,tid2,tid3;
    pthread_mutex_init(&shareRes.mutex,NULL);
    pthread_create(&tid1,NULL,threadFun1,&shareRes);
    pthread_create(&tid2,NULL,threadFun2,&shareRes);
    pthread_create(&tid3,NULL,threadFun3,&shareRes);
    pthread_join(tid1,NULL);
    pthread_join(tid2,NULL);
    pthread_join(tid3,NULL);
    printf("ticketCount = %d",shareRes.ticketCount);
    return 0;
}

