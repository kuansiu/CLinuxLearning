#include <my_header.h>
#define MAX_SIZE 10

bool used[100] = {false};
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond_full = PTHREAD_COND_INITIALIZER;
pthread_cond_t cond_empty = PTHREAD_COND_INITIALIZER;

typedef struct node_s{
    int data;
    struct node_s *next;
}QueueNode;

typedef struct{
    int size;
    QueueNode *front;
    QueueNode *rear;
}LinkedListQueue;

LinkedListQueue *queue;

LinkedListQueue *creat_queue(){
    LinkedListQueue *q = calloc(1,sizeof(LinkedListQueue));
    return q;
}

void destroy_queue(LinkedListQueue *q){
    QueueNode *curr = q->front;
    while(curr != NULL){
        QueueNode *temp = curr->next;
        free(curr);
        curr = temp;
    }
    free(q);
}

bool is_empty(LinkedListQueue *q){
    return q->size == 0;
}

bool is_full(LinkedListQueue *q){
    return q->size == MAX_SIZE;
}
bool enqueue(LinkedListQueue *q,int element){
    QueueNode *new_node = malloc(sizeof(QueueNode));
    ERROR_CHECK(new_node,NULL,"malloc new_node");

    new_node->data = element;
    new_node->next = q->front;
    q->front = new_node;
    if(q->rear == NULL){
        q->rear = new_node;
    }
    q->size++;
    return true;
}

int dequeue(LinkedListQueue *q){
    if(is_empty(q)){
        printf("queue is empty\n");
        exit(1);
    }
    //如果只有一个节点,清空队列
    if(q->front==q->rear){
        QueueNode* temp = q->rear;
        int rm_data = temp->data;
        q->front = q->rear = NULL;
        free(temp);
        q->size--;
        return rm_data;
    }else{

        //多个节点,找到倒数第二个节点
        QueueNode *curr = q->front;
        while(curr->next != q->rear){
            curr = curr->next;
        }
        QueueNode *temp = q->rear;
        int rm_data = temp->data;
        curr->next = NULL;
        q->rear = curr;
        free(temp);
        q->size--;
        return rm_data;
    }
}

void printQueue(LinkedListQueue *q){
    QueueNode* curr = q->front;
    while(curr!=NULL){
        printf("%d->",curr->data);
        curr = curr->next;
    }
    printf("NULL\n");
}


void* producer_thread1(void* arg){
    int id = *(int*)arg;

    while(1){
        sleep(3);
        pthread_mutex_lock(&mutex);

        while(is_full(queue)){
            pthread_cond_wait(&cond_full,&mutex);
        }
        int no = rand()%100;
        while(1){
            if(!used[no]){
                used[no] = true;
                enqueue(queue,no);
                break;    
            }
        }
        printf("producer %d produce product: %d,queue size %d\n",id,no,queue->size);
        printf("当前queue:\n");
        printQueue(queue);
        pthread_cond_signal(&cond_empty);
        pthread_mutex_unlock(&mutex);
        
    }
    return NULL;
}


void* producer_thread2(void* arg){
    int id = *(int*)arg;

    while(1){
        sleep(3);
        pthread_mutex_lock(&mutex);

        while(is_full(queue)){
            pthread_cond_wait(&cond_full,&mutex);
        }
        int no = rand()%100;
        while(1){
            if(!used[no]){
                used[no] = true;
                enqueue(queue,no);
                break;    
            }
        }
        printf("producer %d produce product: %d,queue size %d\n",id,no,queue->size);
        printf("当前queue:\n");
        printQueue(queue);
        pthread_cond_signal(&cond_empty);
        pthread_mutex_unlock(&mutex);
        
    }
    return NULL;
}



void* producer_thread3(void* arg){
    int id = *(int*)arg;

    while(1){
        sleep(3);
        pthread_mutex_lock(&mutex);

        while(is_full(queue)){
            pthread_cond_wait(&cond_full,&mutex);
        }
        int no = rand()%100;
        while(1){
            if(!used[no]){
                used[no] = true;
                enqueue(queue,no);
                break;    
            }
        }
        printf("producer %d produce product: %d,queue size %d\n",id,no,queue->size);
        printf("当前queue:\n");
        printQueue(queue);
        pthread_cond_signal(&cond_empty);
        pthread_mutex_unlock(&mutex);
        
    }
    return NULL;
}


void* consumer_thread1(void* arg){
    int id = *(int*)arg;
    sleep(5);
    while(1){
        sleep(1);
        pthread_mutex_lock(&mutex);
        while(is_empty(queue)){
            pthread_cond_wait(&cond_empty,&mutex);
        }

        int num = dequeue(queue);
        used[num] = false;

        printf("consumer %d consume product: %d ,queue size %d\n",id,num,queue->size);
        printf("当前queue:\n");                                                                                                                          
        printQueue(queue);

        
        pthread_cond_signal(&cond_full);
        pthread_mutex_unlock(&mutex);

    }
    return NULL;
}

void* consumer_thread2(void* arg){
    int id = *(int*)arg;
    sleep(5);
    while(1){
        sleep(1);
        pthread_mutex_lock(&mutex);
        while(is_empty(queue)){
            pthread_cond_wait(&cond_empty,&mutex);
        }

        int num = dequeue(queue);
        used[num] = false;

        printf("consumer %d consume product: %d ,queue size %d\n",id,num,queue->size);
        printf("当前queue:\n");                                                                                                                          
        printQueue(queue);

        
        pthread_cond_signal(&cond_full);
        pthread_mutex_unlock(&mutex);

    }
    return NULL;
}

void *(*producers[3])(void *) = {
    producer_thread1,
    producer_thread2,
    producer_thread3
};
void *(*consumers[2])(void *) = {
    consumer_thread1,
    consumer_thread2
};
int main(int argc, char *argv[]){                                  

    queue = creat_queue();
    srand(time(NULL));
    int count =0;
    while(count<8){
        int no = rand()%100;
        if(!used[no]){
            used[no] = true;
            enqueue(queue,no);
            count++;
        }
    }
    printf("队列初始化结束,队列大小%d\n",queue->size);
    printQueue(queue);


    pthread_t producer[3],consumer[2];
    int prod_ids[3] = {1,2,3},cons_ids[2] = {1,2};

    for(int i = 0;i<3;i++){
        pthread_create(&producer[i],NULL,producers[i],&prod_ids[i]);
    }

    for(int i =0;i<2;i++){
        pthread_create(&consumer[i],NULL,consumers[i],&cons_ids[i]);
    }

    for(int i =0;i<3;i++){
        pthread_join(producer[i],NULL);}

    for(int  i =0;i<2;i++){
        pthread_join(consumer[i],NULL);
    }

    dequeue(queue);
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond_full);
    pthread_cond_destroy(&cond_empty);

    return 0;
}

