//////////////////////////////////////////////////////
//     �ļ���: uart0.h
//   �ļ��汾: 1.0.0
//   ����ʱ��: 09�� 11��30��
//   ��������:  
//       ����: ����
//       ��ע: ��
//
//////////////////////////////////////////////////////

#pragma once 


//#define NULL   (void *)0
//����������
#define UART0_MAXIndex 3
//ÿ�г���
#define UART0_MAXBUFFLEN 215
 
#define UART0_GSM_TYPE    1
#define UART0_DTU_TYPE    2
#define UART0_GTM900_TYPE 3

void UART0_Open(int _type); 
void UART0_Open_9600(int _type);
void Wifi_Uart0_Open();//��wifi������
void UART0_Close();
void UART0_ClearBuffer();
int  UART0_Send(char * data ,int len, int CR);
int GPRS_RecvLineTry(char *dest, int maxNum, int * pNum);
int  UART0_RecvLine(char *dest ,int maxNum, int * pNum);
int  UART0_RecvLineTry(char *dest,const int maxNum, int * pNum);
int  UART0_RecvLineWait(char *dest ,const int maxNum, int * pNum);
int  UART0_RecvLineLongWait(char *dest,int maxNum, int * pNum); 
int  UART0_WaitInput();
int  UART0_ClearInput();
int New_UART0_RecvLineWait(); 


extern char UART0_Rx_Buffer[UART0_MAXIndex][UART0_MAXBUFFLEN];