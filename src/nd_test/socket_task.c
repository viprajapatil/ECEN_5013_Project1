#include "socket_task.h"

void socket_server()
{
	int sock;
	struct sockaddr_in server, client;
	int mysock;
	char buff[1024];
	int rval;

	//create socket
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if(sock < 0)
	{
		perror("Failed to create a socket");
		exit(1);
	}
	
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons(6005);
	
	//bind
	if(bind(sock, (struct sockaddr *)&server, sizeof(server)) < 0)
	{
		perror("Didn't bind");
		exit(1);

	}

	//Listen
	if(listen(sock, 5) < 0)
	{
		perror("Listening error");
		exit(1);
	}

	//Accept
	while(1)
	{
	mysock = accept(sock, (struct sockaddr *)0, 0);
	if(mysock == -1)
	{
		perror("Accept failed");
		exit(1);
	}
	int incoming;
	int data_in = read(mysock,&incoming,sizeof(incoming));
     
	if (data_in < 0)
	{ 
		perror("Error reading");
		exit(1);
	}
        printf("Message: %d \n", incoming);

	if(incoming == 0)
	{
		//call get_light_task
		printf("Light task called \n");
	}
	else if(incoming == 1)
	{
		//call get temp task
		printf("temp task called \n");
	}
	}
	exit(1);
}
