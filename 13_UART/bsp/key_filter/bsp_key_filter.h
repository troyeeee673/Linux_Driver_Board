#ifndef __BSP_KEY_FILTER_H
#define __BSP_KEY_FILTER_H
#include "imx6u.h"
void key_filter_init();
void filtertimer_init(unsigned int value);
void filtertimer_stop(void);
void filtertimer_restart(unsigned int value);
void filtertimer_irqhandler(unsigned int gicciar, void*param);
void gpio1_io_16_31_irqhandler(unsigned int gicciar, void*param);
#endif // !__BSP_KEY_FILTER_H
