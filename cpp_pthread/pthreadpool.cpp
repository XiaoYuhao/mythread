#include<iostream>
#include"pthreadpool.h"
using namespace std;

ThreadPool::ThreadPool(int n):stop(false){
    //threads.reserve(n);
    threads.resize(n);
    for(auto &thread : threads){
        int ret = pthread_create(&thread, NULL, &empty_task, this);
        if(ret != 0){
            cout<<"thread create failed..."<<endl;
            exit(-1);
        }
    }
}

ThreadPool::~ThreadPool(){
    stop = true;
    condition.notify_all();
    for(auto &thread : threads){
        pthread_join(thread, NULL);
    }
    cout<<"destroy thread pool success..."<<endl;
}

void ThreadPool::append(task_t &t){
    queue_mutex.lock();
    task_queue.push(t);
    queue_mutex.unlock();
    condition.notify_one();
}

void *empty_task(void *threadpool){
    ThreadPool *pool = (ThreadPool *)threadpool;

    while(!pool->stop){
        unique_lock<mutex> lk(pool->queue_mutex);
        while(pool->task_queue.empty()){
            pool->condition.wait(lk);
            if(pool->stop) break;
        }
        if(pool->task_queue.empty()){
            continue;
        }
        else{
            task_t task = pool->task_queue.front();
            pool->task_queue.pop();
            lk.unlock();                //解锁后再去执行任务
            (*task.handler)(task.arg);
        }
    }

    pthread_exit(NULL);
}