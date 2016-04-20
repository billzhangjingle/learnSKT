
#include <unistd.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <fcntl.h>

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <signal.h>
#include <arpa/inet.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

// 自己定义一个错误的打印信息
#define ERR_EXIT(m)  \
		do{ \
			perror(m); \
			exit(EXIT_FAILURE); \
		}while(0)
			

int main(void)
{
	int sockfd;
	
	//int socket(int domain, int type, int protocol);
	sockfd = socket(AF_INET,SOCK_STREAM, 0);
	if(sockfd < 0){
		perror("socket err\n");
		exit(0);
	}
	
	//给绑定的地址赋值。
	//刚刚找到查找结构的地址， man 7 ip 熟练使用man 手册
	// 主机序号 转化为网络序
	struct sockaddr_in myaddr;
	myaddr.sin_family  =  AF_INET;
	myaddr.sin_port   =  htons(8001);
	inet_aton("127.0.0.1", &myaddr.sin_addr); 
	
	//添加地址复用的功能
	int optval = 1;
	if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) < 0)
	{
		perror("setsockopt bind\n");
		exit(0);
	}
	
	
	//绑定一定的地址
	 //int bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
	if( bind(sockfd, (const struct sockaddr *)&myaddr,  sizeof(myaddr)) == -1 ){
		perror("bind err");
		exit(0);
	}
	
	//第二个参数是监听的队列的长度
	if ( listen(sockfd, SOMAXCONN)<0  )
	{
		perror("fun listen\n");
		exit(0);
	}
	
	//一般也是需要转换的
	struct sockaddr_in  peeraddr; 
	socklen_t peerlen = sizeof(peeraddr);
	
	
	while(1){
		//int accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);
		
		unsigned conn;
		conn  = accept(sockfd, (struct sockaddr *)&peeraddr, &peerlen);
		
		if( conn < 0){
			perror("accept err\n");
			exit(0);
		}
		
		pid_t pid = fork();
		if(pid == 0){
			//子进程
			close(sockfd);
			char recvbuf[1024] = {0};
			
			while(1){
				//tcp        0      0 192.168.244.128:8001    0.0.0.0:*               LISTEN     
				//tcp        0      0 192.168.244.128:8001    192.168.244.128:48061   ESTABLISHED
				//tcp        0      0 192.168.244.128:48061   192.168.244.128:8001    ESTABLISHED
				//可以确定netstat 命令显示出来的都是主机序的端口
				printf("ip:%s port:%d\n",inet_ntoa(peeraddr.sin_addr), ntohs(peeraddr.sin_port) );
				int ret = read(conn, recvbuf, sizeof(recvbuf));	
				
				if(ret == 0){
					printf("对方已关闭\n");
					exit(0);
				}else if(ret < 0){
					perror("读数据失败\n"); 
					exit(0);
				}
				
				fputs(recvbuf, stdout); //服务器端收到数据，打印屏幕
				write(conn, recvbuf, ret); //服务器端回发信息
				memset(recvbuf, 0, sizeof(recvbuf));
			}	
			
		}else if(pid > 0){
			//父进程
			close(conn);
		}else if(pid < 0){
			perror("fork err\n");
			exit(0);
		}
		
	}
	
	close(sockfd);
	return 0;
}
