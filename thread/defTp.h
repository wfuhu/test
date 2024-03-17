#ifndef _threadpool_h_

#define _threadpool_h_
#include "TaskQueue.h"

template <typename T>
class ThreadPool
{
public:
    ThreadPool(int min,int max);
    ~ThreadPool();

    void addTask(Task<T> t);

    int getBusyNum();

    int getAliveNum();

private:
    static void* work(void* arg);

    static void* manager(void* arg);

    void threadExit();


private:
    TaskQueue<T>* taskQ;
    
    pthread_t managerID;
    pthread_t* threadIDS;
    int minNum;
    int maxNum;
    int liveNum;
    int busyNum;
    int exitNum;
    pthread_mutex_t mutexPool;
    pthread_cond_t notEmpty;

    static const int NUMBER=2;

    bool shutdown;

};

#endif