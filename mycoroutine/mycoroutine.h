#ifndef _MY_COROUTINE_H_
#define _MY_COROUTINE_H_

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

typedef void *(*task_handler_t)(int id);
typedef int coroutine_status;
const int INIT = 0;
const int RUNABLE = 1;
const int WAIT = 2;
const int DEAD = 3;

class Coroutine{
    int tid;                            //协程ID号
    int para1;                          //可带一个参数
    void *stack;
    void *stack_top;
    task_handler_t handler;
public:
    ctx_buf_t ctx;                      //寄存器上下文
    coroutine_status status;
    Coroutine(){};
    Coroutine(int id, task_handler_t handler, int para);
    Coroutine(const Coroutine &t);            //拷贝构造函数
    Coroutine(Coroutine &&t) noexcept;      //移动构造函数
    Coroutine& operator=(Coroutine &&t) noexcept;
    Coroutine& operator=(const Coroutine &t);
    ~Coroutine();
    void start();
    int get_id();
};

#endif

