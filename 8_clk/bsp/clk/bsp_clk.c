#include "bsp_clk.h"

/*
 * @description : 使能I.MX6U所有外设时钟
 * @param       : 无
 * @return      : 无
 */
void clk_enable(void)
{
    CCM->CCGR0 = 0XFFFFFFFF;
    CCM->CCGR1 = 0XFFFFFFFF;

    CCM->CCGR2 = 0XFFFFFFFF;
    CCM->CCGR3 = 0XFFFFFFFF;
    CCM->CCGR4 = 0XFFFFFFFF;
    CCM->CCGR5 = 0XFFFFFFFF;
    CCM->CCGR6 = 0XFFFFFFFF;
}

/*初始化时钟*/
void imx6u_clkInit(void)
{
    unsigned int reg = 0;
    /*初始化主频为528（如果是其他SOC可以相应的进行修改）*/
    if (((CCM->CCSR >> 2) & 0x01) == 0) // 等于0，表示多路选择器选择的是pll1_main_clk
    {
        /*初始化step_clk（在修改pll1时，作为备用时钟）*/
        CCM->CCSR &= ~(1 << 8); // 设置step_clk的多路选择器选择 osc_clk = 24MHz（晶振）
        CCM->CCSR |= (1 << 2);  // 设置pll1_sw_clk选择step_clk
    }

    /*1 设置PLL1为1056MHz,使能输出*/
    CCM_ANALOG->PLL_ARM = (1 << 13) | ((88 << 0) & 0x7f);

    /*先设置2分频*/
    CCM->CACRR = 1;
    /*切换时钟为pll1_main_clk*/
    CCM->CCSR &= ~(1 << 2); // 设置pll1_sw_clk选择pll1_main_clk

    /* 2、设置PLL2(SYS PLL)各个PFD */
    reg = CCM_ANALOG->PFD_528;
    reg &= ~(0X3F3F3F3F);       /* 清除原来的设置                      */
    reg |= 32<<24;                /* PLL2_PFD3=528*18/32=297Mhz   */
    reg |= 24<<16;                /* PLL2_PFD2=528*18/24=396Mhz(DDR使用的时钟，最大400Mhz) */
    reg |= 16<<8;             /* PLL2_PFD1=528*18/16=594Mhz   */
    reg |= 27<<0;             /* PLL2_PFD0=528*18/27=352Mhz   */
    CCM_ANALOG->PFD_528=reg; /* 设置PLL2_PFD0~3                */
 
    /* 3、设置PLL3(USB1)各个PFD */
    reg = 0;                    /* 清零   */
    reg = CCM_ANALOG->PFD_480;
    reg &= ~(0X3F3F3F3F);       /* 清除原来的设置                          */
    reg |= 19<<24;                /* PLL3_PFD3=480*18/19=454.74Mhz    */
    reg |= 17<<16;                /* PLL3_PFD2=480*18/17=508.24Mhz    */
    reg |= 16<<8;             /* PLL3_PFD1=480*18/16=540Mhz       */
    reg |= 12<<0;             /* PLL3_PFD0=480*18/12=720Mhz       */
    CCM_ANALOG->PFD_480=reg; /* 设置PLL3_PFD0~3                    */ 

    /*4、设置AHB_CLK_ROOT = 132 MHz*/
    // 先清零，再置位
    CCM->CBCMR &= ~(3 << 18);
    CCM->CBCMR |= 1 << 18; // 设置pre_periph clock = PLL2_PFD2 = 396MHz
    while (CCM->CDHIPR & (1 << 5));
    CCM->CBCDR &= ~(1 << 25);
#if 0
    CCM->CBCDR &= ~(7 << 10);
    CCM->CBCDR |= (2 << 10); /* 3分频率 */
    while (CCM->CDHIPR & (1 << 1));//等待握手忙标志位为0
#endif
    

    /*5、设置IPG_CLK_ROOT = 66MHz*/
    CCM->CBCDR &= ~(3 << 8);
    CCM->CBCDR |= (1 << 8);//设置2分频 IPG_CLK_ROOT =  AHB_CLK_ROOT / 2 = 66 MHz

    /*6、设置PER_CLK_ROOT = 66MHz*/
    CCM->CSCMR1 &= ~(1 << 6);//PER_CLK_ROOT 时钟源为IPG_CLK_ROOT = 66MHz
    CCM->CSCMR1 &= ~(0x3f << 0);//设置1分频
}