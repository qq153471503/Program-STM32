#ifndef _DHT11_H
#define _DHT11_H

#include"sys.h"
#include"delay.h"

/*��dht11���ݿ����ó�����*/
#define DHT11_IO_IN()  {GPIOG->CRH&=0XFFFF0FFF;GPIOG->CRH|=8<<12;}
/*��dht11���ݿ����ó����*/
#define DHT11_IO_OUT() {GPIOG->CRH&=0XFFFF0FFF;GPIOG->CRH|=3<<12;}

#define dhtin PGin(11)
#define dhtout PGout(11)

/*�����ⲿ���� ���ô�ͷ�ļ�������ȫ����ʹ����Щ����
�¶ȸ�λ
�¶ȵ�λ
ʪ�ȸ�λ
ʪ�ȵ�λ  */
extern u8 temph,templ,humih,humil;

void Start(void);				  //��ʼ
u8 ReceiveByte(void);			  //����һ���ֽ�
void ReceiveData(void);			  //����ȫ������
void Dht11IoInit(void); 		  //dht11���ݿڳ�ʼ��

#endif

