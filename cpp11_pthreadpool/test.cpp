#include<iostream>
#include<unistd.h>
#include<cstdlib>
#include"pthreadpool.h"
using namespace std;

int func(int id){
    int t = rand()%1000+1;
    usleep(t*1000);
    cout<<"Task id: "<<id<<"time: "<<t<<endl;
    return id;
}


int main(){
    ThreadPool pool;
    for(int i=0;i<50;i++){
        pool.append(bind(func, i));
    }
    sleep(10);
    return 1;
}

