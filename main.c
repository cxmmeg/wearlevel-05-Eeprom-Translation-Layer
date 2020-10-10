#include "main.h"
#include "DTU.h"
#include "Sampler.h"
#include "common.h"
#include "console.h"
#include "led.h"
#include "msp430common.h"
#include "rtc.h"
#include "store.h"
#include "timer.h"
// #include "GSM.h"
#include "uart0.h"
#include "uart1.h"
#include "uart2.h"
#include "uart3.h"
#include "wifi_config.h"
//#include "flow.h"
//#include "hydrology.h"
#include "reportbinding.h"
#include <string.h>
// #include "GTM900C.h"
#include "BC95.h"
#include "blueTooth.h"
#include "driverlib.h"
#include "hydrologycommand.h"
#include "hydrologytask.h"
#include "ioDev.h"
#include "lowpower.h"
#include "rom.h"
#include "uart_config.h"
#include <stdlib.h>

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
int IsSame(const char* data1, const char* data2, size_t len) {
	for (size_t i = 0; i < len - 1; ++i)
		if (data1[ i + 1 ] != data2[ i ]) {
			printf("index : %d, data1 : %c, data2 : %c\r\n", i,
			       data1[ i ], data2[ i ]);
			return 0;
		}
	return 1;
}

extern int DataPacketLen;

void CircleQueueTest() {
	DataPacketLen				      = HYDROLOGY_DATA_ITEM_LEN;
	char test_data[ HYDROLOGY_DATA_ITEM_LEN + 1 ] = { 0 };
	for (int i = 0; i < HYDROLOGY_DATA_ITEM_LEN; ++i)
		test_data[ i ] = 'a' + rand() % 26;

	const int try_times = 3600;
	for (int i = 0; i < try_times; ++i) {
		if (Store_WriteDataItemAuto(test_data) < 0)
			printf("Store_WriteDataItemAuto falied at %d times\r\n",
			       i);

		int package_cnt = 0;
		int _endIdx = 0, _startIdx = 0;
		Hydrology_ReadEndIdx(&_endIdx);
		Hydrology_ReadStartIdx(&_startIdx);
		Hydrology_ReadDataPacketCount(&package_cnt);
		printf("cur cnt : %d, startidx : %d, endidx : %d\r\n",
		       package_cnt, _startIdx, _endIdx);
	}

	char read_buff[ HYDROLOGY_DATA_ITEM_LEN + 1 ] = { 0 };

	int package_cnt = 0;
	Hydrology_ReadDataPacketCount(&package_cnt);
	printf("package cnt : %d, DataPacketLen : %d\r\n", package_cnt,
	       DataPacketLen);
	while (package_cnt > 0) {
		Hydrology_ReadDataPacketCount(&package_cnt);
		printf("package cnt : %d, DataPacketLen : %d\r\n", package_cnt,
		       DataPacketLen);
		//       for (int i = 0; i < try_times; ++i) {
		memset(read_buff, 0, HYDROLOGY_DATA_ITEM_LEN);
		int start_idx, end_idx;
		if (FlowCheckSampleData(&start_idx, &end_idx) != 0) {
			printf("FlowCheckSampleData failed at %d "
			       "times\r\n",
			       package_cnt);
			while (1)
				;
		}
		printf("start_idx: %d, end_idx:%d \r\n", start_idx, end_idx);

		int read_len = Store_ReadDataItem(start_idx, read_buff, 0);
		if (read_len < 0) {
			if (read_len == -3) {
				printf("bad block\r\n");
			}
			else {
				printf("Store_ReadDataItem failed at "
				       "%d "
				       "package_cnt\r\n",
				       package_cnt);
			}
			continue;
			// while (1)
			// 	;
		}

		Hydrology_SetDataPacketCount(package_cnt
					     - 1);  //更新_effect_count
		if (start_idx >= HYDROLOGY_DATA_MAX_IDX) {
			start_idx = HYDROLOGY_DATA_MIN_IDX;
		}
		else {
			++start_idx;
		}
		Hydrology_SetStartIdx(start_idx);

		if (!IsSame(test_data, read_buff, HYDROLOGY_DATA_ITEM_LEN)) {
			printf("write:\r\n%s\r\n", test_data);
			printf("read:\r\n%s\r\n", read_buff);
			printf("read not the same at %d package left\r\n",
			       package_cnt);
			while (1)
				;
		}
	}
	printf("CircleQueueTest done, no exception\r\n");
	while (1)
		;
}

int main(void) {
	BoardPowerOn();
	Restart_Init();
	Main_Init();

	/*  500ms */
	Led_Round();
	Select_Debug_Mode(0);
	TraceMsg("Device Open !", 1);
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
	GPIO_selectInterruptEdge(GPIO_PORT_P2, GPIO_PIN0,
				 GPIO_HIGH_TO_LOW_TRANSITION);
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
	Led_OffAll();

	//    P9DIR |= BIT7;              //ly 拉高232芯片控制引脚
	//    P9OUT |= BIT7;

	Timer0_A0_Init(1024);  //系统运行状态指示灯控制   LSHB 20200506
	Timer1_A0_Init(30720);	// 系统定时器初始化
	TimerB_Init(61440);

	Store_Init();	 // 初始化ROM
	Sampler_Init();	 //端口初始化,需要先初始化Store

	P10DIR |= BIT0;	 // ly
			 // P100拉高，uart3用于调试，低的话P104，,105就是485口
	P10OUT |= BIT0;

	P10DIR |= BIT1;	 // ly P101拉高，uart1
			 // P104，,105，低的话就是485口,高的话就是蓝牙
	P10OUT &= ~BIT1;
	Is_RS485_1 =
		true;  //现在把uart1暂时直接定义为485-1，以后使用蓝牙双功能时再改
		       // LSHB 20200506
	P11DIR |=
		BIT2;  //工作指示灯控制P11.2，1：系统工作正常，0：系统发生错误，相关指示灯闪烁

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

int Restart_GSMInit() {
	GSM_Open();
	if (GSM_CheckOK() < 0) {
		System_Delayms(1000);
		GSM_Open();
		if (GSM_CheckOK() < 0) {
			System_Delayms(1000);
			GSM_Open();
			if (GSM_CheckOK() < 0) {  //无法工作就放弃了.
				GSM_Close(1);
				return -1;
			}
		}
	}

	char _phone[ 12 ];
	char _data[ 30 ];  // 30足够了

	_data[ 0 ] = '$';
	if (Store_ReadDeviceNO(&_data[ 1 ]) < 0) {
		_data[ 1 ]  = '0';
		_data[ 2 ]  = '0';
		_data[ 3 ]  = '0';
		_data[ 4 ]  = '0';
		_data[ 5 ]  = '0';
		_data[ 6 ]  = '0';
		_data[ 7 ]  = '0';
		_data[ 8 ]  = '0';
		_data[ 9 ]  = '0';
		_data[ 10 ] = '0';
		_data[ 11 ] = '0';
	}

	Utility_Strncpy(&_data[ 12 ], "<restart#", 9);
	if (Store_GSM_ReadCenterPhone(5, _phone) == 0) {
		if (_phone[ 0 ] != '0' && _phone[ 1 ] != '0'
		    && _phone[ 2 ] != '0') {
			GSM_SendMsgTxt(_phone, _data, 21);
		}
	}
	GSM_Process(1, 0);     // GSM 主流程
	GSM_Close(0);	       //关闭 GSM
	System_Delayms(2000);  //避免关机后过快开机导致失败.
	return 0;
}

int Restart_DTUInit() {
	char _data[ UART3_MAXBUFFLEN ];
	int  _dataLen = 0;
	int  _repeats = 0;
	int  _out     = 0;
	//
	//这里增加一个DTU等待串口配置的过程.
	//
	if (trace_open == 0) {
		//如果没打开调试的话,串口是要额外打开的
		// Console_Open();
	}
	UART3_ClearBuffer();
	// Console_WriteStringln("ACK");

	if (UART3_RecvLineLongWait(_data, UART3_MAXBUFFLEN, &_dataLen) == 0) {
		//如果等到串口数据, 就进入配置状态.等待15秒钟
		// Console_WriteStringln("waiting for 15 seconds .");
		if (Main_ProcCommand(_data, _dataLen, NULL)
		    == 3) {  //返回了一个SYN,就进入配置状态
			while (1) {
				_repeats = 0;
				while (UART3_RecvLineLongWait(_data,
							      UART3_MAXBUFFLEN,
							      &_dataLen)
				       < 0) {
					++_repeats;
					if (_repeats > 4) {
						_out = 1;
						break;
					}
				}
				if (_out != 0)
					break;
				if (Main_ProcCommand(_data, _dataLen,
						     NULL)
				    == 2) {  //表示退出配置状态
					break;
				}
			}
		}
	}
	//当不在调试状态的时候要关闭的
	if (trace_open == 0) {	//如果调试是关闭的,串口到此要关闭
				// Console_Close();
	}

	return 0;
}

int WorkMode_Init(char* ptype) {
	char _curType, _selType;
	int  _ret;

#if 0
    //获取选择工作状态    DTU/GSM   232 
    P3DIR &= ~BIT3;   //P33为输入
    P3DIR &= ~BIT2;   //P32为输入
    if(P3IN & BIT2)
    {//P32为高
        _selType='D';
    }
    else
    {
        _selType='G';
    }
#endif
	_selType = 'S';	 // GPRS模式 GTM900
	switch (_selType) {
	case 'G':
		g_main_type = MAIN_TYPE_GSM;
		TraceMsg("Device is GSM Mode !", 1);
		break;
	case 'D':
		g_main_type = MAIN_TYPE_DTU;
		TraceMsg("Device is DTU Mode !", 1);
		break;

	case 'S':
		g_main_type = MAIN_TYPE_GTM900;
		TraceMsg("Device is GPRS Mode !", 1);
		break;

	default:
		//这里出现问题.就重启

		TraceMsg("Bad Mode !", 1);
		System_Reset();
		break;
	}

	//判断当前工作状态,   DTU/GSM
	if (Store_ReadSystemType(&_curType)
	    < 0) {  //如果无法读出 保存的模式,就不判断是否 重新设置了.
		_ret = 0;
	}
	else {
		_ret = _selType != _curType ? 1 : 0;
	}

	*ptype = _selType;

	return _ret;
}

void Main_Init() {  //时钟使用8M
	//开中断,以防之前部分有错误导致中断被关闭
	EnInt();
	TimerB_Clear();
	WatchDog_Clear();  // 清除复位计数器
	//在里再次运行这个函数  增加可靠性.

	TraceOpen();
	Sampler_Init();

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
	Sampler_Close();
	TimerB_Clear();
	WatchDog_Clear();
	// TraceMsg("Device sleep !",1); //此处串口已经关闭
	LPM2;
	//    LPM3;
}

/*
  intervalType : INTERVAL_HOUR , INTERVAL_MIN
  return : 0(success)  else(failed)
*/
int setWakeupInterval(const char interval, const char intervalType) {
	clearAlarmAFlag();
	char nextHour, nextMin;
	char currHour = _BCDtoHEX(_RTC_Read_OneByte(RegAddr_Hour));
	char currMin  = _BCDtoHEX(_RTC_Read_OneByte(RegAddr_Min));
	if (currHour < 0 || currHour >= 24) {
		printf("currHour error : %d:%d", currHour, currMin);
		return -1;
	}

	if (currMin < 0 || currMin >= 60) {
		printf("currMin error : %d:%d", currHour, currMin);
		return -2;
	}

	if (intervalType == INTERVAL_HOUR) {
		if (interval < 0 || interval >= 24) {
			printf("interval error : %d \n", interval);
			return -3;
		}
		char carry = currMin >= 59 ? 1 : 0;
		nextHour = (((currHour + carry) / interval + 1) * interval - 1)
			   % 24;
		nextMin = 59;
	}
	else {
		if (interval < 0 || interval >= 60) {
			printf("interval error : %d \n", interval);
			return -4;
		}
		char carry = (currMin + 1) % interval == 0 ? 1 : 0;
		nextMin	   = (((currMin + carry) / interval + 1) * interval - 1)
			  % 60;
		nextHour =
			(currHour
			 + ((((currMin + carry) / interval + 1) * interval - 1)
			    / 60))
			% 24;
	}

	RTC_SetAlarm(nextHour, nextMin, 0);

	// // Enable P2.0 internal resistance as pull-Up resistance
	// // P2.0 interrupt enabled
	// GPIO_enableInterrupt(GPIO_PORT_P2, GPIO_PIN0);
	// // P2.0 Hi/Lo edge
	// GPIO_selectInterruptEdge(GPIO_PORT_P2, GPIO_PIN0,
	// 			 GPIO_HIGH_TO_LOW_TRANSITION);
	// // P2.0 IFG cleared
	// GPIO_clearInterrupt(GPIO_PORT_P2, GPIO_PIN0);

	printf("next wakeup time: %d:%d:00 \r\n", nextHour, nextMin);
	/* msp430释放RTC_INT引脚的控制权，此时RTC_INT会变为高电平或者高阻态，让继电器松开，板子掉电*/
	GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P2, GPIO_PIN0);
	System_Delayms(1000);
	// P2DIR |= BIT0;
	// P2OUT |= BIT0;
	return 0;
}
