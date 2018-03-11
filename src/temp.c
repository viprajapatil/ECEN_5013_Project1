#include "temp.h"

void write_pointer_reg(FILE *fd, uint8_t reg)
{
	int ret = write(fd, reg, sizeof(reg));
	if (ret < 0)
	{
		perror("Write to pointer reg failed");
	}	
}

uint16_t read_tlow_reg(FILE *fd)
{
	uint16_t tlow_value;
	uint8_t buff[1] = {0};
	
	write_pointer_reg(fd, Addr_tlow_reg);
	int ret =  read(fd,buff,sizeof(buff));
	if (ret < 0)
		perror("Read tlow reg failed");
	
	//shift?  tlow_value = 
	return tlow_value;
}

uint16_t read_thigh_reg(FILE *fd)
{
	uint16_t thigh_value;
	uint8_t buff[1] = {0};
	write_pointer_reg(fd, Addr_thigh_reg);
	int ret =  read(fd,buff,sizeof(buff));
	if (ret < 0)
		perror("Read thigh reg failed");
	
	//shift?  thigh_value = 
	return thigh_value;
}

float read_temp_reg(FILE *fd, int unit)
{
	float temp;
	uint16_t buff;
	write_pointer_reg(fd, Addr_temp_reg);
	int ret = read(fd, buff, sizeof(buff));
	if (ret < 0)
		perror("Read temp reg failed");
	
	buff = buff>>4;
	int msb_bit;
	if (msb & buff)
		msb_bit = 1;
	else msb_bit = 0;
	
	temp = buff;
	int msb = 80;
	
	if (!msb_bit)
	{	temp = temp*0.0625;
		if (unit == Celsius)
		{
			return temp;
		}
		else if (unit == Kelvin)
		{
			temp = temp + 273.5;
			return temp;
		}
		else if (unit == Fahrenheit)
		{
			temp = (temp*1.8);
			temp = temp + 32;
			return temp;
		}
		else 
		{
			printf("Invalid unit of temperature");
			return Fail;
		}
	}
	/*else 
	{
		
		if (unit == Celsius)
		{
			return temp;
		}
		else if (unit == Kelvin)
		{
			temp = temp + 273.5;
			return temp;
		}
		else if (unit == Fahrenheit)
		{
			temp = (temp*1.8);
			temp = temp + 32;
			return temp;
		}
		else 
		{
			printf("Invalid unit of temperature");
			return Fail;
		}
	}*/
	
	
}

int write_config_reg(FILE *fd, uint16_t reg)
{
	write_pointer_reg(fd, Addr_config_reg);
	int ret = write(fd, reg, sizeof(reg));
	if (ret < 0)
		perror("Write to config reg failed");

}
 
uint8_t read_config_reg(FILE *fd)
{
	uint16_t config_value;
	uint8_t buff[1] = {0};
	
	write_pointer_reg(fd, Addr_config_reg);
	int ret =  read(fd,buff,sizeof(buff));
	if (ret < 0)
		perror("Read config reg failed");
	
	//shift?  config_value = 
	return config_value;
}

void temp_init()
{
	int file;
	char filename[20];
	snprintf(filename, 19, "/dev/i2c-2");
	file = open(filename, O_RDWR);
	if (file < 0) 
	{
		perror("Error in file opening");
		exit(-1);
	}
	int addr = 0x40;

	if (ioctl(file, I2C_SLAVE, addr) < 0) 
	{
		perror("Ioctl error");
		exit(-1);
	}	
}






