#ifndef _FLASH_H_
#define _FLASH_H_

#include "sys.h"

#define STM32_FlASH_SIZE 512				//��ѡSTM��Ƭ��FLASK��С

//flash ��ʼ��ַ
#define STM32_FLASH_BASE (uint32_t)0X08000000

//STM32ZET6 FLASH ÿ��������2K 2*1024=2048���ֽ�
#define FLASH_SECTOR_SIZE	2048

//���������������ܣ���ָ����ַ��ʼд��/��ȡ ָ�����ȵ�����
void Flash_Write(u32 address,u16 *p,u16 num);
void Flash_Read(u32 address,u16 *p,u16 num);

//������
u16 FLASH_ReadHalfWord(u32 address);		 

//������д��
void Flash_WriteLenByte(u32 address,u16 *p, u16 num);

#endif
