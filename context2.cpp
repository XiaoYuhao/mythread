#include<iostream>
#include<ucontext.h>
#include<unistd.h>
using namespace std;
int main(){
    ucontext_t context;
    getcontext(&context);
    cout<<"yuhaoxiao"<<endl;
    sleep(1);
    setcontext(&context);
    return 0;
}

