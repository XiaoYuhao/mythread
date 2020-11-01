#include<stdlib.h>
#include<string.h>
#include<cstdio>
#include<unistd.h>
#include"myscheduler.h"

extern "C"
{
    extern int save_context(ctx_buf_t *from);
    extern int restore_context(ctx_buf_t *to);
    extern int start_context(int id);
}

Scheduler::Scheduler():admin(-1,NULL,0),coroutine_count(0){
    epfd = epoll_create(MAX_EVENT_NUM);
}

Scheduler::~Scheduler(){
    close(epfd);
}

Scheduler* Scheduler::create(){
    if(_instance == NULL){
        _instance = new Scheduler;
    }   
    return _instance;
}

void Scheduler::add_task(task_handler_t handler, int para){
    /*
    tasks.emplace_back(coroutine_count++, handler, para);       //push_back会先构造临时变量，再拷贝构造进vector，而emplace_back则原地构造，
                                                                //另外需要注意的是tasks初始大小很小，扩充大小的时候需要拷贝原来的元素过去，再
                                                                //调用析构函数，需要注意，尤其是有内存申请和释放情况下。
    */
   tasks[coroutine_count] = Coroutine(coroutine_count, handler, para);
   coroutine_count++;
}

void Scheduler::do_switch(Coroutine &from, Coroutine &to){
    //printf("from ID: %d switch to ID :%d \n", from.get_id(), to.get_id());
    int ret = save_context(&from.ctx);       /**/
    if(ret == 0){
        current_id = to.get_id();
        restore_context(&to.ctx);
    }
    else{
        current_id = from.get_id();
        return;
    }
}

void Scheduler::switch_to_admin(){
    //printf("from ID: %d switch to admin...\n", current_id);
    Coroutine &from = tasks[current_id];            //这里引入vector erase之后出现了大bug...
    do_switch(from, admin);
}

int Scheduler::get_current_id(){
    return current_id;
}

void Scheduler::add_wait_fd(int fd, int flags){
    wait_info wi;
    wi.fd = fd;
    wi.id = current_id;
    epoll_event event;
    event.data.ptr = (void *)&wi;
    event.events = flags;
    epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &event);
    tasks[current_id].status = WAIT;                          //将协程状态置为WAIT等待状态
    switch_to_admin();
}

void Scheduler::del_wait_fd(int fd){
    epoll_ctl(epfd, EPOLL_CTL_DEL, fd, 0);
}


void Scheduler::work(){
    epoll_event events[MAX_EVENT_NUM];
    while(1){
        int num = epoll_wait(epfd, events, MAX_EVENT_NUM, 0); //epoll_wait立即返回，去寻找进行就绪的fd
        for(int i=0;i<num;i++){
            wait_info *wi = (wait_info *)events[i].data.ptr;
            tasks[wi->id].status = RUNABLE;                   //将协程状态置为RUNABLE，就绪态 
            printf("Id: %d is runable...\n", wi->id);
        }

        for(auto t = tasks.begin(); t!=tasks.end();){
            if((*t).second.status == INIT){
                (*t).second.status = RUNABLE;
                int ret = save_context(&admin.ctx);
                if(ret == 0){
                    current_id = (*t).first;
                    printf("ID :%d start...\n", current_id);
                    (*t).second.start();
                    switch_to_admin();
                }
            }
            else if((*t).second.status == RUNABLE){
                do_switch(admin, (*t).second);
            }
            if((*t).second.status == DEAD){
                printf("ID :%d task is dead.\n", (*t).first);
                t = tasks.erase(t);
                continue;
            }
            t++;
        }
        if(tasks.empty()){
            printf("tasks queue is empty...\n");
            break;
        }
        /*
        for(auto iter = tasks.begin(); iter!=tasks.end();iter++){
            if((*iter).status == INIT){
                (*iter).status = RUNABLE;
                int ret = save_context(&admin.ctx);
                if(ret == 0){
                    current_id = (*iter).get_id();
                    printf("ID :%d start...\n", current_id);
                    (*iter).start();
                    switch_to_admin();
                }
            }
            else if((*iter).status == RUNABLE){
                do_switch(admin,(*iter));
            }
            if((*iter).status == DEAD){
                continue;
            }
        }
        */
    } 
}

Scheduler* Scheduler::_instance = NULL;
Scheduler* schedptr = Scheduler::create();

/* Open API */
void Scheduler::add_coroutine(task_handler_t handler, int para){
    _instance->add_task(handler, para);
}

void Scheduler::start(){
    _instance->work();
}

void Scheduler::add_wait(int fd, int flags){
    _instance->add_wait_fd(fd, flags);
}

void Scheduler::del_wait(int fd){
    _instance->del_wait_fd(fd);
}

void Scheduler::finish_coroutine(){
    _instance->tasks[_instance->current_id].status = DEAD;
    _instance->switch_to_admin();
}