//////////////////////////////////////////////////////
//     �ļ���: uart3.h
//   �ļ��汾: 1.0.0
//   ����ʱ��: 09��11��30��
//   ��������:  
//       ����: ����
//       ��ע: ��
//
//////////////////////////////////////////////////////

#pragma once 


//����1 ��ҪΪ3����ʹ��.��3�����ǻ����ų��.PC����ͨ��,DTU����ͨ��,485����ͨ��
//   
#define UART3_CONSOLE_TYPE 1
#define UART3_U485_TYPE    2


//#define NULL (void *)0
//����������
#define UART3_MAXIndex 2
//ÿ�г���
#define UART3_MAXBUFFLEN 400


int  UART3_SendtoInt(int num);
int  UART3_Open(int type);
void UART3_Open_9600(int _type);
void UART3_Close();
void UART3_ClearBuffer();
int  UART3_Send(char * data ,int len, int CR); 
int  UART3_RecvLine(char *dest ,int maxNum, int * pNum); 
int  UART3_RecvLineTry(char *dest,const int maxNum, int * pNum); 
int  UART3_RecvLineWait(char *dest ,const int maxNum, int * pNum);
int  UART3_RecvLineLongWait(char *dest,int maxNum, int * pNum);

extern char UART3_Rx_Buffer[UART3_MAXIndex][UART3_MAXBUFFLEN];