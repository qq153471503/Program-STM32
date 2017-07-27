#ifndef _SERVO_H
#define _SERVO_H

#include "sys.h"

#define SO_Single PAout(15)

void Servo_Init(void);
void Servo_Left_PWM(u16 arr,u16 psc);
void Servo_Right_PWM(u16 arr,u16 psc);

void StartOne(void);
void Start(void);

#endif

