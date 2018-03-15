// light task
#include "light_task.h"

uint8_t light_sensor_setup()
{
	char filename[20] = "/dev/i2c-2";
	int file;
	file = open(filename, O_RDWR);
	if (file < 0) 
	{
		perror("Error in file opening");
		exit(-1);
	}
	int slave_addr = 0x39;

	if (ioctl(file, I2C_SLAVE, slave_addr) < 0) 
	{
		perror("Ioctl error");
		exit(-1);
	}
	return file;	
}

uint8_t write_command_register(int fd)
{
	uint8_t data;
	data = command_start;
	if(write(fd, &data, 1) < 0)
	{
		perror("Write error");
		exit(-1);
	}
}
	
uint8_t read_control_reg(int fd)
{
	uint8_t data;
	data = command_start | control_reg;
	if(write(fd, &data, 1) < 0)
	{
		perror("Write error");
		exit(-1);
	}
	if(read(fd, &data, 1) < 0)
	{
		perror("Read error");
		exit(-1);
	}
	return data;
}

uint8_t write_control_reg(int fd, uint8_t write_val)
{
	uint8_t data;
	data = command_start | control_reg;
	if(write(fd, &data, 1) < 0)
	{
		perror("Write error");
		exit(-1);
	}
	data = write_val;
	if(write(fd, &data, 1) < 0)
	{
		perror("Read error");
		exit(-1);
	}
	return data;
}

uint8_t read_timing_reg(int fd)
{
	uint8_t data;
	data = command_start | timing_reg;
	if(write(fd, &data, 1) < 0)
	{
		perror("Write error");
		exit(-1);
	}
	if(read(fd, &data, 1) < 0)
	{
		perror("Read error");
		exit(-1);
	}
	return data;
}

uint8_t write_timing_reg(int fd, uint8_t write_val)
{
	uint8_t data;
	data = command_start | timing_reg;
	if(write(fd, &data, 1) < 0)
	{
		perror("Write error");
		exit(-1);
	}
	data = write_val;
	if(write(fd, &data, 1) < 0)
	{
		perror("Read error");
		exit(-1);
	}
	return data;
}

uint8_t write_int_control_reg(int fd, uint8_t write_val)
{
	uint8_t data;
	data = command_start | interrupt_reg;
	if(write(fd, &data, 1) < 0)
	{
		perror("Write error");
		exit(-1);
	}
	data = write_val;
	if(read(fd, &data, 1) < 0)
	{
		perror("Read error");
		exit(-1);
	}
	return data;
}

uint8_t read_id_reg(int fd)
{
	uint8_t data;
	data = command_start | id_reg;
	if(write(fd, &data, 1) < 0)
	{
		perror("Write error");
		exit(-1);
	}
	if(read(fd, &data, 1) < 0)
	{
		perror("Read error");
		exit(-1);
	}
	return data;
}

uint8_t * read_interrupt_threshold_reg(int fd, uint8_t * read_buffer)
{
	uint8_t data;
	data = command_start | th_low_low;
	if(write(fd, &data, 1) < 0)
	{
		perror("Write error");
		exit(-1);
	}
	if(read(fd, &data, 1) < 0)
	{
		perror("Read error");
		exit(-1);
	}
	read_buffer[0] = data;

	data = command_start | th_low_high;
	if(write(fd, &data, 1) < 0)
	{
		perror("Write error");
		exit(-1);
	}
	if(read(fd, &data, 1) < 0)
	{
		perror("Read error");
		exit(-1);
	}
	read_buffer[1] = data;
	
	data = command_start | th_high_low;
	if(write(fd, &data, 1) < 0)
	{
		perror("Write error");
		exit(-1);
	}
	if(read(fd, &data, 1) < 0)
	{
		perror("Read error");
		exit(-1);
	}
	read_buffer[2] = data;

	data = command_start | th_high_high;
	if(write(fd, &data, 1) < 0)
	{
		perror("Write error");
		exit(-1);
	}
	if(read(fd, &data, 1) < 0)
	{
		perror("Read error");
		exit(-1);
	}
	read_buffer[3] = data;

	return read_buffer;
}

uint8_t * write_interrupt_threshold_reg(int fd, uint8_t * write_buffer)
{
	uint8_t data;
	data = command_start | th_low_low;
	if(write(fd, &data, 1) < 0)
	{
		perror("Write error");
		exit(-1);
	}
	data = write_buffer[0];
	if(write(fd, &data, 1) < 0)
	{
		perror("Read error");
		exit(-1);
	}

	data = command_start | th_low_high;
	if(write(fd, &data, 1) < 0)
	{
		perror("Write error");
		exit(-1);
	}
	data = write_buffer[1];
	if(write(fd, &data, 1) < 0)
	{
		perror("Read error");
		exit(-1);
	}
	
	data = command_start | th_high_low;
	if(write(fd, &data, 1) < 0)
	{
		perror("Write error");
		exit(-1);
	}
	data = write_buffer[2];
	if(write(fd, &data, 1) < 0)
	{
		perror("Read error");
		exit(-1);
	}

	data = command_start | th_high_high;
	if(write(fd, &data, 1) < 0)
	{
		perror("Write error");
		exit(-1);
	}
	data = write_buffer[3];
	if(write(fd, &data, 1) < 0)
	{
		perror("Read error");
		exit(-1);
	}
	return write_buffer;
}

uint16_t read_data0_reg(int fd)
{
	uint16_t data_lsb, data_msb, final_data0;
	uint8_t addr;
	addr = command_start | data0_low_reg;
	if(write(fd, &addr, 1) < 0)
	{
		perror("Write error");
		exit(-1);
	}
	if(read(fd, &data_lsb, 1) < 0)
	{
		perror("Read error");
		exit(-1);
	}

	addr = command_start | data0_high_reg;
	if(write(fd, &addr, 1) < 0)
	{
		perror("Write error");
		exit(-1);
	}
	if(read(fd, &data_msb, 1) < 0)
	{
		perror("Read error");
		exit(-1);
	}
	data_msb = data_msb << 8;
	
	final_data0 = data_msb | data_lsb;

	return final_data0;
}

uint16_t read_data1_reg(int fd)
{
	uint16_t data_lsb, data_msb, final_data1;
	uint8_t addr;
	addr = command_start | data1_low_reg;
	if(write(fd, &addr, 1) < 0)
	{
		perror("Write error");
		exit(-1);
	}
	if(read(fd, &data_lsb, 1) < 0)
	{
		perror("Read error");
		exit(-1);
	}

	addr = command_start | data1_high_reg;
	if(write(fd, &addr, 1) < 0)
	{
		perror("Write error");
		exit(-1);
	}
	if(read(fd, &data_msb, 1) < 0)
	{
		perror("Read error");
		exit(-1);
	}
	data_msb = data_msb << 8;
	
	final_data1 = data_msb | data_lsb;

	return final_data1;
}

float get_lux_value(int fd)
{
	float lux_value = 0;
	uint8_t check;
	check = light_sensor_setup();
	if(check < 0)
	{
		perror("Sensor not set up");
	}
	write_control_reg(fd, 0x03);
	write_timing_reg(fd, 0x12);
	usleep(500);
	
	float data0_op, data1_op;
	data0_op = (float)read_data0_reg(fd);
	data1_op = (float)read_data1_reg(fd);
	
	if(data0_op == 0)
	{
		perror("Error divide by zero");
		exit(-1);
	}
	float adc_count = data1_op/data0_op;
	
	if(0 < adc_count < 0.50)
	{
		lux_value = (0.0304 * data0_op) - (0.062 * data0_op * powf(adc_count, 1.4));
	}
	else if(0.50 < adc_count < 0.61)
	{
		lux_value = (0.0224 * data0_op) - (0.031 * data1_op);
	}
	else if(0.61 < adc_count < 0.80)
	{
		lux_value = (0.0128 * data0_op) - (0.0153 * data1_op);
	}
	else if(0.80 < adc_count < 1.30)
	{
		lux_value = (0.00146 * data0_op) - (0.00112 * data1_op);
	}
	else
	{
		lux_value = 0;
	}
	return lux_value;
}


/*void main()
{
	//float data;
	int fd = light_sensor_setup();
	//write_control_reg(fd, 0x1);
	/*uint8_t data;
	data = command_start | control_reg;
	if(write(fd, &data, 1) < 0)
	{
		perror("Write error");
		exit(-1);
	}
	data = 5;
	if(write(fd, &data, 1) < 0)
	{
		perror("Write error");
		exit(-1);
	}*/	
	//data = read_control_reg(fd);

	//uint8_t data;
	/*data = command_start | control_reg;
	if(write(fd, &data, 1) < 0)
	{
		perror("Write error");
		exit(-1);
	}
	if(read(fd, &data, 1) < 0)
	{
		perror("Read error");
		exit(-1);
	}*/
	//get_lux_value(fd);
	/*float lux;
	lux = get_lux_value(fd);
	printf("%f", lux);
	/*uint8_t value = 4;
	value = write_timing_reg(fd, value);
	value = read_timing_reg(fd);
	printf("%d", value);*/
	
//}
	
	


