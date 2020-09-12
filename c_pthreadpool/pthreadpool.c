#include<stdlib.h>
#include<stdio.h>
#include"pthreadpool.h"

int createpool(pthreadpool *pool, int thread_num, int task_queue_size){
    pool->stop = 0;
    pool->threads_num = thread_num;
    pool->threads = (pthread_t *)malloc(sizeof(pthread_t)*thread_num);
    if(!pool->threads){
        printf("malloc threads memory failed.\n");
        return -1;
    }
    int ret;
    for(int i=0;i<thread_num;i++){
       ret = pthread_create(&pool->threads[i], NULL, &empty_task, pool); 
       if(ret != 0){
           printf("pthread create failed.\n");
           return -1;
       }
    }

    pool->task_queue = (task_t *)malloc(sizeof(task_t)*task_queue_size);
    if(!pool->task_queue){
        printf("malloc tasks queue memory failed.\n");
        return -1;
    }
    
    pool->queue_size = task_queue_size;
    pool->queue_front = 0;
    pool->queue_end = 0;
    /*
    ret = sem_init(&pool->tasks_num,0,0);
    if(ret != 0){
        printf("sem init error.\n");
        return -1;
    }
    */
    ret = pthread_cond_init(&pool->cv, NULL);
    if(ret != 0){
        printf("pthread cond init failed.\n");
        return -1;
    }

    ret = pthread_mutex_init(&pool->mutex, NULL);
    if(ret != 0){
        printf("mutex init error.\n");
        return -1;
    }

    printf("create pthread pool success.\n");
    return 1;
}

int destroypool(pthreadpool *pool){
    pool->stop = 1;
    /*
    for(int i=0;i<pool->threads_num;i++){           //唤醒所有线程
        sem_post(&pool->tasks_num);             
    }
    */
    pthread_cond_broadcast(&pool->cv);             //唤醒所有线程   
    for(int i=0;i<pool->threads_num;i++){
        pthread_join(pool->threads[i], NULL);      //等待所有线程结束
    }
    free(pool->threads);                            //释放空间
    free(pool->task_queue);
    //sem_destroy(&pool->tasks_num);
    pthread_cond_destroy(&pool->cv);
    pthread_mutex_destroy(&pool->mutex);     
    printf("destory pthread pool success...\n");       
    return 1;
}

int append(pthreadpool *pool, task_t t){
    pthread_mutex_lock(&pool->mutex);
    if((pool->queue_end+1) % pool->queue_size == pool->queue_front){    //队列已满
        printf("task queue full...\n");
        pthread_mutex_unlock(&pool->mutex);
        return -1;
    }
    pool->task_queue[pool->queue_end] = t;                              //任务入队
    pool->queue_end = (pool->queue_end+1)%pool->queue_size;
    pthread_mutex_unlock(&pool->mutex);
    pthread_cond_signal(&pool->cv);                                     //唤醒消费者线程
    return 1;
}

void *empty_task(void *threadpool){
    pthreadpool *pool = (pthreadpool *)threadpool;

    while(1){
        //sem_wait(&pool->tasks_num);
        pthread_mutex_lock(&pool->mutex);                   //从任务队列中获取任务时，先将任务队列上锁
        while(pool->queue_front == pool->queue_end){        //若任务队列为空，则应该一直阻塞
            pthread_cond_wait(&pool->cv, &pool->mutex);
            if(pool->stop) break;
        }
        if(pool->stop){                                     //收到线程结束信号
            pthread_mutex_unlock(&pool->mutex); 
            break;
        }
        //pthread_mutex_lock(&pool->mutex);
        task_t t = pool->task_queue[pool->queue_front];     //任务出队
        pool->queue_front = (pool->queue_front+1) % pool->queue_size;
        pthread_mutex_unlock(&pool->mutex);                 //解锁任务队列
        *(t.handler)(t.arg);
    }
    pthread_exit(NULL);
}