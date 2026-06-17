#ifndef __BSP_DELAY_H
#define __BSP_DELAY_H
#include "fsl_common.h"
#include "fsl_iomuxc.h"
#include "MCIMX6Y2.h"
#include "imx6u.h"

void delay_init();
void delay_us(unsigned int usdelay);
void delay_ms(unsigned int msdelay);
void gpt1_irqhandler(unsigned int gicciar, void* param);
void delay_short(volatile unsigned int n);
void delay(volatile unsigned int n);

#endif // !__BSP_CLK_H
