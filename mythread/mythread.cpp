#include<stdlib.h>
#include<string.h>
#include"mythread.h"

extern "C"
{
    extern int save_context(ctx_buf_t *from);
    extern int restore_context(ctx_buf_t *to);
    extern int start_context(int fd);
}


void do_switch(thread_t *from, thread_t *to){
    int ret = save_context(&from->ctx);
    if(ret == 0){
        restore_context(&to->ctx);
    }
    else{
        return;
    }
}

int thread_create(thread_t *t, int id, thread_handler_t handler){
    int stack_size = (1<<20);   //1MB
    void *stack_top = malloc(stack_size);       //注意，这里的stack_top是栈可用空间最顶部（栈由高到低生长）
    
    t->id = id;
    t->stack = stack_top + stack_size;          //指向栈的底部，ebp
    t->handler = handler;
    memset(&t->ctx, 0, sizeof(ctx_buf_t));
    t->ctx.buffer[rsp] = (long)t->stack;
    t->ctx.buffer[pc_addr] = (long)t->handler;
    return id;
}

void thread_destory(thread_t *t){
    free(t->stack);
}

int thread_start(thread_t *t){
    ctx_buf_t *tp = &t->ctx;
    __asm__("movq %0, %%rax"::"r"(tp));
    //restore_context(&t->ctx);
    start_context(523);
    return -1;
}


