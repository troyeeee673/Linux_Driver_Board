
.global _start @全局标号

_start:
    /*使能所有CCGR，开启所有外设时钟 */
    /*初始化CCGR0 */
    ldr r0, = 0x020c4068    @CCGR0地址
    ldr r1, = 0xffffffff    @要写入的值
    str r1, [r0]            @写入数据

    /*初始化CCGR1 */
    ldr r0, =0x020c406c    
    str r1, [r0]           

    /*初始化CCGR1 */
    ldr r0, =0x020c4070    
    str r1, [r0]           

    /*初始化CCGR1 */
    ldr r0, =0x020c4074    
    str r1, [r0]               

    /*初始化CCGR1 */
    ldr r0, =0x020c4078    
    str r1, [r0]           

    /*初始化CCGR1 */
    ldr r0, =0x020c407c    
    str r1, [r0]           

    /*初始化CCGR1 */
    ldr r0, =0x020c4080    
    str r1, [r0]           
    /*
    * 配置 GPIO1_IO03 PIN的复用为GPIO，也就是设置
    * IOMUXC_SW_MUX_CTL_PAD_GPIO1_IO03=5
    * IOMUXC_SW_MUX_CTL_PAD_GPIO1_IO03寄存器地址为0X020E0068
    */

    ldr r0, = 0X020E0068    
    ldr r1, = 0x00000101    
    str r1, [r0]            

    /* 配置GPIO1_IO03的电气属性 也就是寄存器:
    * IOMUXC_SW_PAD_CTL_PAD_GPIO1_IO03
    * IOMUXC_SW_PAD_CTL_PAD_GPIO1_IO03寄存器地址为0x020e02f4
    *
    * bit0:      0 低速率
    * bit5:3:    110 R0/6驱动能力
    * bit7:6:    10 100MHz速度
    * bit11:     0 关闭开路输出
    * bit12:     1 使能pull/kepper
    * bit13:     0 kepper
    * bit15:14:  00 100K下拉
    * bit16:     0 关闭hys
    */
    ldr r0, = 0x020e02f4
    ldr r1, = 0x10b0
    str r1, [r0]


    /* 设置GPIO
    * 设置GPIO1_GDIR寄存器，设置GPIO1_GPIO003为输出
    * GPIO1_GDIR寄存器地址为0x0209c004,设置GPIO1_GDIR寄存器bit3为1,
    * 也就是设置GPIO1_IO03为输出。
    */
    ldr r0, = 0x0209c004
    ldr r1, = 0x8
    str r1, [r0]

loop:
    b loop //执行完开灯后进入死循环，放置程序进入未知状态










