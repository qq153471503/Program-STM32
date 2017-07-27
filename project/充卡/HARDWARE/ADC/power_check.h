
#ifndef __POWER_CHECK_H__
#define __POWER_CHECK_H__

#ifdef __POWER_CHECK_C__
#define GLOBAL
#else
#define GLOBAL extern
#endif

#include "sys.h"
void Electricity_Init(void);
void Parameter_Init(void) ;
u16  Get_Electricity( u8 ch ,u8 times );
void Power_Check(void);

GLOBAL u8 Electric_Buf[2] ;


#undef GLOBAL

#endif //__POWER_CHECK_H__


