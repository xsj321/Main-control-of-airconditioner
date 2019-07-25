/******************** (C) COPYRIGHT 2013 www.armjishu.com  ********************
 * 文件名  ：SZ_STM32F107VC_LCD.c
 * 描述    ：提供STM32F107VC神舟IV号开发板的TFT_LCD彩屏相关库函数
 * 实验平台：STM32神舟开发板
 * 作者    ：www.armjishu.com 
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "SZ_STM32F107VC_LCD.h"
#include "SZ_STM32F107VC_LIB.h"


/**-------------------------------------------------------
  * LCD默认采用横屏显示，其中坐标表示为(x,y)
  * x LCD的行数  
  * y LCD的列数
  (0,0)     表示LCD左上角的点 
  (239,319) 表示LCD右下角的点   
  (0,2)     表示LCD第1行第3列的点行
  (238,2)   表示LCD第239行第3列的点行
          yyyy                               Y
          0123                               319             
  X = 0   |--(0,2)-------- ~~~ --------------|
  X = 1   |--------------- ~~~ --------------|
  X = 2   |--------------- ~~~ --------------|
  .....
  X = 237 |--------------- ~~~ --------------|
  X = 238 |--(238,2)------ ~~~ --------------|  
  X = 239 |--------------- ~~~ --------------|(239,319)
  
***------------------------------------------------------*/



#define LCD_DEBUG_PRINTF               printf
//#define LCD_DEBUG_PRINTF(str,args...)    ((void)0)

#define SetCs  Periph_BB((uint32_t)&LCD_PORT_CS->ODR, LCD_PinNum_CS) = 1;
#define ClrCs  Periph_BB((uint32_t)&LCD_PORT_CS->ODR, LCD_PinNum_CS) = 0;
 

#define SetWr  Periph_BB((uint32_t)&LCD_PORT_WR->ODR, LCD_PinNum_WR) = 1;
#define ClrWr  Periph_BB((uint32_t)&LCD_PORT_WR->ODR, LCD_PinNum_WR) = 0;

#define SetRs  Periph_BB((uint32_t)&LCD_PORT_RS->ODR, LCD_PinNum_RS) = 1;
#define ClrRs  Periph_BB((uint32_t)&LCD_PORT_RS->ODR, LCD_PinNum_RS) = 0;

#define SetRd  Periph_BB((uint32_t)&LCD_PORT_RD->ODR, LCD_PinNum_RD) = 1;
#define ClrRd  Periph_BB((uint32_t)&LCD_PORT_RD->ODR, LCD_PinNum_RD) = 0;

#define LCD_Write(LCD_DATA)  LCD_PORT_DAT->ODR = LCD_DATA
#define LCD_Read()           LCD_PORT_DAT->IDR

/* delay函数在"SZ_STM32F107VC_LIB.h"有定义 */
#define Delay(nCount)  delay(12000 * nCount) 

/* 前景色TextColor和背景色BackColor */
static  __IO uint16_t TextColor = 0x0000, BackColor = 0xFFFF;
/* LCD模组驱动芯片ID */
static  __IO uint16_t DeviceIdCode;

/** @defgroup STM3210C_EVAL_LCD_Private_Defines
  * @{
  */ 

#define MAX_POLY_CORNERS   200
#define POLY_Y(Z)          ((int32_t)((Points + Z)->X))
#define POLY_X(Z)          ((int32_t)((Points + Z)->Y))

#define ABS(X)  ((X) > 0 ? (X) : -(X))    

/*******************************************************************************
* Function Name  : LCD_DB_AS_InPut
* Description    : config MCU LCD_DB pins AS InPut
* Input          : None
* Output         : None
* Return         : None.
*******************************************************************************/
void LCD_DB_AS_InPut(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  // DB15--0
  GPIO_InitStructure.GPIO_Pin = LCD_PIN_DATA;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(LCD_PORT_DAT, &GPIO_InitStructure);
}

/*******************************************************************************
* Function Name  : LCD_DB_AS_OutPut
* Description    : config MCU LCD_DB pins AS OutPut
* Input          : None
* Output         : None
* Return         : None.
*******************************************************************************/
void LCD_DB_AS_OutPut(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  // DB15--0
  GPIO_InitStructure.GPIO_Pin = LCD_PIN_DATA;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(LCD_PORT_DAT, &GPIO_InitStructure);
}


/****************************************************************************
* 名    称：void LCD_Setup()
* 功    能：初始化 LCD 控制器
* 入口参数：无
* 出口参数：无
* 说    明：
* 调用方法：LCD_Setup();
* 作    者： www.armjishu.com TEAM
****************************************************************************/
void LCD_Setup(void)
{ 
    /* Configure the LCD pins config --------------------------------------------*/
    LCD_Pins_Config();

    Delay(1); /* Delay 50 ms */
    LCD_WriteReg(0x0000,0x0001);          //start internal osc
    Delay(1); /* Delay 50 ms */
    DeviceIdCode = LCD_ReadReg(0x0000);
    DeviceIdCode = LCD_ReadReg(0x0000);
    LCD_DEBUG_PRINTF("\n\r ###### www.armjishu.com DeviceIdCode = 0x%x ###### ", DeviceIdCode);

    Delay(1); /* Delay 50 ms */            //start internal osc

    if(DeviceIdCode == 0x8989)
    {
        LCD_DEBUG_PRINTF("\n\r This LCD is SSD1289 ");
    }

    if(DeviceIdCode == 0x9325)
    {
        LCD_DEBUG_PRINTF("\n\r This LCD is ili9325 ");
    }
  
    if(DeviceIdCode==0x9325||DeviceIdCode==0x9328)
    {
        LCD_WriteReg(0x00e3,0x3008);
        LCD_WriteReg(0x00e7,0x0012);
        LCD_WriteReg(0x00ef,0x1231);        //Set the internal vcore voltage
        // LCD_WriteReg(0x00e7,0x0010);      
        LCD_WriteReg(0x0000,0x0001);        //start internal osc
        LCD_WriteReg(0x0001,0x0100);     
        LCD_WriteReg(0x0002,0x0700);        //Driving Wave Control                   
        LCD_WriteReg(0x0003,(1<<12)|(0<<5)|(1<<4)|(1<<3) );     //65K 
        LCD_WriteReg(0x0004,0x0000);                                   
        LCD_WriteReg(0x0008,0x0207);               
        LCD_WriteReg(0x0009,0x0000);         
        LCD_WriteReg(0x000a,0x0000);        //display setting         
        LCD_WriteReg(0x000c,0x0001);        //display setting          
        LCD_WriteReg(0x000d,0x0000);        //0f3c          
        LCD_WriteReg(0x000f,0x0000);
        //Power On sequence //
        LCD_WriteReg(0x0010,0x0000);   
        LCD_WriteReg(0x0011,0x0007);
        LCD_WriteReg(0x0012,0x0000);                                                                 
        LCD_WriteReg(0x0013,0x0000);                 
        Delay(5);
        Delay(5);
        LCD_WriteReg(0x0010,0x1590);   
        LCD_WriteReg(0x0011,0x0227);
        Delay(5);
        Delay(5);
        LCD_WriteReg(0x0012,0x009c);                  
        Delay(5);
        Delay(5);
        LCD_WriteReg(0x0013,0x1900);   
        LCD_WriteReg(0x0029,0x0023);
        LCD_WriteReg(0x002b,0x000e);
        Delay(5);
        Delay(5);
        LCD_WriteReg(0x0020,0x0000);                                                            
        LCD_WriteReg(0x0021,0x0000);           
        Delay(5);
        Delay(5);
        LCD_WriteReg(0x0030,0x0007); 
        LCD_WriteReg(0x0031,0x0707);   
        LCD_WriteReg(0x0032,0x0006);
        LCD_WriteReg(0x0035,0x0704);
        LCD_WriteReg(0x0036,0x1f04); 
        LCD_WriteReg(0x0037,0x0004);
        LCD_WriteReg(0x0038,0x0000);        
        LCD_WriteReg(0x0039,0x0706);     
        LCD_WriteReg(0x003c,0x0701);
        LCD_WriteReg(0x003d,0x000f);
        Delay(5);
        Delay(5);
        LCD_WriteReg(0x0050,0x0000);        
        LCD_WriteReg(0x0051,0x00ef);   
        LCD_WriteReg(0x0052,0x0000);     
        LCD_WriteReg(0x0053,0x013f);
        LCD_WriteReg(0x0060,0xa700);        
        LCD_WriteReg(0x0061,0x0001); 
        LCD_WriteReg(0x006a,0x0000);
        LCD_WriteReg(0x0080,0x0000);
        LCD_WriteReg(0x0081,0x0000);
        LCD_WriteReg(0x0082,0x0000);
        LCD_WriteReg(0x0083,0x0000);
        LCD_WriteReg(0x0084,0x0000);
        LCD_WriteReg(0x0085,0x0000);
      
        LCD_WriteReg(0x0090,0x0010);     
        LCD_WriteReg(0x0092,0x0600);  
        if(DeviceIdCode==0x9328)
        {   
            LCD_WriteReg(0x0093,0x0003);
            LCD_WriteReg(0x0095,0x0110);
            LCD_WriteReg(0x0097,0x0000);        
            LCD_WriteReg(0x0098,0x0000);  
         }
         //display on sequence     
        LCD_WriteReg(0x0007,0x0133);
    
        LCD_WriteReg(0x0020,0x0000);                                                            
        LCD_WriteReg(0x0021,0x0000);
    }
    else if(DeviceIdCode==0x9320||DeviceIdCode==0x9300)
    {
        LCD_DEBUG_PRINTF("\n\r This LCD is ili%x.", DeviceIdCode);
        LCD_WriteReg(0x00,0x0001);
        LCD_WriteReg(0x01,0x0100);      //Driver Output Contral. 0x0100
        LCD_WriteReg(0x02,0x0700);      //LCD Driver Waveform Contral.
        LCD_WriteReg(0x03,0x1018);      //Entry Mode Set. 0x1018 0x1030
    
        LCD_WriteReg(0x04,0x0000);      //Scalling Contral.
        LCD_WriteReg(0x08,0x0202);      //Display Contral 2.(0x0207)
        LCD_WriteReg(0x09,0x0000);      //Display Contral 3.(0x0000)
        LCD_WriteReg(0x0a,0x0000);      //Frame Cycle Contal.(0x0000)
        LCD_WriteReg(0x0c,(1<<0));      //Extern Display Interface Contral 1.(0x0000)
        LCD_WriteReg(0x0d,0x0000);      //Frame Maker Position.
        LCD_WriteReg(0x0f,0x0000);      //Extern Display Interface Contral 2.
    
        Delay(5);
        LCD_WriteReg(0x07,0x0101);      //Display Contral.
        Delay(5);
    
        LCD_WriteReg(0x10,(1<<12)|(0<<8)|(1<<7)|(1<<6)|(0<<4));     //Power Control 1.(0x16b0)
        LCD_WriteReg(0x11,0x0007);                                  //Power Control 2.(0x0001)
        LCD_WriteReg(0x12,(1<<8)|(1<<4)|(0<<0));                    //Power Control 3.(0x0138)
        LCD_WriteReg(0x13,0x0b00);                                  //Power Control 4.
        LCD_WriteReg(0x29,0x0000);                                  //Power Control 7.
        LCD_WriteReg(0x2b,(1<<14)|(1<<4));
        
        LCD_WriteReg(0x50,0);           //Set X Start.
        LCD_WriteReg(0x51,239);         //Set X End.
        LCD_WriteReg(0x52,0);           //Set Y Start.
        LCD_WriteReg(0x53,319);         //Set Y End.
        
        LCD_WriteReg(0x60,0x2700);      //Driver Output Control.
        LCD_WriteReg(0x61,0x0001);      //Driver Output Control.
        LCD_WriteReg(0x6a,0x0000);      //Vertical Srcoll Control.
    
        LCD_WriteReg(0x80,0x0000);      //Display Position? Partial Display 1.
        LCD_WriteReg(0x81,0x0000);      //RAM Address Start? Partial Display 1.
        LCD_WriteReg(0x82,0x0000);      //RAM Address End-Partial Display 1.
        LCD_WriteReg(0x83,0x0000);      //Displsy Position? Partial Display 2.
        LCD_WriteReg(0x84,0x0000);      //RAM Address Start? Partial Display 2.
        LCD_WriteReg(0x85,0x0000);      //RAM Address End? Partial Display 2.
    
        LCD_WriteReg(0x90,(0<<7)|(16<<0));    //Frame Cycle Contral.(0x0013)
        LCD_WriteReg(0x92,0x0000);      //Panel Interface Contral 2.(0x0000)
        LCD_WriteReg(0x93,0x0001);      //Panel Interface Contral 3.
        LCD_WriteReg(0x95,0x0110);      //Frame Cycle Contral.(0x0110)
        LCD_WriteReg(0x97,(0<<8));      //
        LCD_WriteReg(0x98,0x0000);      //Frame Cycle Contral.
        Delay(5);
        LCD_WriteReg(0x07,0x0173);      //(0x0173)
        Delay(5);
    }
    else if(DeviceIdCode==0x9331)
    {
        LCD_WriteReg(0x00E7, 0x1014);
        LCD_WriteReg(0x0001, 0x0100);   // set SS and SM bit   0x0100
        LCD_WriteReg(0x0002, 0x0200);   // set 1 line inversion
        LCD_WriteReg(0x0003, 0x1030);   // set GRAM write direction and BGR=1.     0x1030
        LCD_WriteReg(0x0008, 0x0202);   // set the back porch and front porch
        LCD_WriteReg(0x0009, 0x0000);   // set non-display area refresh cycle ISC[3:0]
        LCD_WriteReg(0x000A, 0x0000);   // FMARK function
        LCD_WriteReg(0x000C, 0x0000);   // RGB interface setting
        LCD_WriteReg(0x000D, 0x0000);   // Frame marker Position
        LCD_WriteReg(0x000F, 0x0000);   // RGB interface polarity
        //*************Power On sequence ****************//
        LCD_WriteReg(0x0010, 0x0000);   // SAP, BT[3:0], AP, DSTB, SLP, STB
        LCD_WriteReg(0x0011, 0x0007);   // DC1[2:0], DC0[2:0], VC[2:0]
        LCD_WriteReg(0x0012, 0x0000);   // VREG1OUT voltage
        LCD_WriteReg(0x0013, 0x0000);   // VDV[4:0] for VCOM amplitude
        Delay(5); // Dis-charge capacitor power voltage
        LCD_WriteReg(0x0010, 0x1690);   // SAP, BT[3:0], AP, DSTB, SLP, STB
        LCD_WriteReg(0x0011, 0x0227);   // DC1[2:0], DC0[2:0], VC[2:0]
        Delay(2); // Delay 50ms
        LCD_WriteReg(0x0012, 0x000C);   // Internal reference voltage= Vci;
        Delay(2); // Delay 50ms
        LCD_WriteReg(0x0013, 0x0800);   // Set VDV[4:0] for VCOM amplitude
        LCD_WriteReg(0x0029, 0x0011);   // Set VCM[5:0] for VCOMH
        LCD_WriteReg(0x002B, 0x000B);   // Set Frame Rate
        Delay(2); // Delay 50ms
        LCD_WriteReg(0x0020, 0x0000);   // GRAM horizontal Address
        LCD_WriteReg(0x0021, 0x0000);   // GRAM Vertical Address
        // ----------- Adjust the Gamma Curve ----------//
        LCD_WriteReg(0x0030, 0x0000);
        LCD_WriteReg(0x0031, 0x0106);
        LCD_WriteReg(0x0032, 0x0000);
        LCD_WriteReg(0x0035, 0x0204);
        LCD_WriteReg(0x0036, 0x160A);
        LCD_WriteReg(0x0037, 0x0707);
        LCD_WriteReg(0x0038, 0x0106);
        LCD_WriteReg(0x0039, 0x0707);
        LCD_WriteReg(0x003C, 0x0402);
        LCD_WriteReg(0x003D, 0x0C0F);
        //------------------ Set GRAM area ---------------//
        LCD_WriteReg(0x0050, 0x0000);   // Horizontal GRAM Start Address
        LCD_WriteReg(0x0051, 0x00EF);   // Horizontal GRAM End Address
        LCD_WriteReg(0x0052, 0x0000);   // Vertical GRAM Start Address
        LCD_WriteReg(0x0053, 0x013F);   // Vertical GRAM Start Address
        LCD_WriteReg(0x0060, 0x2700);   // Gate Scan Line
        LCD_WriteReg(0x0061, 0x0001);   // NDL,VLE, REV
        LCD_WriteReg(0x006A, 0x0000);   // set scrolling line
        //-------------- Partial Display Control ---------//
        LCD_WriteReg(0x0080, 0x0000);
        LCD_WriteReg(0x0081, 0x0000);
        LCD_WriteReg(0x0082, 0x0000);
        LCD_WriteReg(0x0083, 0x0000);
        LCD_WriteReg(0x0084, 0x0000);
        LCD_WriteReg(0x0085, 0x0000);
        //-------------- Panel Control -------------------//
        LCD_WriteReg(0x0090, 0x0010);
        LCD_WriteReg(0x0092, 0x0600);
        LCD_WriteReg(0x0007,0x0021);        
        Delay(2);
        LCD_WriteReg(0x0007,0x0061);
        Delay(2);
        LCD_WriteReg(0x0007,0x0133);    // 262K color and display ON
        Delay(2);
    }
    else if(DeviceIdCode==0x9919)
    {
        //*********POWER ON &RESET DISPLAY OFF
         LCD_WriteReg(0x28,0x0006);
         LCD_WriteReg(0x00,0x0001);
         LCD_WriteReg(0x10,0x0000);
         LCD_WriteReg(0x01,0x72ef);
         LCD_WriteReg(0x02,0x0600);
         LCD_WriteReg(0x03,0x6a38);
         LCD_WriteReg(0x11,0x6874);
         
         //  RAM WRITE DATA MASK
         LCD_WriteReg(0x0f,0x0000); 
         //  RAM WRITE DATA MASK
         LCD_WriteReg(0x0b,0x5308); 
         LCD_WriteReg(0x0c,0x0003);
         LCD_WriteReg(0x0d,0x000a);
         LCD_WriteReg(0x0e,0x2e00);
         LCD_WriteReg(0x1e,0x00be);
         LCD_WriteReg(0x25,0x8000);
         LCD_WriteReg(0x26,0x7800);
         LCD_WriteReg(0x27,0x0078);
         LCD_WriteReg(0x4e,0x0000);
         LCD_WriteReg(0x4f,0x0000);
         LCD_WriteReg(0x12,0x08d9);
         // -----------------Adjust the Gamma Curve----//
         LCD_WriteReg(0x30,0x0000);     //0007
         LCD_WriteReg(0x31,0x0104);     //0203
         LCD_WriteReg(0x32,0x0100);     //0001
         LCD_WriteReg(0x33,0x0305);     //0007
         LCD_WriteReg(0x34,0x0505);     //0007
         LCD_WriteReg(0x35,0x0305);     //0407
         LCD_WriteReg(0x36,0x0707);     //0407
         LCD_WriteReg(0x37,0x0300);     //0607
         LCD_WriteReg(0x3a,0x1200);     //0106
         LCD_WriteReg(0x3b,0x0800);         
         LCD_WriteReg(0x07,0x0033);
    }
    else if(DeviceIdCode==0x1505)
    {
        // second release on 3/5  ,luminance is acceptable,water wave appear during camera preview
        LCD_WriteReg(0x0007,0x0000);
        Delay(2);
        LCD_WriteReg(0x0012,0x011C);    //0x011A   why need to set several times?
        LCD_WriteReg(0x00A4,0x0001);    //NVM
        LCD_WriteReg(0x0008,0x000F);
        LCD_WriteReg(0x000A,0x0008);
        LCD_WriteReg(0x000D,0x0008);
       
        //GAMMA CONTROL/
        LCD_WriteReg(0x0030,0x0707);
        LCD_WriteReg(0x0031,0x0007);    //0x0707
        LCD_WriteReg(0x0032,0x0603); 
        LCD_WriteReg(0x0033,0x0700); 
        LCD_WriteReg(0x0034,0x0202); 
        LCD_WriteReg(0x0035,0x0002);    //?0x0606
        LCD_WriteReg(0x0036,0x1F0F);
        LCD_WriteReg(0x0037,0x0707);    //0x0f0f  0x0105
        LCD_WriteReg(0x0038,0x0000); 
        LCD_WriteReg(0x0039,0x0000); 
        LCD_WriteReg(0x003A,0x0707); 
        LCD_WriteReg(0x003B,0x0000);    //0x0303
        LCD_WriteReg(0x003C,0x0007);    //?0x0707
        LCD_WriteReg(0x003D,0x0000);    //0x1313//0x1f08
        Delay(2);
        LCD_WriteReg(0x0007,0x0001);
        LCD_WriteReg(0x0017,0x0001);    //Power supply startup enable
        Delay(2);
  
        //power control//
        LCD_WriteReg(0x0010,0x17A0); 
        LCD_WriteReg(0x0011,0x0217);    //reference voltage VC[2:0]   Vciout = 1.00*Vcivl
        LCD_WriteReg(0x0012,0x011E);    //0x011c  //Vreg1out = Vcilvl*1.80   is it the same as Vgama1out ?
        LCD_WriteReg(0x0013,0x0F00);    //VDV[4:0]-->VCOM Amplitude VcomL = VcomH - Vcom Ampl
        LCD_WriteReg(0x002A,0x0000);  
        LCD_WriteReg(0x0029,0x000A);    //0x0001F  Vcomh = VCM1[4:0]*Vreg1out    gate source voltage??
        LCD_WriteReg(0x0012,0x013E);    // 0x013C  power supply on
           //Coordinates Control//
        LCD_WriteReg(0x0050,0x0000);    //0x0e00
        LCD_WriteReg(0x0051,0x00EF); 
        LCD_WriteReg(0x0052,0x0000); 
        LCD_WriteReg(0x0053,0x013F); 
        //Pannel Image Control//
        LCD_WriteReg(0x0060,0x2700); 
        LCD_WriteReg(0x0061,0x0001); 
        LCD_WriteReg(0x006A,0x0000); 
        LCD_WriteReg(0x0080,0x0000); 
        //Partial Image Control//
        LCD_WriteReg(0x0081,0x0000); 
        LCD_WriteReg(0x0082,0x0000); 
        LCD_WriteReg(0x0083,0x0000); 
        LCD_WriteReg(0x0084,0x0000); 
        LCD_WriteReg(0x0085,0x0000); 
        //Panel Interface Control//
        LCD_WriteReg(0x0090,0x0013);    //0x0010 frenqucy
        LCD_WriteReg(0x0092,0x0300); 
        LCD_WriteReg(0x0093,0x0005); 
        LCD_WriteReg(0x0095,0x0000); 
        LCD_WriteReg(0x0097,0x0000); 
        LCD_WriteReg(0x0098,0x0000); 
  
        LCD_WriteReg(0x0001,0x0100); 
        LCD_WriteReg(0x0002,0x0700); 
        LCD_WriteReg(0x0003,0x1030); 
        LCD_WriteReg(0x0004,0x0000); 
        LCD_WriteReg(0x000C,0x0000); 
        LCD_WriteReg(0x000F,0x0000); 
        LCD_WriteReg(0x0020,0x0000); 
        LCD_WriteReg(0x0021,0x0000); 
        LCD_WriteReg(0x0007,0x0021); 
        Delay(2);
        LCD_WriteReg(0x0007,0x0061); 
        Delay(2);
        LCD_WriteReg(0x0007,0x0173); 
        Delay(2);
    }                             
    else if(DeviceIdCode==0x8989)
    {
        LCD_WriteReg(0x0000,0x0001);    //打开晶振
        LCD_WriteReg(0x0003,0xA8A4);    //0xA8A4
		//LCD_WriteReg(0x0003,0x6664);    //0xA8A4
        LCD_WriteReg(0x000C,0x0000);    
        LCD_WriteReg(0x000D,0x080C);    
        LCD_WriteReg(0x000E,0x2B00);    
        LCD_WriteReg(0x001E,0x00B0);    
        LCD_WriteReg(0x0001,0x293F);    //驱动输出控制320*240  0x693F  0x2B3F 0x293F
        LCD_WriteReg(0x0002,0x0600);     //LCD Driving Waveform control
        LCD_WriteReg(0x0010,0x0000);    
        LCD_WriteReg(0x0011,0x6078);    //0x4030    //定义数据格式  16位色 0x6070    横屏 0x6058
        LCD_WriteReg(0x0005,0x0000);    
        LCD_WriteReg(0x0006,0x0000);    
        LCD_WriteReg(0x0016,0xEF1C);    
        LCD_WriteReg(0x0017,0x0003);    
        LCD_WriteReg(0x0007,0x0233);    //0x0233       
        LCD_WriteReg(0x000B,0x0000);    
        LCD_WriteReg(0x000F,0x0000);    //扫描开始地址
        LCD_WriteReg(0x0041,0x0000);    
        LCD_WriteReg(0x0042,0x0000);    
        LCD_WriteReg(0x0048,0x0000);    
        LCD_WriteReg(0x0049,0x013F);    
        LCD_WriteReg(0x004A,0x0000);    
        LCD_WriteReg(0x004B,0x0000);    
        LCD_WriteReg(0x0044,0xEF00);    //Specify the start/end positions of the window address in the horizontal direction by an address unit
        LCD_WriteReg(0x0045,0x0000);    //Specify the start positions of the window address in the vertical direction by an address unit 
        LCD_WriteReg(0x0046,0x013F);    //Specify the end positions of the window address in the vertical direction by an address unit 
        LCD_WriteReg(0x0030,0x0707);    
        LCD_WriteReg(0x0031,0x0204);    
        LCD_WriteReg(0x0032,0x0204);    
        LCD_WriteReg(0x0033,0x0502);    
        LCD_WriteReg(0x0034,0x0507);    
        LCD_WriteReg(0x0035,0x0204);    
        LCD_WriteReg(0x0036,0x0204);    
        LCD_WriteReg(0x0037,0x0502);    
        LCD_WriteReg(0x003A,0x0302);    
        LCD_WriteReg(0x003B,0x0302);    
        LCD_WriteReg(0x0023,0x0000);    
        LCD_WriteReg(0x0024,0x0000);    
        LCD_WriteReg(0x0025,0xC000);    //Frame Frequency Control 
        LCD_WriteReg(0x004e,0);         //列(X)首址0
        LCD_WriteReg(0x004f,0);         //行(Y)首址0
    }
    else
    {
        LCD_DEBUG_PRINTF("\n\r ###### Err: Unknow DeviceIdCode 0x%x ###### ", DeviceIdCode);
    }

    LCD_Clear(LCD_COLOR_GREEN);
}


/**
  * @brief  Initializes the LCD.
  * @param  None
  * @retval None
  */
void SZ_STM32_LCDInit(void)
{
    /* Setups the LCD */
    LCD_Setup();
}

/**
  * @brief  Sets the LCD Text and Background colors.
  * @param  _TextColor: specifies the Text Color.
  * @param  _BackColor: specifies the Background Color.
  * @retval None
  */
void LCD_SetColors(__IO uint16_t _TextColor, __IO uint16_t _BackColor)
{
    TextColor = _TextColor; 
    BackColor = _BackColor;
}

/**
  * @brief  Gets the LCD Text and Background colors.
  * @param  _TextColor: pointer to the variable that will contain the Text 
            Color.
  * @param  _BackColor: pointer to the variable that will contain the Background 
            Color.
  * @retval None
  */
void LCD_GetColors(__IO uint16_t *_TextColor, __IO uint16_t *_BackColor)
{
    *_TextColor = TextColor; *_BackColor = BackColor;
}

/**
  * @brief  Sets the Text color.
  * @param  Color: specifies the Text color code RGB(5-6-5).
  * @retval None
  */
void LCD_SetTextColor(__IO uint16_t Color)
{
    TextColor = Color;
}


/**
  * @brief  Sets the Background color.
  * @param  Color: specifies the Background color code RGB(5-6-5).
  * @retval None
  */
void LCD_SetBackColor(__IO uint16_t Color)
{
    BackColor = Color;
}

/**
  * @brief  Clears the hole LCD.
  * @param  Color: the color of the background.
  * @retval None
  */
void LCD_Clear(uint16_t Color)
{
    uint32_t index = 0;

    LCD_SetCursor(0,0); 
    LCD_WriteRAM_Prepare(); /* Prepare to write GRAM */

    // 视频加速 www.armjishu.com
    ClrCs
    SetRs
    LCD_Write(Color);
    for(index=0; index<(LCD_PIXEL_WIDTH * LCD_PIXEL_HEIGHT); index++)
    {
        ClrWr
        //LCD_Write(Color);
        SetWr
    }
    SetCs
}


/**
  * @brief  Sets the cursor position.
  * @param  Xpos: specifies the X position.
  * @param  Ypos: specifies the Y position. 
  * @retval None
  */
void LCD_SetCursor(uint16_t Xpos, uint16_t Ypos)
{
    if(DeviceIdCode==0x8989)
    {
        LCD_WriteReg(0x004e,Xpos);      //行
        LCD_WriteReg(0x004f,Ypos);      //列
    }
    else if((DeviceIdCode==0x9320))
    {
        LCD_WriteReg(0x0020,Xpos);      // 行
        LCD_WriteReg(0x0021,0x13f-Ypos);// 列
    }
    else if((DeviceIdCode==0x9919))
    {
        LCD_WriteReg(0x004e,Xpos);      // 行
        LCD_WriteReg(0x004f,Ypos);      // 列    
    }
    else
    {
        LCD_WriteReg(0x0020,Xpos);      // 列
        LCD_WriteReg(0x0021,0x13f-Ypos);// 行
    }
}

/**
  * @brief  Displays a Point.
  * @param  Color: specifies the Color.
  * @retval None
  */
void LCD_SetPoint(uint16_t Xpos, uint16_t Ypos, uint16_t Color)
{ 
    LCD_SetCursor(Xpos, Ypos);
    LCD_WriteRAM_Prepare(); /* Prepare to write GRAM */
    LCD_WriteRAM(Color);
    SetCs
}

/**
  * @brief  Displays a pixel.
  * @param  x: pixel x.
  * @param  y: pixel y.  
  * @retval None
  */
void PutPixel(int16_t Xpos, int16_t Ypos)
{ 
    if(Xpos < 0 || Xpos > LCD_PIXEL_HEIGHT || Ypos < 0 || Ypos > LCD_PIXEL_WIDTH)
    {
        return;  
    }
    LCD_SetPoint(Xpos, Ypos, TextColor);
}

/**
  * @brief  Displays a Point.
  * @param  Color: specifies the Color.
  * @retval None
  */
uint16_t LCD_GetPoint(uint16_t Xpos, uint16_t Ypos)
{ 
    uint16_t Color;
    LCD_SetCursor(Xpos, Ypos);

    /* Write 16-bit Index (then Read Reg) */
    ClrCs
    ClrRs
    ClrWr
    LCD_Write(LCD_REG_34);
    SetWr
    SetRs


    LCD_DB_AS_InPut();
    /* Read 16-bit Reg */
    ClrRd  //Dummy read
    SetRd
    ClrRd
    SetRd
    Color = LCD_Read();
    SetCs

    LCD_DB_AS_OutPut();

    return (Color);
}

void LCD_RAM_WrRdTest(void)
{
    uint16_t ReadData = 0;  
    
    LCD_SetPoint(1,1,0x0);
    ReadData = LCD_GetPoint(1,1);
    if(0x0 != ReadData)
    {
        LCD_DEBUG_PRINTF("\r\n Err: LCD_SetPoint(1,1,0x0) LCD_GetPoint(1,1)=0x%04X", ReadData);
    }
    LCD_SetPoint(1,1,0xFFFF);
    ReadData = LCD_GetPoint(1,1);
    if(0xFFFF != ReadData)
    {
        LCD_DEBUG_PRINTF("\r\n Err: LCD_SetPoint(1,1,0x0) LCD_GetPoint(1,1)=0x%04X", ReadData);
    }

    LCD_SetPoint(1,1,0x5a5a);
    ReadData = LCD_GetPoint(1,1);
    if(0x5a5a != ReadData)
    {
        LCD_DEBUG_PRINTF("\r\n Note: LCD Write order is R-G-B, but Read order is B-G-R");
    }
}


/*=====================================================================*/
void LCD_DrawBigPoint(uint16_t xScreen, uint16_t yScreen, uint16_t Color)
{
    LCD_SetPoint(xScreen,   yScreen,   Color);
    LCD_SetPoint(xScreen-1, yScreen,   Color);
    LCD_SetPoint(xScreen+1, yScreen,   Color);
    LCD_SetPoint(xScreen,   yScreen-1, Color);
    LCD_SetPoint(xScreen,   yScreen+1, Color);
    LCD_SetPoint(xScreen-1, yScreen-1, Color);
    LCD_SetPoint(xScreen-1, yScreen+1, Color);
    LCD_SetPoint(xScreen+1, yScreen-1, Color);
    LCD_SetPoint(xScreen+1, yScreen+1, Color);
}

/**
  * @brief  Displays a line.
  * @param  Xpos: specifies the X position.
  * @param  Ypos: specifies the Y position.
  * @param  Length: line length.
  * @param  Direction: line direction.
  *   This parameter can be one of the following values: Vertical or Horizontal.
  * @retval None
  */
void LCD_DrawLine(uint16_t Xpos, uint16_t Ypos, uint16_t Length, uint8_t Direction)
{
    uint32_t i = 0;

    LCD_SetCursor(Xpos, Ypos);

    if(Direction == LCD_DIR_HORIZONTAL)
    { 
        LCD_WriteRAM_Prepare(); /* Prepare to write GRAM */
        LCD_WriteRAM(TextColor);
        for(i = 1; i < Length; i++)
        {
            ClrWr
            SetWr
        }
    }
    else
    {
        for(i = 0; i < Length; i++)
        {
            LCD_WriteRAMWord(TextColor);
            Xpos++;
            LCD_SetCursor(Xpos, Ypos);
        }
    }
    SetCs
}

/**
  * @brief  Displays a Cross.
  * @param  Xpos: specifies the left X position.
  * @param  Ypos: specifies the botton Y position.
  * @retval None
  */
void LCD_DrawCross(uint16_t Xpos, uint16_t Ypos)
{
    LCD_DrawLine(Xpos, Ypos - 15, 30, LCD_DIR_HORIZONTAL); /* Horizontal Line */
    LCD_DrawLine(Xpos - 15, Ypos, 30, LCD_DIR_VERTICAL);   /* Vertical Line   */
}


/**
  * @brief  Displays a rectangle.
  * @param  Xpos: specifies the X position.
  * @param  Ypos: specifies the Y position.
  * @param  Height: display rectangle height.
  * @param  Width: display rectangle width.
  * @retval None
  */
void LCD_DrawRect(uint16_t Xpos, uint16_t Ypos, uint8_t Height, uint16_t Width)
{
    LCD_DrawLine(Xpos, Ypos, Width, LCD_DIR_HORIZONTAL);
    LCD_DrawLine((Xpos + Height), Ypos, Width, LCD_DIR_HORIZONTAL);

    LCD_DrawLine(Xpos, Ypos, Height, LCD_DIR_VERTICAL);
    LCD_DrawLine(Xpos, (Ypos + Width - 1), Height, LCD_DIR_VERTICAL);
}


/**
  * @brief  Displays a circle.
  * @param  Xpos: specifies the X position.
  * @param  Ypos: specifies the Y position.
  * @param  Radius
  * @retval None
  */
void LCD_DrawCircle(uint16_t Xpos, uint16_t Ypos, uint16_t Radius)
{
    int32_t   D;    /* Decision Variable */
    uint32_t  CurX; /* Current X Value */
    uint32_t  CurY; /* Current Y Value */ 

    D = 3 - (Radius << 1);
    CurX = 0;
    CurY = Radius;

    while (CurX <= CurY)
    {
        LCD_SetCursor(Xpos + CurX, Ypos + CurY);
        LCD_WriteRAMWord(TextColor);
        LCD_SetCursor(Xpos + CurX, Ypos - CurY);
        LCD_WriteRAMWord(TextColor);
        LCD_SetCursor(Xpos - CurX, Ypos + CurY);
        LCD_WriteRAMWord(TextColor);
        LCD_SetCursor(Xpos - CurX, Ypos - CurY);
        LCD_WriteRAMWord(TextColor);
        LCD_SetCursor(Xpos + CurY, Ypos + CurX);
        LCD_WriteRAMWord(TextColor);
        LCD_SetCursor(Xpos + CurY, Ypos - CurX);
        LCD_WriteRAMWord(TextColor);
        LCD_SetCursor(Xpos - CurY, Ypos + CurX);
        LCD_WriteRAMWord(TextColor);
        LCD_SetCursor(Xpos - CurY, Ypos - CurX);
        LCD_WriteRAMWord(TextColor);
        if (D < 0)
        { 
            D += (CurX << 2) + 6;
        }
        else
        {
            D += ((CurX - CurY) << 2) + 10;
            CurY--;
        }
        CurX++;
    }
}

/**
  * @brief  Displays a full rectangle.
  * @param  Xpos: specifies the X position.
  * @param  Ypos: specifies the Y position.
  * @param  Height: rectangle height.
  * @param  Width: rectangle width.
  * @retval None
  */
void LCD_DrawFullRect(uint16_t Xpos, uint16_t Ypos, uint16_t Width, uint16_t Height)
{
    uint16_t OldTextColor = TextColor;

    LCD_DrawRect(Xpos, Ypos, Width, Height);

    Width -= 2;
    Height--;
    Ypos++;

    LCD_SetTextColor(BackColor);

    while(Height--)
    {
        LCD_DrawLine(++Xpos, Ypos, Width, LCD_DIR_HORIZONTAL);    
    }

    LCD_SetTextColor(OldTextColor);
}

/**
  * @brief  Displays a full circle.
  * @param  Xpos: specifies the X position.
  * @param  Ypos: specifies the Y position.
  * @param  Radius
  * @retval None
  */
void LCD_DrawFullCircle(uint16_t Xpos, uint16_t Ypos, uint16_t Radius)
{
    int32_t  D;    /* Decision Variable */ 
    uint32_t  CurX;/* Current X Value */
    uint32_t  CurY;/* Current Y Value */ 
    uint16_t OldTextColor = TextColor;

    D = 3 - (Radius << 1);

    CurX = 0;
    CurY = Radius;

    LCD_SetTextColor(BackColor);

    while (CurX <= CurY)
    {
        if(CurY > 0) 
        {
            LCD_DrawLine(Xpos - CurX, Ypos - CurY, 2*CurY, LCD_DIR_HORIZONTAL);
            LCD_DrawLine(Xpos + CurX, Ypos - CurY, 2*CurY, LCD_DIR_HORIZONTAL);
        }

        if(CurX > 0) 
        {
            LCD_DrawLine(Xpos - CurY, Ypos - CurX, 2*CurX, LCD_DIR_HORIZONTAL);
            LCD_DrawLine(Xpos + CurY, Ypos - CurX, 2*CurX, LCD_DIR_HORIZONTAL);
        }
        if (D < 0)
        { 
            D += (CurX << 2) + 6;
        }
        else
        {
            D += ((CurX - CurY) << 2) + 10;
            CurY--;
        }
        CurX++;
    }

    LCD_SetTextColor(OldTextColor);
    LCD_DrawCircle(Xpos, Ypos, Radius);
}

/**
  * @brief  Displays an uni line (between two points).
  * @param  x1: specifies the point 1 x position.
  * @param  y1: specifies the point 1 y position.
  * @param  x2: specifies the point 2 x position.
  * @param  y2: specifies the point 2 y position.
  * @retval None
  */
void LCD_DrawUniLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
    int16_t deltax = 0, deltay = 0, x = 0, y = 0, xinc1 = 0, xinc2 = 0, 
    yinc1 = 0, yinc2 = 0, den = 0, num = 0, numadd = 0, numpixels = 0, 
    curpixel = 0;

    deltax = ABS(x2 - x1);        /* The difference between the x's */
    deltay = ABS(y2 - y1);        /* The difference between the y's */
    x = x1;                       /* Start x off at the first pixel */
    y = y1;                       /* Start y off at the first pixel */

    if (x2 >= x1)                 /* The x-values are increasing */
    {
        xinc1 = 1;
        xinc2 = 1;
    }
    else                          /* The x-values are decreasing */
    {
        xinc1 = -1;
        xinc2 = -1;
    }

    if (y2 >= y1)                 /* The y-values are increasing */
    {
        yinc1 = 1;
        yinc2 = 1;
    }
    else                          /* The y-values are decreasing */
    {
        yinc1 = -1;
        yinc2 = -1;
    }

    if (deltax >= deltay)         /* There is at least one x-value for every y-value */
    {
        xinc1 = 0;                  /* Don't change the x when numerator >= denominator */
        yinc2 = 0;                  /* Don't change the y for every iteration */
        den = deltax;
        num = deltax / 2;
        numadd = deltay;
        numpixels = deltax;         /* There are more x-values than y-values */
    }
    else                          /* There is at least one y-value for every x-value */
    {
        xinc2 = 0;                  /* Don't change the x for every iteration */
        yinc1 = 0;                  /* Don't change the y when numerator >= denominator */
        den = deltay;
        num = deltay / 2;
        numadd = deltax;
        numpixels = deltay;         /* There are more y-values than x-values */
    }

    for (curpixel = 0; curpixel <= numpixels; curpixel++)
    {
        PutPixel(x, y);             /* Draw the current pixel */
        num += numadd;              /* Increase the numerator by the top of the fraction */
        if (num >= den)             /* Check if numerator >= denominator */
        {
            num -= den;               /* Calculate the new numerator value */
            x += xinc1;               /* Change the x as appropriate */
            y += yinc1;               /* Change the y as appropriate */
        }
        x += xinc2;                 /* Change the x as appropriate */
        y += yinc2;                 /* Change the y as appropriate */
    }
}

/**
  * @brief  Displays an polyline (between many points).
  * @param  Points: pointer to the points array.
  * @param  PointCount: Number of points.
  * @retval None
  */
void LCD_PolyLine(pPoint Points, uint16_t PointCount)
{
    int16_t X = 0, Y = 0;

    if(PointCount < 2)
    {
        return;
    }

    while(--PointCount)
    {
        X = Points->X;
        Y = Points->Y;
        Points++;
        LCD_DrawUniLine(X, Y, Points->X, Points->Y);
    }
}

/**
  * @brief  Displays an relative polyline (between many points).
  * @param  Points: pointer to the points array.
  * @param  PointCount: Number of points.
  * @param  Closed: specifies if the draw is closed or not.
  *           1: closed, 0 : not closed.
  * @retval None
  */
static void LCD_PolyLineRelativeClosed(pPoint Points, uint16_t PointCount, uint16_t Closed)
{
    int16_t X = 0, Y = 0;
    pPoint First = Points;

    if(PointCount < 2)
    {
        return;
    }  
    X = Points->X;
    Y = Points->Y;
    while(--PointCount)
    {
        Points++;
        LCD_DrawUniLine(X, Y, X + Points->X, Y + Points->Y);
        X = X + Points->X;
        Y = Y + Points->Y;
    }
    if(Closed)
    {
        LCD_DrawUniLine(First->X, First->Y, X, Y);
    }  
}

/**
  * @brief  Displays a closed polyline (between many points).
  * @param  Points: pointer to the points array.
  * @param  PointCount: Number of points.
  * @retval None
  */
void LCD_ClosedPolyLine(pPoint Points, uint16_t PointCount)
{
    LCD_PolyLine(Points, PointCount);
    LCD_DrawUniLine(Points->X, Points->Y, (Points+PointCount-1)->X, (Points+PointCount-1)->Y);
}

/**
  * @brief  Displays a relative polyline (between many points).
  * @param  Points: pointer to the points array.
  * @param  PointCount: Number of points.
  * @retval None
  */
void LCD_PolyLineRelative(pPoint Points, uint16_t PointCount)
{
    LCD_PolyLineRelativeClosed(Points, PointCount, 0);
}

/**
  * @brief  Displays a closed relative polyline (between many points).
  * @param  Points: pointer to the points array.
  * @param  PointCount: Number of points.
  * @retval None
  */
void LCD_ClosedPolyLineRelative(pPoint Points, uint16_t PointCount)
{
    LCD_PolyLineRelativeClosed(Points, PointCount, 1);
}


/**
  * @brief  Displays a  full polyline (between many points).
  * @param  Points: pointer to the points array.
  * @param  PointCount: Number of points.
  * @retval None
  */
void LCD_FillPolyLine(pPoint Points, uint16_t PointCount)
{
    /*  public-domain code by Darel Rex Finley, 2007 */
    uint16_t  nodes = 0, nodeX[MAX_POLY_CORNERS], pixelX = 0, pixelY = 0, i = 0,
    j = 0, swap = 0;
    uint16_t  IMAGE_LEFT = 0, IMAGE_RIGHT = 0, IMAGE_TOP = 0, IMAGE_BOTTOM = 0;

    IMAGE_LEFT = IMAGE_RIGHT = Points->X;
    IMAGE_TOP= IMAGE_BOTTOM = Points->Y;

    for(i = 1; i < PointCount; i++)
    {
        pixelX = POLY_X(i);
        if(pixelX < IMAGE_LEFT)
        {
            IMAGE_LEFT = pixelX;
        }
        if(pixelX > IMAGE_RIGHT)
        {
            IMAGE_RIGHT = pixelX;
        }

        pixelY = POLY_Y(i);
        if(pixelY < IMAGE_TOP)
        { 
            IMAGE_TOP = pixelY;
        }
        if(pixelY > IMAGE_BOTTOM)
        {
            IMAGE_BOTTOM = pixelY;
        }
    }

    LCD_SetTextColor(BackColor);  

    /*  Loop through the rows of the image. */
    for (pixelY = IMAGE_TOP; pixelY < IMAGE_BOTTOM; pixelY++) 
    {  
        /* Build a list of nodes. */
        nodes = 0; j = PointCount-1;

        for (i = 0; i < PointCount; i++) 
        {
            if (POLY_Y(i)<(double) pixelY && POLY_Y(j)>=(double) pixelY || POLY_Y(j)<(double) pixelY && POLY_Y(i)>=(double) pixelY) 
            {
                nodeX[nodes++]=(int) (POLY_X(i)+((pixelY-POLY_Y(i))*(POLY_X(j)-POLY_X(i)))/(POLY_Y(j)-POLY_Y(i))); 
            }
            j = i; 
        }

        /* Sort the nodes, via a simple 揃ubble?sort. */
        i = 0;
        while (i < nodes-1) 
        {
            if (nodeX[i]>nodeX[i+1]) 
            {
                swap = nodeX[i]; 
                nodeX[i] = nodeX[i+1]; 
                nodeX[i+1] = swap; 
            if(i)
            {
                i--; 
            }
            }
            else 
            {
                i++;
            }
        }

        /*  Fill the pixels between node pairs. */
        for (i = 0; i < nodes; i+=2) 
        {
            if(nodeX[i] >= IMAGE_RIGHT) 
            {
                break;
            }
            if(nodeX[i+1] > IMAGE_LEFT) 
            {
                if (nodeX[i] < IMAGE_LEFT)
                {
                    nodeX[i]=IMAGE_LEFT;
                }
                if(nodeX[i+1] > IMAGE_RIGHT)
                {
                    nodeX[i+1] = IMAGE_RIGHT;
                }
                LCD_SetTextColor(BackColor);
                LCD_DrawLine(pixelY, nodeX[i+1], nodeX[i+1] - nodeX[i], LCD_DIR_HORIZONTAL);
                LCD_SetTextColor(TextColor);
                PutPixel(pixelY, nodeX[i+1]);
                PutPixel(pixelY, nodeX[i]);
                /* for (j=nodeX[i]; j<nodeX[i+1]; j++) PutPixel(j,pixelY); */
            }
        }
    } 

    /* draw the edges */
    LCD_SetTextColor(TextColor);
}


/**
  * @brief  Writes index to select the LCD register.
  * @param  LCD_Reg: address of the selected register.
  * @retval None
  */
void LCD_WriteRegIndex(uint8_t LCD_Reg)
{
    ClrCs
    ClrRs
    ClrWr
    LCD_Write(LCD_Reg);
    SetWr
    SetCs
}


/**
  * @brief  Reads the selected LCD Register.
  * @param  None
  * @retval LCD Register Value.
  */
uint16_t LCD_ReadReg(uint8_t LCD_Reg)
{
    uint16_t data;

    /* Write 16-bit Index (then Read Reg) */
    ClrCs
    ClrRs
    ClrWr
    LCD_Write(LCD_Reg);
    SetWr

    LCD_DB_AS_InPut();

    /* Read 16-bit Reg */
    SetRs
    ClrRd
    SetRd
    data = LCD_Read(); 
    SetCs
    LCD_DB_AS_OutPut();

    return    data;
}


/**
  * @brief  Prepare to write to the LCD RAM.
  * @param  None
  * @retval None
  */
void LCD_WriteRAM_Prepare(void)
{
    /* Write 16-bit Index, then Write Reg */
    ClrCs
    ClrRs
    ClrWr
    LCD_Write(LCD_REG_34);
    SetWr
    SetRs
}


/**
  * @brief  Writes 1 word to the LCD RAM.
  * @param  RGB_Code: the pixel color in RGB mode (5-6-5).
  * @retval None
  */
void LCD_WriteRAMWord(uint16_t RGB_Code)
{
    LCD_WriteRAM_Prepare();
    LCD_WriteRAM(RGB_Code);
    SetCs
}


/**
  * @brief  Writes to the selected LCD register.
  * @param  LCD_Reg: address of the selected register.
  * @param  LCD_RegValue: value to write to the selected register.
  * @retval None
  */
void LCD_WriteReg(uint8_t LCD_Reg, uint16_t LCD_RegValue)
{

    /* Write 16-bit Index, then Write Reg */
    ClrCs
    ClrRs
    ClrWr
    LCD_Write(LCD_Reg);
    SetWr
    /* Write 16-bit Reg */
    SetRs
    ClrWr
    LCD_Write(LCD_RegValue);
    SetWr
    SetCs
}


/**
  * @brief  Writes to the LCD RAM.
  * @param  RGB_Code: the pixel color in RGB mode (5-6-5).
  * @retval None
  */
void LCD_WriteRAM(uint16_t RGB_Code)
{
    ClrWr
    LCD_Write(RGB_Code);
    SetWr
}


/**
  * @brief  Power on the LCD.
  * @param  None
  * @retval None
  */
void LCD_PowerOn(void)
{ 
}


/*******************************************************************************
* Function Name  : LCD_Pins_Config
* Description    : Configures LCD Pins
                   Push-Pull mode.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void LCD_Pins_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(LCD_CLK_RS  | LCD_CLK_DATA |
                            LCD_CLK_WR | LCD_CLK_RD   |
                            LCD_CLK_CS, ENABLE);

    // DB15--0
    GPIO_InitStructure.GPIO_Pin = LCD_PIN_DATA;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(LCD_PORT_DAT, &GPIO_InitStructure);

    //LCD_Pin_WR
    GPIO_InitStructure.GPIO_Pin = LCD_Pin_WR;
    GPIO_Init(LCD_PORT_WR, &GPIO_InitStructure);

    //LCD_Pin_CS
    GPIO_InitStructure.GPIO_Pin = LCD_Pin_CS;
    GPIO_Init(LCD_PORT_CS, &GPIO_InitStructure);

    //LCD_Pin_RS
    GPIO_InitStructure.GPIO_Pin = LCD_Pin_RS;
    GPIO_Init(LCD_PORT_RS, &GPIO_InitStructure);

    //LCD_Pin_RD
    GPIO_InitStructure.GPIO_Pin = LCD_Pin_RD;
    GPIO_Init(LCD_PORT_RD, &GPIO_InitStructure);

    SetCs
    SetWr
    SetRd
    SetRs
}
 
/*******************  (C) COPYRIGHT 2013 www.armjishu.com  *****END OF FILE****/
