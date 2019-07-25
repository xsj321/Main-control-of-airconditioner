/********************   (C) COPYRIGHT 2013 www.armjishu.com   ********************
 * 文件名  ：SZ_STM32F107VC_LIB.c
 * 描述    ：提供STM32F107VC神舟IV号开发板的库函数
 * 实验平台：STM32神舟开发板
 * 作者    ：www.armjishu.com 
**********************************************************************************/
  
/* Includes ------------------------------------------------------------------*/
#include "SZ_STM32F107VC_LIB.h"

__IO uint32_t TimingDelay;

GPIO_TypeDef* GPIO_PORT[LEDn] = {LED1_GPIO_PORT, LED2_GPIO_PORT, LED3_GPIO_PORT, LED4_GPIO_PORT};
const uint16_t GPIO_PIN_NUM[LEDn] = {LED1_PIN_NUM, LED2_PIN_NUM, LED3_PIN_NUM, LED4_PIN_NUM};
const uint16_t GPIO_PIN[LEDn] = {LED1_PIN, LED2_PIN, LED3_PIN, LED4_PIN};
const uint32_t GPIO_CLK[LEDn] = {LED1_GPIO_CLK, LED2_GPIO_CLK, LED3_GPIO_CLK, LED4_GPIO_CLK};

GPIO_TypeDef* BUTTON_PORT[BUTTONn] = {KEY1_BUTTON_GPIO_PORT, KEY2_BUTTON_GPIO_PORT, KEY3_BUTTON_GPIO_PORT, KEY4_BUTTON_GPIO_PORT}; 
const uint16_t BUTTON_PIN_NUM[BUTTONn] = {KEY1_BUTTON_PIN_NUM, KEY2_BUTTON_PIN_NUM, KEY3_BUTTON_PIN_NUM, KEY4_BUTTON_PIN_NUM}; 
const uint16_t BUTTON_PIN[BUTTONn] = {KEY1_BUTTON_PIN, KEY2_BUTTON_PIN, KEY3_BUTTON_PIN, KEY4_BUTTON_PIN}; 
const uint32_t BUTTON_CLK[BUTTONn] = {KEY1_BUTTON_GPIO_CLK, KEY2_BUTTON_GPIO_CLK, KEY3_BUTTON_GPIO_CLK, KEY4_BUTTON_GPIO_CLK};
const uint16_t BUTTON_EXTI_LINE[BUTTONn] = {KEY1_BUTTON_EXTI_LINE, KEY2_BUTTON_EXTI_LINE, KEY3_BUTTON_EXTI_LINE, KEY4_BUTTON_EXTI_LINE};
const uint16_t BUTTON_PORT_SOURCE[BUTTONn] = {KEY1_BUTTON_EXTI_PORT_SOURCE, KEY2_BUTTON_EXTI_PORT_SOURCE, KEY3_BUTTON_EXTI_PORT_SOURCE, KEY4_BUTTON_EXTI_PORT_SOURCE};
const uint16_t BUTTON_PIN_SOURCE[BUTTONn] = {KEY1_BUTTON_EXTI_PIN_SOURCE, KEY2_BUTTON_EXTI_PIN_SOURCE, KEY3_BUTTON_EXTI_PIN_SOURCE, KEY4_BUTTON_EXTI_PIN_SOURCE}; 
const uint16_t BUTTON_IRQn[BUTTONn] = {KEY1_BUTTON_EXTI_IRQn, KEY2_BUTTON_EXTI_IRQn, KEY3_BUTTON_EXTI_IRQn, KEY4_BUTTON_EXTI_IRQn};

USART_TypeDef* COM_USART[COMn] = {SZ_STM32_COM1, SZ_STM32_COM2}; 
GPIO_TypeDef* COM_TX_PORT[COMn] = {SZ_STM32_COM1_TX_GPIO_PORT, SZ_STM32_COM2_TX_GPIO_PORT};
GPIO_TypeDef* COM_RX_PORT[COMn] = {SZ_STM32_COM1_RX_GPIO_PORT, SZ_STM32_COM2_RX_GPIO_PORT};
const uint32_t COM_USART_CLK[COMn] = {SZ_STM32_COM1_CLK, SZ_STM32_COM2_CLK};
const uint32_t COM_TX_PORT_CLK[COMn] = {SZ_STM32_COM1_TX_GPIO_CLK, SZ_STM32_COM2_TX_GPIO_CLK};
const uint32_t COM_RX_PORT_CLK[COMn] = {SZ_STM32_COM1_RX_GPIO_CLK, SZ_STM32_COM2_RX_GPIO_CLK};
const uint16_t COM_TX_PIN[COMn] = {SZ_STM32_COM1_TX_PIN, SZ_STM32_COM2_TX_PIN};
const uint16_t COM_RX_PIN[COMn] = {SZ_STM32_COM1_RX_PIN, SZ_STM32_COM2_RX_PIN};

const uint8_t STM32F10x_STR[] = {"\r\n"\
           "  _____ _______ __  __ ____ ___  ______ __  ___\r\n"\
           " / ____|__   __|  \\/  |___ \\__ \\|  ____/_ |/ _ \\\r\n"\
           "| (___    | |  | \\  / | __) | ) | |__   | | | | |_  __\r\n"\
           " \\___ \\   | |  | |\\/| ||__ < / /|  __|  | | | | \\ \\/ /\r\n"\
           " ____) |  | |  | |  | |___) / /_| |     | | |_| |>  <\r\n"\
           "|_____/   |_|  |_|  |_|____/____|_|     |_|\\___//_/\\_\\神舟系列开发板\r\n"\
           "\r\n"};

					 
uint32_t STM32DeviceSerialID[3]; /* 全局变量IntDeviceSerial存放读到的设备ID */

/**-------------------------------------------------------
  * @函数名 delay
  * @功能   简单的delay延时函数.
  * @参数   延迟周期数 0--0xFFFFFFFF
  * @返回值 无
***------------------------------------------------------*/
void delay(__IO uint32_t nCount)
{
    for (; nCount != 0; nCount--);
}

/**-------------------------------------------------------
  * @函数名 NVIC_GroupConfig
  * @功能   配置NVIC中断优先级分组函数.
  *         默认配置为1比特表示主优先级, 3比特表示次优先级
  *         用户可以根据需要修改
  * @参数   无
  * @返回值 无
***------------------------------------------------------*/
void NVIC_GroupConfig(void)
{
    /* 配置NVIC中断优先级分组:
     - 1比特表示主优先级  主优先级合法取值为 0 或 1 
     - 3比特表示次优先级  次优先级合法取值为 0..7
     - 数值越低优先级越高，取值超过合法范围时取低bit位 
    */
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);

    /*==================================================================================
      NVIC_PriorityGroup   |  主优先级范围  |  次优先级范围  |   描述
      ==================================================================================
     NVIC_PriorityGroup_0  |      0         |      0-15      |   0 比特表示主优先级
                           |                |                |   4 比特表示次优先级 
     ----------------------------------------------------------------------------------
     NVIC_PriorityGroup_1  |      0-1       |      0-7       |   1 比特表示主优先级
                           |                |                |   3 比特表示次优先级 
     ----------------------------------------------------------------------------------
     NVIC_PriorityGroup_2  |      0-3       |      0-3       |   2 比特表示主优先级
                           |                |                |   2 比特表示次优先级 
     ----------------------------------------------------------------------------------
     NVIC_PriorityGroup_3  |      0-7       |      0-1       |   3 比特表示主优先级
                           |                |                |   1 比特表示次优先级 
     ----------------------------------------------------------------------------------
     NVIC_PriorityGroup_4  |      0-15      |      0         |   4 比特表示主优先级
                           |                |                |   0 比特表示次优先级   
    ==================================================================================*/
}

/**-------------------------------------------------------
  * @函数名 SZ_STM32_SysTickInit
  * @功能   初始化系统定时器SysTick
  *         用户可以根据需要修改
  * @参数   每秒中断次数
  * @返回值 无
***------------------------------------------------------*/
void SZ_STM32_SysTickInit(uint32_t HzPreSecond)
{
    /* HzPreSecond = 1000 to Setup SysTick Timer for 1 msec interrupts.
     ------------------------------------------
    1. The SysTick_Config() function is a CMSIS function which configure:
       - The SysTick Reload register with value passed as function parameter.
       - Configure the SysTick IRQ priority to the lowest value (0x0F).
       - Reset the SysTick Counter register.
       - Configure the SysTick Counter clock source to be Core Clock Source (HCLK).
       - Enable the SysTick Interrupt.
       - Start the SysTick Counter.
    
    2. You can change the SysTick Clock source to be HCLK_Div8 by calling the
       SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8) just after the
       SysTick_Config() function call. The SysTick_CLKSourceConfig() is defined
       inside the misc.c file.

    3. You can change the SysTick IRQ priority by calling the
       NVIC_SetPriority(SysTick_IRQn,...) just after the SysTick_Config() function 
       call. The NVIC_SetPriority() is defined inside the core_cm3.h file.

    4. To adjust the SysTick time base, use the following formula:
                            
         Reload Value = SysTick Counter Clock (Hz) x  Desired Time base (s)
       - Reload Value is the parameter to be passed for SysTick_Config() function
       - Reload Value should not exceed 0xFFFFFF
    */
    if (SysTick_Config(SystemCoreClock / HzPreSecond))
    { 
        /* Capture error */ 
        while (1);
    }
}

/**-------------------------------------------------------
  * @函数名 SysTickDelay
  * @功能   系统定时器实现的延时函数，可以在用户程序中调用
  *         用户可以根据需要修改
  * @参数   延迟周期数，延迟时间为nTime/HzPreSecond
  *         误差为 -1/HzPreSecond
  * @返回值 无
***------------------------------------------------------*/
void SysTickDelay(__IO uint32_t nTime)
{ 
    TimingDelay = nTime;

    while(TimingDelay != 0);
}

/**-------------------------------------------------------
  * @函数名 TimingDelay_Decrement
  * @功能   系统节拍定时器服务函数调用的子函数
  *         将全局变量TimingDelay减一，用于实现延时
  * @参数   无
  * @返回值 无
***------------------------------------------------------*/
void TimingDelay_Decrement(void)
{
    if (TimingDelay != 0x00)
    { 
        TimingDelay--;
    }
}

extern void SysTick_Handler_User(void);
/**-------------------------------------------------------
  * @函数名 SysTick_Handler
  * @功能   系统节拍定时器服务请求处理函数
  * @参数   无
  * @返回值 无
***------------------------------------------------------*/
void SysTick_Handler(void)
{
    TimingDelay_Decrement();

    SysTick_Handler_User();
}

void GetDeviceSerialID(void)
{
    STM32DeviceSerialID[0] = *(__IO uint32_t*)(0x1FFFF7E8);
    STM32DeviceSerialID[1] = *(__IO uint32_t*)(0x1FFFF7EC);
    STM32DeviceSerialID[2] = *(__IO uint32_t*)(0x1FFFF7F0);
}

/**-------------------------------------------------------
  * @函数名 SZ_STM32_LEDInit
  * @功能   初始化LED的GPIO管脚，配置为推挽输出
  * @参数   LED1  对应开发板上第一个指示灯
  *         LED2  对应开发板上第二个指示灯
  *         LED3  对应开发板上第三个指示灯
  *         LED4  对应开发板上第四个指示灯
  * @返回值 无
***------------------------------------------------------*/
void SZ_STM32_LEDInit(Led_TypeDef Led)
{
    GPIO_InitTypeDef  GPIO_InitStructure;

    /* Enable the GPIO_LED Clock */
    /* 使能LED对应GPIO的Clock时钟 */
    RCC_APB2PeriphClockCmd(GPIO_CLK[Led], ENABLE);

    /* Configure the GPIO_LED pin */
    /* 初始化LED的GPIO管脚，配置为推挽输出 */
    GPIO_InitStructure.GPIO_Pin = GPIO_PIN[Led];
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

    GPIO_Init(GPIO_PORT[Led], &GPIO_InitStructure);
}

/**-------------------------------------------------------
  * @函数名 SZ_STM32_LEDOn
  * @功能   点亮对应的LED指示灯
  * @参数   LED1  对应开发板上第一个指示灯
  *         LED2  对应开发板上第二个指示灯
  *         LED3  对应开发板上第三个指示灯
  *         LED4  对应开发板上第四个指示灯
  * @返回值 无
***------------------------------------------------------*/
void SZ_STM32_LEDOn(Led_TypeDef Led)
{
    /* 指定管脚输出低电平，点亮对应的LED指示灯 */
    GPIO_PORT[Led]->BRR = GPIO_PIN[Led];  
    //若要提高效率，建议直接调用 LEDnOBB = 0;
}

/**-------------------------------------------------------
  * @函数名 SZ_STM32_LEDOn
  * @功能   熄灭对应的LED指示灯
  * @参数   LED1  对应开发板上第一个指示灯
  *         LED2  对应开发板上第二个指示灯
  *         LED3  对应开发板上第三个指示灯
  *         LED4  对应开发板上第四个指示灯
  * @返回值 无
***------------------------------------------------------*/
void SZ_STM32_LEDOff(Led_TypeDef Led)
{
    /* 指定管脚输出高电平，熄灭对应的LED指示灯 */
    GPIO_PORT[Led]->BSRR = GPIO_PIN[Led];   
    //若要提高效率，建议直接调用 LEDnOBB = 1;
}

/**-------------------------------------------------------
  * @函数名 SZ_STM32_LEDToggle
  * @功能   将对应的LED指示灯状态取反
  * @参数   LED1  对应开发板上第一个指示灯
  *         LED2  对应开发板上第二个指示灯
  *         LED3  对应开发板上第三个指示灯
  *         LED4  对应开发板上第四个指示灯
  * @返回值 无
***------------------------------------------------------*/
void SZ_STM32_LEDToggle(Led_TypeDef Led)
{
    /* 指定管脚输出异或 1，实现对应的LED指示灯状态取反目的 */
    GPIO_PORT[Led]->ODR ^= GPIO_PIN[Led];
	//若要提高效率，建议直接调用 LEDnOBB = !LEDnOBB;
}


/**-------------------------------------------------------
  * @函数名 SZ_STM32_BEEPInit
  * @功能   初始化蜂鸣器的GPIO管脚，配置为推挽输出
  * @参数   无
  * @返回值 无
***------------------------------------------------------*/
void SZ_STM32_BEEPInit(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;

    /* Enable the GPIO_LED Clock */
    /* 使能蜂鸣器对应GPIO的Clock时钟 */
    RCC_APB2PeriphClockCmd(BEEP_GPIO_CLK, ENABLE);

    /* Configure the GPIO_LED pin */
    /* 初始化蜂鸣器的GPIO管脚，配置为推挽输出 */
    GPIO_InitStructure.GPIO_Pin = BEEP_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

    GPIO_Init(BEEP_GPIO_PORT, &GPIO_InitStructure);

    BEEPOBB = 1;//初始化时关闭蜂鸣器
}

/**-------------------------------------------------------
  * @函数名 SZ_STM32_BEEPOn
  * @功能   使蜂鸣器开始鸣响
  * @参数   无
  * @返回值 无
***------------------------------------------------------*/
void SZ_STM32_BEEPOn(void)
{
    /* 指定管脚输出低电平，使蜂鸣器开始鸣响 */
    //BEEP_GPIO_PORT->BRR = BEEP_PIN;  
    BEEPOBB = 0;
}

/**-------------------------------------------------------
  * @函数名 SZ_STM32_BEEPOff
  * @功能   使蜂鸣器停止鸣响
  * @参数   无
  * @返回值 无
***------------------------------------------------------*/
void SZ_STM32_BEEPOff(void)
{
    /* 指定管脚输出高电平，使蜂鸣器停止鸣响 */
    //BEEP_GPIO_PORT->BSRR = BEEP_PIN;   
    BEEPOBB = 1;
}

/**-------------------------------------------------------
  * @函数名 SZ_STM32_BEEPToggle
  * @功能   使蜂鸣器状态取反
  * @参数   无
  * @返回值 无
***------------------------------------------------------*/
void SZ_STM32_BEEPToggle(void)
{
    /* 指定管脚输出异或 1，实现对蜂鸣器状态取反目的 */
    //BEEP_GPIO_PORT->ODR ^= BEEP_PIN;
    BEEPOBB = !BEEPOBB;
}


/**-------------------------------------------------------
  * @函数名 SZ_STM32_KEYInit
  * @功能   初始化KEY按键的GPIO管脚，配置为带上拉的输入
  *         按键按下时为低电平0
  * @参数1  KEY1  对应开发板上第一个KEY按键
  *         KEY2  对应开发板上第二个KEY按键
  *         KEY3  对应开发板上第三个KEY按键
  *         KEY4  对应开发板上第四个KEY按键
  * @参数2  BUTTON_MODE_GPIO  配置为普通输入管脚使用
  *         BUTTON_MODE_EXTI  配置为外部EXTI中断使用
  *                           需要完善中断处理函数
  * @返回值 无
***------------------------------------------------------*/
void SZ_STM32_KEYInit(Button_TypeDef Button, ButtonMode_TypeDef Button_Mode)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    EXTI_InitTypeDef EXTI_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    /* Enable the BUTTON Clock */
    /* 使能KEY按键对应GPIO的Clock时钟 */
    RCC_APB2PeriphClockCmd(BUTTON_CLK[Button] | RCC_APB2Periph_AFIO, ENABLE);

    /* Configure Button pin as input floating */
    /* 初始化KEY按键的GPIO管脚，配置为带上拉的输入 */
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_InitStructure.GPIO_Pin = BUTTON_PIN[Button];
    GPIO_Init(BUTTON_PORT[Button], &GPIO_InitStructure);

    /* 初始化KEY按键为中断模式 */
    if (Button_Mode == BUTTON_MODE_EXTI)
    {
        /* Connect Button EXTI Line to Button GPIO Pin */
        /* 将KEY按键对应的管脚连接到内部中断线 */    
        GPIO_EXTILineConfig(BUTTON_PORT_SOURCE[Button], BUTTON_PIN_SOURCE[Button]);

        /* Configure Button EXTI line */
        /* 将KEY按键配置为中断模式，下降沿触发中断 */    
        EXTI_InitStructure.EXTI_Line = BUTTON_EXTI_LINE[Button];
        EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
        EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;  
        EXTI_InitStructure.EXTI_LineCmd = ENABLE;
        EXTI_Init(&EXTI_InitStructure);

        /* Enable and set Button EXTI Interrupt to the lowest priority */
        /* 将KEY按键的中断优先级配置为最低 */  
        NVIC_InitStructure.NVIC_IRQChannel = BUTTON_IRQn[Button];
        NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0F;
        NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0F;
        NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
        NVIC_Init(&NVIC_InitStructure); 
    }
}

/**-------------------------------------------------------
  * @函数名 SZ_STM32_KEYGetState
  * @功能   获取KEY按键的输入电平状态，按键按下时为低电平0
  * @参数   KEY1  对应开发板上第一个KEY按键
  *         KEY2  对应开发板上第二个KEY按键
  *         KEY3  对应开发板上第三个KEY按键
  *         KEY4  对应开发板上第四个KEY按键
  * @返回值 无
***------------------------------------------------------*/
uint32_t SZ_STM32_KEYGetState(Button_TypeDef Button)
{
    return GPIO_ReadInputDataBit(BUTTON_PORT[Button], BUTTON_PIN[Button]);
	//若要提高效率，建议直接调用 KeyState = KEYnIBB;
}

/**-------------------------------------------------------
  * @函数名 SZ_STM32_KEYScan
  * @功能   获取KEY按键的输入电平状态，按键按下时为低电平0
  * @参数   无
  * @返回值 1  对应开发板上第一个KEY按键处于按下状态
  *         2  对应开发板上第二个KEY按键处于按下状态
  *         3  对应开发板上第三个KEY按键处于按下状态
  *         4  对应开发板上第四个KEY按键处于按下状态
  *         0  开发板上没有KEY按键处于按下状态
***------------------------------------------------------*/
uint32_t SZ_STM32_KEYScan(void)
{
    /* 获取KEY按键的输入电平状态，按键按下时为低电平0 */
    if(0 == KEY1IBB)
    {
         /* 延迟去抖 */
        delay(150000);
        if(0 == KEY1IBB)
        {
            return 1;
        }
    }

    /* 获取KEY按键的输入电平状态，按键按下时为低电平0 */
    if(0 == KEY2IBB)
    {
         /* 延迟去抖 */
        delay(150000);
        if(0 == KEY2IBB)
        {
            return 2;
        }
    }

    /* 获取KEY按键的输入电平状态，按键按下时为低电平0 */
    if(0 == KEY3IBB)
    {
         /* 延迟去抖 */
        delay(150000);
        if(0 == KEY3IBB)
        {
            return 3;
        }
    }

    /* 获取KEY按键的输入电平状态，按键按下时为低电平0 */
    if(0 == KEY4IBB)
    {
         /* 延迟去抖 */
        delay(150000);
        if(0 == KEY4IBB)
        {
            return 4;
        }
    }
    
    return 0;
}


/**-------------------------------------------------------
  * @函数名 __SZ_STM32_COMInit
  * @功能   对STM32的USART初始化底层函数
  * @参数1  COM1  对应STM32的USART1 对应开发板上串口1
  *         COM2  对应STM32的USART2 对应开发板上串口2
  * @参数2  指向一个成员已赋值USART_InitTypeDef结构体的指针
  * @返回值 无
***------------------------------------------------------*/
void __SZ_STM32_COMInit(COM_TypeDef COM, USART_InitTypeDef* USART_InitStruct)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    /* Enable GPIO clock */
    /* 使能STM32的USART对应GPIO的Clock时钟 */
    RCC_APB2PeriphClockCmd(COM_TX_PORT_CLK[COM] | COM_RX_PORT_CLK[COM] | RCC_APB2Periph_AFIO, ENABLE);

    if (COM == COM1)
    {
        /* 使能STM32的USART1的Clock时钟 */
        RCC_APB2PeriphClockCmd(COM_USART_CLK[COM], ENABLE); 
    }
    else
    {
        /* Enable the USART2 Pins Software Remapping */
        /* 使能STM32的USART2的管脚重映射 */
        GPIO_PinRemapConfig(GPIO_Remap_USART2, ENABLE);
        /* 使能STM32的USART的Clock时钟 */
        RCC_APB1PeriphClockCmd(COM_USART_CLK[COM], ENABLE);
    }

    /* Configure USART Tx as alternate function push-pull */
    /* 初始化STM32的USART的TX管脚，配置为复用功能推挽输出 */
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Pin = COM_TX_PIN[COM];
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(COM_TX_PORT[COM], &GPIO_InitStructure);

    /* Configure USART Rx as input floating */
    /* 初始化STM32的USART的RX管脚，配置为复用功能输入 */
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_InitStructure.GPIO_Pin = COM_RX_PIN[COM];
    GPIO_Init(COM_RX_PORT[COM], &GPIO_InitStructure);

    /* USART configuration */
    /* 根据传入的参数初始化STM32的USART配置 */
    USART_Init(COM_USART[COM], USART_InitStruct);

    /* Enable USART */
    /* 使能STM32的USART功能模块 */
    USART_Cmd(COM_USART[COM], ENABLE);
}

/**-------------------------------------------------------
  * @函数名 __SZ_STM32_COMInit
  * @功能   面向用户的STM32的USART初始化函数
  * @参数1  COM1  对应STM32的USART1 对应开发板上串口1
  *         COM2  对应STM32的USART2 对应开发板上串口2
  * @参数2  BaudRate 串口的波特率，例如"115200"
  * @返回值 无
***------------------------------------------------------*/
void SZ_STM32_COMInit(COM_TypeDef COM, uint32_t BaudRate)
{
  
    USART_InitTypeDef USART_InitStructure;
  
    /* USARTx 默认配置:
          - BaudRate = 115200 baud  
          - Word Length = 8 Bits
          - One Stop Bit
          - No parity
          - Hardware flow control disabled (RTS and CTS signals)
          - Receive and transmit enabled
    */
    USART_InitStructure.USART_BaudRate = BaudRate;              //串口的波特率，例如115200 最高达4.5Mbits/s
    USART_InitStructure.USART_WordLength = USART_WordLength_8b; //数据字长度(8位或9位)
    USART_InitStructure.USART_StopBits = USART_StopBits_1;      //可配置的停止位-支持1或2个停止位
    USART_InitStructure.USART_Parity = USART_Parity_No;         //无奇偶校验  
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //无硬件流控制
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; //双工模式，使能发送和接收
  
    __SZ_STM32_COMInit(COM, &USART_InitStructure);  // 调用STM32的USART初始化底层函数
  
    printf("\r\n\n\n\r WWW.ARMJISHU.COM  %s printf configured....", 
  	                            (COM == COM1)? SZ_STM32_COM1_STR:SZ_STM32_COM2_STR);
    printf("\n\r ############ WWW.ARMJISHU.COM! ############ ("__DATE__ " - " __TIME__ ")");
  
    printf("%s", STM32F10x_STR);
  
    printf(" WWW.ARMJISHU.COM use __STM32F10X_STDPERIPH_VERSION %d.%d.%d",
  			__STM32F10X_STDPERIPH_VERSION_MAIN,
  			__STM32F10X_STDPERIPH_VERSION_SUB1,
  			__STM32F10X_STDPERIPH_VERSION_SUB2);
    printf("\n\r 产品内部Flash大小为：%dK字节！ \t www.armjishu.com",
              *(__IO uint16_t*)(0x1FFFF7E0));
    SystemCoreClockUpdate();
    printf("\n\r 系统内核时钟频率(SystemCoreClock)为：%dHz.\n\r",
            SystemCoreClock);
}


/*
    加入以下代码,支持printf函数,不需要选择use MicroLIB	  
*/
#ifndef MicroLIB
//#pragma import(__use_no_semihosting)             //没有实现fgetc时需要声明该参数   
/* 标准库需要的支持函数 使用printf()调试打印不需要实现该函数 */               
struct __FILE 
{ 
	int handle; 
    /* Whatever you require here. If the only file you are using is */    
    /* standard output using printf() for debugging, no file handling */    
    /* is required. */
}; 

FILE __stdout;       
//定义_sys_exit()以避免使用半主机模式    
_sys_exit(int x) 
{ 
	x = x; 
} 
/* 重定义fputc函数 如果使用MicroLIB只需要重定义fputc函数即可 */  
int fputc(int ch, FILE *f)
{
    /* Place your implementation of fputc here */
    /* Loop until the end of transmission */
    while (USART_GetFlagStatus(SZ_STM32_COM2, USART_FLAG_TC) == RESET)
    {}

    /* e.g. write a character to the USART */
    USART_SendData(SZ_STM32_COM2, (uint8_t) ch);

    return ch;
}
/*
可以直接使用putchar
不需要再定义 int putchar(int ch)，因为stdio.h中有如下定义
 #define putchar(c) putc(c, stdout)
*/

int ferror(FILE *f) {  
    /* Your implementation of ferror */  
    return EOF;  
} 
#endif 

FILE __stdin;

int fgetc(FILE *fp)
{
	int ch = 0;
	
    while(USART_GetFlagStatus(SZ_STM32_COM2, USART_FLAG_RXNE) == RESET)
    {
    }

    ch = (int)SZ_STM32_COM2->DR & 0xFF;
	
    putchar(ch); //回显
	
	return ch;
}

#ifdef USE_ADC
/**-------------------------------------------------------
  * @函数名 ADC_GPIO_Configuration
  * @功能   ADC模数转换的GPIO配置初始化函数
  * @参数   无
  * @返回值 无
***------------------------------------------------------*/
void ADC_GPIO_Configuration(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    /* Enable GPIOC clock */
    /* 使能GPIOC时钟      */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);	  //

    /* Configure PA.01 (ADC Channel12) as analog input -------------------------*/
    //PC0 作为模拟通道11输入引脚                         
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;       //管脚0
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;   //输入模式
    GPIO_Init(GPIOC, &GPIO_InitStructure);          //GPIO组
}

/* STM32芯片ADC转换结果DR寄存器基地址 */
#define DR_ADDRESS                  ((uint32_t)0x4001244C) 
/* 存放ADC为12位模数转换器结果的变量，只有ADCConvertedValue的低12位有效 */
__IO uint16_t ADCConvertedValue;     // 

/**-------------------------------------------------------
  * @函数名 SZ_STM32_ADC_Configuration
  * @功能   ADC模数转换的参数配置函数
  * @参数   无
  * @返回值 无
***------------------------------------------------------*/
void SZ_STM32_ADC_Configuration(void)
{
    ADC_InitTypeDef ADC_InitStructure;        //ADC初始化结构体声明
    DMA_InitTypeDef DMA_InitStructure;        //DMA初始化结构体声明     

    ADC_GPIO_Configuration();

    /* Enable DMA1 clock */
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);		 //使能DMA时钟

    /* DMA1 channel1 configuration ----------------------------------------------*/
    DMA_DeInit(DMA1_Channel1);		  //开启DMA1的第一通道
    DMA_InitStructure.DMA_PeripheralBaseAddr = DR_ADDRESS;		  //DMA对应的外设基地址
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)&ADCConvertedValue;   //内存存储基地址
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;	//DMA的转换模式为SRC模式，由外设搬移到内存
    DMA_InitStructure.DMA_BufferSize = 1;		   //DMA缓存大小，1个
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;	//接收一次数据后，设备地址禁止后移
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Disable;	//关闭接收一次数据后，目标内存地址后移
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;  //定义外设数据宽度为16位
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;  //DMA搬移数据尺寸，HalfWord就是为16位
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;   //转换模式，循环缓存模式。
    DMA_InitStructure.DMA_Priority = DMA_Priority_High;	//DMA优先级高
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;		  //M2M模式禁用
    DMA_Init(DMA1_Channel1, &DMA_InitStructure);          
    /* Enable DMA1 channel1 */
    DMA_Cmd(DMA1_Channel1, ENABLE);

    /* Enable ADC1 and GPIOC clock */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);	  //使能ADC时钟

    /* ADC1 configuration ------------------------------------------------------*/
    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;		//独立的转换模式
    ADC_InitStructure.ADC_ScanConvMode = ENABLE;		  //开启扫描模式
    ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;   //开启连续转换模式
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	//ADC外部开关，关闭状态
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;   //对齐方式,ADC为12位中，右对齐方式
    ADC_InitStructure.ADC_NbrOfChannel = 1;	 //开启通道数，1个
    ADC_Init(ADC1, &ADC_InitStructure);

    /* ADC1 regular channel10 configuration */ 
    ADC_RegularChannelConfig(ADC1, ADC_Channel_10, 1, ADC_SampleTime_55Cycles5);
                          //ADC通道组， 第1个通道 采样顺序1，转换时间 
    /* Enable ADC1 DMA */
    ADC_DMACmd(ADC1, ENABLE);	  //ADC命令，使能
    /* Enable ADC1 */
    ADC_Cmd(ADC1, ENABLE);  //开启ADC1

    /* Enable ADC1 reset calibaration register */   
    ADC_ResetCalibration(ADC1);	  //重新校准
    /* Check the end of ADC1 reset calibration register */
    while(ADC_GetResetCalibrationStatus(ADC1));  //等待重新校准完成
    /* Start ADC1 calibaration */
    ADC_StartCalibration(ADC1);		//开始校准
    /* Check the end of ADC1 calibration */
    while(ADC_GetCalibrationStatus(ADC1));	   //等待校准完成
    /* Start ADC1 Software Conversion */ 
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);	//连续转换开始，ADC通过DMA方式不断的更新RAM区。
}

/**-------------------------------------------------------
  * @函数名 GetADCConvertedValue
  * @功能   获取ADC模数转换的结果
  * @参数   无
  * @返回值 ADC转换结果的变量，只有的低12位有效
***------------------------------------------------------*/
uint16_t GetADCConvertedValue(void)
{
    return ADCConvertedValue;
}
#endif

#ifdef USE_RTC
/**-------------------------------------------------------
  * @函数名 RTC_Configuration
  * @功能   RTC实时时钟的参数配置函数
  * @参数   无
  * @返回值 无
***------------------------------------------------------*/
void RTC_Configuration(void)
{
    /* Enable PWR and BKP clocks */
    /* PWR时钟（电源控制）与BKP时钟（RTC后备寄存器）使能 */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);

    /* Allow access to BKP Domain */
    /*使能RTC和后备寄存器访问 */
    PWR_BackupAccessCmd(ENABLE);

    /* Reset Backup Domain */
    /* 将外设BKP的全部寄存器重设为缺省值 */
    BKP_DeInit();

    /* Enable LSE */
    /* 使能LSE（外部32.768KHz低速晶振）*/
    RCC_LSEConfig(RCC_LSE_ON);

    /* Wait till LSE is ready */
    /* 等待外部晶振震荡稳定输出 */
    while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)
    {}

    /* Select LSE as RTC Clock Source */
    /*使用外部32.768KHz晶振作为RTC时钟 */
    RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);

    /* Enable RTC Clock */
    /* 使能 RTC 的时钟供给 */
    RCC_RTCCLKCmd(ENABLE);

    /* Wait for RTC registers synchronization */
    /*等待RTC寄存器同步 */
    RTC_WaitForSynchro();

    /* Wait until last write operation on RTC registers has finished */
    /* 等待上一次对RTC寄存器的写操作完成 */
    RTC_WaitForLastTask();

    /* Enable the RTC Second */
    /* 使能RTC的秒中断 */
    RTC_ITConfig(RTC_IT_SEC, ENABLE);

    /* Wait until last write operation on RTC registers has finished */
    /* 等待上一次对RTC寄存器的写操作完成 */
    RTC_WaitForLastTask();

    /* Set RTC prescaler: set RTC period to 1sec */
    /* 32.768KHz晶振预分频值是32767,如果对精度要求很高可以修改此分频值来校准晶振 */
    RTC_SetPrescaler(32767); /* RTC period = RTCCLK/RTC_PR = (32.768 KHz)/(32767+1) */

    /* Wait until last write operation on RTC registers has finished */
    /* 等待上一次对RTC寄存器的写操作完成 */
    RTC_WaitForLastTask();
}

/**-------------------------------------------------------
  * @函数名 RTC_NVIC_Configuration
  * @功能   RTC实时时钟的中断参数配置函数
  * @参数   无
  * @返回值 无
***------------------------------------------------------*/
void RTC_NVIC_Configuration(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;

    /* Configure one bit for preemption priority */
    //NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);

    /* Enable the RTC Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = RTC_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

__IO uint32_t TimeDisplay = 1;
/**-------------------------------------------------------
  * @函数名 RTC_IRQHandler
  * @功能   RTC实时时钟的中断处理函数
  * @参数   无
  * @返回值 无
***------------------------------------------------------*/
void RTC_IRQHandler(void)
{
    /*  判断RTC是否发生了秒中断（也有可能是溢出或者闹钟中断) */
    if (RTC_GetITStatus(RTC_IT_SEC) != RESET)
    {
        /* Clear the RTC Second interrupt */
        /* 清除秒中断标志 */
        RTC_ClearITPendingBit(RTC_IT_SEC);

        /* Toggle LED1 改变LED1的亮灭状态(取反) */
        SZ_STM32_LEDToggle(LED1);

        /* Enable time update */
        /* 置全局变量为1，通知主程序打印时间 */
        TimeDisplay = 1;

        /* Wait until last write operation on RTC registers has finished */
        /* 等待上一次对RTC寄存器的写操作完成 */
        RTC_WaitForLastTask();
   
    }
}
#endif

/**-------------------------------------------------------
  * @函数名 Buffercmp
  * @功能   比较两个缓冲区给定长度的内容是否相同的函数
  * @参数1  pBuffer1 和 pBuffer2 需要比较两个缓冲区起始地址
  * @参数2  BufferLength 给定的比较长度，字节为单位
  * @返回值 PASSED: 缓冲器给定长度的内容相同
  *         FAILED: 缓冲器给定长度的内容不相同
***------------------------------------------------------*/
TestStatus Buffercmp(u8* pBuffer1, u8* pBuffer2, u16 BufferLength)
{
    while(BufferLength--)
    {
        if(*pBuffer1 != *pBuffer2)
        {
            return FAILED;
        }

        pBuffer1++;
        pBuffer2++;
    }

    return PASSED;
}

#ifdef USE_TIM5
/**-------------------------------------------------------
  * @函数名 NVIC_TIM5Configuration
  * @功能   配置TIM5中断向量参数函数
  * @参数   无
  * @返回值 无
***------------------------------------------------------*/
static void NVIC_TIM5Configuration(void)
{ 
    NVIC_InitTypeDef NVIC_InitStructure;

    /* Set the Vector Table base address at 0x08000000 */
    //NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0000);

    /* Enable the TIM5 gloabal Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;

    NVIC_Init(&NVIC_InitStructure);
}
    
/**-------------------------------------------------------
  * @函数名 NVIC_TIM5Configuration
  * @功能   配置TIM5参数函数，每秒计数器中断一次 
  * @参数   无
  * @返回值 无
***------------------------------------------------------*/
void TIM5_Init(void)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

    /* TIM5 clock enable */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);

    /* ---------------------------------------------------------------
    TIM4 Configuration: Output Compare Timing Mode:
    TIM2CLK = 36 MHz, Prescaler = 7200, TIM2 counter clock = 7.2 MHz
    --------------------------------------------------------------- */

    /* Time base configuration */
    //这个就是自动装载的计数值，由于计数是从0开始的，计数10000次后为9999
    TIM_TimeBaseStructure.TIM_Period = (10000 - 1);
    // 这个就是预分频系数，当由于为0时表示不分频所以要减1
    TIM_TimeBaseStructure.TIM_Prescaler = (7200 - 1);
    // 高级应用本次不涉及。定义在定时器时钟(CK_INT)频率与数字滤波器(ETR,TIx)
    // 使用的采样频率之间的分频比例
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    //向上计数
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    //初始化定时器5
    TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure);

    /* Clear TIM5 update pending flag[清除TIM5溢出中断标志] */
    TIM_ClearITPendingBit(TIM5, TIM_IT_Update);

    /* TIM IT enable */ //打开溢出中断
    TIM_ITConfig(TIM5, TIM_IT_Update, ENABLE);

    /* TIM5 enable counter */
    TIM_Cmd(TIM5, ENABLE);  //计数器使能，开始工作

    /* 中断参数配置 */
    NVIC_TIM5Configuration();
}


/**-------------------------------------------------------
  * @函数名 TIM5_IRQHandler
  * @功能   TIM5中断处理函数，每秒中断一次 
  * @参数   无
  * @返回值 无
***------------------------------------------------------*/
void TIM5_IRQHandler(void)
{
    /* www.armjishu.com ARM技术论坛 */
    static u32 counter = 0;

    if (TIM_GetITStatus(TIM5, TIM_IT_Update) != RESET)
    {
        TIM_ClearITPendingBit(TIM5, TIM_IT_Update);
        /* LED1指示灯状态取反 */
        SZ_STM32_LEDToggle(LED1);

        /* armjishu.com提心您: 不建议在中断中使用Printf, 此示例只是演示。 */
        printf("\n\rarmjishu.com提示您: 不建议在中断中使用Printf, 此示例只是演示。\n\r");
        printf("ARMJISHU.COM-->TIM5:%d\n\r", counter++);
    }
}
#endif

#ifdef USE_TIM3
/**-------------------------------------------------------
  * @函数名 NVIC_TIM3Configuration
  * @功能   配置TIM3中断向量参数函数
  * @参数   无
  * @返回值 无
***------------------------------------------------------*/
static void NVIC_TIM3Configuration(void)
{ 
    NVIC_InitTypeDef NVIC_InitStructure;

    /* Set the Vector Table base address at 0x08000000 */
    //NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0000);

    /* Enable the TIM5 gloabal Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;

    NVIC_Init(&NVIC_InitStructure);
}

/**-------------------------------------------------------
  * @函数名 TIM3_PWM_Init
  * @功能   配置TIM3为PWM模式
  * @参数   无
  * @返回值 无
***------------------------------------------------------*/
void TIM3_PWM_Init(void)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    TIM_OCInitTypeDef  TIM_OCInitStructure;
  	const uint16_t Period_Val = 1020;
    __IO uint16_t CCR1_Val = Period_Val/2;
    __IO uint16_t CCR2_Val = Period_Val-20;
    uint16_t PrescalerValue = 0;
         
    /* System Clocks Configuration */ /* PCLK1 = HCLK/2 = 36MHz */
    /* TIM3 clock enable */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
  
    /* -----------------------------------------------------------------------
      TIM3 Configuration: generate 4 PWM signals with 4 different duty cycles:
      The TIM3CLK frequency is set to SystemCoreClock (Hz), to get TIM3 counter
      clock at 24 MHz the Prescaler is computed as following:
       - Prescaler = (TIM3CLK / TIM3 counter clock) - 1
      SystemCoreClock is set to 72 MHz for Low-density, Medium-density, High-density
      and Connectivity line devices and to 24 MHz for Low-Density Value line and
      Medium-Density Value line devices
  
      The TIM3 is running at 36 KHz: TIM3 Frequency = TIM3 counter clock/(ARR + 1)
                                                    = 24 MHz / 1000 = 24 KHz
      TIM3 Channel1 duty cycle = (TIM3_CCR1/ TIM3_ARR)* 100 = 50%
      TIM3 Channel2 duty cycle = (TIM3_CCR2/ TIM3_ARR)* 100 = 37.5%
    ----------------------------------------------------------------------- */
    /* Compute the prescaler value */
    PrescalerValue = (uint16_t) (SystemCoreClock / 24000000) - 1;
    /* Time base configuration */
    TIM_TimeBaseStructure.TIM_Period = Period_Val;
    TIM_TimeBaseStructure.TIM_Prescaler = PrescalerValue;
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

    /* PWM1 Mode configuration: Channel1 */
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Disable;
    TIM_OCInitStructure.TIM_Pulse = CCR1_Val;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;

    TIM_OC1Init(TIM3, &TIM_OCInitStructure);

    TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);

    /* PWM1 Mode configuration: Channel2 */
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Disable;
    TIM_OCInitStructure.TIM_Pulse = CCR2_Val;

    TIM_OC2Init(TIM3, &TIM_OCInitStructure);

    TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);

    /* TIM IT enable */
    TIM_ITConfig(TIM3, TIM_IT_CC1 | TIM_IT_CC2, ENABLE);

    //TIM_ARRPreloadConfig(TIM3, ENABLE);

    /* TIM3 enable counter */
    TIM_Cmd(TIM3, ENABLE);

    NVIC_TIM3Configuration();
}



/**-------------------------------------------------------
  * @函数名 TIM3_IRQHandler
  * @功能   TIM3中断处理函数，PWM控制指示灯亮度
  * @参数   无
  * @返回值 无
***------------------------------------------------------*/
void TIM3_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM3, TIM_IT_CC1) != RESET)
    {
        TIM_ClearITPendingBit(TIM3, TIM_IT_CC1);

        SZ_STM32_LEDOn(LED1);
        SZ_STM32_LEDOn(LED3);
        SZ_STM32_LEDOff(LED2);
        SZ_STM32_LEDOff(LED4);
    }

    if (TIM_GetITStatus(TIM3, TIM_IT_CC2) != RESET)
    {
        TIM_ClearITPendingBit(TIM3, TIM_IT_CC2);

        SZ_STM32_LEDOff(LED1);
        SZ_STM32_LEDOff(LED3);
        SZ_STM32_LEDOn(LED2);
        SZ_STM32_LEDOn(LED4);
    }
}
#endif

#ifdef USE_IWDG
/**-------------------------------------------------------
  * @函数名 SZ_STM32_IWDGInit
  * @功能   STM32独立看门狗初始化配置函数
  * @参数   TimeoutFreMs  看门狗定时器多少毫秒溢出一次
  * @返回值 无
  * @喂狗方式(Reload IWDG counter) IWDG_ReloadCounter(); 
***------------------------------------------------------*/
void SZ_STM32_IWDGInit(uint32_t TimeoutFreMs)/* 2 -  3276*/
{
    uint32_t ReloadValue;

    /* IWDG timeout equal to 280 ms (the timeout may varies due to LSI frequency
    dispersion) */
    /* Enable write access to IWDG_PR and IWDG_RLR registers */
    IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);

    /* IWDG counter clock: 40KHz(LSI) / 32 = 1.25 KHz */
    IWDG_SetPrescaler(IWDG_Prescaler_32);

    /* Set counter reload value to 349 *//* range from 0 to 4095 */
    //IWDG_SetReload(349);
    if(TimeoutFreMs < 2)
    {
        TimeoutFreMs = 2;
    }

    if(TimeoutFreMs > 3276)
    {
        TimeoutFreMs = 3276;
    }

    ReloadValue = ((TimeoutFreMs * 1250)/1000) - 1;
    IWDG_SetReload(ReloadValue);

    printf("\r\n IWDG Timeout frequency %dms, ReloadValue is set to %d.", TimeoutFreMs, ReloadValue);

    /* Reload IWDG counter */
    IWDG_ReloadCounter();

    /* Enable IWDG (the LSI oscillator will be enabled by hardware) */
    IWDG_Enable();

}
#endif

#ifdef USE_PVD
/**-------------------------------------------------------
  * @函数名 SZ_STM32_PVDConfiguration
  * @功能   可编程电压监测器(programmable voltage detector)初始化
  * @参数   无
  * @返回值 无
***------------------------------------------------------*/
void SZ_STM32_PVDConfiguration(void)
{
    EXTI_InitTypeDef EXTI_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    /* Enable PWR and BKP clock */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);

    /* Configure EXTI Line16(PVD Output) to generate an 
     interrupt on rising and falling edges */
    EXTI_ClearITPendingBit(EXTI_Line16);
    EXTI_InitStructure.EXTI_Line = EXTI_Line16;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);

    /* Enable the PVD Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = PVD_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    /* Enable the PVD Output */
    PWR_PVDCmd(ENABLE);
}

/**-------------------------------------------------------
  * @函数名 PVD_IRQHandler
  * @功能   可编程电压监测器PVD中断处理函数
  * @参数   无
  * @返回值 无
***------------------------------------------------------*/
void PVD_IRQHandler(void)
{
    if(EXTI_GetITStatus(EXTI_Line16) != RESET)
    {
        /* LED1状态取反 */
        LED1OBB = !LED1OBB;
        /* 其它指示灯熄灭 */
        LED2OBB = 1;        	
        LED3OBB = 1;
        LED4OBB = 1;
        
        printf("PVD 2.9V");
        	
        /* Clear the Key Button EXTI line pending bit */
        EXTI_ClearITPendingBit(EXTI_Line16);
    }
}
  
#endif

#ifdef USE_RTC_ALARM
/**-------------------------------------------------------
  * @函数名 RTCAlarm_Configuration
  * @功能   RTC闹钟初始化
  * @参数   无
  * @返回值 无
***------------------------------------------------------*/
void RTCAlarm_Configuration(void)
{
    /* Enable PWR and BKP clocks */
    /* PWR时钟（电源控制）与BKP时钟（RTC后备寄存器）使能 */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);

    /* Allow access to BKP Domain */
    /*使能RTC和后备寄存器访问 */
    PWR_BackupAccessCmd(ENABLE);

    /* Reset Backup Domain */
    /* 将外设BKP的全部寄存器重设为缺省值 */
    //BKP_DeInit();

    /* Enable LSE */
    /* 使能LSE（外部32.768KHz低速晶振）*/
    RCC_LSEConfig(RCC_LSE_ON);

    /* Wait till LSE is ready */
    /* 等待外部晶振震荡稳定输出 */
    while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)
    {}

    /* Select LSE as RTC Clock Source */
    /*使用外部32.768KHz晶振作为RTC时钟 */
    RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);

    /* Enable RTC Clock */
    /* 使能 RTC 的时钟供给 */
    RCC_RTCCLKCmd(ENABLE);

    /* Wait for RTC registers synchronization */
    /*等待RTC寄存器同步 */
    RTC_WaitForSynchro();

    /* Wait until last write operation on RTC registers has finished */
    /* 等待上一次对RTC寄存器的写操作完成 */
    RTC_WaitForLastTask();

    /* Enable the RTC Alarm interrupt */
    /* 使能RTC的闹钟中断 */
    RTC_ITConfig(RTC_IT_ALR, ENABLE);

    /* Wait until last write operation on RTC registers has finished */
    /* 等待上一次对RTC寄存器的写操作完成 */
    RTC_WaitForLastTask();

    /* Set RTC prescaler: set RTC period to 1sec */
    /* 32.768KHz晶振预分频值是32767,如果对精度要求很高可以修改此分频值来校准晶振 */
    RTC_SetPrescaler(32767); /* RTC period = RTCCLK/RTC_PR = (32.768 KHz)/(32767+1) */

    /* Wait until last write operation on RTC registers has finished */
    /* 等待上一次对RTC寄存器的写操作完成 */
    RTC_WaitForLastTask();
}

/**-------------------------------------------------------
  * @函数名 RTCAlarm_NVIC_Configuration
  * @功能   配置RTC闹钟中断向量参数函数
  * @参数   无
  * @返回值 无
***------------------------------------------------------*/
void RTCAlarm_NVIC_Configuration(void)
{
    EXTI_InitTypeDef EXTI_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    /* Configure EXTI Line17(RTC Alarm) to generate 
                      an interrupt on rising edge */
    EXTI_ClearITPendingBit(EXTI_Line17);
    EXTI_InitStructure.EXTI_Line = EXTI_Line17;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);

    /* 2 bits for Preemption Priority and 2 bits for Sub Priority */
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

    /* Enable the RTC Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = RTCAlarm_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}


/**-------------------------------------------------------
  * @函数名 RTC_STANDBY_Configuration
  * @功能   RTC的STANDBY模式初始化
  * @参数   无
  * @返回值 无
***------------------------------------------------------*/
void RTC_STANDBY_Configuration(void)
{
    /* Enable PWR and BKP clocks */
    /* PWR时钟（电源控制）与BKP时钟（RTC后备寄存器）使能 */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);

    /* Allow access to BKP Domain */
    /*使能RTC和后备寄存器访问 */
    PWR_BackupAccessCmd(ENABLE);

    /* Enable WKUP pin */
    PWR_WakeUpPinCmd(ENABLE);


    /* Check if the StandBy flag is set */
    if(PWR_GetFlagStatus(PWR_FLAG_SB) != RESET)
    {/* System resumed from STANDBY mode */

        /* Turn on LED2 */
        SZ_STM32_LEDOn(LED2);

        printf("\r\n resumed from StandBy mode..从STANDBY待机模式退出");

        /* Clear StandBy flag */
        PWR_ClearFlag(PWR_FLAG_SB);

        /* Wait for RTC APB registers synchronisation */
        RTC_WaitForSynchro();
        /* No need to configure the RTC as the RTC configuration(clock source, enable,
        prescaler,...) is kept after wake-up from STANDBY */
    }
    else
    {/* StandBy flag is not set */
        printf("\r\n resumed from Reset..从运行模式复位");
        /* RTC clock source configuration ----------------------------------------*/

        /* Reset Backup Domain */
        /* 将外设BKP的全部寄存器重设为缺省值 */
        //BKP_DeInit();

        /* Enable LSE */
        /* 使能LSE（外部32.768KHz低速晶振）*/
        RCC_LSEConfig(RCC_LSE_ON);

        /* Wait till LSE is ready */
        /* 等待外部晶振震荡稳定输出 */
        while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)
        {
        }

        /* Select LSE as RTC Clock Source */
        /*使用外部32.768KHz晶振作为RTC时钟 */
        RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);

        /* Enable RTC Clock */
        /* 使能 RTC 的时钟供给 */
        RCC_RTCCLKCmd(ENABLE);

        /* RTC configuration -----------------------------------------------------*/
        /* Wait for RTC APB registers synchronisation */
        /*等待RTC寄存器同步 */
        RTC_WaitForSynchro();


        /* Set RTC prescaler: set RTC period to 1sec */
        /* 32.768KHz晶振预分频值是32767,如果对精度要求很高可以修改此分频值来校准晶振 */
        RTC_SetPrescaler(32767); /* RTC period = RTCCLK/RTC_PR = (32.768 KHz)/(32767+1) */

        /* Wait until last write operation on RTC registers has finished */
        /* 等待上一次对RTC寄存器的写操作完成 */
        RTC_WaitForLastTask();
    }
}
#endif

#ifdef  USE_FULL_ASSERT
// 需要在工程设置Option(快捷键ALT+F7)C++属性页的define栏输入"USE_FULL_ASSERT"
/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
    /* User can add his own implementation to report the file name and line number,
       ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
     
    printf("Wrong parameters value: file %s on line %d\r\n", file, line);

    /* Infinite loop */
    while (1)
    {
    }
}
#endif

/******************* (C) COPYRIGHT 2013 www.armjishu.com *****END OF FILE****/
