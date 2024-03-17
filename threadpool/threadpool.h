#ifndef _THREADPOOL_H_
#define _THREADPOOL_H_

typedef struct ThreadPool ThreadPool;


//创建线程池并初始化
ThreadPool* threadPoolCreate(int min,int max,int queueSize);


//销毁线程池


//给线程池添加任务
void threadPoolAdd(ThreadPool* pool,void(*function)(void*),void* arg);

//获取线程池中的工作线程


//活着的线程数









void* worker(void* arg);



void* manager(void* arg);


void threadExit(ThreadPool* pool);
#endif _THREADPOOL_H_