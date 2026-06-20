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
#include "bsp_lcdapi.h"
#include "bsp_ap3216c.h"
#include "bsp_i2c.h"
#include "bsp_icm20608.h"
#include "bsp_ft5426.h"
#include "bsp_backlight.h"

unsigned int backclor[10] = {
    LCD_BLUE, LCD_GREEN, LCD_RED, LCD_CYAN, LCD_YELLOW,
    LCD_LIGHTBLUE, LCD_DARKBLUE, LCD_WHITE, LCD_BLACK,
    LCD_ORANGE};

/*
 * @description : 使能I.MX6U的硬件NEON和FPU
 * @param       : 无
 * @return      : 无
 */
void imx6ul_hardfpu_enable(void)
{
    uint32_t cpacr;
    uint32_t fpexc;

    /* 使能NEON和FPU */
    cpacr = __get_CPACR();
    cpacr = (cpacr & ~(CPACR_ASEDIS_Msk | CPACR_D32DIS_Msk)) | (3UL << CPACR_cp10_Pos) | (3UL << CPACR_cp11_Pos);
    __set_CPACR(cpacr);
    fpexc = __get_FPEXC();
    fpexc |= 0x40000000UL;
    __set_FPEXC(fpexc);
}

int main(void)
{
    unsigned char key;
    unsigned char duty = 0;
    unsigned char state = OFF;

    imx6ul_hardfpu_enable(); /* 使能I.MX6U的硬件浮点            */
    int_init();              /* 初始化中断(一定要最先调用！) */
    imx6u_clkinit();         /* 初始化系统时钟                  */
    delay_init();            /* 初始化延时                    */
    clk_enable();            /* 使能所有的时钟                  */
    led_init();              /* 初始化led                   */
    beep_init();             /* 初始化beep                  */
    uart_init();             /* 初始化串口，波特率115200 */
    lcd_init();              /* 初始化LCD                   */
    backlight_init();        /*初始化背光*/

    tftlcd_dev.forecolor = LCD_RED;
    lcd_show_string(50, 10, 400, 24, 24, (char *)"ALPHA-IMX6U backlight TEST");
    lcd_show_string(50, 40, 200, 16, 16, (char *)"backlight TEST");
    lcd_show_string(50, 60, 200, 16, 16, (char *)"ATOM@ALIENTEK");
    lcd_show_string(50, 80, 200, 16, 16, (char *)"2026/6/20");

    while (1)
    {
        key = key_getValue();
        if (key == KEY_VALUE0)
        {
            duty += 10;
            if (duty >= 100)
                duty = 0;
            printf("pwm1 duty: %d%%\r\n", duty);
        }
        delay_ms(50);
        state = !state;
        led_switch(LED0, state);
    }
}
