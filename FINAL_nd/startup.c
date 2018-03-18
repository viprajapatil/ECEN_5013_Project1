#include "startup.h"

bool startup()
{
	// temperatture intilization
	int fd = temp_init();
	if (fd < 0)
	{
		printf("Temperature sensor intialization failed.\n");
		return false;
	}
	float value = read_temp_reg(fd,Celsius);
	if (value < -40 && value > 128)
	{
		printf("Temp out of range.\n");
		return false;
	}
	
	// Light sensor task intialization
	fd = light_sensor_setup();
	if (fd < 0)
	{
		printf("Light sensor intialization failed.\n");
		return false;
	}
	float lux = get_lux_value(fd);
	if (lux < -100 && lux > 1500)
	{
		printf("Lux value out of range.\n");
		return false;
	}

	// Thread creation check
	/*if (!temp_task_life)
	{
		printf("Temperature thread creation failed. \n");
		return false;
	}
	if (!light_task_life)
	{
		printf("Light thread creation failed. \n");
		return false;
	}
	if (!logger_task_life)
	{
		printf("Logger thread creation failed. \n");
		return false;
	}
	if (!socket_task_life)
	{
		printf("Socket thread creation failed. \n");
		return false;
	}*/

	return true;
	 
}

