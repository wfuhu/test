#include<pthread.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>

struct Test
{
    int num;
    int age;
};

struct Test t;//1

pthread_mutex_t mutex;//互斥锁
int num=0;//共享资源

void* callback(void* arg)
{
    for(int i=0;i<5;i++)
    {
    pthread_mutex_lock(&mutex);
    printf("自线程%d\n",i);
    num++;
    pthread_mutex_unlock(&mutex);
    }

    struct Test* t1=(struct Test*)arg;//2
    


    
    printf("子线程%ld\n",pthread_self());
    //pthread_exit(&t);//1为取地址，2为t
    return NULL;//可传递数据

}


void* callbacka(void* arg)
{
    for(int i=0;i<5;i++)
    {
    pthread_mutex_lock(&mutex);
    printf("自线程%d\n",i);
    num++;
    pthread_mutex_unlock(&mutex);
    }

    struct Test* t1=(struct Test*)arg;//2
    


    
    printf("子线程%ld\n",pthread_self());
    //pthread_exit(&t);//1为取地址，2为t
    return NULL;//可传递数据

}


int main()
{
    pthread_t tid;
    pthread_t tid0;
    struct Test* t0;//2

    pthread_mutex_init(&mutex,NULL);//互斥锁初始化

    pthread_create(&tid,NULL,callback,&t0);//2,第四个参数为传入数据
     pthread_create(&tid0,NULL,callbacka,&t0);
    for(int i=0;i<5;++i)
    {
        printf("主线程:%d\n",i);
    }
    printf("主线程%ld\n",pthread_self());
    //pthread_exit(null)退出线程，不回收资源

    pthread_detach(tid);//线程分离，子线程不会被回收资源

    pthread_cancel(tid);//杀死子线程

    void* ptr;
    pthread_join(tid,ptr);//1,线程堵塞，等待结束】子线程退出，第二个参数为该线程传出数据
    pthread_join(tid0,NULL);



    struct Test* pt=(struct Test*)pt;//1

    pthread_mutex_destroy(&mutex);//互斥锁销毁

    sleep(5);
    return 0;
}    