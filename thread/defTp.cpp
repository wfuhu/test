#ifndef _threadpool_cpp_

#define _threadpool_cpp_

#include "defTp.h"
#include "TaskQueue.h"
#include <string.h>
#include<string>
#include<unistd.h>
#include<iostream>
template <typename T>
ThreadPool<T>::ThreadPool(int min, int max)
{
    
    do{
        
        taskQ=new TaskQueue<T>;
         if(taskQ==nullptr)
        {
            std::cout<<"malloc taskQ fail..."<<std::endl;
            break;
        }

        threadIDS=new pthread_t[max];
        if(threadIDS==nullptr)
        {
            std::cout<<"malloc threadIDS fail..."<<std::endl;
            break;
        }
        memset(threadIDS,0,sizeof(pthread_t)*max);
        minNum=min;
        maxNum=max;
        busyNum=0;
        liveNum=min;
        exitNum=0;   
        if(pthread_mutex_init(&mutexPool,NULL)!=0||
        pthread_cond_init(&notEmpty,NULL)!=0)
        {
            std::cout<<"mutex init fail.."<<std::endl;
            break;
        }

        shutdown=false;

        pthread_create(&managerID,NULL,manager,this);
        for (int i = 0; i < min; i++)
        {
            /* code */
            pthread_create(&threadIDS[i],NULL,work,this);
        }
        
        return ;

    }while(0);

    if(threadIDS) delete[] threadIDS;
    if(taskQ) delete taskQ;


}
template <typename T>
ThreadPool<T>::~ThreadPool()
{
    shutdown=true;
    pthread_join(managerID,NULL);
    for (size_t i = 0; i < liveNum; i++)
    {
        pthread_cond_signal(&notEmpty);
    }

    if(taskQ)
    {
        delete taskQ;
    }
    
    if(threadIDS)
    {
        delete threadIDS;
    }

    pthread_mutex_destroy(&mutexPool);
    pthread_cond_destroy(&notEmpty);



}
template <typename T>
void ThreadPool<T>::addTask(Task<T> t)
{
    pthread_mutex_lock(&mutexPool);
    if(shutdown)
    {
         return ;
    }
    taskQ->addTask(t);
     pthread_cond_signal(&notEmpty);


}
template <typename T>
int ThreadPool<T>::getBusyNum()
{
    pthread_mutex_lock(&mutexPool);
    int busyNumber=this->busyNum;
    pthread_mutex_unlock(&mutexPool);
    return busyNum;

}
template <typename T>
int ThreadPool<T>::getAliveNum()
{
    pthread_mutex_lock(&mutexPool);
    int liveNumber=this->liveNum;
    pthread_mutex_unlock(&mutexPool);
    return liveNum;
}
template <typename T>
 void *ThreadPool<T>::work(void* arg)
{
    ThreadPool* pool=static_cast<ThreadPool*>(arg);

    while (1)
    {
        pthread_mutex_lock(&pool->mutexPool);
        while (pool->taskQ->taskNumber()==0&&!pool->shutdown)
        {
            pthread_cond_wait(&pool->notEmpty,&pool->mutexPool);
            if(pool->exitNum>0)
            {
                pool->exitNum--;
                if(pool->liveNum>pool->minNum)
                {
                    pool->liveNum--;
                    pthread_mutex_unlock(&pool->mutexPool);
                    pool->threadExit();
                }
            }
        }
        

            if(pool->shutdown)
        {
            pthread_mutex_unlock(&pool->mutexPool);
            pool->threadExit();
        }
        Task<T> task=pool->taskQ->takeTask();

        pool->busyNum++;
        pthread_mutex_unlock(&pool->mutexPool);

        std::cout<<"thread "+pthread_self() +std::string("working...")<<std::endl;
        
        
        task.function(task.arg);
        delete task.arg;
        task.arg=nullptr;

        pthread_mutex_lock(&pool->mutexPool);
        pool->busyNum--;
        pthread_mutex_unlock(&pool->mutexPool);
    }

    return NULL;
}
template <typename T>
 void *ThreadPool<T>::manager(void *arg)
{
    ThreadPool* pool=static_cast<ThreadPool*>(arg);
    while (!pool->shutdown)      
    {
        sleep(3);
        pthread_mutex_lock(&pool->mutexPool);
        int queueSize=pool->taskQ->taskNumber();
        int liveNum=pool->liveNum;
        pthread_mutex_unlock(&pool->mutexPool);

        if(queueSize>liveNum&&liveNum<pool->maxNum)
        {
            pthread_mutex_lock(&pool->mutexPool);
            int count=0;
            for (size_t i = 0; i < pool->maxNum&&count<NUMBER
            &&pool->liveNum<pool->maxNum; i++)
            {
                if(pool->threadIDS[i]==0)
                {
                    pthread_create(&pool->threadIDS[i],NULL,work,pool);
                    count++;
                    pool->liveNum++;
                }
                /* code */
            }
            pthread_mutex_unlock(&pool->mutexPool);
            
        }

        if(pool->busyNum*2<liveNum&&liveNum>pool->minNum)
        {
            pthread_mutex_lock(&pool->mutexPool);
            pool->exitNum=NUMBER;
            pthread_mutex_unlock(&pool->mutexPool);
            for (size_t i = 0; i < NUMBER; i++)
            {
                pthread_cond_signal(&pool->notEmpty);
            }
            
        }

       
    }
    

    return NULL;
}
template <typename T>
void ThreadPool<T>::threadExit()
{
    pthread_t tid=pthread_self();
    for (size_t i = 0; i < maxNum; i++)
    {
        if(threadIDS[i]==0)
        threadIDS[i]=0;
        std::cout<<"exit"+tid<<std::endl;
        break;
    }
    
    pthread_exit(NULL);
    
}

#endif
