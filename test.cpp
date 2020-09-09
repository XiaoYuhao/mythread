#include<iostream>
using namespace std;
//int num[1024*256*20];

int main(){
    char num[1024*1024*20];
    num[0]='0';
    return 0;
}

/*
测试CentOS下默认函数栈大小
在main中开1024*1024*20 (20M)数组 : Segmentation Fault
在main中开1024*1024*10 (10M)数组 : Segmentation Fault
在main中开1024*8192    (8M linux默认函数栈大小) : Segmentation Fault
在main中开1024*8180    (<8M) : Run Success
在main中开1024*1024*5  (5M)数组 : Run Success
上述结果说明CentOS默认函数栈大小为小于等于1024*8192 Bytes

在CentOS 使用 ulimit -s 命令查看默认函数栈大小，与实验结果相符
可以使用ulimit -s sum 来将函数栈大小设置为sum KB
*/