#include "bsp_lcd.h"
#include "bsp_gpio.h"
#include "stdio.h"
#include "bsp_delay.h"

/*屏幕参数结构体变量*/
struct tftlcd_typedef tftlcd_dev;
/*LCD初始化*/
void lcd_init()
{
    /*通过串口打印lcd id*/
    unsigned short lcdid = 0;
    lcdid = lcd_read_panelid();
    printf("LCD ID=%#X\r\n", lcdid);

    /*初始化屏幕IO*/
    lcdgpio_init();

    /*复位*/
    lcd_reset();
    delay_ms(10);
    /*停止复位*/
    lcd_noreset();

    /*根据不同的屏幕id设置屏幕参数*/
    if (lcdid == ATK4342)
    {
        tftlcd_dev.height = 272;
        tftlcd_dev.width = 480;
        //这里开始vspw和hspw都为0，屏幕背光会闪烁，不稳定，改为1后，相当于手动“伪造”了同步脉冲宽度，恰好把一帧的总时间修正到了屏幕能接受的范围内。
        tftlcd_dev.vbpd = 8;
        tftlcd_dev.vspw = 1;
        tftlcd_dev.vfpd = 8;
        tftlcd_dev.hspw = 1;
        tftlcd_dev.hbpd = 40;
        tftlcd_dev.hfpd = 5;
        lcdclk_init(27, 8, 8); /* 初始化LCD时钟 */
    }
    else if (lcdid == ATK4384)
    {
        tftlcd_dev.height = 480;
        tftlcd_dev.width = 800;
        tftlcd_dev.vspw = 3;
        tftlcd_dev.vbpd = 32;
        tftlcd_dev.vfpd = 13;
        tftlcd_dev.hspw = 48;
        tftlcd_dev.hbpd = 88;
        tftlcd_dev.hfpd = 40;
        // lcdclk_init(42, 4, 8); /* 初始化LCD时钟 31.5MHz */
    }
    else if (lcdid == ATK7084)
    {
        tftlcd_dev.height = 480;
        tftlcd_dev.width = 800;
        tftlcd_dev.vspw = 1;
        tftlcd_dev.vbpd = 23;
        tftlcd_dev.vfpd = 22;
        tftlcd_dev.hspw = 1;
        tftlcd_dev.hbpd = 46;
        tftlcd_dev.hfpd = 210;
        // lcdclk_init(30, 3, 7); /* 初始化LCD时钟 34.2MHz */
    }
    else if (lcdid == ATK7016)
    {
        tftlcd_dev.height = 600;
        tftlcd_dev.width = 1024;
        tftlcd_dev.vspw = 3;
        tftlcd_dev.vbpd = 20;
        tftlcd_dev.vfpd = 12;
        tftlcd_dev.hspw = 20;
        tftlcd_dev.hbpd = 140;
        tftlcd_dev.hfpd = 160;
        // lcdclk_init(32, 3, 5); /* 初始化LCD时钟 51.2MHz */
    }

    /*后面这几个参数对于不同屏幕都适用*/
    tftlcd_dev.pixsize = 4; /* 每个像素4个字节 */
    tftlcd_dev.framebuffer = LCD_FRAMEBUF_ADDR;
    tftlcd_dev.forecolor = LCD_WHITE; /* 前景色白色 */
    tftlcd_dev.backcolor = LCD_BLACK; /* 背景色黑色 */

    /*配置LCDIF控制器接口*/
    LCDIF->CTRL = 0;
    LCDIF->CTRL |= (1 << 5) | (3 << 8) | (3 << 10) | (1 << 17) | (1 << 19); // 设置为工作在主机模式下, 设置为24位像素输入格式， 设置24位总线传输格式， 使用DOTCLOCK模式,在这个模式下，bit19必须为1

    LCDIF->CTRL1 = 0;
    LCDIF->CTRL1 |= 7 << 16;

    LCDIF->TRANSFER_COUNT = 0;
    LCDIF->TRANSFER_COUNT |= (tftlcd_dev.height << 16); // 16~31设置行数
    LCDIF->TRANSFER_COUNT |= (tftlcd_dev.width << 0);   // 0~15设置一行有多少个像素点

    LCDIF->VDCTRL0 = 0;
    LCDIF->VDCTRL0 |= (tftlcd_dev.vspw << 0); // 0~17 设置vspw
    LCDIF->VDCTRL0 |= (1 << 20);              // Bit20 设置 vsync 信号的宽度单位
    LCDIF->VDCTRL0 |= (1 << 21);
    LCDIF->VDCTRL0 |= (1 << 24);                                                // enable极性为1， 高电平有效
    LCDIF->VDCTRL0 |= (0 << 25) | (0 << 26) | (0 << 27) | (1 < 28) | (0 << 29); //Bit25 设置 CLK 信号极性，设置为 0。
                                                        //bit26 设置 HSYNC 信号极性，设置 0，低电平有效，
                                                        //bit27 设置 VSYNC 信号极性，设置为 0，低电平有效。
                                                        //Bit28 设置 1，开始 ENABLE 信号。Bit29 设置为 0，
                                                        //VSYNC 输出。

    LCDIF->VDCTRL1 = tftlcd_dev.vspw + tftlcd_dev.vbpd + tftlcd_dev.height + tftlcd_dev.vfpd; // LCDIF_VDCTRL1 寄存器为两个 VSYNC 信号之间的长度
    LCDIF->VDCTRL2 = (tftlcd_dev.hspw + tftlcd_dev.hbpd + tftlcd_dev.width + tftlcd_dev.hfpd) | (tftlcd_dev.hspw << 18);

    LCDIF->VDCTRL3 = (tftlcd_dev.vspw + tftlcd_dev.vbpd) | (tftlcd_dev.hspw + tftlcd_dev.hbpd) << 16;

    LCDIF->VDCTRL4 = (tftlcd_dev.width) | (1 << 18);

    LCDIF->CUR_BUF = (unsigned int)tftlcd_dev.framebuffer;
    LCDIF->NEXT_BUF = (unsigned int)tftlcd_dev.framebuffer;

    lcd_enable();//开启lcd控制器
    delay_ms(20);
    lcd_clear(LCD_WHITE);
}

/* 像素时钟初始化
 * loopDiv 设置DIV_SELECT，范围27~54
 * prediv 设置为1~8，
 * div    可选范围1~8
 */
void lcdclk_init(unsigned char loopDiv, unsigned char prediv, unsigned char div)
{
    /* 不使用小数分频器 */
    CCM_ANALOG->PLL_VIDEO_NUM = 0;
    CCM_ANALOG->PLL_VIDEO_DENOM = 0;

    CCM_ANALOG->PLL_VIDEO = (1 << 13) | (2 << 19) | (loopDiv << 0);

    CCM_ANALOG->MISC2 &= ~(3 << 30);

    CCM->CSCDR2 &= ~(7 << 15);
    CCM->CSCDR2 |= (2 << 15);

    CCM->CSCDR2 &= ~(7 << 12);
    CCM->CSCDR2 |= (prediv - 1) << 12;

    CCM->CBCMR &= ~(7 << 23);
    CCM->CBCMR |= (div - 1) << 23;

    CCM->CSCDR2 &= ~(7 << 9);
    CCM->CSCDR2 |= 0 << 9;
}

/* 复位LCD控制器 */
void lcd_reset(void)
{
    LCDIF->CTRL |= 1 << 31;
}

/* 停止复位 */
void lcd_noreset(void)
{
    LCDIF->CTRL &= ~(1 << 31);
}

/* 使能LCD控制器 */
void lcd_enable(void)
{
    LCDIF->CTRL |= 1 << 0;
}

/*
 * 读取屏幕ID，
 * 描述: LCD_DATA23=R7(M0);LCD_DATA15=G7(M1);LCD_DATA07=B7(M2);
 *      M2:M1:M0
 *      0 :0 :0 //4.3寸480*272 RGB屏,ID=0X4342
 *      0 :0 :1 //7寸800*480 RGB屏,ID=0X7084
 *      0 :1 :0 //7寸1024*600 RGB屏,ID=0X7016
 *      1 :0 :1 //10.1寸1280*800,RGB屏,ID=0X1018
 *      1 :0 :0 //4.3寸800*480 RGB屏,ID=0X4384
 * @param      : 无
 * @return     : 屏幕ID
 */
/*读取屏幕id*/
unsigned short lcd_read_panelid(void)
{
    unsigned char idx;
    /* 打开模拟开关,设置LCD_VSYNC为高电平 */
    gpio_pin_config_t lcdio_config;
    IOMUXC_SetPinMux(IOMUXC_LCD_VSYNC_GPIO3_IO03, 0); /*复用为GPIO3_IO03*/
    IOMUXC_SetPinConfig(IOMUXC_LCD_VSYNC_GPIO3_IO03, 0X10b0);

    /* GPIO初始化 */
    lcdio_config.direction = kGPIO_DigitalOutput;
    lcdio_config.outputLogic = 1;
    gpio_init(GPIO3, 3, &lcdio_config);

    /* 读取屏幕ID */
    IOMUXC_SetPinMux(IOMUXC_LCD_DATA07_GPIO3_IO12, 0); /* B7(M2) */
    IOMUXC_SetPinMux(IOMUXC_LCD_DATA15_GPIO3_IO20, 0); /* G7(M1) */
    IOMUXC_SetPinMux(IOMUXC_LCD_DATA23_GPIO3_IO28, 0); /* R7(M0) */

    IOMUXC_SetPinConfig(IOMUXC_LCD_DATA07_GPIO3_IO12, 0xF080);
    IOMUXC_SetPinConfig(IOMUXC_LCD_DATA15_GPIO3_IO20, 0xF080);
    IOMUXC_SetPinConfig(IOMUXC_LCD_DATA23_GPIO3_IO28, 0xF080);

    lcdio_config.direction = kGPIO_DigitalInput;
    gpio_init(GPIO3, 12, &lcdio_config);
    gpio_init(GPIO3, 20, &lcdio_config);
    gpio_init(GPIO3, 28, &lcdio_config);

    idx = (unsigned char)gpio_PinRead(GPIO3, 28);       /*读取M0*/
    idx |= (unsigned char)gpio_PinRead(GPIO3, 20) << 1; /*读取M1*/
    idx |= (unsigned char)gpio_PinRead(GPIO3, 12) << 2; /*读取M2*/

    if (idx == 0)
        return ATK4342;
    else if (idx == 1)
        return ATK7084;
    else if (idx == 2)
        return ATK7016;
    else if (idx == 4)
        return ATK4384;
    else if (idx == 5)
        return ATK1018;
    return 0;
}

/*lcd io初始化*/
void lcdgpio_init()
{
    /* 1、IO初始化复用功能 */
    IOMUXC_SetPinMux(IOMUXC_LCD_DATA00_LCDIF_DATA00, 0);
    IOMUXC_SetPinMux(IOMUXC_LCD_DATA01_LCDIF_DATA01, 0);
    IOMUXC_SetPinMux(IOMUXC_LCD_DATA02_LCDIF_DATA02, 0);
    IOMUXC_SetPinMux(IOMUXC_LCD_DATA03_LCDIF_DATA03, 0);
    IOMUXC_SetPinMux(IOMUXC_LCD_DATA04_LCDIF_DATA04, 0);
    IOMUXC_SetPinMux(IOMUXC_LCD_DATA05_LCDIF_DATA05, 0);
    IOMUXC_SetPinMux(IOMUXC_LCD_DATA06_LCDIF_DATA06, 0);
    IOMUXC_SetPinMux(IOMUXC_LCD_DATA07_LCDIF_DATA07, 0);
    IOMUXC_SetPinMux(IOMUXC_LCD_DATA08_LCDIF_DATA08, 0);
    IOMUXC_SetPinMux(IOMUXC_LCD_DATA09_LCDIF_DATA09, 0);
    IOMUXC_SetPinMux(IOMUXC_LCD_DATA10_LCDIF_DATA10, 0);
    IOMUXC_SetPinMux(IOMUXC_LCD_DATA11_LCDIF_DATA11, 0);
    IOMUXC_SetPinMux(IOMUXC_LCD_DATA12_LCDIF_DATA12, 0);
    IOMUXC_SetPinMux(IOMUXC_LCD_DATA13_LCDIF_DATA13, 0);
    IOMUXC_SetPinMux(IOMUXC_LCD_DATA14_LCDIF_DATA14, 0);
    IOMUXC_SetPinMux(IOMUXC_LCD_DATA15_LCDIF_DATA15, 0);
    IOMUXC_SetPinMux(IOMUXC_LCD_DATA16_LCDIF_DATA16, 0);
    IOMUXC_SetPinMux(IOMUXC_LCD_DATA17_LCDIF_DATA17, 0);
    IOMUXC_SetPinMux(IOMUXC_LCD_DATA18_LCDIF_DATA18, 0);
    IOMUXC_SetPinMux(IOMUXC_LCD_DATA19_LCDIF_DATA19, 0);
    IOMUXC_SetPinMux(IOMUXC_LCD_DATA20_LCDIF_DATA20, 0);
    IOMUXC_SetPinMux(IOMUXC_LCD_DATA21_LCDIF_DATA21, 0);
    IOMUXC_SetPinMux(IOMUXC_LCD_DATA22_LCDIF_DATA22, 0);
    IOMUXC_SetPinMux(IOMUXC_LCD_DATA23_LCDIF_DATA23, 0);

    IOMUXC_SetPinMux(IOMUXC_LCD_CLK_LCDIF_CLK, 0);
    IOMUXC_SetPinMux(IOMUXC_LCD_ENABLE_LCDIF_ENABLE, 0);
    IOMUXC_SetPinMux(IOMUXC_LCD_HSYNC_LCDIF_HSYNC, 0);
    IOMUXC_SetPinMux(IOMUXC_LCD_VSYNC_LCDIF_VSYNC, 0);

    // 配置电气属性
    /* 2、配置LCD IO属性
     *bit 16:0 HYS关闭
     *bit [15:14]: 0 默认22K上拉
     *bit [13]: 0 pull功能
     *bit [12]: 0 pull/keeper使能
     *bit [11]: 0 关闭开路输出
     *bit [7:6]: 10 速度100Mhz
     *bit [5:3]: 111 驱动能力为R0/7
     *bit [0]: 1 高转换率
     */
    IOMUXC_SetPinConfig(IOMUXC_LCD_DATA00_LCDIF_DATA00, 0xB9);
    IOMUXC_SetPinConfig(IOMUXC_LCD_DATA01_LCDIF_DATA01, 0xB9);
    IOMUXC_SetPinConfig(IOMUXC_LCD_DATA02_LCDIF_DATA02, 0xB9);
    IOMUXC_SetPinConfig(IOMUXC_LCD_DATA03_LCDIF_DATA03, 0xB9);
    IOMUXC_SetPinConfig(IOMUXC_LCD_DATA04_LCDIF_DATA04, 0xB9);
    IOMUXC_SetPinConfig(IOMUXC_LCD_DATA05_LCDIF_DATA05, 0xB9);
    IOMUXC_SetPinConfig(IOMUXC_LCD_DATA06_LCDIF_DATA06, 0xB9);
    IOMUXC_SetPinConfig(IOMUXC_LCD_DATA07_LCDIF_DATA07, 0xB9);
    IOMUXC_SetPinConfig(IOMUXC_LCD_DATA08_LCDIF_DATA08, 0xB9);
    IOMUXC_SetPinConfig(IOMUXC_LCD_DATA09_LCDIF_DATA09, 0xB9);
    IOMUXC_SetPinConfig(IOMUXC_LCD_DATA10_LCDIF_DATA10, 0xB9);
    IOMUXC_SetPinConfig(IOMUXC_LCD_DATA11_LCDIF_DATA11, 0xB9);
    IOMUXC_SetPinConfig(IOMUXC_LCD_DATA12_LCDIF_DATA12, 0xB9);
    IOMUXC_SetPinConfig(IOMUXC_LCD_DATA13_LCDIF_DATA13, 0xB9);
    IOMUXC_SetPinConfig(IOMUXC_LCD_DATA14_LCDIF_DATA14, 0xB9);
    IOMUXC_SetPinConfig(IOMUXC_LCD_DATA15_LCDIF_DATA15, 0xB9);
    IOMUXC_SetPinConfig(IOMUXC_LCD_DATA16_LCDIF_DATA16, 0xB9);
    IOMUXC_SetPinConfig(IOMUXC_LCD_DATA17_LCDIF_DATA17, 0xB9);
    IOMUXC_SetPinConfig(IOMUXC_LCD_DATA18_LCDIF_DATA18, 0xB9);
    IOMUXC_SetPinConfig(IOMUXC_LCD_DATA19_LCDIF_DATA19, 0xB9);
    IOMUXC_SetPinConfig(IOMUXC_LCD_DATA20_LCDIF_DATA20, 0xB9);
    IOMUXC_SetPinConfig(IOMUXC_LCD_DATA21_LCDIF_DATA21, 0xB9);
    IOMUXC_SetPinConfig(IOMUXC_LCD_DATA22_LCDIF_DATA22, 0xB9);
    IOMUXC_SetPinConfig(IOMUXC_LCD_DATA23_LCDIF_DATA23, 0xB9);

    IOMUXC_SetPinConfig(IOMUXC_LCD_CLK_LCDIF_CLK, 0xB9);
    IOMUXC_SetPinConfig(IOMUXC_LCD_ENABLE_LCDIF_ENABLE, 0xB9);
    IOMUXC_SetPinConfig(IOMUXC_LCD_HSYNC_LCDIF_HSYNC, 0xB9);
    IOMUXC_SetPinConfig(IOMUXC_LCD_VSYNC_LCDIF_VSYNC, 0xB9);

    /* 背光GPIO初始化 */
    gpio_pin_config_t bl_config;
    bl_config.direction = kGPIO_DigitalOutput;
    bl_config.outputLogic = 1;
    gpio_init(GPIO1, 8, &bl_config);
}

/*打点函数*/
inline void lcd_drawpoint(unsigned short x, unsigned short y, unsigned int color)
{
    *(unsigned int *)((unsigned int)tftlcd_dev.framebuffer + tftlcd_dev.pixsize * (tftlcd_dev.width * y + x)) = color;

}

/*读点函数*/
inline unsigned int lcd_readpoint(unsigned short x, unsigned short y)
{
    return *(unsigned int *)((unsigned int)tftlcd_dev.framebuffer + tftlcd_dev.pixsize * (tftlcd_dev.width * y + x));

}

/*清屏函数*/
void lcd_clear(unsigned int color)
{
    unsigned int num ;
    unsigned int i ;
    //显存起始地址
    unsigned int *startaddr = (unsigned int*)tftlcd_dev.framebuffer;

    num = (unsigned int)(tftlcd_dev.height * tftlcd_dev.width);

    for(i = 0 ;i < num ;i++)
    {
        startaddr[i] = color;
    }

}


/*
 * @description   : 以指定的颜色填充一块矩形
 * @param - x0    : 矩形起始点坐标x轴
 * @param - y0    : 矩形起始点坐标y轴
 * @param - x1    : 矩形终止点坐标x轴
 * @param - y1    : 矩形终止点坐标y轴
 * @param - color : 要填充的颜色
 * @return        : 读取到的指定点的颜色值
 */
void lcd_fill(unsigned short x0, unsigned short y0,
              unsigned short x1, unsigned short y1, unsigned int color)
{
    unsigned short x, y;

    if(x0 < 0) x0 = 0;
    if(y0 < 0) y0 = 0;
    if(x1 >= tftlcd_dev.width) x1 = tftlcd_dev.width - 1;
    if(y1 >= tftlcd_dev.height) y1 = tftlcd_dev.height - 1;

    for(y = y0; y <= y1; y++)
    {
        for(x = x0; x <= x1; x++)
            lcd_drawpoint(x, y, color);
    }
}