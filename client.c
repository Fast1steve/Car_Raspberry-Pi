#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <string.h>
#include <errno.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define SERVERIP "123.207.173.74"
#define SERVERPORT 12345
#define MAXBUFFER 256

char forward[]="前";
char backward[]="后";
char left[]="左";
char right[]="右";
char num[][4]={{"一"},{"二"},{"三"},{"四"},{"五"},{"六"},{"七"},{"八"},{"九"}};
char cmd[3];
void analyze(char* str)
{
	for(int i=0;i<9;++i)
	{
		if(num[i][0]==str[6] && num[i][1]==str[7] && num[i][2]==str[8])
		{
			cmd[1]=(char)((i+1)+'0');
			break;
		}
	}
	if(str[0]==forward[0] && str[1]==forward[1] && str[2]==forward[2])
	{
		cmd[0]='F';
	}
	else if(str[0]==backward[0] && str[1]==backward[1] && str[2]==backward[2])
	{
		cmd[0]='B';
	}
	else if(str[0]==left[0] && str[1]==left[1] && str[2]==left[2])
	{
		cmd[0]='L';
	}
	else
	{
		cmd[0]='R';
	}
	cmd[2]='\0';
}
int main(int argc, char** argv)
{
	while(1)
	{
		int clientFd,ret;
		struct sockaddr_in serveraddr;
		char buf[MAXBUFFER];
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
		bzero(buf,sizeof(buf));
		read(clientFd,buf,sizeof(buf));//读数据
		printf("echo:%s\n",buf);
		analyze(buf+3);
		printf("%s\n",cmd);
		char str[80];
		sprintf(str,"python test.py %s",cmd);
		system(str);
		sleep(1);
		close(clientFd);
	}
	return (EXIT_SUCCESS);
}
