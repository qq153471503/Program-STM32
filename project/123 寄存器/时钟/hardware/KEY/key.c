#include"key.h"
#include"delay.h"
#include"lcd1602.h"
#include"timer.h"	

char count;
char miao,fen,shi,ri,yue,nian1,nian2;

/*********************************************************************
* �� �� ��         : KeyScan()
* ��������		   : �����ӿڳ�ʼ��
* ��    ��         : ��
* ��    ��         : ��
*********************************************************************/
void KeyInit(void)
{
	RCC -> APB2ENR |= 1<<6;			  //ʹ��PD��ʱ��
	RCC -> APB2ENR |= 1<<2;			  //ʹ��PA��ʱ��

	GPIOA -> CRL &= 0XFFFFFFF0;		 
	GPIOA -> CRL |= 0x00000008;		 //����Ϊ����ģʽ ����


	GPIOE -> CRL &= 0XFFF000FF;
	GPIOE -> CRL |= 0X00088800;		//����Ϊ����ģʽ ����
	GPIOE -> ODR |= 7<<2;
}
void KeyScan()
{
/******************************���λ�����ӳ����**************************************/
	if(wk_up == 1)			//���ܼ�������
	{
		delayms(2);			 //��ʱ��������
		if(wk_up == 1)
		{
			count++;		 //�������´��� ����
			if(count >= 8)	 //���´������ڵ���8��  ���ص���ʼ״̬
			{
				count = 0;
			}while(wk_up);		//�ȴ������ͷ�
		}

		if(count != 0)		  //���count��������0  ˵��������������ʱ��ʱ��ֹͣ
			disable
		else 	
		enable
	}
/****************************************************************/
	if(count != 0)
	{
/***************************���ܼ�����һ�� д �� �����*************************************/
		if(count == 1)
		{
			LcdWriteCmd(0x0f);			   //�򿪹����˸
			LcdWriteCmd(0xc0+11);
			delayms(500);				   //��ʱ500����
			LcdWriteCmd(0x0c);			   //�رչ����˸

			if(key2 == 0)
			{
				delayms(2);
				if(key2 == 0)
				{
					miao++;
					if(miao >=60)	miao = 0;
					LcdWriteSfm(10,miao);
	
				}while(!key2);
			}
			if(key1 == 0)
			{
				delayms(2);
				if(key1 == 0)
				{
					miao--;
					if(miao <= 0)	miao = 59;
					LcdWriteSfm(10,miao);					
				}while(!key1);
			}
		}
/******************************���ܼ����¶��� д �� �����****************************************/
		if(count == 2)
		{
			LcdWriteCmd(0x0f);
			LcdWriteCmd(0xc0+8);
			delayms(500);
			LcdWriteCmd(0x0c);

			if(key2 == 0)
			{
				delayms(2);
				if(key2 == 0)
				{
					fen++;
					if(fen >=60)	fen = 0;
					LcdWriteSfm(7,fen);
				}while(!key2);
			}
			if(key1 == 0)
			{
				delayms(2);
				if(key1 == 0)
				{
					fen--;
					if(fen <= 0)	fen = 59;
					LcdWriteSfm(7,fen);					
				}while(!key1);
			}
		}
/********************************���ܼ��������� д Сʱ �����*****************************************/
		if(count == 3)
		{
			LcdWriteCmd(0x0f);
			LcdWriteCmd(0xc0+5);
			delayms(500);
			LcdWriteCmd(0x0c);

			if(key2 == 0)
			{
				delayms(2);
				if(key2 == 0)
				{
					shi++;
					if(shi >= 24)	shi = 0;
					LcdWriteSfm(4,shi);
				}while(!key2);
			}
			if(key1 == 0)
			{
				delayms(2);
				if(key1 == 0)
				{
					shi--;
					if(shi <= 0)	shi = 23;
					LcdWriteSfm(4,shi);					
				}while(!key1);
			}
		}
/*******************************���ܼ������Ĵ� д �� �����********************************************/
		if(count == 4)
		{
			LcdWriteCmd(0x0f);
			LcdWriteCmd(0x80+12);
			delayms(500);
			LcdWriteCmd(0x0c);

			if(key2 == 0)
			{
				delayms(2);
				if(key2 == 0)
				{
					ri++;
					if(ri >=32)	ri = 0;
					LcdWriteNyr(11,ri);
	
				}while(!key2);
			}
			if(key1 == 0)
			{
				delayms(2);
				if(key1 == 0)
				{
					ri--;
					if(ri <= 0)	ri = 31;
					LcdWriteNyr(11,ri);					
				}while(!key1);
			}
		}
/************************************���ܼ�������� д �� �����****************************************************/

		if(count == 5)
		{
			LcdWriteCmd(0x0f);
			LcdWriteCmd(0x80+9);
			delayms(500);
			LcdWriteCmd(0x0c);

			if(key2 == 0)
			{
				delayms(2);
				if(key2 == 0)
				{
					yue++;
					if(yue >=13)	yue = 0;
					LcdWriteNyr(8,yue);
	
				}while(!key2);
			}
			if(key1 == 0)
			{
				delayms(2);
				if(key1 == 0)
				{
					yue--;
					if(yue <= 0)	yue = 12;
					LcdWriteNyr(8,yue);					
				}while(!key1);
			}
		}
/****************************************���ܼ��������� д �� ����λ �����***********************************************/
		if(count == 6)
		{
			LcdWriteCmd(0x0f);
			LcdWriteCmd(0x80+6);
			delayms(500);
			LcdWriteCmd(0x0c);


			if(key2 == 0)
			{
				delayms(2);
				if(key2 == 0)
				{
					nian1++;
					if(nian1 >=100)	nian1 = 0;
					LcdWriteNyr(5,nian1);
	
				}while(!key2);
			}
			if(key1 == 0)
			{
				delayms(2);
				if(key1 == 0)
				{
					nian1--;
					if(nian1 <= 0)	nian1 = 99;
					LcdWriteNyr(5,nian1);					
				}while(!key1);
			}
		}
/*****************************************���ܼ������ߴ� д ��ǰ��λ �����*****************************************************/
		if(count == 7)
		{
			LcdWriteCmd(0x0f);
			LcdWriteCmd(0x80+4);
			delayms(500);
			LcdWriteCmd(0x0c);


			if(key2 == 0)
			{
				delayms(2);
				if(key2 == 0)
				{
					nian2++;
					if(nian2 >=100)	nian2 = 0;
					LcdWriteNyr(3,nian2);
	
				}while(!key2);
			}
			if(key1 == 0)
			{
				delayms(2);
				if(key1 == 0)
				{
					nian2--;
					if(nian2 <= 0)	nian2 = 99;
					LcdWriteNyr(3,nian2);					
				}while(!key1);
			}
		}
/*********************************ȷ�ϰ��������*************************************************/
		if(key0 == 0)				  //���key0������
		{
			enable					  //�򿪶�ʱ��
			LcdWriteCmd(0x0c);		  //ȡ�������˸
			count = 0;				  //������������
		}
/***********************************************************************************/
	}	
}



