#ifndef _SRAM_H
#define _SRAM_H

#include "sys.h"

void SRAM_FSMC_Init(void);
void SRAM_FSMC_Write(u8 *pBuffer,u32 address,u32 NumHalfword);
void SRAM_FSMC_Read(u8 *pBuffer,u32 address,u32 NumHalfword);
void SRAM_FSMC_TEST_WRITE(u32 addr);
u8 SRAM_FSMC_TEST_READ(u32 addr);

#endif

