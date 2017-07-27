#ifndef _LED_H
#define _LED_H


/*led灯 给0 点亮*/
#include "sys.h"

void LedInit(void);

#define led0 PBout(5) /*红色灯*/
#define led1 PEout(5) /*黄色灯*/

#endif
