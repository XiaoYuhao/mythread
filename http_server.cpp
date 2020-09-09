/*Linux Server cpp empty template*/
#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<stdlib.h>
#include<unistd.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<string.h>
#include<fcntl.h>
#include<sys/shm.h>
#include<time.h>
#include<signal.h>
#include<errno.h>
#include<sys/epoll.h>
using namespace std;


char http_response_buf[4096]="HTTP/1.1 200 OK\n" \
                            "Date: Mon, 20 Jan 2020 03:22:28 GMT\n" \
                            "Server: Apache/2.4.6 (CentOS) PHP/5.4.16\n" \
                            "Last-Modified: Fri, 17 Jan 2020 09:02:59 GMT\n" \
                            "ETag: \"1ebe-59c52355673f9\"\n" \
                            "Accept-Ranges: bytes\n" \
                            "Content-Length: 42\n" \
                            "Content-Type: text/html; charset=UTF-8\n\n" \
                            "<html><body>I am yuhaoxiao.</body></html>"; //
int startup(int _port){
    int sock=socket(AF_INET,SOCK_STREAM,0);
    if(sock<0){
        perror("socket");
        exit(1);
    }
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

int main() {
    int listen_sock=startup(8080);
    struct sockaddr_in remote;
    socklen_t len=sizeof(struct sockaddr_in);

    int epfd=epoll_create(1024);
    struct epoll_event events[1024],ev;
    
    ev.data.fd=listen_sock;
    ev.events=EPOLLIN;
    
    epoll_ctl(epfd,EPOLL_CTL_ADD,listen_sock,&ev);

    for(;;){
        int nfds=epoll_wait(epfd,events,1024,-1);
        for(int i=0;i<nfds;i++){
            if(events[i].data.fd==listen_sock){//listen_sock上有可读数据
                int client_sock=accept(listen_sock,(struct sockaddr*)&remote,&len);
                printf("accept a connect client: %d...\n",client_sock);
                ev.data.fd=client_sock;
                ev.events=EPOLLIN;
                epoll_ctl(epfd,EPOLL_CTL_ADD,client_sock,&ev);
            }
            else if(events[i].events&EPOLLIN){//有可读数据
                char recv_buf[4096];
                int client_sock=events[i].data.fd;
                int ret=recv(client_sock,recv_buf,sizeof(recv_buf),MSG_DONTWAIT);
                if(ret==0){
                    printf("client: %d disconnect...\n",client_sock);
                    epoll_ctl(epfd,EPOLL_CTL_DEL,client_sock,&ev);
                    close(client_sock);
                    continue;
                }
                printf("has receive %d bytes from client: %d...\n",ret,client_sock);
                printf("%s",recv_buf);
                ev.data.fd=client_sock;
                ev.events=EPOLLOUT;
                epoll_ctl(epfd,EPOLL_CTL_MOD,client_sock,&ev);
            }
            else if(events[i].events&EPOLLOUT){//有可写数据并可写
                int client_sock=events[i].data.fd;
                printf("now can send data to client: %d...\n",client_sock);
                //char send_buf[4096];
                
                int ret=send(client_sock,http_response_buf,strlen(http_response_buf),MSG_DONTWAIT);
                if (ret<0){
                    printf("client: %d send data error\n",client_sock);
                    epoll_ctl(epfd,EPOLL_CTL_DEL,client_sock,&ev);
                    close(client_sock);
                    continue;
                }
                printf("success send %d bytes data to client: %d...",ret,client_sock);

                ev.data.fd=client_sock;
                ev.events=EPOLLIN;
                epoll_ctl(epfd,EPOLL_CTL_MOD,client_sock,&ev);
            }
        }
    }
    close(listen_sock);
    close(epfd);
    return 0;
}
