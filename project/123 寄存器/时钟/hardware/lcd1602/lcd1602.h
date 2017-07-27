#ifndef _LCD1602_H_
#define _LCD1602_H_

#include"sys.h"
#include"delay.h"

#define lcden0 GPIOC -> BRR = 0x0004
#define lcden1 GPIOC -> BSRR = 0X0004

#define lcdrs0 GPIOC -> BRR = 0X0001
#define lcdrs1 GPIOC -> BSRR = 0X0001

#define lcdrw0 GPIOC -> BRR = 0X0002
#define lcdrw1 GPIOC -> BSRR = 0X0002

#define Data GPIOD -> ODR

void LcdWriteCmd(u8 cmd);
void LcdWriteData(u8 dat);
void LcdInit(void);
void LcdWriteSfm(u8 add,char dat);
void LcdWriteNyr(u8 add,char dat);
void DisInit(void);

#endif
