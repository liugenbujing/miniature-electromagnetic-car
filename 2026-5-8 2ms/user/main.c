#include "main.h"

uint8 ready = 0;
uint16 number = 0;
uint16 duty = 0;
uint8 key_number = 0;
uint8 pump_enable = 0;
volatile uint8 vofa_send_flag = 0;

void tim1_task(void);
void get_data(void);
void take_run(void);
void hardware_init(void);
void system_init(void);
void display_update(void);
void vofa_send_speed(void);

void main()
{
    clock_init(SYSTEM_CLOCK_30M);

	debug_init();

    hardware_init();

    system_init();

	while(1)
	{
        display_update();
//        if(vofa_send_flag)
//        {
//            vofa_send_flag = 0;
//            vofa_send_speed();
//        }


            
        
	}
}


void hardware_init(void)
{
    Motor_Init(&motor_left,PWMA_CH2P_P62,IO_P60,
               (uint32)25000,(int16)9900,(int16)-9900,1);

    Motor_Init(&motor_right,PWMA_CH4P_P66,IO_P64,
               (uint32)25000,(int16)9900,(int16)-9900,1);

    duty = 500;

    pwm_init(PWMB_CH4_P77,50,duty);

    tft_init();

    tuoluoyi_init();

    adc_init(ADC_CHANNEL0,ADC_12BIT);
	adc_init(ADC_CHANNEL1,ADC_12BIT);
	adc_init(ADC_CHANNEL2,ADC_12BIT);
	adc_init(ADC_CHANNEL3,ADC_12BIT);
//    adc_init(ADC_CH5_P15,ADC_12BIT);

    Encoder_Init(&encoder_left,TIM0_ENCOEDER,IO_P35,TIM0_ENCOEDER_P34,0.5f,1);
    Encoder_Init(&encoder_right,TIM3_ENCOEDER,IO_P53,TIM3_ENCOEDER_P04,0.5f,-1);
    wireless_uart_init();
    key_gpio_init();
}


void system_init(void)
{
    PID_int();

    pit_ms_init(TIM1_PIT,2);

    interrupt_set_priority(TIMER1_IRQn,1);

    tim1_irq_handler = tim1_task;
}


void display_update(void)
{
    tft180_show_uint16(0,15,ad_left_x);
    tft180_show_uint16(0,35,ad_left_y);
    tft180_show_uint16(0,55,ad_right_y);
    tft180_show_uint16(0,75,ad_right_x);
    tft180_show_float(0,95,err_dianci,2,1);
//    tft180_show_uint16(0,15,ADC_temp0);
//    tft180_show_uint16(0,35,ADC_temp1);
//    tft180_show_uint16(0,55,ADC_temp2);
//    tft180_show_uint16(0,75,ADC_temp3);
}

void tim1_task(void)
{
    static uint8 vofa_send_div = 0;

    get_data();

    if(pump_enable == 1) take_run();

    else
    {
        Motor_SetDuty(&motor_left,0);

        Motor_SetDuty(&motor_right,0);

        pwm_set_duty(PWMB_CH4_P77,0);
    }

    if(++vofa_send_div >= 5)
    {
        vofa_send_div = 0;
        vofa_send_flag = 1;
    }
}

void get_data(void)
{
    adc_update();
    
    Key_Scan_Handler();

    key_number = Key_GetNum();

    if(key_number == 1)
    {
        if(state == 9)
        {
            pump_enable = 1;

            state = 1;

            ready = 0;

            number = 0;

            duty = 500;
        }
        else pump_enable = !pump_enable;

    }

    loop_update();

    get_gyro();

}

void take_run(void)
{
    switch(ready)
    {

        case 0:

            angle_clear();

            if(++duty > 900) duty = 900;

            pwm_set_duty(PWMB_CH4_P77,duty);

            if(duty == 900 ) ready = 1;

            break;

        case 1:

            if(++number > 800) ready = 2;

            break;

        case 2:

            dianci_xunji();
//            target_speed = 200;
//            left_target = target_speed;
//            right_target = target_speed;

//            speed_loop_l(&speed_loop_l_pid, left_target);
//            speed_loop_r(&speed_loop_r_pid, right_target);

//            Motor_SetDuty(&motor_left,  (int16)out_l);
//            Motor_SetDuty(&motor_right, (int16)out_r);

            break;
    }
}
void vofa_send_speed(void)
{
  static char buf[64];
  // FireWater 格式: 逗号分隔，\r\n 结尾
//  int len = sprintf(buf, "%.1f,%.1f,%.1f,%.1f\r\n",(float)ad_left_x,(float)ad_left_y,(float)ad_right_y,(float)ad_right_x);
//    int len = sprintf(buf, "%.1f,%.1f,%.1f,%.1f\r\n",(float)speed_Left,(float)speed_Right,(float)left_target,(float)right_target);
    int len = sprintf(buf, "%.1f,%.1f,%.1f,%.1f\r\n",(float)state,(float)element_flag,(float)angle,(float)gyro_z_avl);
//    int len = sprintf(buf, "%.1f,%.1f,%.1f,%.1f\r\n",(float)ADC_temp0,(float)ADC_temp1,(float)ADC_temp2,(float)ADC_temp3);
  wireless_uart_send_buffer((uint8*)buf, len);
}
  
