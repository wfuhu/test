#include <iostream>
#include <string>
#include <mutex>

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
int main()
{
    TaskQueue *taskQ = TaskQueue::getInstance;
    taskQ->print();
    return 0;
}