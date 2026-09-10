#include "imu660ra.h"
#include "zf_device_imu660ra.h"

float gyro_z_pv,gyro_z_pv_last,gyro_y_pv,gyro_y_pv_last;
float gyro_z_avl = 0;
float gyro_y_avl = 0;
float zero_drift = 0;
float angle = 0;


void tuoluoyi_init(void)
{
    imu660ra_init();
}

void get_gyro(void)
{
    imu660ra_get_gyro();

    gyro_z_pv = (-imu660ra_gyro_z) * 0.5 + gyro_z_pv_last * 0.5;
    gyro_y_pv = imu660ra_gyro_y * 0.9 + gyro_y_pv_last * 0.1;
    

    gyro_z_pv_last = gyro_z_pv;
    gyro_y_pv_last = gyro_y_pv;

    gyro_z_avl = imu660ra_gyro_transition(gyro_z_pv);
    gyro_y_avl = imu660ra_gyro_transition(gyro_y_pv);

    angle_get();
}

void angle_get(void)
{

    angle += gyro_z_avl * 0.002;
}

void angle_clear(void)
{
    angle = 0;
}

