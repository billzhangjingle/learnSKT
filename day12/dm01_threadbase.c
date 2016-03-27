#include <unistd.h>
#include <sys/types.h>

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

#include <pthread.h>
//进程  和  	线程
// fork 		pthread_creat
//	pid 					tid
//    pcb       tcb
//	僵死进程			僵死线程


/*
int pthread_create(pthread_t *restrict thread,
              const pthread_attr_t *restrict attr,
              void *(*start_routine)(void*), void *restrict arg);
              */

//线程从现象来讲：依赖于进程生命周期。。。和子进程不一样、。。
//线程函数

//程序：放在磁盘上，静态数据 一堆指令的集合 数据+指令组成
//进程：程序的动态执行过程，相当于一个cpu的调度； 数据段、堆栈段 、代码段、PCB控制块
	//linux内核通过pcb来控制进程。。有了进程内核可以管理资源。。 进程是资源竞争的基本单位
	
//线程：程序执行最小单元。。。线程体，就是一个函数调用。。。

//从数据共享、内存四区的角度理解线程
//线程可以使用进程的全变量。。。
//抛砖1:如果从进程里面往线程中传入数据
//      如果从线程中传出来。。。。

//方法1：全局变量。。。。//指针

int g_num = 0;

void *thread_routine(void* arg)
{
	int i = 0;
	//sleep(3);
	printf("g_num:%d", g_num);
	printf("我是线程....\n");
	for (i=0; i<20; i++)
	{
		printf("B");
		fflush(stdout);
	}
	return NULL;
} 
int main()
{
	
	int i = 0;
	printf("hello....\n");
	
	pthread_t tid;
	g_num = 11;
	
	pthread_create(&tid, NULL, thread_routine, NULL);
	//pthread_create(&tid, NULL, thread_routine, NULL);
	
	
	for (i=0; i<20; i++)
	{
		printf("A");
		fflush(stdout);
	}
	sleep(1);
	
	return 0;
}


