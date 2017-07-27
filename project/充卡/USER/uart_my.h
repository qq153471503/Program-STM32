#ifndef __UART_MY_H
#define __UART_MY_H
#include <stm32f10x.h>


#ifdef __UART_MY_C__
#define GLOBAL
#else
#define GLOBAL extern
#endif

GLOBAL void  USART_WIFI_JX( u8 *buf );	// 串口处理机械臂数据
GLOBAL void Normal_data(void);			//  数据正常接收8字节
GLOBAL void Abnormal_data(void);	 // 串口1数据接收异常
GLOBAL void send_data_zigbee( u8 *s ,u8 len);	 // 将数据发给zigbee
GLOBAL void send_data_wifi( u8 *s ,u8 len);	 // 将数据发给wifi

#undef GLOBAL
#endif

