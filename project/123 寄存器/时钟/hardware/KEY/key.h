#ifndef _KEY_H_
#define _KEY_H_

#include"sys.h"

/*按键io接口宏定义*/
#define wk_up PAin(0)
#define key0 PEin(4)
#define key1 PEin(3)
#define key2 PEin(2)

/*声明函数*/
void KeyInit(void);
void KeyScan(void);

#endif

