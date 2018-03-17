#include <cmocka.h>
#include <stdlib.h>
#include <stdio.h>
#include "temp.h"

void test_temp_init(void **state)
{
	int status = temp_init();	
	assert_not_equal(status, -1);
}

void test_pointer_reg(void **state)
{
	int fd = temp_init();
	int wpr = write_pointer_reg(fd, Addr_temp_reg);
	int status = read_pointer_reg(fd);	
	assert_int_equal(status, wpr);
}

void test_config_reg(void **state)
{
	int fd = temp_init();
	int a = 5;
	int wpr1 = write_pointer_reg(fd, Addr_config_reg);
	int wpr = write_config_reg(fd, a)
	int status = read_pointer_reg(fd);	
	assert_int_equal(status, wpr);
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

int main(int argc, char **argv)
{
  const struct UnitTest tests[] = 
  {	
	cmocka_unit_test(test_temp_init),
	cmocka_unit_test(test_pointer_reg),
	cmocka_unit_test(test_config_reg),
	cmocka_unit_test(test_tlow_reg),
	cmocka_unit_test(test_thigh_reg),
  };

  return cmocka_run_group_tests(tests, NULL, NULL);
}
