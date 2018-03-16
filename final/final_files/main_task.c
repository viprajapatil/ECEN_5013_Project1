#include <stdio.h>
#include <stdint.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <fcntl.h> 
#include <sys/stat.h>
#include <mqueue.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>

#include "light_task.h"
#include "temp_task.h"
#include "logger_task.h"

#define queue_name 		"/my_queue"
#define queue_size		8

#define SERVER_QUEUE_NAME   "/my_msg_queue_server"
#define QUEUE_PERMISSIONS 0666
#define MAX_MESSAGES 10
#define MAX_MSG_SIZE 256
#define MSG_BUFFER_SIZE MAX_MSG_SIZE + 10

int count = 0;

/*typedef struct {
	float tempval;
	float t;
	int log_source_id;
}temp_data;*/

/*typedef enum
{
    Light = 0,
    Temp = 1
}command_enum;*/

pthread_t light_thread;
pthread_t temp_thread;
pthread_t logger_thread;
pthread_t socket_thread;

int light_task_life = 0;
int temp_task_life = 0;
int logger_task_life = 0;
int socket_task_life = 0;

void * light_thread_func()
{
	//light_task_life = 1;
	//while(1)
	/*{
		int sock;
	struct sockaddr_in server;
	char buff[1024];
	struct hostent *hp;
	
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if(sock < 0)
	{
		perror("socket failed");
		exit(1);
	}
	server.sin_family = AF_INET;
	hp = gethostbyname("localhost");
	if(hp == 0)
	{
		perror("gethost failed");
		exit(1);
	}
	memcpy(&server.sin_addr, hp->h_addr, hp->h_length);
	server.sin_port = htons(6006);
	
	if(connect(sock, (struct sockaddr *)&server, sizeof(server)) < 0)
	{
		perror("connection failes");
		exit(1);
	}

	int command = Light;
	
	
	if(send(sock, (void*)&command, sizeof(command), 0) < 0)
	{
		perror("Send failed");
		exit(1);
	}
	//printf("Sent : %d \n", command);
	
	sleep(1);
	close(sock);
	}
	//light_task_life = 0;*/
	int fd = light_sensor_setup();
	float lux;

	mqd_t logger;   // queue descriptors   
   	struct mq_attr attr;

	attr.mq_maxmsg = MAX_MESSAGES;
    	attr.mq_msgsize = sizeof(temp_data);

	while(1)
	{
		int sock;
	struct sockaddr_in server;
	char buff[1024];
	struct hostent *hp;
	
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if(sock < 0)
	{
		perror("socket failed");
		exit(1);
	}
	server.sin_family = AF_INET;
	hp = gethostbyname("localhost");
	if(hp == 0)
	{
		perror("gethost failed");
		exit(1);
	}
	memcpy(&server.sin_addr, hp->h_addr, hp->h_length);
	server.sin_port = htons(6006);
	
	if(connect(sock, (struct sockaddr *)&server, sizeof(server)) < 0)
	{
		perror("connection failes");
		exit(1);
	}

	int command = Light;
	
	
	if(send(sock, (void*)&command, sizeof(command), 0) < 0)
	{
		perror("Send failed");
		exit(1);
	}
	//printf("Sent : %d \n", command);
	
	sleep(1);
	close(sock);
		lux = get_lux_value(fd);
		printf("LUX: %f", lux);
		temp_data temp;
	
		logger = mq_open (SERVER_QUEUE_NAME, O_RDWR | O_CREAT, QUEUE_PERMISSIONS, &attr);
		if (logger < 0) 
        	printf("ERROR opening message queue\n");
	
		temp.tempval = lux;
		temp.t = 8;
		temp.log_source_id = Light_task;
		char* buffptr = (char*)(&temp);
	
		if (mq_send (logger, buffptr, sizeof(temp_data), 0) == -1)
		printf("ERROR mq_send\n");

		//loggerFN();
		sleep(1);
	}
}

void * temp_thread_func()
{
	//while(1);
	//temp_task_life = 0;
	int fd;
	float value;
	fd = temp_init();
	mqd_t logger;   // queue descriptors   
   	struct mq_attr attr;
	
	float* buff;
	char * finalptr;
	temp_data temp;
	
    	attr.mq_maxmsg = MAX_MESSAGES;
    	attr.mq_msgsize = sizeof(temp_data);

	while(1)
	{
		int sock;
	struct sockaddr_in server;
	char buff[1024];
	struct hostent *hp;
	
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if(sock < 0)
	{
		perror("socket failed");
		exit(1);
	}
	server.sin_family = AF_INET;
	hp = gethostbyname("localhost");
	if(hp == 0)
	{
		perror("gethost failed");
		exit(1);
	}
	memcpy(&server.sin_addr, hp->h_addr, hp->h_length);
	server.sin_port = htons(6006);
	
	if(connect(sock, (struct sockaddr *)&server, sizeof(server)) < 0)
	{
		perror("connection failes");
		exit(1);
	}

	int command = Light;
	
	
	if(send(sock, (void*)&command, sizeof(command), 0) < 0)
	{
		perror("Send failed");
		exit(1);
	}
	//printf("Sent : %d \n", command);
	
	//sleep(1);
	close(sock);		
		
		value = read_temp_reg(fd,Celsius);
		printf("TEMP: %f", value);
		
		logger = mq_open (SERVER_QUEUE_NAME, O_RDWR | O_CREAT, QUEUE_PERMISSIONS, &attr);
		if (logger < 0) 
        	printf("ERROR opening message queue\n");
	
		temp.tempval = value;
		temp.t = 7;
		temp.log_source_id = Temp_task;
		char* buffptr = (char*)(&temp);
	
		if (mq_send (logger, buffptr, sizeof(temp_data), 0) == -1)
		printf("ERROR mq_send\n");
		sleep(1);

		//mq_unlink(SERVER_QUEUE_NAME);

	
	}
	temp_task_life = 1;
}

void * logger_thread_func()
{
	//logger_task_life = 0;
	//while(1);
	loggerFN();
}

void * socket_thread_func()
{
	//while(1);
	/*int sock;
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
	server.sin_port = htons(6003);
	
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
	exit(1);*/
	socket_server();
}

void check_status()
{
	int sock;
	struct sockaddr_in server, client;
	int mysock;
	char buff[1024];
	int rval;
	int flag = 0;
	//create socket
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if(sock < 0)
	{
		perror("Failed to create a socket");
		exit(1);
	}
	
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons(6006);
	
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

	if(incoming == 0)
	{
		//call get_light_task
		flag = 1;
		printf("Temp task alive \n");
	}
	if(incoming  != 0)
	{
		printf("Light task Dead\n");
		break;
	}

	/*if(temp_task_life == 1)
	{
		printf("Temp task Dead");
		break;
	}*/

	//count++;

	}
	exit(1);
}



void main()
{
	int light_thread_check;
	int temp_thread_check;
	int logger_thread_check;
	int socket_thread_check;
	int d;

	/*mqd_t myqueue;
	struct mq_attr myqueue_attr;
	myqueue_attr.mq_maxmsg = queue_size;
	myqueue_attr.mq_msgsize = sizeof(light_task_life);*/

	mqd_t logger;   // queue descriptors   
   	struct mq_attr attr;
	
	float* buff;
	char * finalptr;
	temp_data temp;
	
    	attr.mq_maxmsg = MAX_MESSAGES;
    	attr.mq_msgsize = sizeof(temp_data);

	logger = mq_open (SERVER_QUEUE_NAME, O_RDWR | O_CREAT, QUEUE_PERMISSIONS, &attr);
	if (logger < 0) 
        printf("ERROR opening message queue\n");
	
	temp.tempval = 0;
	temp.t = 0;
	temp.log_source_id = Main_task;
	char* buffptr = (char*)(&temp);
	
	if (mq_send (logger, buffptr, sizeof(temp_data), 0) == -1)
	printf("ERROR mq_send\n");

	light_thread_check = pthread_create(&light_thread, NULL, light_thread_func, NULL);
	if(light_thread_check)
	{
		perror("Error creating light thread");
		exit(-1);
	}
	else
	{
		light_task_life = 1;
		logger = mq_open (SERVER_QUEUE_NAME, O_RDWR | O_CREAT, QUEUE_PERMISSIONS, &attr);
		if (logger < 0) 
        		printf("ERROR opening message queue\n");
	
		temp.tempval = 0;
		temp.t = 1;
		temp.log_source_id = Main_task;
		char* buffptr = (char*)(&temp);
	
		if (mq_send (logger, buffptr, sizeof(temp_data), 0) == -1)
		printf("ERROR mq_send\n");
	}

	temp_thread_check = pthread_create(&temp_thread, NULL, temp_thread_func, NULL);
	if(temp_thread_check)
	{
		perror("Error creating temp thread");
		exit(-1);
	}
	else
	{
		temp_task_life = 1;
		logger = mq_open (SERVER_QUEUE_NAME, O_RDWR | O_CREAT, QUEUE_PERMISSIONS, &attr);
		if (logger < 0) 
        		printf("ERROR opening message queue\n");
	
		temp.tempval = 0;
		temp.t = 2;
		temp.log_source_id = Main_task;
		char* buffptr = (char*)(&temp);
	
		if (mq_send (logger, buffptr, sizeof(temp_data), 0) == -1)
		printf("ERROR mq_send\n");
	}

	logger_thread_check = pthread_create(&logger_thread, NULL, logger_thread_func, NULL);
	if(logger_thread_check)
	{
		perror("Error creating logger thread");
		exit(-1);
	}
	else
	{
		logger_task_life = 1;
		logger = mq_open (SERVER_QUEUE_NAME, O_RDWR | O_CREAT, QUEUE_PERMISSIONS, &attr);
		if (logger < 0) 
        		printf("ERROR opening message queue\n");
	
		temp.tempval = 0;
		temp.t = 3;
		temp.log_source_id = Main_task;
		char* buffptr = (char*)(&temp);
	
		if (mq_send (logger, buffptr, sizeof(temp_data), 0) == -1)
		printf("ERROR mq_send\n");
	}
	
	socket_thread_check = pthread_create(&socket_thread, NULL, socket_thread_func, NULL);
	if(socket_thread_check)
	{
		perror("Error creating socket thread");
		exit(-1);
	}
	else
	{
		socket_task_life = 1;
		logger = mq_open (SERVER_QUEUE_NAME, O_RDWR | O_CREAT, QUEUE_PERMISSIONS, &attr);
		if (logger < 0) 
        		printf("ERROR opening message queue\n");
	
		temp.tempval = 0;
		temp.t = 4;
		temp.log_source_id = Main_task;
		char* buffptr = (char*)(&temp);
	
		if (mq_send (logger, buffptr, sizeof(temp_data), 0) == -1)
		printf("ERROR mq_send\n");
	}

	/*myqueue = mq_open(queue_name, O_CREAT | O_RDWR, 0666, &myqueue_attr);
	if(myqueue == -1)
	{
		perror("Queue opening error");
		exit(1);
	}*/

	check_status();	

	//while(1)
	{
		/*if(light_task_life == 1)
			printf("Light thread alive \n");
		else
		{
			printf("Light thread dead \n");
			exit(-1);
		}
			
		if(temp_task_life == 1)
			printf("Temp thread alive \n");
		else
		{
			printf("Temp thread dead \n");
			exit(-1);
		}

		if(logger_task_life == 1)
			printf("Logger thread alive \n");
		else
		{
			printf("Logger thread dead \n");
			exit(-1);
		}

		if(socket_task_life == 1)
			printf("Socket thread alive \n");
		else
		{
			printf("Socket thread dead \n");
			exit(-1);
		}*/

		
		//sleep(1);
	}

	pthread_join(light_thread, NULL);
	pthread_join(temp_thread, NULL);
	pthread_join(logger_thread, NULL);
	pthread_join(socket_thread, NULL);
}
