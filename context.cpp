#include<iostream>
#include<ucontext.h>
#include<unistd.h>
#include<cstring>
#include<cstdlib>
#include<cstdio>
#include<sys/time.h>
#include<queue>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<fcntl.h>
using namespace std;

//char stack[1024*128],stack2[1024*128];
//static ucontext_t main_c,other,func;
struct mythread{
    char sstack[1024*64];
    ucontext_t context;
    int fd;
}Thread[105];

int global_fd;
fd_set rfd;
queue<int> ready;
ucontext_t main_c;

void mytask(int fd);
void register_thread(){
    getcontext(& Thread[global_fd].context);    //获取当前上下文，保存到Thread[global_fd].context结构中
    Thread[global_fd].context.uc_stack.ss_sp=Thread[global_fd].sstack;
    Thread[global_fd].context.uc_stack.ss_size=sizeof(Thread[global_fd].sstack);
    Thread[global_fd].context.uc_stack.ss_flags=0;
    Thread[global_fd].context.uc_link=&main_c;  //uc_link指向后继上下文，当前上下文运行终止时系统会恢复指向的上下文
    Thread[global_fd].fd=global_fd;
    makecontext(&Thread[global_fd].context,(void(*)(void))mytask,1,global_fd);  //
    global_fd++;
}

int startup(int _port)
{
	int sock=socket(AF_INET,SOCK_STREAM,0);
	if(sock<0)
	{
		perror("socket");
		exit(1);
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
	
	struct sockaddr_in local;
	local.sin_family=AF_INET;
	local.sin_port=htons(_port);
	local.sin_addr.s_addr=htonl(INADDR_ANY);
	socklen_t len=sizeof(local);
	
	if(bind(sock,(struct sockaddr*)&local,len)<0)
	{
		perror("bind");
		exit(2);
	}
	
	if(listen(sock,5)<0)
	{
		perror("listen");
		exit(3);
	}
	
	return sock;
}

void scheduler(){
    int listen_sock=startup(6123);
    struct sockaddr_in remote;
    socklen_t len=sizeof(struct sockaddr_in);

    global_fd++;
    while(1){
        FD_ZERO(&rfd);
        FD_SET(listen_sock,&rfd);
        if(select(global_fd,&rfd,NULL,NULL,0)>0){
            if(FD_ISSET(listen_sock,&rfd)){
                FD_CLR(listen_sock,&rfd);
                
            }
            for(int i=0;i<global_fd;i++){           //select的缺陷之一，需要遍历一遍才能找到可读写的fd，效率很低
                if(FD_ISSET(i,&rfd)){               //找到可读写的fd，将等待它们的协程号码放入就绪队列中，等待被调度上台运行
                    FD_CLR(i,&rfd); 
                    ready.push(i);
                }
            }
        }
        int todo=ready.front();
        ready.pop();
        //切换协程
        swapcontext(&main_c,&Thread[todo].context);
    }
    
   /*
   for(int i=0;i<global_fd;i++){
       swapcontext(&main_c,&Thread[i].context);
   }
   for(int i=0;i<global_fd;i++){
       swapcontext(&main_c,&Thread[i].context);
   }
   */
}

void swap_thread(int fd){
    getcontext(&Thread[fd].context);

}

void mytask(int fd){
    printf("This is %d ...\n",fd);
    swapcontext(&Thread[fd].context,&main_c);
    printf("That is %d ...\n",fd);
    return;
}


int main(){
    global_fd=0;
    printf("start to register thread...\n");
    for(int i=0;i<50;i++){
        register_thread();
    }
    printf("start scheduler...\n");
    scheduler();
    printf("end work...\n");
    return 0;
}

/*
void function(){
    char temp[1024];
    cout<<"run this"<<endl;
    sleep(1);
    return;
}

void function2(){
    cout<<"I am yuhaoxiao."<<endl;
    return;
}

int main(){
    cout<<"in main"<<endl;
    char stack[1024*128],stack2[1024*128];
    memset(stack,0,sizeof(stack));
    memset(stack2,0,sizeof(stack2));
    ucontext_t main_c,other,func;
    getcontext(&main_c);

    getcontext(&func);

    func.uc_stack.ss_sp=stack2;
    func.uc_stack.ss_size=sizeof(stack2);
    func.uc_stack.ss_flags=0;
    func.uc_link=&other;

    makecontext(&func,function2,0);
 
    main_c.uc_stack.ss_sp=stack;
    main_c.uc_stack.ss_size=sizeof(stack);
    main_c.uc_stack.ss_flags=0;
    main_c.uc_link=&func;
    
    makecontext(&main_c,function,0);
    cout<<"test flag 1"<<endl;
    swapcontext(&other,&main_c);
    //setcontext(&main);
    cout<<"in main"<<endl;
    return 0;
}
*/
/*
遇到的问题：
1.void function()函数中的
sleep(1)函数
当ucontext和stack的定义放在main函数中
运行时等待1秒：会抛出segmentation fault错误
当stack定义在main函数外面作为全局变量时
运行时立即派出segmentation fault
当stack定义在main函数中而ucontext定义在main函数中时
运行成功
...
发现把stack大小开到1024*128 就不会出现上述问题了...
...

*/