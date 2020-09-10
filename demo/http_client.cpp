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
using namespace std;

int main() {
    int sock=socket(AF_INET,SOCK_STREAM,0);
    if(sock<0){
        perror("socket");
        return 1;
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

    struct sockaddr_in server;
    server.sin_family=AF_INET;
    server.sin_port=htons(80);
    server.sin_addr.s_addr=inet_addr("0.0.0.0");
    socklen_t len=sizeof(struct sockaddr_in);
    
}