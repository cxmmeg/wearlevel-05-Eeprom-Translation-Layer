//////////////////////////////////////////////////////
//     �ļ���: gsm.h
//   �ļ��汾: 1.0.0
//   ����ʱ��: 09�� 11��30��
//   ��������: 
//       ����: ����
//       ��ע: ��
// 
//////////////////////////////////////////////////////

#pragma once 

int GSM_Open();   //called by others
int GSM_Close(int _type); //called by others
int GSM_Init(); // �رջ���,�����ö���Ϊtxtģʽ   //called by others
int GSM_CheckOK();  //called by others
int GSM_Waiting_Process(int type);  
int GSM_Report_Process();  
int GSM_Alert_Process();
int GSM_CheckLive();// ���GSM�Ƿ���������
int GSM_SendReportMsg();
int GSM_ProcMessageTxt(int index);
int GSM_StartCheckSIM();
int GSM_EndCheckSIM();
int GSM_Process(int _waits, int _type);

int GSM_SendMsgTxt(char * phoneNum, char * data,int dataLen); 

int GSM_ProcMsgData(char * data,char* rePhone,int dataLen);

int GSM_GetMsgStore(char *recv); //����Ӧ��ȡ�ö������� 

int GSM_GetRePhone(char *src, char *dest);//��ö��ŵĻظ����� 
 
int GSM_ProcMessageTxt(int _index);
 
int GSM_ProcMsgArray();   
int GSM_CallLeader(char *phone); //�������˴�绰  

int GSM_DealData(char * _recv, int _dataLen);
int GSM_DealAllData();

//AT����
int  GSM_AT_QuerySim();
int GSM_AT_QueryStore();     
int GSM_AT_QueryNet();   
int GSM_AT_SetMsgmode(int mode);   
int GSM_AT_CloseFeedback();   
int GSM_AT_SetMsgNotify();  
int GSM_AT_DeleteMsg(int index);    
int GSM_AT_ShutDown();  
int GSM_AT_QuitInput();  
int GSM_AT_OffCall(); 
int  GSM_AT_QueryTime(char *year,char *month,char *date,char *hour,char *min,char *second);