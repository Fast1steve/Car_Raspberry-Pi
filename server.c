#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <string.h>
#include <errno.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define SERVERIP "0.0.0.0"
#define SERVERPORT 12345
#define MAXBUFFER 256

int main(int argc, char** argv)
{
	int serverFd,connfd,localfd,ret;
	socklen_t len;
	struct sockaddr_in serveraddr,clientaddr,localaddr;
	char readBuf[MAXBUFFER]={0};
	char ip[40]={0};
	serverFd=socket(AF_INET,SOCK_STREAM,0);//����socket
	if(serverFd < 0)
	{
		printf("socket error:%s\n",strerror(errno));
		exit(-1);
	}
	bzero(&serveraddr,sizeof(serveraddr));
	serveraddr.sin_family=AF_INET;
	serveraddr.sin_port=htons(SERVERPORT);
	inet_pton(AF_INET,SERVERIP,&serveraddr.sin_addr); //��c�����ֽ���ת��Ϊ�����ֽ���
	ret=bind(serverFd,(struct sockaddr *)&serveraddr,sizeof(serveraddr));//��IP�Ͷ˿�
	if(ret!=0)
	{
		close(serverFd);
		printf("bind error:%s\n",strerror(errno));
		exit(-1);
	}
	ret=listen(serverFd,5);//����
	if(ret!=0)
	{
		close(serverFd);
		printf("listen error:%s\n",strerror(errno));
		exit(-1);
	}
	len=sizeof(clientaddr);
	bzero(&clientaddr,sizeof(clientaddr));
	bzero(&localaddr,sizeof(localaddr));
	while(1)
	{
		connfd=accept(serverFd,(struct sockaddr *)&clientaddr,&len);//���ܿͻ��˵�����
		localfd=accept(serverFd,(struct sockaddr *)&localaddr,&len);//���ܱ��ص�����
		if(serverFd<0)
		{
			printf("accept error : %s\n", strerror(errno));
			continue;
		}
		while((ret=read(localfd,readBuf,MAXBUFFER)))//���ͻ��˷��͵�����
		{
			write(connfd,readBuf,MAXBUFFER);//д�ؿͻ���
			bzero(readBuf,MAXBUFFER);
		}
		if(ret==0)
		{
			printf("�ͻ��˹ر�����\n");
		}
		else
		{
			printf("read error:%s\n",strerror(errno));
		}
		close(localfd);
		close(connfd);
	}
	close(serverFd);
	return 0;
}
