#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <mqueue.h>
#include <errno.h>

enum{
Celsius,
Kelvin,
Fahrenheit
}Temp_unit;

enum{
	Success,
	Fail
}return_value;

#define Addr_temp_reg    0x00
#define Addr_config_reg  0x01
#define Addr_tlow_reg    0x02
#define Addr_thigh_reg 0x03

#define SERVER_QUEUE_NAME   "/my_msg_queue_server"
#define QUEUE_PERMISSIONS 0666
#define MAX_MESSAGES 10
#define MAX_MSG_SIZE 256
#define MSG_BUFFER_SIZE MAX_MSG_SIZE + 10

void write_pointer_reg(int fd, uint8_t reg);

uint16_t read_tlow_reg(int fd);

uint16_t read_thigh_reg(int fd);

float read_temp_reg(int fd, int unit);

int write_config_reg(int fd, uint16_t reg);

uint8_t read_config_reg(int fd);

int temp_init();

