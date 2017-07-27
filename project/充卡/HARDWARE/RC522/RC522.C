#include "sys.h"
#include "rc522.h"
#include "delay.h"
#include "string.h" 
#include "test.h"
#include "canp_hostcom.h"
#include "bz.h"
#include "djqd.h"
#include "data_channel.h"
#include "uart_my.h"
#define MAXRLEN 18  

#define MF522_RST   PFout(1)

                    // 块地址
#define  card_key    (card_addr/4)*4+3	    // 块地址对应密钥块地址
#define  DATA_LEN    16                     // 定义数据字节长度
unsigned char status1;
u8 Kahao_flag=0;
u8 Read_flag=0;
u8 Write_flag=0;
unsigned char CT[2];//卡类型
unsigned char SN[4]; //卡号
unsigned char upKahao[16]={1,2,3,4,5,6,7,8,9,0,1,2,3,4,5,6};
unsigned char READ_RFID[16]={1,2,3,4,5,6,7,8,9,7,8,6,5,4,1,2};			//存放RFID 
u8 KEY_A[6]={0x19,0x9f,0x19,0x21,0x85,0x38};   	// A密钥
u8 KEY_B[6]={0xff,0xff,0xff,0xff,0xff,0xff};	// B密钥
unsigned char WRITE_RFID[DATA_LEN]={0x10,0x27,0x00,0x00,0xEF,0xD8,0xFF,0xFF,0x10,0x27,0x00,0x00,0x19,0xE6,0x19,0xE6};
//unsigned char RFID1[16]={0x01,0x02,0x03,0x04,0x05,0x06,0x08,0x77,0x8f,0x00,0xff,0xff,0xff,0xff,0xff,0xff};
u8 contrast_data(u8 *s1 ,u8 *s2, u8 len);
u8 Rc522(u8 card_addr,u8 MODE);
u8 Cishu=0;

void delay_ns(u32 ns)
{
  u32 i;
  for(i=0;i<ns;i++)
  {
    __nop();
    __nop();
    __nop();
  }
}

//初始化IO 串口1 
//bound:波特率
void RC522_Uart_init(u32 bound)
{
//    //GPIO端口设置
//	GPIO_InitTypeDef GPIO_InitStructure;
//	USART_InitTypeDef USART_InitStructure;
//	NVIC_InitTypeDef NVIC_InitStructure;
//	
//	//RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
//	//RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE);
//	//GPIO_PinRemapConfig(RCC_APB1Periph_USART3, ENABLE); 
// 	//USART_DeInit(USART3);  //复位串口3
//	USART_DeInit(UART4);  //复位串口4
//	//USART4_TX   PC.10
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; 
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
//	GPIO_Init(GPIOC, &GPIO_InitStructure); //初始化PC.10

//	//USART4_RX	  PC.11
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
//	GPIO_Init(GPIOC, &GPIO_InitStructure);  //初始化PC11


//	//USART 初始化设置

//	USART_InitStructure.USART_BaudRate = bound;//一般设置为9600;
//	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
//	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
//	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
//	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
//	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式

//	USART_Init(UART4, &USART_InitStructure); //初始化串口
//	//#if EN_USART4_RX		  //如果使能了接收  
//	//Usart4 NVIC 配置
////	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
////	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//抢占优先级3
////	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//子优先级3
////	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
////	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器

////	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//开启中断
//	//#endif
//	USART_Cmd(UART4, ENABLE);                    //使能串口 
//	//USART_ClearFlag(USART3 , USART_FLAG_TC);
//	//USART_GetFlagStatus(UART4,USART_FLAG_TC);
//	//USART_ClearFlag(USART1, USART_FLAG_TC);

   //GPIO端口设置
    GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA, ENABLE);	//使能USART1，GPIOA时钟
 	USART_DeInit(USART1);  //复位串口1
	 //USART1_TX   PA.9
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //PA.9
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
    GPIO_Init(GPIOA, &GPIO_InitStructure); //初始化PA9
   
    //USART1_RX	  PA.10
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
    GPIO_Init(GPIOA, &GPIO_InitStructure);  //初始化PA10

   //USART 初始化设置
	USART_InitStructure.USART_BaudRate = bound;//一般设置为9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式

    USART_Init(USART1, &USART_InitStructure); //初始化串口
#if EN_USART1_RX		  //如果使能了接收  
   //Usart1 NVIC 配置
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
   
    //USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//开启中断
#endif
    USART_Cmd(USART1, ENABLE);                    //使能串口 
	USART_GetFlagStatus(USART1,USART_FLAG_TC);
}

void InitRc522(void)
{
  PcdReset();
  PcdAntennaOff();
  delay_ms(2);  
  PcdAntennaOn();
  M500PcdConfigISOType( 'A' );
}

void Reset_RC522(void)
{
  PcdReset();
  PcdAntennaOff();
  delay_ms(2);  
  PcdAntennaOn();
}   
/////////////////////////////////////////////////////////////////////
//功    能：寻卡
//参数说明: req_code[IN]:寻卡方式
//                0x52 = 寻感应区内所有符合14443A标准的卡
//                0x26 = 寻未进入休眠状态的卡
//          	  pTagType[OUT]：卡片类型代码
//                0x4400 = Mifare_UltraLight
//                0x0400 = Mifare_One(S50)
//                0x0200 = Mifare_One(S70)
//                0x0800 = Mifare_Pro(X)
//                0x4403 = Mifare_DESFire
//返    回: 成功返回MI_OK
/////////////////////////////////////////////////////////////////////
char PcdRequest(unsigned char req_code,unsigned char *pTagType)
{
   char status;  
   unsigned int  unLen;
   unsigned char ucComMF522Buf[MAXRLEN]; 
//  unsigned char xTest ;
   ClearBitMask(Status2Reg,0x08);
   WriteRawRC(BitFramingReg,0x07);

//  xTest = ReadRawRC(BitFramingReg);
//  if(xTest == 0x07 )
 //   { LED_GREEN  =0 ;}
 // else {LED_GREEN =1 ;while(1){}}
   SetBitMask(TxControlReg,0x03);
 
   ucComMF522Buf[0] = req_code;

   status = PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,1,ucComMF522Buf,&unLen);
//     if(status  == MI_OK )
//   { LED_GREEN  =0 ;}
//   else {LED_GREEN =1 ;}
   if ((status == MI_OK) && (unLen == 0x10))
   {    
       *pTagType     = ucComMF522Buf[0];
       *(pTagType+1) = ucComMF522Buf[1];
   }
   else
   {   status = MI_ERR;   }
   
   return status;
}

/////////////////////////////////////////////////////////////////////
//功    能：防冲撞
//参数说明: pSnr[OUT]:卡片序列号，4字节
//返    回: 成功返回MI_OK
/////////////////////////////////////////////////////////////////////  
char PcdAnticoll(unsigned char *pSnr)
{
    char status;
    unsigned char i,snr_check=0;
    unsigned int  unLen;
    unsigned char ucComMF522Buf[MAXRLEN]; 
    

    ClearBitMask(Status2Reg,0x08);
    WriteRawRC(BitFramingReg,0x00);
    ClearBitMask(CollReg,0x80);
 
    ucComMF522Buf[0] = PICC_ANTICOLL1;
    ucComMF522Buf[1] = 0x20;

    status = PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,2,ucComMF522Buf,&unLen);

    if (status == MI_OK)
    {
    	 for (i=0; i<4; i++)
         {   
             *(pSnr+i)  = ucComMF522Buf[i];
             snr_check ^= ucComMF522Buf[i];
         }
         if (snr_check != ucComMF522Buf[i])
         {   status = MI_ERR;    }
    }
    SetBitMask(CollReg,0x80);
    return status;
}

/////////////////////////////////////////////////////////////////////
//功    能：选定卡片
//参数说明: pSnr[IN]:卡片序列号，4字节
//返    回: 成功返回MI_OK
/////////////////////////////////////////////////////////////////////
char PcdSelect(unsigned char *pSnr)
{
    char status;
    unsigned char i;
    unsigned int  unLen;
    unsigned char ucComMF522Buf[MAXRLEN]; 
    
    ucComMF522Buf[0] = PICC_ANTICOLL1;
    ucComMF522Buf[1] = 0x70;
    ucComMF522Buf[6] = 0;
    for (i=0; i<4; i++)
    {
    	ucComMF522Buf[i+2] = *(pSnr+i);
    	ucComMF522Buf[6]  ^= *(pSnr+i);
    }
    CalulateCRC(ucComMF522Buf,7,&ucComMF522Buf[7]);
  
    ClearBitMask(Status2Reg,0x08);

    status = PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,9,ucComMF522Buf,&unLen);
    
    if ((status == MI_OK) && (unLen == 0x18))
    {   status = MI_OK;  }
    else
    {   status = MI_ERR;    }

    return status;
}

/////////////////////////////////////////////////////////////////////
//功    能：验证卡片密码
//参数说明: auth_mode[IN]: 密码验证模式
//                 0x60 = 验证A密钥
//                 0x61 = 验证B密钥 
//          addr[IN]：块地址
//          pKey[IN]：密码
//          pSnr[IN]：卡片序列号，4字节
//返    回: 成功返回MI_OK
/////////////////////////////////////////////////////////////////////               
char PcdAuthState(unsigned char auth_mode,unsigned char addr,unsigned char *pKey,unsigned char *pSnr)
{
    char status;
    unsigned int  unLen;
    unsigned char i,ucComMF522Buf[MAXRLEN]; 

    ucComMF522Buf[0] = auth_mode;
    ucComMF522Buf[1] = addr;
    for (i=0; i<6; i++)
    {    ucComMF522Buf[i+2] = *(pKey+i);   }
    for (i=0; i<6; i++)
    {    ucComMF522Buf[i+8] = *(pSnr+i);   }
 //   memcpy(&ucComMF522Buf[2], pKey, 6); 
 //   memcpy(&ucComMF522Buf[8], pSnr, 4); 
    
    status = PcdComMF522(PCD_AUTHENT,ucComMF522Buf,12,ucComMF522Buf,&unLen);
    if ((status != MI_OK) || (!(ReadRawRC(Status2Reg) & 0x08)))
    {   status = MI_ERR;   }
    return status;
}

/////////////////////////////////////////////////////////////////////
//功    能：读取M1卡一块数据
//参数说明: addr[IN]：块地址
//          pData[OUT]：读出的数据，16字节
//返    回: 成功返回MI_OK
///////////////////////////////////////////////////////////////////// 
char PcdRead(unsigned char addr,unsigned char *pData)
{
    char status;
    unsigned int  unLen;
    unsigned char i,ucComMF522Buf[MAXRLEN]; 

    ucComMF522Buf[0] = PICC_READ;
    ucComMF522Buf[1] = addr;
    CalulateCRC(ucComMF522Buf,2,&ucComMF522Buf[2]);
   
    status = PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,4,ucComMF522Buf,&unLen);
    if ((status == MI_OK) && (unLen == 0x90))
 //   {   memcpy(pData, ucComMF522Buf, 16);   }
    {
        for (i=0; i<16; i++)
        {    *(pData+i) = ucComMF522Buf[i];   }
    }
    else
    {   status = MI_ERR;   }
    
    return status;
}

/////////////////////////////////////////////////////////////////////
//功    能：写数据到M1卡一块
//参数说明: addr[IN]：块地址
//          pData[IN]：写入的数据，16字节
//返    回: 成功返回MI_OK
/////////////////////////////////////////////////////////////////////                  
char PcdWrite(unsigned char addr,unsigned char *pData)
{
    char status;
    unsigned int  unLen;
    unsigned char i,ucComMF522Buf[MAXRLEN]; 
    
    ucComMF522Buf[0] = PICC_WRITE;
    ucComMF522Buf[1] = addr;
    CalulateCRC(ucComMF522Buf,2,&ucComMF522Buf[2]);
 
    status = PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,4,ucComMF522Buf,&unLen);

    if ((status != MI_OK) || (unLen != 4) || ((ucComMF522Buf[0] & 0x0F) != 0x0A))
    {   status = MI_ERR;   }
        
    if (status == MI_OK)
    {
        //memcpy(ucComMF522Buf, pData, 16);
        for (i=0; i<16; i++)
        {    ucComMF522Buf[i] = *(pData+i);   }
        CalulateCRC(ucComMF522Buf,16,&ucComMF522Buf[16]);

        status = PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,18,ucComMF522Buf,&unLen);
        if ((status != MI_OK) || (unLen != 4) || ((ucComMF522Buf[0] & 0x0F) != 0x0A))
        {   status = MI_ERR;   }
    }
    
    return status;
}



/////////////////////////////////////////////////////////////////////
//功    能：命令卡片进入休眠状态
//返    回: 成功返回MI_OK
/////////////////////////////////////////////////////////////////////
char PcdHalt(void)
{
    char status;
    unsigned int  unLen;
    unsigned char ucComMF522Buf[MAXRLEN]; 

    ucComMF522Buf[0] = PICC_HALT;
    ucComMF522Buf[1] = 0;
    CalulateCRC(ucComMF522Buf,2,&ucComMF522Buf[2]);
 
    status = PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,4,ucComMF522Buf,&unLen);

    return MI_OK;
}

/////////////////////////////////////////////////////////////////////
//用MF522计算CRC16函数
/////////////////////////////////////////////////////////////////////
void CalulateCRC(unsigned char *pIndata,unsigned char len,unsigned char *pOutData)
{
    unsigned char i,n;
    ClearBitMask(DivIrqReg,0x04);
    WriteRawRC(CommandReg,PCD_IDLE);
    SetBitMask(FIFOLevelReg,0x80);
    for (i=0; i<len; i++)
    {   WriteRawRC(FIFODataReg, *(pIndata+i));   }
    WriteRawRC(CommandReg, PCD_CALCCRC);
    i = 0xFF;
    do 
    {
        n = ReadRawRC(DivIrqReg);
        i--;
    }
    while ((i!=0) && !(n&0x04));
    pOutData[0] = ReadRawRC(CRCResultRegL);
    pOutData[1] = ReadRawRC(CRCResultRegM);
}

/////////////////////////////////////////////////////////////////////
//功    能：复位RC522
//返    回: 成功返回MI_OK
/////////////////////////////////////////////////////////////////////
char PcdReset(void)
{
	/*
    MF522_RST=1;
    delay_ns(10);
    MF522_RST=0;
    delay_ns(10);
    MF522_RST=1;
    delay_ns(10);
	*/
    WriteRawRC(CommandReg,PCD_RESETPHASE);
	  delay_ns(10);
	
    WriteRawRC(ModeReg,0x3D);            //和Mifare卡通讯，CRC初始值0x6363
    WriteRawRC(TReloadRegL,30);           
    WriteRawRC(TReloadRegH,0);
    WriteRawRC(TModeReg,0x8D);
    WriteRawRC(TPrescalerReg,0x3E);
    WriteRawRC(TxAutoReg,0x40); 
	  
    return MI_OK;
}
//////////////////////////////////////////////////////////////////////
//设置RC522的工作方式 
//////////////////////////////////////////////////////////////////////
char M500PcdConfigISOType(unsigned char type)
{
   if (type == 'A')                     //ISO14443_A
   { 
		   
       ClearBitMask(Status2Reg,0x08);
       WriteRawRC(ModeReg,0x3D);//3F
       WriteRawRC(RxSelReg,0x86);//84
       WriteRawRC(RFCfgReg,0x7F);   //4F
   	   WriteRawRC(TReloadRegL,30);//tmoLength);// TReloadVal = 'h6a =tmoLength(dec) 
			 WriteRawRC(TReloadRegH,0);
       WriteRawRC(TModeReg,0x8D);
	     WriteRawRC(TPrescalerReg,0x3E);
			 delay_ms(10);
       PcdAntennaOn();
         WriteRawRC(SerialSpeedReg,0x7A);//	  设置波特率115200
		 
		    //WriteRawRC(CommandReg,0x00);//
		   
   }
   else{ return -1; }
   
   return MI_OK;
}
/*************************************************
Function:       Send_data
Description:
     write a byte to serial port
Parameter:
     ch            the byte to write
Return:
     None
**************************************************/
void Send_data(unsigned char ch)
{
	  /*
	  USART3->SR;
    USART_SendData(USART3,ch); 
		while(USART_GetFlagStatus(USART3,USART_FLAG_TC)!=SET);
	 */
//	  UART4->SR;
//    USART_SendData(UART4,ch); 
//		while(USART_GetFlagStatus(UART4,USART_FLAG_TC)!=SET);
	
	  USART1->SR;
      USART_SendData(USART1,ch); 
		while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);
}

/*************************************************
Function:       Rece_data
Description:
     get a byte from serial port
Parameter:
     ch            the byte to save the received byte
     WaitTime      the counter for polling, avoid endless loop.
Return:
     STATUS_SUCCESS         get a byte data successfully
     STATUS_IO_TIMEOUT      time out
**************************************************/
short Rece_data(unsigned char *ch, unsigned int WaitTime)
{
	/*
    unsigned int j;
    j=0;
  while(!(USART1->SR & USART_FLAG_RXNE))
	{
		j++;
		if(j>=WaitTime)          //avoid endless loop
			break;
	}
	if(j<WaitTime)
	{
		*ch = USART_ReceiveData(USART1);
		//RI=0;
		return STATUS_SUCCESS;
	}
	else
	    return STATUS_IO_TIMEOUT;
	*/
	
//	while(!(UART4->SR & USART_FLAG_RXNE));
//	*ch = (uint8_t)USART_ReceiveData(UART4);
//	return STATUS_SUCCESS;
	
//	while(!(USART3->SR & USART_FLAG_RXNE));
//	*ch = (uint8_t)USART_ReceiveData(USART3);
//	return STATUS_SUCCESS;
	while(!(USART1->SR & USART_FLAG_RXNE));
	*ch = (uint8_t)USART_ReceiveData(USART1);
	return STATUS_SUCCESS;
}
/////////////////////////////////////////////////////////////////////
//功    能：读RC632寄存器
//参数说明：Address[IN]:寄存器地址
//返    回：读出的值
/////////////////////////////////////////////////////////////////////
unsigned char ReadRawRC(unsigned char Address)
{
    unsigned char RegVal;
    short status;
    Address = (Address & 0x3f) | 0x80;   //code the first byte
    Send_data(Address);
    status = Rece_data(&RegVal, 10000);
    if(status != STATUS_SUCCESS)
        return 0xff;
    return RegVal;
}

/////////////////////////////////////////////////////////////////////
//功    能：写RC632寄存器
//参数说明：Address[IN]:寄存器地址
//          value[IN]:写入的值
/////////////////////////////////////////////////////////////////////
void WriteRawRC(unsigned char Address, unsigned char value)
{  
    unsigned char EchoByte;
    short status;
    Address &= 0x3f;   //code the first byte
    Send_data(Address);
    Send_data(value);
    status = Rece_data(&EchoByte, 10000);
}

/////////////////////////////////////////////////////////////////////
//功    能：置RC522寄存器位
//参数说明：reg[IN]:寄存器地址
//          mask[IN]:置位值
/////////////////////////////////////////////////////////////////////
void SetBitMask(unsigned char reg,unsigned char mask)  
{
    char tmp = 0x0;
    tmp = ReadRawRC(reg);
    WriteRawRC(reg,tmp | mask);  // set bit mask
}

/////////////////////////////////////////////////////////////////////
//功    能：清RC522寄存器位
//参数说明：reg[IN]:寄存器地址
//          mask[IN]:清位值
/////////////////////////////////////////////////////////////////////
void ClearBitMask(unsigned char reg,unsigned char mask)  
{
    char tmp = 0x0;
    tmp = ReadRawRC(reg);
    WriteRawRC(reg, tmp & ~mask);  // clear bit mask
} 

/////////////////////////////////////////////////////////////////////
//功    能：通过RC522和ISO14443卡通讯
//参数说明：Command[IN]:RC522命令字
//          pInData[IN]:通过RC522发送到卡片的数据
//          InLenByte[IN]:发送数据的字节长度
//          pOutData[OUT]:接收到的卡片返回数据
//          *pOutLenBit[OUT]:返回数据的位长度
/////////////////////////////////////////////////////////////////////
char PcdComMF522(unsigned char Command, 
                 unsigned char *pInData, 
                 unsigned char InLenByte,
                 unsigned char *pOutData, 
                 unsigned int  *pOutLenBit)
{
    char status = MI_ERR;
    unsigned char irqEn   = 0x00;
    unsigned char waitFor = 0x00;
    unsigned char lastBits;
    unsigned char n;
    unsigned int i;
    switch (Command)
    {
       case PCD_AUTHENT:
          irqEn   = 0x12;
          waitFor = 0x10;
          break;
       case PCD_TRANSCEIVE:
          irqEn   = 0x77;
          waitFor = 0x30;
          break;
       default:
         break;
    }
   
    WriteRawRC(ComIEnReg,irqEn|0x80);
    ClearBitMask(ComIrqReg,0x80);
    WriteRawRC(CommandReg,PCD_IDLE);
    SetBitMask(FIFOLevelReg,0x80);
    
    for (i=0; i<InLenByte; i++)
    {   WriteRawRC(FIFODataReg, pInData[i]);    }
    WriteRawRC(CommandReg, Command);
   
    
    if (Command == PCD_TRANSCEIVE)
    {    SetBitMask(BitFramingReg,0x80);  }
    
//    i = 600;//根据时钟频率调整，操作M1卡最大等待时间25ms
 i = 2000;
    do 
    {
         n = ReadRawRC(ComIrqReg);
         i--;
    }
    while ((i!=0) && !(n&0x01) && !(n&waitFor));
    ClearBitMask(BitFramingReg,0x80);
	      
    if (i!=0)
    {    
         if(!(ReadRawRC(ErrorReg)&0x1B))
         {
             status = MI_OK;
             if (n & irqEn & 0x01)
             {   status = MI_NOTAGERR;   }
             if (Command == PCD_TRANSCEIVE)
             {
               	n = ReadRawRC(FIFOLevelReg);
              	lastBits = ReadRawRC(ControlReg) & 0x07;
                if (lastBits)
                {   *pOutLenBit = (n-1)*8 + lastBits;   }
                else
                {   *pOutLenBit = n*8;   }
                if (n == 0)
                {   n = 1;    }
                if (n > MAXRLEN)
                {   n = MAXRLEN;   }
                for (i=0; i<n; i++)
                {   pOutData[i] = ReadRawRC(FIFODataReg);    }
            }
         }
         else
         {   status = MI_ERR;   }
        
   }
   

   SetBitMask(ControlReg,0x80);           // stop timer now
   WriteRawRC(CommandReg,PCD_IDLE); 
   return status;
}


/////////////////////////////////////////////////////////////////////
//开启天线  
//每次启动或关闭天险发射之间应至少有1ms的间隔
/////////////////////////////////////////////////////////////////////
void PcdAntennaOn()
{
    unsigned char i;
    i = ReadRawRC(TxControlReg);
    if (!(i & 0x03))
    {
        SetBitMask(TxControlReg, 0x03);
    }
}


/////////////////////////////////////////////////////////////////////
//关闭天线
/////////////////////////////////////////////////////////////////////
void PcdAntennaOff()
{
    ClearBitMask(TxControlReg, 0x03);
}

//等待卡离开
void WaitCardOff(void)
{
	unsigned char status, TagType[2];

	while(1)
	{
		status = PcdRequest(REQ_ALL, TagType);
		if(status)
		{
			status = PcdRequest(REQ_ALL, TagType);
			if(status)
			{
				status = PcdRequest(REQ_ALL, TagType);
				if(status)
				{
					return;
				}
			}
		}
		delay_ms(10);
	}
}
u8 Rc522(u8 card_addr,u8 MODE)
{
		u8 i;
	Cishu++;
//	if(((card_addr==card_key)&&(MODE))||(card_addr>0x3f))	 // 密钥区最好不要修改	 S50卡块地址0~63（0x3f）
//		  {
//		  	   while(1)
//			   {
//					delay_ms(500);
//					delay_ms(100);
//			   }
//		  }
//			
			status1 = PcdRequest(PICC_REQALL,CT);/*尋卡*/
			if(status1 == MI_OK)							//尋卡成功
			{
				 status1 = MI_ERR;
				 status1 = PcdAnticoll(SN);	/*防冲撞*/			 
			}

			if (status1 == MI_OK)						//防衝撞成功
			{
				status1 = MI_ERR;		
				status1 = PcdSelect(SN);
			}
			else
			{
			}

			if(status1 == MI_OK)//選卡成功
			{
			 status1 = MI_ERR;
			 status1 = PcdAuthState(KEYA,card_key,KEY_A,SN);	//验证A密钥	  一般只验证A
			}

		  if( MODE )	// 判读是否写入	
		  {
		  	  if(status1 == MI_OK)//驗證成功
			 {
			  status1 = MI_ERR;
			  status1 = PcdWrite(card_addr,WRITE_RFID);	  //写入数据
			 }
		  }	
			else if(card_addr==0)
			{
				if(status1 == MI_OK)//驗證成功
				{
					status1 = MI_ERR;
					status1 = PcdRead(card_addr,upKahao);	  //读取数据
					send_data_wifi(upKahao,16);
					Kahao_flag=1;
				}
			}	
			else
			{
				if(status1 == MI_OK)
				{
					status1 = MI_ERR;
					status1 = PcdRead(card_addr,READ_RFID);
					send_data_zigbee(READ_RFID,16);
					LED_L=0;LED_R=0;
					Read_flag=1;
					ka=0;
				}
			}
//		if(status1 == MI_OK)//讀卡成功
//			 {
//				status1 = MI_ERR;
//			  if( MODE )
//			  {
//			  	 if(contrast_data(WRITE_RFID,READ_RFID,DATA_LEN))  // 比对写入和读写的数据
//			    {
//						}
//						if(ka==2)
//						{
//							for(i=0;i<16;i++)
//							{
//								IDka2[i]=READ_RFID[i];
//							}
//							send_data_wifi(IDka2 , 16);	     
//						}	  
//						ka++;
//			 }
		 
	
}

u8 contrast_data(u8 *s1 ,u8 *s2, u8 len)
{
	  u8 i;
	  for(i=0;i<len;i++)
	  {
	  	  if(s1[i]!=s2[i])   return 0;
	  }

	  return 1;
}

