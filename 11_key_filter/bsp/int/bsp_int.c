#include "bsp_int.h"

static unsigned int irqNesting = 0;//记录中断嵌套数

/*中断处理函数表*/
static sys_irq_handle_t irqTable[NUMBER_OF_INT_VECTORS];


/* 默认中断处理函数 */
void default_irqhandler(unsigned int gicciar, void *userParam)
{
    while(1)
    {

    }
}

/*初始化中断处理函数表*/
void system_irqTable_init(void)
{
    unsigned int i = 0;
    for(;i < NUMBER_OF_INT_VECTORS ;i ++)
    {
        irqTable[i].irqHandler = default_irqhandler;
        irqTable[i].userParam = NULL;
    }
}

/*注册中断处理函数*/
void system_register_irqhandler(IRQn_Type irq, system_irq_handler_t handler, void* userParam)
{
     irqTable[irq].irqHandler = handler;
     irqTable[irq].userParam = userParam;


}
/*中断初始化函数*/
void int_init(void)
{
    GIC_Init();//中断控制器初始化
    system_irqTable_init();

    /*中断向量偏移色设置*/
    __set_VBAR(0x87800000);



}

/*具体中断处理函数， IRQ_handler会调用此函数*/
void system_irqhandler(unsigned int gicciar)
{
    /*检查中断id是否正常*/
    if(gicciar >= 160)
    {
        return;
    }
    irqNesting++;
    /*根据id读取中断处理函数*/
    irqTable[gicciar].irqHandler(gicciar, irqTable[gicciar].userParam);
    irqNesting--;


}