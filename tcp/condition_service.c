#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<pthread.h>

struct Node
{
    int number;
    struct Node* next;
    
};
struct Node* head=NULL;

void* producer(void *args)
{
    while (1)
    {
        pthread_mutex_lock(&mutex);
        struct Node* newnode=(struct Node*)malloc(sizeof(struct Node));
        newnode->number=rand()%1000;
        newnode->next=head;
        head=newnode;
        printf("id:%ld,%d",pthread_self(),newnode->number);
        pthread_mutex_unlock(&mutex);
        pthread_cond_broadcast(&cond);//解除阻塞
        sleep(rand()%3);
    }
    
    return NULL;
}
void* consumer(void *args)
{
    while (1)
    {
        pthread_mutex_lock(&mutex);
        while (head==NULL)
        {
            pthread_cond_wait(&cond,&mutex);//为空时，启动阻塞
        }
        
        struct Node* node=head;
        printf("id:%ld,%d",pthread_self(),node->number);
        head=head->next;
        free(node);
        pthread_mutex_unlock(&mutex);
        sleep(rand()%3);
    }
    
    return NULL;
}

 pthread_cond_t cond;//控制变量
 pthread_mutex_t mutex;//、互斥索
 int main()
 {
    pthread_mutex_init(&mutex,NULL);
    pthread_cond_init(&cond,NULL);
    pthread_t t1[5],t2[5];
    for (size_t i = 0; i < 5; i++)
    {
        pthread_create(&t1[i],NULL,producer,NULL);

    }

    for (size_t i = 0; i < 5; i++)
    {
        pthread_create(&t2[i],NULL,consumer,NULL);
        
    }

    for (size_t i = 0; i < 5; i++)
    {
        pthread_join(t1[i],NULL);
        pthread_join(t2[i],NULL);
    }

   

    
    




    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);
    return 0;

 }