#include <cmocka.h>
#include <stdlib.h>
#include <stdio.h>
#include "temp_task.h"

void test_temp_init(void **state)
{
	int status = temp_init();	
	assert_not_equal(status, -1);
}

void test_config_reg(void **state)
{
	int fd = temp_init();
	int wpr1 = write_pointer_reg(fd, Addr_config_reg);
	int wpr = write_config_reg(fd, 4096)
	int status = read_pointer_reg(fd);	
	assert_int_equal(status, 45152);
}

void test_tlow_reg(void **state)
{
	int fd = temp_init();
	int a = 5;
	int wpr1 = write_pointer_reg(fd, Addr_tlow_reg);
	int wpr = write_tlow_reg(fd, a)
	int status = read_tlow_reg(fd);	
	assert_int_equal(status, wpr);
}

void test_thigh_reg(void **state)
{
	int fd = temp_init();
	int a = 5;
	int wpr1 = write_pointer_reg(fd, Addr_thigh_reg);
	int wpr = write_thigh_reg(fd, a)
	int status = read_thigh_reg(fd);	
	assert_int_equal(status, wpr);
}

void test_temp_c(void **state)
{
	int fd = temp_init();
	float temp = read_temp_reg(fd, Celsius);
	int status;
	if (temp < -55 && temp > 128)
		status = 1;
	else status = 0;	
	assert_int_equal(status, 0);
}

void test_temp_k(void **state)
{
	int fd = temp_init();
	float tempc = read_temp_reg(fd, Celsius);
	float tempk = read_temp_reg(fd, Kelvin);
	int status;
	if (tempc < -55 && tempc > 128)
		status = 1;
	else 
	{
		if(tempk == tempc+273.5)
			status = 0;
		else status = 1;
	}
	assert_int_equal(status, 0);
}

void test_temp_f(void **state)
{
	int fd = temp_init();
	float tempc = read_temp_reg(fd, Celsius);
	float tempf = read_temp_reg(fd, Fahrenheit);
	int status;
	if (tempc < -55 && tempc > 128)
		status = 1;
	else 
	{
		tempc = tempc * 1.8;
		tempc = tempc + 32;
		if(tempf == tempc)
			status = 0;
		else status = 1;
	}
	assert_int_equal(status, 0);
}

int main(int argc, char **argv)
{
  const struct UnitTest tests[] = 
  {	
	cmocka_unit_test(test_temp_init),
	cmocka_unit_test(test_config_reg),
	cmocka_unit_test(test_tlow_reg),
	cmocka_unit_test(test_thigh_reg),
	cmocka_unit_test(test_temp_c),
	cmocka_unit_test(test_temp_k),
	 cmocka_unit_test(test_temp_f),
  };

  return cmocka_run_group_tests(tests, NULL, NULL);
}
