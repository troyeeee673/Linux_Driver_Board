#include "bsp_delay.h"
#include "bsp_int.h"
#include "bsp_led.h"

/*延时初始化函数*/
void delay_init()
{
    GPT1->CR = 0;
    /*设置软复位*/
    GPT1->SR |= (1 << 15);
    while ((GPT1->SR >> 15) & 0x01)
        ; // 等待复为完成

    /*
     * 设置GPT1时钟源为ipg_clk=66M，restart模式
     * 默认计数寄存器从0开始
     */
    GPT1->CR |= (1 << 1) | (1 << 6);

    /* 分频设置 */
    GPT1->PR = 65; /* 66分频，频率=66000000/66=1MHz */
    
    /*打开定时器*/
    GPT1->CR |= 1 << 0;

#if 0
    /* 配置输出比较通道1 */
    GPT1->OCR[0] = 1000000 / 2; /* 设置中断周期为500ms */

    /*使能中断*/
    GIC_EnableIRQ(GPT1_IRQn);

    /*注册终端服务函数*/
    system_register_irqhandler(GPT1_IRQn, gpt1_irqhandler, NULL);


#endif
}

/*微秒延时*/
void delay_us(unsigned int usdelay)
{
    unsigned long oldcnt, newcnt;
    unsigned long tcntvalue = 0;

    oldcnt = GPT1->CNT;
    while (1)
    {
        newcnt = GPT1->CNT;
        if (newcnt != oldcnt)
        {
            if (newcnt > oldcnt)
                tcntvalue += newcnt - oldcnt;
            else
                tcntvalue += 0XFFFFFFFF - oldcnt + newcnt;
            oldcnt = newcnt;
            if (tcntvalue >= usdelay)
                break;
        }
    }
}

/* 毫秒延时 */
void delay_ms(unsigned int msdelay)
{
    int i = 0;
    for (i = 0; i < msdelay; i++)
        delay_us(1000);
}

#if 0
/*中断服务函数*/
void gpt1_irqhandler(unsigned int gicciar, void *param)
{
    static unsigned char state = OFF;
    if ((GPT1->SR & (1 << 0)) == 1)//判断当前中断是不是第一路触发的
    {
        state = !state;
        led_switch(LED0, state);
    }
    /*中断标志位清零*/
    GPT1->SR |= (1 << 0);//写1清零

}
#endif

/*
 * @description : 短时间延时函数
 * @param - n   : 要延时循环次数(空操作循环次数，模式延时)
 * @return      : 无
 */
void delay_short(volatile unsigned int n)
{
    while (n--)
    {
    }
}

/*
 * @description : 延时函数,在396Mhz的主频下
 *                延时时间大约为1ms
 * @param - n   : 要延时的ms数
 * @return      : 无
 */
void delay(volatile unsigned int n)
{
    while (n--)
    {
        delay_short(0x7ff);
    }
}