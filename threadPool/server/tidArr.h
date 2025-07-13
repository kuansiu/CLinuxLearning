#ifndef __TID_ARR__
#define __TID_ARR__
#include<my_header.h>
typedef struct tidArr_s
{
    pthread_t *tidArr;
    int tidArrSize;
}tidArr_t;

int tidArrInit(int workerNum,tidArr_t* tidArr);

#endif