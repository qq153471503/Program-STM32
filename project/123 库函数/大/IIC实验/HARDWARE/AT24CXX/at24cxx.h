#ifndef _AT24CXX_H
#define _AT24CXX_H

#include "sys.h"
#include "delay.h"
#include "iic.h"

void AT24CXX_Init(void);
void AT24CXX_WriteOneByte(u16 addr,u8 data);
u8 AT24CXX_ReadOneByte(u16 addr);
void AT24CXX_WriteLengthByte(u16 addr,u8 data,u8 len);
u32 AT24CXX_ReadLengthByte(u16 addr,u8 len);
u8 AT24CXX_Check(void);

void AT24CXX_Write(u16 WriteAddr,u8 *pBuffer,u16 NumToWrite);
void AT24CXX_Read(u16 ReadAddr,u8 *pBuffer,u16 NumToRead);

#endif

