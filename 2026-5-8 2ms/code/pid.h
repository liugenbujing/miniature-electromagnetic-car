#ifndef __PID_H_
#define __PID_H_

#include "bsp_system.h"

#define MAX_SPD     450.0f
#define MIN_SPD     300.0f


typedef struct 
{
    float kp;
    float kp_sq;
    float ki;
    float kd;
    float last_err;
} Pid_t;

extern Pid_t speed_loop_l_pid,speed_loop_r_pid,position_Loop_pid,gyro_loop_pid;
extern float out_l,out_r,out_position,out_gyro;


void pid_init(Pid_t *pid,float kp,float kp2,float ki,float kd);
void speed_loop_l(Pid_t *pid,int16 target_speed_l);
void speed_loop_r(Pid_t *pid,int16 target_speed_r);
void Position_Loop(Pid_t *pid,float err);
void speed_condition(void);
void gyro_loop(Pid_t *pid,float target_gyro);


#endif
