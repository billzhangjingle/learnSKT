#include <unistd.h>
#include <sys/types.h>

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

#include <pthread.h>


typedef struct Teacher
{
	char name[64];
	int age ;
}Teacher;

int g_num = 0;

//posix 线程库的函数 线程库

void *thread_routine(void* arg)
{
	int i = 0;
	
	Teacher *p = NULL;
	p = (Teacher *)arg;
	
	printf("name:%s \n", p->name);
	printf("age:%d \n", p->age);
	 p->age =  p->age + 1;
	
	pthread_detach(pthread_self());
	printf("g_num:%d 线程id: %u \n", g_num, pthread_self());
	
	printf("我是线程....\n");
	for (i=0; i<20; i++)
	{
		printf("B");
		fflush(stdout);
	}
	sleep(2);
	printf("子线程睡眠3秒后。。。。。退出了\n");
	
	//pthread_exit(arg);
	return arg;
	//return NULL;
} 

//结论：return arg 和 pthread_exit（）的结果都可以让pthread_join 接过来
int main()
{
	
	int i = 0;
	printf("hello....\n");
	Teacher t1 = {"myname", 35};
	
	pthread_t tid;
	g_num = 11;
	
	pthread_create(&tid, NULL, thread_routine, &t1);
	
	
	for (i=0; i<20; i++)
	{
		printf("A");
		fflush(stdout);
	}
	//sleep(1);
	//pthread_cancel(pthread_t thread);

	//接返回值
	{
		Teacher*p = NULL;
		pthread_join(tid, &p); //等待线程结束。。。
		printf("name:%s \n", p->name);
		printf("age:%d \n", p->age);		 
	}
	
	printf("进程也要结束\n");
	
	return 0;
}


