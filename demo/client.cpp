//client.c
#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<unistd.h>
#include<stdlib.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<string.h>
#include<fcntl.h>
#include<sys/shm.h>
#include<time.h>
#include<signal.h>
#include"thread_pool.h"

int task(int id)
{
    printf("now in task...\n");
	int sock=socket(AF_INET,SOCK_STREAM,0);
	if(sock<0)
	{
		perror("socket");
		return 1;
	}
	
	struct sockaddr_in server;
	server.sin_family=AF_INET;
	server.sin_port=htons(atoi("11234"));
	server.sin_addr.s_addr=inet_addr("111.231.146.249");
	socklen_t len=sizeof(struct sockaddr_in);
	
	int ret = 0;
	ret = connect(sock, (sockaddr*)&server, len);
	if(ret<0){
		printf("connect failed...\n");
		exit(0);
	}
	printf("connect successful...\n");

	char recvbuf[1024];
	char sendbuf[1024];
	sprintf(sendbuf, "client id: %d send message.", id);
	while(true){
		ret = send(sock, sendbuf, strlen(sendbuf), MSG_DONTWAIT);
		if(ret < 0){
			printf("send failed...\n");
			exit(0);
		}
		//printf("sock :%d send successfully : %d bytes\n", id, ret);
		ret = recv(sock, recvbuf, sizeof(recvbuf), 0);
		if(ret <= 0){
			printf("recv failed...\n");
			exit(0);
		}
		printf("sock: %d %s", id, recvbuf);
		sleep(3);
	}

	close(sock);
	return 0;
}


class Test
{
public:
	void process_no_static_bind(const int i) {
		printf("this is %d ...",i);
	}
};

int main(){
    threadPool pool(500);
    int test_num=500;
    Test tt_bind;
    for(int i=0;i<test_num;i++){
        pool.append(std::bind(task, i));
    }
    while(1){
        sleep(10);
    }
    return 0;
}
		