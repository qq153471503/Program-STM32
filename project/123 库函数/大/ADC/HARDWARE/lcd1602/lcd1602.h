#ifndef _LCD_1602_H
#define _LCD_1602_H

#include "sys.h"

void LcdWriteData(u8 dat);
void LcdWriteCmd(u8 cmd);
void LcdInit(void);
void LcdAny(u8 cmd,u8 dat);

/*库函数操作*/
//#define lcden0 GPIO_ResetBits(GPIOC,GPIO_Pin_2)		
//#define lcden1 GPIO_SetBits(GPIOC,GPIO_Pin_2)

//#define lcdrw0 GPIO_ResetBits(GPIOC,GPIO_Pin_1)
//#define lcdrw1 GPIO_SetBits(GPIOC,GPIO_Pin_1)

//#define lcdrs0 GPIO_ResetBits(GPIOC,GPIO_Pin_0)
//#define lcdrs1 GPIO_SetBits(GPIOC,GPIO_Pin_0)


/*位带操作*/
#define lcden0 PCout(2) = 0
#define lcden1 PCout(2) = 1

#define lcdrw0 PCout(1) = 0
#define lcdrw1 PCout(1) = 1

#define lcdrs0 PCout(0) = 0
#define lcdrs1 PCout(0) = 1

#define LcdData GPIOD -> ODR 

#endif
