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
    int tid;
    void *stack;
    void *stack_top;
    thread_handler_t handler;
public:
    ctx_buf_t ctx;
    thread_status status;
    Thread(){}; 
    Thread(int id, thread_handler_t handler);
    Thread(const Thread &t); //拷贝构造函数
    Thread(Thread &&t) noexcept;      //移动构造函数
    ~Thread();
    void start();
};

class Scheduler
{
    Thread admin;
    vector<Thread> tasks;
public:
    Scheduler():admin(-1,NULL){
        //tasks.reserve(100);
    };
    void work();
    void add_thread(int id, thread_handler_t handler);
    void do_switch(Thread &from, Thread &to);
    void switch_to_admin(int id);
};