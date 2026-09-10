#ifndef __MOTOR_H_
#define __MOTOR_H_

#include "bsp_system.h"


typedef struct{
    pwm_channel_enum pwm_channel;
    gpio_pin_enum    dir_pin;
    uint32           pwm_freq;
    int16            max_duty;
    int16            min_duty;
    int16            current_duty;
    uint8            forward_level;


} Motor_t;

extern Motor_t motor_left;
extern Motor_t motor_right;
extern Motor_t bldc_left;

void Motor_Init(Motor_t *motor,pwm_channel_enum pwm_ch,gpio_pin_enum dir_pin,uint32 pwm_freq,int16 max_duty,int16 min_duty,uint8 forward_level);
void Motor_SetDuty(Motor_t *motor,int16 duty);
void Motor_Output_Limit(Motor_t *motor, int16 duty);




#endif
