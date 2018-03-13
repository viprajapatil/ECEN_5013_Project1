#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>
#include <errno.h>


#define SERVER_QUEUE_NAME   "/my_msg_queue_server"
#define QUEUE_PERMISSIONS 0666
#define MAX_MESSAGES 10
#define MAX_MSG_SIZE 256
#define MSG_BUFFER_SIZE MAX_MSG_SIZE + 10

int main()
{
    mqd_t logger;   // queue descriptors   
    struct mq_attr attr;
	
	/*message *ptr;
    message mesg_struct;
    ptr = &mesg_struct;
	 char buff[sizeof(message)] = {0};*/
	 
	
	
    //attr.mq_flags = 0;
    attr.mq_maxmsg = MAX_MESSAGES;
    attr.mq_msgsize = sizeof(float);
    //attr.mq_curmsgs = 0;
	float* buff = 0;
	
	logger = mq_open (SERVER_QUEUE_NAME, O_RDWR | O_CREAT, QUEUE_PERMISSIONS, &attr);
	if (logger < 0) 
        printf("ERROR opening message queue\n");
	
	if (mq_receive (logger, buff, sizeof(float), NULL) < 0)
		printf("ERROR mq_receive\n");
	else printf ("Temp task message received.\n");
	
    printf("Temp task received msg : %f\n", buff);

	mq_unlink(SERVER_QUEUE_NAME);
	
}
