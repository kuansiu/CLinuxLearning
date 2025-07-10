#include <my_header.h>
typedef struct shareRes_s{
    int data;
    pthread_mutex_t mutex;
}shareRes_t;


void* threadFun(void*arg){
    shareRes_t *pshareRes = (shareRes_t*)arg;

    for(int i=0;i<20000000;i++){
        pthread_mutex_lock(&pshareRes->mutex);
        (pshareRes->data)++;
        pthread_mutex_unlock(&pshareRes->mutex);
    }
    pthread_exit(NULL);
}
/* Usage:  */
int main(int argc, char *argv[]){                                  
    shareRes_t shareRes;
    shareRes.data =0;
    pthread_mutex_init(&shareRes.mutex,NULL);
    struct timeval beginTime,endTime;
    gettimeofday(&beginTime,NULL);
    pthread_t tid;
    int ret = pthread_create(&tid,NULL,threadFun,&shareRes);
    THREAD_ERROR_CHECK(ret,"pthread_create");
    for(int i = 0;i<20000000;i++){
        pthread_mutex_lock(&shareRes.mutex);
        (shareRes.data)++;
        pthread_mutex_unlock(&shareRes.mutex);
    }

    pthread_join(tid,NULL);
    gettimeofday(&endTime,NULL);
    long long elapsed = endTime.tv_usec-beginTime.tv_usec+(endTime.tv_sec-beginTime.tv_sec)*1000000LL;
    double avg_us = (double)elapsed / 40000000.0;
    printf("num = %d,time = %lld us\n",
           shareRes.data,elapsed); 
    printf("avg = %.2lf ns",avg_us*1000);
    return 0;
}

