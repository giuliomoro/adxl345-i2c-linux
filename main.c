#include <stdio.h>
#include "adxl345.h"

int main(void)
{
  three_d_space *acceleration;
  
  if(adxl345_default_init())
    return 1;
  while (1) {    
    acceleration = adxl345_get_acceleration();
    if(!acceleration)
      break;
    printf("x:%d y:%d z:%d\n", acceleration->x, acceleration->y, acceleration->z);
  }
  
  adxl345_finish();

  return 0;
}
