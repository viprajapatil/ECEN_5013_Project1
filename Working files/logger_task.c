#include "logger_task.h"

pthread_mutex_t pmutex;

void loggerFN()
{  
    	struct mq_attr attr;
	mqd_t logger;   // queue descriptors 
	

	while(1)
	{
	FILE *fptr = fopen("projectlog.log","a");

	time_t currenttime;
	time(&currenttime);
	char *timeStr = ctime(&currenttime);
	fprintf(fptr,"Time: %s",timeStr);
	
	//message *ptr;
    	temp_data temp;
	 char buff[sizeof(temp_data)] = {0};

    	attr.mq_maxmsg = MAX_MESSAGES;
   	attr.mq_msgsize = sizeof(temp_data);
    
	pthread_mutex_lock(&pmutex);
	logger = mq_open (SERVER_QUEUE_NAME, O_RDWR | O_CREAT, QUEUE_PERMISSIONS, &attr);
	if (logger < 0) 
        printf("ERROR opening message queue\n");
	
	if (mq_receive (logger, buff, sizeof(temp_data), NULL) < 0)
			printf("ERROR mq_receive\n");
	//else printf ("Temp task message received.\n");
	pthread_mutex_unlock(&pmutex);
	
	temp_data* ptr;
	ptr = (temp_data*)(buff);
	char message[1024];
	if(ptr -> log_source_id == 1)
		strcpy(message, "Temperature recorded");
	if(ptr -> log_source_id == 0)
		strcpy(message, "Light lux recorded");
	if(ptr -> log_source_id == 3)
		strcpy(message, "External service requested data");
	if(ptr -> log_source_id == 2 && ptr -> t == 0)
		strcpy(message, "Initializing main task");
	if(ptr -> log_source_id == 2 && ptr -> t == 1)
		strcpy(message, "Light thread created");
	if(ptr -> log_source_id == 2 && ptr -> t == 2)
		strcpy(message, "Temp thread created");
	if(ptr -> log_source_id == 2 && ptr -> t == 3)
		strcpy(message, "Logger thread created");
	if(ptr -> log_source_id == 2 && ptr -> t == 5)
		strcpy(message, "Socket thread created");
	if(ptr -> log_source_id == 2 && ptr -> t == 4)
		strcpy(message, "Terminating process");
	
    	fprintf(fptr,"Data: %f, Message: %s, Log Source: %d\n", ptr->tempval, message, ptr->log_source_id);
	//printf("In logger received: %f", ptr -> tempval); 

	fclose(fptr);

	//mq_unlink(SERVER_QUEUE_NAME);
	}
}
