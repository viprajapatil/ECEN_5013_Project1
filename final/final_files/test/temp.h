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

#define Addr_temp_reg    00
#define Addr_config_reg  01
#define Addr_tlow_reg    02
#define Addr_thigh_reg   03

#define Config_default   0xA060
#define Tlow_default     75
#define Thigh_default    80


#define SERVER_QUEUE_NAME   "/my_msg_queue_server"
#define QUEUE_PERMISSIONS 0666
#define MAX_MESSAGES 10
#define MAX_MSG_SIZE 256
#define MSG_BUFFER_SIZE MAX_MSG_SIZE + 10

uint8_t write_pointer_reg(int fd, uint8_t reg);

uint8_t read_pointer_reg(int fd);

uint16_t read_tlow_reg(int fd);

uint16_t write_tlow_reg(int fd, uint16_t reg);

uint16_t read_thigh_reg(int fd);

uint16_t write_thigh_reg(int fd, uint16_t reg);

float read_temp_reg(int fd, int unit);

uint16_t write_config_reg(int fd, uint16_t reg);
//uint16_t write_configreg(int fd, uint16_t config_val);

uint16_t read_config_reg(int fd);
//uint16_t read_configreg(int fd, uint16_t *res);
int temp_init();
