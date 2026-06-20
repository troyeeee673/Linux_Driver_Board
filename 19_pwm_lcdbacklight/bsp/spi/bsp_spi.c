#include "bsp_spi.h"


/*初始化SPI*/
void spi_init(ECSPI_Type *base)
{
    base->CONREG = 0;//首先将控制寄存器清零
    base->CONREG |= (1 << 0)| (1 << 3) |(1 << 4)|(7 << 20);//bit0置一，使能spi, bit3置一，只要一有数据写入到 TXFIFO 发送缓冲区，ECSPI 就会立即自动开始发送，无需再额外通过软件去设置 XCH 位。
                                        //bit4置一，选择通道0为主模式, bit22:20置7，8bit触发
    
    base->CONFIGREG = 0;//清零的同时完成了置位

    base->PERIODREG = 0x2000;

    //SPI时钟（ICM20608 SPI最大只能到8MHz）, 将CLK 设置为6MHz
    base->CONREG &= ~((0xF << 12) | (0xF << 8));//将bit15：12 bit11：8清零
    base->CONREG |= (9 << 12);//一级10分频


}

/*spi读写函数*/
unsigned char spich0_readwrite_byte(ECSPI_Type *base, unsigned char txdata)
{
    uint32_t spirxdata = 0;
    uint32_t spitxdata = txdata;

    /*选择通道0*/
    base->CONREG &= ~(3 << 18);//清零bit21:18
    base->CONREG |= (0 << 18);//选择通道0

    /*数据发送*/
    while((base->STATREG & (1 << 0)) == 0);//等待发送数据寄存器不为0
    base->TXDATA = spitxdata;

    /*数据接收*/
    while(((base->STATREG) & (1 << 3)) == 0);
    spirxdata = base->RXDATA;
    return spirxdata;
}