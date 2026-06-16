#include "bsp_gpio.h"

/*初始化*/
void gpio_init(GPIO_Type *base, int pin, gpio_pin_config_t *config)
{
    if (config->direction == kGPIO_DigitalInput) // 输入模式
    {
        base->GDIR &= ~(1 << pin); // 将相应的引脚的GDIR寄存器位置为0，表示输入模式
    }
    else // 输出
    {
        base->GDIR |= (1 << pin);
        // 设置默认电平值
        gpio_PinWrite(base, pin, config->outputLogic);
    }
    gpio_intconfig(base, pin, config->interruptMode);
}

void gpio_PinWrite(GPIO_Type *base, int pin, int value)
{
    if (value == 0)
    {
        base->DR &= ~(1 << pin);
    }
    else if (value == 1)
    {
        base->DR |= (1 << pin);
    }
}

int gpio_PinRead(GPIO_Type *base, int pin)
{
    int ret;
    ret = ((base->DR) >> pin) & 0x01;
    return ret;
}

/*使能指定IO引脚的中断*/
void gpio_enable_int(GPIO_Type* base, unsigned int pin)
{
    base->IMR |= (1 << pin);//将相应位置1，使能中断
}

/*失能指定IO引脚中断*/
void gpio_disable_int(GPIO_Type *base, unsigned int pin)
{
    base->IMR &= ~(1 << pin);
}

/*清除指定引脚中断标志位*/
void gpio_clearintflags(GPIO_Type *base, unsigned int pin)
{
    base->ISR |= (1 << pin);//将相应的ISR位置1表示清除中断位
}

/*GPIO中断初始化函数*/
void gpio_intconfig(GPIO_Type *base, unsigned int pin, gpio_interrupt_mode_t pin_int_mode)
{
    volatile uint32_t *icr;//用于指定要使用哪一个ICR寄存器，因为有两个ICR寄存器，分别用于低16个引脚和高16个引脚
    uint32_t icrShift;

    icrShift = pin;
    base->EDGE_SEL &= ~(1 << pin);

    if(pin < 16)
    {
        icr = &base->ICR1;
    }
    else
    {
        icr = &base->ICR2;
        icrShift -= 16;
    }

    switch (pin_int_mode)
    {
    case kGPIO_IntLowLevel:
        /*清零，同时也相当于相应的位写入0*/
        *icr &= ~(3 << 2*icrShift);//一个IO用2个位来表示触发模式，icrShift保存着IO在ICR1或ICR2中具体的位域。
                                   //因此这里就是2*icrShift
        break;
    case kGPIO_IntHighLevel:
        *icr &= ~(3 << 2*icrShift);//先清零
        *icr |= (1 << 2 * icrShift);//设置为高电平触发（置1）
        break;
    case kGPIO_IntRisingEdge:
        *icr &= ~(3 << 2*icrShift);//先清零
        *icr |= (2 << 2 * icrShift);//设置为上升沿触发（置2）
        break;
    case kGPIO_IntFallingEdge:
        *icr &= ~(3 << 2*icrShift);//先清零
        *icr |= (3 << 2 * icrShift);//设置为下降沿触发（置3）
        break;
    case kGPIO_IntRisingOrFallingEdge:
        *icr &= ~(3 << (2 * icrShift));
        base->EDGE_SEL |= (1 << pin);
        break;
    default:
        break;
    }
}
