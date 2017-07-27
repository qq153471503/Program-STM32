#ifndef _LCD12864_H
#define _LCD12864_H

#include"sys.h"
#include"delay.h"

#define LcdData GPIOD -> ODR

#define lcden0 GPIOE -> BRR = 0x0004
#define lcden1 GPIOE -> BSRR = 0X0004

#define lcdrs0 GPIOE -> BRR = 0X0001
#define lcdrs1 GPIOE -> BSRR = 0X0001

#define lcdrw0 GPIOE -> BRR = 0X0002
#define lcdrw1 GPIOE -> BSRR = 0X0002

void LcdWriteCmd(u8 cmd);
void LcdWriteData(u8 dat);
void LcdDisplay(u8 cmd,u8 dat);
void LcdInit(void);
u8 LcdReadBusy(void);
void LcdDisPic(u8 *a);
void LcdQingPing(void);

extern u8 Photo1[];
extern u8 Photo2[];
extern u8 Photo3[];
extern u8 Photo4[];
extern u8 Photo5[];

#endif

