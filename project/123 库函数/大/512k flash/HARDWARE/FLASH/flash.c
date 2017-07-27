#include "flash.h"

u16 FLASH_BUF[FLASH_SECTOR_SIZE/2]; //2K缓存区

//直接写入 不检查
void Flash_WriteLenByte(u32 address,u16 *p, u16 num)
{
	u16 i;
	for(i=0; i<num; i++)
	{
		FLASH_ProgramHalfWord(address,p[i]);
		address+=2;
	}
}

//读出半字
u16 FLASH_ReadHalfWord(u32 address)
{
	return *(vu16 *)address;
}

//指定地址读出指定长度的数据
void Flash_Read(u32 address,u16 *p,u16 num)
{
	u16 i;
	for(i=0; i<num; i++)
	{
		p[i] = FLASH_ReadHalfWord(address);
		address+=2;
	}
}
//指定地址写入指定长度的数据
void Flash_Write(u32 address,u16 *p,u16 num)
{
	u32 SecorAdd;			//扇区地址
	u16 SecorOffset;		//扇区地址偏移(16位计算)
	u16 SecorRemnant;		//扇区剩余地址(16位计算)
	u16 i;
	u32 OffsetAdd;			//地址偏移
	
	//判断写入的地址的有效性
	if(address<STM32_FLASH_BASE || (address>=(STM32_FLASH_BASE+1024*STM32_FlASH_SIZE)))
		return ;
	
	FLASH_Unlock();		//FLASH 解锁
	OffsetAdd = address-STM32_FLASH_BASE;			//实际偏移地址
	SecorAdd = OffsetAdd/FLASH_SECTOR_SIZE;			//扇区的地址
	SecorOffset = (OffsetAdd%FLASH_SECTOR_SIZE)/2;	//在扇区内的偏移
	SecorRemnant = FLASH_SECTOR_SIZE/2-SecorOffset;	//扇区内的剩余空间
	
	//要写入的数据在该扇区内可以容纳下的情况
	if(num <= SecorRemnant)		
		SecorRemnant = num;
	
	while(1)
	{
		//读出整个扇区的内容
		Flash_Read(SecorAdd*FLASH_SECTOR_SIZE+STM32_FLASH_BASE,FLASH_BUF,FLASH_SECTOR_SIZE/2);
		//校验数据是否被擦除
		for(i=0; i<SecorRemnant; i++)
		{
			if(FLASH_BUF[SecorOffset+i] != 0xFFFF)
				break;						//未被擦除 直接跳出
		}
		if(i<SecorRemnant)	//需要擦除
		{
			//擦除整个扇区
			FLASH_ErasePage(SecorAdd*FLASH_SECTOR_SIZE+STM32_FLASH_BASE);
			for(i=0; i<SecorRemnant; i++)
			{
				FLASH_BUF[i] = p[i];
			}
			//写入整个扇区
			Flash_WriteLenByte(SecorAdd*FLASH_SECTOR_SIZE+STM32_FLASH_BASE,FLASH_BUF,FLASH_SECTOR_SIZE/2);
		}
		else		//已经擦除了的 直接写入
			Flash_WriteLenByte(address,p,SecorRemnant);
		
		if(num == SecorRemnant)  //写入结束
			break;
		else					//写入未结束 
		{
			SecorAdd++;					//扇区地址加1
			SecorOffset = 0;			//扇区内偏移为0
			p += SecorRemnant;			//指针偏移
			address +=SecorRemnant;		//写地址偏移
			num -= SecorRemnant;		//剩余要写入数据的长度
			//这个扇区还是写不完
			if(num>(FLASH_SECTOR_SIZE/2))
				SecorRemnant = FLASH_SECTOR_SIZE/2;
			else 
				SecorRemnant = num;	
		}
	}
	FLASH_Lock();	//FLASK 上锁
}

