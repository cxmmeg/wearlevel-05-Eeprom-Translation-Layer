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
extern int s_uart2_type; 
#define UART2_CONSOLE_TYPE 1
#define UART2_U485_TYPE    2
#define UART2_BT_TYPE      3


//#define NULL (void *)0
//����������
#define UART2_MAXIndex 3
//ÿ�г���
#define UART2_MAXBUFFLEN 550


int  UART2_SendtoInt(int num);
int  UART2_Open(int type);
void UART2_Open_9600(int _type);
void UART2_Close();
void UART2_ClearBuffer();
int  UART2_Send(char * data ,int len, int CR); 
int  UART2_RecvLine(char *dest ,int maxNum, int * pNum); 
int  UART2_RecvLineTry(char *dest,const int maxNum, int * pNum); 
int  UART2_RecvLineWait(char *dest ,const int maxNum, int * pNum);
int  UART2_RecvLineLongWait(char *dest,int maxNum, int * pNum);

extern char UART2_Rx_Buffer[UART2_MAXIndex][UART2_MAXBUFFLEN];