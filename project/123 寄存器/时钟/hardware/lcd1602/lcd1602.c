
    #include"lcd1602.h"

/*******************************************************************************
* 函 数 名         : LcdWriteCom
* 函数功能		   : 向LCD写入一个字节的命令
* 输    入         : cmd
* 输    出         : 无
*******************************************************************************/
void LcdWriteCmd(u8 cmd)
{
	lcdrs0;						 
	delayms(1);					//对同一个寄存器写入数据 要延时一会
	lcdrw0;
	delayms(1);

	Data = cmd;
	delayms(1);
	lcden1;
	delayms(1);
	lcden0;
}
/*******************************************************************************
* 函 数 名         : LcdWriteData
* 函数功能		   : 向LCD写入一个字节的数据
* 输    入         : dat
* 输    出         : 无
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
* 函 数 名         : LcdWriteSfm
* 函数功能		   : 向LCD写入时分秒
* 输    入         : dat
* 输    出         : 无
*******************************************************************************/
void LcdWriteSfm(u8 add,char dat)
{
	u8 ge,shi;

	ge = dat%10;			//分解数据  个位 和 十位
	shi = dat/10;

	LcdWriteCmd(0xc0+add);

	LcdWriteData(0x30+shi);		  //0x30 表示将数据转成十进制
	LcdWriteData(0x30+ge);
}
/*******************************************************************************
* 函 数 名         : LcdWriteNyr
* 函数功能		   : 向LCD写年月日
* 输    入         : dat
* 输    出         : 无
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
* 函 数 名         : LcdInit
* 函数功能		   : lcd初始化
* 输    入         : 无
* 输    出         : 无
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
* 函 数 名         : DisInit();
* 函数功能		   : 液晶屏显示初始化
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/
void DisInit(void)
{
/****************************/
	LcdWriteSfm(10,0);
	LcdWriteSfm(7,0);
	LcdWriteSfm(4,0);		 //开机显示初始化程序段
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

