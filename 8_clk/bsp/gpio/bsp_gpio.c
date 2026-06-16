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