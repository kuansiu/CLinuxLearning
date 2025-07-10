#include <my_header.h>

typedef struct node_s{
    int data;
    struct node_s* next;
}node_t;

typedef struct linklist_s{
    node_t *head;
    node_t *tail;
    int size;
}linklist_t;

void createlist(linklist_t *list){
    list->head=NULL;
    list->tail=NULL;
    list->size = 0;
}

void inserttail(linklist_t* list,int data){
    node_t* newnode = (node_t*)malloc(sizeof(node_t));
    newnode->data = data;
    newnode->next=NULL;
    if(list->head==NULL){
        list->head = newnode;
        list->tail=newnode;
    }else{
        list->tail->next = newnode;
        list->tail = newnode;
    }

    list->size++;
}

void printlist(linklist_t *list){
    node_t* curr = list->head;
    while(curr!=NULL){
        printf("%d -> ",curr->data);
        curr = curr->next;
    }
    printf("NULL\n");
}

/* void freelist(linklist_t *list){ */
/*     node_t *curr = list->head; */
/*     while(curr!=NULL){ */
/*         node_t *tmp = curr; */
/*         curr = curr->next; */
/*         free(tmp); */
/*     } */
/*     list->head =NULL; */
/*     list->tail=NULL; */
/*     list->size=0; */
/* } */

void removefront(linklist_t *list){
    if(list->head==NULL){
        return;
    }
    node_t *tmp = list->head;
    list->head = list->head->next;
    if(list->head==NULL){
        list->tail=NULL;
    }
    free(tmp);
    list->size--;
}

typedef struct shareRes_s{
    pthread_mutex_t mutex;
    linklist_t *list;
}shareRes_t;

void* threadFun(void* arg){
    shareRes_t* pshareRes = (shareRes_t*)arg;
    for(int i = 90;i<100;i++){
        pthread_mutex_lock(&pshareRes->mutex);    
        inserttail(pshareRes->list,i);
        pthread_mutex_unlock(&pshareRes->mutex);
    }
    printlist(pshareRes->list);
    pthread_exit(NULL);
}
/* Usage:  */
int main(int argc, char *argv[]){                                  
    linklist_t list;
    shareRes_t shareRes;
    shareRes.list = &list;
    createlist(shareRes.list);
    pthread_mutex_init(&shareRes.mutex,NULL);

    pthread_t tid;
    pthread_create(&tid,NULL,threadFun,&shareRes);

    pthread_join(tid,NULL);
    while(shareRes.list->head!=NULL){
        pthread_mutex_lock(&shareRes.mutex);
        removefront(shareRes.list);
        pthread_mutex_unlock(&shareRes.mutex);
    }
    
    printlist(shareRes.list);

    return 0;
}

