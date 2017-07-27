

#include <stdint.h>
#include <string.h>
#include "stm32f10x.h" 
#include "my_lib.h"
#include "usart.h"
#include "delay_drv.h"
#include "can_drv.h"
#include "fifo_drv.h"
#include "xj.h"
#include "data_channel.h"

#define __CANP_HostCOM_C__
#include "canp_hostcom.h"

//uint8_t canrx_tbuf[8];
//uint8_t cantx_tbuf[8];
uint32_t CanP_RxFMI_Flag;
uint8_t ctbuf[8];
uint8_t Track_buf[2]={0,0};

#define CAN_CURR_MODE	4
/*
1	电机
2	寻迹
3	导航
4	寻迹+导航
5	主机
6	WIFI
6	ZIGBEE
8	显示
9	WIFI+ZIGBEE+显示
*/


Can_Filter_Struct Fileter_Tab[]=
{
	/*
	{CAN_ID_DISP	,CAN_ID_DISP	,0,0},//CAN_ID_HOST,CAN_ID_HOST},
	{CAN_ID_WIFI	,CAN_ID_WIFI	,0,0},//CAN_ID_HOST,CAN_ID_HOST},
	{CAN_ID_ZIGBEE	,CAN_ID_ZIGBEE	,0,0},
	{CAN_ID_MOTOR	,CAN_ID_MOTOR	,0,0},
	{CAN_ID_TRACK	,CAN_ID_TRACK	,0,0},
	{CAN_ID_NAVIG	,CAN_ID_NAVIG	,0,0},
	{CAN_ID_HOST	,CAN_ID_HOST	,0,0},
	
	{0x12,0x12,0,0},
	{0,0,0,0},
	*/
	{CAN_SID_HL(ID_DISP,	ID_ZERO),CAN_SID_HL(ID_ALL,ID_ZERO)},
	{CAN_SID_HL(ID_WIFI,	ID_ZERO),CAN_SID_HL(ID_ALL,ID_ZERO)},
	{CAN_SID_HL(ID_ZIGBEE,	ID_ZERO),CAN_SID_HL(ID_ALL,ID_ZERO)},
//	{CAN_SID_HL(ID_MOTOR,	ID_ZERO),CAN_SID_HL(ID_ALL,ID_ZERO)},
	{CAN_SID_HL(ID_TRACK,	ID_ZERO),CAN_SID_HL(ID_ALL,ID_ZERO)},
	{CAN_SID_HL(ID_NAVIG,	ID_ZERO),CAN_SID_HL(ID_ALL,ID_ZERO)},
	{CAN_SID_HL(ID_HOST,	ID_ZERO),CAN_SID_HL(ID_ALL,ID_ZERO)},
	
//	{0x12,0x12},//0,0},
//	
	{0,0,},
	
};

void CanP_Init(void)
{
	CanP_RxFMI_Flag = 0;
	CanDrv_Hinit();
	Can_Filter_Flag = CAN_FLAG_MASK_16;	//CAN_FLAG_LIST_16 ;//| CAN_FLAG_IDE_EN;
	//Can_Filter_Flag = CAN_FLAG_LIST_16;	//CAN_FLAG_LIST_16 ;//| CAN_FLAG_IDE_EN;
	CanDrv_FiterUpdata(Fileter_Tab,sizeof(Fileter_Tab)/sizeof(Fileter_Tab[0]));
	CanDrv_Sinit();
	
	CanP_FifoInit();
}

void CanP_test_tbuf_init(void)
{
	/*	
	static uint8_t c = 0;
	uint8_t i;
	for(i = 0;i<8;i++)
		cantx_tbuf[i] = c++;
	*/
}

void CanP_Check(void)
{
	static uint32_t canp_ct = 0;
	uint8_t rl;
	if(gt_get_sub(canp_ct) == 0)
	{
		canp_ct = gt_get() + 100;
		//CAN_Transmit(CAN1, &TxMessage);
		CanP_test_tbuf_init();
		//Can_Send_Msg(cantx_tbuf,8,0x12,0x12);
		//CanDrv_TxData(cantx_tbuf,8,0x40,0x50,_NULL);
		
		CanP_TestFifo();
	}
	//if(Ccp_RxFlag)
	rl =CanDrv_RxCheck();
	if(rl)
	{
		CanRxMsg rme;
		//rl = CanDrv_RxData(canrx_tbuf);
		CanDrv_RxGetMeesage(&rme);
		//Ccp_RxFlag = 0;
		//UD_AddStrint((char *)RxMessage.Data,1,RxMessage.DLC);
		//UD_AddStrint((char *)rme.Data,1,rme.DLC);
	}
	
}

//______________________________________________________________________________
Fifo_Drv_Struct Fifo_Info;
Fifo_Drv_Struct Fifo_WifiRx;
Fifo_Drv_Struct Fifo_WifiTx;
Fifo_Drv_Struct Fifo_ZigbRx;
Fifo_Drv_Struct Fifo_ZigbTx;

#define FIFOSIZE_INFO	50
#define FIFOSIZE_WIFIRX	50
#define FIFOSIZE_WIFITX	50
#define FIFOSIZE_ZIGBRX	50
#define FIFOSIZE_ZIGBTX	50

uint8_t FifoBuf_Info[FIFOSIZE_INFO];
uint8_t FifoBuf_WifiRx[FIFOSIZE_WIFIRX];
uint8_t FifoBuf_WifiTx[FIFOSIZE_WIFITX];
uint8_t FifoBuf_ZigbRx[FIFOSIZE_ZIGBRX];
uint8_t FifoBuf_ZigbTx[FIFOSIZE_ZIGBTX];


void CanP_FifoInitS(Fifo_Drv_Struct *p,uint32_t l,uint8_t *buf)
{
	p->ml = l;
	p->buf = buf;
	FifoDrv_Init(p,0);
}

void CanP_FifoInit(void)
{
	CanP_FifoInitS(&Fifo_Info,FIFOSIZE_INFO,FifoBuf_Info);
	CanP_FifoInitS(&Fifo_WifiRx,FIFOSIZE_WIFIRX,FifoBuf_WifiRx);
	CanP_FifoInitS(&Fifo_WifiTx,FIFOSIZE_WIFITX,FifoBuf_WifiTx);
	CanP_FifoInitS(&Fifo_ZigbRx,FIFOSIZE_ZIGBRX,FifoBuf_ZigbRx);
	CanP_FifoInitS(&Fifo_ZigbTx,FIFOSIZE_ZIGBTX,FifoBuf_ZigbTx);
	
}


CanRxMsg crm;
uint8_t crbuf[4][8];

void CanP_CanRx_Irq(void)
{
	CanDrv_RxGetMeesage(&crm);
	switch(crm.FMI)
	{
	case 0:			//disp
		FifoDrv_BufWrite(&Fifo_Info,crm.Data,crm.DLC);
		break;
	case 1:			//wifi rx
		FifoDrv_BufWrite(&Fifo_WifiRx,crm.Data,crm.DLC);
		break;
	case 2:			//zigbee rx
		FifoDrv_BufWrite(&Fifo_ZigbRx,crm.Data,crm.DLC);
		break;
	default:
		if((crm.FMI >= 3)&&(crm.FMI <= 6))
			memcpy(crbuf[crm.FMI-3],crm.Data,8);
		break;
	}
	
	if(crm.FMI <= 6)
		CanP_RxFMI_Flag |= bit_tab[crm.FMI];
}

void CanP_CanRx_Check(void)
{
	while(CanDrv_RxCheck())
	{
		CanP_CanRx_Irq();
	}
}



void CanP_Host_Main(void)
{
	uint8_t t8;
	uint8_t i;
	CanP_CanRx_Check();
	//if(CanP_RxFMI_Flag)
	while(CanP_RxFMI_Flag)
	{
		CanP_CanRx_Check();
		for(i = 0;i<=6;i++)
		{
			if(CanP_RxFMI_Flag & bit_tab[i])
			{
				switch(i)
				{
				case 0:
					while(FifoDrv_ReadOne(&Fifo_Info,&t8))
					//	DispCar_InfoDebug(&t8,1,1,0);
//					U1SendChar( t8);
					break;
				case 1:
					while(FifoDrv_ReadOne(&Fifo_WifiRx,&t8))
						
 						Wifi_data_Receive( t8);
					
					//	DispCar_InfoDebug(&t8,1,1,1);	
					break;
				case 2:
					while(FifoDrv_ReadOne(&Fifo_ZigbRx,&t8))
					//	DispCar_InfoDebug(&t8,1,1,2);		
              Zigbee_data_Receive( t8);		
//              U1SendChar( t8);					
					break;
//				case 3:
//				//	DispCar_UpMotor((int8_t *)crbuf[0]);
//					break;
				case 3:
				//	DispCar_UpTrack(crbuf[1][0],crbuf[1][1]);
				   Host_Receive_UpTrack(crbuf[0][0], crbuf[0][1]);  // 提取循迹数据
//				U1SendChar(crbuf[0][0]);
//				U1SendChar(crbuf[0][1]);
					break;
				case 5:
				//	DispCar_UpNavig(1,U8ToU16_Big(crbuf[2]));
					break;
				case 6:
					if(crbuf[3][0] == 0x01)
//						DispCar_UpPower(crbuf[3]+1);
					break;
				}
				CanP_RxFMI_Flag &= bit_and_tab[i];
			}			
		}
	}
}


void CanP_WaitTx(uint8_t *tmbox,uint32_t m)
{
		while(m--)
		{
			if(CanDrv_TxStatus(*tmbox) == CAN_TxStatus_Ok)
					break;
		}
	
}

void CanP_CanTx_Check(void)
{
	uint8_t tmbox,i,f = 1;
	while(f)
	{
		f = 0;
		i = FifoDrv_BufRead(&Fifo_Info,ctbuf,8);// 调试信息
	
	if(i)
	{
		CanDrv_WhaitTxEmpty();
		//CanDrv_TxData(ctbuf,i,CAN_SID_HL(ID_HOST,ID_TRACK),0,&tmbox);
		//CanDrv_TxData(ctbuf,i,CAN_SID_HL(ID_DISP,0),0,&tmbox);
		f = 1;
	}
	
	
	i = FifoDrv_BufRead(&Fifo_WifiTx,ctbuf,8); // wifi信息
	if(i)
	{
		CanDrv_WhaitTxEmpty();
// 		CanDrv_TxData(ctbuf,i,CAN_SID_HL(ID_DISP,ID_HOST),0,&tmbox);
		CanDrv_TxData(ctbuf,i,CAN_SID_HL(ID_WIFI,0),0,&tmbox);
	  //CanP_WaitTx(&tmbox,0x0ffff);
		f = 1;
	}
	
	i = FifoDrv_BufRead(&Fifo_ZigbTx,ctbuf,8);// Zigbee信息
	if(i)
	{
		CanDrv_WhaitTxEmpty();
// 		CanDrv_TxData(ctbuf,i,CAN_SID_HL(ID_DISP,ID_HOST),0,&tmbox);
		CanDrv_TxData(ctbuf,i,CAN_SID_HL(ID_ZIGBEE,0),0,&tmbox);		
	  //CanP_WaitTx(&tmbox,0x0ffff);
		f = 1;
	}	
}

}

void Send_Electric(u8 x1, u8 x2)  // 上传电量信息
{
    u8 txbuf[3];
	   txbuf[0] =0x01;
	   txbuf[1] = x1;
	   txbuf[2] = x2;
	  
	  CanDrv_WhaitTxEmpty();
		CanDrv_TxData(txbuf,3,CAN_SID_HL(ID_HOST,0),0,_NULL);
	
}

void Send_Debug_Info(u8 *s,u8 len)  // 调试信息
{
 	  
	  CanDrv_WhaitTxEmpty();
		CanDrv_TxData( s,len,CAN_SID_HL(ID_DISP,0),0,_NULL);
	
}

void Send_UpMotor( int x1, int x2) // 发送电机转速
{
     u8 txbuf[4];
	   txbuf[0] = x1;
	   txbuf[1] = x1;
	   txbuf[2] = x2;
	   txbuf[3] = x2;	

  	CanDrv_WhaitTxEmpty();
		CanDrv_TxData(txbuf,4,CAN_SID_HL(ID_MOTOR,0),0,_NULL);	

}


void CanP_TestFifo(void)
{
	static uint8_t c = '0';
	uint32_t i;
	uint8_t t;
	
	for(i = 0;i<10;i++)
	{
		if(FifoDrv_WriteOne(&Fifo_Info,c) == 0)
			break;
		if(++c > '9')
			c  = '0';
		//DispCar_InfoDebug(&c,1,1,0);
	}
	
	
	for(i = 0;i<7;i++)
	{
		if(FifoDrv_ReadOne(&Fifo_Info,&t) == 0)
			break;
		//UD_AddStrint((char *)&t,0,1);
//		DispCar_InfoDebug(&t,1,1,0);
	}
	
	
}  

void Host_Receive_UpTrack( u8 x1, u8 x2)  // 循迹数据接收处理函数
{
   Track_buf[0] = x1;
   Track_buf[1] = x2;	 
}

u16  Get_Host_UpTrack( u8 mode)  // 获取循迹数据
{
	if( mode == TRACK_ALL)   
	 return  (u16)((Track_buf[0] <<8)+ Track_buf[1] );
	else if(mode == TRACK_Q7)
		return  Track_buf[1] ;
	else if( mode == TRACK_H8)
		return Track_buf[0];
	
	return 0;

}

void Host_Set_UpTrack( u8 time)  // 设置循迹数据上传时间间隔
{
      u8 txbuf[2];
	   txbuf[0] = 0X02;  //命令关键字
	   txbuf[1] = time;
  	CanDrv_WhaitTxEmpty();
		CanDrv_TxData(txbuf,2,CAN_SID_HL(ID_TRACK,ID_HOST),0,_NULL);   

}

void Host_Open_UpTrack(u8 time)  //开启循迹数据上传
{
   
	Host_Set_UpTrack( time );
//	 Track_Usart2_Open();
	

}
//////////////////////////修改这个时间试一下。/***********************************************/
void Host_Close_UpTrack(void)   // 关闭循迹数据上传
{
	u8 i;
	for(i=0;i<3;i++)
	{
     Host_Set_UpTrack( 0 );   
		 delay_us(3000);
	}
//   Track_Usart2_Close();
}


void Send_ZigbeeData_To_Fifo( u8 *p ,u8 len)   // 将数据转发给Zigbee
{
	FifoDrv_BufWrite( &Fifo_ZigbTx , p , len);

}

void Send_WifiData_To_Fifo( u8 *p ,u8 len)   // 将数据转发给wifi
{
	FifoDrv_BufWrite( &Fifo_WifiTx , p , len);

}


