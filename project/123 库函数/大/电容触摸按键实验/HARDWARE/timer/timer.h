#ifndef _TIMER_H
#define _TIMER_H

#include "sys.h"


void TIM5_CH2_Cap_Init(u16 arr,u16 psc);
u8 TPAD_Init(u8 psc);
u16 TPAD_Get_Val(void);
void TPAD_Reset(void);
u16 TPAD_Get_Max_Val(u8 n);
u8 TPAD_Scan(u8 mode);


#endif
