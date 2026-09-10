#include "encoder.h"

float speed_Right = 0;
float speed_Left  = 0;
float speed_avl   = 0;

Encoder_t encoder_left;
Encoder_t encoder_right;

void Encoder_Init(Encoder_t *encoder,
                  encoder_index_enum index,
                  gpio_pin_enum dir_pin,
                  encoder_channel_enum pulse_channel,
                  float alpha,
                  int8 reverse)
{
    encoder->index = index;
    encoder->dir_pin = dir_pin;
    encoder->pulse_channel = pulse_channel;
    encoder->alpha = alpha;
    encoder->reverse = reverse;
    encoder->raw_data  = 0;
    encoder->last_data = 0;
    encoder->speed = 0.0f;
    
    encoder_dir_init(index, dir_pin, pulse_channel);
}

void Encoder_Update(Encoder_t *encoder)
{
    int16 raw;

    int16 signed_raw;

    float new_speed;

    raw = encoder_get_count(encoder->index);
    signed_raw = raw * encoder->reverse;
    encoder->raw_data = signed_raw;

    new_speed =
        encoder->alpha * encoder->raw_data +
        (1.0f - encoder->alpha) * encoder->speed;

    encoder->speed = new_speed;
    encoder->last_data = encoder->raw_data;


    encoder_clear_count(encoder->index);
}

float Encoder_GetSpeed(Encoder_t *encoder)
{
    return encoder->speed;
}

void loop_update(void)
{
    Encoder_Update(&encoder_left);
    Encoder_Update(&encoder_right);

    speed_Left  = Encoder_GetSpeed(&encoder_left);
    speed_Right = Encoder_GetSpeed(&encoder_right);

    speed_avl = ((float)(func_abs(speed_Left) + func_abs(speed_Right))) / 2.0f;
}
