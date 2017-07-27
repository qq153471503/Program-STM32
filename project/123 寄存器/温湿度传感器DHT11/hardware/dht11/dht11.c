#include"dht11.h"

u8 temph,templ,humih,humil;

/**�������ܣ�dht11��ʼ����*/
void Start()
{
	DHT11_IO_OUT();				/*��dht11���ݿ����ó�����������źţ�*/

	dhtout = 0;					//����
	delay_ms(20);				//��ʱ20ms	����18ms
	dhtout = 1;					//����
	delay_us(30);				//�˴���ʱ20~40us
}

/*dht11��һ���ֽ�*/
u8 ReceiveByte(void) 
{
	u8 value ,i;
	u8 datbyte;

	for(i=0; i<8; i++)
	{
		while(!dhtin);	   //�ȴ���Ӧ�źŽ���
		value = 0;

		delay_us(28);	   
//��Ӧ�źŽ���֮����ʱ26us-28us  
// ����Ҫ�ܾ�ȷ

		if( dhtin == 1 )  		 //���28us�����Ǹߵ�ƽ  ˵����1
			value = 1;
		while(dhtin);				//�ȴ������źŪ�'0'Ϊ26us-28us��'1'Ϊ70us
		datbyte <<= 1;			//����
		datbyte |= value;		//�������
	}
	return datbyte ;
}
void ReceiveData(void)
{
	u8 check ,jycheck ,TH,TL,HH,HL;
	
	Start();				//���ÿ�ʼ����
	DHT11_IO_IN();			//�ӻ��ź� Ҫ���ó�����
	dhtin = 1;				//������ 

	if(!dhtin)		//�ж��Ƿ��е͵�ƽ��Ӧ�ź�
	{
		while(!dhtin);		//�ж���Ӧ�ź��Ƿ����
		while(dhtin);		//�ȴ���Ӧ�źź��40~50us�ߵ�ƽ����	������ʼ��������
		HH = ReceiveByte();				 //ʪ�ȸ�λ
		HL = ReceiveByte();				 //ʪ�ȵ�λ
		TH = ReceiveByte();				 //�¶ȸ�λ
		TL = ReceiveByte();				 //�¶ȵ�λ
		check = ReceiveByte();			 //У��λ

		dhtin = 0;						 //���ݽ��ս��� ������ʱ50us
		delay_us(50);

		dhtin = 1; 				//���� �������

		jycheck = HH+HL+TH+TL ;		  //��������

		if(jycheck == check)		//�ж�У��λ�����Ƿ��������֮��
		{
			humih = HH;
			humil = HL;
			temph = TH;
			templ = TL;
			check = jycheck;
		}
	}	
}

/*dht11 I/O�ڳ�ʼ��*/
void Dht11IoInit(void)
{
	RCC -> APB2ENR |= 1<<8;  //ʹ��GPIOG��ʱ��

	GPIOG -> CRH &= 0XFFFF0FFF;		   //���ó��������
	GPIOG -> CRH |= 0X00003000;
	GPIOG -> ODR |= 1<<11;			   //����λ�Ҹ�
}

