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

#define queue_name 		"/my_queue"
#define queue_size		8

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
	{
	}
	//light_task_life = 0;
}

void * temp_thread_func()
{
	//while(1);
	//temp_task_life = 0;
}

void * logger_thread_func()
{
	//logger_task_life = 0;
	//while(1);
}

void * socket_thread_func()
{
	//while(1);
}


void main()
{
	int light_thread_check;
	int temp_thread_check;
	int logger_thread_check;
	int socket_thread_check;
	int d;

	mqd_t myqueue;
	struct mq_attr myqueue_attr;
	myqueue_attr.mq_maxmsg = queue_size;
	myqueue_attr.mq_msgsize = sizeof(light_task_life);

	light_thread_check = pthread_create(&light_thread, NULL, light_thread_func, NULL);
	if(light_thread_check)
	{
		perror("Error creating light thread");
		exit(-1);
	}
	else
	light_task_life = 1;

	temp_thread_check = pthread_create(&temp_thread, NULL, temp_thread_func, NULL);
	if(temp_thread_check)
	{
		perror("Error creating temp thread");
		exit(-1);
	}
	else
	temp_task_life = 1;

	logger_thread_check = pthread_create(&logger_thread, NULL, logger_thread_func, NULL);
	if(logger_thread_check)
	{
		perror("Error creating logger thread");
		exit(-1);
	}
	else
	logger_task_life = 1;
	
	socket_thread_check = pthread_create(&socket_thread, NULL, socket_thread_func, NULL);
	if(socket_thread_check)
	{
		perror("Error creating socket thread");
		exit(-1);
	}
	else
	socket_task_life = 1;

	myqueue = mq_open(queue_name, O_CREAT | O_RDWR, 0666, &myqueue_attr);
	if(myqueue == -1)
	{
		perror("Queue opening error");
		exit(1);
	}	

	while(1)
	{
		if(light_task_life == 1)
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
			printf("logger thread alive \n");
		else
		{
			printf("logger thread dead \n");
			exit(-1);
		}

		if(socket_task_life == 1)
			printf("Socket thread alive \n");
		else
		{
			printf("Socket thread dead \n");
			exit(-1);
		}

		sleep(1);
	}

	pthread_join(light_thread, NULL);
	pthread_join(temp_thread, NULL);
	pthread_join(logger_thread, NULL);
	pthread_join(socket_thread, NULL);
}
