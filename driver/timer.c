//////////////////////////////////////////////////////
//     �ļ���: timer.c
//   �ļ��汾: 1.0.0
//   ����ʱ��: 09��11��30��
//   ��������: �ޡ�
//       ����: ����
//       ��ע:
//
//
//////////////////////////////////////////////////////

#include "timer.h"
#include "common.h"
#include "led.h"
#include "msp430common.h"
//#include "hydrologytask.h"

static unsigned int s_ClockTicks  = 0;
static unsigned int s_reset_count = 0;

unsigned int sys_errorcode_runcode = 100;  // ���� sys_errorcode_runcodeΪȫ�ֱ�����ȡֵ1-69��
					   // ��ֵ��Ϊ100����ֹ����   LSHB 20200506

void Timer_Zero() {
	s_ClockTicks = 0;
}
int Timer_Passed_Mins(int _min) {
	if (_min <= s_ClockTicks)
		return 1;
	else
		return 0;
}

/* Timer0_A5*/
void Timer0_A0_Init(unsigned int ccr0)	// ϵͳ����״̬�ʹ���ָʾ�ƿ��ƣ�ccr0=45
					// ϵͳ������˸Ƶ�ʣ�ccr0=15
					// ϵͳͨ�ŷ���������˸Ƶ�ʣ�ccr0=30
					// ϵͳͨ�Ž���������˸Ƶ�ʡ���˸ʱ��һ��3��
					// LSHB 20200506
{
	/*5438 TA0CTL*/
	TA0CTL |= TACLR;      //�������
	TA0CTL |= TASSEL0;    //ѡ�� ACLKʱ��
	TA0CTL |= ID1 + ID0;  // 1/8��Ƶ
	TA0CTL |= MC0;	      // ������ ģʽ
	/*5438 TA1CCR0*/
	TA0CCR0 = ccr0;
	/*2418 TACCTL0 5438 TA0CCTL0*/
	//    TA0CCTL0 = CCIE;	 //
	//    ͨ��Timer0_A0�ж�ר�ú����򿪺͹ر�Timer0_A0�жϣ��˴����ܴ��ж�
}

/*time1 a3*/
//ע�� ACLK ʹ���� 8��Ƶ,
void Timer1_A0_Init(unsigned int ccr0) {
	/*2418 TACTL 5438 TA1CTL*/
	TA1CTL |= TACLR;      //�������
	TA1CTL |= TASSEL0;    //ѡ�� ACLKʱ��
	TA1CTL |= ID1 + ID0;  // 1/8��Ƶ
	TA1CTL |= MC0;	      // ������ ģʽ
	// 1���� �ж�һ��
	/*2418 TACCR0 5438 TA1CCR0*/
	TA1CCR0 = ccr0;	   //  (32768HZ / (8 * 8) ) * 60s =30720
			   /*2418 TACCTL0 5438 TA1CCTL0*/
	TA1CCTL0 |= CCIE;  // CCR0 �ж�����
			   /*2418 TACTL 5438 TA1CTL*/
}

void WatchDog_Init() {
	//������������.
	//ʹ��ACLK 32K/8 ��Ƶ
	/*2418 WDTSSEL 5438 WDTSSEL0*/
	WDTCTL = WDTPW + WDTSSEL0;
}

void WatchDog_Clear() {
	/*2418 WDTSSEL 5438 WDTSSEL0*/
	WDTCTL = WDTPW + WDTSSEL0 + WDTCNTCL;
}

/* Timer0_B7*/
void TimerB_Init(unsigned int ccr0) {
	/*2418 TBCTL 5438 TB0CTL*/
	TB0CTL = TBCLR;
	TB0CTL |= TBSSEL0;  //ѡ��ACLKʱ��
	TB0CTL |= ID0;	    // 1/8��Ƶ
	TB0CTL |= MC1;	    //   ��������ģʽ
	// TBCTL |= TBIE;       //ʹ��TBIFG�ж�
	// 4���� �ж�һ�� ,(��2��)
	// TBCCR0=ccr0; // (32768HZ /(8*8) ) * 240 / 2 = 61440

	TBCCR0 = 61450;	 //��TIMERB0�ж�ָʾ����״̬��ϵͳ����״̬61450�Ǹ���ʼֵ��û��������
			 // LSHB 20200506

	TBCCR1 = 61440;
	//    TBCCR2=10240;
	//    TBCCR2=4096;//�����幷�� �ж�. 2s�ж�1��
	//    TBCCR2=2048;
	TBCCR2 = 1024;	//�����幷�� �ж�.
	/*2418 TBCCTL1 TBCCTL2 5438 TB0CCTL1 TB0CCTL2*/

	//   TB0CCTL0=CCIE;//��������TIMERB0�ж�  LSHB 20191004

	TB0CCTL1 |= CCIE;
	TB0CCTL2 |= CCIE;

	/*5438 P86 systemdebug*/
	P8DIR |= BIT6;
	P8OUT |= BIT6;
}

void TimerB_Clear() {
	s_reset_count = 0;
}

void Timer0_A0_Enable()	 // ����Timer0_A0�ж� LSHB 20200506
{
	TA0CCTL0 |= CCIE;
}

void Timer0_A0_Clear()	// ���Timer0_A0�ж� LSHB 20200506
{
	TA0CCTL0 &= ~CCIE;
}

void TimerB0_Enable()  // ����TIMERB0�ж� LSHB 20200506
{
	TB0CCTL0 = CCIE;
}

void TimerB0_Clear()  // ���TIMERB0�ж� LSHB 20200506
{
	TB0CCTL0 = ~CCIE;
}

#pragma vector = TIMER0_A0_VECTOR
__interrupt void TIMER0_A0_ISR(void) {

	//   sys_error_status(sys_errorcode_runcode);
	//   //����ϵͳ����͹���״ָ̬ʾ�ƿ��� LSHB   20200506
}

/*   TIMERA0_VECTOR   *   TIMER1_A0_VECTOR*/
#pragma vector = TIMER1_A0_VECTOR
__interrupt void TIMERA0_ISR(void) {
	//����CPU,���CPU���������ѵ�,���ᵼ�»���
	// LPM3_EXIT;
	// LPM2_EXIT;
	// Led1_On();
	// System_Delayms(75);
	// Led1_Off();
	++s_ClockTicks;	 //����һ�εδ�
	// HydrologyTimeBase();
	TimerB_Clear();
	WatchDog_Clear();
}

// 30���ӵĿ��Ź�
//#pragma vector = TIMERB0_VECTOR
#pragma vector = TIMERB0_VECTOR
__interrupt void TIMERB0_ISR(void) {
	// �������Ժ���
}

//#pragma vector=TIMERB1_VECTOR
#pragma vector = TIMERB1_VECTOR
__interrupt void TIMERB1_ISR(void) {
	//����Ҫ��һ��TBIV�����ж�
	unsigned int _tbiv = TBIV;
	switch (_tbiv) {
	case 2:
		//�������һ��
		//�����������1000
		// ++s_reset_count;
		// if (s_reset_count > 60) {  //����30���Ӿ�����
		// 	System_Reset();
		// }
		TBCCR1 += 61440;
		break;
	case 4:
		WatchDog_Clear();
		TBCCR2 += 1024;
		Clear_ExternWatchdog();
		break;
	case 10:
		break;
	default:
		break;
	}
}
