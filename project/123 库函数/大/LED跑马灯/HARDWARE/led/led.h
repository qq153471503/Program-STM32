#ifndef _LED_H
#define _LED_H

#include "sys.h"

void LedInit(void);

#define led0 PBout(5)
#define led1 PEout(5)

#endif
