/******************** (C) COPYRIGHT 2013 www.armjishu.com  ********************
 * 文件名  ：lcd.h
 * 描述    ：提供STM32F107VC神舟IV号开发板的TFT_LCD彩屏相关库函数
 * 实验平台：STM32神舟开发板
 * 作者    ：www.armjishu.com 
*******************************************************************************/
#ifdef __cplusplus
 extern "C" {
#endif

#include "stm32f10x.h"
#include "fonts.h"

/* 神舟IV号LCD管脚相关定义  www.armjishu.com Team  */ 
#define LCD_PinNum_WR               14
#define LCD_Pin_WR                  GPIO_Pin_14
#define LCD_PORT_WR                 GPIOB
#define LCD_CLK_WR                  RCC_APB2Periph_GPIOB

#define LCD_PinNum_CS               8
#define LCD_Pin_CS                  GPIO_Pin_8
#define LCD_PORT_CS                 GPIOC
#define LCD_CLK_CS                  RCC_APB2Periph_GPIOC

#define LCD_PinNum_RS               13
#define LCD_Pin_RS                  GPIO_Pin_13
#define LCD_PORT_RS                 GPIOD
#define LCD_CLK_RS                  RCC_APB2Periph_GPIOD

#define LCD_PinNum_RD               15
#define LCD_Pin_RD                  GPIO_Pin_15
#define LCD_PORT_RD                 GPIOD
#define LCD_CLK_RD                  RCC_APB2Periph_GPIOD


#define LCD_PIN_DATA                GPIO_Pin_All
#define LCD_PORT_DAT                GPIOE
#define LCD_CLK_DATA                RCC_APB2Periph_GPIOE

/* 神舟IV号开发板由于管脚资源有限，所以液晶屏的背光常亮 */
#define LCD_LIGHT_ON   
#define LCD_LIGHT_OFF  

/* 神舟IV号LCD相关定义  基本'点'的结构体  www.armjishu.com Team  */ 
typedef struct 
{
    int16_t X;      // 行
    int16_t Y;      // 列 (行内的的偏移)
} Point, * pPoint;   


/* 神舟IV号LCD寄存器相关定义  LCD Registers  www.armjishu.com Team  */ 
#define LCD_REG_0             0x00
#define LCD_REG_1             0x01
#define LCD_REG_2             0x02
#define LCD_REG_3             0x03
#define LCD_REG_4             0x04
#define LCD_REG_5             0x05
#define LCD_REG_6             0x06
#define LCD_REG_7             0x07
#define LCD_REG_8             0x08
#define LCD_REG_9             0x09
#define LCD_REG_10            0x0A
#define LCD_REG_12            0x0C
#define LCD_REG_13            0x0D
#define LCD_REG_14            0x0E
#define LCD_REG_15            0x0F
#define LCD_REG_16            0x10
#define LCD_REG_17            0x11
#define LCD_REG_18            0x12
#define LCD_REG_19            0x13
#define LCD_REG_20            0x14
#define LCD_REG_21            0x15
#define LCD_REG_22            0x16
#define LCD_REG_23            0x17
#define LCD_REG_24            0x18
#define LCD_REG_25            0x19
#define LCD_REG_26            0x1A
#define LCD_REG_27            0x1B
#define LCD_REG_28            0x1C
#define LCD_REG_29            0x1D
#define LCD_REG_30            0x1E
#define LCD_REG_31            0x1F
#define LCD_REG_32            0x20
#define LCD_REG_33            0x21
#define LCD_REG_34            0x22
#define LCD_REG_36            0x24
#define LCD_REG_37            0x25
#define LCD_REG_40            0x28
#define LCD_REG_41            0x29
#define LCD_REG_43            0x2B
#define LCD_REG_45            0x2D
#define LCD_REG_48            0x30
#define LCD_REG_49            0x31
#define LCD_REG_50            0x32
#define LCD_REG_51            0x33
#define LCD_REG_52            0x34
#define LCD_REG_53            0x35
#define LCD_REG_54            0x36
#define LCD_REG_55            0x37
#define LCD_REG_56            0x38
#define LCD_REG_57            0x39
#define LCD_REG_59            0x3B
#define LCD_REG_60            0x3C
#define LCD_REG_61            0x3D
#define LCD_REG_62            0x3E
#define LCD_REG_63            0x3F
#define LCD_REG_64            0x40
#define LCD_REG_65            0x41
#define LCD_REG_66            0x42
#define LCD_REG_67            0x43
#define LCD_REG_68            0x44
#define LCD_REG_69            0x45
#define LCD_REG_70            0x46
#define LCD_REG_71            0x47
#define LCD_REG_72            0x48
#define LCD_REG_73            0x49
#define LCD_REG_74            0x4A
#define LCD_REG_75            0x4B
#define LCD_REG_76            0x4C
#define LCD_REG_77            0x4D
#define LCD_REG_78            0x4E
#define LCD_REG_79            0x4F
#define LCD_REG_80            0x50
#define LCD_REG_81            0x51
#define LCD_REG_82            0x52
#define LCD_REG_83            0x53
#define LCD_REG_96            0x60
#define LCD_REG_97            0x61
#define LCD_REG_106           0x6A
#define LCD_REG_118           0x76
#define LCD_REG_128           0x80
#define LCD_REG_129           0x81
#define LCD_REG_130           0x82
#define LCD_REG_131           0x83
#define LCD_REG_132           0x84
#define LCD_REG_133           0x85
#define LCD_REG_134           0x86
#define LCD_REG_135           0x87
#define LCD_REG_136           0x88
#define LCD_REG_137           0x89
#define LCD_REG_139           0x8B
#define LCD_REG_140           0x8C
#define LCD_REG_141           0x8D
#define LCD_REG_143           0x8F
#define LCD_REG_144           0x90
#define LCD_REG_145           0x91
#define LCD_REG_146           0x92
#define LCD_REG_147           0x93
#define LCD_REG_148           0x94
#define LCD_REG_149           0x95
#define LCD_REG_150           0x96
#define LCD_REG_151           0x97
#define LCD_REG_152           0x98
#define LCD_REG_153           0x99
#define LCD_REG_154           0x9A
#define LCD_REG_157           0x9D
#define LCD_REG_192           0xC0
#define LCD_REG_193           0xC1
#define LCD_REG_229           0xE5

/* 神舟IV号LCD彩屏常用基本颜色定义  LCD color   www.armjishu.com Team  */ 
#define LCD_COLOR_WHITE          0xFFFF
#define LCD_COLOR_BLACK          0x0000
#define LCD_COLOR_GREY           0xE79C
#define LCD_COLOR_BLUE           0x001F
#define LCD_COLOR_BLUE2          0x051F
#define LCD_COLOR_RED            0xF800
#define LCD_COLOR_MAGENTA        0xF81F
#define LCD_COLOR_GREEN          0x07E0
#define LCD_COLOR_CYAN           0x7FFF
#define LCD_COLOR_YELLOW         0xFFE0

/* 神舟IV号LCD彩屏显示字符的行数，与字体有关  www.armjishu.com Team  */ 
#define LCD_LINE_0               LINE(0)
#define LCD_LINE_1               LINE(1)
#define LCD_LINE_2               LINE(2)
#define LCD_LINE_3               LINE(3)
#define LCD_LINE_4               LINE(4)
#define LCD_LINE_5               LINE(5)
#define LCD_LINE_6               LINE(6)
#define LCD_LINE_7               LINE(7)
#define LCD_LINE_8               LINE(8)
#define LCD_LINE_9               LINE(9)
#define LCD_LINE_10              LINE(10)
#define LCD_LINE_11              LINE(11)
#define LCD_LINE_12              LINE(12)
#define LCD_LINE_13              LINE(13)
#define LCD_LINE_14              LINE(14)
#define LCD_LINE_15              LINE(15)
#define LCD_LINE_16              LINE(16)
#define LCD_LINE_17              LINE(17)
#define LCD_LINE_18              LINE(18)
#define LCD_LINE_19              LINE(19)
#define LCD_LINE_20              LINE(20)
#define LCD_LINE_21              LINE(21)
#define LCD_LINE_22              LINE(22)
#define LCD_LINE_23              LINE(23)
#define LCD_LINE_24              LINE(24)
#define LCD_LINE_25              LINE(25)
#define LCD_LINE_26              LINE(26)
#define LCD_LINE_27              LINE(27)
#define LCD_LINE_28              LINE(28)
#define LCD_LINE_29              LINE(29)

/* 神舟IV号LCD彩屏画线等函数的方向Direction定义   www.armjishu.com Team  */ 
#define LCD_DIR_HORIZONTAL       0x0000
#define LCD_DIR_VERTICAL         0x0001

/* 神舟IV号LCD彩屏分辨率定义  宽和高   www.armjishu.com Team  */ 
#define LCD_PIXEL_WIDTH          0x0140  /* 宽320 */
#define LCD_PIXEL_HEIGHT         0x00F0  /* 高240 */

/* 显示字符时透明背景模式开关 */
#define HyalineBackEn           0x01  //  透明背景
#define HyalineBackDis          0x00  //  不透明背景(默认)

/**  Image2Lcd保存的图像数据 typedef struct _HEADGRAY
scan: 扫描模式 
Bit7: 0:自左至右扫描，1:自右至左扫描。 
Bit6: 0:自顶至底扫描，1:自底至顶扫描。 
Bit5: 0:字节内象素数据从高位到低位排列，1:字节内象素数据从低位到高位排列。 
Bit4: 0:WORD类型高低位字节顺序与PC相同，1:WORD类型高低位字节顺序与PC相反。 
Bit3~2: 保留。 
Bit1~0: [00]水平扫描，[01]垂直扫描，[10]数据水平,字节垂直，[11]数据垂直,字节水平。 
gray: 灰度值 
   灰度值，1:单色，2:四灰，4:十六灰，8:256色，12:4096色，16:16位彩色，24:24位彩色，32:32位彩色。
w: 图像的宽度。
h: 图像的高度。
  */ 
typedef struct _HEADGRAY
{
   unsigned char scan;
   unsigned char gray;
   unsigned short w;
   unsigned short h;
}HEADGRAY;

/* LCD当前的英文字库 */
extern sFONT *LCD_Currentfonts;
/* 神舟欢迎文字字库 */
extern const uint8_t WelcomeStr[13][72];

/* 神舟IV号LCD彩屏函数声明   www.armjishu.com Team  */
void delay(__IO uint32_t nCount); 
void SZ_STM32_LCDInit(void);
void LCD_SetCursor(uint16_t Xpos, uint16_t Ypos);
void LCD_Clear(uint16_t Color);
void LCD_SetPoint(uint16_t Xpos, uint16_t Ypos, uint16_t Color);

void LCD_WriteReg(uint8_t LCD_Reg, uint16_t LCD_RegValue);
void LCD_WriteRAM_Prepare(void);
void LCD_WriteRAMWord(uint16_t RGB_Code);
uint16_t LCD_ReadReg(uint8_t LCD_Reg);
void LCD_WriteRAM(uint16_t RGB_Code);
void LCD_Pins_Config(void);
void LCD_DrawChar(uint16_t Xpos, uint16_t Ypos,  const uint8_t *cpFontArray);
void LCD_DisplayChar(uint16_t Line, uint16_t Column, uint8_t Ascii);
void LCD_SetFont(sFONT *fonts);
sFONT *LCD_GetFont(void);
void LCD_SetHyaline(uint8_t NewHyalineBack);
void LCD_ClearLine(uint16_t Line);
void LCD_DisplayStringLine(uint16_t Line, uint8_t *ptr,uint16_t refcolumn);
void LCD_DisplayString(uint16_t Xpos, uint16_t Ypos, uint8_t *ptr);
void LCD_DisplayWelcomeStr(uint16_t Line);
void LCD_SetBackColor(__IO uint16_t Color);
void LCD_SetTextColor(__IO uint16_t Color);
void LCD_DrawFullRect(uint16_t Xpos, uint16_t Ypos, uint16_t Width, uint16_t Height);
void LCD_DrawLine(uint16_t Xpos, uint16_t Ypos, uint16_t Length, uint8_t Direction);
void LCD_WR_REG(uint16_t LCD_Reg);
void LCD_WR_DATA(uint16_t LCD_Data);
void LCD_DrawPoint(uint16_t xsta, uint16_t ysta);

#ifdef __cplusplus
}
#endif



     
/*******************  (C) COPYRIGHT 2013 www.armjishu.com  *****END OF FILE****/
