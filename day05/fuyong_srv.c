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
	srvaddr.sin_addr.s_addr = inet_addr("127.0.0.1"); //127.0.0.1
	
	
	//地址复用的设置，记得这样服务重启的时候就不会发生绑定失败的问题了
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
	
	//listen 只能是被动套接字了
	if ( listen(sockfd, SOMAXCONN)<0  )
	{
		perror("fun listen\n");
		exit(0);
	}
	
	struct sockaddr_in  peeraddr; //tcpip地址结构
	socklen_t peerlen = sizeof(peeraddr);
	
	unsigned int conn = 0;
	//accept 返回一个新的连接 ，这个新的连接是一个主动套接字
	conn = accept(sockfd,  (struct sockaddr *)&peeraddr, (socklen_t *)&peerlen);
	if (conn == -1)
	{
		perror("fun listen\n");
		exit(0);
	}
	
	printf("perradd:%s\n perrport:%d\n", inet_ntoa(peeraddr.sin_addr),  ntohs(peeraddr.sin_port));
	
	char revbuf[1024] = {0};
	while(1){
		int ret = read(conn, revbuf, sizeof(revbuf));
		if (ret == 0)
		{
			printf("对方已关闭\n");
			exit(0);
		}
		else if (ret < 0)
		{
			perror("读数据失败\n"); 
			exit(0);
		}
		
		fputs(revbuf, stdout); //服务器端收到数据，打印屏幕
		write(conn, revbuf, ret); //服务器端回发信息
		
	}
	
	close(conn);
	close(sockfd);
	
	return 0;
}