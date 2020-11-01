#include<vector>
#include<cstdlib>
#include<sys/epoll.h>
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
    int tid;                            //协程ID号
    int para1;                          //可带一个参数
    void *stack;
    void *stack_top;
    thread_handler_t handler;
public:
    ctx_buf_t ctx;
    thread_status status;
    Thread(){};
    Thread(int id, thread_handler_t handler, int para);
    Thread(const Thread &t);            //拷贝构造函数
    Thread(Thread &&t) noexcept;      //移动构造函数
    ~Thread();
    void start();
    int get_id();
};

const int MAX_EVENT_NUM = 10000;

class Scheduler
{
    Thread admin;
    vector<Thread> tasks;
    int current_id;
    int epfd;
    int routine_count;
    Scheduler():admin(-1,NULL,0),routine_count(0),start(false){
        epfd = epoll_create(MAX_EVENT_NUM);
    };
    static Scheduler* _instance;
    int start;
public:
    static Scheduler* create();
    /*
    Scheduler():admin(-1,NULL,0),routine_count(0){
        //tasks.reserve(100);
        epfd = epoll_create(MAX_EVENT_NUM);
    };*/
    ~Scheduler();
    void work();
    void add_thread(thread_handler_t handler, int para);
    void do_switch(Thread &from, Thread &to);
    void switch_to_admin();
    int get_current_id();
    void add_wait_fd(int fd, int flags);
};

struct wait_info{
    int fd;
    int id;
};
