#include "key.h"

Key_TypeDef Key1 = {0};

void key_gpio_init(void)
{
    gpio_init(KEY1_PIN, GPI, 1, GPI_PULL_UP);
//	gpio_init(KEY2_PIN, GPI, 1, GPI_PULL_UP);
//	gpio_init(KEY3_PIN, GPI, 1, GPI_PULL_UP);
//	gpio_init(KEY4_PIN, GPI, 1, GPI_PULL_UP);
}

void Key_Scan_Handler(void)
{
    Key1.current_state = gpio_get_level(KEY1_PIN);
    
    if (Key1.current_state != Key1.last_state) Key1.debounce_count = 0; 
    else 
    {
        if (Key1.debounce_count < 20) 
        {
            Key1.debounce_count++;
            if (Key1.debounce_count == 20) 
            {
                Key1.stable_state = Key1.current_state;
                if (Key1.stable_state == 0 && Key1.press_handled == 0) 
                {
                    Key1.pressed_flag = 1;
                    Key1.press_handled = 1;
                }
                if (Key1.stable_state == 1) 
                {
                    Key1.press_handled = 0;
                }
            }
        }
    }
    Key1.last_state = Key1.current_state;

//    // 按键2处理
//    Key2.current_state = gpio_get_level(KEY2_PIN);
//    
//    if (Key2.current_state != Key2.last_state) {
//        Key2.debounce_count = 0;
//    } else {
//        if (Key2.debounce_count < 20) {
//            Key2.debounce_count++;
//            if (Key2.debounce_count == 20) {
//                Key2.stable_state = Key2.current_state;
//                if (Key2.stable_state == 1 && Key2.press_handled == 0) {
//                    Key2.pressed_flag = 1;
//                    Key2.press_handled = 1;
//                }
//                if (Key2.stable_state == 0) {
//                    Key2.press_handled = 0;
//                }
//            }
//        }
//    }
//    Key2.last_state = Key2.current_state;

//    // 按键3处理
//    Key3.current_state = gpio_get_level(KEY3_PIN);
//    
//    if (Key3.current_state != Key3.last_state) {
//        Key3.debounce_count = 0;
//    } else {
//        if (Key3.debounce_count < 20) {
//            Key3.debounce_count++;
//            if (Key3.debounce_count == 20) {
//                Key3.stable_state = Key3.current_state;
//                if (Key3.stable_state == 1 && Key3.press_handled == 0) {
//                    Key3.pressed_flag = 1;
//                    Key3.press_handled = 1;
//                }
//                if (Key3.stable_state == 0) {
//                    Key3.press_handled = 0;
//                }
//            }
//        }
//    }
//    Key3.last_state = Key3.current_state;

//    // 按键4处理
//    Key4.current_state = gpio_get_level(KEY4_PIN);
//    
//    if (Key4.current_state != Key4.last_state) {
//        Key4.debounce_count = 0;
//    } else {
//        if (Key4.debounce_count < 20) {
//            Key4.debounce_count++;
//            if (Key4.debounce_count == 20) {
//                Key4.stable_state = Key4.current_state;
//                if (Key4.stable_state == 1 && Key4.press_handled == 0) {
//                    Key4.pressed_flag = 1;
//                    Key4.press_handled = 1;
//                }
//                if (Key4.stable_state == 0) {
//                    Key4.press_handled = 0;
//                }
//            }
//        }
//    }
//    Key4.last_state = Key4.current_state;
}

uint8 Key_GetNum(void)
{
    uint8 KeyNum = 0;
    
    if (Key1.pressed_flag) {
        Key1.pressed_flag = 0;
        KeyNum = 1;
    } 
//    else if (Key2.pressed_flag) {
//        Key2.pressed_flag = 0;
//        KeyNum = 2;
//    } else if (Key3.pressed_flag) {
//        Key3.pressed_flag = 0;
//        KeyNum = 3;
//    } else if (Key4.pressed_flag) {
//        Key4.pressed_flag = 0;
//        KeyNum = 4;
//    } 
    
    return KeyNum;
}

