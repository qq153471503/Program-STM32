#ifndef _KEY_H_
#define _keY_H_

#include"sys.h"
#include<stm32f10x_lib.h>

#define key0 PBin(12)
#define key1 PBin(13)
#define key2 PBin(14)
#define key3 PBin(15)

void KeyInit(void);
u8 KeyScan(void);

#endif


