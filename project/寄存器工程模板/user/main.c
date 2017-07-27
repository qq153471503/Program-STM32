#include"delay.h"
#include"sys.h"
#include"usart.h"

int main(void)
{
	Stm32_Clock_Init(9);
	delay_init(72);
}
