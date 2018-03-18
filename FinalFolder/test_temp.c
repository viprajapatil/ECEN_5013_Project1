
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <stdlib.h>
#include <stdio.h>
#include "temp.h"

void test_temp_init(void **state)
{
	int status = temp_init();	
	assert_int_not_equal(status, -1);
}

void test_config_reg(void **state)
{
	int fd = temp_init();
	int wpr1 = write_pointer_reg(fd, Addr_config_reg);
	wpr1++;
	int wpr = write_config_reg(fd, 4096);
	wpr++;
	int status = read_config_reg(fd);	
	assert_int_equal(status, 45152);
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


int main(int argc, char **argv)
{
  const struct CMUnitTest tests[] = 
  {	
	cmocka_unit_test(test_temp_init),
	cmocka_unit_test(test_config_reg),
	cmocka_unit_test(test_temp_c)
  };

  return cmocka_run_group_tests(tests, NULL, NULL);
}
