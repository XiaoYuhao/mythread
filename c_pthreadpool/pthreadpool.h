#include<pthread.h>
#include<semaphore.h>


typedef void *(*thread_handler_t)(void *);
typedef struct{
    thread_handler_t handler;           //函数指针
    void *arg;
}task_t;


typedef struct{
    pthread_t *threads;
    task_t *task_queue;
    int threads_num;
    int stop;
    int queue_size;
    int queue_front;
    int queue_end;

    //sem_t tasks_num;
    pthread_mutex_t mutex;
    pthread_cond_t cv;

}pthreadpool;

int createpool(pthreadpool *pool, int thread_num, int task_queue_size);
int append(pthreadpool *pool, task_t t);
int destroypool(pthreadpool *pool);
void *empty_task(void *threadpool);
