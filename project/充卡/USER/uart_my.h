#ifndef __UART_MY_H
#define __UART_MY_H
#include <stm32f10x.h>


#ifdef __UART_MY_C__
#define GLOBAL
#else
#define GLOBAL extern
#endif

GLOBAL void  USART_WIFI_JX( u8 *buf );	// ���ڴ����е������
GLOBAL void Normal_data(void);			//  ������������8�ֽ�
GLOBAL void Abnormal_data(void);	 // ����1���ݽ����쳣
GLOBAL void send_data_zigbee( u8 *s ,u8 len);	 // �����ݷ���zigbee
GLOBAL void send_data_wifi( u8 *s ,u8 len);	 // �����ݷ���wifi

#undef GLOBAL
#endif

