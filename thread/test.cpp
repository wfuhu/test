#include"defTp.h"
#include "TaskQueue.h"
#include "TaskQueue.cpp"
#include "defTp.cpp"
#include "defTp.h"
#include <unistd.h>
#include <string.h>
#include<string>
#include<iostream>
#include<stdio.h>

void taskFunc(void* arg)
{
    int num=*(int*)arg;
   printf("thread %ld is working ,number=%d",pthread_self(),num);
    sleep(1);

};

int  main()
{
    ThreadPool<int> pool(3,10);
    for (size_t i = 0; i < 100; i++)
    {
        int* num=new int(i+100);

        pool.addTask(Task<int>(taskFunc,num));
    }
    sleep(20);

    return 0;
    
};
