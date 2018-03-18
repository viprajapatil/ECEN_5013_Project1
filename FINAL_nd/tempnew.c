#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<stdint.h>
#include<linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <math.h>
#include<complex.h>
#include<stdint.h>

/*Change to 1 to check library functions from main.c*/
#define CHECK_LIBRARY      0x00

/*Address on I2C device*/
#define TMP102_ADDRESS    0x48

/*Address to pointer register*/
#define POINTER_ADDRESS    0x00
/*Address of different registers
 * Or it with pointer registers
 */
#define TEMPREG_ADDRESS    0x00
#define    CONFREG_ADDRESS    0x01
#define    TLOWREG_ADDRESS    0x02
#define    THIGHREG_ADDRESS    0x03

/*Default configuration on register*/
#define    CONFIG_DEFAULT    0xA060

/*Different modes in config_register
 * Or these with CONFIG_DEFAULT
 */
#define SHUTDOWN_MODE    0x0001
#define    THERMOSTAT_MODE    0x0002
#define POLARITY        0x0004
#define ONESHOT_MODE    0x0080
#define    EXTND_MODE        0x1000

/*Different modes for temperature conversions
 */
#define CELSIUS            0x00
#define FAHRENHEIT        0x01
#define    KELVIN            0x02

/*Enum of returns*/
enum{
    SUCCESS = 0,
    FAIL = -1
} returns;


/**********************************************************************
*@Filename:tmp102.c
*
*@Description:This is a library for TMP102 sensor
*@Author:Sai Raghavendra Sankrantipati
*@Date:11/5/2017
*@compiler:arm-linux-gnueabihf-gcc
*@Usage : Connect TMP102 to I2C 2 and use any of the library function to read and write registers
 **********************************************************************/

/*Initialse sensor by opening dev/i2c-2*/
int tmp102_init(int bus){
    int file;
    char filename[20];

    snprintf(filename, 19, "/dev/i2c-%d", bus);
    file = open(filename, O_RDWR);
    if (file < 0) {
        perror("Unable to open file\n");
        return FAIL;
    }
    int addr = TMP102_ADDRESS; /* The I2C address */

    if (ioctl(file, I2C_SLAVE, addr) < 0) {
        perror("Unable to ioctl\n");
        return FAIL;
    }
    return file;
}



/*Write to pointer register
 * An call to registers are made
 * only through pointer register
 */
int write_pointerreg(int fd, uint8_t reg){
    uint8_t buf = POINTER_ADDRESS | reg;
    if( write(fd, &buf, 1) != 1){
        perror("Unable to write\n");
        return FAIL;
    }
    return SUCCESS;

}

/*Write to Config register
 *  Pass CONFIG_DEFAULT for default configuration and
 *  SHUTDOWN_MODE    0x0001
 *    THERMOSTAT_MODE    0x0002
 *     POLARITY        0x0004
 *     ONESHOT_MODE    0x0080
 *    EXTND_MODE        0x1000
 *    for different mode options
 */

uint16_t write_configreg(int fd, uint16_t config_val){
    write_pointerreg(fd, CONFREG_ADDRESS);

    uint8_t temp1 = (CONFIG_DEFAULT|config_val)>>8;
    uint8_t temp2 = (CONFIG_DEFAULT|config_val);
    uint8_t buf[3] = { POINTER_ADDRESS | CONFREG_ADDRESS, temp2, temp1};

    if( write(fd, buf, 3) != 3){
        perror("Unable to write\n");
        return FAIL;
    }

    return config_val;
}


/* Read config register. Use it for debugging*/
uint16_t read_configreg(int fd){

    write_pointerreg(fd, CONFREG_ADDRESS);
    uint16_t buf;
    if( read(fd, &buf, 2) != 2){
        perror("Unable to read\n");
        return FAIL;
    }
    printf("Config Register (TMP 102) : %d\n", buf);
    return buf;
}

/*This function fetches the 12/13 bit temperature from tmp102*/
int read_tempreg(int fd, uint16_t *res){
    write_pointerreg(fd, TEMPREG_ADDRESS);
    uint8_t buf[2];
    if( read(fd, buf, 2) != 2){
        perror("Unable to read\n");
        return FAIL;
    }

    int temp = (uint16_t)buf[0]<<4 | buf[0]>>4;
    *res = temp;
    return temp;
}


/* This converts temperature in different types and prints on consle
 * Pass 12/13bit adc register value and mode (12 or 13 bit mode)
 * 0.0625 is the resoluion of sensor. So multiply it to get celsius and
 * convert accordingly
 */
int convert_temp(int temp, int mode){

    if(mode == EXTND_MODE){

        if((temp & 0xA00) == 0){
            float celsius = temp * 0.0625;
            float fahrenheit = (1.8 * celsius) +32;
            float kelvin = celsius + 273.15;

            printf("Temperature in Celsius: %f  Fahrenheit:  %f Kelvin: %f\n", celsius, fahrenheit, kelvin);
            return SUCCESS;
        } else {
            temp = temp ^ 0xFFFF;
            float celsius = temp * (-0.0625);
            float fahrenheit = (1.8 * celsius) +32;
            float kelvin = celsius + 273.15;

            printf("Temperature in Celsius: %f  Fahrenheit:  %f Kelvin: %f\n", celsius, fahrenheit, kelvin);
            return SUCCESS;
        }
    } else if(mode == CONFIG_DEFAULT){
        if((temp & 0x800) == 0){
            float celsius = temp * 0.0625;
            float fahrenheit = (1.8 * celsius) +32;
            float kelvin = celsius + 273.15;

            printf("Temperature in Celsius: %f  Fahrenheit:  %f Kelvin: %f\n", celsius, fahrenheit, kelvin);
            return SUCCESS;
        } else {
            temp = temp ^ 0xFFFF;
            float celsius = temp * (-0.0625);
            float fahrenheit = (1.8 * celsius) +32;
            float kelvin = celsius + 273.15;

            printf("Temperature in Celsius: %f  Fahrenheit:  %f Kelvin: %f\n", celsius, fahrenheit, kelvin);
            return SUCCESS;
        }
    }
    return FAIL;
}

/*This calibrates temperature and returns in mode it's asked
 * Mode can be celsius or fahrenheit or kelvin
 * It returns a float
 */
float callibrate_temp(int temp, uint8_t mode){

    float celsius = temp * 0.0625;

    if(mode == CELSIUS)
        return celsius;
    else if(mode == FAHRENHEIT)
        return (1.8 * celsius) + 32;
    else if(mode == KELVIN)
        return celsius + 273.15;

    return FAIL;

}

/* This puts sensor in shutdown mode*/
int shutdown_mode(int fd, int mode){
    return write_configreg(fd, mode);
}

int change_resolution(int fd, int mode){
    return write_configreg(fd, mode);
}

/*It prints temperature*/
int print_temperature(int fd, int mode){
    if( write_configreg(fd, mode) == FAIL)
        return FAIL;
    uint16_t *temp = malloc(sizeof(uint16_t));

    read_tempreg(fd, temp);
    convert_temp(*temp, mode);
    return *temp;
}

/*Closes file descriptor opened in sensor_init*/
int close_tmp102(int fd){
    close(fd);
    return 1;
}


/* A function that calls all the registers and does reads and writes*/
int rw_allregs_tmp102(int fd){
    int status;
    uint16_t *res;
    res = malloc(sizeof(uint16_t));
    status = write_configreg(fd, CONFIG_DEFAULT);
    if( status ==  FAIL )
        return FAIL;
    status = read_configreg(fd);
    if( status ==  FAIL )
        return FAIL;
    status = read_tempreg(fd, res);
    if( status ==  FAIL )
        return FAIL    ;
    return SUCCESS;
}

void main()
{
	//while(1)
{
    int fd = tmp102_init(2);
    uint16_t v1 = write_configreg(fd, 0XE0);
    uint16_t v2 = read_configreg(fd);
    printf("Write %i\n",v1);
    printf("Read %i\n",v2);
	}
}
