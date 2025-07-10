#include <my_header.h>


typedef struct shareRes_s{
    int flag;
    pthread_mutex_t mutex;
}shareRes_t;
void* threadFun(void* arg){
    shareRes_t *pshareRes = (shareRes_t*)arg;

    while(1){
        if(pshareRes->flag == 1){
            printf("Before B!\n");

            sleep(3);

            printf("After B\n");
            break;
        }
    }
    pthread_exit(NULL);
}
/* Usage:  */
int main(int argc, char *argv[]){                                  

    shareRes_t shareRes;

    shareRes.flag = 0;
    pthread_mutex_init(&shareRes.mutex,NULL);

    pthread_t tid;

    pthread_create(&tid,NULL,threadFun,&shareRes);

    while(1){
        if(shareRes.flag == 0){
            printf("Before A!\n");

            sleep(3);


            printf("After A\n");
            pthread_mutex_lock(&shareRes.mutex);
            shareRes.flag =1;
            pthread_mutex_unlock(&shareRes.mutex);
            break;
        }
    }
    pthread_join(tid,NULL);
    return 0;
}

