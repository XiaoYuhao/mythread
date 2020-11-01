#include<stdlib.h>
#include<string.h>
#include<cstdio>
#include<unistd.h>
#include"mycoroutine.h"

extern "C"
{
    extern int save_context(ctx_buf_t *from);
    extern int restore_context(ctx_buf_t *to);
    extern int start_context(int id);
}

Coroutine::Coroutine(int id, task_handler_t handler, int para){
    int stack_size = (1<<20);   //1MB
    stack_top = malloc(stack_size);       //注意，这里的stack_top是栈可用空间最顶部（栈由高到低生长）
    para1 = para;
    tid = id;
    stack = stack_top + stack_size;          //指向栈的底部，ebp
    this->handler = handler;
    memset(&ctx, 0, sizeof(ctx_buf_t));
    ctx.buffer[rsp] = (long)stack;
    ctx.buffer[pc_addr] = (long)handler;

    status = INIT;

    //printf("ID: %d thread has been created...\n", tid);
}

/*下面这个拷贝构造函数，每次拷贝都要重新分配空间。如果对于拷贝的对象是一个临时对象（右值对象）
  这势必是多此一举的，所以可以考虑使用移动构造函数来实现对临时对象的拷贝。*/

/*在这个程序的源代码层面似乎是没有地方使用到拷贝构造函数的，但是实际使用过程中如果没有拷贝构造函数
  大概率会出错，知道为什么吗？因为使用了vector容器存储task对象，当vector需要扩容时，需要将原来的
  对象拷贝到新的内存空间中，这时候调用了拷贝构造函数。如果没有显式定义拷贝函数，默认拷贝构造函数会
  直接将指针拷贝过去，析构函数会将指针指向的原有的资源释放掉，这样运行的时候就会出错了。
*/

Coroutine::Coroutine(const Coroutine &t){          //在有内存申请和释放的类中最好有一个拷贝构造函数
    int stack_size = (1<<20);               
    stack_top = malloc(stack_size);  
    para1 = t.para1;           
    tid = t.tid;
    memcpy(stack_top, t.stack_top, stack_size);
    stack = stack_top + stack_size;          
    memcpy(&ctx, &t.ctx, sizeof(ctx_buf_t));    //寄存器信息并不都是一样的，栈帧的地址变了，其它一样
    //handler = t.handler;
    ctx.buffer[rsp] = (long)stack;              //这是新的栈，不改这个会出现Segmentation fault
    //ctx.buffer[pc_addr] = (long)handler;
    //printf("ID: %d thread has been created...\n", tid);

    /*从拷贝构造函数可以看出，每次都需要重新开辟一个新栈，然后将原来栈的数据拷贝过去，效率低下。所以最好避免这样的拷贝，
      tasks初始化的时候尽量开辟足够大的size，不然扩充的时候会造成大量的拷贝，效率低下且容易出现错误。*/
}

/*下面是移动构造函数的实现，可以看到移动构造函数中，不需要再重新申请新的内存空间，只需要将临时变量的指针置为NULL即可*/
Coroutine::Coroutine(Coroutine &&t) noexcept {
    memcpy(this, &t, sizeof(Coroutine));
    t.stack_top = NULL;
    //printf("ID: %d thread has been move copy...\n", tid);
}

Coroutine::~Coroutine(){
    //printf("ID: %d thread has been destory...\n", tid);
    free(stack_top);
}

void Coroutine::start(){
    __asm__("movq %0, %%rsi;"::"r"(&ctx):"%rsi");       /*先将寄存器信息地址写入r10寄存器，因为不能用start_context的参数传递
                                                          start_context的参数是传给handler的参数，不能动*/
    start_context(para1);                                 /*tid会保存在edi寄存器中，handler函数会取出edi作为其第一个参数，可变
                                                          似乎没有什么好的解决方法，或许可以通过重载start_context？*/
}

int Coroutine::get_id(){
    return tid;
}



