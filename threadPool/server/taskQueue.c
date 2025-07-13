#include"taskQueue.h"

int taskQueueInit(taskQueue_t* taskQueue){
    taskQueue->front = NULL;
    taskQueue->rear = NULL;
    taskQueue->queueSize = 0;
    return 0;
}

int enQueue(taskQueue_t* taskQueue,int netfd){
    node_t* newNode = (node_t*)calloc(1,sizeof(node_t));
    newNode->netfd = netfd;
    if(taskQueue->queueSize == 0){
        taskQueue->front=newNode;
        taskQueue->rear=newNode;
    }else{
        taskQueue->rear->next=newNode;
        taskQueue->rear=newNode;
    }
    (taskQueue->queueSize)++;
    return 0;
}

int deQueue(taskQueue_t *taskQueue){
    node_t* node = taskQueue->front;
    taskQueue->front = node->next;
    if(taskQueue->queueSize == 1){
        taskQueue->rear = NULL;
    }
    free(node);
    (taskQueue->queueSize)--;
    return 0;
}

int printQueue(taskQueue_t *taskQueue){
    node_t *node = taskQueue->front;
    while(node != NULL){
        printf("%d",node->netfd);
        node = node->next;
    }
    printf("\n");
    return 0;
}