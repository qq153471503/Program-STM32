
#include "syn7318.h"
#include "string.h"
#include "delay.h"
#include "led.h"
#include "usart.h"
#include "djqd.h"
#include "test.h"
#include "hw.h"
#include "syn7318.h"
#include "canp_hostcom.h"
#include "data_channel.h"
u8 a=0,RE_flag;
unsigned char Ysn7813_flag=0;
unsigned char Ysn7813_flag_cs=0;

//	                         帧头  数据长度 唤醒 百灵 04/自定义 31=0X1F
unsigned char Wake_Up[] = {0xfd,0x00,0x02,0x51,0x1F};
unsigned char Stop_Wake_Up[] = {0xFD,0x00,0x01,0x52};
//	unsigned char Play_MP3[] = {0xFD,0x00,0x18,0x61,0x01,0x44,0x3A,0x5C,0x4D,0x70,0x33,0x5C,0x57,0x69,0x66,0x69,0x5F,0xCE,0xD2,0xD4,0xDA,0xD5,0xE2,0x2E,0x6D,0x70,0x33};

  unsigned char Play_MP3[] ={ 0xFD,0x00,0x1E,0x01,0x01,0xC6,0xF4,0xB6,0xAF,0xD3, 0xEF ,0xD2, 0xF4,
                            	0xBF, 0xD8, 0xD6 ,0xC6 ,0xBC ,0xDD ,0xCA ,0xBB ,0xA3 ,0xAC, 0xC7, 0xEB,
                             	0xB7, 0xA2, 0xB3, 0xF6 ,0xD6, 0xB8, 0xC1, 0xEE };
//	0xFD 0x00 0x1E 0x01 0x01 0xC6 0xF4 0xB6 0xAF 0xD3 0xEF 0xD2 0xF4 0xBF 0xD8 0xD6 0xC6 0xBC 0xDD 0xCA 0xBB 0xA3 0xAC 0xC7 0xEB 0xB7 0xA2 0xB3 0xF6 0xD6 0xB8 0xC1 0xEE 
//   0xFD 0x00 0x23 0x01 0x01 0xFD 0x00 0x1E 0x01 0x01 0xBB 0xB6 0xD3 0xAD 0xB9 0xDB 0xBF 0xB4 0xD3 0xEF 0xD2 0xF4 0xBA 0xCF 0xB3 0xC9 0xCF 0xB5 0xCD 0xB3 0xB5 0xC4 0xD1 0xDD 0xCA 0xBE 0xA3 0xA1
	//                             帧头  数据长度 识别 词典	
unsigned char Start_ASR_Buf[] = {0xFD,0x00,0x02,0x10,0x03};
unsigned char Stop_ASR_Buf[] = {0xFD,0x00,0x01,0x11};

uchar D_MP3_1[] = {"D:\\Mp3\\Wifi_首次开机.mp3"};
uchar D_MP3_2[] = {"D:\Mp3\Prompt_主人请吩咐.mp3"};
uchar D_MP3_3[] = {"D:\Mp3\Answer_抱歉没听清.mp3"};
uchar D_MP3_4[] = {0xFD,0x00,0x21,0x15,0x03,0x04,0x02,0x01,0x44,0x3A,0x5C,0x4D,0x70,0x33,0x5C,0x50,0x72,0x6F,0x6D,0x70,0x74,0x5F,0xD6,0xF7,0xC8,0xCB,0xC7,0xEB,0xB7,0xD4,0xB8,0xC0,0x2E,0x6D,0x70,0x33};
uchar Stop_Th2O[] = {0xFD,0x00,0x01,0x16};
uchar TF_MP3_1[] = {"E:\\1-歌曲\\一条大鱼不见了.mp3"};

uchar *Dict0[] = {"音量加","音量减","最大音量","最小音量","打开提示音","关闭提示音","请开灯","请关灯","调暗一点","调亮一点","最小亮度","最大亮度"};//00词典
uchar *Dict1[] = {"你好","查无此人","缠绵游戏","长江之歌","常回家看看","超时空爱情","超速游戏","超越梦想","潮湿的心","沉默是金","成人对待","城里的月光"};//01词典
uchar *Dict2[] = {"一条大鱼不见了","上学歌","五指歌","升国旗","可爱的太阳","丑小鸭","两兄弟的故事","会唱歌的秘密","农夫和蛇","北风和太阳","小偷和公鸡","公鸡叫"};//02词典
uchar *Dict3[] = {"第一个最好不放有用的词条"};//03词典 需要自己利用烧录成这个数组中的词条  
uchar ** DICT[10] = {Dict0,Dict1,Dict2,Dict3};

uchar Back[4] = {0};   //接收命令回传的数组
uchar ASR[6] = {0};    //接收识别结果回传的数组
uchar S[4] = {0};      //接收模块当前工作状态回传的数组


/*********************************************************************
【函 数 名】：Delay1ms
【参数说明】：无参
【简    例】：Delay1ms();延时1ms。
*********************************************************************/
void Delay1ms()		 
{
			delay_ms(1);

}

/*********************************************************************
【函 数 名】：Delay500ms
【参数说明】：无参
【简    例】：Delay500ms();延时500ms。
*********************************************************************/
void Delay500ms()	 
{
     delay_ms(500);
}

#define USART_USE     0 // 1 USART1 0 USART2

/*********************************************************************
【函 数 名】：SYN7318_Init
【参数说明】：无参
【简    例】：SYN7318_Init();初始化函数，波特率设置为115200bps
*********************************************************************/
void SYN7318_Init(void)
{
#if  USART_USE
    uart1_init( 115200 );
#else 
	  uart2_init( 115200 ,1);
#endif
	
  RCC->APB2ENR |= 1<<5;	 //PD4
	GPIOD->CRL&=0XFFF0FFFF;//PD4 推挽输出
	GPIOD->CRL|=0X00030000;	   
	GPIOD->ODR|=1<<4;     //PD4 输出高
	
	
}

u8  SYN7318_Rst(void)  //语音模块复位
{
   SYN7318_RST_H;	
	 delay_ms(10);
	 
   SYN7318_RST_L;	
	 delay_ms(100);	

   SYN7318_RST_H;  
	
	while ( 0x55 == SYN7318_Get_char() );
	SYN7318_Get_String(Back ,3 );
	if(Back[2] ==0x4A )  return 1;
	else return 0;
	
}

void SYN7318_Open(void)
{

   Ysn7813_flag =1;
	 Ysn7813_flag_cs =1;

}

void SYN7318_Close(void)
{

  Ysn7813_flag = 0;
	Ysn7813_flag_cs = 0;
//	SYN7318_Put_String(Stop_ASR_Buf,4); //停止语音识

}



/*********************************************************************
【函 数 名】：SYN7318_Put_Char----发送一个字节函数
【参数说明】：txd---待发送的字节（8位）
【简    例】：SYN7318_Put_Char('d');  发送‘d’
*********************************************************************/
void SYN7318_Put_Char(uchar txd)
{
#if  USART_USE	
	  U1SendChar( txd );
#else 
	  U2SendChar( txd );
#endif	
	
	
	
}

/*********************************************************************
【函 数 名】：SYN7318_Put_String----发送字符串函数
【参数说明】：Pst：存放字符串的数组名
              Length：字符串长度
【简    例】：uchar d[4] = {0x00,0x01,0x02,0x03};
              SYN7318_Put_String(d,4); ---发送数组d中的元素
*********************************************************************/
void SYN7318_Put_String(uchar* Pst,uint Length)
{ 
   	uint i;
	for(i = 0; i < Length; i++)
	{
		  SYN7318_Put_Char( Pst[i]);
	}
}

/*********************************************************************
【函 数 名】：SYN7318_Get_char----接收一个字节函数
【参数说明】：无参
【返 回 值】：接收到的字节
【简    例】：uchar d;
              d = SYN7318_Get_char();
              SYN7318_Get_char(d); -----输出接收到的字节
*********************************************************************/
#define WaitTime 10000
uchar SYN7318_Get_char( void) 
{
  	unsigned int return_data=0;	
#if  USART_USE	
	
	  while((!(USART1->SR & USART_FLAG_RXNE))||a>1000)
		{
			a++;
		}
	   return_data = (uint8_t)USART_ReceiveData(USART1);
 
#else 	
		while(!(USART2->SR & USART_FLAG_RXNE));
		return_data = (uint8_t)USART_ReceiveData(USART2);
#endif	
	
	return return_data;
	
}

/*********************************************************************
【函 数 名】：SYN7318_Get_String----接收字符串函数
【参数说明】：Pst：存放接收到的字符串的数组名
              Length：字符串长度
【简    例】：uchar d[4] = {0};
              SYN7318_Get_String(d,4);
              SYN7318_Get_String(d,4);----输出接收到的字符串 
*********************************************************************/
void SYN7318_Get_String(unsigned char *Pst,unsigned char Length)
{
  unsigned char i;
  for(i=0;i<Length;i++)
  {
     Pst[i] = SYN7318_Get_char();
  }
	
}

/*********************************************************************
【函 数 名】：SYN_TTS----语音合成播放函数
【参数说明】：Pst：存放要合成播放的文本的数组名
【简    例】：uchar Data[] = {"北京龙邱"};
              SYN_TTS(Data); -----合成播放北京龙邱
*********************************************************************/
void SYN_TTS(uchar *Pst)
{
	uint Length;
	uchar Frame[5];   //保存发送命令的数组
	
	
	Length = strlen(Pst); 
	Frame[0] = 0xFD;      //帧头
	Frame[1] = 0x00;
	Frame[2] = Length+2;
	Frame[3] = 0x01;      //语音合成播放命令
	Frame[4] = 0x00;      //播放编码格式为“GB2312”
	
  SYN7318_Put_String(Frame, 5);	
	SYN7318_Put_String(Pst, Length);	
	SYN7318_Get_String(Back,4);
	
	//接收成功
	while(!(Back[3] == 0x41))
	{
		SYN7318_Get_String(Back,4);
	}	
	//空闲监测
	SYN7318_Get_String(Back,4);
	while(!(Back[3] == 0x4f))
	{
		SYN7318_Get_String(Back,4);
	}	
}

/*********************************************************************
【函 数 名】：SYN_MP3----MP3播放函数
【参数说明】：Pst：存放要播放的MP3的数组名
【简    例】：uchar D_MP3_1[] = {"D:\\Mp3\\Wifi_首次开机.mp3"};
              SYN_MP3(D_MP3_1); ----播放MP3
*********************************************************************/
void SYN_MP3(uchar *Pst)
{
	uint Length;
	uchar Frame[5];   //保存发送命令的数组
	
	Length = strlen(Pst); 
	Frame[0] = 0xFD;      //帧头
	Frame[1] = 0x00;
	Frame[2] = Length+2;
	Frame[3] = 0x61;      //MP3播放命令
	Frame[4] = 0x04;      //播放编码格式为“GB2312”
	
  SYN7318_Put_String(Frame, 5);	
	SYN7318_Put_String(Pst, Length);	
	SYN7318_Get_String(Back,0);
//	if(Back[3] == 0x41)
//	   LED2 = 0;
}

/*********************************************************************
【函 数 名】：Start_ASR----开始语音识别函数
【参数说明】：Dict：词典编号
【简    例】：Start_ASR(0x00); ----识别0x00词典中的词条
*********************************************************************/
void Start_ASR(uchar Dict)
{

	uchar Frame[5];   //保存发送命令的数组
	
	Frame[0] = 0xFD;      //帧头
	Frame[1] = 0x00;
	Frame[2] = 0x02;   
	Frame[3] = 0x10;      //开始语音识别命令
	Frame[4] = Dict;      //词典编号  在这里修改想要识别的词典编号
	
	ASR[3]=0;
	
  SYN7318_Put_String(Frame, 5);	
	SYN7318_Get_String(Back,4); 
	if(Back[3] == 0x41)
	{
		 SYN7318_Get_String(Back,3);  //语音识别命令回传结果
		if(Back[0] == 0xfc)
		{
			 SYN7318_Get_String(ASR,Back[2]);
		}
	}
}

/*********************************************************************
【函 数 名】：Stop_ASR----停止语音识别函数
【参数说明】：无参
*********************************************************************/
void Stop_ASR()
{
	uchar Frame[4];   //保存发送命令的数组
	
	Frame[0] = 0xFD;      //帧头
	Frame[1] = 0x00;
	Frame[2] = 0x01;   
	Frame[3] = 0x11;      //停止语音识别命令
	
  SYN7318_Put_String(Frame, 4);	
	SYN7318_Get_String(Back,4); 
//	if(Back[3] == 0x41)
//	{
//	   LED8 = 0; //D7亮
//	}
}

/***************************************************************************
【函 数 名】：Dict_TTS----识别词典结果播报函数
【参数说明】：Dict：词典编号
【简    例】：Start_ASR(0x00); ----识别0x00词典中的词条
              Dict_TTS(0x00);------如果识别成功,把识别到的词条播报出来 
***************************************************************************/
void Dict_TTS(uchar Dict)
{
	switch(ASR[0])
	{
		case 0x01:
		case 0x02:
		{
			SYN_TTS(DICT[Dict][ASR[3]]);  //ASR[3]中存放词条ID
		}break;
		case 0x03:
		{
			SYN_TTS("用户静音超时");
		}break;
		case 0x04:
		{
			SYN_TTS("用户语音超时");
		}break;
		case 0x05:
		case 0x07:
		{
			SYN_TTS("识别拒识");
		}break;
		case 0x06:
		{
			SYN_TTS("识别内部错误");
		}break;
	}
}

/*********************************************************************************************************************************************************
【函 数 名】：Start_WakeUp----开始语音唤醒函数
【参数说明】：Wake_ID：唤醒名ID号----0x00：云宝  0x02：小播   0x04：百灵  0x06：叮当管家  0x07：百灵管家  0x08：小播管家  0x09：大管家  0x1F：自定义
【简    例】：Start_WakeUp(0x09);----唤醒名为大管家,唤醒成功之后播报“我在这”
**********************************************************************************************************************************************************/
void Start_WakeUp(uchar Wake_ID)
{
	uchar Frame[5];   //保存发送命令的数组
	
	Frame[0] = 0xFD;      //帧头
	Frame[1] = 0x00;
	Frame[2] = 0x02;   
	Frame[3] = 0x51;      //开始语音唤醒命令
	Frame[4] = Wake_ID;   
	
  SYN7318_Put_String(Frame,5);	
	SYN7318_Get_String(Back,4); 
	if(Back[3] == 0x41)  
	//while(Back[3] == 0x41)
	{
		 SYN7318_Get_String(Back,4);  //语音唤醒命令回传结果
		 if(Back[3] == 0x21)
		 //while(Back[3] == 0x21)
		 {
			 SYN_MP3(D_MP3_2);
		 }
	}
}

/*********************************************************************************************************************************************************
【函 数 名】：Three_One----三合一函数：开启语音唤醒，唤醒后播放提示音，播完后开启语音识别
【参数说明】：Dict：词典编号
              Wake_ID：唤醒名ID号----0x00：云宝  0x02：小播   0x04：百灵  0x06：叮当管家  0x07：百灵管家  0x08：小播管家  0x09：大管家  0x1F：自定义
              Sound：提示音类型  0x00：无提示音  0x01：文本类型  0x02：内置MP3类型
              Pst：提示音的内容
【简    例】：Three_One(0x00, 0x06, 0x02, D_MP3_2); ----唤醒名为叮当管家,唤醒成功后播放“我在这”,之后可以识别0x00词典中的词条 
              Dict_TTS(0x00); ----结合Dict_TTS函数把识别结果播报出来
**********************************************************************************************************************************************************/
void Three_One(uchar Dict, uchar Wake_ID, uchar Sound, uchar *Pst)
{
	uchar Frame[100];   //保存发送命令的数组
	uint Length,i;
	
	Frame[0] = 0xFD;      //帧头
//	Frame[1] = 0x00;
//	Frame[2] = 0x07;    //由于不确定数据区的长度，所以选择暂时屏蔽
	Frame[3] = 0x15;      //三合一识别命令
	Frame[4] = Dict;      //词典编号
	Frame[5] = Wake_ID;   //唤醒名ID号
	Frame[6] = Sound;     //提示音类型
	if(Sound == 0x00)     //无提示音
	{
		Frame[1] = 0x00;
		Frame[2] = 0x04;
	  SYN7318_Put_String(Frame, 7);	
	}
	else                 //有提示音
	{
		Frame[7] = 0x01;      //文本或内置MP3提示音编码格式：0x00：GB2312编码  0x01：GBK编码  0x02：BIG5编码  0x03：Unicode小头  0x04nicode大头
		Length = strlen(Pst); //文本或内置MP3提示音的长度
		Frame[1] = 0x00;
		Frame[2] = Length + 5;
		for(i = 0; i < Length; i++)
		{
			Frame[8 + i] = Pst[i];
		}
	  SYN7318_Put_String(Frame, Length + 8);	
	}
	 SYN7318_Get_String(Back,4); 
	if(Back[3] == 0x41)
	{
		SYN7318_Get_String(Back,3);  //语音识别命令回传结果
		if(Back[0] == 0xfc)
		{
			 SYN7318_Get_String(ASR,Back[2]);
		}
	}
}

/***************************************************************************
【函 数 名】：Status_Query----模块状态查询函数
【参数说明】：无参
【简    例】：Status_Query(); 
***************************************************************************/
void Status_Query()
{
	uchar Frame[4];   //保存发送命令的数组
	
	Frame[0] = 0xFD;    //帧头
	Frame[1] = 0x00;
	Frame[2] = 0x01;   
	Frame[3] = 0x21;   //状态查询命令
	
  SYN7318_Put_String(Frame, 4);	
	SYN7318_Get_String(Back,4); 
	if(Back[3] == 0x41)  
	{
		 SYN7318_Get_String(S,4);  //模块当前工作状态的回传结果
	}
}
/*
<------   接收成功             0xFC 0x0 0x1 0x41 
<------   识别拒识             0xFC 0x0 0x1 0x7 

三合一

<------   识别成功,匹配度=9,词条ID=16,命令ID=117    0xFC 0x0 0x6 0x1 0x9 0x0 0x10 0x0 0x75 
<------   0xFD,0x00,0x21,0x15,0x03,0x04,0x02,0x01,0x44,0x3A,0x5C,0x4D,0x70,0x33,0x5C,0x50,0x72,0x6F,0x6D,0x70,0x74,0x5F,0xD6,0xF7,0xC8,0xCB,0xC7,0xEB,0xB7,0xD4,0xB8,0xC0,0x2E,0x6D,0x70,0x33
          帧头   长度  命令字 词典 唤醒 内置 GBK  内容--->

唤醒

【开启唤醒】: len= 5   FD 00 02 51 04 
<------   接收成功     0xFC 0x0 0x1 0x41 

【语音合成播放】: len= 22   FD 00 13 01 01 5B 6D 35 35 5D A1 B0 B0 D9 C1 E9 A1 B1 5B 6D 33 5D 
<------   唤醒成功          0xFC 0x0 0x1 0x21 
<------   接收成功          0xFC 0x0 0x1 0x41 
<------   模块空闲          0xFC 0x0 0x1 0x4F 
*/

#define SYN_RST_MODE   1  // 0 不复位   1 开启复位

void SYN7318_Test( void)  // 开启语音测试
{ 
	unsigned int i = 0;

	
//	SYN7318_Init();
	  

  
//	Ysn7813_flag_cs =1;
//	while( Ysn7813_flag_cs )
	if(Ysn7813_flag_cs )
	{

#if 	SYN_RST_MODE	
		while( !SYN7318_Rst());
#else 
		
#endif
	//	Ysn7813_flag_cs =0;
		
		SYN_TTS("语音识别测试,请发语音驾驶");
		LED2 = !LED2;
		Status_Query();  //查询模块当前的工作状态
		delay_ms(1000);
		delay_ms(1000);
		Zigb_Rx_Buf[0]=0x55;
		Zigb_Rx_Buf[1]=0x06;
		Zigb_Rx_Buf[2]=0x10;
		Zigb_Rx_Buf[3]=0x01;
		Zigb_Rx_Buf[4]=0x00;
		Zigb_Rx_Buf[5]=0x00;
		Zigb_Rx_Buf[6]=0x11;
		Zigb_Rx_Buf[7]=0xBB;
		Send_ZigbeeData_To_Fifo(Zigb_Rx_Buf,8);
		if(S[3] == 0x4F)  //模块空闲即开启唤醒
		{
			Delay1ms();
			
			SYN7318_Put_String(Wake_Up,5);//发送唤醒指令
			SYN7318_Get_String(Back,4);   //接收反馈信息
		if(Back[3] == 0x41)         //接收成功
		{ 
			SYN7318_Get_String(Back,3); //接收前三位回传数据
			if(Back[0] == 0xfc)       //帧头判断
			{
				SYN7318_Get_String(ASR,Back[2]);//接收回传数据
				if(ASR[0] == 0x21)            //唤醒成功
				{
					SYN7318_Put_String(Play_MP3,33);//播放“我在这” 

					SYN7318_Get_String(Back,4);
					SYN7318_Get_String(Back,4);
					while(!(Back[3] == 0x4f));    //等待空闲
					{
						LED2 = ~LED2;
						Delay500ms();
					}
		//							Ysn7813_flag = 1;
		/*************************************************/
					//开始语音识别
					while(Ysn7813_flag)
					{
						
						SYN7318_Put_String(Start_ASR_Buf,5);//发语音识别命令
						SYN7318_Get_String(Back,4);     //接收反馈信息
						Zigb_Rx_Buf[0]=0x55;
						Zigb_Rx_Buf[1]=0x06;
						Zigb_Rx_Buf[2]=0x10;
						Zigb_Rx_Buf[3]=0x00+tu1;
						Zigb_Rx_Buf[4]=0x00;
						Zigb_Rx_Buf[5]=0x00;
						Zigb_Rx_Buf[6]=(0x10+tu1)%0xFF;
						Zigb_Rx_Buf[7]=0xBB;
						Send_ZigbeeData_To_Fifo(Zigb_Rx_Buf,8);
						if(Back[3] == 0x41)           //接收成功
						{
							LED1 = ~LED1;             //LED1反转
							SYN7318_Get_String(Back,3);  //语音识别命令回传结果
							if(Back[0] == 0xfc)        //帧头判断
							{
								LED2 = ~LED2;
								SYN7318_Get_String(ASR,Back[2]);//接收回传数据
								
								switch(ASR[0])
							{
									case 0x01:
									{
										
										switch(ASR[3])
										{
											case 0x00:  // 第一个词一般不用于识别
											{
												//	SYN_TTS("我在呢");
												SYN7318_Put_String(Stop_ASR_Buf,4); //停止语音识别
													break;
											}
											case 0x01:  // 前进
											{
												  SYN7318_Put_String(Stop_ASR_Buf,4); //停止语音识别
												  SYN7318_Get_String(Back,4);
												  if(Back[3] == 0x41)
													
													Go_Test( 50,50);
													SYN_TTS("小车前进已完成");
													break;
											}
											case 0x02:
											{
												SYN7318_Put_String(Stop_ASR_Buf,4); //停止语音识别 
												  SYN7318_Get_String(Back,4);
												  if(Back[3] == 0x41)
													
													Back_Test(50,50);
													SYN_TTS("小车后退已完成，等待下一步指令");
													break;
											}
											case 0x03:
											{
												  SYN7318_Put_String(Stop_ASR_Buf,4); //停止语音识别;
													SYN7318_Get_String(Back,4);
												  if(Back[3] == 0x41)
												  
												  Left_Test( 80);
													SYN_TTS("小车左转完成，等待下一步指令");
													Ysn7813_flag = 0;
												  Ysn7813_flag_cs =0;
													Bzuozhuan();
													Bzuozhuan();
													xunji();
													zuozhuan();
													xunji();
													qianjingMP(10);
													break;
											}
											case 0x04:
											{
												  SYN7318_Put_String(Stop_ASR_Buf,4); //停止语音识别
													SYN7318_Get_String(Back,4);
												  if(Back[3] == 0x41)
												   
													 Right_Test( 80);
													SYN_TTS("小车右转完成，等待下一步指令");
													Ysn7813_flag = 0;
												  Ysn7813_flag_cs =0;
													xunji();
													zuozhuan();
													xunji();
													qianjingMP(10);
													break;
											}
											case 0x05:
											{
												  SYN7318_Put_String(Stop_ASR_Buf,4); //停止语音识别
													SYN7318_Get_String(Back,4);
												  if(Back[3] == 0x41)
													
													Stop_Test();
													SYN_TTS("已停车，等待下一步指令");
													break;
											}
											case 0x06:
											{
												  SYN7318_Put_String(Stop_ASR_Buf,4); //停止语音识别
												  SYN7318_Get_String(Back,4);
													if(Back[3] == 0x41)
													{
													  
													  Track_Test( 50);
														SYN_TTS("寻迹已完成，等待下一步指令");
													}
													break;
											}
											case 0x07:
											{
												  SYN7318_Put_String(Stop_ASR_Buf,4); //停止语音识别
													 SYN7318_Get_String(Back,4);
													if(Back[3] == 0x41)
													{
														Left_Test( 80);
													SYN_TTS("禁止右转已完成，等待下一步指令");
														Ysn7813_flag = 0;
												  Ysn7813_flag_cs =0;
													Bzuozhuan();
													Bzuozhuan();
													xunji();
													zuozhuan();
													xunji();
													qianjingMP(10);
													}
													break;
											}
											case 0x08:
											{
												  SYN7318_Put_String(Stop_ASR_Buf,4); //停止语音识别
													 SYN7318_Get_String(Back,4);
													if(Back[3] == 0x41)
													{
														Send_ZigbeeData_To_Fifo( DZ_K ,8);  // 开启道闸
												   SYN_TTS("道闸已打开，等待下一步指令");
													}
													break;
											}
											case 0x09:
											{
												  SYN7318_Put_String(Stop_ASR_Buf,4); //停止语音识别
												  SYN7318_Get_String(Back,4);
													if(Back[3] == 0x41)
													{
														Send_ZigbeeData_To_Fifo( DZ_G ,8);  // 开启道闸
													SYN_TTS("道闸已关闭，等待下一步指令");
													}
													break;
											}
											case 0x0a:
											{
												  SYN7318_Put_String(Stop_ASR_Buf,4); //停止语音识别
													SYN7318_Get_String(Back,4);
												 if(Back[3] == 0x41)
													{
														Right_Test( 80);
													SYN_TTS("禁止左转完成，等待下一步指令");
														Ysn7813_flag = 0;
												  Ysn7813_flag_cs =0;
													xunji();
													zuozhuan();
													xunji();
													qianjingMP(10);
													}														
													break;
											}
											case 0x0b:
											{
												  SYN7318_Put_String(Stop_ASR_Buf,4); //停止语音识别
												  SYN7318_Get_String(Back,4);
												if(Back[3] == 0x41)
													{
														Send_ZigbeeData_To_Fifo( SMG_JSK ,8);  // 数码管计时
													SYN_TTS("计时系统已打开，等待下一步指令");
													}															
													break;
											}
											case 0x0c:
											{
												  SYN7318_Put_String(Stop_ASR_Buf,4); //停止语音识别
												  SYN7318_Get_String(Back,4);
												 if(Back[3] == 0x41)
													{
														Send_ZigbeeData_To_Fifo( SMG_JSG ,8);  // 数码管关闭
													SYN_TTS("计时系统已关闭，等待下一步指令");
													}
													break;
											}
											case 0x0d:
											{
												  SYN7318_Put_String(Stop_ASR_Buf,4); //停止语音识别
												  SYN7318_Get_String(Back,4);
												if(Back[3] == 0x41)
													{
														Send_ZigbeeData_To_Fifo( SMG_JL ,8);  // 数码管显示距离
													SYN_TTS("LED显示距离已完成，等待下一步指令");
													}
													break;
											}
											case 0x0e:
											{
												  SYN7318_Put_String(Stop_ASR_Buf,4); //停止语音识别
												  SYN7318_Get_String(Back,4);
												if(Back[3] == 0x41)
													{
														  LED_L =0;  // 开左灯
													SYN_TTS("左侧提示灯已打开，等待下一步指令");
													}
													break;
											}
											case 0x0f:
											{
												  SYN7318_Put_String(Stop_ASR_Buf,4); //停止语音识别
												  SYN7318_Get_String(Back,4);
												if(Back[3] == 0x41)
													{
														  LED_R =0;  // 开右灯
													SYN_TTS("右侧提示灯已打开，等待下一步指令");
													}
													break;
											}
											case 0x10:
											{
												  SYN7318_Put_String(Stop_ASR_Buf,4); //停止语音识别
												  SYN7318_Get_String(Back,4);
												if(Back[3] == 0x41)
													{
														 LED_L =1;  // 关闭提示灯
														 LED_R =1;
													SYN_TTS("提示灯已关闭，等待下一步指令");
													}

													break;
											}
											case 0x11:
											{
												  SYN7318_Put_String(Stop_ASR_Buf,4); //停止语音识别
												  SYN7318_Get_String(Back,4);
												if(Back[3] == 0x41)
													{
														 BEEP =0;  //开蜂鸣器
													 SYN_TTS("蜂鸣器已打开，等待下一步指令");
														 delay_ms(1000);
														 BEEP =1;
													}
													break;
											}
											case 0x12:
											{
												  SYN7318_Put_String(Stop_ASR_Buf,4); //停止语音识别
												  SYN7318_Get_String(Back,4);
												if(Back[3] == 0x41)
													{
														 BEEP =1;  //关蜂鸣器
													 SYN_TTS("蜂鸣器已关闭，等待下一步指令");
													}
													break;
											}
											case 0x13:
											{
												  SYN7318_Put_String(Stop_ASR_Buf,4); //停止语音识别
													SYN7318_Get_String(Back,4);
												  if(Back[3] == 0x41)// 显示车牌
													{
														 Transmition(CP_SHOW1,6);
														 delay_ms(500);
														 Transmition(CP_SHOW2,6);
														
													 SYN_TTS("车牌已显示，等待下一步指令");
													}
													break;
											}
											case 0x14:
											{
												  SYN7318_Put_String(Stop_ASR_Buf,4); //停止语音识别
												  SYN7318_Get_String(Back,4);
												  if(Back[3] == 0x41)// 打开隧道
													{
														 Transmition(H_SD,4);
														
													 SYN_TTS("隧道排风系统已打开，等待下一步指令");
													}
													break;
											}
											case 0x15:
											{
												  SYN7318_Put_String(Stop_ASR_Buf,4); //停止语音识别
												  SYN7318_Get_String(Back,4);
												if(Back[3] == 0x41)// 图片翻页上
													{
														 Transmition(H_S,4);
														
													 SYN_TTS("图片翻页完成，等待下一步指令");
													}
													break;
											}
											case 0x16:
											{
												  SYN7318_Put_String(Stop_ASR_Buf,4); //停止语音识别
												  SYN7318_Get_String(Back,4);
												if(Back[3] == 0x41)// 调光档位+1
													{
														 Transmition(H_1,4);
														
													 SYN_TTS("调[=tiao2]光档[=dang3]位已加1，等待下一步指令");
													}
													break;
											}
											case 0x17:
											{
												  SYN7318_Put_String(Stop_ASR_Buf,4); //停止语音识别
												  SYN7318_Get_String(Back,4);
												if(Back[3] == 0x41)// 语音播报
													{																																
													 SYN_TTS("欢迎使用北京百科融[=rong2]创语音智能小车");
													}
													break;
											}
											case 0x18:
											{
												  SYN7318_Put_String(Stop_ASR_Buf,4); //停止语音识别
												  SYN7318_Get_String(Back,4);
												if(Back[3] == 0x41)// 原地掉头
													{																																
													 SYN_TTS("开始原地掉头，请稍后");
														 Left_Test( 80); 
														 while(L_Flag);
														Left_Test( 80); 
														 while(L_Flag);
														SYN_TTS("原地掉头已完成，等待下一步指令");
														Ysn7813_flag = 0;
												  Ysn7813_flag_cs =0;
													xunji();
													zuozhuan();
														xunji();
														zuozhuan();
														xunji();
														qianjingMP(10);
														xunji();
														qianjingMP(10);
													}
													break;
											}
											case 0x19:
											{
												  SYN7318_Put_String(Stop_ASR_Buf,4); //停止语音识别
													SYN_TTS("好的，结束本次语音控制");
												  Ysn7813_flag = 0;
												  Ysn7813_flag_cs =0;
													break;
											}
											case 0x1A:  // 小车前进
											{
												  SYN7318_Put_String(Stop_ASR_Buf,4); //停止语音识别
												  SYN7318_Get_String(Back,4);
												  if(Back[3] == 0x41)
													{
													Go_Test( 80,10);
													SYN_TTS(" 小车前进已完成，等待下一步指令");
													}
													break;
											}
											case 0x1B:   //  小车后退
											{
												SYN7318_Put_String(Stop_ASR_Buf,4); //停止语音识别 
												  SYN7318_Get_String(Back,4);
												  if(Back[3] == 0x41)
													{
													
													  Back_Test(80,10);
													  SYN_TTS("小车后退已完成，等待下一步指令");	
													}
													break;
											}
											case 0x1C:   //  小车左转
											{
												  SYN7318_Put_String(Stop_ASR_Buf,4); //停止语音识别;
													SYN7318_Get_String(Back,4);
												  if(Back[3] == 0x41)
													{
												  
													Left_Test( 80);
														SYN_TTS("小车左转已完成，等待下一步指令");
													}
													break;
											}
											case 0x1D:  // 小车右转
											{
												  SYN7318_Put_String(Stop_ASR_Buf,4); //停止语音识别
													SYN7318_Get_String(Back,4);
												  if(Back[3] == 0x41)
													{
												   
													  Right_Test( 80);
														SYN_TTS("小车右转已完成，等待下一步指令");
													}
													break;
											}
											case 0x1E:  // 小车停止
											{
												  SYN7318_Put_String(Stop_ASR_Buf,4); //停止语音识别
													SYN7318_Get_String(Back,4);
												  if(Back[3] == 0x41)
													{
													
													  Stop_Test();
														SYN_TTS("已停车，等待下一步指令");
													}
													break;
											}
											case 0x1F:   //  小车循迹
											{
												  SYN7318_Put_String(Stop_ASR_Buf,4); //停止语音识别
												  SYN7318_Get_String(Back,4);
													if(Back[3] == 0x41)
													{
													  
													  Track_Test( 50);
														SYN_TTS("小车寻迹已完成，等待下一步指令");
													}
													break;
											}	
				  case 0x20:   //  向右转弯
											{
												  SYN7318_Put_String(Stop_ASR_Buf,4); //停止语音识别
												  SYN7318_Get_String(Back,4);
													if(Back[3] == 0x41)
													{
													  
													  Right_Test( 80);
														SYN_TTS("右转弯已完成，等待下一步指令");
													}
													break;
											}														
				  case 0x21:   //  禁止右转
											{
												  SYN7318_Put_String(Stop_ASR_Buf,4); //停止语音识别
												  SYN7318_Get_String(Back,4);
													if(Back[3] == 0x41)
													{
													  
													  Left_Test( 80);
														SYN_TTS("禁止右转，执行左转弯完成，等待下一步指令");
													}
													break;
												}
				  case 0x22:   //  向左转弯
											{
												  SYN7318_Put_String(Stop_ASR_Buf,4); //停止语音识别
												  SYN7318_Get_String(Back,4);
													if(Back[3] == 0x41)
													{
													  
													  Left_Test( 80);
														SYN_TTS("左转弯已完成，等待下一步指令");
													}
													break;
											}														
				  case 0x23:   //  禁止右转
											{
												  SYN7318_Put_String(Stop_ASR_Buf,4); //停止语音识别
												  SYN7318_Get_String(Back,4);
													if(Back[3] == 0x41)
													{
													  
													  Left_Test( 80);
														SYN_TTS("禁止左转，执行右转弯完成，等待下一步指令");
													}
													break;													
											}
											default:
											{
												  SYN7318_Put_String(Stop_ASR_Buf,4); //停止语音识别
													SYN_TTS("对不起，我没听清");
													break;
											}
										}
				SYN7318_Put_String(Stop_ASR_Buf,4); //停止语音识别
										break;
									}
									case 0x02: //识别成功（无命令ID号）
									{
										SYN_TTS("对不起，我没听清");
										SYN7318_Put_String(Stop_ASR_Buf,4); //停止语音识别
										break;
									}
									case 0x03://用户静音超时
									{
										SYN_TTS("进入休眠，随时为您服务");
										Ysn7813_flag = 0;
										Ysn7813_flag_cs = 0;
										SYN7318_Put_String(Stop_ASR_Buf,4); //停止语音识别
										break;
									}
									case 0x04:
									{
										SYN_TTS("请安静，麻烦您再说一遍");
										SYN7318_Put_String(Stop_ASR_Buf,4); //停止语音识别
										break;
									}
									case 0x05:
									{
										SYN_TTS("对不起，请再说一遍");
										SYN7318_Put_String(Stop_ASR_Buf,4); //停止语音识别
										break;
									}
									case 0x06:
									{
										SYN_TTS("识别内部错误");
										SYN7318_Put_String(Stop_ASR_Buf,4); //停止语音识别
										break;
									}
									case 0x07:
									{
										SYN_TTS("对不起，请再说一遍");
										SYN7318_Put_String(Stop_ASR_Buf,4); //停止语音识别
										break;
									}
									default:
									{
										SYN_TTS("错误");
										SYN7318_Put_String(Stop_ASR_Buf,4); //停止语音识别
										break;
									}
							}
							}
						}
					}
					/*************************************************/
					SYN7318_Put_String(Stop_Wake_Up,4);//发送停止唤醒指令
				}
				else                          //唤醒内部错误
				{
					LED3 =~LED3;
				}
			}
		}	
		else 
		{
			Zigb_Rx_Buf[0]=0x55;
			Zigb_Rx_Buf[1]=0x06;
			Zigb_Rx_Buf[2]=0x10;
			Zigb_Rx_Buf[3]=0x01;
			Zigb_Rx_Buf[4]=0x00;
			Zigb_Rx_Buf[5]=0x00;
			Zigb_Rx_Buf[6]=0x11;
			Zigb_Rx_Buf[7]=0xBB;
			Send_ZigbeeData_To_Fifo(Zigb_Rx_Buf,8);
			SYN7318_Get_String(Back,4);   //接收反馈信息
		}
		}
		
	}
}
