#include<vector>
#include<cstdlib>
using namespace std;

enum register_tt{
    rbx,
    rsp,
    rbp,
    r12,
    r13,
    r14,
    r15,
    pc_addr,
};

typedef struct{
    long buffer[8];
}ctx_buf_t;

typedef void *(*thread_handler_t)(int id);
typedef int thread_status;
const int INIT = 0;
const int RUNABLE = 1;
const int WAIT = 2;

class Thread{
/*
    int id;
    void *stack;
    void *stack_top;
    thread_handler_t handler;
    ctx_buf_t ctx*/
public:
    int id;
    void *stack;
    void *stack_top;
    thread_handler_t handler;
    ctx_buf_t ctx;
    thread_status status;
    Thread(){};
    Thread(int id, thread_handler_t handler);
    ~Thread();
    void start();
    ctx_buf_t* context();
};


typedef struct{
    int id;
    void *stack;
    thread_handler_t handler;
    ctx_buf_t ctx;
    thread_status status;
}thread_t;

//extern int save_context(ctx_buf_t *from);
//extern int restore_context(ctx_buf_t *to);

void do_switch(thread_t *from, thread_t *to);
int thread_create(thread_t *t, int id, thread_handler_t handler);
void thread_destory(thread_t *t);
int thread_start(thread_t *t);


class Scheduler
{
    Thread admin;
    //vector<Thread> tasks;
    int num;
    Thread tasks[10];
public:
    Scheduler():admin(-1,NULL), num(0){};
    void work();
    void add_thread(int id, thread_handler_t handler);
    void do_switch(Thread &from, Thread &to);
    void switch_to_admin(int id);
};