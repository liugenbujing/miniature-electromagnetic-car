#ifndef ADC_H
#define ADC_H

#include "bsp_system.h"

#define ADC_CHANNEL0                    (ADC_CH9_P01)
#define ADC_CHANNEL1                    (ADC_CH8_P00)
#define ADC_CHANNEL2                    (ADC_CH13_P05)
#define ADC_CHANNEL3                    (ADC_CH14_P06)

extern float ad_left_x,ad_left_y,ad_right_y,ad_right_x;
extern float ADC_temp0,ADC_temp1,ADC_temp2,ADC_temp3;
extern float battery_voltage;
extern float err_dianci,k,LRM_fabs;


void adc_sampling(void);
void adc_normalization(void);
void err_update(void);
void adc_update(void);

#endif
