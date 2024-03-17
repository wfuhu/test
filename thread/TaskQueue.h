#ifndef _TaskQueue_h_

#define _TaskQueue_h_
#include<queue>
#include <iostream>
#include<pthread.h>


using callback=void(*)(void* arg);
//任务函数
template <typename T>
struct Task
{   
    callback function;
    T* arg;
    Task<T>()
    {
        function=nullptr;
        arg=nullptr;
    }
    Task<T>(callback f,void* arg)
    {
        this->arg=(T*)arg;
        function=f;
    }

    
    

};


template <typename T>
class TaskQueue{
    public:
        TaskQueue();
        ~TaskQueue();

        //添加
        void addTask(Task<T> task);

        void addTask(callback f,void* arg);
        //取出
        Task<T> takeTask();
        //任务个数
        inline size_t taskNumber()
        {
            return m_taskQ.size();
        }





    private:
        pthread_mutex_t m_mutex;
        std::queue<Task<T>> m_taskQ;
};

#endif