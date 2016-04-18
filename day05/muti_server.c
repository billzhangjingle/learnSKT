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

int main(void)
{
	int sockfd = 0;
	sockfd =  socket(PF_INET, SOCK_STREAM, 0);
	if (sockfd == -1)
	{
		perror("fun socket\n");
		exit(0);
	}
	
	struct sockaddr_in  srvaddr;
	srvaddr.sin_family = AF_INET;
	srvaddr.sin_port = htons(8001);
	srvaddr.sin_addr.s_addr = inet_addr("192.168.244.128"); //127.0.0.1
	
	int optval = 1;
	if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) < 0)
	{
		perror("setsockopt bind\n");
		exit(0);
	}
	
	if ( bind( sockfd, (struct sockaddr *)&srvaddr, sizeof(srvaddr)) < 0)
	{
		perror("fun bind\n");
		exit(0);
	}
	
	if ( listen(sockfd, SOMAXCONN)<0  )
	{
		perror("fun listen\n");
		exit(0);
	}
	
		
	struct sockaddr_in  peeraddr; //tcpip地址结构
	socklen_t peerlen = sizeof(peeraddr);
	
	char revbuf[1024] = {0};
	
	while(1){
		unsigned int conn ;
		conn = accept(sockfd, (struct sockaddr *)&peeraddr, (socklen_t *)&peerlen );
		
		if( conn == -1){
			close(sockfd);
			perror("fun listen\n");
			exit(0);
		}
		
		int pid = fork();
		
		if( pid == 0){
			
			close(sockfd); //
			printf("perradd:%s\n perrport:%d\n", inet_ntoa(peeraddr.sin_addr),  ntohs(peeraddr.sin_port));
			
			int ret  = read( conn, revbuf, sizeof(revbuf) );
			
			if(ret == 0){
				printf("client close\n");
				exit(0);
			}else if(ret < 0){
				perror("read err");
				exit(0);	
			}
			
			fputs(revbuf, stdout);
			write(conn, revbuf, ret);
		
		}else if( pid > 0){
			close(conn);
			
		}else {
			close(sockfd);
			//如果父进程fork 失败话，这样直接退出是不是有点儿不好呢
			//之前的创建的子进程怎么办，不为他们收尸吗
			// 用信号来处理 还是怎么着
			exit(0);
		}
			
		
	}
	
	//这样写到底对不对呢？？
	//close(conn);
	close(sockfd);
	
	return 0;
}
