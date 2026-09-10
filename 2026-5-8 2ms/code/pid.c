#include "pid.h"

Pid_t speed_loop_l_pid,speed_loop_r_pid,position_Loop_pid,gyro_loop_pid;

float gyro_err,err_l,err_r;
float out_l = 0,out_r = 0,out_position = 0,out_gyro = 0;



void pid_init(Pid_t *pid,float kp,float kp_sq,float ki,float kd)
{
    memset(pid, 0, sizeof(Pid_t));
    pid->kp = kp;
    pid->kp_sq = kp_sq;
    pid->ki = ki;
    pid->kd = kd;
    pid->last_err = 0;
}

void speed_loop_l(Pid_t *pid,int16 target_speed_l)
{
    static float dec_speed_loop_L;
    
    err_l = target_speed_l - speed_Left;
    dec_speed_loop_L = pid->kp * (err_l - pid->last_err) + pid->ki * err_l ;
    out_l +=  dec_speed_loop_L;
    pid->last_err = err_l;
    
    out_l = func_limit_ab(out_l,-9900,9900);
}

void speed_loop_r(Pid_t *pid,int16 target_speed_r)
{
    static float dec_speed_loop_R;
    
    err_r = target_speed_r - speed_Right;
    dec_speed_loop_R = pid->kp * (err_r - pid->last_err) + pid->ki * err_r;
    out_r += dec_speed_loop_R;
    pid->last_err = err_r;
    
    out_r = func_limit_ab(out_r,-9900,9900);
}

//void Position_Loop(Pid_t *pid,float err)
//{
//    float diff_err = 0;
//    
//    diff_err = err - pid->last_err;

//    out_position = err * pid->kp + err * func_abs(err) * pid->kp2 + diff_err * pid->kd;
//    pid->last_err = err;

//    out_position = func_limit_ab(out_position, -100, 100);
//}

void Position_Loop(Pid_t *pid,float err)
{
    float diff_err = 0;
    float kp,kp2,kd;
    float base;
    
    diff_err = err - pid->last_err;
    
    base = func_abs(err) / 20.0f + speed_avl / 80.0f;
    base = func_limit_ab(base,0.0f,10.0f);
    
    kp = pid->kp;
    kp2 = pid->kp_sq + base * 0.003f;
    kd = pid->kd + base * 2.25f;
    pid->last_err = err;
    
    out_position = err * kp + err * func_abs(err) * pid->kp_sq + diff_err * kd;
    out_position = func_limit_ab(out_position,-100,100);
}

void speed_condition(void)
{
    static float last_abs_err = 0;
    float abs_err;
    float err_change;
    float road_strong;
    float slow_k;
    
    abs_err = func_abs(err_dianci);
    err_change = abs_err - last_abs_err;
    last_abs_err = abs_err;
    
    road_strong = ad_left_y + ad_right_y;
    road_strong = func_limit_ab(road_strong, 0.0f, 120.0f) / 120.0f;
    
    slow_k = abs_err / 100.0f * 0.75f;
    slow_k += func_limit_ab(err_change / 25.0f, -0.2f, 0.4f);
    
    slow_k += road_strong * 0.35f;
    
    slow_k = func_limit_ab(slow_k, 0.0f, 1.0f);
    
    target_speed = MAX_SPD - (MAX_SPD - MIN_SPD) * slow_k;
    
}


void gyro_loop(Pid_t *pid,float target_gyro)
{
    gyro_err = target_gyro - gyro_z_avl;
    out_gyro = pid->kp * gyro_err + pid->kd * (gyro_err - pid->last_err);
    pid->last_err = gyro_err;
    
    out_gyro = func_limit_ab(out_gyro,-100,100);
}



