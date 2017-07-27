#ifndef __SYN7138_H
#define __SYN7138_H	 
#include "sys.h"

#define uchar unsigned char
#define uint unsigned int
	
#define SYN7318_RST_H   (GPIOD->BSRR = GPIO_Pin_4 )
#define SYN7318_RST_L   (GPIOD->BRR  = GPIO_Pin_4 )
	

extern void SYN7318_Test( void);  // ø™∆Ù”Ô“Ù≤‚ ‘
extern unsigned char Ysn7813_flag;
extern unsigned char Ysn7813_flag_cs;
void SYN7318_Open(void);
void SYN7318_Close(void);

void SYN7318_Put_Char(uchar txd);
void SYN7318_Put_String(uchar* Pst,uint Length);
uchar SYN7318_Get_char( void) ;
void SYN7318_Get_String(unsigned char *Pst,unsigned char Length);
void SYN_TTS(uchar *Pst);
void SYN_MP3(uchar *Pst);
void Start_ASR(uchar Dict);
void Stop_ASR();
void Dict_TTS(uchar Dict);
void Start_WakeUp(uchar Wake_ID);
void Three_One(uchar Dict, uchar Wake_ID, uchar Sound, uchar *Pst);
void Status_Query();
u8  SYN7318_Rst(void) ;

extern void SYN_TTS(uchar *Pst);
extern void SYN7318_Init(void);

#endif


