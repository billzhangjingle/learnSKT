#include <unistd.h>
#include <sys/types.h>

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

#include <pthread.h>



int g_num = 0;

int 	nNum, nLoop;

//定义锁
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

//定义条件并初始化
pthread_cond_t my_condition=PTHREAD_COND_INITIALIZER;

#define CUSTOM_COUNT 2         //生产者个数
#define PRODUCT_COUNT 2        //消费者


  // int pthread_mutex_lock(pthread_mutex_t *mutex);
  //     int pthread_mutex_trylock(pthread_mutex_t *mutex);
   //    int pthread_mutex_unlock(pthread_mutex_t *mutex);


//posix 线程库的函数 线程库
//消费者线程
void *consume(void* arg)
{
	int i = 0;
	
	printf("consum\n");
	pthread_exit(0);

} 

//生产者线程
void *produce(void* arg)
{
	int i = 0;
	printf("produce\n");

	pthread_exit(0);

} 

//结论：return arg 和 pthread_exit（）的结果都可以让pthread_join 接过来
int main()
{
	int		i =0;
	int		tidArray[CUSTOM_COUNT+PRODUCT_COUNT];//线程个数
	
	//创建消费者线程
	for (i=0; i<CUSTOM_COUNT; i++)
	{
		pthread_create(&tidArray[i], NULL, consume, NULL);
	}
	
	//创建生产线程
	for (i=0; i<PRODUCT_COUNT; i++)
	{
		pthread_create(&tidArray[i+CUSTOM_COUNT], NULL, produce, NULL);
	}
	
	
	
	for (i=0; i<CUSTOM_COUNT+PRODUCT_COUNT; i++)
	{
		pthread_join(tidArray[i], NULL); //等待线程结束。。。
	}
	
	
	printf("进程也要结束1233\n");
	
	return 0;
}


