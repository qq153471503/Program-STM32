#ifndef _LCD1602_H_
#define _LCD1602_H_

#include"sys.h"
#include"delay.h"

#define lcden0 GPIOC -> BRR = 0x0004  //PC2
#define lcden1 GPIOC -> BSRR = 0X0004

#define lcdrs0 GPIOC -> BRR = 0X0001	  //PC0
#define lcdrs1 GPIOC -> BSRR = 0X0001

#define lcdrw0 GPIOC -> BRR = 0X0002		  //PC1
#define lcdrw1 GPIOC -> BSRR = 0X0002

#define Data GPIOD -> ODR					//PD ¿Ú

void LcdWriteCmd(u8 cmd);
void LcdWriteData(u8 dat);
void LcdInit(void);
void LcdWritwChar(u8 cmd,u8 dat);

#endif
