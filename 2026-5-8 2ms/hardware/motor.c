#include "motor.h"

Motor_t motor_left;
Motor_t motor_right;
Motor_t bldc_left;


/***************************电机初始化*********************************
函数：  void Motor_Init()  
功能：  电机初始化
参数：  电机结构体、PWM通道、方向引脚、PWM频率、
        最大占空比、最小占空比、电机正转电平
说明：  初始化PWM与电机方向引脚
返回值：无
************************************************************************/
void Motor_Init(Motor_t *motor,
                pwm_channel_enum pwm_ch,
                gpio_pin_enum dir_pin,
                uint32 pwm_freq,
                int16 max_duty,
                int16 min_duty,
                uint8 forward_level)
{
    motor->pwm_channel = pwm_ch;

    motor->dir_pin = dir_pin;

    motor->pwm_freq = pwm_freq;

    motor->max_duty = max_duty;

    motor->min_duty = min_duty;

    motor->forward_level = forward_level;

    /*
    PWM初始化
    */
    pwm_init(pwm_ch,pwm_freq,0);

    /*
    电机方向引脚初始化
    */
    gpio_init(dir_pin,GPO,1,GPO_PUSH_PULL);
}


/***************************电机输出*********************************
函数：  void Motor_SetDuty()  
功能：  设置电机PWM
参数：  电机结构体、电机占空比
说明：  根据正负值控制电机方向与PWM输出
返回值：无
************************************************************************/
void Motor_SetDuty(Motor_t *motor,int16 duty)
{
    /*
    正转
    */
    if(duty >= 0)
    {
        pwm_set_duty(motor->pwm_channel,duty);

		gpio_set_level(motor->dir_pin,
                       motor->forward_level);
    }

    /*
    反转
    */
    else
	{
		pwm_set_duty(motor->pwm_channel,-duty);

		gpio_set_level(motor->dir_pin,
                       !motor->forward_level);
	}

    /*
    保存当前PWM
    */
    motor->current_duty = duty;
}


/***************************电机限幅输出*********************************
函数：  void Motor_Output_Limit()  
功能：  电机输出限幅
参数：  电机结构体、电机占空比
说明：  对PWM限幅后输出到电机
返回值：无
************************************************************************/
void Motor_Output_Limit(Motor_t *motor, int16 duty)
{
    /*
    PWM限幅
    */
    duty = func_limit_ab(duty, -9900, 9900);

    /*
    输出PWM
    */
    Motor_SetDuty(motor, duty);
}