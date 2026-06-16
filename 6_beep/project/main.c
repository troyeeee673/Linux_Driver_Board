#include "main.h"
#include "bsp_clk.h"
#include "bsp_led.h"
#include "bsp_delay.h"
#include "bsp_beep.h"
 
/*
 * @description : mian函数
 * @param       : 无
 * @return      : 无
 */
int main(void)
{
    clk_enable();       /* 使能所有的时钟          */
    led_init();         /* 初始化led             */
    beep_init();        /*初始化beep             */
 
    while(1)            /* 死循环              */
    {   
        led_off();      /* 关闭LED            */
        beep_switch(OFF);
        delay(500);     /* 延时500ms          */
 
        led_on();       /* 打开LED            */
        beep_switch(ON);
        delay(500);     /* 延时500ms          */
    }
 
    return 0;
}