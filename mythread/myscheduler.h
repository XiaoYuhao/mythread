#ifndef _MY_SCHEDULER_H_
#define _MY_SCHEDULER_H_

#include<vector>
#include<cstdlib>
#include<sys/epoll.h>
#include"mycoroutine.h"
using namespace std;

const int MAX_EVENT_NUM = 10000;

class Scheduler
{
    Coroutine admin;
    vector<Coroutine> tasks;
    int current_id;
    int epfd;
    int coroutine_count;
    static Scheduler* _instance;
    Scheduler();
private:
    void work();
    void add_task(task_handler_t handler, int para);
    void do_switch(Coroutine &from, Coroutine &to);
    void switch_to_admin();
    int get_current_id();
    void add_wait_fd(int fd, int flags);
public:
    ~Scheduler();
    static Scheduler* create();
    static void add_coroutine(task_handler_t handler, int para);
    static void start();
    static void add_wait(int fd, int flags);
};

struct wait_info{
    int fd;
    int id;
};

#endif