#ifndef __XJ_H
#define __XJ_H	 
#include "sys.h"
#include "djqd.h"
#include "test.h"

extern u8 gd;

void XJ_Init(void);//≥ı ºªØ	
void Track(void);


void Car_Run(unsigned char count);
void GO_MP(u16 mp);
void BACK_MP(u16 mp);
void RIGHT_MP(u16 mp);
void LEFT_MP(u16 mp);
	 				    
#endif

