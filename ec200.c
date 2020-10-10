#include "ec200.h"
#include "common.h"
#include "console.h"
#include "hydrologycommand.h"
#include "message.h"
#include "uart0.h"
#include "uart1.h"
#include "uart3.h"
#include <msp430x54x.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define AT_ATE0 "ATE0"	     //关闭回显
#define AT_CFUN "AT+CFUN=1"  //设置全功能
#define AT_CIMI "AT+CIMI"    //查询SIM卡id
#define AT_CERG "AT+CREG?"
#define AT_CSQ "AT+CSQ"	     //查询信号
#define AT_CGATT "AT+CGATT=1"

#define AT_SLEEP "AT+QSCLK=1"	//开启休眠
#define AT_WAKEUP "AT+QSCLK=0"	//关闭休眠

// #define AT_QICSGP "AT+QICSGP=1,1,\"UNINET\",\"\",\"\",1"  //联通卡
// #define AT_QICSGP "AT+QICSGP=1,1,\"CTNET\",\"\",\"\",1"  //电信卡
#define AT_QICSGP "AT+QICSGP=1,1,\"CMNET\",\"\",\"\",1"	 //移动卡
#define AT_QIACT "AT+QIACT=1"				 //激活
#define AT_QIDEACT "AT+QIDEACT=1"			 //退出激活
#define AT_QICLOSE "AT+QICLOSE=0"			 //断开TCP连接
#define AT_SHUTDOWN "AT+QPOWD"
#define EXIT_TT_MODE "+++"  //退出透传

#define TIMEWAIT 300  // ms
#define SEND_TIMEWAIT 3000
#define REPEAT_TIME 3

static char centerIP[ 16 ];
static char centerPort[ 7 ];

static int connected = 0;  // 0 disconnect	1 connect
static int power     = 0;  // 0 poweroff    1 poweron

/*
**	底层函数
*/
static int  Ec200SendWithCR(char* data);
static int  Ec200SendWithLen(char* data, int len);
static int  Ec200Send(char* data);
static int  SetNoEcho();
static int  SetFun();
static int  QuerySIMId();
static int  QuerySIMSignal();
static int  SetCGATT();
static int  SleepMode();
static int  WakeMode();
static int  TransmissionConfiguration();
static int  TransmissionActivation();
static int  TransmissionDeactivation();
static int  CloseTran();
static int  ExitTTMode();
static void judgeintlen(int* ip, char* iplen, char len);
static char judgeportlen(char* port);
static int  OpenTran(long center);

static rtc_time_t get_time();

static int PowerOn();			   //上电
static int PowerOff();			   //掉电
static int TransmissionInit(long center);  //进入透传模式
static int TransmissionEnd();		   //退出透传模式

static int PowerOn() {	//上电
	TraceMsg("Ec200t Open !", 1);
	UART0_Open(UART0_GTM900_TYPE);

	P8DIR |= BIT1;	// P8.1 为 3.3
	P8OUT |= BIT1;
	System_Delayms(100);

	P8DIR |= BIT2;	// P8.2 为 POWERKEY,给置高再置低
	P8OUT &= ~BIT2;
	System_Delayms(1000);
	P8OUT |= BIT2;
	System_Delayms(1000);
	P8OUT &= ~BIT2;

	System_Delayms(3000);
}

static int PowerOff() {	 //下电
	TraceMsg("Ec200t Close !", 1);

	P8DIR |= BIT1;	// P8.1 为 3.3
	P8OUT |= BIT1;
	System_Delayms(100);

	P8DIR |= BIT2;	// P8.2 为 POWERKEY,给置高再置低
	P8OUT &= ~BIT2;
	System_Delayms(1000);
	P8OUT |= BIT2;
	System_Delayms(1000);
	P8OUT &= ~BIT2;

	System_Delayms(3000);

	return 0;
}

static int Ec200SendWithCR(char* data) {
	UART0_Send(data, Utility_Strlen(data), 1);
	return 0;
}

static int Ec200SendWithLen(char* data, int len) {
	UART0_Send(data, len, 0);
	return 0;
}

static int Ec200Send(char* data) {
	UART0_Send(data, Utility_Strlen(data), 0);
	return 0;
}

static int SetNoEcho() {

	int  _repeat		       = 0;
	int  _dataLen		       = 0;
	char _data[ UART0_MAXBUFFLEN ] = { 0 };

	Ec200SendWithCR(AT_ATE0);
	Console_WriteStringln(AT_ATE0);

	while (_repeat < REPEAT_TIME) {
		while (UART0_RecvLineWait(_data, UART0_MAXBUFFLEN, &_dataLen)
		       == 0) {
			Console_WriteStringln(_data);

			if (strstr(_data, "OK")) {
				return 1;
			}
			else if (strstr(_data, "ERROR")) {
				return 2;
			}
		}
		_repeat++;
		System_Delayms(TIMEWAIT);
	}

	return 0;
}

static int SetFun() {

	int  _repeat		       = 0;
	int  _dataLen		       = 0;
	char _data[ UART0_MAXBUFFLEN ] = { 0 };

	Ec200SendWithCR(AT_CFUN);
	Console_WriteStringln(AT_CFUN);

	while (_repeat < REPEAT_TIME) {
		while (UART0_RecvLineWait(_data, UART0_MAXBUFFLEN, &_dataLen)
		       == 0) {
			Console_WriteStringln(_data);

			if (strstr(_data, "OK")) {
				return 1;
			}
		}
		_repeat++;
		System_Delayms(TIMEWAIT);
	}

	return 0;
}

static int QuerySIMId() {

	int  _repeat		       = 0;
	int  _dataLen		       = 0;
	char _data[ UART0_MAXBUFFLEN ] = { 0 };

	Ec200SendWithCR(AT_CIMI);
	Console_WriteStringln(AT_CIMI);

	while (_repeat < REPEAT_TIME) {
		while (UART0_RecvLineWait(_data, UART0_MAXBUFFLEN, &_dataLen)
		       == 0) {
			Console_WriteStringln(_data);

			if (strstr(_data, "OK")) {
				return 1;
			}
		}
		_repeat++;
		System_Delayms(TIMEWAIT);
	}

	return 0;
}



static int QueryCerg() {

	int  _repeat		       = 0;
	int  _dataLen		       = 0;
	char _data[ UART0_MAXBUFFLEN ] = { 0 };

	Ec200SendWithCR(AT_CERG);
	Console_WriteStringln(AT_CERG);

	while (_repeat < REPEAT_TIME) {
		while (UART0_RecvLineWait(_data, UART0_MAXBUFFLEN, &_dataLen)
		       == 0) {
			Console_WriteStringln(_data);

			if (strstr(_data, ",1") || strstr(_data, ",5")) {
				return 1;
			}
		}
		_repeat++;
		System_Delayms(TIMEWAIT);
	}

	return 0;
}

static int QuerySIMSignal() {
	int  _repeat		       = 0;
	int  _dataLen		       = 0;
	char _data[ UART0_MAXBUFFLEN ] = { 0 };

	Ec200SendWithCR(AT_CSQ);
	Console_WriteStringln(AT_CSQ);

	while (_repeat < REPEAT_TIME) {
		while (UART0_RecvLineWait(_data, UART0_MAXBUFFLEN, &_dataLen)
		       == 0) {
			Console_WriteStringln(_data);

			if (!strstr(_data, "99,99") && !strstr(_data, "ERROR") && !strstr(_data, "OK")){
				return 1;
			}
		}
		_repeat++;
		System_Delayms(TIMEWAIT);
	}

	return 0;
}

static int SetCGATT() {

	int  _repeat		       = 0;
	int  _dataLen		       = 0;
	char _data[ UART0_MAXBUFFLEN ] = { 0 };

	Ec200SendWithCR(AT_CGATT);
	Console_WriteStringln(AT_CGATT);

	while (_repeat < REPEAT_TIME) {
		while (UART0_RecvLineWait(_data, UART0_MAXBUFFLEN, &_dataLen)
		       == 0) {
			Console_WriteStringln(_data);

			if (strstr(_data, "OK")) {
				return 1;
			}
		}
		_repeat++;
		System_Delayms(TIMEWAIT);
	}

	return 0;
}

static int SleepMode() {

	int  _repeat		       = 0;
	int  _dataLen		       = 0;
	char _data[ UART0_MAXBUFFLEN ] = { 0 };

	Ec200SendWithCR(AT_SLEEP);
	Console_WriteStringln(AT_SLEEP);

	while (_repeat < REPEAT_TIME) {
		while (UART0_RecvLineWait(_data, UART0_MAXBUFFLEN, &_dataLen)
		       == 0) {
			Console_WriteStringln(_data);

			if (strstr(_data, "OK")) {
				return 1;
			}
		}
		_repeat++;
		System_Delayms(TIMEWAIT);
	}

	return 0;
}

static int WakeMode() {

	int  _repeat		       = 0;
	int  _dataLen		       = 0;
	char _data[ UART0_MAXBUFFLEN ] = { 0 };

	Ec200SendWithCR(AT_WAKEUP);
	Console_WriteStringln(AT_WAKEUP);

	while (_repeat < REPEAT_TIME) {
		while (UART0_RecvLineWait(_data, UART0_MAXBUFFLEN, &_dataLen)
		       == 0) {
			Console_WriteStringln(_data);

			if (strstr(_data, "OK")) {
				return 1;
			}
		}
		_repeat++;
		System_Delayms(TIMEWAIT);
	}

	return 0;
}

static int TransmissionConfiguration() {

	int  _repeat		       = 0;
	int  _dataLen		       = 0;
	char _data[ UART0_MAXBUFFLEN ] = { 0 };

	Ec200SendWithCR(AT_QICSGP);
	Console_WriteStringln(AT_QICSGP);

	while (_repeat < REPEAT_TIME) {
		while (UART0_RecvLineWait(_data, UART0_MAXBUFFLEN, &_dataLen)
		       == 0) {
			Console_WriteStringln(_data);

			if (strstr(_data, "OK")) {
				return 1;
			}
		}
		_repeat++;
		System_Delayms(TIMEWAIT);
	}

	return 0;
}

static int TransmissionActivation() {

	int  _repeat		       = 0;
	int  _dataLen		       = 0;
	char _data[ UART0_MAXBUFFLEN ] = { 0 };

	Ec200SendWithCR(AT_QIACT);
	Console_WriteStringln(AT_QIACT);

	while (_repeat < REPEAT_TIME) {
		while (UART0_RecvLineWait(_data, UART0_MAXBUFFLEN, &_dataLen)
		       == 0) {
			Console_WriteStringln(_data);

			if (strstr(_data, "OK")) {
				return 1;
			}
		}
		_repeat++;
		System_Delayms(TIMEWAIT);
	}

	return 0;
}

static int TransmissionDeactivation() {

	int  _repeat		       = 0;
	int  _dataLen		       = 0;
	char _data[ UART0_MAXBUFFLEN ] = { 0 };

	Ec200SendWithCR(AT_QIDEACT);
	Console_WriteStringln(AT_QIDEACT);

	while (_repeat < REPEAT_TIME) {
		while (UART0_RecvLineWait(_data, UART0_MAXBUFFLEN, &_dataLen)
		       == 0) {
			Console_WriteStringln(_data);

			if (strstr(_data, "OK")) {
				return 1;
			}
		}
		_repeat++;
		System_Delayms(TIMEWAIT);
	}

	return 0;
}

static int CloseTran() {

	int  _repeat		       = 0;
	int  _dataLen		       = 0;
	char _data[ UART0_MAXBUFFLEN ] = { 0 };

	Ec200SendWithCR(AT_QICLOSE);
	Console_WriteStringln(AT_QICLOSE);

	while (_repeat < REPEAT_TIME) {
		while (UART0_RecvLineWait(_data, UART0_MAXBUFFLEN, &_dataLen)
		       == 0) {
			Console_WriteStringln(_data);

			if (strstr(_data, "OK")) {
				return 1;
			}
		}
		_repeat++;
		System_Delayms(TIMEWAIT);
	}

	return 0;
}

static int ExitTTMode() {

	int  _repeat		       = 0;
	int  _dataLen		       = 0;
	char _data[ UART0_MAXBUFFLEN ] = { 0 };

	while (_repeat < REPEAT_TIME) {
		Ec200Send(EXIT_TT_MODE);
		System_Delayms(TIMEWAIT);
		if (SetNoEcho()) {
			return 1;
		}
		_repeat++;
		System_Delayms(TIMEWAIT);
	}

	return 0;
}

static void judgeintlen(int* ip, char* iplen, char len) {
	char i;
	for (i = 0; i < len; i++) {
		if (ip[ i ] >= 100)
			iplen[ i ] = 3;
		else if (ip[ i ] >= 10)
			iplen[ i ] = 2;
		else
			iplen[ i ] = 1;
	}
}
static char judgeportlen(char* port) {
	char portlen[ 3 ] = { 0, 0, 0 };
	char i;

	for (i = 0; i < 3; i++) {
		if (port[ i ] >= 10)
			portlen[ i ] = 2;
		else if (port[ i ] > 0)
			portlen[ i ] = 1;
		else
			portlen[ i ] = 0;
	}
	return portlen[ 0 ] + portlen[ 1 ] + portlen[ 2 ];
}
#define AT_QIOPEN "AT+QIOPEN=1,0,\"TCP\",\"39.103.140.62\",9990,0,2"  // TCP连接
static int OpenTran(long center) {
	int  _cmdLen	   = 0;
	int  IPStrLen	   = 0;
	int  PortStrLen	   = 0;
	char _recv[ 30 ]   = { 0 };
	char centerip[ 6 ] = { 0, 0, 0, 0, 0, 0 };
	int  ip[ 4 ]	   = { 0, 0, 0, 0 };
	char iplen[ 4 ]	   = { 0, 0, 0, 0 };
	char i, j;
	char centerport[ 3 ] = { 0, 0, 0 };
	char centerportlen   = 0;
	char ipvalue[ 10 ];

	char _send[ 50 ] = "AT+QIOPEN=1,0,\"TCP\",\"";
	_cmdLen += 21;
	char _last[ 10 ] = ",0,2";
	int  _lastLen	 = Utility_Strlen(_last);

	memset(ipvalue, 0, sizeof(ipvalue));
	Hydrology_ReadStoreInfo(center, ipvalue, HYDROLOGY_CENTER_IP_LEN);
	TraceHexMsg(ipvalue, HYDROLOGY_CENTER_IP_LEN);
	memcpy(centerip, &ipvalue[ 1 ], 6);
	memcpy(centerport, &ipvalue[ 7 ], 3);

	//    Store_ReadHydrologyServerIP(centerip,center);
	centerip[ 0 ] = _BCDtoHEX(centerip[ 0 ]);
	centerip[ 1 ] = _BCDtoHEX(centerip[ 1 ]);
	centerip[ 2 ] = _BCDtoHEX(centerip[ 2 ]);
	centerip[ 3 ] = _BCDtoHEX(centerip[ 3 ]);
	centerip[ 4 ] = _BCDtoHEX(centerip[ 4 ]);
	centerip[ 5 ] = _BCDtoHEX(centerip[ 5 ]);

	ip[ 0 ] = centerip[ 0 ] * 10 + centerip[ 1 ] / 10;
	ip[ 1 ] = (centerip[ 1 ] % 10) * 100 + centerip[ 2 ];
	ip[ 2 ] = centerip[ 3 ] * 10 + centerip[ 4 ] / 10;
	ip[ 3 ] = (centerip[ 4 ] % 10) * 100 + centerip[ 5 ];

	judgeintlen(ip, iplen, 4);

	for (i = 0, j = 0; i < 4; i++) {
		if (iplen[ i ] == 3) {
			centerIP[ j ]	  = ip[ i ] / 100 + '0';
			centerIP[ j + 1 ] = ((ip[ i ] / 10) % 10) + '0';
			centerIP[ j + 2 ] = ip[ i ] % 10 + '0';
			centerIP[ j + 3 ] = '.';
			j += 4;
		}
		else if (iplen[ i ] == 2) {
			centerIP[ j ]	  = ip[ i ] / 10 + '0';
			centerIP[ j + 1 ] = ip[ i ] % 10 + '0';
			centerIP[ j + 2 ] = '.';
			j += 3;
		}
		else {
			centerIP[ j ]	  = ip[ i ] + '0';
			centerIP[ j + 1 ] = '.';
			j += 2;
		}
	}
	centerIP[ j - 1 ] = '\0';
	TraceMsg(centerIP, 1);

	IPStrLen = Utility_Strlen(centerIP);
	Utility_Strncpy(&_send[ _cmdLen ], centerIP, IPStrLen);
	_cmdLen += IPStrLen;

	_send[ _cmdLen ] = '\"';
	_cmdLen += 1;

	_send[ _cmdLen ] = ',';
	_cmdLen += 1;

	//    Store_ReadHydrologyServerPort(centerport,center);
	centerport[ 0 ] = _BCDtoHEX(centerport[ 0 ]);
	centerport[ 1 ] = _BCDtoHEX(centerport[ 1 ]);
	centerport[ 2 ] = _BCDtoHEX(centerport[ 2 ]);

	centerportlen = judgeportlen(centerport);

	switch (centerportlen) {
	case 6: {
		centerPort[ 0 ] = centerport[ 0 ] / 10 + '0';
		centerPort[ 1 ] = centerport[ 0 ] % 10 + '0';
		centerPort[ 2 ] = centerport[ 1 ] / 10 + '0';
		centerPort[ 3 ] = centerport[ 1 ] % 10 + '0';
		centerPort[ 4 ] = centerport[ 2 ] / 10 + '0';
		centerPort[ 5 ] = centerport[ 2 ] % 10 + '0';
		centerPort[ 6 ] = '\0';
		break;
	}
	case 5: {
		centerPort[ 0 ] = centerport[ 0 ] % 10 + '0';
		centerPort[ 1 ] = centerport[ 1 ] / 10 + '0';
		centerPort[ 2 ] = centerport[ 1 ] % 10 + '0';
		centerPort[ 3 ] = centerport[ 2 ] / 10 + '0';
		centerPort[ 4 ] = centerport[ 2 ] % 10 + '0';
		centerPort[ 5 ] = '\0';
		break;
	}
	case 4: {
		centerPort[ 0 ] = centerport[ 1 ] / 10 + '0';
		centerPort[ 1 ] = centerport[ 1 ] % 10 + '0';
		centerPort[ 2 ] = centerport[ 2 ] / 10 + '0';
		centerPort[ 3 ] = centerport[ 2 ] % 10 + '0';
		centerPort[ 4 ] = '\0';
		break;
	}
	case 3: {
		centerPort[ 0 ] = centerport[ 1 ] % 10 + '0';
		centerPort[ 1 ] = centerport[ 2 ] / 10 + '0';
		centerPort[ 2 ] = centerport[ 2 ] % 10 + '0';
		centerPort[ 3 ] = '\0';
		break;
	}
	case 2: {
		centerPort[ 0 ] = centerport[ 2 ] / 10 + '0';
		centerPort[ 1 ] = centerport[ 2 ] % 10 + '0';
		centerPort[ 2 ] = '\0';
		break;
	}
	case 1: {
		centerPort[ 0 ] = centerport[ 2 ] % 10 + '0';
		centerPort[ 1 ] = '\0';
		break;
	}
	}
	if (Utility_Strncmp("59.48.98.130", centerIP, 12) == 0) {
		Utility_Strncpy(centerPort, "5005", 4);
	}
	PortStrLen = Utility_Strlen(centerPort);
	Utility_Strncpy(&_send[ _cmdLen ], centerPort, PortStrLen);
	_cmdLen += PortStrLen;

	TraceMsg(centerPort, 1);

	Utility_Strncpy(&_send[ _cmdLen ], _last, _lastLen);

	int  _repeat		       = 0;
	int  _dataLen		       = 0;
	char _data[ UART0_MAXBUFFLEN ] = { 0 };

	Ec200SendWithCR(_send);
	Console_WriteStringln(_send);
	// Ec200SendWithCR(AT_QIOPEN);
	// Console_WriteStringln(AT_QIOPEN);

	while (_repeat < REPEAT_TIME) {
		while (UART0_RecvLineWait(_data, UART0_MAXBUFFLEN, &_dataLen)
		       == 0) {
			Console_WriteStringln(_data);

			if (strstr(_data, "CONNECT")) {
				return 1;
			}
			else {
				break;
			}
		}
		_repeat++;
		System_Delayms(TIMEWAIT);
	}

	return 0;
}

static int TransmissionInit(long center) {
	if (connected) {
		return 1;
	}

	int repeat = 0;

	while(!QueryCerg() && (repeat < REPEAT_TIME)){
		repeat++;
	}
	if(repeat == REPEAT_TIME){
		printf("network registration error\r\n");
		return 0;
	}
	repeat = 0;
	System_Delayms(1000);

	while(!QuerySIMSignal() && (repeat < REPEAT_TIME)){
		repeat++;
	}
	if(repeat == REPEAT_TIME){
		printf("singal too bad\r\n");
		return 0;
	}
	repeat = 0;
	System_Delayms(1000);

	while (!TransmissionConfiguration() && (repeat < REPEAT_TIME)) {
		repeat++;
	}
	if (repeat == REPEAT_TIME) {
		printf("TransmissionConfiguration failed");
		return 0;
	}
	repeat = 0;
	System_Delayms(1000);

	while (!TransmissionActivation() && (repeat < REPEAT_TIME)) {
		repeat++;
	}
	if (repeat == REPEAT_TIME) {
		printf("TransmissionActivation failed");
		return 0;
	}
	repeat = 0;
	System_Delayms(1000);

	while (!OpenTran(center) && (repeat < REPEAT_TIME)) {
		repeat++;
	}
	if (repeat == REPEAT_TIME) {
		printf("OpenTran failed");
		return 0;
	}
	repeat = 0;
	System_Delayms(1000);

	connected = 1;
	return 1;
}

static int TransmissionEnd() {
	int repeat = 0;

	while (!ExitTTMode() && (repeat < REPEAT_TIME)) {
		repeat++;
	}
	if (repeat == REPEAT_TIME) {
		return 0;
	}
	repeat = 0;

	while (!CloseTran() && (repeat < REPEAT_TIME)) {
		repeat++;
	}
	if (repeat == REPEAT_TIME) {
		return 0;
	}
	repeat = 0;

	while (!TransmissionDeactivation() && (repeat < REPEAT_TIME)) {
		repeat++;
	}
	if (repeat == REPEAT_TIME) {
		return 0;
	}
	repeat = 0;

	connected = 0;
	return 1;
}

static rtc_time_t get_time() {
	/* char *year,char *month,char *date,char *hour,char *min,char *second
	 */
	int	   _repeat		     = 0;
	int	   _dataLen		     = 0;
	char	   _data[ UART0_MAXBUFFLEN ] = { 0 };
	int	   rcvFlag		     = 0;
	rtc_time_t time;

	if (connected) {
		time.year = 0;
		return time;
	}

	Ec200SendWithCR("AT+CCLK?");
	// Console_WriteStringln("AT+CCLK?");

	while (_repeat < REPEAT_TIME) {
		if (UART0_RecvLineWait(_data, UART0_MAXBUFFLEN, &_dataLen)
		    == 0)  //������
		{
			Console_WriteStringln(_data);
			if (strstr(_data, "+CCLK:") != NULL) {
				rcvFlag = 1;
				break;
			}
		}
		_repeat++;
		System_Delayms(50);
	}

	/* +CCLK: */
	if (rcvFlag) {
		char* substr   = "+CCLK:";
		char* pcAtResp = strstr(_data, substr);
		int   offset   = 8;
		char  year1, year2, month1, month2, date1, date2, hour1, hour2,
			min1, min2, sec1, sec2;
		year1 = *(pcAtResp + offset);
		offset++;
		year2 = *(pcAtResp + offset);
		offset++;
		offset++;
		month1 = *(pcAtResp + offset);
		offset++;
		month2 = *(pcAtResp + offset);
		offset++;
		offset++;
		date1 = *(pcAtResp + offset);
		offset++;
		date2 = *(pcAtResp + offset);
		offset++;
		offset++;
		hour1 = *(pcAtResp + offset);
		offset++;
		hour2 = *(pcAtResp + offset);
		offset++;
		offset++;
		min1 = *(pcAtResp + offset);
		offset++;
		min2 = *(pcAtResp + offset);
		offset++;
		offset++;
		sec1 = *(pcAtResp + offset);
		offset++;
		sec2 = *(pcAtResp + offset);

		/* char *year,char *month,char *date,char *hour,char *min,char
		 * *second */
		time.year  = (year1 - 48) * 10 + (year2 - 48);
		time.month = (month1 - 48) * 10 + (month2 - 48);
		time.date  = (date1 - 48) * 10 + (date2 - 48);
		time.hour  = ((hour1 - 48) * 10 + (hour2 - 48)) + 8;
		time.min   = (min1 - 48) * 10 + (min2 - 48);
		time.sec   = (sec1 - 48) * 10 + (sec2 - 48);
	}
	else {
		time.year = 0;
	}
	return time;
}

int Ec200CheckIfNormal(void) {
	if (SetNoEcho()) {
		return 0;
	}
	return -1;
}

int Ec200Poweron(void) {
	while (power == 0) {
		PowerOn();

		for (int i = 0; i < 3; i++) {
			if (Ec200CheckIfNormal() != 0) {
				printf("wait for boot\r\n");
				;
			}
			else {
				printf("power on\r\n");
				power = 1;
				break;
			}
		}
	}
	System_Delayms(3000);

	return 0;
}

int Ec200Poweroff(void) {
	if (power == 1) {
		PowerOff();
		power = 0;
	}

	return 0;
}

int Ec200Sleep(void) {
	if (SleepMode()) {
		return 0;
	}
	return -1;
}

int Ec200Wakeup(void) {
	if (WakeMode()) {
		return 0;
	}
	return -1;
}

int Ec200SendMsg(char* pSend, int sendDataLen, int isLastPacket, long center) {
	// if(!TransmissionInit(center)){
	// 	return -1;
	// }

	// Ec200SendWithCR(pSend);
	Ec200SendWithLen(pSend, sendDataLen);

	// System_Delayms(SEND_TIMEWAIT);
	// TransmissionEnd();

	return 0;
}

char* Ec200RecvMsg(int* len) {
	char* _data = ( char* )malloc(UART0_MAXBUFFLEN);

	while (UART0_RecvLineWait(_data, UART0_MAXBUFFLEN, len) == 0) {
		Console_WriteStringln("Ec200t recv data:");
		Console_WriteStringln(_data);
	}
	if (*len == 0) {
		free(_data);
		_data = NULL;
	}

	return _data;
}

int Ec200CreateLink(long center) {
	if (TransmissionInit(center)) {
		printf("tcp connection established\r\n");
		return 0;
	}
	return -1;
}

int Ec200CloseLink(void) {
	if (TransmissionEnd()) {
		return 0;
	}
	return -1;
}

rtc_time_t Ec200GetNetworktime(void) {

	rtc_time_t res;
	memset(&res, 0, sizeof(rtc_time_t));
	int repeat = 0;

	// while(!QuerySIMSignal() && (repeat < REPEAT_TIME)){
	// 	repeat++;
	// }
	// if(repeat == REPEAT_TIME){
	// 	printf("singal too bad\r\n");
	// 	return res;
	// }

	repeat = 0;
	while (repeat < 10) {
		rtc_time_t tmp = get_time();
		if (tmp.year != 0 && tmp.year != 13) {
			return tmp;
		}
		repeat++;
	}

	return res;
}

static int COPS() {

	int  _repeat		       = 0;
	int  _dataLen		       = 0;
	char _data[ UART0_MAXBUFFLEN ] = { 0 };

	Ec200SendWithCR("AT+COPS?");
	Console_WriteStringln("AT+COPS?");

	while (_repeat < REPEAT_TIME) {
		while (UART0_RecvLineWait(_data, UART0_MAXBUFFLEN, &_dataLen)
		       == 0) {
			Console_WriteStringln(_data);

			if (strstr(_data, "OK")) {
				return 1;
			}
		}
		_repeat++;
		System_Delayms(TIMEWAIT);
	}

	return 0;
}

static int QSIMSTAT() {

	int  _repeat		       = 0;
	int  _dataLen		       = 0;
	char _data[ UART0_MAXBUFFLEN ] = { 0 };

	Ec200SendWithCR("AT+QSIMSTAT?");
	Console_WriteStringln("AT+QSIMSTAT?");

	while (_repeat < REPEAT_TIME) {
		while (UART0_RecvLineWait(_data, UART0_MAXBUFFLEN, &_dataLen)
		       == 0) {
			Console_WriteStringln(_data);

			if (strstr(_data, "OK")) {
				return 1;
			}
		}
		_repeat++;
		System_Delayms(TIMEWAIT);
	}

	return 0;
}

void ec200_unit_test() {

	while (1) {
		Ec200Poweron();


		rtc_time_t time = Ec200GetNetworktime();
		printf("%u %u %u %u %u %u\r\n", time.year, time.month,
		       time.date, time.hour, time.min, time.sec);
		System_Delayms(1000);

		Ec200CreateLink(HYDROLOGY_CENTER1_IP);
		// Ec200SendMsg("hello", 6, 0, HYDROLOGY_CENTER1_IP);
		// System_Delayms(5000);
		// int len = 0;
		// char *msg;
		// msg = Ec200RecvMsg(&len);
		// free(msg);
		Ec200CloseLink();

		Ec200Poweroff();

		printf("\r\n---------------------\r\n");
	}
}