#ifndef __BZ_H_
#define __BZ_H_
#include "sys.h"



extern u8 Table[8]; 
extern u8 guangdang;
///////////////////////////����־��/////////////////////////////////////////
void ceju(void);
void Distance(void);	//���������Ժ���
void CLight(void);			//���ǿ�����ҵ���
void erweima(void);		//����ɨ���ά��ָ����ȴ�ƽ�巢��ɨ�����
void erweima1(void);
void Chepai(void);		//����ɨ�賵������
void Tuxing(void);		//����ɨ��ͼ������
void LTDisplay(void);	//����������ʾ��־��
void Shibiefangxiang(void);	//ʶ����
void Cixuanfu(void);
void Yuyinbobao(void);	//����������־�����
void duka(void);
void baojingqi(void);
void ETCopen(void);
//////////////////////////��������//////////////////////////////////////////
void zuozhuan(void);	//////////////////��ת
void youzhuan(void);	//////////////////��ת
void Bzuozhuan(void);	//////////////////ԭ����ת
void Byouzhuan(void);	//////////////////ԭ����ת
void qianjinMP(u8 mp);//////////////////ǰ��MP
void qianjingMP(u8 mp);
void houtuiMP(u8 mp);	//////////////////����MP
void xunji(void);
void xunji1(void);
void sishiwu(void);
void qianxun(u8 mp);
////////////////////////wifi��zigbeeͨ��//////////////////////////////////////
void WifiSend(void);	//������ƽ�巢������
void ZigbeeSend(void);//������zigbee��������
//////////////////////////zigbee���Ʊ�־��//////////////////////////////////
void OpenJS(void);	//����ʱ��
void CloseJS(void);	//�ؼ�ʱ��
void OpenDZ(void);	//����բ
void CloseDZ(void);	//�ص�բ
void OpenBJQ(void);
void TFTOpenJS(void);
void TFTCloseJS(void);
void TFTDown(void);
void TFTUp(void);
///////////////////////�ӳ�����//////////////////////////////////////////
void congche(void);
#endif 


