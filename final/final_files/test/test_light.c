
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <stdlib.h>
#include <stdio.h>
#include "light.h"


void test_light_init(void **state)
{
	int status = light_sensor_setup();	
	assert_int_not_equal(status, -1);
}

void test_control_reg(void **state)
{
	int fd = light_sensor_setup();
	uint8_t a = 5;
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

void test_int_control_reg(void **state)
{
	int fd = light_sensor_setup();
	uint8_t a = 5;
	uint8_t wpr = write_int_control_reg(fd, a);
	uint8_t status =read_int_control_reg(fd);	
	assert_int_equal(status, wpr);
}

/*void test_interrupt_threshold_reg(void **state)
{
	int fd = light_sensor_setup();
	uint8_t a = 5;
	uint8_t wpr = write_interrupt_threshold_reg(fd, &a);
	uint8_t status = read_interrupt_threshold_reg(fd, &a);	
	assert_int_equal(status, wpr);
}*/

void test_flux_value(void **state)
{
	int fd = light_sensor_setup();
	float lux = get_lux_value(fd);	
	int status;
	if (lux < -150 && lux> 1500)
	{	
		status = 1;
	}
	else status = 0;
	assert_int_not_equal(status, 1);
}

void test_id_reg(void **state)
{
	int fd = light_sensor_setup();
	int a = 7;
	uint8_t wpr = write_id_reg(fd, a);
	uint8_t status =read_id_reg(fd);	
	assert_int_equal(status, wpr);
}

int main(int argc, char **argv)
{
  const struct CMUnitTest tests[] = 
  {	
	cmocka_unit_test(test_light_init),
	cmocka_unit_test(test_control_reg),
	cmocka_unit_test(test_timing_reg),
	cmocka_unit_test(test_int_control_reg),
	//cmocka_unit_test(test_interrupt_threshold_reg),
	cmocka_unit_test(test_flux_value),
	cmocka_unit_test(test_id_reg),

  };

  return cmocka_run_group_tests(tests, NULL, NULL);
}
