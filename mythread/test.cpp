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

#include"mythread.h"

int startup(int _port){
    int sock=socket(AF_INET,SOCK_STREAM,0);
    if(sock<0){
        perror("socket");
        exit(1);
    }
    /*
    int val;
    if(val=fcntl(sock,F_GETFL,0)<0){
        perror("fcntl");
        close(sock);
        return 0;
    }
    if(fcntl(sock,F_SETFL,val|O_NONBLOCK)<0){
        perror("fcntl");
        close(sock);
        return 0;
    }
    */
    int opt=1;
    setsockopt(sock,SOL_SOCKET,SO_REUSEADDR,(const void*)&opt,sizeof(opt));

    struct sockaddr_in local;
    local.sin_family=AF_INET;
    local.sin_port=htons(_port);
    local.sin_addr.s_addr=htonl(INADDR_ANY);
    socklen_t len=sizeof(local);

    if(bind(sock,(struct sockaddr*)&local,len)<0){
        perror("bind");
        exit(2);
    }
    
    if(listen(sock,5)<0){
        perror("listen");
        exit(3);
    }

    return sock;
}

void addfd(int epollfd, int fd){
    epoll_event event;
    event.data.fd = fd;
    event.events = EPOLLIN | EPOLLET;
    epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &event);
}

void removefd(int epollfd, int fd){
    epoll_ctl(epollfd, EPOLL_CTL_DEL, fd, 0);
    close(fd);
}

extern Scheduler *schedptr;

void *func(int fd){
    /*
    printf("This is thread A. para: %d \n", fd);
    while(1){
        printf("Hello! I am yuhaoxiao. para: %d\n", fd);
        schedptr->switch_to_admin();
        usleep(1000*100);
    }*/
    
    char buf[1024];
    char sendbuf[1024];
    sprintf(sendbuf, "server has received message.\n");
    while(true){
        //printf("come into read function.\n");
        size_t ret = read(fd, (void*)buf, sizeof(buf));
        if(ret<0){
            printf("read error.\n");
            exit(0);
        }
        if(ret==0){
            printf("client quit.\n");
            break;
        }
        printf("recv message : %s \n", buf);
        ret = send(fd, sendbuf, strlen(sendbuf), MSG_DONTWAIT);
    }
    return NULL;
}

void *server(int para1){
    int listen_sock = startup(11234);
    struct sockaddr_in remote;
    socklen_t len=sizeof(struct sockaddr_in);
    while(true){
        int client_sock = accept(listen_sock, (struct sockaddr*)&remote, &len);
        schedptr->add_thread(func, client_sock);
        printf("client fd :%d is connect.\n", client_sock);

    }
    return NULL;
}

int main(){
    /*
    for(int i=0;i<50;i++){
        schedptr->add_thread(func,521);
    }*/
    schedptr->add_thread(server, 0);
    schedptr->work();
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