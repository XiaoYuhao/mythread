#include<stdio.h>
#include<unistd.h>
#include"mythread.h"

Scheduler sched;

void *func(int id){
    printf("This is thread A. ID: %d \n", id);
    while(1){
        printf("Hello! I am yuhaoxiao. ID: %d\n", id);
        sched.switch_to_admin(id);
        usleep(1000*100);
    }
    return NULL;
}

int main(){
    
    for(int i=0;i<50;i++){
        sched.add_thread(i,func);
    }
    sched.work();
    printf("what the fuck...\n");
    return 0;
}
/*
thread_t thread_main;
thread_t g_thread_A;
thread_t g_thread_B;

void *func_A(int id){
    printf("This is thread A. ID: %d \n", id);
    while(1){
        printf("Hello! I am yuhaoxiao.\n");
        do_switch(&g_thread_A, &g_thread_B);
        sleep(1);
    }
    return NULL;
}

void *func_B(int id){
    printf("This is thread B. ID: %d \n", id);
    while(1){
        printf("Hello! I am hebetien.\n");
        do_switch(&g_thread_B, &g_thread_A);
        sleep(1);
    }
    return NULL;
}

int main(){
    //thread_create(&thread_main, 0, scheduler);
    thread_create(&g_thread_A, 1, func_A);
    thread_create(&g_thread_B, 2, func_B);
    thread_start(&g_thread_A);
    thread_destory(&g_thread_A);
    thread_destory(&g_thread_B);
    return 0;
}*/