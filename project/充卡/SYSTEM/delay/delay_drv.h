#include "stm32f10x.h"
#ifndef __DELAY_DRV_H__
#define __DELAY_DRV_H__

#ifdef __DELAY_DRV_C__
#define GLOBAL
#else
#define GLOBAL extern
#endif

GLOBAL uint32_t global_times;

GLOBAL void sleep(unsigned long c);

GLOBAL volatile u32 delay_ms_const;
GLOBAL void delay_ms(u32 c);
GLOBAL void delay_ms_set(u32 c);
GLOBAL  u32 delay_ms_getlost(void);
GLOBAL   u8 delay_ms_endcheck(void);

GLOBAL u32 gt_get(void);
GLOBAL u32 gt_get_sub(u32 c);
GLOBAL void delay_us(u32 c);
GLOBAL void delay_init(void);

GLOBAL void Delay_us_Init(void);
GLOBAL void delay_us(u32 c);
#undef GLOBAL
#endif //__DELAY_DRV_H__

