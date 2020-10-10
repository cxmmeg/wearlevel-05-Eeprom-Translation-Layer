
//#include "adc.h"
#include "common.h"
//#include "ioDev.h"
#include "led.h"
#include "main.h"
//#include "message.h"
#include "msp430common.h"
#include "string.h"
#include "uart1.h"
#include "uart3.h"

char	    isUARTConfig = 0;
extern bool sys_errorcode_runcode, Is_uart3_RX_INT;  // LSHB 20200506

int Hydrology_ProcessUARTReceieve() {
	char buffer[ 300 ];
	int  count = 0;

	memset(buffer, 0, 300);

	UART3_RecvLineTry(buffer, UART3_MAXBUFFLEN, &count);
	if (count != 0) {
		TraceHexMsg(buffer, count);
		isUARTConfig = 1;
		//hydrologyProcessReceieve(buffer, count);
		// uart3 �н����жϣ�led8����0.5s��
		if (Is_uart3_RX_INT == true) {
			//ͨ��Timer0_A0�ж���ʾ������Ϣ���� LSHB 20200506
			sys_errorcode_runcode = 69;
			//Led_OffAll();
			//led_flash_init(75);
			//Timer0_A0_Enable();  // Timer0_A0���ж�
			System_Delayms(500);
			//Timer0_A0_Clear();  // Timer0_A0���ж�
			//Led_OffAll();
			Is_uart3_RX_INT = false;
			// end  LSHB 20200506
		}
	}

	return 0;
}



void Hydrology_InitWaitConfig() {
	/* ֻ�е���״̬�²���Ҫ�ȴ�����  
	if (!IsDebug)
		return;

	//BleDriverInstall();
	//PT_IODev ptDevBle = getIODev();
	//ptDevBle->bletaskstart();

	int trycount = 15000;

	TraceMsg("Device is waiting for configing within 10s", 1);
	//ptDevBle->sendMsg("waiting config ... ",
	//				sizeof("waiting config ... "));

	while (trycount--) {
		Hydrology_ProcessUARTReceieve();
		//Hydrology_ProcessBLEReceieve();
		System_Delayms(1);
	}

	ptDevBle->sendMsg("config finished",
					sizeof("config finished"));
	//ble_waiting_query();

	ptDevBle->bletaskend();
*/
}