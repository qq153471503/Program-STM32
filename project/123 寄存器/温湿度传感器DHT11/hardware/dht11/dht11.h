#ifndef _DHT11_H
#define _DHT11_H

#include"sys.h"
#include"delay.h"

/*将dht11数据口设置成输入*/
#define DHT11_IO_IN()  {GPIOG->CRH&=0XFFFF0FFF;GPIOG->CRH|=8<<12;}
/*将dht11数据口设置成输出*/
#define DHT11_IO_OUT() {GPIOG->CRH&=0XFFFF0FFF;GPIOG->CRH|=3<<12;}

#define dhtin PGin(11)
#define dhtout PGout(11)

/*声明外部变量 调用次头文件可以在全局内使用这些变量
温度高位
温度低位
湿度高位
湿度低位  */
extern u8 temph,templ,humih,humil;

void Start(void);				  //开始
u8 ReceiveByte(void);			  //接收一个字节
void ReceiveData(void);			  //接收全部数据
void Dht11IoInit(void); 		  //dht11数据口初始化

#endif

