#ifndef __BSP_BEEP_h
#define __BSP_BEEP_h
#include "fsl_common.h"
#include "fsl_iomuxc.h"
#include "MCIMX6Y2.h"

void beep_init(void);
void beep_switch(int status);
#endif // !__BSP_BEEP_h
