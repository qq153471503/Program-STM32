#ifndef _CAN_H_
#define _CAN_H_

#include "sys.h"

#define CAN_RX0_INT_ENABLE 0

u8 CAN_Mode_Init(u8 tswj,u8 tbs1,u8 tbs2,u16 brp,u8 mode);
u8 CAN_Send_Msg(u8 *msg,u8 len);
u8 CAN_Receive_Msg(u8 *buf);

#endif
