#include"lcd12864.h"

void LcdWriteCmd(u8 cmd)
{
	lcdrs0;
	lcdrw0;
	lcden0;
	LcdData = cmd;
	delay_ms(1);
	lcden1;
	delay_ms(1);
	lcden0;
}
void LcdWriteData(u8 dat)
{
	lcdrs1;
	lcdrw0;
	lcden0;
	LcdData = dat;
	delay_ms(1);
	lcden1;
	delay_ms(1);
	lcden0;
}
void LcdDisplay(u8 cmd,u8 dat)
{
	LcdWriteCmd(cmd);
	LcdWriteData(dat);
}

void LcdInit()
{
	RCC -> APB2ENR |= 1<<5;
	RCC -> APB2ENR |= 1<<6;

	GPIOD -> CRL &= 0X00000000;
	GPIOD -> CRL |= 0X77777777;
	GPIOD -> ODR |= 0XFFFF;

	GPIOE -> CRL &= 0XFFFFF000;
	GPIOE -> CRL |= 0X00000777;
	GPIOE -> ODR |= 0X0007;

	delay_ms(100);
	LcdWriteCmd(0x30);
	LcdWriteCmd(0x0c);	
	LcdWriteCmd(0x01);	
}
void LcdDisPic(u8 *a)	   
{
	u8 i,j;
	LcdWriteCmd(0x34);      //������չָ������رջ�ͼ��ʾ��
	for(i=0;i<32;i++)			  //��ΪLCD��������32������д��ʮ����
	{
		LcdWriteCmd(0x80+i);		  //��д��������Y��ֵ
		LcdWriteCmd(0x80);		  //��д�������X��ֵ
		for(j=0; j<16; j++)		  //��������16λ��ÿλд�������ֽڵĵ����ݣ�Ҳ
		{						  //��д��32����Ϊ��д�������ֽ�֮����������
			LcdWriteData(*a); 	      //����1�����ԾͲ����ٴ�д���ַ�ˡ�
			a++;
		}
	}  
	for(i=0; i<32; i++)			  //��ΪLCD��������32������д��ʮ����
	{
		
		LcdWriteCmd(0x80+i);		  //��д��������Y��ֵ
		LcdWriteCmd(0x88);		  //��д�������X��ֵ
		for(j=0; j<16; j++)		  //��������16λ��ÿλд�������ֽڵĵ����ݣ�Ҳ
		{						  //��д��32����Ϊ��д�������ֽ�֮����������
			LcdWriteData(*a); 	      //����1�����ԾͲ����ٴ�д���ַ�ˡ�
			a++;		
		}
	}	
	LcdWriteCmd(0x36);	   //����ʾ
	LcdWriteCmd(0x30);  	   //ת�ػ���ָ�
}
void LcdQingPing(void)
{
	u8 i,j;
	LcdWriteCmd(0x34);	  //������չָ�

	for(i=0;i<32;i++)			  //��ΪLCD��������32������д��ʮ����
	{
		LcdWriteCmd(0x80+i);		  //��д��������Y��ֵ
		LcdWriteCmd(0x80);		  //��д�������X��ֵ
		for(j=0; j<32; j++)		  //��������16λ��ÿλд�������ֽڵĵ����ݣ�Ҳ
		{						  //��д��32����Ϊ��д�������ֽ�֮����������
			LcdWriteData(0x00);	  //����1�����ԾͲ����ٴ�д���ַ�ˡ�	   0xff ����Ļ����
		}
	}

	LcdWriteCmd(0x36);	 //0x36��չָ������򿪻�ͼ��ʾ
	delay_ms(10);
	LcdWriteCmd(0x30);	 //�ָ�����ָ�
}

