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

unsigned int backclor[10] = {
    LCD_BLUE, LCD_GREEN, LCD_RED, LCD_CYAN, LCD_YELLOW,
    LCD_LIGHTBLUE, LCD_DARKBLUE, LCD_WHITE, LCD_BLACK,
    LCD_ORANGE};

/*
 * @description : mian函数
 * @param       : 无
 * @return      : 无
 */
int main(void)
{
    // unsigned int color;
    unsigned char index = 0;
    static unsigned char state = OFF;
    int_init();      /*初始化中断*/
    imx6u_clkInit(); /*初始化时钟*/
    delay_init();    /*初始化GPT延时函数*/
    lcd_init();      /*初始化lcd*/
    uart_init();     /*初始化串口*/
    clk_enable();    /* 使能所有的时钟          */
    led_init();      /* 初始化led             */
    beep_init();     /*初始化beep             */
    key_init();      /*初始化按键             */

    // /*测试打点函数*/
    // lcd_drawpoint(0, 0, LCD_RED);                                        /*左上方*/
    // lcd_drawpoint(tftlcd_dev.width - 1, 0, LCD_RED);                     /*右上方*/
    // lcd_drawpoint(0, tftlcd_dev.height - 1, LCD_RED);                    /*左下方*/
    // lcd_drawpoint(tftlcd_dev.width - 1, tftlcd_dev.height - 1, LCD_RED); /*右下方*/
    // color = lcd_readpoint(0, 0);
    // printf("左上方 = %#x\r\n", color);

    // color = lcd_readpoint(tftlcd_dev.width - 1, 0);
    // printf("右上方 = %#x\r\n", color);

    // color = lcd_readpoint(0, tftlcd_dev.height - 1);
    // printf("左下方 = %#x\r\n", color);

    // color = lcd_readpoint(tftlcd_dev.width - 1, tftlcd_dev.height - 1);
    // printf("右下方 = %#x\r\n", color);

    tftlcd_dev.forecolor = LCD_RED;
    tftlcd_dev.forecolor = LCD_WHITE;

    while (1)
    {
        index++;
        lcd_clear(backclor[index % 10]);
        lcd_show_string(10, 40, 260, 32, 32, (char *)"ALPAH IMX6U");
        lcd_show_string(10, 80, 240, 24, 24, (char *)"RGBLCD TEST");
        lcd_show_string(10, 110, 240, 16, 16, (char *)"ATOM&ALIENTEK");
        lcd_show_string(10, 130, 240, 12, 12, (char *)"2019/8/16 1:36");
        state = !state;
        led_switch(LED0, state);
        delay_ms(1000);
    }

    return 0;
}
