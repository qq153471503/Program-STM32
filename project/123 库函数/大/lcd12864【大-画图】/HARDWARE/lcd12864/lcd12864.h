#ifndef _LCD12864_H_
#define _LCD12864_H_

#include "sys.h"

///*此处注释是战舰板端口定义*//*慢速IO口*/
//#define lcden0 GPIO_ResetBits(GPIOC,GPIO_Pin_2)
//#define lcden1 GPIO_SetBits(GPIOC,GPIO_Pin_2)

//#define lcdrw0 GPIO_ResetBits(GPIOC,GPIO_Pin_1)
//#define lcdrw1 GPIO_SetBits(GPIOC,GPIO_Pin_1)

//#define lcdrs0 GPIO_ResetBits(GPIOC,GPIO_Pin_0)
//#define lcdrs1 GPIO_SetBits(GPIOC,GPIO_Pin_0)

/*此处注释是战舰板端口定义*//*快速io口*/

#define lcden0 GPIOC -> BRR = 0X0004
#define lcden1 GPIOC -> BSRR = 0X0004

#define lcdrw0 GPIOC -> BRR = 0X0002
#define lcdrw1 GPIOC -> BSRR = 0X0002

#define lcdrs0 GPIOC -> BRR = 0X0001
#define lcdrs1 GPIOC -> BSRR = 0X0001


#define LcdData GPIOD -> ODR

void LcdWriteData(u8 dat);
void LcdWriteCmd(u8 cmd);
void LcdInit(void);
void LcdWriteChar(u8 cmd,u8 dat);/*指定位置写字符*/
void LcdShowPic(u8 *a);
void LcdCleanScreen(void);

#endif

