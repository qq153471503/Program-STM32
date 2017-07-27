#include "usart.h"
#include "can.h"


//注意：设置顺序：CAN单元设置->波特率->筛选器 
//若没有按照此顺序，CAN在回环模式下，接收不到数据！！！！！

u8 CAN_Mode_Init(u8 tsjw,u8 tbs1,u8 tbs2,u16 brp,u8 mode)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	CAN_InitTypeDef CAN_InitStructure;
	CAN_FilterInitTypeDef CAN_FilterInitStructure;
#if CAN_RX0_INT_ENABLE
	NVIC_InitTypeDef NVIC_InitStructure;
#endif
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;				//CAN_TX
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;				//CAN_RX
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOA,&GPIO_InitStructure);	
		
	//CAN单元设置
	CAN_InitStructure.CAN_ABOM = DISABLE;		//软件自动离线管理
	CAN_InitStructure.CAN_AWUM = DISABLE;		//睡眠模式通过软件唤醒(清除CAN->MCR的SLEEP位)
	CAN_InitStructure.CAN_NART = ENABLE;		//禁致报文自动传送
	CAN_InitStructure.CAN_RFLM = DISABLE;		//报文不锁定，新的覆盖旧的
	CAN_InitStructure.CAN_TTCM = DISABLE;		//非时间触发通信模式
	CAN_InitStructure.CAN_TXFP = DISABLE;		//优先级由报文标识符决定
	CAN_InitStructure.CAN_Mode = mode;			//模式设置： mode:0,普通模式;1,回环模式; 
	
	//设置波特率
	CAN_InitStructure.CAN_BS1 = tbs1;
	CAN_InitStructure.CAN_BS2 = tbs2;
	CAN_InitStructure.CAN_SJW = tsjw;
	CAN_InitStructure.CAN_Prescaler = brp;
	CAN_Init(CAN1,&CAN_InitStructure);

	CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;					//激活过滤器
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_FilterFIFO0;		//过滤器0关联到FIFO0
	CAN_FilterInitStructure.CAN_FilterIdHigh = 0x0000;
	CAN_FilterInitStructure.CAN_FilterIdLow = 0x0000;
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh = 0x0000;
	CAN_FilterInitStructure.CAN_FilterMaskIdLow = 0x0000;
	CAN_FilterInitStructure.CAN_FilterNumber = 0;							//过滤器0
	CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit;
	CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdMask;			//屏蔽位模式
	CAN_FilterInit(&CAN_FilterInitStructure);
	
#if CAN_RX0_INT_ENABLE
	CAN_ITConfig(CAN1,CAN_IT_FMP0,ENABLE);	//FIFO0消息挂号中断允许.		
	NVIC_InitStructure.NVIC_IRQChannel = USB_LP_CAN1_RX0_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
#endif
	return 0;
}
#if CAN_RX0_INT_ENABLE
void USB_LP_CAN1_IRQHandler(void)
{
	CanRxMsg RxMessage;
	unsigned char i = 0;
	CAN_Receive(CAN1,0,&RxMessage);
	for(i=0; i<8; i++)
	printf("rxbuf[%d]:%d\r\n",i,RxMessage.Data[i]);
}
#endif

//发送消息
u8 CAN_Send_Msg(u8 *msg,u8 len)
{
	u8 mbox;
	u16 i = 0;
	CanTxMsg TxMessage;
	TxMessage.StdId = 0x12;					//设置为标准标识符
	TxMessage.ExtId = 0x12;					//设置扩展标识符
	TxMessage.IDE = CAN_Id_Standard;		//设置为标准帧
	TxMessage.RTR = CAN_RTR_DATA;			//设置为数据帧
	TxMessage.DLC = len;					//数据长度
	for(i=0; i<len; i++)
		TxMessage.Data[i] = msg[i];
	mbox = CAN_Transmit(CAN1,&TxMessage);
	i = 0;
	while((CAN_TransmitStatus(CAN1,mbox) == CAN_TxStatus_Failed)&&(i<0xfff))	//略加延迟等待发送结束
		i++;
	if(i >= 0xfff)	//延时完毕还没有发送完成
		return 1;	//返回1 表示发送失败
	return 0;
}
//接收消息
u8 CAN_Receive_Msg(u8 *buf)
{
	u32 i;
	CanRxMsg RxMessage;
	if(CAN_MessagePending(CAN1,CAN_FIFO0) == 0)		//该函数返回值为挂号的消息数量
		return 0;							//没有被挂起的消息 说明没有接收到数据 直接返回退出
	CAN_Receive(CAN1,CAN_FIFO0,&RxMessage);
	for(i=0; i<8; i++)
		buf[i] = RxMessage.Data[i];
	return RxMessage.DLC;			//DLC用来设定帧长度 取值0-0x8
}

