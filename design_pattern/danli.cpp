#include <iostream>
#include <string>
#include <mutex>
#include<atomic>

// 饿汉模式--定义类的时候创建单例对象
class TaskQueue
{
public:
    TaskQueue(const TaskQueue &t) = delete;
    TaskQueue &operator=(const TaskQueue &t) = delete;

    static TaskQueue *getInstance()
    {
        return m_taskQ;
    }
    void print()
    {
        std::cout << "单例对象函数" << std::endl;
    }

private:
    TaskQueue() = default;
    // TaskQueue(const TaskQueue &t) = default;
    // 通过类名访问静态属性或方法
    static TaskQueue *m_taskQ;
};
TaskQueue *TaskQueue::m_taskQ = new TaskQueue();

// 懒汉模式--使用类的时候创建单例对象  节省空间
class TaskQueue1
{
public:
    TaskQueue1(const TaskQueue1 &t) = delete;
    TaskQueue1 &operator=(const TaskQueue1 &t) = delete;

    static TaskQueue1 *getInstance()
    {
        if (m_taskQ == nullptr)//双层if
        {
            m_mutex.lock();
            if (m_taskQ == nullptr)
            {
                m_taskQ = new TaskQueue1;
            }
            m_mutex.unlock();
        }
        return m_taskQ;
    }
    void print()
    {
        std::cout << "单例对象函数" << std::endl;
    }

private:
    TaskQueue1() = default;
    // TaskQueue(const TaskQueue &t) = default;
    // 通过类名访问静态属性或方法
    static TaskQueue1 *m_taskQ;
    static mutex m_mutex; // 多线程 线程安全问题
};
TaskQueue1 *TaskQueue1::m_taskQ = nullptr;
mutex TaskQueue1::m_mutex;




// 懒汉模式--使用类的时候创建单例对象  节省空间  使用原子变量
class TaskQueue2
{
public:
    TaskQueue2(const TaskQueue2 &t) = delete;
    TaskQueue2 &operator=(const TaskQueue2 &t) = delete;

    static TaskQueue2 *getInstance()
    {
        TaskQueue2* task=m_taskQ.load();
        if (task == nullptr)//双层if
        {
            m_mutex.lock();
            task=taskQ.load();
            if (task == nullptr)
            {
                task = new TaskQueue2;
                m_taskQ.store(task);
            }
            m_mutex.unlock();
        }
        return task;
    }
    void print()
    {
        std::cout << "单例对象函数" << std::endl;
    }

private:
    TaskQueue2() = default;
    // TaskQueue(const TaskQueue &t) = default;
    // 通过类名访问静态属性或方法
    //static TaskQueue2 *m_taskQ;
    static mutex m_mutex; // 多线程 线程安全问题
    static atomic<TaskQueue2*> m_taskQ;
};
//TaskQueue2 *TaskQueue2::m_taskQ = nullptr;
atomic<TaskQueue2> *TaskQueue2::m_taskQ ;//提交数据.store()  取数据.load()百度
mutex TaskQueue2::m_mutex;



// 懒汉模式--使用类的时候创建单例对象  节省空间  
//使用静态局部对象解决线程安全 c++11
class TaskQueue3
{
public:
    TaskQueue3(const TaskQueue3 &t) = delete;
    TaskQueue3 &operator=(const TaskQueue3 &t) = delete;

    static TaskQueue3 *getInstance()
    {
       static TaskQueue3 task;
        return &task;
    }
    void print()
    {
        std::cout << "单例对象函数" << std::endl;
    }

private:
    TaskQueue3() = default;
    // TaskQueue(const TaskQueue &t) = default;
    // 通过类名访问静态属性或方法
   
};



int main()
{
    TaskQueue *taskQ = TaskQueue::getInstance;
    taskQ->print();
    return 0;
}