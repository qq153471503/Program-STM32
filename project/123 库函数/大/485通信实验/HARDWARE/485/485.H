#ifndef _485_H
#define _485_H

#include "sys.h"

extern u8 RS485_RX_BUF[64];	/*接收缓存数组*/
extern u8 RS485_RX_CNT;		/*接收数据字节多少计数*/

#define RS485_TX_EN PGout(9)	/*RS485模式设置		1发送	0接收*/

#define EN_USART2_RX 1		/*串口2 接收使能  1接收  0禁止*/

void RS485_Send_Data(u8 *buf,u8 length);
void RS485_Receive_Data(u8 *buf,u8 *length);
void RS485_Init(u32 bound);
#endif
