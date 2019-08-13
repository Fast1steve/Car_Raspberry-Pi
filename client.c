#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <string.h>
#include <errno.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define SERVERIP "127.0.0.1"
#define SERVERPORT 12345
#define MAXBUFFER 256

int main(int argc, char** argv)  
{
	int begin,end=strlen(argv[3])-1;
	for(int i=0;i<strlen(argv[3]);++i)
	{
		if(argv[3][i]=='=')
		{
			begin=i+1;
			break;
		}
	}
	char* cmd=(char*)malloc(sizeof(char)*(end-begin+1));
	for(int i=0;i<=end-begin;++i)
		cmd[i]=argv[3][begin+i];
	int clientFd,ret;
	struct sockaddr_in serveraddr;
	clientFd=socket(AF_INET,SOCK_STREAM,0);//创建socket
	if(clientFd<0)
	{
		printf("socket error:%s\n",strerror(errno));
		exit(-1);
	}
	bzero(&serveraddr,sizeof(serveraddr));
	serveraddr.sin_family=AF_INET;
	serveraddr.sin_port=htons(SERVERPORT);
	inet_pton(AF_INET,SERVERIP,&serveraddr.sin_addr);
	ret=connect(clientFd,(struct sockaddr *)&serveraddr,sizeof(serveraddr));//连接到服务器
	if(ret!=0)
	{
		close(clientFd);
		printf("connect error:%s\n",strerror(errno));
		exit(-1);
	}
	write(clientFd,cmd,sizeof(char)*(end-begin+1));//写数据
	close(clientFd);
	free(cmd);
	return (EXIT_SUCCESS);
}