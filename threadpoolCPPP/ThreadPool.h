#pragma once
#include"TaskQueue.h"
class ThreadPool{
private:
    //任务队列
    TaskQueue* taskQ;


    pthread_t managerID;//管理者线程
    pthread_t *threadIDS;//工作线程

    int minNum;//最小
    int maxNum;//最大
    int busyNum;//忙碌
    int liveNUm;//存活
    int exitNum;//退出线程数


    pthread_mutex_t mutexpool;


    pthread_cond_t noempty;// 空

    bool  shutdown; //是否销毁线程池，1,0

    static const int NUMBER{2};

public:
    ThreadPool(int min,int max);
    ~ThreadPool();

    void addTask(Task task);

    int getBusyNum();
    int getLiveNum();
private:
    static void* worker(void* arg);

    static void* manager(void* arg);

    void threadExit(); 
};