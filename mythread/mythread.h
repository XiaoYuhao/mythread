

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
typedef struct{
    int id;
    void *stack;
    thread_handler_t handler;
    ctx_buf_t ctx;
}thread_t;

//extern int save_context(ctx_buf_t *from);
//extern int restore_context(ctx_buf_t *to);

void do_switch(thread_t *from, thread_t *to);
int thread_create(thread_t *t, int id, thread_handler_t handler);
void thread_destory(thread_t *t);
int thread_start(thread_t *t);


class Scheduler
{
    int thread_num;
    thread_t admin;
public:
    Scheduler();
    void do_switch(thread_t *from, thread_t *to);
};