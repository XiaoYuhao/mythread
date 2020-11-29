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
int sock;

void senddata(){
	char sendbuf[15]="12345678901234";
	int ret=send(sock,sendbuf,sizeof(sendbuf),MSG_DONTWAIT);
	printf("send返回值：%d\n",ret);
	alarm(3);//3秒之后再次唤醒
}
int task()
{
    printf("now in task...");
	sock=socket(AF_INET,SOCK_STREAM,0);
	if(sock<0)
	{
		perror("socket");
		return 1;
	}
	
	int val;
	if(val=fcntl(sock,F_GETFL,0)<0){//获取文件状态标志
		perror("fcntl");
		close(sock);
		return 0;
	}
	if(fcntl(sock,F_SETFL,val|O_NONBLOCK)<0){//设置文件状态标志
		perror("fcntl");
		close(sock);
		return 0;
	}
	
	if(signal(SIGALRM,(void(*)(int))senddata)==SIG_ERR)
	{
		perror("signal");
		return 0;
	}
	
	struct sockaddr_in server;
	server.sin_family=AF_INET;
	server.sin_port=htons(atoi("6123"));
	server.sin_addr.s_addr=inet_addr("0.0.0.0");
	socklen_t len=sizeof(struct sockaddr_in);
	
	fd_set rfd;
	FD_ZERO(&rfd);
	FD_SET(sock,&rfd);
	switch(select(sock+1,&rfd,NULL,NULL,NULL))
	{
		case -1:
			perror("select");
			break;
		case 0:
			printf("超时！\n");
			break;
		default:
			connect(sock,(struct sockaddr*)&server,len);
	}
	
	printf("连接成功！\n");
	
	alarm(3);
	
	char recvbuf[100];
	int iDataNum;
	
	int ret;
	
	while(1){
		FD_ZERO(&rfd);
		FD_SET(sock,&rfd);
		switch(select(sock+1,&rfd,NULL,NULL,NULL))
		{
			case -1:
				break;
			case 0:
				sleep(1);
				printf("超时\n");
				break;
			default:
				ret=recv(sock,recvbuf,sizeof(recvbuf),MSG_DONTWAIT);
				printf("read返回值：%d\n",ret);
		}
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
    threadPool pool(10);
    int test_num=3;
    Test tt_bind;
    for(int i=0;i<test_num;i++){
        pool.append(std::bind(task));
    }
    while(1){
        sleep(10);
    }
    return 0;
}
		