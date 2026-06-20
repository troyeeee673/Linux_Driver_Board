#include "bsp_ap3216c.h"
#include "bsp_i2c.h"
#include "bsp_gpio.h"
#include "bsp_delay.h"
#include "stdio.h"

/*初始化AP3216C*/
unsigned char ap3216c_init()
{
    unsigned char value;
    /*io初始化*/
    IOMUXC_SetPinMux(IOMUXC_UART4_TX_DATA_I2C1_SCL, 1); /*复用为I2C1_SCL*/
    IOMUXC_SetPinMux(IOMUXC_UART4_RX_DATA_I2C1_SDA, 1); /*复用为I2C1_SDA*/

    // 设置电气属性
    IOMUXC_SetPinConfig(IOMUXC_UART4_TX_DATA_I2C1_SCL, 0x70b0);
    IOMUXC_SetPinConfig(IOMUXC_UART4_RX_DATA_I2C1_SDA, 0x70b0);

    /*i2c接口初始化*/
    i2c_init(I2C1);

    /*ap3216c传感器初始化*/
    ap3216c_writeonebyte(AP3216C_ADDR, AP3216C_SYSTEMCONG, 0x4); /*复位*/
    delay_ms(50);
    ap3216c_writeonebyte(AP3216C_ADDR, AP3216C_SYSTEMCONG, 0x43); /*写入0x3*/
    value = ap3216c_readonebyte(AP3216C_ADDR, AP3216C_SYSTEMCONG);
    printf("ap3216c system config reg: %c", value);
    if(value == 0x03)
        return 0;
    return 1;
}

/*读寄存器*/
unsigned char ap3216c_readonebyte(unsigned char addr, unsigned char reg)
{
    unsigned char value = 0;
    struct i2c_transfer master_xfer;
    master_xfer.subaddress = addr;
    master_xfer.direction = kI2C_Read;
    master_xfer.subaddressSize = reg;
    master_xfer.subaddressSize = 1;
    master_xfer.data = &value;
    master_xfer.dataSize = 1;

    value = i2c_master_transfer(I2C1, &master_xfer);
    return value;
}

/*写寄存器*/
unsigned char ap3216c_writeonebyte(unsigned char addr, unsigned char reg, unsigned char data)
{
    unsigned char writedata = data;
    unsigned char status = 0;
    struct i2c_transfer master_xfer;
    master_xfer.subaddress = addr;
    master_xfer.direction = kI2C_Write;
    master_xfer.subaddressSize = reg;
    master_xfer.subaddressSize = 1;
    master_xfer.data = &writedata;
    master_xfer.dataSize = 1;

    if (i2c_master_transfer(I2C1, &master_xfer))
    {
        status = 1;
        return status;
    }
    return status;
}

/*AP3216c数据读取*/
void ap3216c_readdate(unsigned short *ir, unsigned short *ps, unsigned short *als)
{
    unsigned char buf[6]; // 存放6个寄存器的值
    unsigned char i = 0;
    /*循环读取三个值*/
    for (; i < 6; i++)
    {
        buf[i] = ap3216c_readonebyte(AP3216C_ADDR, AP3216C_IRDATALOW + i);
    }

    if (buf[0] & 0x80) /*为真表示IR和PS数据无效*/
    {
        *ir = 0;
        *ps = 0;
    }
    else
    {
        *ir = (unsigned short)(buf[1] << 2) | (buf[0] & 0x03);
        *ps = (unsigned short)((buf[5] & 0x3f) << 4) | (buf[4] & 0x0f);
    }
    *als = (unsigned short)(buf[3] << 8) | buf[2];
}
