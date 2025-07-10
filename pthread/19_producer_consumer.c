#include <my_header.h>
#define MAX_SIZE 10
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
bool used[100] = {false};

typedef struct node_s{
    int data;
    struct node_s* next;
}node_t;

typedef struct queue_s{
    node_t* front;
    node_t* rear;
    int size;
}queue_t;

queue_t* Queue;

queue_t* creatQueue(){
    queue_t* queue = calloc(1,sizeof(queue_t));
    return queue;
}

bool is_empty(queue_t* queue){
    return queue->size == 0;
}

bool is_full(queue_t* queue){
    return queue->size==MAX_SIZE;
}

int enQueue(queue_t *queue,int data){

    node_t* newnode = (node_t*)malloc(sizeof(node_t));
    newnode->next = NULL;
    newnode->data = data;
    //如果队列为空,则节点直接为头指针,尾指针
    if(queue->front == NULL){
        queue->front = newnode;
        queue->rear = newnode;
        //如果不是空队列,则尾插入
    }else{
        queue->rear->next = newnode;
        queue->rear = newnode;
    }

    (queue->size)++;
    return 0;
}

int deQueue(queue_t* queue){
    //如果队列为空,直接退出
    if(queue->front == NULL){
        printf("queue is empty,exit\n");
        exit(1);
        //如果队列只有一个元素,清空队列,头尾均置NULL
    }else if(queue->front == queue->rear){
        node_t* tmp = queue->rear;
        int rmdata = tmp->data;
        queue->front = queue->rear = NULL;
        free(tmp);
        (queue->size)--;
        return rmdata;
        //有多个元素,删除第一个头节点
    }else{
        node_t* curr = queue->front;
        queue->front = curr->next;
        (queue->size)--;
        int rmdata = curr->data;
        free(curr);
        return rmdata;
    }
}

void printQueue(queue_t* queue){
    node_t* curr = queue->front;
    while(curr!=NULL){
        printf("%d->",curr->data);
        curr = curr->next;
    }
    printf("NULL\n");
}


/* int main(){ */

/*     queue = creatQueue(); */

/*     for(int i = 0;i<10;i++){ */
/*         enQueue(queue,i*i); */
/*     } */
/*     printQueue(queue); */


/*     return 0; */
/* } */

void* producer(void* arg){

    while(1){
        pthread_mutex_lock(&mutex);

        while(Queue->size>=10){
            printf("queue is full\n");
            pthread_cond_wait(&cond,&mutex);
        }


        int num = rand()%100;
        while(1){
            if(!used[num]){
                enQueue(Queue,num);
                used[num]=true;
                break;
            }
        }
        printf("produce product:%d,queue size %d\n",num,Queue->size);
        printf("当前queue:");
        printQueue(Queue);
        pthread_cond_broadcast(&cond);
        pthread_mutex_unlock(&mutex);
        sleep(3);
    }

    pthread_exit(NULL);

}

void* consumer(void* arg){
    sleep(5);
    while(1){
        pthread_mutex_lock(&mutex);
        while(Queue->size<=0){
            pthread_cond_wait(&cond,&mutex);
        }

        int num = deQueue(Queue);
        used[num] = false;
        printf("consume product:%d,queue size %d\n",num,Queue->size);
        printf("当前queue:");
        printQueue(Queue);

        pthread_cond_broadcast(&cond);
        pthread_mutex_unlock(&mutex);
        sleep(1);
    }
    pthread_exit(NULL);
}

int main(){
    Queue = creatQueue();
    int count = 0;
    srand(time(NULL));
    while(count<8){
        int num = rand()%100;
        if(!used[num]){
            enQueue(Queue,num);
            used[num] = true;
            count++;
        }

    }
    printQueue(Queue);
    pthread_t protid[3],constid[2];
    for(int i = 0;i<3;i++){
        pthread_create(&protid[i],NULL,producer,NULL);
    }

    for(int i = 0;i<2;i++){
        pthread_create(&constid[i],NULL,consumer,NULL);
    }

    for(int i =0;i<3;i++){
        pthread_join(protid[i],NULL);}

    for(int i =0;i<2;i++){
        pthread_join(constid[i],NULL);}

    return 0;
}
