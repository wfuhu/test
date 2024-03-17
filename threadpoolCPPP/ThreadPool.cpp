#include "ThreadPool.h"
#include<string>
#include <unistd.h>
ThreadPool::ThreadPool(int min, int max)
{
    taskQ=new TaskQueue;
    do
    {
        threadIDS=new pthread_t[max];
        if(threadIDS==nullptr)
        {
            break;
        }

        minNum=min;
        maxNum=max;
        busyNum=0;
        liveNUm=min;
        exitNum=0;


        if(pthread_mutex_init(&mutexpool,NULL)!=0||
        pthread_cond_init(&noempty,NULL)!=0)
        {
            break;
        }

        shutdown=false;

        pthread_create(&managerID,NULL,manager,this);
        for (size_t i = 0; i < min; i++)
        {
            pthread_create(&threadIDS[i],NULL,worker,this);
        }

        return;
        

    } while (0);
    
    if(threadIDS) delete[] threadIDS;
    if(taskQ) delete taskQ;

}

ThreadPool::~ThreadPool()
{
    shutdown=true;
    pthread_join(managerID,NULL);
    for (size_t i = 0; i < liveNUm; i++)
    {
        pthread_cond_signal(&noempty);
    }
    if(taskQ)
    delete  taskQ;
    if(threadIDS)
    delete []threadIDS;

    pthread_mutex_destroy(&mutexpool);
    pthread_cond_destroy(&noempty);
}

void ThreadPool::addTask(Task task)
{

            if(shutdown)
            {

                return;
            }
            /* code */
    taskQ->addTask(task);

    pthread_cond_signal(&noempty);

}

int ThreadPool::getBusyNum()
{
    pthread_mutex_lock(&mutexpool);
    int busyNum=this->busyNum;
    pthread_mutex_unlock(&mutexpool);
    return busyNum;
}

int ThreadPool::getLiveNum()
{
    pthread_mutex_lock(&mutexpool);
    int liveNUm=this->liveNUm;
    pthread_mutex_unlock(&mutexpool);
    return liveNUm;
}

void *ThreadPool::worker(void* arg)
{
    ThreadPool* pool=static_cast<ThreadPool*>(arg);
     while (true)
    {
        pthread_mutex_lock(&pool->mutexpool);
        //当前队列是否为空
        while (pool->taskQ->taskNumber()==0&&!pool->shutdown)
        {
            //阻塞线程
            pthread_cond_wait(&pool->noempty,&pool->mutexpool);

            if(pool->exitNum>0)
            {
                pool->exitNum--;
                if(pool->liveNUm>pool->minNum){
                    pool->liveNUm--;
                pthread_mutex_unlock(&pool->mutexpool);
                pool->threadExit();
                }
            }
        }

        if(pool->shutdown)
        {
            pthread_mutex_unlock(&pool->mutexpool);
            pool->threadExit();
        }
        

        Task task=pool->taskQ->takeTask();

        pool->busyNum++;
        pthread_mutex_unlock(&pool->mutexpool);

        task.function(task.arg);
        delete task.arg;
        task.arg=nullptr;


        pthread_mutex_lock(&pool->mutexpool);
        pool->busyNum--;
        pthread_mutex_unlock(&pool->mutexpool);




    }
    return nullptr;
}

void *ThreadPool::manager(void *arg)
{
    ThreadPool* pool=static_cast<ThreadPool*>(arg);
    while (!pool->shutdown)
    {
        /* code */
        sleep(3);
        pthread_mutex_lock(&pool->mutexpool);
        int queueSize=pool->taskQ->taskNumber();
        int liveNUm=pool->liveNUm;
        int busyNum=pool->busyNum;
        pthread_mutex_unlock(&pool->mutexpool);
        /* code */


        

      


        //添加线程
        //人物的个数>存活的线程个数&&村获得线程数<最大的线程数
        if(queueSize>liveNUm&&liveNUm<pool->maxNum)
        {
            pthread_mutex_lock(&pool->mutexpool);
            int count=0;
            for (size_t i = 0; i < pool->maxNum&&count<NUMBER
            &&pool->liveNUm<pool->maxNum; i++)
            {
                /* code */
                if(pool->threadIDS[i]==0)
                {
                    pthread_create(&pool->threadIDS[i],NULL,worker,pool);
                    count++;
                    pool->liveNUm++;
                }
            }
            pthread_mutex_unlock(&pool->mutexpool);
            
        }


        //销毁线程
        //忙的线程*2<存活的&&村获得>最小线程数
        if(busyNum*2<liveNUm&&liveNUm>pool->minNum)
        {
            pthread_mutex_lock(&pool->mutexpool);
            pool->exitNum=NUMBER;
            pthread_mutex_unlock(&pool->mutexpool);    
            //工作的线程自杀
            for (size_t i = 0; i < NUMBER; i++)
            {
                pthread_cond_signal(&pool->noempty);

            }
                        
        }
    }
    
    return nullptr;
}

void ThreadPool::threadExit()
{
    pthread_t tid=pthread_self();
    for (size_t i = 0; i < maxNum; i++)
    {
        if (threadIDS[i]==tid)
        {
            threadIDS[i]=0;
            break;
        }
        
    }
    pthread_exit(NULL);
    
}
