#include "main.h"
#include "bsp_clk.h"
#include "bsp_led.h"
#include "bsp_delay.h"
#include "bsp_beep.h"
#include "bsp_key.h"

/*
 * @description : mian函数
 * @param       : 无
 * @return      : 无
 */
int main(void)
{
    int i = 0;
    unsigned char led_state = OFF;
    unsigned char beep_state = OFF;
    clk_enable();       /* 使能所有的时钟          */
    led_init();         /* 初始化led             */
    beep_init();        /*初始化beep             */
    key_init();         /*初始化按键             */
 
    while(1)            /* 死循环              */
    {   
        int key_value;
        /*按键控制蜂鸣器*/
        key_value = key_getValue();
        if(key_value)//不为0表示有按键按下
        {
            /*根据不同的按键进行不同的操作*/
            if(key_value == KEY_VALUE0)
            {
                beep_state = !beep_state;
                beep_switch(beep_state);
            }
             

        }

        /*led灯闪烁*/
        i++;
        //一次循环约为10ms，当i=50时，大约500ms，所以LED每隔500ms进行一次闪烁
        if(i == 50)
        {
            i = 0;
            led_state = !led_state;
            led_switch(LED0, led_state);
        }
        delay(10);
    }
 
    return 0;
}