#include"dht11.h"

u8 temph,templ,humih,humil;

/**函数功能：dht11开始工作*/
void Start()
{
	DHT11_IO_OUT();				/*将dht11数据口设置成输出（主机信号）*/

	dhtout = 0;					//拉低
	delay_ms(20);				//延时20ms	至少18ms
	dhtout = 1;					//拉高
	delay_us(30);				//此处延时20~40us
}

/*dht11读一个字节*/
u8 ReceiveByte(void) 
{
	u8 value ,i;
	u8 datbyte;

	for(i=0; i<8; i++)
	{
		while(!dhtin);	   //等待响应信号结束
		value = 0;

		delay_us(28);	   
//响应信号结束之后延时26us-28us  
// 必须要很精确

		if( dhtin == 1 )  		 //如果28us过后还是高电平  说明是1
			value = 1;
		while(dhtin);				//等待数据信号'0'为26us-28us'1'为70us
		datbyte <<= 1;			//左移
		datbyte |= value;		//组合数据
	}
	return datbyte ;
}
void ReceiveData(void)
{
	u8 check ,jycheck ,TH,TL,HH,HL;
	
	Start();				//调用开始函数
	DHT11_IO_IN();			//从机信号 要设置成输入
	dhtin = 1;				//先拉高 

	if(!dhtin)		//判断是否有低电平响应信号
	{
		while(!dhtin);		//判断响应信号是否结束
		while(dhtin);		//等待响应信号后的40~50us高电平结束	结束后开始接收数据
		HH = ReceiveByte();				 //湿度高位
		HL = ReceiveByte();				 //湿度低位
		TH = ReceiveByte();				 //温度高位
		TL = ReceiveByte();				 //温度低位
		check = ReceiveByte();			 //校验位

		dhtin = 0;						 //数据接收结束 拉低延时50us
		delay_us(50);

		dhtin = 1; 				//拉高 进入空闲

		jycheck = HH+HL+TH+TL ;		  //数据整合

		if(jycheck == check)		//判断校验位数据是否等于数据之和
		{
			humih = HH;
			humil = HL;
			temph = TH;
			templ = TL;
			check = jycheck;
		}
	}	
}

/*dht11 I/O口初始化*/
void Dht11IoInit(void)
{
	RCC -> APB2ENR |= 1<<8;  //使能GPIOG口时钟

	GPIOG -> CRH &= 0XFFFF0FFF;		   //设置成推挽输出
	GPIOG -> CRH |= 0X00003000;
	GPIOG -> ODR |= 1<<11;			   //将该位挂高
}

