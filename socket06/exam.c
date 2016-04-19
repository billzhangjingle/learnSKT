/*编程程序，要去实现如下功能：
	父进程创建子进程1和子进程2、子进程1向子进程2发送可靠信号，并传送额外数据为子进程1的pid*2;
	子进程2接受可靠信号的值，并发送给父进程，父进程把接受的值进行打印。
	提示：用sigqueue和sigaction实现*/

#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>

//利用全局变量来交换数据
int  myarry[10];

//处理信号之间的关系,根据num来区分
//问题？ 怎样保证他们的先后顺序呢？ 通过5秒， 10秒的睡眠 
//父进程->1号进程--->2号进程----->父进程
void myhandle(int num, siginfo_t *st, void *p)
{
	if( num == SIGRTMIN+1){
		//父进程给1号进程发送的 2号进程id
		printf("1号进程收到父进程发送的2号进程id:%d\n", st->si_value.sival_int);
		myarry[1] = st->si_value.sival_int;
	
	}
	
	if( num == SIGRTMIN+2){
		//1号进程给 2号进程发送的自己的id *2
		
	    printf("2号进程收到1进程pid:%d\n", st->si_value.sival_int);
		myarry[3] = st->si_value.sival_int;
		//王保明果然用的是3
	}
	
	if( num == SIGRTMIN+3){
		//2 号进程给父进程发送的1号进程的id 打印出来
		
		printf("我是父进程，我打印信息：%d\n",st->si_value.sival_int );
		
	}
	
	
}




int main(void)
{
	int i = 0;
	
	//int sigqueue(pid_t pid, int sig, const union sigval value);
	//int sigaction(int signum, const struct sigaction *act,
    //                 struct sigaction *oldact);
	
	pid_t pid;
	struct sigaction act;
	
	
	//用的比较复杂信号注册函数
	act.sa_sigaction   = myhandle;
	//act.sa_sigaction = 
	act.sa_flags	 = SA_SIGINFO;
	
	
	
    //
   if(sigaction( SIGRTMIN+1, &act, NULL) == -1){
	   perror("func sigction err:");
	   return -2;
   }
   
   if(sigaction( SIGRTMIN+2, &act, NULL) == -1){
	   perror("func sigction err:");
	   return -2;
   }
   
   if(sigaction( SIGRTMIN+3, &act, NULL) == -1){
	   perror("func sigction err:");
	   return -2;
   }
   //注册函数：这是第一步
   
   //创建进程,将进程号保存起来。
   
   
   
  
  // myarry[0] = fork();
	//还是父进程把子进程的 PID 保存起来了。
	for(i=0; i<2; i++){
		myarry[i] = pid = fork();
		
		if(pid == 0){
			break;
		}else if(pid > 0){
			;
		}
	}
	
	//果然还是用父进程把 2号进程的 pid 给 1号进程发送过去。
	
	if(pid > 0){
		printf("parent is running\n");
		
		union sigval mysigval;
		mysigval.sival_int = myarry[1];
		
		//给1号进程发送信号， 值传递的是
		int res;
		res = sigqueue(myarry[0], SIGRTMIN+1, mysigval);
		
		
	}
	if(pid == 0 && i == 0){
		printf("1号 开始运行\n");
		sleep(5);
		
		union sigval mysigval;
		mysigval.sival_int = 2 * getpid();
		
		//给1号进程发送信号， 值传递的是
		int res;
		res = sigqueue(myarry[1], SIGRTMIN+2, mysigval);
		
		exit(0);
		
	}
	
	if(pid == 0 && i == 1){	
		
		printf("2号 开始运行\n");
		sleep(5);
		
		union sigval mysigval;
		//mysigval.sival_int = myarry[0];
		mysigval.sival_int = myarry[3];
		
		//给1号进程发送信号， 值传递的是
		int res;
		res = sigqueue(getppid(), SIGRTMIN+3, mysigval);
		
		exit(0);
		
	}
   //1号进程怎么知道2号的进程呢？？
   //1 号进程给2号发送信号。
   //最终还是 通过父进程开始的中转，
	//for(i=0; i<2; i++){
		
	//pid_t wait(int *status);
    //pid_t waitpid(pid_t pid, int *status, int options);
    //int waitid(idtype_t idtype, id_t id, siginfo_t *infop, int options);
	sleep(10);
	pid_t mypid;
	//清理函数
	while( mypid = waitpid(-1, NULL, WNOHANG) > 0){
		printf("子进程退出：%d\n", mypid);
	}
	
	//在进程之间没有通信的情况下， 清理函数没有派上用场
	
	
	return 0;
}
