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
	LcdWriteCmd(0x34);      //开启扩展指令集，并关闭画图显示。
	for(i=0;i<32;i++)			  //因为LCD有纵坐标32格所以写三十二次
	{
		LcdWriteCmd(0x80+i);		  //先写入纵坐标Y的值
		LcdWriteCmd(0x80);		  //再写入横坐标X的值
		for(j=0; j<16; j++)		  //横坐标有16位，每位写入两个字节的的数据，也
		{						  //就写入32次以为当写入两个字节之后横坐标会自
			LcdWriteData(*a); 	      //动加1，所以就不用再次写入地址了。
			a++;
		}
	}  
	for(i=0; i<32; i++)			  //因为LCD有纵坐标32格所以写三十二次
	{
		
		LcdWriteCmd(0x80+i);		  //先写入纵坐标Y的值
		LcdWriteCmd(0x88);		  //再写入横坐标X的值
		for(j=0; j<16; j++)		  //横坐标有16位，每位写入两个字节的的数据，也
		{						  //就写入32次以为当写入两个字节之后横坐标会自
			LcdWriteData(*a); 	      //动加1，所以就不用再次写入地址了。
			a++;		
		}
	}	
	LcdWriteCmd(0x36);	   //开显示
	LcdWriteCmd(0x30);  	   //转回基本指令集
}
void LcdQingPing(void)
{
	u8 i,j;
	LcdWriteCmd(0x34);	  //开启拓展指令集

	for(i=0;i<32;i++)			  //因为LCD有纵坐标32格所以写三十二次
	{
		LcdWriteCmd(0x80+i);		  //先写入纵坐标Y的值
		LcdWriteCmd(0x80);		  //再写入横坐标X的值
		for(j=0; j<32; j++)		  //横坐标有16位，每位写入两个字节的的数据，也
		{						  //就写入32次以为当写入两个字节之后横坐标会自
			LcdWriteData(0x00);	  //动加1，所以就不用再次写入地址了。	   0xff 是屏幕反白
		}
	}

	LcdWriteCmd(0x36);	 //0x36扩展指令里面打开绘图显示
	delay_ms(10);
	LcdWriteCmd(0x30);	 //恢复基本指令集
}

