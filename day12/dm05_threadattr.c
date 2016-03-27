#include <unistd.h>
#include <sys/types.h>
#include <pthread.h>

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

#define ERR_EXIT(m) \
        do \
        { \
                perror(m); \
                exit(EXIT_FAILURE); \
        } while(0)

int main(void)
{
	//定义线程属性  结构
	pthread_attr_t attr;
	//初始化获取线程默认的属性数据 
	pthread_attr_init(&attr);
	
	//int a[1024*1024*11];
	
	int state;
	
	//获取线程的分离属性
	pthread_attr_getdetachstate(&attr, &state);
	if (state == PTHREAD_CREATE_JOINABLE)
		printf("detachstate:PTHREAD_CREATE_JOINABLE\n");
	else if (state == PTHREAD_CREATE_DETACHED)
		printf("detachstate:PTHREAD_CREATE_DETACHED");

	size_t size;
	//获取线程的栈大小
	pthread_attr_getstacksize(&attr, &size);
	printf("线程的栈stacksize:%d\n", size);
	
	//获取获取与设置栈溢出保护区大小
	pthread_attr_getguardsize(&attr, &size);
	printf("栈溢出保护区guardsize:%d\n", size);

	int scope;
	//线程的竞争范围  进程范围内竞争 系统范围内竞争
	pthread_attr_getscope(&attr, &scope);
	if (scope == PTHREAD_SCOPE_PROCESS)
		printf("进程范围内竞争scope:PTHREAD_SCOPE_PROCESS\n");
	if (scope == PTHREAD_SCOPE_SYSTEM)
		printf("系统范围内竞争scope:PTHREAD_SCOPE_SYSTEM\n");
	

	int policy;
	//获取默认线程调度策略
	pthread_attr_getschedpolicy(&attr, &policy);
	if (policy == SCHED_FIFO) //如果线程有相同的优先级 按照先进先出的方式来调度线程
		printf("policy:SCHED_FIFO\n");
	else if (policy == SCHED_RR) //如果线程有相同的优先级，后来的线程可以抢占式调度
		printf("policy:SCHED_RR\n");
	else if (policy == SCHED_OTHER) //其他情况
		printf("policy:SCHED_OTHER\n");
	

	int inheritsched;
	//获取与设置继承的调度策略
	pthread_attr_getinheritsched(&attr, &inheritsched);
	if (inheritsched == PTHREAD_INHERIT_SCHED)
		printf("inheritsched:PTHREAD_INHERIT_SCHED\n");
	else if (inheritsched == PTHREAD_EXPLICIT_SCHED)
                printf("inheritsched:PTHREAD_EXPLICIT_SCHED\n");

	
	struct sched_param param;
	//获取与设置调度参数 只需要关注线程的优先级
	pthread_attr_getschedparam(&attr, &param);
	printf("线程调度优先级sched_priority:%d\n", param.sched_priority);


	pthread_attr_destroy(&attr);

	int level;
	//获取与设置并发级别 
	//默认值是0 表示内核按照自己的合适的方式来映射
	level = pthread_getconcurrency();
	printf("level:%d\n", level);
	
	
	char buf[1024*10];
	strcpy(buf, "aaaaaaadddddddd");
	printf("buf:%s\n", buf);
	
	return 0;
}
