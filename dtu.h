//////////////////////////////////////////////////////
//     �ļ���: dtu.h
//   �ļ��汾: 1.0.0
//   ����ʱ��: 09��11��30��
//   ��������:  
//       ����: ����
//       ��ע: ��
//
//////////////////////////////////////////////////////

#pragma once
int  DTU_Open(int _wait); 
int  DTU_CheckOK();
void DTU_Close();
int  DTU_Report_Process();
int  DTU_Waiting_Process(int type);
int  DTU_Alert_Process();
int  DTU_SendData(char * _data, char _len);
int  DTU_ProcRecvData(char * _data, int _dataLen); 
int  DTU_ProcAllData();
  
