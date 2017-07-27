#ifndef _KEY_H_
#define _KEY_H_

		#include"delay.h"
		#include"sys.h"
		#include"stm32f10x.h"

void KeyInit(void);
u8 KeyScan(u8 mode);

/*位带操作*/
//#define key0 PAin(13)
//#define key1 PAin(15)
//#define wk_up PAin(0)

/*库函数操作*/
#define key0 GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_13)
#define key1 GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_15)
#define wk_up GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)


#endif

