#include"TRACK.H"

#include"timer.h"

u8 flag ;

void XjInit(void)
{
	RCC -> APB2ENR |= 1<<2;
	GPIOA -> CRL &= 0XFFFFFFFF;
	GPIOA -> CRL |= 0X88888888;
	GPIOA -> ODR |= 0X00FF;
}

void Track(void)
{
	flag = GPIOA -> IDR&0xff;

	if(flag == (0xe7||0xf7||0xef))			//前进		   if(flag == (0xe7||0xf7||0xef))
	{
		LeftPwm = RightPwm = 900;
		Motor_Go();

//		if(flag == (0XFD||0XFE))					 //右拐			if()
//		{
//			LeftPwm = RightPwm = 300;
//			Motor_Right();
//			if(flag == (0xe7||0xf7||0xef))			//前进
//			{
//				LeftPwm = RightPwm = 900;
//				Motor_Go();
//				if(flag == (0x7F||0XBF))				  //左拐
//				{
//					LeftPwm = RightPwm = 300;
//					Motor_Left();
//
//					if(flag == (0xe7||0xf7||0xef))		   //前进
//					{
//						LeftPwm = RightPwm = 900;
//						Motor_Go();
//
//						if(flag == (0x7F||0XBF))				//左拐
//						{
//							LeftPwm = RightPwm = 300;
//							Motor_Left();
//
//							if(flag == (0xe7||0xf7||0xef))		//前进
//							{
//								LeftPwm = RightPwm = 900;
//								Motor_Go();
//
//								if(flag == (0XFD||0XFE))			   //右拐
//								{
//									LeftPwm = RightPwm = 300;
//									Motor_Right();
//
//									if(flag == (0xe7||0xf7||0xef))	   //前进
//									{
//										LeftPwm = RightPwm = 900;
//										Motor_Go();
//
//										if(flag == (0XFD||0XFE))
//										{
//											LeftPwm = RightPwm = 300;
//											Motor_Right();
//
//											if(flag == (0xe7||0xf7||0xef))
//											{
//												LeftPwm = RightPwm = 900;
//												Motor_Go();
//
//												if(flag == (0x7F||0XBF))
//												{
//													LeftPwm = RightPwm = 300;
//													Motor_Left();
//
//													if(flag == (0xe7||0xf7||0xef))
//													{
//														LeftPwm = RightPwm = 900;
//														Motor_Go();
//														
//														if(flag == (0x7F||0XBF))
//														{
//																			
//															LeftPwm = RightPwm = 300;
//															Motor_Left();
//
//															if(flag == (0xe7||0xf7||0xef))
//															{
//																LeftPwm = RightPwm = 900;
//																Motor_Go();
//															}
//														}
//															
//													}
//													
//														
//												}
//												
//													
//											}
//										}
//
//
//									}
//								}
//							}
//						}				
//					}
//				}
//			
//			}	
//		}
 	}
}
