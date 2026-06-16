#include "bsp_key_filter.h"
#include "bsp_gpio.h"
#include "bsp_int.h"
#include "bsp_beep.h"


/*keyfilter初始化*/
void key_filter_init()
{
    gpio_pin_config_t key_config;
    IOMUXC_SetPinMux(IOMUXC_UART1_CTS_B_GPIO1_IO18, 0); /* 复用为GPIO1_IO18 */
    IOMUXC_SetPinConfig(IOMUXC_UART1_CTS_B_GPIO1_IO18, 0XF080);

    /* GPIO初始化 */
    key_config.direction = kGPIO_DigitalInput;
    key_config.interruptMode = kGPIO_IntFallingEdge;
    gpio_init(GPIO1, 18, &key_config);

    GIC_EnableIRQ(GPIO1_Combined_16_31_IRQn);
    system_register_irqhandler(GPIO1_Combined_16_31_IRQn,  gpio1_io_16_31_irqhandler, NULL);
    gpio_enable_int(GPIO1, 18);

    /*定时器初始化*/
    filtertimer_init(66000000/100);
}


//EPIT1定时器初始化
void filtertimer_init(unsigned int value)
{
    /*配置EPIT1寄存器*/
    EPIT1->CR = 0;//清零
    EPIT1->CR = (1 << 1) | (1 << 2) | (1 << 3) | (1 << 24);

    EPIT1->LR = value; /* 加载寄存器，相当于倒计数值 */

    EPIT1->CMPR = 0;

    /*初始化中断*/
    GIC_EnableIRQ(EPIT1_IRQn) ;//使能中断

    system_register_irqhandler(EPIT1_IRQn, filtertimer_irqhandler, NULL);

}


/* 关闭EPIT1定时器 */
void filtertimer_stop(void)
{
    EPIT1->CR &= ~(1 << 0);
}

/* 重启EPIT1定时器 */
void filtertimer_restart(unsigned int value)
{
    EPIT1->CR &= ~(1 << 0);
    EPIT1->LR = value;
    EPIT1->CR |= 1 << 0;
}

void filtertimer_irqhandler(unsigned int gicciar, void*param)
{
    static unsigned char state = OFF;

    if(EPIT1->SR & (1 << 0))//中断触发
    {
        /*关闭定时器，防止重复触发*/
        filtertimer_stop();
        if(gpio_PinRead(GPIO1, 18) == 0)
        {
            state = !state;
            beep_switch(state);
        }

    }
    /*清楚标志位*/
    EPIT1->SR |= (1 << 0);
}

//按键中断服务函数
void gpio1_io_16_31_irqhandler(unsigned int gicciar, void*param)
{
    /*开启定时器*/
    filtertimer_restart(66000000/100);
    /*清楚标志位*/
    gpio_clearintflags(GPIO1, 18);
}