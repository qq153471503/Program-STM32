#ifndef _SPI_H
#define _SPI_H
#include "sys.h"


void SPI2_Init(void);
u8 SPI2_ReadWriteByte(u8 txd);
void SPI2_Set_Speed(u8 SPI_BaudRatePrescaler);

#endif
