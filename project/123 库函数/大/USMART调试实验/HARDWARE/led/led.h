#ifndef _LED_H
#define _LED_H


/*led�� ��0 ����*/
#include "sys.h"

void LedInit(void);

#define led0 PBout(5) /*��ɫ��*/
#define led1 PEout(5) /*��ɫ��*/

#endif
