void StartOne(void)
{
/*�ҽ�������һ������*/
		for(i=150; i<170; i++)					//�����
		{
			TIM_SetCompare1(TIM2,i);
			delay_ms(15);
		}
		for(i=150; i<=200; i++)					//����ǰ
		{
			TIM_SetCompare2(TIM2,i);
			TIM_SetCompare3(TIM2,i);
			delay_ms(15);
		}		
		for(i=170; i>=150; i--)					//�����
		{
			TIM_SetCompare1(TIM2,i);
			delay_ms(15);
		}
/*--------------------------------------------------------------------------------*/
		for(i=150; i>=130; i--)					//�ҽ���
		{
			TIM_SetCompare4(TIM2,i);
			delay_ms(15);
		}
		for(i=200; i>=120; i--)				
		{
			TIM_SetCompare3(TIM2,i);
			TIM_SetCompare2(TIM2,i);
			delay_ms(15);
		}	
		for(i=130; i<=150; i++)					//�ҽ���
		{
			TIM_SetCompare4(TIM2,i);
			delay_ms(15);
		}
}
void Start(void)
{
		for(i=150; i<170; i++)					//�����
		{
			TIM_SetCompare1(TIM2,i);
			delay_ms(15);
		}
		for(i=120; i<=200; i++)				
		{
			TIM_SetCompare2(TIM2,i);
			TIM_SetCompare3(TIM2,i);
			delay_ms(15);
		}
		for(i=170; i>=150; i--)					//�����
		{
			TIM_SetCompare1(TIM2,i);
			delay_ms(15);
		}
/*------------------------------------------------------------------*/
		for(i=150; i>=130; i--)					//�ҽ���
		{
			TIM_SetCompare4(TIM2,i);
			delay_ms(15);
		}
		for(i=200; i>=120; i--)				
		{
			TIM_SetCompare3(TIM2,i);
			TIM_SetCompare2(TIM2,i);
			delay_ms(15);
		}	
		for(i=130; i<=150; i++)					//�ҽ���
		{
			TIM_SetCompare4(TIM2,i);
			delay_ms(15);
		}
}


