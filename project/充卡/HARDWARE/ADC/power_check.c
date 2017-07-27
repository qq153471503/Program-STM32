#include "adc.h"
#include "delay.h"
#define __POWER_CHECK_C__
#include "power_check.h"

#define POWER_CHECK_L    14 // 电量采集通道
#define POWER_CHECK_R    15
#define POWER_CHECK_NUM  3  // ADC采样次数

#define PWR_MAX  12000.0    // 12V
#define PWR_MIN   9000.0    // 9V
#define PWR_DV   (PWR_MAX - PWR_MIN)

float Pa =0.0,Pb =0.0;  // 计算参数

u8 pwr_ck_l =0;  // 电量数据
u8 pwr_ck_r =0;



u16  Get_Electricity( u8 ch ,u8 times )
{
	u32 temp_val=0;
	u8 t;
	for(t=0;t<times;t++)
	{
		temp_val+= Get_Adc(ch);
		 delay_ms(5);
	}
	return temp_val/times;
}


void Parameter_Init(void) // 电量计算参数初始化
{
   Pb =(float) (PWR_MIN / PWR_DV);
	 Pb *= 100; 
	
	 Pa = (float)(3300*11)/4096 ;
	 Pa = (float)((Pa *100) /PWR_DV);
	
	

}

void Electricity_Init(void)  // 电量检测初始化
{
    Parameter_Init();
	  Adc_Init();

}

void Power_Check(void)
{
    u16 temp;
	
	  temp =  Get_Electricity( POWER_CHECK_L , POWER_CHECK_NUM );	
    temp = (Pa*temp); // 电量计算方法
	  	
	  if( temp < Pb ) pwr_ck_l =0;
	  else 
		{
       pwr_ck_l = (u8) ( temp - Pb);
			
			 if( pwr_ck_l >100)  pwr_ck_l =100;
    }

	  temp =  Get_Electricity( POWER_CHECK_R , POWER_CHECK_NUM );	
    temp = (Pa*temp); // 电量计算方法
	  	
	  if( temp < Pb ) pwr_ck_r =0;
	  else 
		{
       pwr_ck_r = (u8) ( temp - Pb);
			
			 if( pwr_ck_r >100)  pwr_ck_r =100;
    }	  

	
	 Electric_Buf[0] = pwr_ck_l;
	 Electric_Buf[1] = pwr_ck_r;	 
	
}



