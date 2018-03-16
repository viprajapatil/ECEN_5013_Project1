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

uint8_t light_sensor_setup();

uint8_t write_command_register(int fd);

uint8_t read_control_reg(int fd);

uint8_t write_control_reg(int fd, uint8_t write_val);

uint8_t read_timing_reg(int fd);

uint8_t write_timing_reg(int fd, uint8_t write_val);

uint8_t write_int_control_reg(int fd, uint8_t write_val);

uint8_t read_id_reg(int fd);

uint8_t * read_interrupt_threshold_reg(int fd, uint8_t * read_buffer);

uint8_t * write_interrupt_threshold_reg(int fd, uint8_t * write_buffer);

uint16_t read_data0_reg(int fd);

uint16_t read_data1_reg(int fd);

float get_lux_value(int fd);
