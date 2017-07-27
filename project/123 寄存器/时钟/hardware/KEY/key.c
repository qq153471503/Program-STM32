#include"key.h"
#include"delay.h"
#include"lcd1602.h"
#include"timer.h"	

char count;
char miao,fen,shi,ri,yue,nian1,nian2;

/*********************************************************************
* 函 数 名         : KeyScan()
* 函数功能		   : 按键接口初始化
* 输    入         : 无
* 输    出         : 无
*********************************************************************/
void KeyInit(void)
{
	RCC -> APB2ENR |= 1<<6;			  //使能PD口时钟
	RCC -> APB2ENR |= 1<<2;			  //使能PA口时钟

	GPIOA -> CRL &= 0XFFFFFFF0;		 
	GPIOA -> CRL |= 0x00000008;		 //设置为输入模式 下拉


	GPIOE -> CRL &= 0XFFF000FF;
	GPIOE -> CRL |= 0X00088800;		//设置为输入模式 上拉
	GPIOE -> ODR |= 7<<2;
}
void KeyScan()
{
/******************************光标位置增加程序段**************************************/
	if(wk_up == 1)			//功能键被按下
	{
		delayms(2);			 //延时消除抖动
		if(wk_up == 1)
		{
			count++;		 //按键按下次数 计数
			if(count >= 8)	 //按下次数大于等于8次  返回到开始状态
			{
				count = 0;
			}while(wk_up);		//等待按键释放
		}

		if(count != 0)		  //如果count计数不是0  说明按键被按，此时定时器停止
			disable
		else 	
		enable
	}
/****************************************************************/
	if(count != 0)
	{
/***************************功能键按下一次 写 秒 程序段*************************************/
		if(count == 1)
		{
			LcdWriteCmd(0x0f);			   //打开光标闪烁
			LcdWriteCmd(0xc0+11);
			delayms(500);				   //延时500毫秒
			LcdWriteCmd(0x0c);			   //关闭光标闪烁

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
/******************************功能键按下二次 写 分 程序段****************************************/
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
/********************************功能键按下三次 写 小时 程序段*****************************************/
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
/*******************************功能键按下四次 写 日 程序段********************************************/
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
/************************************功能键按下五次 写 月 程序段****************************************************/

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
/****************************************功能键按下六次 写 年 后两位 程序段***********************************************/
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
/*****************************************功能键按下七次 写 年前两位 程序段*****************************************************/
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
/*********************************确认按键程序段*************************************************/
		if(key0 == 0)				  //如果key0被按下
		{
			enable					  //打开定时器
			LcdWriteCmd(0x0c);		  //取消光标闪烁
			count = 0;				  //按键计数清零
		}
/***********************************************************************************/
	}	
}



