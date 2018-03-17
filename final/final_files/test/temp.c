#include "temp.h"

uint8_t write_pointer_reg(int fd, uint8_t reg)
{
	int ret = write(fd, &reg, sizeof(reg));
	if (ret < 0)
	{
		perror("Write to pointer reg failed");
	}
	return reg; 	
}

uint8_t read_pointer_reg(int fd)
{
	//uint8_t val;
	uint8_t buff = 0;
	write_pointer_reg(fd, Addr_tlow_reg);
	int ret =  read(fd,buff,sizeof(buff));
	if (ret < 0)
		perror("Read pointer reg failed")

	return buff;
}

uint16_t read_tlow_reg(int fd)
{
	uint16_t tlow_value;
	uint8_t buff[2] = {0};
	
	write_pointer_reg(fd, Addr_tlow_reg);
	int ret =  read(fd,buff,sizeof(buff));
	if (ret < 0)
		perror("Read tlow reg failed");
	msbval = buff[0];
	lsbval = buff[1];
	tlow_value = ((msbval << 8) | lsbval) >> 4;
	
	return tlow_value;
}

uint8_t write_tlow_reg(int fd, uint8_t reg)
{
	int ret = write(fd, &reg, sizeof(reg));
	if (ret < 0)
	{
		perror("Write to pointer reg failed");
	}
	return reg; 	
}

uint16_t read_thigh_reg(int fd)
{
	uint16_t thigh_value;
	uint8_t buff[1] = {0};
	write_pointer_reg(fd, Addr_thigh_reg);
	int ret =  read(fd,buff,sizeof(buff));
	if (ret < 0)
		perror("Read thigh reg failed");
	
	msbval = buff[0];
	lsbval = buff[1];
	thigh_value = ((msbval << 8) | lsbval) >> 4; 
	return thigh_value;
}

float read_temp_reg(int fd, int unit)
{
	
	int temp;
	float final_temp;
  	uint8_t msbval, lsbval;
	uint8_t buff[2];
	write_pointer_reg(fd, Addr_temp_reg);
	int ret = read(fd, &buff, sizeof(buff));
	if (ret != 2)
		perror("Read temp reg failed");
	
	//buff = buff>>4;
	msbval = buff[0];
	lsbval = buff[1];
	int msb = 80;
	int msb_bit = 0;
	/*if (msb & buff)
		msb_bit = 1;
	else msb_bit = 0;*/
	
	temp = ((msbval << 8) | lsbval) >> 4;
	printf("%d\n", temp);
	
	if (!msb_bit)
	{	final_temp = temp*0.0625;
		if (unit == Celsius)
		{
			return final_temp;
		}
		else if (unit == Kelvin)
		{
			final_temp = final_temp + 273.5;
			return final_temp;
		}
		else if (unit == Fahrenheit)
		{
			final_temp = (final_temp*1.8);
			final_temp = final_temp + 32;
			return final_temp;
		}
		else 
		{
			printf("Invalid unit of temperature");
			return Fail;
		}
	}
	else 
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
	}
	


}


uint16_t write_config_reg(int fd, uint16_t reg)
{
	write_pointer_reg(fd, Addr_config_reg);
	int ret = write(fd, &reg, sizeof(reg));
	if (ret < 0)
		perror("Write to config reg failed");
	return reg;
}
 
uint16_t read_config_reg(int fd)
{
	//uint16_t config_value;
	uint16_t buff = 0;
	
	write_pointer_reg(fd, Addr_config_reg);
	int ret =  read(fd,&buff,sizeof(buff));
	if (ret < 0)
		perror("Read config reg failed");

	return buff;
}

int temp_init()
{
	char i2c_name[20] = "/dev/i2c-2";
	int file;
	file = open(i2c_name, O_RDWR);
	if (file < 0) 
	{
		perror("Error in file opening");
		exit(-1);
	}
	int addr = 0x48;

	if (ioctl(file, I2C_SLAVE, addr) < 0) 
	{
		perror("Can't access the bus");
		exit(-1);
	}
	return file;	
}

//void main()
//{
	/*int fd;
	float value;
	fd = temp_init();
	value = read_temp_reg(fd,Celsius);
	printf("%f", value);*/

	/*mqd_t logger;   // queue descriptors   
    	struct mq_attr attr;
	
	float* buff;
	char * finalptr;
	
    	//attr.mq_flags = 0;
    	attr.mq_maxmsg = MAX_MESSAGES;
    	attr.mq_msgsize = sizeof(finalptr);
    	//attr.mq_curmsgs = 0;
	
	logger = mq_open (SERVER_QUEUE_NAME, O_RDWR | O_CREAT, QUEUE_PERMISSIONS, &attr);
	if (logger < 0) 
        printf("ERROR opening message queue\n");
	char a = 'b';
	char* ptr = &a;
	buff = &value;
	//finalptr = (char *)&value;
	if (mq_send (logger, ptr, sizeof(a), 0) == -1)
		printf("ERROR mq_send\n");
	else printf("Temp task message sent-> %c\n",*ptr);
	
	//mq_unlink(SERVER_QUEUE_NAME);*/
//}
