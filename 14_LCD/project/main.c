#include "main.h"
#include "bsp_clk.h"
#include "bsp_led.h"
#include "bsp_delay.h"
#include "bsp_beep.h"
#include "bsp_key.h"
#include "bsp_int.h"
#include "bsp_exit.h"
#include "bsp_epit.h"
#include "bsp_key_filter.h"
#include "bsp_uart.h"
#include "stdio.h"
#include "bsp_lcd.h"

/*
 * @description : mian函数
 * @param       : 无
 * @return      : 无
 */
int main(void)
{
    int a, b;
    static unsigned char state = OFF;
    int_init();      /*初始化中断*/
    imx6u_clkInit(); /*初始化时钟*/
    delay_init();   /*初始化GPT延时函数*/
    lcd_init();     /*初始化lcd*/
    uart_init();    /*初始化串口*/
    clk_enable();    /* 使能所有的时钟          */
    led_init();      /* 初始化led             */
    beep_init();     /*初始化beep             */
    key_init();      /*初始化按键             */


    while (1) /* 死循环              */
    {
        state = !state;
        led_switch(LED0, state);
        delay_ms(500);
    }

    return 0;
}