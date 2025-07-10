#include <my_header.h>
void* threadFun(void* arg){
    printf("i am child thread,tid = %lu\n",pthread_self());
    char *tret = (char*)malloc(20);
    return (void*)tret;
}
/* Usage:  */
int main(int argc, char *argv[]){                                  
    pthread_t tid,tid2,tid3;
    int ret = pthread_create(&tid,NULL,threadFun,NULL);
    THREAD_ERROR_CHECK(ret,"pthread_create1");
    ret = pthread_create(&tid2,NULL,threadFun,NULL);
    THREAD_ERROR_CHECK(ret,"pthraed_creat2");
    ret = pthread_create(&tid3,NULL,threadFun,NULL);
    THREAD_ERROR_CHECK(ret,"pthread_create3");

    printf("main thread,tid = %lu\n",pthread_self());
    void* tret;
    ret = pthread_join(tid,&tret);
    THREAD_ERROR_CHECK(ret,"pthread_join");
    printf("tret = %ld\n",(long)tret);
    
    ret = pthread_join(tid2,&tret);
    THREAD_ERROR_CHECK(ret,"pthread_join");
    printf("tret = %ld\n",(long)tret);
    
    ret = pthread_join(tid3,&tret);
    THREAD_ERROR_CHECK(ret,"pthread_join");
    printf("tret = %ld\n",(long)tret);
    return 0;
}

