#include <stdio.h>
#include "adxl345.h"

int main(void)
{
  int x, y, z;

  adxl345_init();
  while (1) {
    adxl345_read_values(&x, &y, &z);
    printf("x:%d y:%d z:%d\n", x, y, z);
  }

  adxl345_finish();

  return 0;
}
