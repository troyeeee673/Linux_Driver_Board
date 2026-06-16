#include "bsp_beep.h"
#include "cc.h"


/* BEEP初始化 */
void beep_init(void)
{
    IOMUXC_SetPinMux(IOMUXC_SNVS_SNVS_TAMPER1_GPIO5_IO01, 0); /*复用为GPIO*/
    IOMUXC_SetPinConfig(IOMUXC_SNVS_SNVS_TAMPER1_GPIO5_IO01, 0X10b0);

    /* GPIO初始化 */
    GPIO5->GDIR |= (1<<1);    /* 设置为输出 */
    GPIO5->DR   |= (1<<1);   /* 默认关闭 */
}

/* 蜂鸣器控制函数 */
void beep_switch(int status)
{
    if(status == OFF)
        GPIO5->DR |= (1<<1);
    else if(status == ON)
        GPIO5->DR &= ~(1<<1);
}