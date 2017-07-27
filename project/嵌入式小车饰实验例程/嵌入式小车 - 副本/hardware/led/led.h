#ifndef _LED_H_
#define _LED_H_

#define led0 PDout(8)
#define led1 PDout(9)
#define led2 PDout(10)
#define led3 PDout(11)

#define beep PDout(12)

#include"sys.h"

void LedInit(void);

#endif
