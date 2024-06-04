#include <iostream>
#include <queue>
#include <mutex>
#include <thread>
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

    bool isEmpty()
    {
        lock_guard<mutex> locker(m_mutex);
        bool flag = m_data.empty();
        return flag;
    }

    void addTask(int node)
    {
        lock_guard<mutex> locker(m_mutex);

        m_data.push(node);
    }
    bool popTask()
    {
        lock_guard<mutex> locker(m_mutex);
        if (m_data.empty())
        {
            return false;
        }
        m_data.pop();
        return true;
    }

    void takeTask()
    {
        lock_guard<mutex> locker(m_mutex);
        if (m_data.empty())
        {
            return -1;
        }
        int data = m_data.front();
        return data;
    }

private:
    TaskQueue() = default;
    // TaskQueue(const TaskQueue &t) = default;
    // 通过类名访问静态属性或方法
    static TaskQueue *m_taskQ;
    // 定义任务队列
    queue<int> m_data;
    mutex m_mutex;
};
TaskQueue *TaskQueue::m_taskQ = new TaskQueue();

int main()
{
    TaskQueue *taskQ = TaskQueue::getInstance();

    thread t1([=]()
              {
    for (size_t i = 0; i < 100; i++)
    {
        taskQ->addTask(i+100);
        std::cout<<"+++data:"<<i+200<<
        "thread_id"<<this_thread::get_id()<<std::endl;
        this_thread::sleep_for(chrono::milliseconds(500));
    } });


     thread t2([=]()
              {
    this_thread::sleep_for(chrono::milliseconds(500));
    for (size_t i = 0; i < 100; i++)
    {
        int num=taskQ->takeTask();
        std::cout<<"+++data:"<<num<<
        "thread_id"<<this_thread::get_id()<<std::endl;
        taskQ->popTask();
         this_thread::sleep_for(chrono::milliseconds(1000));
    } });

    t1.join();
    t2.join();
    return 0;
}