#pragma once

#include<queue>
#include<pthread.h>

using callback=void(*)(void* arg);

struct Task
{
    Task(){
        function=nullptr;
        arg=nullptr;
    }
    Task(callback f,void* arg)
    {
        this->arg=arg;
        function=f;
    }
    callback function;;
    void* arg;
};


class TaskQueue
{
private:
    pthread_mutex_t m_mutex;

    std::queue<Task> m_taskQ;
    /* data */
public:
    TaskQueue();
    ~TaskQueue();


    //添加任务
    void addTask(Task task);
    void addTask(callback f,void* arg);

    //取出一个任务
    Task takeTask();

    //获取当前任务个数
    inline int taskNumber()
    {
        return m_taskQ.size();      
    }
};


