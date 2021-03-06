#ifndef __ADC_H
#define __ADC_H	
#include "sys.h"

#define true 1
#define false 0


// these variables are volatile because they are used during the extern interrupt service routine!
extern int BPM;                   			 // used to hold the pulse rate
extern int Signal;               			 // holds the incoming raw data
extern int IBI;            			 // holds the time between beats, must be seeded! 
extern unsigned char Pulse ;     // true when pulse wave is high, false when it's low
extern unsigned char QS;        // becomes true when Arduoino finds a beat.
extern int rate[10];                    // array to hold last ten IBI values
extern unsigned long sampleCounter; // used to determine pulse timing
extern unsigned long lastBeatTime;  // used to find IBI
extern int P;                      // used to find peak in pulse wave, seeded
extern int T;                     // used to find trough in pulse wave, seeded
extern int thresh;                // used to find instant moment of heart beat, seeded
extern int amp;                   // used to hold amplitude of pulse waveform, seeded
extern unsigned char firstBeat;  // used to seed rate array so we startup with reasonable BPM
extern unsigned char secondBeat;// used to seed rate array so we startup with reaso

void Adc_Init(void);
u16  Get_Adc(u8 ch); 
u16 Get_Adc_Average(u8 ch,u8 times); 
 
#endif 
