#ifndef __hmc5883l_H
#define __hmc5883l_H
#include "sys.h" 
 
 
#define HMC_SCL_H (GPIOB->BSRR =GPIO_Pin_6 )
#define HMC_SCL_L (GPIOB->BRR =GPIO_Pin_6 )

#define HMC_SDA_H (GPIOB->BSRR =GPIO_Pin_7)
#define HMC_SDA_L (GPIOB->BRR =GPIO_Pin_7)

#define HMC_SDA (GPIOB->IDR & GPIO_Pin_7)

#define HMC5883_SDA_IN()  {GPIOB->CRL&=0X0FFFFFFF;GPIOB->CRL|=0X80000000;}

#define HMC5883L_OFFSET_X (9)
#define HMC5883L_OFFSET_Y (149)

#define HMC5883L_GAIN_Y 10403

extern u8 hmc5883l_buf[2];


void HMC_Delay(void);
void HMC_GPIO_Config(void);
u8 HMC_Start(void);
void HMC_I2C_Stop(void);
void HMC_I2C_Ack(void);
void HMC_I2C_NAck(void);
u8 HMC_I2C_WaitAck(void);
void HMC_I2C_SendByte(u8 SendByte);
u8 HMC_I2C_ReceiveByte(void);
u8 HMC_I2C_Write(u8 address,u8 data);
u8 HMC_I2C_Read(u8 address);

u8 HMC5883_Init(void); 
void HMC_MultipleRead(u8* pBuffer); 
void HMC_GetValue(s16* pBuffer);
//void HMC5883_GetAngle(void); 
double HMC5883_GetAngle(void);

#endif
















