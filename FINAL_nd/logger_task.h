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

#define SERVER_QUEUE_NAME   "/my_msg_queue_server"
#define QUEUE_PERMISSIONS 0666
#define MAX_MESSAGES 10
#define MAX_MSG_SIZE 256
#define MSG_BUFFER_SIZE MAX_MSG_SIZE + 10

typedef struct {
	float tempval;
	float t;
	int log_source_id;
}temp_data;

/*typedef enum
{
	Info,
	Data,
	Error
}level_id;*/

typedef enum
{
    Light = 0,
    Temp = 1,
    Dead = 2,
    Logger = 3,
    Socket = 4
}command_enum;

typedef enum
{
    Day = 0,
    Night = 1
}day_or_night;

typedef enum
{
    Light_task = 0,
    Temp_task = 1,
    Main_task = 2,
    Socket_task = 3
}logger_source_id;

void loggerFN();
