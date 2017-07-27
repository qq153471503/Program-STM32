#ifndef _ADC_H_
#define _ADC_H_

#include "sys.h"

void AdcInit(void);
u16 GetAdcValue(u8 ch);
u16 GetAdcAverage(u8 ch,u8 times);

#endif 
