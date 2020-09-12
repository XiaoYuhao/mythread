#include<unistd.h>
#include<stdlib.h>
#include"pthreadpool.h"

int func(int id){
    int t = rand()%1000+1;
    usleep(t*1000);
    printf("Task id: %d time: %d\n", id, t);
    return id;
}

int main(){
    pthreadpool *pool = (pthreadpool*)malloc(sizeof(pthreadpool));
    createpool(pool, 20, 100);
    task_t t;
    t.handler = &func;
    for(int i=0;i<50;i++){
        t.arg = (void*)i;
        append(pool, t);
    }
    sleep(10);
    destroypool(pool);
    return 0;
}