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
extern int s_uart1_type; 
#define UART1_CONSOLE_TYPE 1
#define UART1_U485_TYPE    2

//#define NULL (void *)0
//����������
#define UART1_MAXIndex 2
//ÿ�г���
#define UART1_MAXBUFFLEN 3


int  UART1_SendtoInt(int num);
int  UART1_Open(int type);
void UART1_Open_9600(int _type);
void UART1_Close();
void UART1_ClearBuffer();
int  UART1_Send(char * data ,int len, int CR); 
int  UART1_RecvLine(char *dest ,int maxNum, int * pNum); 
int  UART1_RecvLineTry(char *dest,const int maxNum, int * pNum); 
int  UART1_RecvLineWait(char *dest ,const int maxNum, int * pNum);
int  UART1_RecvLineLongWait(char *dest,int maxNum, int * pNum);

extern char UART1_Rx_Buffer[UART1_MAXIndex][UART1_MAXBUFFLEN];