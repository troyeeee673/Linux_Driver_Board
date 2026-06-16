#include "bsp_key.h"
#include "bsp_delay.h"
#include "bsp_gpio.h"

/*初始化按键*/
void key_init()
{
    gpio_pin_config_t config;
    //复用为GPIO1——GPIO18
    IOMUXC_SetPinMux(IOMUXC_UART1_CTS_B_GPIO1_IO18, 0);

    //设置电气属性
    IOMUXC_SetPinConfig(IOMUXC_UART1_CTS_B_GPIO1_IO18, 0xF080);

    //配置GPIO
    // GPIO1->GDIR &= ~(1<<18); //设置为输入
    config.direction = kGPIO_DigitalInput;
    gpio_init(GPIO1, 18, &config);

}

/*读取按键值
 *返回值：0表示按键按下，1表示未按下
**/
int read_key(void)
{
    int ret = 0;
    // ret = ((GPIO1->DR) >> 18) & 0x01;//读取GPIO1数据寄存器DR，读取第18号引脚的值
    ret = gpio_PinRead(GPIO1, 18);
    return ret;
}

/*获取按键键值*/
int key_getValue(void)
{
    int ret = 0 ;
    static unsigned char release = 1;//为1表示按键被释放

    if(release == 1 && gpio_PinRead(GPIO1, 18) == 0)
    {
        delay(10);
        release = 0;
        if(gpio_PinRead(GPIO1, 18) == 0)
        {
            ret = KEY_VALUE0; //这里表示key0被按下，因为目前只枚举了KEY0
        }
    }else if(gpio_PinRead(GPIO1, 18)==1)
    {
        ret = KEY_NONE; //没有按键按下
    }
    return ret;//返回键值
}