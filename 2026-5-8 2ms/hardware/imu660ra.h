#ifndef __IMU660RA_H
#define __IMU660RA_H

#include "bsp_system.h"

extern float gyro_z_avl,gyro_y_avl;
extern float angle,angle_y;

void tuoluoyi_init(void);
void get_gyro(void);
int8 drift_calculate(void);
void angle_clear(void);
void angle_get(void);

#endif
