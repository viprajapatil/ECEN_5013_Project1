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

enum{
Celsius,
Kelvin,
Fahrenheit
}Temp_unit;

enum{
	Success,
	Fail
}return_value

#define Addr_temp_reg    0x00
#define Addr_config_reg  0x01
#define Addr_tlow_reg    0x02
#define Addr_thigh_reg   0x03