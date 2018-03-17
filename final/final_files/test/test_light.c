#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <stdlib.h>
#include <stdio.h>
#include "light.h"


void test_light_init(void **state)
{
	int status = temp_init();	
	assert_int_not_equal(light_sensor_setup, -1);
}

void test_control_reg(void **state)
{
	int fd = light_sensor_setup();
	int a = 5;
	uint8_t wpr = write_control_reg(fd, a);
	uint8_t status =read_control_reg(fd);	
	assert_int_equal(status, wpr);
}

void test_timing_reg(void **state)
{
	int fd = light_sensor_setup();
	int a = 5;
	uint8_t wpr = write_timing_reg(fd, a);
	uint8_t status =read_timing_reg(fd);	
	assert_int_equal(status, wpr);
}


/*void test_int_control_reg(void **state)
{
	int fd = light_sensor_setup();
	int a = 5;
	int wpr = write_timing_reg(fd, a);
	int status =read_timing_reg(fd);	
	assert_int_equal(status, wpr);
}*/

int main(int argc, char **argv)
{
  const struct CMUnitTest tests[] = 
  {	
	cmocka_unit_test(test_light_init),
	cmocka_unit_test(test_control_reg),
	cmocka_unit_test(test_timing_reg),

  };

  return cmocka_run_group_tests(tests, NULL, NULL);
}


