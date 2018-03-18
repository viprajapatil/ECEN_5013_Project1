/*
@file - light.h
@brief - Header file of temp.c which includes all the functions for writing and reading various APDS-9301 registers
@author - Nikhil Divekar & Vipraja Patil
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <unistd.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <math.h>

#define slave_address 	0x39
#define command_start 	0x80

#define control_reg 	0x00
#define timing_reg 	0x01
#define th_low_low 	0x02
#define th_low_high 	0x03
#define th_high_low 	0x04
#define th_high_high 	0x05
#define interrupt_reg	0x06
#define id_reg		0x0A
#define data0_low_reg	0x0C
#define data0_high_reg	0x0D
#define data1_low_reg	0x0E
#define data1_high_reg	0x0F

#define control_value 	0x03

#define i2c_name	"/dev/i2c-2"
#define file_name	"my_file"

/*
@brief - 	light_sensor_setup
			Initializing APDS-9301 sensor 
@param -	
@return - 	file descriptor
*/
uint8_t light_sensor_setup();

/*
@brief - 	write_command_register
			Write to command register
@param -	file descriptor
@return - 	value written to the register 
*/
uint8_t write_command_register(int fd);

/*
@brief - 	read_control_reg
			Read value from Control register
@param -	file descriptor
@return - 	value read from the register 
*/
uint8_t read_control_reg(int fd);

/*
@brief - 	write_control_register
			Write to control register
@param -	file descriptor
			value to be written in the register
@return - 	value written to the register 
*/
uint8_t write_control_reg(int fd, uint8_t write_val);

/*
@brief - 	read_timing_reg
			Read value from Timing register
@param -	file descriptor
@return - 	value read from the register 
*/
uint8_t read_timing_reg(int fd);

/*
@brief - 	write_timing_register
			Write to Timing register
@param -	file descriptor
			value to be written in the register
@return - 	value written to the register 
*/
uint8_t write_timing_reg(int fd, uint8_t write_val);

/*
@brief - 	read_int_control_reg
			Read value from Interrupt Control register
@param -	file descriptor
@return - 	value read from the register 
*/
uint8_t write_int_control_reg(int fd, uint8_t write_val);

/*
@brief - 	write_int_control_register
			Write to Interrupt control register
@param -	file descriptor
			value to be written in the register
@return - 	value written to the register 
*/
uint8_t read_int_control_reg(int fd);

/*
@brief - 	read_id_reg
			Read value from Identification register
@param -	file descriptor
@return - 	value read from the register 
*/
uint8_t read_id_reg(int fd);

/*
@brief - 	write_id_register
			Write to Identification register
@param -	file descriptor
			value to be written in the register
@return - 	value written to the register 
*/
uint8_t write_id_reg(int fd, uint8_t write_val);

/*
@brief - 	read_interrupt_threshold_reg
			Read value from Interrupt threshold register
@param -	file descriptor
@return - 	value read from the register 
*/
uint8_t * read_interrupt_threshold_reg(int fd, uint8_t * read_buffer);

/*
@brief - 	write_interrupt_threshold_register
			Write to Interrupt threshold register
@param -	file descriptor
			value to be written in the register
@return - 	value written to the register 
*/
uint8_t * write_interrupt_threshold_reg(int fd, uint8_t * write_buffer);

/*
@brief - 	read_data0_reg
			Read value from Data0 register
@param -	file descriptor
@return - 	value read from the register 
*/
uint16_t read_data0_reg(int fd);

/*
@brief - 	read_data1_reg
			Read value from Data0 register
@param -	file descriptor
@return - 	value read from the register 
*/
uint16_t read_data1_reg(int fd);

/*
@brief - 	get_lux_value
			Get lux values 
@param -	file descriptor
@return - 	lux value 
*/
float get_lux_value(int fd);
