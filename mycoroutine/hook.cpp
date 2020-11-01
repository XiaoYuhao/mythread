#include<sys/types.h>
#include<stdio.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<fcntl.h>
#include<sys/epoll.h>
#include<dlfcn.h>
#include<iostream>
#include"myscheduler.h"



extern "C" {

int read(int fd, void *buf, size_t nbytes){
    int flags = fcntl(fd, F_GETFL, 0);

    size_t (*readcp)(int fd, void *buf, size_t nbytes);
    readcp = (size_t (*)(int fd, void *buf, size_t nbytes))dlsym(RTLD_NEXT, "read");
    
    if(flags&O_NONBLOCK){
        return readcp(fd, buf, nbytes);
        return 0;
    }

    std::cout<<"Task into Read WAIT status"<<endl;
    Scheduler::add_wait(fd, EPOLLIN);
    Scheduler::del_wait(fd);
    return readcp(fd, buf, nbytes);
}



int accept(int sockfd, struct sockaddr *cliaddr, socklen_t *addrlen){
    int flags = fcntl(sockfd, F_GETFL, 0);
    
    int (*acceptcp)(int sockfd, struct sockaddr *cliaddr, socklen_t *addrlen);
    acceptcp = (int (*)(int sockfd, struct sockaddr *cliaddr, socklen_t *addrlen))dlsym(RTLD_NEXT, "accept");

    if(flags&O_NONBLOCK){
        return acceptcp(sockfd, cliaddr, addrlen);
    }

    std::cout<<"Task into Accept WAIT status"<<endl;
    Scheduler::add_wait(sockfd, EPOLLIN);
    Scheduler::del_wait(sockfd);
    return acceptcp(sockfd, cliaddr, addrlen);
}

}

