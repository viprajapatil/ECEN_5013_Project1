#include "logger_task.h"

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
    
	 
	logger = mq_open (SERVER_QUEUE_NAME, O_RDWR | O_CREAT, QUEUE_PERMISSIONS, &attr);
	if (logger < 0) 
        printf("ERROR opening message queue\n");
	
	if (mq_receive (logger, buff, sizeof(temp_data), NULL) < 0)
			printf("ERROR mq_receive\n");
	else printf ("Temp task message received.\n");
	
	temp_data* ptr;
	ptr = (temp_data*)(buff);
	
    	fprintf(fptr,"Temp task received msg : %f, %f, Log Source: %d\n", ptr->tempval, ptr->t, ptr->log_source_id);
	printf("In logger received: %f\n", ptr -> tempval); 

	fclose(fptr);

	//mq_unlink(SERVER_QUEUE_NAME);
	}
}
