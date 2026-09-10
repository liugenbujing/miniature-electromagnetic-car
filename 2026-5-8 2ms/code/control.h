#ifndef __CONTROL_H_
#define __CONTROL_H_

#include "bsp_system.h" 

#define ELEMENT_NONE                 0
#define ELEMENT_QIANG                1
#define ELEMENT_LEFTRING             4
#define ELEMENT_RIGHTRING            3
#define ELEMENT_TONG                 2
#define ELEMENT_QQB                  5
#define ELEMENT_LEFTRING_1           6
#define ELEMENT_RIGHTRING_1          7

#define STATE_TRACK                  1
#define STATE_ELEMENT                2
#define STATE_STOP                   9                 

extern int8 ring_flag,state;
extern float err_dianci,k_diff;
extern int32 distance,test_distance,element_distance;
extern uint16 element_flag;
extern uint8 element,element_count;
extern float left_target,right_target,target_speed,target_yaw,real_target_speed;

void PID_int(void);
void control_motor_reset(void);
void dianci_xunji(void);
void motor_update(void);


#endif
