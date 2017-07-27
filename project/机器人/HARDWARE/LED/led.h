#ifndef _LED_BAN_
#define _LED_BAN_

void LED_Init(void);

#define led0 PBout(8)	//黄色灯  0为亮
#define led1 PBout(9)	//红色灯  0为亮

#endif
