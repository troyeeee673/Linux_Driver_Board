#include "main.h"
#include "bsp_clk.h"
#include "bsp_led.h"
#include "bsp_delay.h"
#include "bsp_beep.h"
#include "bsp_key.h"
#include "bsp_int.h"
#include "bsp_exit.h"
#include "bsp_epit.h"

/*
 * @description : mian函数
 * @param       : 无
 * @return      : 无
 */
int main(void)
{
    int_init();      /*初始化中断*/
    imx6u_clkInit(); /*初始化时钟*/
    clk_enable();    /* 使能所有的时钟          */
    led_init();      /* 初始化led             */
    beep_init();     /*初始化beep             */
    key_init();      /*初始化按键             */
    exit_init();     /*初始化外部中断*/
    epit1_init(0,66000000/2);  /* 初始化epit1,周期为500ms */

    while (1) /* 死循环              */
    {
        delay(500);
    }

    return 0;
}