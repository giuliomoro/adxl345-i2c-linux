#ifndef ADXL345_H
#define ADXL345_H

int adxl345_init(void);
int adxl345_read_values(int *x, int *y, int *z);
int adxl345_finish(void);

#endif // ADXL345_H
