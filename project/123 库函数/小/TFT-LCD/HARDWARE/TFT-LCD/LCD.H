#ifndef _LCD_H
#define _LCD_H

#include "sys.h"

extern u16 PointColor;//默认红色    
extern u16 BackColor; //背景颜色.默认为白色

#define USE_HORIZONTAL  1	//定义是否使用横屏 		0,不使用.1,使用.

#if USE_HORIZONTAL == 1	//使用横屏
#define LCD_W 320
#define LCD_H 240
#else
#define LCD_W 240
#define LCD_H 320
#endif

#define	LCD_LED PCout(10) //LCD背光    		 PC10 

#define DataOut(x) GPIOB -> ODR = x;	//数据输出
#define DataIn GPIOB -> IDR	//数据输入

#define	lcdcs1  GPIOC->BSRR=1<<9    //片选端口  	     PC9
#define	lcdrs1	GPIOC->BSRR=1<<8    //数据/命令          PC8	   
#define lcdwr1	GPIOC->BSRR=1<<7    //写数据			 PC7
#define	lcdrd1	GPIOC->BSRR=1<<6    //读数据			 PC6
								    
#define	lcdcs0  GPIOC->BRR=1<<9     //片选端口  	     PC9
#define	lcdrs0	GPIOC->BRR=1<<8     //数据/命令          PC8	   
#define	lcdwr0	GPIOC->BRR=1<<7     //写数据			 PC7
#define	lcdrd0	GPIOC->BRR=1<<6     //读数据			 PC6

//扫描方向定义
#define L2R_U2D  0 //从左到右,从上到下
#define L2R_D2U  1 //从左到右,从下到上
#define R2L_U2D  2 //从右到左,从上到下
#define R2L_D2U  3 //从右到左,从下到上

#define U2D_L2R  4 //从上到下,从左到右
#define U2D_R2L  5 //从上到下,从右到左
#define D2U_L2R  6 //从下到上,从左到右
#define D2U_R2L  7 //从下到上,从右到左

#define DFT_SCAN_DIR  L2R_U2D  //默认的扫描方向

//画笔颜色
#define WHITE         	 0xFFFF
#define BLACK         	 0x0000	  
#define BLUE         	 0x001F  
#define BRED             0XF81F
#define GRED 			 0XFFE0
#define GBLUE			 0X07FF
#define RED           	 0xF800
#define MAGENTA       	 0xF81F
#define GREEN         	 0x07E0
#define CYAN          	 0x7FFF
#define YELLOW        	 0xFFE0
#define BROWN 			 0XBC40 //棕色
#define BRRED 			 0XFC07 //棕红色
#define GRAY  			 0X8430 //灰色

//GUI颜色
#define DARKBLUE      	 0X01CF	//深蓝色
#define LIGHTBLUE      	 0X7D7C	//浅蓝色  
#define GRAYBLUE       	 0X5458 //灰蓝色
//以上三色为PANEL/*面板*/的颜色 
 
#define LIGHTGREEN     	 0X841F //浅绿色
//#define LIGHTGRAY        0XEF5B //浅灰色(PANNEL)
#define LGRAY 			 0XC618 //浅灰色(PANNEL),窗体背景色

#define LGRAYBLUE        0XA651 //浅灰蓝色(中间层颜色)
#define LBBLUE           0X2B12 //浅棕蓝色(选择条目的反色)

/*写8位数据 用宏定义 提高速度*/
#define LcdWriteData(dat)	{lcdrs1;lcdcs0; DataOut(dat); lcdwr0; lcdwr1; lcdcs1; }
/*写寄存器 类似1602/12864的写命令*/
//#define LcdWriteCmd(cmd)	{lcdrs0;lcdcs0; DataOut(cmd); lcdwr0; lcdwr1; lcdcs1; }

void LcdInit(void);	//	初始化函数
void LcdWriteReg(u8 Reg,u16 RegValue);//写入寄存器 值

u16 LcdReadReg(u8 Reg);
u16 LcdReadPoint(u16 x,u16 y);

void LcdWriteRAMPrepare(void);
void LcdWriteRAM(u16 RGB_Code);

u16 Lcd_BGR2RGB(u16 color);

void LcdSetCursor(u16 Xpos,u16 Ypos);
void LcdSetWindows(u16 StartX,u16 StartY,u16 EndX,u16 EndY);/*开窗显示图片函数*/

void LcdClearScreen(u16 color);
void LcdScanDir(u8 dir);
void LcdWriteCmd(u8 cmd);

void LcdShowString(u16 x,u16 y,const u8 *p);
void LcdShowChar(u16 x,u16 y,u8 num,u8 size,u8 mode);

void LcdShowPicture(u16 xStart, u16 yStart, u16 xEnd, u16 yEnd);

void LcdDrawPoint(u16 x,u16 y);//画点
void LcdDrawLine(u16 x1,u16 y1,u16 x2,u16 y2);//画线
void LcdDrawCircle(u16 x0,u16 y0,u8 r);//画圆
void LcdDrawRectangle(u16 x1,u16 y1,u16 x2,u16 y2);//画矩形
void LcdDrawPicture(u16 StartX,u16 StartY,u8 wide,u16 high);/*画图*/






//9320/9325 LCD寄存器
#define R0             0x00
#define R1             0x01
#define R2             0x02
#define R3             0x03
#define R4             0x04
#define R5             0x05
#define R6             0x06
#define R7             0x07
#define R8             0x08
#define R9             0x09
#define R10            0x0A
#define R12            0x0C
#define R13            0x0D
#define R14            0x0E
#define R15            0x0F
#define R16            0x10
#define R17            0x11
#define R18            0x12
#define R19            0x13
#define R20            0x14
#define R21            0x15
#define R22            0x16
#define R23            0x17
#define R24            0x18
#define R25            0x19
#define R26            0x1A
#define R27            0x1B
#define R28            0x1C
#define R29            0x1D
#define R30            0x1E
#define R31            0x1F
#define R32            0x20
#define R33            0x21
#define R34            0x22
#define R36            0x24
#define R37            0x25
#define R40            0x28
#define R41            0x29
#define R43            0x2B
#define R45            0x2D
#define R48            0x30
#define R49            0x31
#define R50            0x32
#define R51            0x33
#define R52            0x34
#define R53            0x35
#define R54            0x36
#define R55            0x37
#define R56            0x38
#define R57            0x39
#define R59            0x3B
#define R60            0x3C
#define R61            0x3D
#define R62            0x3E
#define R63            0x3F
#define R64            0x40
#define R65            0x41
#define R66            0x42
#define R67            0x43
#define R68            0x44
#define R69            0x45
#define R70            0x46
#define R71            0x47
#define R72            0x48
#define R73            0x49
#define R74            0x4A
#define R75            0x4B
#define R76            0x4C
#define R77            0x4D
#define R78            0x4E
#define R79            0x4F
#define R80            0x50
#define R81            0x51
#define R82            0x52
#define R83            0x53
#define R96            0x60
#define R97            0x61
#define R106           0x6A
#define R118           0x76
#define R128           0x80
#define R129           0x81
#define R130           0x82
#define R131           0x83
#define R132           0x84
#define R133           0x85
#define R134           0x86
#define R135           0x87
#define R136           0x88
#define R137           0x89
#define R139           0x8B
#define R140           0x8C
#define R141           0x8D
#define R143           0x8F
#define R144           0x90
#define R145           0x91
#define R146           0x92
#define R147           0x93
#define R148           0x94
#define R149           0x95
#define R150           0x96
#define R151           0x97
#define R152           0x98
#define R153           0x99
#define R154           0x9A
#define R157           0x9D
#define R192           0xC0
#define R193           0xC1
#define R229           0xE5	

#endif
