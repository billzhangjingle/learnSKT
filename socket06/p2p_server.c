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
#include <netinet/ip.h>

int main(void)
{
	int skdstor = 0;
	skdstor =  socket(AF_INET, SOCK_STREAM , 0);
	
	struct sockaddr_in addrin;
	addrin.sin_family = AF_INET;
	addrin.sin_port = htons(8001);//��һ�������͵ı����ֽ���ת�������ֽ���   ͨ��man�ɲ鵽�����Ϣ
	addrin.sin_addr.s_addr = inet_addr("127.0.0.1");
	//addrin.sin_addr.s_addr = inet_addr("192.168.244.128");
	
	
	int optval = 1;
	if (setsockopt(skdstor, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) < 0)
	{
		perror("setsockopt bind\n");
		exit(0);
	}
	
	//�󶨶˿�
	if(bind(skdstor, (const struct sockaddr *)&addrin,  sizeof(addrin)) == -1){
		perror("fun bind\n");
		exit(0);
	}
	
	if ( listen(skdstor, SOMAXCONN) <0  )
	{
		perror("fun listen\n");
		exit(0);
	}
	struct sockaddr_in  peeraddr;
	socklen_t peerlen = sizeof(peeraddr);
	
	unsigned int conn = 0;
	conn = accept(skdstor,  (struct sockaddr *)&peeraddr, (socklen_t *)&peerlen);
	if (conn == -1)
	{
		perror("fun listen\n");
		exit(0);
	}
	
	char revbuf[1024] = {0};
	
	pid_t pid;
	pid = fork();
	if( pid > 0){
		// ���տͻ�����Ϣ����ʾ���ն�
		char recvbuf[1024] = {0};
		while(1){
			//ʲôʱ���ϵsockstor �׽��֣� ����Ҫ������ӵ�ʱ��
			int ret = read(conn, recvbuf, sizeof(revbuf));
			if(ret == 0){
				printf("�Է��ѹر�");
				break;
			}else if(ret < 0){
				printf("��ȡʧ��");
				exit(0);
			}
			
			fputs(recvbuf, stdout);
			memset(recvbuf, 0, sizeof(recvbuf));
		}
		//û��д��� close(conn);
		
		close(conn);
		
		
	}else if(pid == 0){
		//���ն���������ݣ����͵��ͻ�����
		char sendbuf[1024] = {0};
		while( fgets(sendbuf, sizeof(sendbuf), stdin) != NULL){
			write(conn, sendbuf, strlen(sendbuf));
			memset(sendbuf, 0, sizeof(sendbuf));
		}
		
		close(conn);
		
	}else{
		//fork ʧ��
		close(skdstor);
		exit(0);
	}
	close(skdstor);
	return 0;
}
