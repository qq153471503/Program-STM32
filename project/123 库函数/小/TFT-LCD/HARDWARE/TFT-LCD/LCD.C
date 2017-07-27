#include "lcd.h"
#include "delay.h"
#include "usart.h"
#include "font.h"
#include "pic.h"

//画笔颜色,背景颜色
u16 PointColor = 0x0000,BackColor = 0xFFFF;  
u16 DeviceCode;
/**********************************************************************/
/*写寄存器
LCD_Reg:寄存器编号
LCD_RegValue:要写入的值*/
void LcdWriteReg(u8 LCD_Reg, u16 LCD_RegValue)
{	
	LcdWriteCmd(LCD_Reg);  
	LcdWriteData(LCD_RegValue);	    		 
}
/*写寄存器函数
类似于1602/12864 写命令*/
void LcdWriteCmd(u8 dat)
{ 
	lcdrs0;//写地址  
 	lcdcs0; 
	DataOut(dat); 
	lcdwr0; 
	lcdwr1; 
 	lcdcs1;   
}
/*开始写GRAM*/
 void LcdWriteRAMPrepare(void)
{
	if(DeviceCode == 0X9341)
		LcdWriteCmd(0x2C);//ILI9341是以0X2C开始的
	else
		LcdWriteCmd(R34);
}	 
/*LCD写GRAM
RGB_Code:颜色值*/
void LcdWriteRAM(u16 RGB_Code)
{							    
	LcdWriteData(RGB_Code);//写十六位GRAM
}
/****************************************************************/
/*//读LCD数据
//返回值:读到的值*/
u16 LcdReadData(void)
{										   
	u16 t;
 	GPIOB->CRL=0X88888888; //PB0-7  上拉输入
	GPIOB->CRH=0X88888888; //PB8-15 上拉输入
	GPIOB->ODR=0X0000;     //全部输出0

	lcdrs1;
	lcdcs0;
	//读取数据(读寄存器时,并不需要读2次)
	lcdrd0;					   
	lcdrd1;
	t = DataIn;  
	lcdcs1; 

	GPIOB->CRL=0X33333333; //PB0-7  上拉输出
	GPIOB->CRH=0X33333333; //PB8-15 上拉输出
	GPIOB->ODR=0XFFFF;    //全部输出高
	return t;  
}	
/**********************************************************************/
/*读寄存器
//LCD_Reg:寄存器编号
//返回值:读到的值*/
u16 LcdReadReg(u8 Reg)
{
	LcdWriteCmd(Reg);
	
	return LcdReadData();
}
/***************************************************************************/
//从ILI93xx读出的数据为GBR格式，而我们写入的时候为RGB格式。
//通过该函数转换
//c:GBR格式的颜色值
//返回值：RGB格式的颜色值
u16 Lcd_BGR2RGB(u16 color)
{
	u16  r,g,b,rgb;   
	b=(color>>0)&0x1f;
	g=(color>>5)&0x3f;
	r=(color>>11)&0x1f;	 
	rgb=(b<<11)+(g<<5)+(r<<0);
	
	return(rgb);
}
/**************************************************************************/
//读取个某点的颜色值	 
//x:0~239
//y:0~319
//返回值:此点的颜色
u16 LcdReadPoint(u16 x,u16 y)
{
 	u16 r,g,b;
	if(x>=LCD_W||y>=LCD_H)return 0;			//超过了范围,直接返回		   
	LcdSetCursor(x,y);
	
	if(DeviceCode==0X9341)
		LcdWriteCmd(0X2E);	//ILI9341发送读GRAM指令
	else 
		LcdWriteCmd(R34);      		 		//其他IC发送读GRAM指令
	GPIOB->CRL=0X88888888; //PB0-7  上拉输入
	GPIOB->CRH=0X88888888; //PB8-15 上拉输入
	GPIOB->ODR=0XFFFF;     //全部输出高

	lcdrs1;
	lcdcs0;
	//读取数据(读GRAM时,第一次为假读)	
	lcdrd0;	
  	lcdrd1;
 	//dummy READ
	lcdrd0;					   
	if(DeviceCode==0X8989)delay_us(2);//FOR 8989,延时2us					   
	lcdrd1;
 	r = DataIn;  
 	if(DeviceCode==0X9341)
	{	 
		lcdrd0;					   
	 	lcdrd1;
		b= DataIn;//读取蓝色值  	  
		g=r&0XFF;//对于9341,第一次读取的是RG的值,R在前,G在后,各占8位
		g<<=8;
	}	 
	lcdcs1;	

	GPIOB->CRL=0X33333333; //PB0-7  上拉输出
	GPIOB->CRH=0X33333333; //PB8-15 上拉输出
	GPIOB->ODR=0XFFFF;    //全部输出高  
	if(DeviceCode==0X4535||DeviceCode==0X4531||DeviceCode==0X8989||DeviceCode==0XB505)return r;	//这几种IC直接返回颜色值
	else if(DeviceCode==0X9341)
		return (((r>>11)<<11)|((g>>10)<<5)|(b>>11));						//ILI9341需要公式转换一下
	else return Lcd_BGR2RGB(r);	
}
/***************************************************************************/
/*清屏函数
Color:要清屏的填充色*/
void LcdClearScreen(u16 Color)
{
	u32 index=0;      
	LcdSetCursor(0,0);//设置光标位置 
	LcdWriteRAMPrepare();     //开始写入GRAM	 	  
	for(index=0; index<76800; index++)
	{
		LcdWriteData(Color);    
	}
}
/************************************************************************/
/* 设置游标位置函数*/
__inline void LcdSetCursor(u16 Xpos, u16 Ypos)
{
#if USE_HORIZONTAL==1 
	if(DeviceCode==0X8989)
	{
		Xpos=319-Xpos;
		LcdWriteReg(0X4E, Ypos);
		LcdWriteReg(0X4F, Xpos);
    }else if(DeviceCode==0X9341)//9341,设置坐标
	{			 
		LcdWriteCmd(0x2B); 
		LcdWriteData(Ypos>>8); 
		LcdWriteData(Ypos&0XFF);			 
		LcdWriteCmd(0x2A); 
		LcdWriteData(Xpos>>8); 
		LcdWriteData(Xpos&0XFF);
	}else
	{
		Xpos=319-Xpos;
		LcdWriteReg(R32,Ypos);
		LcdWriteReg(R33,Xpos);
	}							   
#else
	if(DeviceCode==0X8989)
	{
		LcdWriteReg(0X4E, Xpos);
		LcdWriteReg(0X4F, Ypos);
    }else if(DeviceCode==0X9341)
	{		    
		LcdWriteCmd(0x2A); 
		LcdWriteData(Xpos>>8); 
		LcdWriteData(Xpos&0XFF);	 
		LcdWriteCmd(0x2B); 
		LcdWriteData(Ypos>>8); 
		LcdWriteData(Ypos&0XFF);
	}else
	{
		LcdWriteReg(R32, Xpos);
		LcdWriteReg(R33, Ypos);
	}						    
#endif
}
/***********************************************************************/
//开窗函数

void LcdSetWindows(u16 StartX,u16 StartY,u16 EndX,u16 EndY)		/*开窗函数*/
{
	LcdWriteReg(R80,StartX);
	LcdWriteReg(R82,StartY);
	
	LcdWriteReg(R81,EndX);
	LcdWriteReg(R83,EndY);
	
//	LcdWriteRAMPrepare();     //开始写入GRAM	
}
/*********************************画点******************************************/
//画点
//x:0~239
//y:0~319
//POINT_COLOR:此点的颜色
void LcdDrawPoint(u16 x,u16 y)
{
	LcdSetCursor(x,y);//设置光标位置 
	LcdWriteRAMPrepare();	//开始写入GRAM
	LcdWriteData(PointColor); 
} 
/******************************画线*********************************************/
//画线
//x1:初始点
//y1:初始点
//x1:结束点
//y1:结束点
//使用了布兰森汉姆算法 画直线
void LcdDrawLine(u16 x1,u16 y1,u16 x2,u16 y2)
{
	u16 count;				
	int i,k;
	int x,y,xadd,yadd;
	int value;
	int heng = 0,shu = 0;
	
	
	x = x1;		
	y = y1;
	
	xadd = x2-x1;		//计算坐标增量
	yadd = y2-y1;
	
	if(xadd>0)	//如果x增量大于0  设坐标为正 则每次加一
		i = 1;
	else if(xadd == 0)  //如果增量是0  说明x坐标没有变 是在画垂直线
		i = 0;
	else 				//如果增量为负数  
	{
		i = -1;			//x坐标反向增加
		xadd = -xadd;	//x坐标方向取反
	}
	
	if(yadd > 0)		//y坐标  同理x坐标
		k = 1;
	else if(yadd == 0)
		k = 0;
	else 
	{
		k = -1;
		yadd = -yadd;
	}
	
	if(xadd > yadd)		//计算需要循环的次数  x y 两个都需要循环显示出来 所以谁数值大 取谁
		value = xadd;
	else
		value = yadd;
	
	for(count=0; count<=value+1; count++)//循环要算上本来的初始点 所以要加1
	{
		LcdDrawPoint(x,y);   //画点函数 已经写好初始点 所以下面if语句小于count即可
		
		heng = heng+xadd;  //横     这两句的作用是：
		shu = shu+yadd;		//竖				计算x每增加一  y的增量
		
		if(heng > count)	//横坐标限制条件
		{
			heng = heng-value;
			x = x+i;
		}
		if(shu > count)		///纵坐标限制条件
		{
			shu = shu-value;
			y = y+k;
		}
	}
}
/*************************** 画矩形 ************************************************/
void LcdDrawRectangle(u16 x1,u16 y1,u16 x2,u16 y2)
{
	LcdDrawLine(x1,y1,x2,y1);
	LcdDrawLine(x1,y1,x1,y2);
	LcdDrawLine(x1,y2,x2,y2);
	LcdDrawLine(x2,y1,x2,y2);
}
/*****************************  画圆  *************************************************/
void LcdDrawCircle(u16 x0,u16 y0,u8 r)
{
	int a,b;
	int di;
	a = 0;
	b = r;	  
	di = 3-(r<<1);             //判断下个点位置的标志
	while(a<=b)
	{
		LcdDrawPoint(x0-b,y0-a);             //3           
		LcdDrawPoint(x0+b,y0-a);             //0 	
		LcdDrawPoint(x0-a,y0+b);             //1       
		LcdDrawPoint(x0-b,y0-a);             //7           
		LcdDrawPoint(x0-a,y0-b);             //2             
		LcdDrawPoint(x0+b,y0+a);             //4               
		LcdDrawPoint(x0+a,y0-b);             //5
		LcdDrawPoint(x0+a,y0+b);             //6 
		LcdDrawPoint(x0-b,y0+a);  
		
		a++;
		
		//使用Bresenham算法画圆     
		if(di<0)
			
			di +=4*a+6;	  
		else
			
		{
			di+=10+4*(a-b);   
			b--;
		} 
		LcdDrawPoint(x0+a,y0+b);
	}	
}
/*********************************显示图片********************************************/
//显示图片
//StartX:	起始X坐标
//StartY:	起始Y坐标
//wide:		图片宽度
//high:		图片高度
void LcdDrawPicture(u16 StartX,u16 StartY,u8 wide,u16 high)
{
	u32 i,k;
	u32 j = 0;
	
	for(i=StartY; i<high+StartY; i++)
	{
		for(k=StartX; k<StartX+wide; k++)
		{
			LcdSetCursor(k,i);
			LcdWriteRAMPrepare();     //开始写入GRAM
			LcdWriteRAM((meheng[2*j]<<8)+(meheng[2*j+1]));	
			j++;
		}
	}
	
//	for(i=StartY; i<high+StartY; i++)
//	{
//		for(k=StartX; k<StartX+wide; k++)
//		{
//			LcdSetCursor(k,i);
//			LcdWriteRAMPrepare();     //开始写入GRAM
//			LcdWriteRAM((*p)+(*(p+1)));	
//			p++;
//		}
//	}
}
/*******************************************************************************/
//任意位置写一个字符
void LcdShowChar(u16 x,u16 y,u8 num,u8 size,u8 mode)
{
	#if USE_HORIZONTAL==1
	#define MaxXpos 312
	#define MaxYpos 232 
	#else     
	#define MaxXpos 232
	#define MaxYpos 312
	#endif

	u8 temp ;
	u8 pos,t;
	u16 x0 = x;
	u16 ColorTemp = PointColor;
	
	if(x>MaxXpos||y>MaxYpos)
		return ;
	
	num = num-' ';
	
	if(!mode)/*非叠加模式*/
	{
		for(pos=0; pos<size; pos++)   //一级循环  控制y 一共有几行
		{
			if(size == 12)
				temp = asc2_1206[num][pos];
			else
				temp = asc2_1608[num][pos];
			
				for(t=0; t<size/2; t++)
				{
					if(temp&0x01)
						PointColor = ColorTemp;
					else 
						PointColor = GREEN;//BackColor;  改变此处的颜色 是改变要显示字符的背景色
					LcdDrawPoint(x,y);					
					temp >>= 1; 					
					x++;
				}
				x = x0;
				y++;
		}
	}
	else//叠加方式
	{
		for(pos=0;pos<size;pos++)
		{
			if(size==12)
				temp=asc2_1206[num][pos];//调用1206字体
			else 
				temp=asc2_1608[num][pos];		 //调用1608字体
			
			for(t=0;t<size/2;t++)
		    {                 
		        if(temp&0x01)
					LcdDrawPoint(x+t,y+pos);//画一个点     
		        temp>>=1; 
		    }
		}
	}
	PointColor = ColorTemp;	 	
}
/***************************************************************************/
//任意位置写一个字符串
void LcdShowString(u16 x,u16 y,const u8 *p)
{	
	while(*p != '\0')
	{
		if(x>MaxXpos)
		{
			x = 0;
			y += 16;
		}
		
		if(y>MaxYpos)
		{
			y = x = 0;
			LcdClearScreen(WHITE);
		}
		
		LcdShowChar(x,y,*p,16,0);	
		x += 8;
		p++;
	}
}
/**********************************************************************/
void LcdInit(void)
{  
     										 
	GPIO_InitTypeDef GPIO_InitStructure;

 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO, ENABLE); 
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable , ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10|GPIO_Pin_9|GPIO_Pin_8|GPIO_Pin_7|GPIO_Pin_6;	   //GPIO_Pin_10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure); //GPIOC	
    //GPIO_WriteBit(GPIOC,GPIO_Pin_10|GPIO_Pin_9|GPIO_Pin_8|GPIO_Pin_7|GPIO_Pin_6,Bit_SET);
	GPIO_SetBits(GPIOC,GPIO_Pin_10|GPIO_Pin_9|GPIO_Pin_8|GPIO_Pin_7|GPIO_Pin_6);


	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;	//  
	GPIO_Init(GPIOB, &GPIO_InitStructure); //GPIOB
	//GPIO_WriteBit(GPIOC,GPIO_Pin_All,Bit_SET); 
	GPIO_SetBits(GPIOB,GPIO_Pin_All);
 
	delay_ms(50); // delay 50 ms 
	LcdWriteReg(0x0000,0x0001);
	delay_ms(50); // delay 50 ms 
	DeviceCode = LcdReadReg(0x0000);   
 	if(DeviceCode==0||DeviceCode==0XFFFF)//读到ID不正确
	{	
		//可能是9341,尝试9341的ID读取		
		LcdWriteCmd(0XD3);				   
		LcdReadData(); 			//dummy read 	
 		LcdReadData();   	    //读回0X00
  		DeviceCode=LcdReadData()&0XBF;//这里读回0XD3,实际是0X93才对.强制去掉第6位   								   
 		DeviceCode<<=8;
		DeviceCode|=LcdReadData();	     
	}
	printf(" LCD ID:%x\r\n",DeviceCode); //打印LCD ID 
	if(DeviceCode==0X9341)	//9341初始化
	{
		LcdWriteCmd(0xCF);  
		LcdWriteData(0x00); 
		LcdWriteData(0xC1); 
		LcdWriteData(0X30); 
		LcdWriteCmd(0xED);  
		LcdWriteData(0x64); 
		LcdWriteData(0x03); 
		LcdWriteData(0X12); 
		LcdWriteData(0X81); 
		LcdWriteCmd(0xE8);  
		LcdWriteData(0x85); 
		LcdWriteData(0x10); 
		LcdWriteData(0x7A); 
		LcdWriteCmd(0xCB);  
		LcdWriteData(0x39); 
		LcdWriteData(0x2C); 
		LcdWriteData(0x00); 
		LcdWriteData(0x34); 
		LcdWriteData(0x02); 
		LcdWriteCmd(0xF7);  
		LcdWriteData(0x20); 
		LcdWriteCmd(0xEA);  
		LcdWriteData(0x00); 
		LcdWriteData(0x00); 
		LcdWriteCmd(0xC0);    //Power control 
		LcdWriteData(0x1B);   //VRH[5:0] 
		LcdWriteCmd(0xC1);    //Power control 
		LcdWriteData(0x01);   //SAP[2:0];BT[3:0] 
		LcdWriteCmd(0xC5);    //VCM control 
		LcdWriteData(0x30); 	 //3F
		LcdWriteData(0x30); 	 //3C
		LcdWriteCmd(0xC7);    //VCM control2 
		LcdWriteData(0XB7); 
		LcdWriteCmd(0x36);    // Memory Access Control 
		LcdWriteData(0x48); 
		LcdWriteCmd(0x3A);   
		LcdWriteData(0x55); 
		LcdWriteCmd(0xB1);   
		LcdWriteData(0x00);   
		LcdWriteData(0x1A); 
		LcdWriteCmd(0xB6);    // Display Function Control 
		LcdWriteData(0x0A); 
		LcdWriteData(0xA2); 
		LcdWriteCmd(0xF2);    // 3Gamma Function Disable 
		LcdWriteData(0x00); 
		LcdWriteCmd(0x26);    //Gamma curve selected 
		LcdWriteData(0x01); 
		LcdWriteCmd(0xE0);    //Set Gamma 
		LcdWriteData(0x0F); 
		LcdWriteData(0x2A); 
		LcdWriteData(0x28); 
		LcdWriteData(0x08); 
		LcdWriteData(0x0E); 
		LcdWriteData(0x08); 
		LcdWriteData(0x54); 
		LcdWriteData(0XA9); 
		LcdWriteData(0x43); 
		LcdWriteData(0x0A); 
		LcdWriteData(0x0F); 
		LcdWriteData(0x00); 
		LcdWriteData(0x00); 
		LcdWriteData(0x00); 
		LcdWriteData(0x00); 		 
		LcdWriteCmd(0XE1);    //Set Gamma 
		LcdWriteData(0x00); 
		LcdWriteData(0x15); 
		LcdWriteData(0x17); 
		LcdWriteData(0x07); 
		LcdWriteData(0x11); 
		LcdWriteData(0x06); 
		LcdWriteData(0x2B); 
		LcdWriteData(0x56); 
		LcdWriteData(0x3C); 
		LcdWriteData(0x05); 
		LcdWriteData(0x10); 
		LcdWriteData(0x0F); 
		LcdWriteData(0x3F); 
		LcdWriteData(0x3F); 
		LcdWriteData(0x0F); 
		LcdWriteCmd(0x2B); 
		LcdWriteData(0x00);
		LcdWriteData(0x00);
		LcdWriteData(0x01);
		LcdWriteData(0x3f);
		LcdWriteCmd(0x2A); 
		LcdWriteData(0x00);
		LcdWriteData(0x00);
		LcdWriteData(0x00);
		LcdWriteData(0xef);	 
		LcdWriteCmd(0x11); //Exit Sleep
		delay_ms(120);
		LcdWriteCmd(0x29); //display on	
	}else if(DeviceCode==0x9325) //9325初始化
	{
		LcdWriteReg(0x00E5,0x78F0); 
		LcdWriteReg(0x0001,0x0100); 
		LcdWriteReg(0x0002,0x0700); 
		LcdWriteReg(0x0003,0x1030); 
		LcdWriteReg(0x0004,0x0000); 
		LcdWriteReg(0x0008,0x0202);  
		LcdWriteReg(0x0009,0x0000);
		LcdWriteReg(0x000A,0x0000); 
		LcdWriteReg(0x000C,0x0000); 
		LcdWriteReg(0x000D,0x0000);
		LcdWriteReg(0x000F,0x0000);
		//power on sequence VGHVGL
		LcdWriteReg(0x0010,0x0000);   
		LcdWriteReg(0x0011,0x0007);  
		LcdWriteReg(0x0012,0x0000);  
		LcdWriteReg(0x0013,0x0000); 
		LcdWriteReg(0x0007,0x0000); 
		//vgh 
		LcdWriteReg(0x0010,0x1690);   
		LcdWriteReg(0x0011,0x0227);
		//delayms(100);
		//vregiout 
		LcdWriteReg(0x0012,0x009D); //0x001b
		//delayms(100); 
		//vom amplitude
		LcdWriteReg(0x0013,0x1900);
		//delayms(100); 
		//vom H
		LcdWriteReg(0x0029,0x0025); 
		LcdWriteReg(0x002B,0x000D); 
		//gamma
		LcdWriteReg(0x0030,0x0007);
		LcdWriteReg(0x0031,0x0303);
		LcdWriteReg(0x0032,0x0003);// 0006
		LcdWriteReg(0x0035,0x0206);
		LcdWriteReg(0x0036,0x0008);
		LcdWriteReg(0x0037,0x0406); 
		LcdWriteReg(0x0038,0x0304);//0200
		LcdWriteReg(0x0039,0x0007); 
		LcdWriteReg(0x003C,0x0602);// 0504
		LcdWriteReg(0x003D,0x0008); 
		//ram
		LcdWriteReg(0x0050,0x0000); 
		LcdWriteReg(0x0051,0x00EF);
		LcdWriteReg(0x0052,0x0000); 
		LcdWriteReg(0x0053,0x013F);  
		LcdWriteReg(0x0060,0xA700); 
		LcdWriteReg(0x0061,0x0001); 
		LcdWriteReg(0x006A,0x0000); 
		//
		LcdWriteReg(0x0080,0x0000); 
		LcdWriteReg(0x0081,0x0000); 
		LcdWriteReg(0x0082,0x0000); 
		LcdWriteReg(0x0083,0x0000); 
		LcdWriteReg(0x0084,0x0000); 
		LcdWriteReg(0x0085,0x0000); 
		//
		LcdWriteReg(0x0090,0x0010); 
		LcdWriteReg(0x0092,0x0600); 
		
		LcdWriteReg(0x0007,0x0133);
		LcdWriteReg(0x00,0x0022);//	
	}else if(DeviceCode==0x9328)//ILI9328
	{
  		LcdWriteReg(0x00e7,0x0010);      
        LcdWriteReg(0x0000,0x0001);//开启内部时钟
        LcdWriteReg(0x0001,0x0100);     
        LcdWriteReg(0x0002,0x0700);//电源开启                    
		//LcdWriteReg(0x0003,(1<<3)|(1<<4) ); 	//65K  RGB
		//DRIVE TABLE(寄存器 03H)
		//BIT3=AM BIT4:5=ID0:1
		//AM ID0 ID1   FUNCATION
		// 0  0   0	   R->L D->U
		// 1  0   0	   D->U	R->L
		// 0  1   0	   L->R D->U
		// 1  1   0    D->U	L->R
		// 0  0   1	   R->L U->D
		// 1  0   1    U->D	R->L
		// 0  1   1    L->R U->D 正常就用这个.
		// 1  1   1	   U->D	L->R
        LcdWriteReg(0x0003,(1<<12)|(3<<4)|(0<<3) );//65K    
        LcdWriteReg(0x0004,0x0000);                                   
        LcdWriteReg(0x0008,0x0207);	           
        LcdWriteReg(0x0009,0x0000);         
        LcdWriteReg(0x000a,0x0000);//display setting         
        LcdWriteReg(0x000c,0x0001);//display setting          
        LcdWriteReg(0x000d,0x0000);//0f3c          
        LcdWriteReg(0x000f,0x0000);
		//电源配置
        LcdWriteReg(0x0010,0x0000);   
        LcdWriteReg(0x0011,0x0007);
        LcdWriteReg(0x0012,0x0000);                                                                 
        LcdWriteReg(0x0013,0x0000);                 
        delay_ms(50); 
        LcdWriteReg(0x0010,0x1590);   
        LcdWriteReg(0x0011,0x0227);
        delay_ms(50); 
        LcdWriteReg(0x0012,0x009c);                  
        delay_ms(50); 
        LcdWriteReg(0x0013,0x1900);   
        LcdWriteReg(0x0029,0x0023);
        LcdWriteReg(0x002b,0x000e);
        delay_ms(50); 
        LcdWriteReg(0x0020,0x0000);                                                            
        LcdWriteReg(0x0021,0x013f);           
		delay_ms(50); 
		//伽马校正
        LcdWriteReg(0x0030,0x0007); 
        LcdWriteReg(0x0031,0x0707);   
        LcdWriteReg(0x0032,0x0006);
        LcdWriteReg(0x0035,0x0704);
        LcdWriteReg(0x0036,0x1f04); 
        LcdWriteReg(0x0037,0x0004);
        LcdWriteReg(0x0038,0x0000);        
        LcdWriteReg(0x0039,0x0706);     
        LcdWriteReg(0x003c,0x0701);
        LcdWriteReg(0x003d,0x000f);
        delay_ms(50); 
        LcdWriteReg(0x0050,0x0000); //水平GRAM起始位置 
        LcdWriteReg(0x0051,0x00ef); //水平GRAM终止位置                    
        LcdWriteReg(0x0052,0x0000); //垂直GRAM起始位置                    
        LcdWriteReg(0x0053,0x013f); //垂直GRAM终止位置  
        
        LcdWriteReg(0x0060,0xa700);        
        LcdWriteReg(0x0061,0x0001); 
        LcdWriteReg(0x006a,0x0000);
        LcdWriteReg(0x0080,0x0000);
        LcdWriteReg(0x0081,0x0000);
        LcdWriteReg(0x0082,0x0000);
        LcdWriteReg(0x0083,0x0000);
        LcdWriteReg(0x0084,0x0000);
        LcdWriteReg(0x0085,0x0000);
      
        LcdWriteReg(0x0090,0x0010);     
        LcdWriteReg(0x0092,0x0000);  
        LcdWriteReg(0x0093,0x0003);
        LcdWriteReg(0x0095,0x0110);
        LcdWriteReg(0x0097,0x0000);        
        LcdWriteReg(0x0098,0x0000);  
        //开启显示设置    
        LcdWriteReg(0x0007,0x0133);   
        LcdWriteReg(0x0020,0x0000);                                                            
        LcdWriteReg(0x0021,0x013f);
	}else if(DeviceCode==0x9320||DeviceCode==0x9300)
	{
		LcdWriteReg(0x00,0x0000);
		LcdWriteReg(0x01,0x0100);	//Driver Output Contral.
		LcdWriteReg(0x02,0x0700);	//LCD Driver Waveform Contral.
		LcdWriteReg(0x03,0x1030);//Entry Mode Set.
		//LcdWriteReg(0x03,0x1018);	//Entry Mode Set.
	
		LcdWriteReg(0x04,0x0000);	//Scalling Contral.
		LcdWriteReg(0x08,0x0202);	//Display Contral 2.(0x0207)
		LcdWriteReg(0x09,0x0000);	//Display Contral 3.(0x0000)
		LcdWriteReg(0x0a,0x0000);	//Frame Cycle Contal.(0x0000)
		LcdWriteReg(0x0c,(1<<0));	//Extern Display Interface Contral 1.(0x0000)
		LcdWriteReg(0x0d,0x0000);	//Frame Maker Position.
		LcdWriteReg(0x0f,0x0000);	//Extern Display Interface Contral 2.	    
		delay_ms(50); 
		LcdWriteReg(0x07,0x0101);	//Display Contral.
		delay_ms(50); 								  
		LcdWriteReg(0x10,(1<<12)|(0<<8)|(1<<7)|(1<<6)|(0<<4));	//Power Control 1.(0x16b0)
		LcdWriteReg(0x11,0x0007);								//Power Control 2.(0x0001)
		LcdWriteReg(0x12,(1<<8)|(1<<4)|(0<<0));				//Power Control 3.(0x0138)
		LcdWriteReg(0x13,0x0b00);								//Power Control 4.
		LcdWriteReg(0x29,0x0000);								//Power Control 7.
	
		LcdWriteReg(0x2b,(1<<14)|(1<<4));	    
		LcdWriteReg(0x50,0);	//Set X Star
		//水平GRAM终止位置Set X End.
		LcdWriteReg(0x51,239);	//Set Y Star
		LcdWriteReg(0x52,0);	//Set Y End.t.
		LcdWriteReg(0x53,319);	//
	
		LcdWriteReg(0x60,0x2700);	//Driver Output Control.
		LcdWriteReg(0x61,0x0001);	//Driver Output Control.
		LcdWriteReg(0x6a,0x0000);	//Vertical Srcoll Control.
	
		LcdWriteReg(0x80,0x0000);	//Display Position? Partial Display 1.
		LcdWriteReg(0x81,0x0000);	//RAM Address Start? Partial Display 1.
		LcdWriteReg(0x82,0x0000);	//RAM Address End-Partial Display 1.
		LcdWriteReg(0x83,0x0000);	//Displsy Position? Partial Display 2.
		LcdWriteReg(0x84,0x0000);	//RAM Address Start? Partial Display 2.
		LcdWriteReg(0x85,0x0000);	//RAM Address End? Partial Display 2.
	
		LcdWriteReg(0x90,(0<<7)|(16<<0));	//Frame Cycle Contral.(0x0013)
		LcdWriteReg(0x92,0x0000);	//Panel Interface Contral 2.(0x0000)
		LcdWriteReg(0x93,0x0001);	//Panel Interface Contral 3.
		LcdWriteReg(0x95,0x0110);	//Frame Cycle Contral.(0x0110)
		LcdWriteReg(0x97,(0<<8));	//
		LcdWriteReg(0x98,0x0000);	//Frame Cycle Contral.	   
		LcdWriteReg(0x07,0x0173);	//(0x0173)
	}else if(DeviceCode==0x5408)
	{
		LcdWriteReg(0x01,0x0100);								  
		LcdWriteReg(0x02,0x0700);//LCD Driving Waveform Contral 
		LcdWriteReg(0x03,0x1030);//Entry Mode设置 	   
		//指针从左至右自上而下的自动增模式
		//Normal Mode(Window Mode disable)
		//RGB格式
		//16位数据2次传输的8总线设置
		LcdWriteReg(0x04,0x0000); //Scalling Control register     
		LcdWriteReg(0x08,0x0207); //Display Control 2 
		LcdWriteReg(0x09,0x0000); //Display Control 3	 
		LcdWriteReg(0x0A,0x0000); //Frame Cycle Control	 
		LcdWriteReg(0x0C,0x0000); //External Display Interface Control 1 
		LcdWriteReg(0x0D,0x0000); //Frame Maker Position		 
		LcdWriteReg(0x0F,0x0000); //External Display Interface Control 2 
 		delay_ms(20);
		//TFT 液晶彩色图像显示方法14
		LcdWriteReg(0x10,0x16B0); //0x14B0 //Power Control 1
		LcdWriteReg(0x11,0x0001); //0x0007 //Power Control 2
		LcdWriteReg(0x17,0x0001); //0x0000 //Power Control 3
		LcdWriteReg(0x12,0x0138); //0x013B //Power Control 4
		LcdWriteReg(0x13,0x0800); //0x0800 //Power Control 5
		LcdWriteReg(0x29,0x0009); //NVM read data 2
		LcdWriteReg(0x2a,0x0009); //NVM read data 3
		LcdWriteReg(0xa4,0x0000);	 
		LcdWriteReg(0x50,0x0000); //设置操作窗口的X轴开始列
		LcdWriteReg(0x51,0x00EF); //设置操作窗口的X轴结束列
		LcdWriteReg(0x52,0x0000); //设置操作窗口的Y轴开始行
		LcdWriteReg(0x53,0x013F); //设置操作窗口的Y轴结束行
		LcdWriteReg(0x60,0x2700); //Driver Output Control
		//设置屏幕的点数以及扫描的起始行
		LcdWriteReg(0x61,0x0001); //Driver Output Control
		LcdWriteReg(0x6A,0x0000); //Vertical Scroll Control
		LcdWriteReg(0x80,0x0000); //Display Position – Partial Display 1
		LcdWriteReg(0x81,0x0000); //RAM Address Start – Partial Display 1
		LcdWriteReg(0x82,0x0000); //RAM address End - Partial Display 1
		LcdWriteReg(0x83,0x0000); //Display Position – Partial Display 2
		LcdWriteReg(0x84,0x0000); //RAM Address Start – Partial Display 2
		LcdWriteReg(0x85,0x0000); //RAM address End – Partail Display2
		LcdWriteReg(0x90,0x0013); //Frame Cycle Control
		LcdWriteReg(0x92,0x0000);  //Panel Interface Control 2
		LcdWriteReg(0x93,0x0003); //Panel Interface control 3
		LcdWriteReg(0x95,0x0110);  //Frame Cycle Control
		LcdWriteReg(0x07,0x0173);		 
		delay_ms(50);
	}	
	else if(DeviceCode==0x1505)
	{
		// second release on 3/5  ,luminance is acceptable,water wave appear during camera preview
        LcdWriteReg(0x0007,0x0000);
        delay_ms(50); 
        LcdWriteReg(0x0012,0x011C);//0x011A   why need to set several times?
        LcdWriteReg(0x00A4,0x0001);//NVM	 
        LcdWriteReg(0x0008,0x000F);
        LcdWriteReg(0x000A,0x0008);
        LcdWriteReg(0x000D,0x0008);	    
  		//伽马校正
        LcdWriteReg(0x0030,0x0707);
        LcdWriteReg(0x0031,0x0007); //0x0707
        LcdWriteReg(0x0032,0x0603); 
        LcdWriteReg(0x0033,0x0700); 
        LcdWriteReg(0x0034,0x0202); 
        LcdWriteReg(0x0035,0x0002); //?0x0606
        LcdWriteReg(0x0036,0x1F0F);
        LcdWriteReg(0x0037,0x0707); //0x0f0f  0x0105
        LcdWriteReg(0x0038,0x0000); 
        LcdWriteReg(0x0039,0x0000); 
        LcdWriteReg(0x003A,0x0707); 
        LcdWriteReg(0x003B,0x0000); //0x0303
        LcdWriteReg(0x003C,0x0007); //?0x0707
        LcdWriteReg(0x003D,0x0000); //0x1313//0x1f08
        delay_ms(50); 
        LcdWriteReg(0x0007,0x0001);
        LcdWriteReg(0x0017,0x0001);//开启电源
        delay_ms(50); 
  		//电源配置
        LcdWriteReg(0x0010,0x17A0); 
        LcdWriteReg(0x0011,0x0217);//reference voltage VC[2:0]   Vciout = 1.00*Vcivl
        LcdWriteReg(0x0012,0x011E);//0x011c  //Vreg1out = Vcilvl*1.80   is it the same as Vgama1out ?
        LcdWriteReg(0x0013,0x0F00);//VDV[4:0]-->VCOM Amplitude VcomL = VcomH - Vcom Ampl
        LcdWriteReg(0x002A,0x0000);  
        LcdWriteReg(0x0029,0x000A);//0x0001F  Vcomh = VCM1[4:0]*Vreg1out    gate source voltage??
        LcdWriteReg(0x0012,0x013E);// 0x013C  power supply on
        //Coordinates Control//
        LcdWriteReg(0x0050,0x0000);//0x0e00
        LcdWriteReg(0x0051,0x00EF); 
        LcdWriteReg(0x0052,0x0000); 
        LcdWriteReg(0x0053,0x013F); 
    	//Pannel Image Control//
        LcdWriteReg(0x0060,0x2700); 
        LcdWriteReg(0x0061,0x0001); 
        LcdWriteReg(0x006A,0x0000); 
        LcdWriteReg(0x0080,0x0000); 
    	//Partial Image Control//
        LcdWriteReg(0x0081,0x0000); 
        LcdWriteReg(0x0082,0x0000); 
        LcdWriteReg(0x0083,0x0000); 
        LcdWriteReg(0x0084,0x0000); 
        LcdWriteReg(0x0085,0x0000); 
  		//Panel Interface Control//
        LcdWriteReg(0x0090,0x0013);//0x0010 frenqucy
        LcdWriteReg(0x0092,0x0300); 
        LcdWriteReg(0x0093,0x0005); 
        LcdWriteReg(0x0095,0x0000); 
        LcdWriteReg(0x0097,0x0000); 
        LcdWriteReg(0x0098,0x0000); 
  
        LcdWriteReg(0x0001,0x0100); 
        LcdWriteReg(0x0002,0x0700); 
        LcdWriteReg(0x0003,0x1030); 
        LcdWriteReg(0x0004,0x0000); 
        LcdWriteReg(0x000C,0x0000); 
        LcdWriteReg(0x000F,0x0000); 
        LcdWriteReg(0x0020,0x0000); 
        LcdWriteReg(0x0021,0x0000); 
        LcdWriteReg(0x0007,0x0021); 
        delay_ms(20);
        LcdWriteReg(0x0007,0x0061); 
        delay_ms(20);
        LcdWriteReg(0x0007,0x0173); 
        delay_ms(20);
	}else if(DeviceCode==0xB505)
	{
		LcdWriteReg(0x0000,0x0000);
		LcdWriteReg(0x0000,0x0000);
		LcdWriteReg(0x0000,0x0000);
		LcdWriteReg(0x0000,0x0000);
		
		LcdWriteReg(0x00a4,0x0001);
		delay_ms(20);		  
		LcdWriteReg(0x0060,0x2700);
		LcdWriteReg(0x0008,0x0202);
		
		LcdWriteReg(0x0030,0x0214);
		LcdWriteReg(0x0031,0x3715);
		LcdWriteReg(0x0032,0x0604);
		LcdWriteReg(0x0033,0x0e16);
		LcdWriteReg(0x0034,0x2211);
		LcdWriteReg(0x0035,0x1500);
		LcdWriteReg(0x0036,0x8507);
		LcdWriteReg(0x0037,0x1407);
		LcdWriteReg(0x0038,0x1403);
		LcdWriteReg(0x0039,0x0020);
		
		LcdWriteReg(0x0090,0x001a);
		LcdWriteReg(0x0010,0x0000);
		LcdWriteReg(0x0011,0x0007);
		LcdWriteReg(0x0012,0x0000);
		LcdWriteReg(0x0013,0x0000);
		delay_ms(20);
		
		LcdWriteReg(0x0010,0x0730);
		LcdWriteReg(0x0011,0x0137);
		delay_ms(20);
		
		LcdWriteReg(0x0012,0x01b8);
		delay_ms(20);
		
		LcdWriteReg(0x0013,0x0f00);
		LcdWriteReg(0x002a,0x0080);
		LcdWriteReg(0x0029,0x0048);
		delay_ms(20);
		
		LcdWriteReg(0x0001,0x0100);
		LcdWriteReg(0x0002,0x0700);
		LcdWriteReg(0x0003,0x1230);
		LcdWriteReg(0x0008,0x0202);
		LcdWriteReg(0x000a,0x0000);
		LcdWriteReg(0x000c,0x0000);
		LcdWriteReg(0x000d,0x0000);
		LcdWriteReg(0x000e,0x0030);
		LcdWriteReg(0x0050,0x0000);
		LcdWriteReg(0x0051,0x00ef);
		LcdWriteReg(0x0052,0x0000);
		LcdWriteReg(0x0053,0x013f);
		LcdWriteReg(0x0060,0x2700);
		LcdWriteReg(0x0061,0x0001);
		LcdWriteReg(0x006a,0x0000);
		//LcdWriteReg(0x0080,0x0000);
		//LcdWriteReg(0x0081,0x0000);
		LcdWriteReg(0x0090,0X0011);
		LcdWriteReg(0x0092,0x0600);
		LcdWriteReg(0x0093,0x0402);
		LcdWriteReg(0x0094,0x0002);
		delay_ms(20);
		
		LcdWriteReg(0x0007,0x0001);
		delay_ms(20);
		LcdWriteReg(0x0007,0x0061);
		LcdWriteReg(0x0007,0x0173);
		
		LcdWriteReg(0x0020,0x0000);
		LcdWriteReg(0x0021,0x0000);	  
		LcdWriteReg(0x00,0x22);  
	}else if(DeviceCode==0xC505)
	{
		LcdWriteReg(0x0000,0x0000);
		LcdWriteReg(0x0000,0x0000);
		delay_ms(20);		  
		LcdWriteReg(0x0000,0x0000);
		LcdWriteReg(0x0000,0x0000);
		LcdWriteReg(0x0000,0x0000);
		LcdWriteReg(0x0000,0x0000);
 		LcdWriteReg(0x00a4,0x0001);
		delay_ms(20);		  
		LcdWriteReg(0x0060,0x2700);
		LcdWriteReg(0x0008,0x0806);
		
		LcdWriteReg(0x0030,0x0703);//gamma setting
		LcdWriteReg(0x0031,0x0001);
		LcdWriteReg(0x0032,0x0004);
		LcdWriteReg(0x0033,0x0102);
		LcdWriteReg(0x0034,0x0300);
		LcdWriteReg(0x0035,0x0103);
		LcdWriteReg(0x0036,0x001F);
		LcdWriteReg(0x0037,0x0703);
		LcdWriteReg(0x0038,0x0001);
		LcdWriteReg(0x0039,0x0004);
		
		
		
		LcdWriteReg(0x0090, 0x0015);	//80Hz
		LcdWriteReg(0x0010, 0X0410);	//BT,AP
		LcdWriteReg(0x0011,0x0247);	//DC1,DC0,VC
		LcdWriteReg(0x0012, 0x01BC);
		LcdWriteReg(0x0013, 0x0e00);
		delay_ms(120);
		LcdWriteReg(0x0001, 0x0100);
		LcdWriteReg(0x0002, 0x0200);
		LcdWriteReg(0x0003, 0x1030);
		
		LcdWriteReg(0x000A, 0x0008);
		LcdWriteReg(0x000C, 0x0000);
		
		LcdWriteReg(0x000E, 0x0020);
		LcdWriteReg(0x000F, 0x0000);
		LcdWriteReg(0x0020, 0x0000);	//H Start
		LcdWriteReg(0x0021, 0x0000);	//V Start
		LcdWriteReg(0x002A,0x003D);	//vcom2
		delay_ms(20);
		LcdWriteReg(0x0029, 0x002d);
		LcdWriteReg(0x0050, 0x0000);
		LcdWriteReg(0x0051, 0xD0EF);
		LcdWriteReg(0x0052, 0x0000);
		LcdWriteReg(0x0053, 0x013F);
		LcdWriteReg(0x0061, 0x0000);
		LcdWriteReg(0x006A, 0x0000);
		LcdWriteReg(0x0092,0x0300); 
 
 		LcdWriteReg(0x0093, 0x0005);
		LcdWriteReg(0x0007, 0x0100);
	}else if(DeviceCode==0x8989)
	{	   
		LcdWriteReg(0x0000,0x0001);//打开晶振
    	LcdWriteReg(0x0003,0xA8A4);//0xA8A4
    	LcdWriteReg(0x000C,0x0000);    
    	LcdWriteReg(0x000D,0x080C);   
    	LcdWriteReg(0x000E,0x2B00);    
    	LcdWriteReg(0x001E,0x00B0);    
    	LcdWriteReg(0x0001,0x2B3F);//驱动输出控制320*240  0x6B3F
    	LcdWriteReg(0x0002,0x0600);
    	LcdWriteReg(0x0010,0x0000);  
    	LcdWriteReg(0x0011,0x6070); //定义数据格式  16位色 		横屏 0x6058
    	LcdWriteReg(0x0005,0x0000);  
    	LcdWriteReg(0x0006,0x0000);  
    	LcdWriteReg(0x0016,0xEF1C);  
    	LcdWriteReg(0x0017,0x0003);  
    	LcdWriteReg(0x0007,0x0233); //0x0233       
    	LcdWriteReg(0x000B,0x0000);  
    	LcdWriteReg(0x000F,0x0000); //扫描开始地址
    	LcdWriteReg(0x0041,0x0000);  
    	LcdWriteReg(0x0042,0x0000);  
    	LcdWriteReg(0x0048,0x0000);  
    	LcdWriteReg(0x0049,0x013F);  
    	LcdWriteReg(0x004A,0x0000);  
    	LcdWriteReg(0x004B,0x0000);  
    	LcdWriteReg(0x0044,0xEF00);  
    	LcdWriteReg(0x0045,0x0000);  
    	LcdWriteReg(0x0046,0x013F);  
    	LcdWriteReg(0x0030,0x0707);  
    	LcdWriteReg(0x0031,0x0204);  
    	LcdWriteReg(0x0032,0x0204);  
    	LcdWriteReg(0x0033,0x0502);  
    	LcdWriteReg(0x0034,0x0507);  
    	LcdWriteReg(0x0035,0x0204);  
    	LcdWriteReg(0x0036,0x0204);  
    	LcdWriteReg(0x0037,0x0502);  
    	LcdWriteReg(0x003A,0x0302);  
    	LcdWriteReg(0x003B,0x0302);  
    	LcdWriteReg(0x0023,0x0000);  
    	LcdWriteReg(0x0024,0x0000);  
    	LcdWriteReg(0x0025,0x8000);  
    	LcdWriteReg(0x004f,0);        //行首址0
    	LcdWriteReg(0x004e,0);        //列首址0
	}else if(DeviceCode==0x4531)
	{
		LcdWriteReg(0X00,0X0001);   
		delay_ms(10);   
		LcdWriteReg(0X10,0X1628);   
		LcdWriteReg(0X12,0X000e);//0x0006    
		LcdWriteReg(0X13,0X0A39);   
		delay_ms(10);   
		LcdWriteReg(0X11,0X0040);   
		LcdWriteReg(0X15,0X0050);   
		delay_ms(10);   
		LcdWriteReg(0X12,0X001e);//16    
		delay_ms(10);   
		LcdWriteReg(0X10,0X1620);   
		LcdWriteReg(0X13,0X2A39);   
		delay_ms(10);   
		LcdWriteReg(0X01,0X0100);   
		LcdWriteReg(0X02,0X0300);   
		LcdWriteReg(0X03,0X1030);//改变方向的   
		LcdWriteReg(0X08,0X0202);   
		LcdWriteReg(0X0A,0X0008);   
		LcdWriteReg(0X30,0X0000);   
		LcdWriteReg(0X31,0X0402);   
		LcdWriteReg(0X32,0X0106);   
		LcdWriteReg(0X33,0X0503);   
		LcdWriteReg(0X34,0X0104);   
		LcdWriteReg(0X35,0X0301);   
		LcdWriteReg(0X36,0X0707);   
		LcdWriteReg(0X37,0X0305);   
		LcdWriteReg(0X38,0X0208);   
		LcdWriteReg(0X39,0X0F0B);   
		LcdWriteReg(0X41,0X0002);   
		LcdWriteReg(0X60,0X2700);   
		LcdWriteReg(0X61,0X0001);   
		LcdWriteReg(0X90,0X0210);   
		LcdWriteReg(0X92,0X010A);   
		LcdWriteReg(0X93,0X0004);   
		LcdWriteReg(0XA0,0X0100);   
		LcdWriteReg(0X07,0X0001);   
		LcdWriteReg(0X07,0X0021);   
		LcdWriteReg(0X07,0X0023);   
		LcdWriteReg(0X07,0X0033);   
		LcdWriteReg(0X07,0X0133);   
		LcdWriteReg(0XA0,0X0000); 
	}else if(DeviceCode==0x4535)
	{			      
		LcdWriteReg(0X15,0X0030);   
		LcdWriteReg(0X9A,0X0010);   
 		LcdWriteReg(0X11,0X0020);   
 		LcdWriteReg(0X10,0X3428);   
		LcdWriteReg(0X12,0X0002);//16    
 		LcdWriteReg(0X13,0X1038);   
		delay_ms(40);   
		LcdWriteReg(0X12,0X0012);//16    
		delay_ms(40);   
  		LcdWriteReg(0X10,0X3420);   
 		LcdWriteReg(0X13,0X3038);   
		delay_ms(70);   
		LcdWriteReg(0X30,0X0000);   
		LcdWriteReg(0X31,0X0402);   
		LcdWriteReg(0X32,0X0307);   
		LcdWriteReg(0X33,0X0304);   
		LcdWriteReg(0X34,0X0004);   
		LcdWriteReg(0X35,0X0401);   
		LcdWriteReg(0X36,0X0707);   
		LcdWriteReg(0X37,0X0305);   
		LcdWriteReg(0X38,0X0610);   
		LcdWriteReg(0X39,0X0610); 
		  
		LcdWriteReg(0X01,0X0100);   
		LcdWriteReg(0X02,0X0300);   
		LcdWriteReg(0X03,0X1030);//改变方向的   
		LcdWriteReg(0X08,0X0808);   
		LcdWriteReg(0X0A,0X0008);   
 		LcdWriteReg(0X60,0X2700);   
		LcdWriteReg(0X61,0X0001);   
		LcdWriteReg(0X90,0X013E);   
		LcdWriteReg(0X92,0X0100);   
		LcdWriteReg(0X93,0X0100);   
 		LcdWriteReg(0XA0,0X3000);   
 		LcdWriteReg(0XA3,0X0010);   
		LcdWriteReg(0X07,0X0001);   
		LcdWriteReg(0X07,0X0021);   
		LcdWriteReg(0X07,0X0023);   
		LcdWriteReg(0X07,0X0033);   
		LcdWriteReg(0X07,0X0133);   
	}	
	LcdScanDir(DFT_SCAN_DIR);		 
	LCD_LED = 1;//点亮背光	 
	LcdClearScreen(WHITE);
} 
//设置LCD的自动扫描方向
//注意:其他函数可能会受到此函数设置的影响(尤其是9341这个奇葩),所以,
//一般设置为L2R_U2D即可,如果设置为其他扫描方式,可能导致显示不正常.
//0~7：代表8个方向(具体定义见lcd.h)
//9320/9325/9328/4531/4535/1505/b505/8989/5408/9341等IC已经实际测试	   
void LcdScanDir(u8 dir)
{
	u16 regval = 0;
	u8 dirreg = 0;
#if USE_HORIZONTAL//使用横屏
	switch(dir)//方向转换
	{
		case 0:dir=6;break;
		case 1:dir=7;break;
		case 2:dir=4;break;
		case 3:dir=5;break;
		case 4:dir=1;break;
		case 5:dir=0;break;
		case 6:dir=3;break;
		case 7:dir=2;break;	     
	}
#endif
	if(DeviceCode==0x9341)//9341,很特殊
	{
		switch(dir)
		{
			case L2R_U2D://从左到右,从上到下
				regval|=(0<<7)|(0<<6)|(0<<5); 
				break;
			case L2R_D2U://从左到右,从下到上
				regval|=(1<<7)|(0<<6)|(0<<5); 
				break;
			case R2L_U2D://从右到左,从上到下
				regval|=(0<<7)|(1<<6)|(0<<5); 
				break;
			case R2L_D2U://从右到左,从下到上
				regval|=(1<<7)|(1<<6)|(0<<5); 
				break;	 
			case U2D_L2R://从上到下,从左到右
				regval|=(0<<7)|(0<<6)|(1<<5); 
				break;
			case U2D_R2L://从上到下,从右到左
				regval|=(0<<7)|(1<<6)|(1<<5); 
				break;
			case D2U_L2R://从下到上,从左到右
				regval|=(1<<7)|(0<<6)|(1<<5); 
				break;
			case D2U_R2L://从下到上,从右到左
				regval|=(1<<7)|(1<<6)|(1<<5); 
				break;	 
		}
		dirreg=0X36;
 		regval|=0X08;//BGR   
		LcdWriteReg(dirreg,regval);
		if(regval&0X20)
		{
 	 		LcdWriteCmd(0x2A); 
			LcdWriteData(0);
			LcdWriteData(0);
			LcdWriteData(319>>8);
			LcdWriteData(319&0XFF);
	 		LcdWriteCmd(0x2B); 
			LcdWriteData(0);
			LcdWriteData(0);
			LcdWriteData(239>>8);
			LcdWriteData(239&0XFF);
		}else 
		{
 	 		LcdWriteCmd(0x2A); 
			LcdWriteData(0);
			LcdWriteData(0);
			LcdWriteData(239>>8);
			LcdWriteData(239&0XFF);
	 		LcdWriteCmd(0x2B); 
			LcdWriteData(0);
			LcdWriteData(0);
			LcdWriteData(319>>8);
			LcdWriteData(319&0XFF);
		}  
 	}else 
	{
		switch(dir)
		{
			case L2R_U2D://从左到右,从上到下
				regval|=(1<<5)|(1<<4)|(0<<3); 
				break;
			case L2R_D2U://从左到右,从下到上
				regval|=(0<<5)|(1<<4)|(0<<3); 
				break;
			case R2L_U2D://从右到左,从上到下
				regval|=(1<<5)|(0<<4)|(0<<3);
				break;
			case R2L_D2U://从右到左,从下到上
				regval|=(0<<5)|(0<<4)|(0<<3); 
				break;	 
			case U2D_L2R://从上到下,从左到右
				regval|=(1<<5)|(1<<4)|(1<<3); 
				break;
			case U2D_R2L://从上到下,从右到左
				regval|=(1<<5)|(0<<4)|(1<<3); 
				break;
			case D2U_L2R://从下到上,从左到右
				regval|=(0<<5)|(1<<4)|(1<<3); 
				break;
			case D2U_R2L://从下到上,从右到左
				regval|=(0<<5)|(0<<4)|(1<<3); 
				break;	 
		}
		if(DeviceCode == 0x8989)//8989 IC
		{
			dirreg = 0X11;
			regval|=0X6040;	//65K   
	 	}else//其他驱动IC		  
		{
			dirreg = 0X03;
			regval|=1<<12;  
		}
		LcdWriteReg(dirreg,regval);
	}
}



