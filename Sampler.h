//////////////////////////////////////////////////////
//     �ļ���: sampler.h
//   �ļ��汾: 1.0.0
//   ����ʱ��: 09�� 11��30��
//   ��������:  
//       ����: ����
//       ��ע: ��
//
//////////////////////////////////////////////////////

#pragma once 


extern char g_pulse_rate[4]; //  ������

extern char g_pulse_range[4][3]; 

//  ����ģ����  �Ϳ����� 
int  Sampler_Init();

int  Sampler_Open();

int  Sampler_Close();

int  Sampler_Sample();

int  Sampler_CheckNormal(); 
int  Sampler_GSM_ReadAlertString(char * _dest);
int  Sampler_DTU_ReadAlertString(char * _dest);
int  Sampler_SaveData(char * _saveTime);
int  Sampler_IO_Level(int _ioIdx, int _level);

