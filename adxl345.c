#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>

#include "adxl345.h"

#define DEVICES "/dev/i2c-0"

#define I2C_ADDR  0x1d
#define I2C_WRITE 0x3a
#define I2C_READ  0x3b

#define BW_RATE     0x2C
#define POWER_CTL   0x2d
#define INT_SOURCE  0x30
#define DATA_FORMAT 0x31
#define DATAX0      0x32

#define DATA_READY 0x80

static int fd;

static int adxl345_open(void)
{
  if ((fd = open(DEVICES, O_RDWR)) < 0) {
    printf("Faild to open i2c port\n");
    exit(-1);
    return 0;
  }

  if (ioctl(fd, I2C_SLAVE, I2C_ADDR) < 0) {
    printf("Unable to get bus access to talk to slave\n");
    return 0;
  }

  return fd;
}

static int adxl345_write(unsigned char addr, unsigned char data)
{
  unsigned char buf[2];

  buf[0] = addr;
  buf[1] = data;

  if (write(fd, buf, 2) != 2) {
    fprintf(stderr, "Error writing to i2c slave\n");
    return 1;
  }

  return 0;
}

static int adxl345_read(unsigned char addr, int num_byte, unsigned char *data)
{
  int i;

  for (i = 0; i < num_byte; ++i, ++addr) {
    /* write address */
    if (write(fd, &addr, 1) != 1) {
      fprintf(stderr, "Error writing to i2c slave\n");
      return 1;
    }

    /* read data */
    if (read(fd, &data[i], 1) != 1) {
      fprintf(stderr, "Error reading from i2c slave\n");
      return 1;
    }
  }

  return 0;
}

int adxl345_init(void)
{
  adxl345_open();
  adxl345_write(POWER_CTL, 0x08); // 測定モード
  adxl345_write(DATA_FORMAT, 0x03); // 16g
  adxl345_write(BW_RATE, 0x07); // 12.5Hz

  return 0;
}

int adxl345_read_values(int *x, int *y, int *z)
{
  unsigned char values[6];

  for (values[0] = 0; !(values[0] & DATA_READY); ) {
    adxl345_read(INT_SOURCE, 1, values);
  }

  adxl345_read(DATAX0, 6, values);

  *x  = ((int) values[1] << 8) | (int) values[0];
  *y  = ((int) values[3] << 8) | (int) values[2];
  *z  = ((int) values[5] << 8) | (int) values[4];

  return 0;
}

int adxl345_finish(void)
{
  close(fd);

  return 0;
}
