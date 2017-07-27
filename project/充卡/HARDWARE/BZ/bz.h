#ifndef __BZ_H_
#define __BZ_H_
#include "sys.h"



extern u8 Table[8]; 
extern u8 guangdang;
///////////////////////////各标志物/////////////////////////////////////////
void ceju(void);
void Distance(void);	//超声波测试函数
void CLight(void);			//测光强，并且调光
void erweima(void);		//发送扫描二维码指令，并等待平板发回扫描完成
void erweima1(void);
void Chepai(void);		//发送扫描车牌命令
void Tuxing(void);		//发送扫描图形命令
void LTDisplay(void);	//控制立体显示标志物
void Shibiefangxiang(void);	//识别方向
void Cixuanfu(void);
void Yuyinbobao(void);	//语音播报标志物控制
void duka(void);
void baojingqi(void);
void ETCopen(void);
//////////////////////////主车控制//////////////////////////////////////////
void zuozhuan(void);	//////////////////左转
void youzhuan(void);	//////////////////右转
void Bzuozhuan(void);	//////////////////原地左转
void Byouzhuan(void);	//////////////////原地右转
void qianjinMP(u8 mp);//////////////////前进MP
void qianjingMP(u8 mp);
void houtuiMP(u8 mp);	//////////////////后退MP
void xunji(void);
void xunji1(void);
void sishiwu(void);
void qianxun(u8 mp);
////////////////////////wifi，zigbee通信//////////////////////////////////////
void WifiSend(void);	//主车给平板发送数据
void ZigbeeSend(void);//主车给zigbee发送数据
//////////////////////////zigbee控制标志物//////////////////////////////////
void OpenJS(void);	//开计时器
void CloseJS(void);	//关计时器
void OpenDZ(void);	//开道闸
void CloseDZ(void);	//关道闸
void OpenBJQ(void);
void TFTOpenJS(void);
void TFTCloseJS(void);
void TFTDown(void);
void TFTUp(void);
///////////////////////从车控制//////////////////////////////////////////
void congche(void);
#endif 


