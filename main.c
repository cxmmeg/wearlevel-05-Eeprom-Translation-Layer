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
					     - 1);  //����_effect_count
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
	GPIO_selectInterruptEdge(GPIO_PORT_P2, GPIO_PIN0,
				 GPIO_HIGH_TO_LOW_TRANSITION);
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
	Led_OffAll();

	//    P9DIR |= BIT7;              //ly ����232оƬ��������
	//    P9OUT |= BIT7;

	Timer0_A0_Init(1024);  //ϵͳ����״ָ̬ʾ�ƿ���   LSHB 20200506
	Timer1_A0_Init(30720);	// ϵͳ��ʱ����ʼ��
	TimerB_Init(61440);

	Store_Init();	 // ��ʼ��ROM
	Sampler_Init();	 //�˿ڳ�ʼ��,��Ҫ�ȳ�ʼ��Store

	P10DIR |= BIT0;	 // ly
			 // P100���ߣ�uart3���ڵ��ԣ��͵Ļ�P104��,105����485��
	P10OUT |= BIT0;

	P10DIR |= BIT1;	 // ly P101���ߣ�uart1
			 // P104��,105���͵Ļ�����485��,�ߵĻ���������
	P10OUT &= ~BIT1;
	Is_RS485_1 =
		true;  //���ڰ�uart1��ʱֱ�Ӷ���Ϊ485-1���Ժ�ʹ������˫����ʱ�ٸ�
		       // LSHB 20200506
	P11DIR |=
		BIT2;  //����ָʾ�ƿ���P11.2��1��ϵͳ����������0��ϵͳ�����������ָʾ����˸

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

int Restart_GSMInit() {
	GSM_Open();
	if (GSM_CheckOK() < 0) {
		System_Delayms(1000);
		GSM_Open();
		if (GSM_CheckOK() < 0) {
			System_Delayms(1000);
			GSM_Open();
			if (GSM_CheckOK() < 0) {  //�޷������ͷ�����.
				GSM_Close(1);
				return -1;
			}
		}
	}

	char _phone[ 12 ];
	char _data[ 30 ];  // 30�㹻��

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
	GSM_Process(1, 0);     // GSM ������
	GSM_Close(0);	       //�ر� GSM
	System_Delayms(2000);  //����ػ�����쿪������ʧ��.
	return 0;
}

int Restart_DTUInit() {
	char _data[ UART3_MAXBUFFLEN ];
	int  _dataLen = 0;
	int  _repeats = 0;
	int  _out     = 0;
	//
	//��������һ��DTU�ȴ��������õĹ���.
	//
	if (trace_open == 0) {
		//���û�򿪵��ԵĻ�,������Ҫ����򿪵�
		// Console_Open();
	}
	UART3_ClearBuffer();
	// Console_WriteStringln("ACK");

	if (UART3_RecvLineLongWait(_data, UART3_MAXBUFFLEN, &_dataLen) == 0) {
		//����ȵ���������, �ͽ�������״̬.�ȴ�15����
		// Console_WriteStringln("waiting for 15 seconds .");
		if (Main_ProcCommand(_data, _dataLen, NULL)
		    == 3) {  //������һ��SYN,�ͽ�������״̬
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
				    == 2) {  //��ʾ�˳�����״̬
					break;
				}
			}
		}
	}
	//�����ڵ���״̬��ʱ��Ҫ�رյ�
	if (trace_open == 0) {	//��������ǹرյ�,���ڵ���Ҫ�ر�
				// Console_Close();
	}

	return 0;
}

int WorkMode_Init(char* ptype) {
	char _curType, _selType;
	int  _ret;

#if 0
    //��ȡѡ����״̬    DTU/GSM   232 
    P3DIR &= ~BIT3;   //P33Ϊ����
    P3DIR &= ~BIT2;   //P32Ϊ����
    if(P3IN & BIT2)
    {//P32Ϊ��
        _selType='D';
    }
    else
    {
        _selType='G';
    }
#endif
	_selType = 'S';	 // GPRSģʽ GTM900
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
		//�����������.������

		TraceMsg("Bad Mode !", 1);
		System_Reset();
		break;
	}

	//�жϵ�ǰ����״̬,   DTU/GSM
	if (Store_ReadSystemType(&_curType)
	    < 0) {  //����޷����� �����ģʽ,�Ͳ��ж��Ƿ� ����������.
		_ret = 0;
	}
	else {
		_ret = _selType != _curType ? 1 : 0;
	}

	*ptype = _selType;

	return _ret;
}

void Main_Init() {  //ʱ��ʹ��8M
	//���ж�,�Է�֮ǰ�����д������жϱ��ر�
	EnInt();
	TimerB_Clear();
	WatchDog_Clear();  // �����λ������
	//�����ٴ������������  ���ӿɿ���.

	TraceOpen();
	Sampler_Init();

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
	Sampler_Close();
	TimerB_Clear();
	WatchDog_Clear();
	// TraceMsg("Device sleep !",1); //�˴������Ѿ��ر�
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
	/* msp430�ͷ�RTC_INT���ŵĿ���Ȩ����ʱRTC_INT���Ϊ�ߵ�ƽ���߸���̬���ü̵����ɿ������ӵ���*/
	GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P2, GPIO_PIN0);
	System_Delayms(1000);
	// P2DIR |= BIT0;
	// P2OUT |= BIT0;
	return 0;
}
