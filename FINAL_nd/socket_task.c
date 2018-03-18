#include "socket_task.h"
#include "logger_task.h"
#include "temp_task.h"
#include "light_task.h"

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
	server.sin_port = htons(6000);
	
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
     	mqd_t logger;   // queue descriptors   
   	struct mq_attr attr;

	attr.mq_maxmsg = MAX_MESSAGES;
    	attr.mq_msgsize = sizeof(temp_data);
	temp_data temp;

	logger = mq_open (SERVER_QUEUE_NAME, O_RDWR | O_CREAT, QUEUE_PERMISSIONS, &attr);
		if (logger < 0) 
        		printf("ERROR opening message queue\n");
	
		temp.tempval = 0;
		//temp.t = 7;
		temp.log_source_id = Socket_task;
		temp.level = 0;
		char* buffptr = (char*)(&temp);
	
		if (mq_send (logger, buffptr, sizeof(temp_data), 0) == -1)
		printf("ERROR mq_send\n");

	logger = mq_open (SERVER_QUEUE_NAME, O_RDWR | O_CREAT, QUEUE_PERMISSIONS, &attr);
	if (logger < 0) 
        printf("ERROR opening message queue\n");
	
	
	if (data_in < 0)
	{ 
		perror("Error reading");
		exit(1);
	}
        printf("Message: %d \n", incoming);

	if(incoming == 4)
	{
		//call get_light_task
		printf("Light task called \n");
		int fd = light_sensor_setup();
		float lux = get_lux_value(fd);
		printf("LUX: %f", lux);
		temp_data temp;

		int data_out = send(mysock, (void*)&lux, sizeof(lux), 0);
		if (data_out < 0)
		{
			perror("Error writing");
			exit(1);
		}
		else
			printf("Data sent back");
	}
	else if(incoming == 1)
	{
		//call get temp task
		printf("temp task called \n");
		int fd = temp_init();
		float value = read_temp_reg(fd,Celsius);
		printf("TEMP: %f", value);

		int data_out = send(mysock, (void*)&value, sizeof(value), 0);
		if (data_out < 0)
		{
			perror("Error writing");
			exit(1);
		}
		printf("Data sent back");
	}
	else if(incoming == 2)
	{
		//call get temp task
		printf("temp task called \n");
		int fd = temp_init();
		float value = read_temp_reg(fd,Kelvin);
		printf("TEMP: %f", value);

		int data_out = send(mysock, (void*)&value, sizeof(value), 0);
		if (data_out < 0)
		{
			perror("Error writing");
			exit(1);
		}
		printf("Data sent back");
	}
	else if(incoming == 3)
	{
		//call get temp task
		printf("temp task called \n");
		int fd = temp_init();
		float value = read_temp_reg(fd,Fahrenheit);
		printf("TEMP: %f", value);

		int data_out = send(mysock, (void*)&value, sizeof(value), 0);
		if (data_out < 0)
		{
			perror("Error writing");
			exit(1);
		}
		printf("Data sent back");
	}
	else if(incoming == 5)
	{
		//call get_light_task
		printf("Light task called \n");
		int fd = light_sensor_setup();
		float lux = get_lux_value(fd);
		int DN = is_it_day_or_night(lux);
		//printf("LUX: %f", lux);
		temp_data temp;

		int data_out = send(mysock, (void*)&DN, sizeof(DN), 0);
		if (data_out < 0)
		{
			perror("Error writing");
			exit(1);
		}
		else
			printf("Data sent back");
	}
	}
	exit(1);
}
