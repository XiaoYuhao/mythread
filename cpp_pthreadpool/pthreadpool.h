#include<mutex>
#include<pthread.h>
#include<vector>
#include<queue>
#include<condition_variable>
using namespace std;

typedef void *(*thread_handler_t)(void *);
typedef struct{
    thread_handler_t handler;           //函数指针
    void *arg;
}task_t;

class ThreadPool{
    vector<pthread_t> threads;
    queue<task_t> task_queue;
    mutex queue_mutex;
    condition_variable condition;
    bool stop;
public:
    ThreadPool(int n=20);
    ~ThreadPool();
    void append(task_t &t);
    bool task_queue_empty();
    friend void *empty_task(void *threadpool);
};

void *empty_task(void *threadpool);

