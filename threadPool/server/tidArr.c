#include"tidArr.h"

int tidArrInit(int workerNum,tidArr_t* tidArr){
    tidArr->tidArrSize = workerNum;
    tidArr->tidArr=(pthread_t*)calloc(workerNum,sizeof(pthread_t));
    return 0;
}