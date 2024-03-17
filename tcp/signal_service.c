#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<pthread.h>
#include<semaphore.h>

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
        sem_wait(&semp);
        pthread_mutex_lock(&mutex);
        struct Node* newnode=(struct Node*)malloc(sizeof(struct Node));
        newnode->number=rand()%1000;
        newnode->next=head;
        head=newnode;
        printf("id:%ld,%d",pthread_self(),newnode->number);
        sem_post(&semc);
        pthread_mutex_unlock(&mutex);
        sleep(rand()%3);
    }
    
    return NULL;
}
void* consumer(void *args)
{
    while (1)
    {
        sem_wait(&semc);
        pthread_mutex_lock(&mutex);
        struct Node* node=head;
        printf("id:%ld,%d",pthread_self(),node->number);
        head=head->next;
        free(node);
        sem_post(&semp);
        pthread_mutex_unlock(&mutex);
        sleep(rand()%3);
    }
    
    return NULL;
}

sem_t semp;//】生产者信号量
sem_t semc;//】消费者信号量
pthread_mutex_t mutex;
 int main()
 {
    pthread_mutex_init(&mutex,NULL);
    //生产者
    sem_init(&semp,0,1);
    //消费者，初始为为0,阻塞
    sem_init(&semc,0,0);

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
        /* code */
        pthread_join(t1[i],NULL);
        pthread_join(t2[i],NULL);
    }
    


    pthread_mutex_destroy(&mutex);

       sem_destroy(&semc);
       sem_destroy(&semp);

    
    return 0;

 }