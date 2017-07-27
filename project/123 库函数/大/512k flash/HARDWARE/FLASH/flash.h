#ifndef _FLASH_H_
#define _FLASH_H_

#include "sys.h"

#define STM32_FlASH_SIZE 512				//所选STM单片机FLASK大小

//flash 起始地址
#define STM32_FLASH_BASE (uint32_t)0X08000000

//STM32ZET6 FLASH 每个扇区是2K 2*1024=2048个字节
#define FLASH_SECTOR_SIZE	2048

//以下两个函数功能：从指定地址开始写入/读取 指定长度的数据
void Flash_Write(u32 address,u16 *p,u16 num);
void Flash_Read(u32 address,u16 *p,u16 num);

//读半字
u16 FLASH_ReadHalfWord(u32 address);		 

//不检查的写入
void Flash_WriteLenByte(u32 address,u16 *p, u16 num);

#endif
