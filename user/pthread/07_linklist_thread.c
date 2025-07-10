#include <my_header.h>

typedef struct Node{
    int data;
    struct Node* next;
}Node;

Node* createNode(int data){
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->data = data;
    newNode->next = NULL;
    return newNode;
}

void* insertNode(Node** head,int data){
    Node* newNode = createNode(data);
    newNode->next = *head;
    *head = newNode;
}

void* traverseList(void* arg){
    Node* head = (Node*)arg;
    printf("child thread %lu start traverse list:\n",pthread_self());

    if(head == NULL){
        printf("list empty\n");
        return NULL;
    }

    Node* curr = head;
    int count = 0;

    while(curr!=NULL){
        printf("Node %d: Data = %d\n",++count,curr->data);
        curr = curr->next;
    }

    printf("child tread traverse finish, total Node :%d",count);
    return NULL;
}

void freeList(Node* head){
    Node* curr = head;
    while(curr!=NULL){
        Node* temp = curr;
        curr = curr->next;
        free(temp);
    }
}

void printList(Node* head){
    printf("main create list:");
    Node* curr = head;
    while(curr !=NULL){
        printf("%d -> ",curr->data);
        curr = curr->next;
    }
    printf("NULL\n");
}


/* Usage:  */
int main(int argc, char *argv[]){                                  
    
    pthread_t tid;

    Node* head = NULL;

    insertNode(&head,10);
    insertNode(&head,70);
    insertNode(&head,60);
    insertNode(&head,40);
    insertNode(&head,90);
    insertNode(&head,30);
    insertNode(&head,50);
    
    printList(head);

    int ret = pthread_create(&tid,NULL,traverseList,(void*)head);
    
    THREAD_ERROR_CHECK(ret,"create");

    pthread_join(tid,NULL);

    printf("clean main thread resource\n");
    freeList(head);

    return 0;
}

