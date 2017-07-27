#include "GUI.h"

//����EMWIN�ڴ��С
#define GUI_NUMBYTES  (10*1024)
#define GUI_BLOCKSIZE 0X80  //���С


//GUI_X_Config
//��ʼ����ʱ�����,��������emwin��ʹ�õ��ڴ�
void GUI_X_Config(void){

		static U32 aMemory[GUI_NUMBYTES/4];
		GUI_ALLOC_AssignMemory(aMemory, GUI_NUMBYTES); //Ϊ�洢����ϵͳ����һ���洢��
		GUI_ALLOC_SetAvBlockSize(GUI_BLOCKSIZE); //���ô洢���ƽ���ߴ�,����Խ��,���õĴ洢������Խ��
		GUI_SetDefaultFont(GUI_FONT_6X8); //����Ĭ������
}
