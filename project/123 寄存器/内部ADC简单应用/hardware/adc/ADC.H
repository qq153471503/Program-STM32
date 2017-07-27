#ifndef _ADC_H
#define _ADC_H

#include"sys.h"

#define ADC_CH0  0  //Í¨µÀ1	
#define ADC_CH1  1
#define ADC_CH2  2
#define ADC_CH3  3

u16 AdcGet(u8 ch);
void AdcInit(void);

#endif


