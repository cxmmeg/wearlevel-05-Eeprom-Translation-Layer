#include "main.h"
#include "common.h"
#include "console.h"
#include "datapage.h"
#include "driverlib.h"
#include "duallru.h"
#include "dualpool.h"
#include "etltest.h"
#include "led.h"
#include "lowpower.h"
#include "lru.h"
#include "mathtool.h"
#include "msp430common.h"
#include "pagetable.h"
#include "rom.h"
#include "rtc.h"
#include "timer.h"
#include "tool.h"
#include "uart0.h"
#include "uart1.h"
#include "uart3.h"
#include "uart_config.h"
#include <list>
#include <queue>
#include <set>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <vector>

#define DISABLE_WATCHDOG

extern bool Is_uart3_RX_INT;  //���� LSHB200508
int	    Opened = 1;	      // Opened���ƴ�ӡ��Ϣ���� LSHB200510

int IsDebug = 1;

bool Is_RS485_1 = false, Is_bluetooth = false,
     Is_uart1_RX_INT =
	     false;  //����ָʾ�ƿ��ƣ��������(1)��ǰUART1�Ƿ���485����2����ǰUART1�Ƿ�����������3���Ƿ�����uart1�ж�
		     // LSHB 20200506
void UpdateDebugState() {
	GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P2, GPIO_PIN4);
	if (GPIO_getInputPinValue(GPIO_PORT_P2, GPIO_PIN4) == 0) {
		printf("IsDebug : 1\r\n");
		IsDebug = 1;
	}
	else {
		printf("IsDebug : 0\r\n");
		IsDebug = 0;
	}
}

int main(void) {
	BoardPowerOn();
	Restart_Init();
	Main_Init();

	Led_Round();
	Max3222_Open();
	TraceMsg("Device Open !", 1);
	UpdateDebugState();  // p2.4Ϊ�͵�ƽʱ��IsDebug=1;

	Time time;
	time.GetRtcTime();
	LOG_INFO("start time : \r\n");
	time.Show();

	priority_queue< int > min_heap;
	min_heap.push(3);
	min_heap.push(2);
	min_heap.push(1);
	LOG_INFO("%d \r\n\r\n", min_heap.top());

	/* Tool Test */
	// BitOperationTest();
	// TestLRU();
	// TestDualLRU();
	// TestPageTable();

	/* ETL Test */
	// TestETLWriteByte();
	// ETLWriteDataPage();
	// ETLWriteAndReadTest();
	// ETLFullWriteAndReadFullTest();
	// DualPoolTeste();
	// TestSingleHotPage(1000);
	// TestMultiHotPage();
	// TestHotPageToColdPage(1500);

	time.GetRtcTime();
	LOG_INFO("end time : \r\n");
	time.Show();

	while (1) {
		System_Delayms(1000);
		WatchDog_Clear();
	}
}

#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector = PORT2_VECTOR  // P20 �����ⲿʱ�ӻ����ж�
__interrupt
#elif defined(__GNUC__)
__attribute__((interrupt(PORT2_VECTOR)))
#endif
	void
	Port_2(void) {
	if (GPIO_getInterruptStatus(GPIO_PORT_P2, GPIO_PIN0) != 0) {
		// P2.0 IFG cleared
		GPIO_clearInterrupt(GPIO_PORT_P2, GPIO_PIN0);
		// wakeup cpu
		__bic_SR_register_on_exit(SCG1 + SCG0 + OSCOFF + CPUOFF);

		printf("wake up!\r\n");
	}
}
void rtcIntPinInit() {
	// Enable P2.0 internal resistance as pull-Up resistance
	GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P2, GPIO_PIN2);
	// P2.0 interrupt enabled
	GPIO_enableInterrupt(GPIO_PORT_P2, GPIO_PIN0);
	// P2.0 Hi/Lo edge
	GPIO_selectInterruptEdge(GPIO_PORT_P2, GPIO_PIN0, GPIO_HIGH_TO_LOW_TRANSITION);
	// P2.0 IFG cleared
	GPIO_clearInterrupt(GPIO_PORT_P2, GPIO_PIN0);
}

void Restart_Init() {

	P1SEL = 0x00;  //�ر�����˿��ж�
	P1DIR = 0x00;
	P1IE  = 0x00;
	Clock_Init();  // CPUʱ�ӳ�ʼ��

	_EINT();  //���ж�
	// IE1|=OFIE+NMIIE+ACCVIE;
	// //���������ж�����NMI�ж�����FLASH�洢���Ƿ������ж�����
	SFRIE1 |= OFIE + NMIIE + ACCVIE;
	WatchDog_Init();
	Led_Init();  // ָʾ�� ��ʼ��
	// Led_OffAll();

	//    P9DIR |= BIT7;              //ly ����232оƬ��������
	//    P9OUT |= BIT7;

	// Timer0_A0_Init(1024);  //ϵͳ����״ָ̬ʾ�ƿ���   LSHB 20200506
	// Timer1_A0_Init(30720);	// ϵͳ��ʱ����ʼ��
	TimerB_Init(61440);

	// Store_Init();	 // ��ʼ��ROM
	// Sampler_Init();	 //�˿ڳ�ʼ��,��Ҫ�ȳ�ʼ��Store

	P10DIR |= BIT0;	 // ly
			 // P100���ߣ�uart3���ڵ��ԣ��͵Ļ�P104��,105����485��
	P10OUT |= BIT0;

	P10DIR |= BIT1;	 // ly P101���ߣ�uart1
			 // P104��,105���͵Ļ�����485��,�ߵĻ���������
	P10OUT &= ~BIT1;
	Is_RS485_1 = true;  //���ڰ�uart1��ʱֱ�Ӷ���Ϊ485-1���Ժ�ʹ������˫����ʱ�ٸ�
			    // LSHB 20200506
	P11DIR |= BIT2;	 //����ָʾ�ƿ���P11.2��1��ϵͳ����������0��ϵͳ�����������ָʾ����˸

	P8DIR |= BIT6;	//��ʼ��Ӳ�����Ź�   LSHB 20200510
	P8OUT |= BIT6;
	P9OUT &= ~BIT3;	 //�ر�24V��Դ

	Led1_WorkLightOff();  //Ϊ�˽��ܹرչ���ָʾ��

	Console_Open();

	/*wyq  ����485ȥ�������Ĳ���*/
	//     P3DIR &= ~BIT2;
	//    if(P3IN & BIT2)
	//    {
	//        IsDebug = 1;
	//    }
	//    else
	//    {
	//        IsDebug = 0;
	//    }

	return;
}

void Main_Init() {  //ʱ��ʹ��8M
	//���ж�,�Է�֮ǰ�����д������жϱ��ر�
	EnInt();
	TimerB_Clear();
	WatchDog_Clear();  // �����λ������
	//�����ٴ������������  ���ӿɿ���.

	TraceOpen();
	// Sampler_Init();

	/* �ϵ���������дrtc��control�Ĵ���������������ʱ��RTC_INT�����������*/
	RTC_Open();
	_RTC_Write_OneByte(RegAddr_Control, 1 << 6);
}

void Main_GotoSleep() {
	//���ж�,�Է�֮ǰ�����д������жϱ��ر�
	EnInt();

	TraceMsg("Device is going to sleep !", 1);
	//�ر���Χ,��������
	// RTC_Close();
	Console_Close();
	//    DTU_Close();
	//    GSM_Close(1);
	//    call gprs close
	// Sampler_Close();
	TimerB_Clear();
	WatchDog_Clear();
	// TraceMsg("Device sleep !",1); //�˴������Ѿ��ر�
	LPM2;
	//    LPM3;
}
