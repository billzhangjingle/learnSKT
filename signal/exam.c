/*考试题
要求：父进程创建子进程1和子进程2、子进程1向子进程2发送可靠信号，并传送额外数据为子进程1的pid*2;
	子进程2接受可靠信号的值，并发送给父进程，父进程把接受的值进行打印。
	提示：用sigqueue和sigaction实现
*/

#include <unistd.h>
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>

#

int main(void)
{
	 //int sigqueue(pid_t pid, int sig, const union sigval value);
	 
	 //int sigaction(int signum, const struct sigaction *act,
     //                struct sigaction *oldact);
	 
	 pid_t 
	 
	
	return 0;
}