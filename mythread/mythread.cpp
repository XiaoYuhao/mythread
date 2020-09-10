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
    
    tid = id;
    stack = stack_top + stack_size;          //指向栈的底部，ebp
    this->handler = handler;
    memset(&ctx, 0, sizeof(ctx_buf_t));
    ctx.buffer[rsp] = (long)stack;
    ctx.buffer[pc_addr] = (long)handler;

    status = INIT;

    printf("ID: %d thread has been created...\n", tid);
}

Thread::Thread(const Thread &t){          //在有内存申请和释放的类中最好有一个拷贝构造函数
    int stack_size = (1<<20);               
    stack_top = malloc(stack_size);             
    tid = t.tid;
    memcpy(stack_top, t.stack_top, stack_size);
    stack = stack_top + stack_size;          
    memcpy(&ctx, &t.ctx, sizeof(ctx_buf_t));    //寄存器信息并不都是一样的，栈帧的地址变了，其它一样
    //handler = t.handler;
    ctx.buffer[rsp] = (long)stack;              //这是新的栈，不改这个会出现Segmentation fault
    //ctx.buffer[pc_addr] = (long)handler;
    printf("ID: %d thread has been created...\n", tid);

    /*从拷贝构造函数可以看出，每次都需要重新开辟一个新栈，然后将原来栈的数据拷贝过去，效率低下。所以最好避免这样的拷贝，
      tasks初始化的时候尽量开辟足够大的size，不然扩充的时候会造成大量的拷贝，效率低下且容易出现错误。*/
}

Thread::~Thread(){
    printf("ID: %d thread has been destory...\n", tid);
    free(stack_top);
}

void Thread::start(){
    __asm__("movq %0, %%r10;"::"r"(&ctx):"%r10");       /*先将寄存器信息地址写入r10寄存器，因为不能用start_context的参数传递
                                                          start_context的参数是传给handler的参数，不能动*/
    start_context(tid);                                 /*tid会保存在edi寄存器中，handler函数会取出edi作为其第一个参数，可变
                                                          似乎没有什么好的解决方法，或许可以通过重载start_context？*/
}


void Scheduler::add_thread(int id, thread_handler_t handler){
    tasks.emplace_back(id, handler);         /*push_back会先构造临时变量，再拷贝构造进vector，而emplace_back则原地构造，
                                               另外需要注意的是tasks初始大小很小，扩充大小的时候需要拷贝原来的元素过去，再
                                               调用析构函数，需要注意，尤其是有内存申请和释放情况下。*/
}

void Scheduler::do_switch(Thread &from, Thread &to){
    int ret = save_context(&from.ctx);       /**/
    if(ret == 0){
        restore_context(&to.ctx);
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
        for(auto &t: tasks){
            if(t.status == INIT){
                t.status = RUNABLE;
                int ret = save_context(&admin.ctx);
                if(ret == 0){
                    printf("start...\n");
                    t.start();
                }
            }
            else if(t.status == RUNABLE){
                do_switch(admin,t);
            }
            else{
                continue;
            }
        }
    } 
}