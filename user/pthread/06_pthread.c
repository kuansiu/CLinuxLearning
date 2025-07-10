#include <my_header.h>
int global_count = 0;

void *threadFunc_global(void* arg){
    for(int i = 0; i < 10000000; i++){
        global_count++;  // 全局变量版本
    }
    printf("线程 %lu 完成，全局count = %d\n", pthread_self(), global_count);
    return NULL;
}

/* Usage:  */
int main(int argc, char *argv[]){                                  
    pthread_t tid1, tid2;
    
    global_count = 0;
    
    pthread_create(&tid1, NULL, threadFunc_global, NULL);
    pthread_create(&tid2, NULL, threadFunc_global, NULL);
    
    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);
    
    printf("最终结果: %d (期望: 20000000)\n", global_count);
    
    return 0;
}

