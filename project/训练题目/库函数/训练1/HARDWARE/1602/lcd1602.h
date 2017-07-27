#ifndef __LCD1602_H
#define __LCD1602_H	 
#include "stm32f10x.h"

#define LCD_RS_1 GPIOC->BSRR=GPIO_Pin_0 //PC.0--(LCD)RS 
#define LCD_RS_0 GPIOC->BRR =GPIO_Pin_0  

#define LCD_RW_1 GPIOC->BSRR=GPIO_Pin_1 //PC1.1--(LCD)RW 
#define LCD_RW_0 GPIOC->BRR =GPIO_Pin_1
  
#define LCD_EN_1 GPIOC->BSRR=GPIO_Pin_2 //PC2.2--(LCD)E 
#define LCD_EN_0 GPIOC->BRR =GPIO_Pin_2 
 
#define DATAOUT  GPIOD->ODR             //PD[0..7]--(LCD)D0~D7 

void LCD_Writecmd(u8 cmd); 
void LCD_Writedata(u8 dat);  
void LcdInit(void); 
void LcdWriteChar(u8 cmd,u8 dat);
#endif


