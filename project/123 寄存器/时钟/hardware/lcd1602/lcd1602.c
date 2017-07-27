
    #include"lcd1602.h"

/*******************************************************************************
* �� �� ��         : LcdWriteCom
* ��������		   : ��LCDд��һ���ֽڵ�����
* ��    ��         : cmd
* ��    ��         : ��
*******************************************************************************/
void LcdWriteCmd(u8 cmd)
{
	lcdrs0;						 
	delayms(1);					//��ͬһ���Ĵ���д������ Ҫ��ʱһ��
	lcdrw0;
	delayms(1);

	Data = cmd;
	delayms(1);
	lcden1;
	delayms(1);
	lcden0;
}
/*******************************************************************************
* �� �� ��         : LcdWriteData
* ��������		   : ��LCDд��һ���ֽڵ�����
* ��    ��         : dat
* ��    ��         : ��
*******************************************************************************/
void LcdWriteData(u8 dat)
{
	lcdrs1;
	delayms(1);
	lcdrw0;
	delayms(1);

	Data = dat;
	delayms(1);
	lcden1;
	delayms(1);
	lcden0;
}

/*******************************************************************************
* �� �� ��         : LcdWriteSfm
* ��������		   : ��LCDд��ʱ����
* ��    ��         : dat
* ��    ��         : ��
*******************************************************************************/
void LcdWriteSfm(u8 add,char dat)
{
	u8 ge,shi;

	ge = dat%10;			//�ֽ�����  ��λ �� ʮλ
	shi = dat/10;

	LcdWriteCmd(0xc0+add);

	LcdWriteData(0x30+shi);		  //0x30 ��ʾ������ת��ʮ����
	LcdWriteData(0x30+ge);
}
/*******************************************************************************
* �� �� ��         : LcdWriteNyr
* ��������		   : ��LCDд������
* ��    ��         : dat
* ��    ��         : ��
*******************************************************************************/
void LcdWriteNyr(u8 add,char dat)
{
	u8 ge,shi;

	ge = dat%10;
	shi = dat/10;

	LcdWriteCmd(0x80+add);

	LcdWriteData(0x30+shi);
	LcdWriteData(0x30+ge);
}
/*******************************************************************************
* �� �� ��         : LcdInit
* ��������		   : lcd��ʼ��
* ��    ��         : ��
* ��    ��         : ��
*******************************************************************************/
void LcdInit(void)
{
	RCC -> APB2ENR |= 1<<5;
	RCC -> APB2ENR |= 1<<4;

	GPIOD -> CRL &= 0X00000000;
	GPIOD -> CRL |= 0X77777777;
	GPIOD -> ODR |= 0XFFFF;

	GPIOC -> CRL &= 0XFFFFF000;
	GPIOC -> CRL |= 0X00000777;
	GPIOC -> ODR |= 0X0007;

	delayms(5);

	 lcden0;
	 delayms(1);
	 lcdrs0;
	 delayms(1);
	 lcdrw0;
	 delayms(1);

	LcdWriteCmd(0x38);
	LcdWriteCmd(0x0c);
	LcdWriteCmd(0x06);
	LcdWriteCmd(0x01);
}
/*******************************************************************************
* �� �� ��         : DisInit();
* ��������		   : Һ������ʾ��ʼ��
* ��    ��         : ��
* ��    ��         : ��
*******************************************************************************/
void DisInit(void)
{
/****************************/
	LcdWriteSfm(10,0);
	LcdWriteSfm(7,0);
	LcdWriteSfm(4,0);		 //������ʾ��ʼ�������
	LcdWriteNyr(11,0);		 //0000-00-00
	LcdWriteNyr(8,0);		 //00-00-00
	LcdWriteNyr(5,0);
	LcdWriteNyr(3,0);
/***************************/

	LcdWriteCmd(0x80+7);
	LcdWriteData('-');
	LcdWriteCmd(0x80+10);
	LcdWriteData('-');

	LcdWriteCmd(0xc0+6);
	LcdWriteData(':');
	LcdWriteCmd(0xc0+9);
	LcdWriteData(':');
}
//////////////////////////////////////////////////////////////////////////////////

