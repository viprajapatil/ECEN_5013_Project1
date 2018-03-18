/*
@file - temp_task.h
@brief - Header file of temp.c which includes all the functions for writing and reading various TMP102 registers
@author - Nikhil Divekar & Vipraja Patil
*/

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

/*
@brief - 	write_pointer_reg
			Write to pointer register
@param -	file descriptor
			Address of the register value to be written
@return - 	value written to the register 
*/
uint8_t write_pointer_reg(int fd, uint8_t reg);

/*
@brief - 	write_thigh_reg
			Write to THIGH register
@param -	file descriptor
			data to be written in the register
@return - 	value written to the register 
*/
uint8_t read_pointer_reg(int fd);

/*
@brief - 	read_tlow_reg
			Read value from TLOW register
@param -	file descriptor
@return - 	value read from the register 
*/
uint16_t read_tlow_reg(int fd);

uint16_t write_tlow_reg(int fd, uint16_t reg);
/*
@brief - 	read_thigh_reg
			Read value from THIGH register
@param -	file descriptor
@return - 	value read from the register 
*/
uint16_t read_thigh_reg(int fd);
/*
@brief - 	write_thigh_reg
			Write to THIGH register
@param -	file descriptor
			data to be written in the register
@return - 	value written to the register 
*/
uint16_t write_thigh_reg(int fd, uint16_t reg);

/*
@brief - 	read_temp_reg
			Read temperature value from temperature register in Celsius, Kelvin or Fahrenheit
@param -	file descriptor
			Unit of temperature: Celsius, Kelvin or Fahrenheit
@return - 	Temperature value
*/
float read_temp_reg(int fd, int unit);

/*
@brief - 	write_config_reg
			Write to config register
@param -	file descriptor
			data to be written in the register
@return - 	value written to the register 
*/
uint16_t write_config_reg(int fd, uint16_t reg);

/*
@brief - 	read_config_reg
			Read value from config register
@param -	file descriptor
@return - 	Value read from the register
*/
uint16_t read_config_reg(int fd);

/*
@brief - 	temp_init
			Initializing TMP102 sensor 
@param -	
@return - 	file descriptor
*/
int temp_init();
