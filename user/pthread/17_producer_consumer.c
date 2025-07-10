#include <my_header.h>
#define MAX_SIZE 10
typedef struct node_s{
    int data;
    struct node_s *next;
}QueueNode;

typedef struct{
    int size;
    QueueNode *front;
    QueueNode *rear;
}LinkedListQueue;

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
        return rm_data;
    }

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
    return rm_data;
}
int peek_queue(LinkedListQueue *q){
    if(is_empty(q)){
        printf("queue is empty\n");
        exit(1);
    }
    return q->rear->data;
}

void printQueue(LinkedListQueue *q){
    QueueNode* curr = q->front;
    while(curr!=NULL){
        printf("%d->",curr->data);
        curr = curr->next;
    }
    printf("NULL\n");
}

LinkedListQueue *queue;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond_full = PTHREAD_COND_INITIALIZER;
pthread_cond_t cond_empty = PTHREAD_COND_INITIALIZER;

void* producer(void* arg){

}

void* consumer(void* arg){

}


int main(int argc, char *argv[]){                                  

    queue = creat_queue();
    srand(time(NULL));
    bool used[100] = {false};
    int count =0;
    while(count<8){
        int no = rand()%100;
        if(!used[no]){
            used[no] = true;
            enqueue(queue,no);
            count++;
        }
    }
    printQueue(queue);
    pthread_t producer[3],consumer[2];
    int prod_ids[3] = {1,2,3},cons_ids[2] = {1,2};

    for(int i = 0;i<3,i++){
        pthread_create(&producer[i],NULL,producer,&prod_ids[i]);
    }

    for(int i =0;i<2;i++){
        pthread_create(&consumer[i],NULL,consumer,&cons_ids[i]);
    }

    for(int i =0;i<3;i++){
        pthread_join(prod_ids[i],NULL);}

    for(int  i =0;i<2;i++){
        pthread_join(cons_ids[i],NULL);
    }

    dequeue(queue);
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond_full);
    pthread_cond_destroy(&cond_empty);

    return 0;
}

