#ifndef _DMA_H
#define _DMA_H

#include "sys.h"

void DMA_Config(void);
void DMA_Enable(DMA_Channel_TypeDef * DMA_CHx,u8 x);
extern u16 ADC_Data;

#endif
