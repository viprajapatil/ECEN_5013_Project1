#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>

typedef enum
{
    Light = 0,
    Temp = 1
}command_enum;

void main(int argc, char *argv[])
{
	//while(1)
	{
	int sock;
	struct sockaddr_in server;
	char buff[1024];
	struct hostent *hp;
	int command;
	
	printf("List of commands available:\n");
	printf("0. Get lumen output\n");
	printf("1. Get temp output\n");
	printf("Enter approprate command number\n");

	scanf("%d", &command);
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if(sock < 0)
	{
		perror("socket failed");
		exit(1);
	}
	server.sin_family = AF_INET;
	hp = gethostbyname(argv[1]);
	if(hp == 0)
	{
		perror("gethost failed");
		exit(1);
	}
	memcpy(&server.sin_addr, hp->h_addr, hp->h_length);
	server.sin_port = htons(6000);
	
	if(connect(sock, (struct sockaddr *)&server, sizeof(server)) < 0)
	{
		perror("connection failes");
		exit(1);
	}

	//int command = Light;
	
	
	if(send(sock, (void*)&command, sizeof(command), 0) < 0)
	{
		perror("Send failed");
		exit(1);
	}
	printf("Sent : %d \n", command);
	//sleep(1);

	/*int data_in = read(sock,buff,sizeof(buff));
        if (data_in < 0)
	{	 
        	perror("Send failed");
		exit(1);
	}*/

	//close(sock);
	//exit(1);
	sleep(1);
	}
}
