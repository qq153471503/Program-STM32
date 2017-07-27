#ifndef _KEY_H
#define _KEY_H

#include"sys.h"


void KeyScan(void);
void KeyInit(void);

#define key0 PEin(4)
#define key1 PEin(3)
#define key2 PEin(2)

#define key3 PAin(0)

extern char i ;
extern char count;
extern u8 table[27];

#endif

