#include<stdlib.h>
#include<string.h>
#include<cstdio>
#include"mythread.h"

extern "C"
{
    extern int save_context(ctx_buf_t *from);
    extern int restore_context(ctx_buf_t *to);
    extern int start_context(int id);
}

Thread::Thread(int id, thread_handler_t handler){
    int stack_size = (1<<20);   //1MB
    stack_top = malloc(stack_size);       //注意，这里的stack_top是栈可用空间最顶部（栈由高到低生长）
    
    id = id;
    stack = stack_top + stack_size;          //指向栈的底部，ebp
    handler = handler;
    memset(&ctx, 0, sizeof(ctx_buf_t));
    ctx.buffer[rsp] = (long)stack;
    ctx.buffer[pc_addr] = (long)handler;

    status = INIT;

    printf("ID: %d thread has been created...\n", id);
}

Thread::~Thread(){
    printf("ID: %d thread has been destory...\n", id);
    free(stack_top);
}

void Thread::start(){
    __asm__("movq %0, %%r10;"::"r"(&ctx):"%r10");
    start_context(id);
    //restore_context(&ctx);
}

ctx_buf_t* Thread::context(){
    return &ctx;
}


void Scheduler::add_thread(int id, thread_handler_t handler){
    //tasks.emplace_back(id,handler);
    int stack_size = (1<<20);   //1MB
    tasks[num].stack_top = malloc(stack_size);       //注意，这里的stack_top是栈可用空间最顶部（栈由高到低生长）
    
    tasks[num].id = id;
    tasks[num].stack = tasks[num].stack_top + stack_size;          //指向栈的底部，ebp
    tasks[num].handler = handler;
    memset(&tasks[num].ctx, 0, sizeof(ctx_buf_t));
    tasks[num].ctx.buffer[rsp] = (long)tasks[num].stack;
    tasks[num].ctx.buffer[pc_addr] = (long)handler;

    tasks[num].status = INIT;

    printf("ID: %d thread has been created...\n", num);

    num++;
}

void Scheduler::do_switch(Thread &from, Thread &to){
    int ret = save_context(from.context());
    if(ret == 0){
        restore_context(to.context());
    }
    else{
        return;
    }
}

void Scheduler::switch_to_admin(int id){
    printf("from ID: %d switch to admin...\n", id);
    Thread &from = tasks[id];
    do_switch(from, admin);
}

void Scheduler::work(){
    while(1){
    for(int i=0;i<num;i++){
        if(tasks[i].status == INIT){
            tasks[i].status = RUNABLE;
            printf("ID: %d start is here..\n", i);
            int ret = save_context(admin.context());
            if(ret == 0){
                tasks[i].start();
            }
        }
        else if(tasks[i].status == RUNABLE){
            do_switch(admin,tasks[i]);
        }
        else{
            continue;
        }
    }
    }
    /*
    for(auto &t: tasks){
        if(t.status == INIT){
            t.status = RUNABLE;
            t.start();
        }
        else if(t.status == RUNABLE){
            do_switch(admin,t);
        }
        else{
            continue;
        }
    }
    */
}

/*
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

*/
