#ifndef __ENCODER_H
#define __ENCODER_H

#include "bsp_system.h"

typedef struct{
    encoder_index_enum   index;
    gpio_pin_enum        dir_pin;
    encoder_channel_enum pulse_channel;
    int16                raw_data;
    int16                last_data;
    float                speed;
    float                alpha;
    int8                 reverse;


}Encoder_t;

extern Encoder_t encoder_left;
extern Encoder_t encoder_right;
extern float speed_Right,speed_Left,speed_avl;

void Encoder_Init(Encoder_t *encoder,encoder_index_enum index,gpio_pin_enum dir_pin,encoder_channel_enum pulse_channel,float alpha,int8 reverse);
void Encoder_Update(Encoder_t *encoder);
void loop_update(void);


#endif 
