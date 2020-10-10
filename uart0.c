//////////////////////////////////////////////////////
//     �ļ���: uart0.c
//   �ļ��汾: 1.0.0
//   ����ʱ��: 09��11��30��
//   ��������:
//       ����: ����
//       ��ע: ��
//
//////////////////////////////////////////////////////

#include "uart0.h"
#include "common.h"
#include "dtu.h"
#include "led.h"
#include "msp430common.h"
#include "uart3.h"
//#include "hydrology.h"
/************A0********/
#define TXD0 BIT4
#define RXD0 BIT5

//���ջ����� ���У�ÿ���������MAXBUFFLEN���ַ�
//�����Խ��� 5��
// �жϴ������
//  ����д�ı���:  1.������ 2.ÿ�г��ȣ���ȡһ�����˺�,��0��дΪ����)
//                 2.��ǰ����д��λ��(���һ�к�,�Ͱ�λ�ö�����һ��)
//  ������ı���:  1.ÿ�г���(�������0,��ʾ����δ����,�ͱ������������)
//                 2.��ǰ����д��λ��,�����д����.
//
// ���ճ���
//  ����д�ı���:  1.ÿ�еĳ���,��ȡ���ʱ���������Ϊ0,
//                                �жϳ����֪�����п��Ը���
//                 2.����λ��, ����һ��,��λ����һ��,�´ο��Լ�����
//  ������ı���:  1.�ö���λ��,  2.���еĳ���
//
//��������״̬����
int IsGetInput = 0;
//���ͻ������ɵ������ṩ
char* UART0_Tx_Buf = NULL;
char UART0_Rx_Buffer[ UART0_MAXIndex ][ UART0_MAXBUFFLEN ];  //  ���ݴ洢��
int  UART0_Rx_BufLen[ UART0_MAXIndex ];	 //  ÿ�н��յ������ݳ���
int  UART0_Rx_INTIndex	      = 0;	 //  �жϸ�д����λ��
int  UART0_Rx_INTLen	      = 0;  //  �жϸ�д���еĵڼ����ַ�
int  UART0_Rx_RecvIndex	      = 0;  //  ��ǰ�ö�����λ��
unsigned int UART0_Tx_Flag    = 0;
unsigned int UART0_Tx_Len     = 0;
int	     Take_Photo_Count = 0;
extern char  _receiveUart0[];
int	     uart0_count  = 0;
int	     real_count	  = 0;
int	     flag_message = 0;
int	     order_length = 0;
//ָʾ��ǰ���͵�
static int    s_uart0_type = 0;
unsigned char test[ 3 ];

char uart0_test[ 2 ];
int  ucount = 0;

//����0�ĳ�ʼ��
void UART0_Open(int _type) {
	//    TraceMsg("uart0 open",1);
	s_uart0_type = _type;
	//   UCTL0 = 0x00;
	//   UCTL0 &=~ SWRST;
	//����λΪ8bit
	//   UCTL0 |= CHAR;
	//���Ĵ�������������
	//   UTCTL0 = 0X00;
	//  9600,    MCLK=32768,  ACLK, ������̫��
	// UTCTL0=SSEL0; UBR0_0 = 0x03;  UBR1_0 = 0X00;  UMCTL0 = 0x4A;
	//  2400,    MCLK=32768,  ACLK, ���ݳ��ֶ�ʧ, 2400̫����
	// UTCTL0=SSEL0; UBR0_0 = 0x0D; UBR1_0 = 0X00; UMCTL0 = 0x6D;
	//  4800,    MCLK=32768,  ACLK, ���ݲ��ȶ�  ,
	// UTCTL0=SSEL0; UBR0_0 = 0x06;  UBR1_0 = 0X00;  UMCTL0 = 0xFB;

	// 57600,    XT2=8000000   SMCLK
	// UTCTL0=SSEL1;UBR0_0 = 0x8A;UBR1_0 = 0x00;UMCTL0 = 0xDE;
	//  9600,    XT2=8000000   SMCLK
	// UTCTL0=SSEL1;UBR0_0 = 0x41;UBR1_0 = 0x03;UMCTL0 = 0x00;  ԭ����
	// 19200��   XT2=8000000   SMCLK
	// UTCTL0=SSEL1;UBR0_0 = 0xA0;UBR1_0 = 0x01;UMCTL0 = 0xC0;
	// 115200��   XT2=8000000   SMCLK
	//   UTCTL0=SSEL1;UBR0_0 = 0x45;UBR1_0 = 0x00;UMCTL0 = 0x4A;

	//   UART0_ClearBuffer();//һЩ���� Ҫ��0

	//   ME1 |= UTXE0 + URXE0; //ʹ��UART0��TXD��RXD
	//   IE1 |= URXIE0;//ʹ��UART0��RX�ж�
	//   IE1 |= UTXIE0;//ʹ��UART0��TX�ж�
	//    P3SEL |= BIT4;//����P3.4ΪUART0��TXD
	//    P3SEL |= BIT5;//����P3.5ΪUART0��RXD
	//    P3DIR |= BIT4;//P3.4Ϊ����ܽ�
	//    return ;

	// GSM/GPRSģ��ͨ��ʱ����TTL��ƽ��ֱ����CPU���͸�ģ��TTL��ƽ�źţ�����ת����ƽ

	UCA0CTL1 |= UCSWRST;  //��λUCSWRSTλ��ʹUARTģ��ļĴ������ڳ�ʼ״̬
	UCA0CTL1 |= UCSSEL1;  // UCLK=SMCLK=1MHz;
	/*
	  ͨ�����������벨�����йصļĴ�����������������
	  UxBR1��UxBR0���Ͳ����������Ĵ���UxMCTL��
	  ʹ�����¹�ʽ���㲨���ʿ��Ʋ�����
	  �����ʲ�����UxBR0��UxBR1��=������ʱ��ԴƵ��/���貨����
	  �������ȡ�������֣�
	  ���磺������ʱ��ԴƵ��=8MHz�����貨����=115200����ô�����ʲ���UxBR0��UxBR1����8000KHz/115.2KHz=69.444444444��ȡ����Ϊ69��С������0.444444444��Ϊ�����Ĵ����еĲ�����
	  ʹ�����¹�ʽ���㣺
	  ����������������UxMCTL��=8 x С�����֣����ȡ�������֣�
	  ��ô��Ӧ����8 x 0.444444444=3.5����ô�������ȡ3��4������ȡ4 ��
	  ��ˣ���Ҫ4������岹λ����UxMCTL�Ĵ����У�����ѡ��4λ��UxMCTL=0x55��ע�⣬�������岹�����ɢ���õ�UxMCTL8��λ�С�
	  ���ˣ����ڲ����ʲ���������ɡ�

	  TI
	  MSP430ϵ�е�Ƭ����usartģ��Ĳ�����ֵ�趨��ͨ�����������Ĵ��������ģ�UxBR0,UxBR1,UxMCTL

	  �����ʣ�BRCLK/N ,��Ҫ�Ǽ����N��

	  BRCLK:ʱ��Դ������ͨ���Ĵ����趨��Ϊʱ��Դ;
	  ͨ���Ĵ���UCAxCTL1��SSEL��λѡ��01:ACLK,02:SMCLK

	  N:�����ʲ����ķ�Ƶ���ӡ�N=UxBR1+UxBR0+UxMCTL,����UxBR1+UxBR0Ϊ�������֣�UxMCTLΪ�趨С�����֣�
	  ���� UxBR1Ϊ��λ��UxBR0Ϊ��λ�����߽������Ϊһ��16λ���֡�

	  ����˵���������ʣ�115200��ʱ��Դ��8MHz ��Ϊ�ⲿ��������

	  N=8000000/115200��69.44
	  �������ԣ�UxBR1+UxBR0��13����UxBR1��0��UxBR0��0x45�����Ȱ�С������0.44
	  ��8��
	  ��3.52��ȡ����Ϊ3�����3��ʾ��UxMCTL�е�8λ��ȥ�����һλ��ǰ7λֵΪ3�����ң�UxMCTL��ΪFirst
	  Stage Modulation�� Second Stage
	  Modulation��Ҳ����ǰ��Ϊ��4λ��ȡֵ��Χ0-F������Ϊ��4λ��3��Ӧ00000110,Ҳ��0x06=UCBRS0+USBRS1����

	  ���ڱ�������ԣ�ʹ����ʱ��SMLK 1M��Ҳ��1000000
	  GTM900�в����ʣ�300,600,1200,2400,4800,9600,14400,19200,38400,57600,115200;
	  1000000/115200=8.7, 0.7*8=5.6, ȡ��Ϊ5��UxBRSΪ 101,
	  UCA0MCTL�ܵ�Ϊ00001010 = 0x0A; 1000000/57600=17.36111,
	  0.36111*8=2.888��ȡ��Ϊ2��UxBRSΪ010,�ܵ�Ϊ00000100 = 0x08;
	  1000000/38400=26.0417,
	  0.0417*8=0.333��ȡ��Ϊ0��UxBRSΪ000,�ܵ�Ϊ00000000 = 0x00;
	  1000000/19200=52.08, 0.08*8=0.64��ȡ��Ϊ0��UxBRSΪ000,�ܵ�Ϊ00000000 =
	  0x00; 1000000/14400=69.44,
	  0.44*8=3.5��ȡ��Ϊ3��UxBRSΪ011,�ܵ�Ϊ00000110 = 0x06;
	  1000000/9600=104.166, 0.166*8=1.3��ȡ��Ϊ1��UxBRSΪ001,�ܵ�Ϊ00000010
	  = 0x02; 1000000/4800=208.333,
	  0.333*8=2.6��ȡ��Ϊ2��UxBRSΪ010,�ܵ�Ϊ00000100 = 0x04;

	  */

	UCA0BR0 = 8;
	UCA0BR1 = 0;
	//  UCA0MCTL |= UCBRF_0+UCBRS_6; //������
	UCA0MCTL = 0x0A;
	UCA0CTL1 &= ~UCSWRST;

	UART0_ClearBuffer();
	/********************/
	P3DIR |= TXD0;
	P3SEL |= TXD0 + RXD0;  //���ͺͽ�������Ϊ��2����
	/*2418 IE2 UCA0RXIE 5438 UCA0IE UCRXIE*/
	UCA0IE |= UCRXIE;  //�����ж�ʹ��
}

void UART0_Open_9600(int _type) {
	TraceMsg("uart0 open", 1);
	s_uart0_type = _type;

	// GSM/GPRSģ��ͨ��ʱ����TTL��ƽ��ֱ����CPU���͸�ģ��TTL��ƽ�źţ�����ת����ƽ

	UCA0CTL1 |= UCSWRST;  //��λUCSWRSTλ��ʹUARTģ��ļĴ������ڳ�ʼ״̬
	UCA0CTL1 |= UCSSEL1;  // UCLK=SMCLK=1MHz;

	UCA0BR0 = 104;
	UCA0BR1 = 0;
	// UCA0MCTL |= UCBRF_0+UCBRS_0; ������
	UCA0MCTL = 2;
	UCA0CTL1 &= ~UCSWRST;
	UART0_ClearBuffer();
	/********************/
	P3DIR |= TXD0;
	P3SEL |= TXD0 + RXD0;  //���ͺͽ�������Ϊ��2����
	/*2418 IE2 UCA0RXIE 5438 UCA0IE UCRXIE*/
	UCA0IE |= UCRXIE;  //�����ж�ʹ��
}

/*δʹ��*/
void Wifi_Uart0_Open()	//��wifi������
{
	//����ƽת����·,
	// WIFI����ʱ����Ҫ���ⷢ�����ݣ���ҪRS232��ƽ������GSM/GPRSģ��ͨ��ʱ����TTL��ƽ

	/****************/
	P4DIR |= BIT0;	//
	P4OUT |= BIT0;

	UCA0CTL1 |= UCSWRST;  //��λUCSWRSTλ��ʹUARTģ��ļĴ������ڳ�ʼ״̬
	UCA0CTL1 |= UCSSEL1;  // UCLK=SMCLK=1MHz;
	//  UCA0BR0 = 52;
	//  UCA0BR1 = 0;
	//  UCA0MCTL |= UCBRF_0+UCBRS_0;

	UCA0BR0 = 8;
	UCA0BR1 = 0;
	UCA0MCTL |= UCBRF_0 + UCBRS_6;

	UCA0CTL1 &= ~UCSWRST;
	UART0_ClearBuffer();
	P3DIR |= TXD0;
	P3SEL |= TXD0 + RXD0;  //���ͺͽ�������Ϊ��2����

	// IE2 |= UCA0RXIE;           //�����ж�ʹ��

	/*2418 UC0IE UCA0RXIE 5438 UCA0IE UCRXIE*/
	UCA0IE |= UCRXIE;
}

void UART0_Close() {  //�ر�UART0�Ľ��պ� ���͹���

	//   TraceMsg("uart0 close",1);
	/*2418 UC0IE UCA0RXIE 5438 UCA0IE UCRXIE*/
	UCA0IE &= ~UCRXIE;
	//  IE2 &= ~UCA0TXIE;

	//һЩ���� Ҫ��0
	UART0_ClearBuffer();
}

void UART0_ClearBuffer() {
	DownInt();  //���ж�

	UART0_Tx_Buf	   = 0;
	UART0_Rx_INTIndex  = 0;
	UART0_Rx_INTLen	   = 0;
	UART0_Rx_RecvIndex = 0;

	UART0_Tx_Flag = 0;
	UART0_Tx_Len  = 0;
	IsGetInput    = 0;
	for (int i = 0; i < UART0_MAXIndex; ++i)
		UART0_Rx_BufLen[ i ] = 0;

	UpInt();  //���ж�
}

int UART0_WaitInput() {
	int temp = 0;
	while (IsGetInput != 1) {
		System_Delayms(150);  // 10 * 100 ms( 1.5s��δ�յ�)
		if (temp++ > 10)
			return -1;
	}
	IsGetInput = 0;	 // �ȵ��� return 0
	return 0;
}
int UART0_ClearInput() {
	IsGetInput = 0;
	return 0;
}
int UART0_Send(char* data, int len, int CR) {

	if (UART0_Tx_Flag != 0)	 //�ȴ���һ�η��ͽ���
	{			 //�͵�500ms
		System_Delayms(500);
		UART0_Tx_Flag = 0;  //ǿ������Ϊ0;
	}
	//��ȫ�ֱ�����ֵ
	UART0_Tx_Buf = data;  //�����һ�������ж� ����Ϊ0
	UART0_Tx_Len = len;  //�����һ�������ж� ����Ϊ0,�෢���һ����������
	UART0_Tx_Flag = 1;
	for (int i = 0; i < UART0_Tx_Len; i++) {
		while (!(UCA0IFG & UCTXIFG))
			;
		UCA0TXBUF = UART0_Tx_Buf[ i ];
	}
	UART0_Tx_Flag = 0;
	if (!CR) {
		return 0;
	}
	// ATָ���һ������
	if (s_uart0_type == UART0_GSM_TYPE) {  // ATָ����Ϊ13��Ϊ����
		char _cr1    = 13;
		UART0_Tx_Buf = &_cr1;
		UART0_Tx_Len = 1;
	}
	else {	// DTU��Ϊ 13 10Ϊ����
		char _cr2[ 2 ];
		_cr2[ 0 ]    = 13;
		_cr2[ 1 ]    = 10;
		UART0_Tx_Buf = _cr2;
		UART0_Tx_Len = 2;
	}
	UART0_Tx_Flag = 1;
	for (int i = 0; i < UART0_Tx_Len; i++) {
		/*2418 UC0IFG UCA0TXIFG 5438 UCA0IFG UCTXIFG*/
		while (!(UCA0IFG & UCTXIFG))
			;
		UCA0TXBUF = UART0_Tx_Buf[ i ];
	}
	UART0_Tx_Flag = 0;
	return 0;
}

int GPRS_RecvLineTry(char* dest, int maxNum, int* pNum) {
	int i = 0;
	if (UART0_Rx_BufLen[ UART0_Rx_RecvIndex ] == 0)
		return -1;
	//�������ˣ��Ͱ����ݸ��Ƴ���,
	for (i = 0; (i < maxNum) && (i < UART0_Rx_BufLen[ UART0_Rx_RecvIndex ]);
	     ++i) {
		dest[ i ] = UART0_Rx_Buffer[ UART0_Rx_RecvIndex ][ i ];
	}
	// i���ַ������һ���±���i-1
	dest[ i - 1 ] = '\0';
	*pNum	      = i - 1;
	return 0;
}

int UART0_RecvLine(char* dest, int maxNum, int* pNum) {
	int i;
	//�ö���λ�ó���Ϊ0, ��ѭ���ȴ�
	while (UART0_Rx_BufLen[ UART0_Rx_RecvIndex ] == 0)
		;
	//�������ˣ��Ͱ����ݸ��Ƴ���,
	for (i = 0; (i < maxNum) && (i < UART0_Rx_BufLen[ UART0_Rx_RecvIndex ]);
	     ++i) {
		dest[ i ] = UART0_Rx_Buffer[ UART0_Rx_RecvIndex ][ i ];
	}
	// i���ַ������һ���±���i-1
	dest[ i - 1 ] = '\0';
	*pNum	      = i;
	//������Ϻ�,�ͰѸ�λ�õĳ�������Ϊ0,�����жϿ��Ը�����.
	UART0_Rx_BufLen[ UART0_Rx_RecvIndex ] = 0;
	//��λ����һ��
	// ��� ����9 �ͼ�ȥ9 ,�ӵ�һ�п�ʼ,����ͼ�������.
	UART0_Rx_RecvIndex += UART0_Rx_RecvIndex < (UART0_MAXIndex - 1)
				      ? 1
				      : 1 - UART0_MAXIndex;
	return 0;
}
int UART0_RecvLineTry(char* dest, int maxNum, int* pNum) {
	int i = 0;
	if (UART0_Rx_BufLen[ UART0_Rx_RecvIndex ] == 0)
		return -1;
	//�������ˣ��Ͱ����ݸ��Ƴ���,
	for (i = 0; (i < maxNum) && (i < UART0_Rx_BufLen[ UART0_Rx_RecvIndex ]);
	     ++i) {
		dest[ i ] = UART0_Rx_Buffer[ UART0_Rx_RecvIndex ][ i ];
	}
	// i���ַ������һ���±���i-1
	dest[ i - 1 ] = '\0';
	*pNum	      = i - 1;
	//������Ϻ�,�ͰѸ�λ�õĳ�������Ϊ0,�����жϿ��Ը�����.
	UART0_Rx_BufLen[ UART0_Rx_RecvIndex ] = 0;
	//��λ����һ��
	// ��� ����MAX-1 �ͼ�ȥMAX-1 ,�ӵ�һ�п�ʼ,����ͼ�������.
	UART0_Rx_RecvIndex += UART0_Rx_RecvIndex < (UART0_MAXIndex - 1)
				      ? 1
				      : 1 - UART0_MAXIndex;
	return 0;
}
int UART0_RecvLineWait(char* dest, int maxNum, int* pNum) {  //���300ms
	int i = 0;
	int _waits;
	if (s_uart0_type == UART0_GSM_TYPE)
		_waits = 10;  // 300ms
	else
		_waits = 100;  // 3��

	while (UART0_Rx_BufLen[ UART0_Rx_RecvIndex ]
	       == 0)  //�ö���λ�ó���Ϊ0, ��ѭ���ȴ�
	{
		System_Delayms(30);
		++i;
		if (i > _waits) {  //������
			return -1;
		}
	}
	//�������ˣ��Ͱ����ݸ��Ƴ���,
	for (i = 0; (i < maxNum) && (i < UART0_Rx_BufLen[ UART0_Rx_RecvIndex ]);
	     ++i) {
		dest[ i ] = UART0_Rx_Buffer[ UART0_Rx_RecvIndex ][ i ];
	}
	// i���ַ������һ���±���i-1
	dest[ i - 1 ] = '\0';
	*pNum	      = i - 1;
	//������Ϻ�,�ͰѸ�λ�õĳ�������Ϊ0,�����жϿ��Ը�����.
	UART0_Rx_BufLen[ UART0_Rx_RecvIndex ] = 0;
	//��λ����һ��
	// ��� ����MAX-1 �ͼ�ȥMAX-1 ,�ӵ�һ�п�ʼ,����ͼ�������.
	UART0_Rx_RecvIndex += UART0_Rx_RecvIndex < (UART0_MAXIndex - 1)
				      ? 1
				      : 1 - UART0_MAXIndex;
	return 0;
}

int New_UART0_RecvLineWait() {	//���300ms
	int i = 0;
	int _waits;
	if (s_uart0_type == UART0_GSM_TYPE)
		_waits = 10;  // 300ms
	else
		_waits = 100;  // 3��

	while (uart0_count == 0)  //�ö���λ�ó���Ϊ0, ��ѭ���ȴ�
	{
		System_Delayms(30);
		++i;
		if (i > _waits) {  //������
			return -1;
		}
	}
	return 0;
}

int UART0_RecvLineLongWait(char* dest, int maxNum, int* pNum) {
	int i	   = 0;
	int times1 = 0;
	//��ͬ��ģʽ��waits��һ��.
	int _waits;
	if (s_uart0_type == UART0_GSM_TYPE)
		_waits = 60;  // 3��
	else
		_waits = 200;  // 10��

	//�ö���λ�ó���Ϊ0, ��ѭ���ȴ�
	while (UART0_Rx_BufLen[ UART0_Rx_RecvIndex ] == 0) {
		System_Delayms(50);
		++times1;
		if (times1 > _waits) {	// ������
			return -1;
		}
	}
	//�������ˣ��Ͱ����ݸ��Ƴ���,
	for (i = 0; (i < maxNum) && (i < UART0_Rx_BufLen[ UART0_Rx_RecvIndex ]);
	     ++i) {
		dest[ i ] = UART0_Rx_Buffer[ UART0_Rx_RecvIndex ][ i ];
	}
	// i���ַ������һ���±���i-1
	dest[ i - 1 ] = '\0';
	*pNum	      = i - 1;
	//������Ϻ�,�ͰѸ�λ�õĳ�������Ϊ0,�����жϿ��Ը�����.
	UART0_Rx_BufLen[ UART0_Rx_RecvIndex ] = 0;
	//��λ����һ��
	// ��� ����MAX-1 �ͼ�ȥMAX-1 ,�ӵ�һ�п�ʼ,����ͼ�������.
	UART0_Rx_RecvIndex += UART0_Rx_RecvIndex < (UART0_MAXIndex - 1)
				      ? 1
				      : 1 - UART0_MAXIndex;
	return 0;
}

//
//  �����ж� ����ͨ������
//
//  ���⴦���:
//     ��ͷ�� 13 10 ����ֱ������.
//
// UART0_Rx_Buffer[UART0_MAXIndex][UART0_MAXBUFFLEN];//  ���ݴ洢��
// UART0_Rx_BufLen[UART0_MAXIndex];            //  ÿ�н��յ������ݳ���
// UART0_Rx_INTIndex=0;                //  �жϸ�д����λ��
// UART0_Rx_INTLen=0;                  //  �жϸ�д���еĵڼ����ַ�
// UART0_Rx_RecvIndex=0;               //  ��ǰ�ö�����λ��
/******************VECTOR****/
#pragma vector = USCI_A0_VECTOR

__interrupt void UART0_RX_ISR(void)  //�����յ����ַ���ʾ���������
{

	char _temp;  //%IPDATA:1,1,87

	_temp = UCA0RXBUF;
//     UCA1TXBUF=_temp;
//    UART3_Send(&_temp,1,0);//�����õĴ���
#if 1
	//�����ͷ�յ� > ����,˵��������ʾ���ĵ���,
	//�������>,������ȫ�ֱ��
	// �����ܸ���ʱ�� ����, ���ǲ��ڼ���Ƿ�����ͷ.
	// ������������������֤���ᱻ�����е� > Ӱ�쵽.
	if (_temp == '>')  // && UART0_Rx_INTLen==0)
	{
		IsGetInput = 1;
		return;
	}
	//��ͷ�յ��ո�, ������,��Ϊ�� > �󸽴��� �ո�
	if (_temp == ' ' && UART0_Rx_INTLen == 0) {
		return;
	}
	//һ�����
	//�ж��Ƿ���DTU״̬.���״̬������0x1F���µ�,���˻��� �س�
	// if(s_uart0_type==UART0_DTU_TYPE&& _temp<=0x1F && _temp!=0x0A && _temp
	// != 0x0D && _temp != 0x00)
	//{
	//    return ;
	//}
	if (UART0_Rx_INTLen
	    >= UART0_MAXBUFFLEN - 1) {	//�г��������, ����ֱ�ӽضϳ�һ��.
		UART0_Rx_Buffer[ UART0_Rx_INTIndex ][ UART0_Rx_INTLen - 1 ] =
			13;  //������ø�13
		UART0_Rx_BufLen[ UART0_Rx_INTIndex ] =
			UART0_Rx_INTLen;  //���������з�
		UART0_Rx_INTLen = 0;
		UART0_Rx_INTIndex += UART0_Rx_INTIndex < (UART0_MAXIndex - 1)
					     ? 1
					     : 1 - UART0_MAXIndex;
	}
	// ֻ��Ϊ13 10(0x0D 0x0A)�ǻ��з���. �г��Ȳ��������з���
	// (�����Ҫ�رջ���,��Ϊ������13��β)
	//
	if ((_temp == 0x0A) && (UART0_Rx_INTLen != 0)
	    && (UART0_Rx_Buffer[ UART0_Rx_INTIndex ][ UART0_Rx_INTLen - 1 ]
		== 0x0D)) {
		//�����ͷ���յ���������з���
		// ֱ������
		if (UART0_Rx_INTLen == 1) {
			UART0_Rx_INTLen = 0;  //���¿�ʼ����
			return;
		}
		else {
			//���д���
			//�յ�����һ�к�Ĵ���
			//   ���еĳ�������Ϊ���ݳ���
			//   ��λ����һ��
			//   ��д���ַ�λ ����Ϊ0
			//   ���з��Ŵ���Ϊ13
			UART0_Rx_Buffer[ UART0_Rx_INTIndex ]
				       [ UART0_Rx_INTLen - 1 ] =
					       13;  //������ø�13
			UART0_Rx_BufLen[ UART0_Rx_INTIndex ] =
				UART0_Rx_INTLen;  //���������з�
			UART0_Rx_INTLen = 0;
			UART0_Rx_INTIndex +=
				UART0_Rx_INTIndex < (UART0_MAXIndex - 1)
					? 1
					: 1 - UART0_MAXIndex;
			//            JudgeServerDataArrived();
			return;
		}
	}

	//�ж��Ƿ�����
	if (UART0_Rx_BufLen[ UART0_Rx_INTIndex ] != 0) {
		//��һ�л�δ�������Ǿ͸�����������һ��
		UART0_Rx_INTIndex -=
			UART0_Rx_INTIndex > 0 ? 1 : 1 - UART0_MAXIndex;
		//�Ѹ��г�������Ϊ0,���ж�ռ��
		UART0_Rx_BufLen[ UART0_Rx_INTIndex ] = 0;
	}
	//һ�����
	UART0_Rx_Buffer[ UART0_Rx_INTIndex ][ UART0_Rx_INTLen ] = _temp;
	++UART0_Rx_INTLen;
#endif
}
