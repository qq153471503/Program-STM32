#include "iic.h"
#include "delay.h"

extern void Bh1750_Test(void);
extern void Hmc5883_Test( void);



void IIC_Test(void)
{
    while( 1)
		{
//      Bh1750_Test();
// 		 delay_ms(500);
     Hmc5883_Test();
     delay_ms(1000);

    }


}

