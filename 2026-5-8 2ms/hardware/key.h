#ifndef _KEY_H_
#define _KEY_H_

#include "bsp_system.h"

#define KEY1_PIN    IO_P36
//#define KEY2_PIN    IO_P71
//#define KEY3_PIN    IO_P72
//#define KEY4_PIN    IO_P73

typedef struct {
    uint8 current_state;    
    uint8 last_state;       
    uint8 debounce_count;     
    uint8 stable_state;     
    uint8 pressed_flag; 
    uint8 press_handled;    
} Key_TypeDef;

extern Key_TypeDef Key1;

void Key_Scan_Handler(void);
void key_gpio_init(void);
uint8 Key_GetNum(void);

#endif
