#include "control.h"

int8 state = STATE_TRACK;
int8 ring_flag = 0;
uint16 element_flag = 0;
int8 ring_dir = 0;
uint8 element = 0,element_count = 0;
uint8 stop_count = 0;
uint8 stop_number = 0;
float target_speed = 0;
float target_yaw  = 0;
float real_target_speed = 0;
float k_diff = 0;
int32 element_distance = 0;
int32 distance = 0;
int32 test_distance = 0;
float left_target,right_target;
static uint16 stall_left_cnt = 0;
static uint16 stall_right_cnt = 0;
uint16 time_qqb = 0;
static uint16 ring_timeout_cnt = 0;
float ring_save_angle = 0;
uint8 manghe = 0;
uint16 manghe_time = 0;

void PID_int(void)   
{
    pid_init(&speed_loop_l_pid, 15 , 0 , 1.5 , 0);
    pid_init(&speed_loop_r_pid, 15 , 0 , 1.5 , 0);
    pid_init(&position_Loop_pid, 1.25 , 0.0175 , 0 , 25);
    pid_init(&gyro_loop_pid, 0.22 , 0 , 0 , 0);

}

void element_detect(void)
{
    if (ad_left_x + ad_left_y + ad_right_y + ad_right_x < 35)
    {
        stop_count ++;
        if(stop_count >= 50)
        {
            stop_count = 0;
            state = STATE_STOP;
            return;
        }
    }
    else stop_count = 0;

    if( state != STATE_ELEMENT && (ad_right_x + ad_left_x) > 135 && element_flag == 0)
    {
        if(++element_count > 9) 
        {
            element_count = 1;
            stop_number++;
        }
        
        state = STATE_ELEMENT;
        element_flag = 600;
        switch(element_count)
        {
            case 1:
                element = ELEMENT_QIANG;
                break;
            case 2:
                element = ELEMENT_RIGHTRING;
                ring_dir = -1;
                ring_save_angle = angle;
                break;
            case 3:
                element = ELEMENT_LEFTRING;
                ring_dir = 1;
                ring_save_angle = angle;
                break;
            case 4:
                element = ELEMENT_TONG;
                break;
            case 5:
                element = ELEMENT_QIANG;
                break;
            case 6:
                element = ELEMENT_RIGHTRING;
                ring_dir = -1;
                ring_save_angle = angle;
                break;
            case 7:
                element = ELEMENT_LEFTRING;
                ring_dir = 1;
                ring_save_angle = angle;
                break;
            case 8:
                element = ELEMENT_TONG;
                break;
            case 9:
                element = ELEMENT_QIANG;
                manghe = 1;
                break;
            default:
                element = ELEMENT_NONE;
                break;
        }
        ring_flag = 0;  
        distance = 0;
    }
    if(ad_right_x < 30 && ad_left_x < 30 && gyro_y_avl > 250 && (ad_left_y + ad_right_y) < 20 && element_flag == 0)
    {
        state = STATE_ELEMENT;
        element_flag = 400;
        element = ELEMENT_QQB;
    }
    if(stop_number > 0) state = STATE_STOP;
    
}

void ring_update_1(void)//岛内循迹环岛
{
    if(ring_dir == 1)
    {
        switch(ring_flag)
        {
            case 0:
                target_speed = 200;
                distance += (int32)speed_avl;
                Position_Loop(&position_Loop_pid,err_dianci);   
                if(distance > 13000)
                {
                        ring_flag = 1;
                        distance = 0;
                }
                break;
            case 1:
                target_speed = 200;   
                distance += (int32)speed_avl;
                Position_Loop(&position_Loop_pid,10);   
                if(distance > 8000)  
                {
                    distance = 0;
                    ring_flag = 2;
                }
                break;
            case 2:
                target_speed = 300;
                Position_Loop(&position_Loop_pid,err_dianci);
                if(abs(angle - ring_save_angle) > 310) 
                {
                    ring_flag = 3;
                    
                }
                break;
            case 3:
                target_speed = 200;
                distance += (int32)speed_avl;
                Position_Loop(&position_Loop_pid,0);
                if(distance > 16000)
                {
                    distance = 0;
                    angle_clear();
                    state = STATE_TRACK;
                    // state = STATE_STOP;
                    ring_flag = 0;
                    element = ELEMENT_NONE;
                }
                break;
        }
    }
    else
    {
        switch(ring_flag)
        {
            case 0:
                target_speed = 200;   
                distance += (int32)speed_avl;
                Position_Loop(&position_Loop_pid,err_dianci);  
                if(distance > 10000)
                {
                    // if(ad_right_x + ad_left_x > 100)  
                    // {
                        ring_flag = 1;
                        distance = 0;
                        // state = STATE_STOP;
                    // }
                    // else if(ad_right_x + ad_left_x < 100)  
                    // {
                    //     distance = 0;
                    //     ring_flag = 0;
                    //     element = ELEMENT_NONE;
                    //     state = STATE_TRACK;
                    // }
                }
                break;
            case 1:
                target_speed = 200;  
                distance += (int32)speed_avl;
                Position_Loop(&position_Loop_pid,-15);   
                if(distance > 8000)  
                {
                    distance = 0;
                    ring_flag = 2;
                }
                break;
            case 2:
                target_speed = 300;
                Position_Loop(&position_Loop_pid,err_dianci);
                if(abs(angle - ring_save_angle) > 310) 
                {
                    ring_flag = 3;
                    
                }
                break;
            case 3:
                target_speed = 200;
                distance += (int32)speed_avl;
                Position_Loop(&position_Loop_pid,0);
                if(distance > 15000)
                {
                    distance = 0;
                    angle_clear();
                    state = STATE_TRACK;
                    ring_flag = 0;
                    element = ELEMENT_NONE;
                }
                break;
            
        }
    }
    
}




void strategy_update(void)
{
    switch(state)
    {
        case STATE_TRACK:
            speed_condition();
            Position_Loop(&position_Loop_pid,err_dianci);
            break;
        case STATE_ELEMENT:
            switch(element)
            {
                case ELEMENT_QQB:
                    target_speed = 100;
                    if(++time_qqb > 100)
                    {
                        time_qqb = 0;
                        state = STATE_TRACK;
                        element = ELEMENT_NONE;
                    }
                    Position_Loop(&position_Loop_pid,err_dianci);
                    break;
                case ELEMENT_LEFTRING:
                    ring_update_1();
                    break;
                case ELEMENT_RIGHTRING:
                    ring_update_1();
                    break;
                case ELEMENT_TONG:
                    state = STATE_TRACK;
                    element = ELEMENT_NONE;                    
                    break;

                case ELEMENT_QIANG:
                    if(manghe == 1)
                    {
                        target_speed = 400;
                        Position_Loop(&position_Loop_pid,err_dianci);
                        element_distance += (int32)speed_avl;
                        if(++manghe_time > 250)
                        {
                            state = STATE_STOP;
                            manghe_time = 0;
                            manghe = 0;
                            element = ELEMENT_NONE;
                        }
                        break;
                    }
                    else
                    {
                        target_speed = 400;
                        Position_Loop(&position_Loop_pid,err_dianci);
                        element_distance += (int32)speed_avl;
                        if(element_distance > 95000)
                        {
                            state = STATE_TRACK;
                            element_distance = 0;
                            element = ELEMENT_NONE;
                        }
                        break;
                    }
            }
            break;
        case STATE_STOP:
            target_speed = 0;
            target_yaw = 0;
            control_motor_reset();
            Motor_SetDuty(&motor_left,0);
            Motor_SetDuty(&motor_right,0);

            break;
    }
    if(element_flag > 0) element_flag --;
}

void control_motor_reset(void)
{
    target_speed = 0;
    left_target = 0;
    right_target = 0;
    out_l = 0;
    out_r = 0;
    speed_loop_l_pid.last_err = 0;
    speed_loop_r_pid.last_err = 0;
}


void motor_update(void)
{
    k_diff = out_position * 0.01;
    k_diff = func_limit_ab(k_diff,-0.85,0.85);
    if(k_diff >= 0)
    {           
        left_target = target_speed * (1 - k_diff);
        right_target = target_speed * (1 + k_diff * 0.2);
    }
    else
    {
        k_diff *= -1;
        left_target = target_speed * (1 + k_diff * 0.2);
        right_target = target_speed * (1 - k_diff);
    }

    speed_loop_l(&speed_loop_l_pid, left_target);
    speed_loop_r(&speed_loop_r_pid, right_target);
    Motor_Output_Limit(&motor_left,  (int16)out_l);
    Motor_Output_Limit(&motor_right, (int16)out_r);
}

void dianci_xunji(void)
{
    if(state != STATE_STOP)
    {
        element_detect();
    }
    strategy_update();
    motor_update();
}
