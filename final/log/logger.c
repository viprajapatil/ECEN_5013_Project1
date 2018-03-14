#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>
#include <time.h>
#include <errno.h>

typedef struct {
	float tempval;
	float t;
}temp_data;

#define SERVER_QUEUE_NAME   "/my_msg_queue_server"
#define QUEUE_PERMISSIONS 0666
#define MAX_MESSAGES 10
#define MAX_MSG_SIZE 256
#define MSG_BUFFER_SIZE MAX_MSG_SIZE + 10

int main()
{
    mqd_t logger;   // queue descriptors   
    struct mq_attr attr;
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
	
    fprintf(fptr,"Temp task received msg : %f, %f\n", ptr->tempval, ptr->t);

	mq_unlink(SERVER_QUEUE_NAME);
	
}
