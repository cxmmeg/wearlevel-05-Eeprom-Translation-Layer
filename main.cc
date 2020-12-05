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

extern bool Is_uart3_RX_INT;  //增加 LSHB200508
int	    Opened = 1;	      // Opened控制打印信息开关 LSHB200510

int IsDebug = 1;

bool Is_RS485_1 = false, Is_bluetooth = false,
     Is_uart1_RX_INT =
	     false;  //工作指示灯控制，三种情况(1)当前UART1是否是485，（2）当前UART1是否是蓝牙，（3）是否发生了uart1中断
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
	UpdateDebugState();  // p2.4为低电平时，IsDebug=1;

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
#pragma vector = PORT2_VECTOR  // P20 用于外部时钟唤醒中断
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

	P1SEL = 0x00;  //关闭脉冲端口中断
	P1DIR = 0x00;
	P1IE  = 0x00;
	Clock_Init();  // CPU时钟初始化

	_EINT();  //打开中断
	// IE1|=OFIE+NMIIE+ACCVIE;
	// //振荡器故障中断允许、NMI中断允许、FLASH存储器非法访问中断允许
	SFRIE1 |= OFIE + NMIIE + ACCVIE;
	WatchDog_Init();
	Led_Init();  // 指示灯 初始化
	// Led_OffAll();

	//    P9DIR |= BIT7;              //ly 拉高232芯片控制引脚
	//    P9OUT |= BIT7;

	// Timer0_A0_Init(1024);  //系统运行状态指示灯控制   LSHB 20200506
	// Timer1_A0_Init(30720);	// 系统定时器初始化
	TimerB_Init(61440);

	// Store_Init();	 // 初始化ROM
	// Sampler_Init();	 //端口初始化,需要先初始化Store

	P10DIR |= BIT0;	 // ly
			 // P100拉高，uart3用于调试，低的话P104，,105就是485口
	P10OUT |= BIT0;

	P10DIR |= BIT1;	 // ly P101拉高，uart1
			 // P104，,105，低的话就是485口,高的话就是蓝牙
	P10OUT &= ~BIT1;
	Is_RS485_1 = true;  //现在把uart1暂时直接定义为485-1，以后使用蓝牙双功能时再改
			    // LSHB 20200506
	P11DIR |= BIT2;	 //工作指示灯控制P11.2，1：系统工作正常，0：系统发生错误，相关指示灯闪烁

	P8DIR |= BIT6;	//初始化硬件看门狗   LSHB 20200510
	P8OUT |= BIT6;
	P9OUT &= ~BIT3;	 //关闭24V电源

	Led1_WorkLightOff();  //为了节能关闭工作指示灯

	Console_Open();

	/*wyq  调试485去掉联网的步骤*/
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

void Main_Init() {  //时钟使用8M
	//开中断,以防之前部分有错误导致中断被关闭
	EnInt();
	TimerB_Clear();
	WatchDog_Clear();  // 清除复位计数器
	//在里再次运行这个函数  增加可靠性.

	TraceOpen();
	// Sampler_Init();

	/* 上电后必须马上写rtc的control寄存器，让意外掉电的时候RTC_INT引脚输出方波*/
	RTC_Open();
	_RTC_Write_OneByte(RegAddr_Control, 1 << 6);
}

void Main_GotoSleep() {
	//开中断,以防之前部分有错误导致中断被关闭
	EnInt();

	TraceMsg("Device is going to sleep !", 1);
	//关闭外围,进入休眠
	// RTC_Close();
	Console_Close();
	//    DTU_Close();
	//    GSM_Close(1);
	//    call gprs close
	// Sampler_Close();
	TimerB_Clear();
	WatchDog_Clear();
	// TraceMsg("Device sleep !",1); //此处串口已经关闭
	LPM2;
	//    LPM3;
}
