#include <sys/types.h>  
#include <sys/socket.h>  
#include <netinet/in.h>  
#include <arpa/inet.h>  
#include <assert.h>  
#include <stdio.h>  
#include <unistd.h>  
#include <errno.h>  
#include <string.h>  
#include <string>
#include <map>
#include <fcntl.h>  
#include <stdlib.h>  
#include <sys/epoll.h>  
#include <signal.h>  
#include <sys/wait.h>  
#include <sys/stat.h>
#include <sys/mman.h>

#include "myscheduler.h"


void *func(int fd){
    printf("ID: %d coroutine is running...\n", fd);
    return NULL;
}


int main(){
    for(int i=0;i<10;i++){
        Scheduler::add_coroutine(func, i);
    }
    
    Scheduler::start();
    return 0;
}