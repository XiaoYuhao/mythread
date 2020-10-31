#include<sys/types.h>
#include<stdlib.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<string.h>
#include<fcntl.h>
#include<sys/shm.h>
#include<time.h>
#include<signal.h>
#include<errno.h>
#include<sys/epoll.h>
#include<dlfcn.h>
#include<iostream>
#include"mythread.h"

extern Scheduler sched;

size_t read(int fd, void *buf, size_t nbytes){
    int flags = fcntl(fd, F_GETFL, 0);

    size_t (*readcp)(int fd, void *buf, size_t nbytes);
    readcp = (size_t (*)(int fd, void *buf, size_t nbytes))dlsym(RTLD_NEXT, "read");

    if(flags&O_NONBLOCK){
        return readcp(fd, buf, nbytes);
    }

    sched.add_wait_fd(fd, EPOLLIN | EPOLLET);
    std::cout<<"Task into read WAIT status"<<endl;
    return readcp(fd, buf, nbytes);
}

int accept(int sockfd, struct sockaddr *cliaddr, socklen_t *addrlen){
    int flags = fcntl(sockfd, F_GETFL, 0);

    int (*acceptcp)(int sockfd, struct sockaddr *cliaddr, socklen_t *addrlen);
    acceptcp = (int (*)(int sockfd, struct sockaddr *cliaddr, socklen_t *addrlen))dlsym(RTLD_NEXT, "accept");

    if(flags&O_NONBLOCK){
        return acceptcp(sockfd, cliaddr, addrlen);
    }

    sched.add_wait_fd(sockfd, EPOLLIN | EPOLLET);
    std::cout<<"Task into accept WAIT status"<<endl;
    return acceptcp(sockfd, cliaddr, addrlen);
}

int printf(const char *format, ...){
    int (*printfcp)(const char *format, ...);
    printfcp = (int (*)(const char *format, ...))dlsym(RTLD_NEXT, "vprintf");

    printfcp("hook successful!\n");


    return 1;
}