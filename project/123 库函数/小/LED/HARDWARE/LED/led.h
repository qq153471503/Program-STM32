#ifndef _LED_H_
#define _LED_H_

#include"sys.h"

void LedInit(void);

#define led0 PAout(8)
#define led1 PDout(2)

#endif
