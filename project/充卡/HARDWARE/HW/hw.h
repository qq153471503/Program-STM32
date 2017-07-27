#ifndef __HW_H
#define __HW_H	 
#include "sys.h"

#define RI_TXD PEout(1) // PE1

void HW_Init(void);
void Transmition(u8 *s,int n);
	 				    
#endif
