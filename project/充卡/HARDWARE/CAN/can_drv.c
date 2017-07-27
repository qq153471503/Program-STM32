
#include <string.h>
#include "stm32f10x.h" 
#include "delay_drv.h"
#include "fifo_drv.h"

#define __CAN_DRV_C__
#include "can_drv.h"

#define GPIO_CAN GPIOA
#define CANx CAN1


static CanTxMsg TxMessage;
static CanRxMsg RxMessage;

uint8_t Ccp_RxFlag;

#define CAN_BAUDRATE_MODE		0	//0: 1Mbps  1: 450Kbps
#define EN_CAN_RXIRQ			0//1 //1//0

#define CAN_REMAP_MODE			2  //1//2

void CanDrv_Hinit(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	CAN_InitTypeDef        CAN_InitStructure;
	CAN_FilterInitTypeDef  CAN_FilterInitStructure;	
	
	/* GPIO clock enable */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
#if CAN_REMAP_MODE == 2
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);	
#endif
	
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);
	
	/* Configure CAN pin: RX */
#if CAN_REMAP_MODE == 0
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIO_CAN, &GPIO_InitStructure);
	
	/* Configure CAN pin: TX */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIO_CAN, &GPIO_InitStructure);
	
#elif CAN_REMAP_MODE == 2
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	
	/* Configure CAN pin: TX */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	
#endif
	
	/* Configure CAN pin: S */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);	
	CanDrv_HSpeedMode(0);	
	
#if CAN_REMAP_MODE == 0
	//GPIO_PinRemapConfig(GPIO_Remapping_CAN , ENABLE);
#elif CAN_REMAP_MODE == 1
	GPIO_PinRemapConfig(GPIO_Remap1_CAN1 , ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);
#elif CAN_REMAP_MODE == 2
	GPIO_PinRemapConfig(GPIO_Remap2_CAN1 , ENABLE);
	//GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);
	//GPIO_PinRemapConfig(GPIO_Remap2_CAN1 | GPIO_Remap_SWJ_JTAGDisable, ENABLE);

#endif
	
	/* CANx Periph clock enable */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);

	
	/* CAN register init */
	CAN_DeInit(CANx);
	CAN_StructInit(&CAN_InitStructure);
	
	/* CAN cell init */
	CAN_InitStructure.CAN_TTCM = DISABLE;
	CAN_InitStructure.CAN_ABOM = ENABLE;//DISABLE;
	CAN_InitStructure.CAN_AWUM = DISABLE;
	CAN_InitStructure.CAN_NART = ENABLE;	//DISABLE;
	CAN_InitStructure.CAN_RFLM = DISABLE;
	CAN_InitStructure.CAN_TXFP = ENABLE;//DISABLE;
	CAN_InitStructure.CAN_Mode = CAN_Mode_Normal;//CAN_Mode_LoopBack;//CAN_Mode_Normal;
	
	/* CAN Baudrate = 1MBps*/
#if CAN_BAUDRATE_MODE == 0
	CAN_InitStructure.CAN_SJW = CAN_SJW_1tq;
	CAN_InitStructure.CAN_BS1 = CAN_BS1_3tq;
	CAN_InitStructure.CAN_BS2 = CAN_BS2_5tq;
#else //CAN_BAUDRATE_MODE == 1
	CAN_InitStructure.CAN_SJW = CAN_SJW_1tq;
	CAN_InitStructure.CAN_BS1 = CAN_BS1_8tq;
	CAN_InitStructure.CAN_BS2 = CAN_BS2_7tq;
#endif
	CAN_InitStructure.CAN_Prescaler = 5;//4;
	CAN_Init(CANx, &CAN_InitStructure);
	
	/* CAN filter init */
	CAN_FilterInitStructure.CAN_FilterNumber = 0;
	CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdMask;
	CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit;
	CAN_FilterInitStructure.CAN_FilterIdHigh = 0x0000;
	CAN_FilterInitStructure.CAN_FilterIdLow = 0x0000;
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh = 0x0000;
	CAN_FilterInitStructure.CAN_FilterMaskIdLow = 0x0000;
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment = 0;
	CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;
	CAN_FilterInit(&CAN_FilterInitStructure);
	
	/* Transmit */
	TxMessage.StdId = 0x12;//0x321;
	TxMessage.ExtId = 0x12;//0x01;
	TxMessage.RTR = CAN_RTR_DATA;
	TxMessage.IDE = CAN_ID_STD;
	TxMessage.DLC = 1;	
	
	Ccp_RxFlag = 0;
	CAN_ITConfig(CAN1, CAN_IT_FMP0, DISABLE);	
	
}




void CanDrv_Sinit(void)
{
#if EN_CAN_RXIRQ
	CAN_ITConfig(CAN1, CAN_IT_FMP0, ENABLE);
#endif
	CanDrv_HSpeedMode(0);	
}

void CanDrv_HSpeedMode(uint8_t f)
{
	if(f == 0)
		GPIO_ResetBits(GPIOA,GPIO_Pin_15);
	else
		GPIO_SetBits(GPIOA,GPIO_Pin_15);	
}


extern void CanP_CanRx_Irq(void);

#if 1
void USB_LP_CAN1_RX0_IRQHandler(void)
{
	//CAN_Receive(CAN1, CAN_FIFO0, &RxMessage);
#if 0
	if ((RxMessage.StdId == 0x321)&&(RxMessage.IDE == CAN_ID_STD)&&(RxMessage.DLC == 1))
	{
		LED_Display(RxMessage.Data[0]);
		//Key_Pressed_Number = RxMessage.Data[0];
	}
#endif
	//if(RxMessage.StdId == 0x12)	//0x321)
	{
		Ccp_RxFlag = 1;
		CanP_CanRx_Irq();
	}
}
#endif


//______________________________________________________________________________

uint8_t CanDrv_TxEmptyCheck(void)
{
	return (CANx->TSR&CAN_TSR_TME) ? 1:0;
}

uint8_t CanDrv_TxData(uint8_t *tbuf,uint8_t len,uint32_t sid,uint32_t eid,uint8_t *tmbox)
{
	uint8_t mbox,Rt = 0;
	//uint16_t i;
	TxMessage.StdId = sid;
	TxMessage.ExtId = eid;
	TxMessage.IDE = CAN_ID_STD;//CAN_ID_STD;
	TxMessage.RTR = CAN_RTR_DATA;
	TxMessage.DLC = len;	
	memcpy(TxMessage.Data,tbuf,len);	
	mbox = CAN_Transmit(CAN1, &TxMessage);
	if(mbox != CAN_TxStatus_NoMailBox)
	{
		Rt = 1;
		//for(i = 0;i<0x0fff;i++)
			if(CAN_TransmitStatus(CAN1, mbox) == CAN_TxStatus_Ok )
			{
				Rt = 2;
		//		break;
			}
	}
	if((Rt >= 1)&&(tmbox != NULL))
		*tmbox = mbox;
		
	return Rt;		
}

uint8_t CanDrv_TxStatus(uint8_t TMbox)
{
	return CAN_TransmitStatus(CAN1, TMbox);
}

uint8_t CanDrv_TxDelay(uint8_t tmbox,uint32_t step,uint32_t m)
{
	uint8_t Rt = 0;
	if(tmbox < 3)
	while(m--)
	{
		if(CAN_TransmitStatus(CAN1, tmbox) == CAN_TxStatus_Ok )
		{
			Rt = 1;
			break;
		}
		sleep(step);
	}
	return Rt;
}


void CanDrv_WhaitTxEmpty(void)
{
	while(CanDrv_TxEmptyCheck() == 0)
		;
}

//______________________________________________________________________________

uint8_t CanDrv_RxCheck(void)
{
	return CAN_MessagePending(CANx, CAN_FIFO0);
}

uint8_t CanDrv_RxData(uint8_t *rbuf)
{
	uint8_t Rt = 0;
	if(rbuf != NULL)
	{
		Rt = CAN_MessagePending(CANx, CAN_FIFO0);
		if(Rt != 0)
		{
			CAN_Receive(CAN1, CAN_FIFO0, &RxMessage);
			Rt = RxMessage.DLC;	
			memcpy(rbuf,RxMessage.Data,Rt);
		}
	}
	return Rt;
}

uint8_t CanDrv_RxGetMeesage(CanRxMsg *p)
{
	uint8_t Rt = 0;
	if(p != NULL)
	{
		Rt = CAN_MessagePending(CANx, CAN_FIFO0);
		if(Rt != 0)
		{
			//CAN_Receive(CAN1, CAN_FIFO0, &RxMessage);
			//Rt = RxMessage.DLC;	
			//memcpy(rbuf,RxMessage.Data,Rt);
			//memcpy(p,&RxMessage,sizeof(RxMessage));
			CAN_Receive(CAN1, CAN_FIFO0, p);
		}
	}
	return Rt;
}

//______________________________________________________________________________

static uint16_t CanDrv_Fiter_Create16bit(uint32_t s,uint32_t e,uint8_t RTR ,uint8_t IDE)
{
	s &= 0x03ff;
	RTR = (RTR)? 0x10:0;
	IDE = (IDE)? 0x08:0;
	e = (IDE)? (e>>15)&0x03:0;
	return (uint16_t)(s<<5)|RTR|IDE|e;
}

static uint32_t CanDrv_Fiter_Create32bit(uint32_t s,uint32_t e,uint8_t RTR ,uint8_t IDE)
{
	s &= 0x03ff;
	RTR = (RTR)? 0x02:0;
	IDE = (IDE)? 0x04:0;
	e = (IDE)? (e&0x3ffff)<<3:0;
	return (uint32_t)(s<<13)|RTR|IDE|e;
}

void CanDrv_FiterUpdata(Can_Filter_Struct *p,uint8_t len)
{

	CAN_FilterInitTypeDef  CAN_FilterInitStructure;	
	
#define FXR1_LOW	CAN_FilterInitStructure.CAN_FilterIdLow
#define FXR1_HIG	CAN_FilterInitStructure.CAN_FilterIdHigh
#define FXR2_LOW	CAN_FilterInitStructure.CAN_FilterMaskIdLow
#define FXR2_HIG	CAN_FilterInitStructure.CAN_FilterMaskIdHigh

	uint8_t err_flag = 0;
	uint8_t c,FilterNumber;
	uint8_t ide;
	uint32_t t32;

	
	if((len == 0)||(p == NULL))
		err_flag = 1;
	
	if((Can_Filter_Flag & CAN_FLAG_MODEMASK)>3)
		err_flag = 1;
			
	ide = (Can_Filter_Flag & CAN_FLAG_IDE_EN)? 1:0;
	
	if(err_flag)
	{
		
		CAN_FilterInitStructure.CAN_FilterNumber = 0;
		CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdMask;
		CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit;
		CAN_FilterInitStructure.CAN_FilterIdHigh = 0x0000;
		CAN_FilterInitStructure.CAN_FilterIdLow = 0x0000;
		CAN_FilterInitStructure.CAN_FilterMaskIdHigh = 0x0000;
		CAN_FilterInitStructure.CAN_FilterMaskIdLow = 0x0000;
		CAN_FilterInitStructure.CAN_FilterFIFOAssignment = 0;
		CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;
		CAN_FilterInit(&CAN_FilterInitStructure);
	}
	else
	{
		

		CAN_FilterInitStructure.CAN_FilterFIFOAssignment = 0;
		CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;
		
		FilterNumber = 0;
		c = 0;
		while(1)
		{
			
			switch(Can_Filter_Flag & CAN_FLAG_MODEMASK)
			{
			case CAN_FLAG_MASK_16:
				CAN_FilterInitStructure.CAN_FilterMode	= CAN_FilterMode_IdMask;
				CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_16bit;
				//FXR1_LOW = CanDrv_Fiter_Create16bit(p[c].sid_id,p[c].eid_id,0,ide);
				//FXR1_HIG = CanDrv_Fiter_Create16bit(p[c].sid_mask,p[c].eid_mask,0,ide);
				FXR1_LOW = CanDrv_Fiter_Create16bit(p[c].sid_id,	0,0,ide);
				FXR2_LOW = CanDrv_Fiter_Create16bit(p[c].sid_mask,	0,0,ide);
				if(++c < len)
				{
					//FXR2_LOW = CanDrv_Fiter_Create16bit(p[c].sid_id,p[c].eid_id,0,ide);
					//FXR2_HIG = CanDrv_Fiter_Create16bit(p[c].sid_mask,p[c].eid_mask,0,ide);				
					FXR1_HIG = CanDrv_Fiter_Create16bit(p[c].sid_id,	0,0,ide);
					FXR2_HIG = CanDrv_Fiter_Create16bit(p[c].sid_mask,	0,0,ide);				
				}
				else
				{
					FXR2_LOW = 0;
					FXR2_HIG = 0;
				}
				break;
			case CAN_FLAG_LIST_16:
				CAN_FilterInitStructure.CAN_FilterMode	= CAN_FilterMode_IdList;
				CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_16bit;
				//FXR1_LOW = CanDrv_Fiter_Create16bit(p[c].sid_id,p[c].eid_id,0,ide);
				//FXR2_LOW = (++c < len)? CanDrv_Fiter_Create16bit(p[c].sid_id,p[c].eid_id,0,ide):0;
				//FXR1_HIG = (++c < len)? CanDrv_Fiter_Create16bit(p[c].sid_id,p[c].eid_id,0,ide):0;
				//FXR2_HIG = (++c < len)? CanDrv_Fiter_Create16bit(p[c].sid_id,p[c].eid_id,0,ide):0;
				FXR1_LOW = CanDrv_Fiter_Create16bit(p[c].sid_id,		0,0,ide);
				FXR2_LOW = (++c < len)? CanDrv_Fiter_Create16bit(p[c].sid_id,	0,0,ide):0;
				FXR1_HIG = (++c < len)? CanDrv_Fiter_Create16bit(p[c].sid_id,	0,0,ide):0;
				FXR2_HIG = (++c < len)? CanDrv_Fiter_Create16bit(p[c].sid_id,	0,0,ide):0;
				break;
			case CAN_FLAG_MASK_32:
				CAN_FilterInitStructure.CAN_FilterMode	= CAN_FilterMode_IdMask;
				CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit;
				//t32 = CanDrv_Fiter_Create32bit(p[c].sid_id,p[c].eid_id,0,ide);
				t32 = CanDrv_Fiter_Create32bit(p[c].sid_id,0,0,ide);
				FXR1_LOW = t32 & 0x0ffff;
				FXR1_HIG = t32 >> 16;
				//t32 = CanDrv_Fiter_Create32bit(p[c].sid_mask,p[c].eid_mask,0,ide);
				t32 = CanDrv_Fiter_Create32bit(p[c].sid_mask,0,0,ide);
				FXR2_LOW = t32 & 0x0ffff;
				FXR2_HIG = t32 >> 16;	
				break;
			case CAN_FLAG_LIST_32:
				CAN_FilterInitStructure.CAN_FilterMode	= CAN_FilterMode_IdList;
				CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit;
				//t32 = CanDrv_Fiter_Create32bit(p[c].sid_id,p[c].eid_id,0,ide);
				t32 = CanDrv_Fiter_Create32bit(p[c].sid_id,0,0,ide);
				FXR1_LOW = t32 & 0x0ffff;
				FXR1_HIG = t32 >> 16;
				//t32 = (++c < len)? CanDrv_Fiter_Create32bit(p[c].sid_mask,p[c].eid_mask,0,ide):0;
				t32 = (++c < len)? CanDrv_Fiter_Create32bit(p[c].sid_mask,0,0,ide):0;
				FXR2_LOW = t32 & 0x0ffff;
				FXR2_HIG = t32 >> 16;	
				break;
			default:
				FXR1_LOW = 0;
				FXR1_HIG = 0;
				FXR2_LOW = 0;
				FXR2_HIG = 0;
				c = len;
				err_flag = 1;
			}
			
			CAN_FilterInitStructure.CAN_FilterNumber = FilterNumber;

			CAN_FilterInit(&CAN_FilterInitStructure);			
			
			if(c < len)
				c++;
			else
				break;
			
			if(++FilterNumber >= 13)
				break;
		}
		
		
		
	}
	
}




