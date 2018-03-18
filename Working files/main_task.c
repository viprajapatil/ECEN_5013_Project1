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
#include <stdbool.h>

#include "light_task.h"
#include "temp_task.h"
#include "logger_task.h"
#include "socket_task.h"

#define queue_name 		"/my_queue"
#define queue_size		8

#define SERVER_QUEUE_NAME   "/my_msg_queue_server"
#define QUEUE_PERMISSIONS 0666
#define MAX_MESSAGES 10
#define MAX_MSG_SIZE 256
#define MSG_BUFFER_SIZE MAX_MSG_SIZE + 10

int count = 0;

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
	int fd = light_sensor_setup();
	float lux;

	mqd_t logger;   // queue descriptors   
   	struct mq_attr attr;

	attr.mq_maxmsg = MAX_MESSAGES;
    	attr.mq_msgsize = sizeof(temp_data);
	temp_data temp;
	int command;
	int sock;

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
	command = Dead;
	if(send(sock, (void*)&command, sizeof(command), 0) < 0)
	{
		perror("Send failed");
		printf("Some thread dead, Terminating process\n");

		logger = mq_open (SERVER_QUEUE_NAME, O_RDWR | O_CREAT, QUEUE_PERMISSIONS, &attr);
		if (logger < 0) 
        	printf("ERROR opening message queue\n");
	
		temp.tempval = 0;
		temp.t = 4;
		temp.log_source_id = Main_task;
		char* buffptr = (char*)(&temp);
	
		if (mq_send (logger, buffptr, sizeof(temp_data), 0) == -1)
		printf("ERROR mq_send\n");
		exit(1);
	}
}

void * temp_thread_func()
{
	int fd;
	float value;
	fd = temp_init();
	mqd_t logger;   // queue descriptors   
   	struct mq_attr attr;
	
	float* buff;
	char * finalptr;
	temp_data temp;
	int command;
	int sock;
	
    	attr.mq_maxmsg = MAX_MESSAGES;
    	attr.mq_msgsize = sizeof(temp_data);

	while(1)
	{
	struct sockaddr_in check_server;
	char buff[1024];
	struct hostent *hp;
	
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if(sock < 0)
	{
		perror("socket failed");
		exit(1);
	}
	check_server.sin_family = AF_INET;
	hp = gethostbyname("localhost");
	if(hp == 0)
	{
		perror("gethost failed");
		exit(1);
	}
	memcpy(&check_server.sin_addr, hp->h_addr, hp->h_length);
	check_server.sin_port = htons(6006);
	
	if(connect(sock, (struct sockaddr *)&check_server, sizeof(check_server)) < 0)
	{
		perror("connection failes");
		exit(1);
	}

	command = Light;
	
	
	if(send(sock, (void*)&command, sizeof(command), 0) < 0)
	{
		perror("Send failed");
		exit(1);
	}
	close(sock);		
		
		value = read_temp_reg(fd,Kelvin);
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
	command = Dead;
	if(send(sock, (void*)&command, sizeof(command), 0) < 0)
	{
		perror("Send failed");
		printf("Some thread dead, Terminating process\n");

		logger = mq_open (SERVER_QUEUE_NAME, O_RDWR | O_CREAT, QUEUE_PERMISSIONS, &attr);
		if (logger < 0) 
        	printf("ERROR opening message queue\n");
	
		temp.tempval = 0;
		temp.t = 4;
		temp.log_source_id = Main_task;
		char* buffptr = (char*)(&temp);
	
		if (mq_send (logger, buffptr, sizeof(temp_data), 0) == -1)
		printf("ERROR mq_send\n");
		exit(1);
	}
}

void * logger_thread_func()
{	
	loggerFN();
	mqd_t logger;   // queue descriptors   
   	struct mq_attr attr;
	
	float* buff;
	char * finalptr;
	temp_data temp;
	int command;
	int sock;
	
    	attr.mq_maxmsg = MAX_MESSAGES;
    	attr.mq_msgsize = sizeof(temp_data);
	while(1)
	{
	struct sockaddr_in check_server;
	char buff[1024];
	struct hostent *hp;
	
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if(sock < 0)
	{
		perror("socket failed");
		exit(1);
	}
	check_server.sin_family = AF_INET;
	hp = gethostbyname("localhost");
	if(hp == 0)
	{
		perror("gethost failed");
		exit(1);
	}
	memcpy(&check_server.sin_addr, hp->h_addr, hp->h_length);
	check_server.sin_port = htons(6006);
	
	if(connect(sock, (struct sockaddr *)&check_server, sizeof(check_server)) < 0)
	{
		perror("connection failes");
		exit(1);
	}

	command = Logger;
	
	
	if(send(sock, (void*)&command, sizeof(command), 0) < 0)
	{
		perror("Send failed");
		exit(1);
	}
	close(sock);
	}
	command = Dead;
	if(send(sock, (void*)&command, sizeof(command), 0) < 0)
	{
		perror("Send failed");
		printf("Some thread dead, Terminating process\n");

		logger = mq_open (SERVER_QUEUE_NAME, O_RDWR | O_CREAT, QUEUE_PERMISSIONS, &attr);
		if (logger < 0) 
        	printf("ERROR opening message queue\n");
	
		temp.tempval = 0;
		temp.t = 4;
		temp.log_source_id = Main_task;
		char* buffptr = (char*)(&temp);
	
		if (mq_send (logger, buffptr, sizeof(temp_data), 0) == -1)
		printf("ERROR mq_send\n");
		exit(1);
	}
}

void * socket_thread_func()
{
	socket_server();
}

bool startup()
{
	int fd = temp_init();
	if (fd < 0)
	{
		printf("Temperature sensor intialization failed.\n");
		return false;
	}
	float value = read_temp_reg(fd,Celsius);
	if (value < -40 && value > 128)
	{
		printf("Temp out of range.\n");
		return false;
	}
	
	fd = light_sensor_setup();
	if (fd < 0)
	{
		printf("Light sensor intialization failed.\n");
		return false;
	}
	float lux = get_lux_value(fd);
	if (lux < -100 && lux > 1500)
	{
		printf("Lux value out of range.\n");
		return false;
	}
	if (!temp_task_life)
	{
		printf("Temperature thread creation failed. \n");
		return false;
	}
	if (!light_task_life)
	{
		printf("Light thread creation failed. \n");
		return false;
	}
	if (!logger_task_life)
	{
		printf("Logger thread creation failed. \n");
		return false;
	}
	if (!socket_task_life)
	{
		printf("Socket thread creation failed. \n");
		return false;
	}
	printf("Startup tests successful, proceeding with actual functionalities\n");
	return true;

}

void check_status()
{
	int sock;
	struct sockaddr_in check_server;
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
	
	check_server.sin_family = AF_INET;
	check_server.sin_addr.s_addr = INADDR_ANY;
	check_server.sin_port = htons(6006);
	
	//bind
	if(bind(sock, (struct sockaddr *)&check_server, sizeof(check_server)) < 0)
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

	if(incoming  == 2)
	{
		printf("Temp task Dead\n");
		break;
	}

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
	printf("Getting started with things, Main task initiated\n");

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
	
	printf("Initializing creation of thread processes\n");

	light_thread_check = pthread_create(&light_thread, NULL, light_thread_func, NULL);
	if(light_thread_check)
	{
		perror("Error creating light thread");
		exit(-1);
	}
	else
	{
		light_task_life = 1;
		printf("Light thread created\n");
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
		printf("Temp thread created\n");
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
		printf("Logger thread created\n");
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
		printf("Socket thread created\n");
		socket_task_life = 1;
		logger = mq_open (SERVER_QUEUE_NAME, O_RDWR | O_CREAT, QUEUE_PERMISSIONS, &attr);
		if (logger < 0) 
        		printf("ERROR opening message queue\n");
	
		temp.tempval = 0;
		temp.t = 5;
		temp.log_source_id = Main_task;
		char* buffptr = (char*)(&temp);
	
		if (mq_send (logger, buffptr, sizeof(temp_data), 0) == -1)
		printf("ERROR mq_send\n");
	}

	if(!startup())
		exit(-1);

	check_status();	

	pthread_join(light_thread, NULL);
	pthread_join(temp_thread, NULL);
	pthread_join(logger_thread, NULL);
	pthread_join(socket_thread, NULL);
}
