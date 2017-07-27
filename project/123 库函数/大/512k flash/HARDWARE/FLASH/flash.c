#include "flash.h"

u16 FLASH_BUF[FLASH_SECTOR_SIZE/2]; //2K������

//ֱ��д�� �����
void Flash_WriteLenByte(u32 address,u16 *p, u16 num)
{
	u16 i;
	for(i=0; i<num; i++)
	{
		FLASH_ProgramHalfWord(address,p[i]);
		address+=2;
	}
}

//��������
u16 FLASH_ReadHalfWord(u32 address)
{
	return *(vu16 *)address;
}

//ָ����ַ����ָ�����ȵ�����
void Flash_Read(u32 address,u16 *p,u16 num)
{
	u16 i;
	for(i=0; i<num; i++)
	{
		p[i] = FLASH_ReadHalfWord(address);
		address+=2;
	}
}
//ָ����ַд��ָ�����ȵ�����
void Flash_Write(u32 address,u16 *p,u16 num)
{
	u32 SecorAdd;			//������ַ
	u16 SecorOffset;		//������ַƫ��(16λ����)
	u16 SecorRemnant;		//����ʣ���ַ(16λ����)
	u16 i;
	u32 OffsetAdd;			//��ַƫ��
	
	//�ж�д��ĵ�ַ����Ч��
	if(address<STM32_FLASH_BASE || (address>=(STM32_FLASH_BASE+1024*STM32_FlASH_SIZE)))
		return ;
	
	FLASH_Unlock();		//FLASH ����
	OffsetAdd = address-STM32_FLASH_BASE;			//ʵ��ƫ�Ƶ�ַ
	SecorAdd = OffsetAdd/FLASH_SECTOR_SIZE;			//�����ĵ�ַ
	SecorOffset = (OffsetAdd%FLASH_SECTOR_SIZE)/2;	//�������ڵ�ƫ��
	SecorRemnant = FLASH_SECTOR_SIZE/2-SecorOffset;	//�����ڵ�ʣ��ռ�
	
	//Ҫд��������ڸ������ڿ��������µ����
	if(num <= SecorRemnant)		
		SecorRemnant = num;
	
	while(1)
	{
		//������������������
		Flash_Read(SecorAdd*FLASH_SECTOR_SIZE+STM32_FLASH_BASE,FLASH_BUF,FLASH_SECTOR_SIZE/2);
		//У�������Ƿ񱻲���
		for(i=0; i<SecorRemnant; i++)
		{
			if(FLASH_BUF[SecorOffset+i] != 0xFFFF)
				break;						//δ������ ֱ������
		}
		if(i<SecorRemnant)	//��Ҫ����
		{
			//������������
			FLASH_ErasePage(SecorAdd*FLASH_SECTOR_SIZE+STM32_FLASH_BASE);
			for(i=0; i<SecorRemnant; i++)
			{
				FLASH_BUF[i] = p[i];
			}
			//д����������
			Flash_WriteLenByte(SecorAdd*FLASH_SECTOR_SIZE+STM32_FLASH_BASE,FLASH_BUF,FLASH_SECTOR_SIZE/2);
		}
		else		//�Ѿ������˵� ֱ��д��
			Flash_WriteLenByte(address,p,SecorRemnant);
		
		if(num == SecorRemnant)  //д�����
			break;
		else					//д��δ���� 
		{
			SecorAdd++;					//������ַ��1
			SecorOffset = 0;			//������ƫ��Ϊ0
			p += SecorRemnant;			//ָ��ƫ��
			address +=SecorRemnant;		//д��ַƫ��
			num -= SecorRemnant;		//ʣ��Ҫд�����ݵĳ���
			//�����������д����
			if(num>(FLASH_SECTOR_SIZE/2))
				SecorRemnant = FLASH_SECTOR_SIZE/2;
			else 
				SecorRemnant = num;	
		}
	}
	FLASH_Lock();	//FLASK ����
}

