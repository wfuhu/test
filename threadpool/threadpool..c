#include "threadpool.h"
#include<pthread.h>
const int NUMBER=2;
typedef struct Task
{
    void(*function)(void* arg);
    void* arg;
}Task;

typedef struct ThreadPool{
    
    //任务队列
    Task* taskQ;
    int queueCapacity;//容量
    int queueSize;//个数
    int queueFront;//队头
    int queueRear;//队尾

    pthread_t managerID;//管理者线程
    pthread_t *threadIDS;//工作线程

    int minNum;//最小
    int maxNum;//最大
    int busyNum;//忙碌
    int liveNUm;//存活
    int exitNum;//退出线程数


    pthread_mutex_t mutexpool;
    pthread_mutex_t mutexbusy;
    pthread_cond_t notfull;//队伍是否满
    pthread_cond_t noempty;// 空

    int shutdown; //是否销毁线程池，1,0
};


ThreadPool* threadPoolCreate(int min,int max,int queueSize)
{
    ThreadPool* pool=(ThreadPool*)malloc(sizeof(ThreadPool));
    do
    {

        if(pool==NULL)
    {
        printf("fail");
        break;  
    }

    pool->threadIDS=(pthread_t*)malloc(sizeof(pthread_t)*max);
    if(pool->threadIDS==NULL)
    {
        printf("fail1");
        break;
    }
    memset(pool->threadIDS,0,sizeof(pthread_t)*max);
    pool->minNum=min;
    pool->maxNum=max;
    pool->busyNum=0;
    pool->liveNUm=min;
    pool->exitNum=0;


    if(pthread_mutex_init(&pool->mutexpool,NULL)!=0||
    pthread_mutex_init(&pool->mutexbusy,NULL)!=0||
    pthread_cond_init(&pool->noempty,NULL)!=0||
    pthread_cond_init(&pool->notfull,NULL)!=0)
    {
        printf("fail2");
        break;
    }

    pool->taskQ=(Task*)mallc(sizeof(Task)*queueSize);
    pool->queueCapacity=queueSize;
    pool->queueFront=0;
    pool->queueRear=0;
    pool->queueSize=0;


    pool->shutdown=0;

    pthread_create(&pool->managerID,NULL,manager,pool);
    for (size_t i = 0; i < min; i++)
    {
        /* code */
        pthread_create(&pool->threadIDS[i],NULL,worker,pool);
    }
        /* code */
    } while (0);
    
    if(pool&&pool->threadIDS)
    free(pool->threadIDS);

    if(pool&&pool->taskQ)
    free(pool->taskQ);


    if(pool)
    free(pool);

    return NULL;
}


void* worker(void* arg)
{
    ThreadPool* pool=(ThreadPool*)arg;
    while (1)
    {
        pthread_mutex_lock(&pool->mutexpool);
        //当前队列是否为空
        while (pool->queueSize==0&&!pool->shutdown)
        {
            //阻塞线程
            pthread_cond_wait(&pool->noempty,&pool->mutexpool);

            if(pool->exitNum>0)
            {
                pool->exitNum--;
                if(pool->liveNUm>pool->minNum){
                    pool->liveNUm--;
                pthread_mutex_unlock(&pool->mutexpool);
                threadExit(pool);
                }
            }
        }

        if(pool->shutdown)
        {
            pthread_mutex_unlock(&pool->mutexpool);
            threadExit(pool);
        }
        

        Task task;
        task.function=pool->taskQ[pool->queueFront].function;
        task.arg=pool->taskQ[pool->queueFront].arg;

        pool->queueFront=(pool->queueFront+1)%pool->queueCapacity;
        pool->queueSize--;

        pthread_cond_signal(&pool->notfull);
        pthread_mutex_unlock(&pool->mutexpool);

        pthread_mutex_lock(&pool->busyNum);
        pool->busyNum++;
        pthread_mutex_unlock(&pool->busyNum);
        task.function(task.arg);
        free(task.arg);
        task.arg=NULL;


        pthread_mutex_lock(&pool->busyNum);
        pool->busyNum--;
        pthread_mutex_unlock(&pool->busyNum);




    }
    


    return NULL;
}

void* manager(void* arg)
{
    ThreadPool* pool=(ThreadPool*)arg;
    while (!pool->shutdown)
    {
        sleep(3);

        //取出线程池中任务的数量和当前线程的数量
        pthread_mutex_lock(&pool->mutexpool);
        int queueSize=pool->queueSize;
        int liveNUm=pool->liveNUm;

        pthread_mutex_unlock(&pool->mutexpool);
        /* code */

        //取出忙的线程数量
        pthread_mutex_lock(&pool->mutexbusy);
        int busyNum=pool->busyNum;

        pthread_mutex_unlock(&pool->mutexbusy);


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



    return NULL;
}

void threadExit(ThreadPool* pool)
{
    pthread_t tid=pthread_self();
    for (size_t i = 0; i < pool->maxNum; i++)
    {
        if(pool->threadIDS[i]==tid)
        {
            pool->threadIDS[i]=0;

            break;
        }
    }
    pthread_exit(NULL);
    
}

void threadPoolAdd(ThreadPool* pool,void(*func)(void*),void* arg)
{
    pthread_mutex_lock(&pool->mutexpool);
    while (pool->queueSize==pool->queueCapacity&& !pool->shutdown)
    {

        pthread_cond_wait(&pool->notfull,&pool->mutexpool);
        if(pool->shutdown)
        {
            pthread_mutex_unlock(&pool->mutexpool);
            return;
        }
        /* code */
    }

    //添加任务
    pool->taskQ[pool->queueRear].function=func;
    pool->taskQ[pool->queueRear].arg=arg;
    pool->queueRear=(pool->queueRear+1)%pool->queueCapacity;
    pool->queueSize++;


    pthread_cond_signal(&pool->noempty);

    



    pthread_mutex_unlock(&pool->mutexpool);
}
