#ifndef __BSP_KEY_H
#define __BSP_KEY_H
#include "fsl_common.h"
#include "fsl_iomuxc.h"
#include "MCIMX6Y2.h"

/*按键值枚举*/
enum keyValue{
    KEY_NONE = 0,
    KEY_VALUE0 , //后续可以添加更多的枚举元素

};

/*函数声明*/
void key_init(void);
int read_key(void);
int key_getValue(void);

#endif // !__BSP_KEY_H
