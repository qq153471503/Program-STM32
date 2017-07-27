#ifndef _DMA_H
#define _DMA_H

#include "sys.h"

void DMA_Config(DMA_Channel_TypeDef * DMA_CHx,u32 p,u32 m,u16 length);
void DMA_Enable(DMA_Channel_TypeDef * DMA_CHx);

#endif
