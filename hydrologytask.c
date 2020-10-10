#include "hydrologytask.h"
#include "Sampler.h"
#include "adc.h"
#include "common.h"
#include "ec200.h"
#include "main.h"
#include "math.h"
#include "msp430common.h"
#include "packet.h"
#include "rtc.h"
#include "stdint.h"
#include "store.h"
//#include "convertsampledata.h"
//#include "hydrology.h"
//#include "hydrologmakebody.h"
#include "GTM900C.h"
#include "hydrologycommand.h"
#include "led.h"
#include "message.h"
#include "string.h"
#include "timer.h"
#include "uart1.h"
#include "uart3.h"
#include "uart_config.h"

// char LinkMaintenance_Flag = 0;
// char Test_Flag = 0;
// char EvenPeriodInformation_Flag = 0;
// char TimerReport_Flag = 0;
// char AddReport_Flag = 0;
// char Hour_Flag = 1;
// char ArtificialNumber_Flag = 0;
// char Picture_Flag = 0;
// char Realtime_Flag = 0;
// char Period_Flag = 0;
// char InquireArtificialNumber_Flag = 0;
// char SpecifiedElement_Flag = 0;
// char ConfigurationModification_Flag = 0;
// char ConfigurationRead_Flag = 0;
// char ParameterModification_Flag = 0;
// char ParameterRead_Flag = 0;
// char WaterPumpMotor_Flag = 0;
// char SoftwareVersion_Flag = 0;
// char Status_Flag = 0;
// char InitializeSolidStorage_Flag = 0;
// char Reset_Flag = 0;
// char ChangePassword_Flag = 0;
// char SetClock_Flag = 0;
// char SetICCard_Flag = 0;
// char Pump_Flag = 0;
// char Valve_Flag = 0;
// char Gate_Flag = 0;
// char WaterSetting_Flag = 0;
// char Record_Flag = 0;
// char Time_Flag = 0;

extern int		UserElementCount;    //++++++++++++++++++++++++++++++++
extern int		RS485RegisterCount;  //++++++++++
extern int		IsDebug;
extern int		DataPacketLen;
extern hydrologyElement inputPara[ MAX_ELEMENT ];
// extern hydrologyElement outputPara[MAX_ELEMENT];
uint16_t time_10min = 0, time_5min = 0, time_1min = 1, time_1s = 0;

int TryToSyncTime() {
	TraceMsg("trying to sync time through network.... ", 1);
	char newtime[ 6 ] = { 0 };
	// Ec200Poweron();
	System_Delayms(50);
	rtc_time_t rtc_time = Ec200GetNetworktime();
	// Ec200Poweroff();
	if (rtc_time.year != 0) {
		_RTC_SetTime(rtc_time.sec, rtc_time.min, rtc_time.hour,
			     rtc_time.date, rtc_time.month, 1, rtc_time.year,
			     0);  //修改RTC的系统时间
		printf("update time through network success: %d/%d/%d "
		       "%d:%d:%d\r\n",
		       rtc_time.year, rtc_time.month, rtc_time.date,
		       rtc_time.hour, rtc_time.min, rtc_time.sec);
		char rtc_nowTime[ 6 ];
		char date, control;
		_RTC_ReadTime(&rtc_nowTime[ 5 ], &rtc_nowTime[ 4 ],
			      &rtc_nowTime[ 3 ], &rtc_nowTime[ 2 ],
			      &rtc_nowTime[ 1 ], &date, &rtc_nowTime[ 0 ],
			      &control);
		printf("rtc time: %d/%d/%d %d:%d:%d \r\n", rtc_nowTime[ 0 ],
		       rtc_nowTime[ 1 ], rtc_nowTime[ 2 ], rtc_nowTime[ 3 ],
		       rtc_nowTime[ 4 ], rtc_nowTime[ 5 ]);
		Hydrology_TimeCheck();
		return 0;
	}
	return -1;
}
void HydrologyTimeBase() {
	time_1min++;
	time_5min++;
	time_10min++;
}

void convertSampleTimetoHydrology(char* src, char* dst) {
	dst[ 0 ] = _HEXtoBCD(src[ 0 ]);
	dst[ 1 ] = _HEXtoBCD(src[ 1 ]);
	dst[ 2 ] = _HEXtoBCD(src[ 2 ]);
	dst[ 3 ] = _HEXtoBCD(src[ 3 ]);
	dst[ 4 ] = _HEXtoBCD(src[ 4 ]);
}

void convertSendTimetoHydrology(char* src, char* dst) {
	dst[ 0 ] = _HEXtoBCD(src[ 0 ]);
	dst[ 1 ] = _HEXtoBCD(src[ 1 ]);
	dst[ 2 ] = _HEXtoBCD(src[ 2 ]);
	dst[ 3 ] = _HEXtoBCD(src[ 3 ]);
	dst[ 4 ] = _HEXtoBCD(src[ 4 ]);
	dst[ 5 ] = _HEXtoBCD(src[ 5 ]);
}

void convert_array_bcd2dec(char* array, int len) {
	for (int i = 0; i < len; i++) {
		array[ i ] = _BCDtoHEX(array[ i ]);
	}
}
float ConvertAnalog(int v, int index) {
	float tmp;
	char  range[ HYDROLOGY_ADC_RANGE_LEN ]		   = { 0, 1 };
	char  coefficient[ HYDROLOGY_ADC_COEFFICIENT_LEN ] = { 1, 0 };
	char  offset[ HYDROLOGY_ADC_OFFSET_LEN ]	   = { 0 };
	char  base[ HYDROLOGY_ADC_BASE_LEN ]		   = { 0 };
	char  upper, lower, warnning1, warnning2;
	float Range, Coefficient, Offset;
	float Base;

	//  Hydrology_ReadStoreInfo(HYDROLOGY_ADC_RANGE1+index*HYDROLOGY_ADC_RANGE_LEN,range,HYDROLOGY_ADC_RANGE_LEN);
	//  Hydrology_ReadStoreInfo(HYDROLOGY_ADC_COEFFICIENT1+index*HYDROLOGY_ADC_COEFFICIENT_LEN,coefficient,HYDROLOGY_ADC_COEFFICIENT_LEN);
	//  Hydrology_ReadStoreInfo(HYDROLOGY_ADC_OFFSET1+index*HYDROLOGY_ADC_OFFSET_LEN,offset,HYDROLOGY_ADC_OFFSET_LEN);
	//  Hydrology_ReadStoreInfo(HYDROLOGY_ADC_BASE1+index*HYDROLOGY_ADC_BASE_LEN,base,HYDROLOGY_ADC_BASE_LEN);
	//  Hydrology_ReadStoreInfo(HYDROLOGY_ADC_UPPER1+index*HYDROLOGY_ADC_UPPER_LEN,&upper,HYDROLOGY_ADC_UPPER_LEN);
	//  Hydrology_ReadStoreInfo(HYDROLOGY_ADC_LOWER1+index*HYDROLOGY_ADC_LOWER_LEN,&lower,HYDROLOGY_ADC_LOWER_LEN);
	//  Hydrology_ReadStoreInfo(HYDROLOGY_ADC_WARNNING1_1+index*HYDROLOGY_ADC_WARNNING_LEN,&warnning1,HYDROLOGY_ADC_WARNNING_LEN);
	//  Hydrology_ReadStoreInfo(HYDROLOGY_ADC_WARNNING2_1+index*HYDROLOGY_ADC_WARNNING_LEN,&warnning2,HYDROLOGY_ADC_WARNNING_LEN);

	Read_Flash_Segment(HYDROLOGY_ADC_RANGE1_BP
				   + index * HYDROLOGY_ADC_RANGE_LEN,
			   range, HYDROLOGY_ADC_RANGE_LEN);
	Read_Flash_Segment(HYDROLOGY_ADC_COEFFICIENT1_BP
				   + index * HYDROLOGY_ADC_COEFFICIENT_LEN,
			   coefficient, HYDROLOGY_ADC_COEFFICIENT_LEN);
	Read_Flash_Segment(HYDROLOGY_ADC_OFFSET1_BP
				   + index * HYDROLOGY_ADC_OFFSET_LEN,
			   offset, HYDROLOGY_ADC_OFFSET_LEN);
	Read_Flash_Segment(HYDROLOGY_ADC_BASE1_BP
				   + index * HYDROLOGY_ADC_BASE_LEN,
			   base, HYDROLOGY_ADC_BASE_LEN);
	Read_Flash_Segment(HYDROLOGY_ADC_UPPER1_BP
				   + index * HYDROLOGY_ADC_UPPER_LEN,
			   &upper, HYDROLOGY_ADC_UPPER_LEN);
	Read_Flash_Segment(HYDROLOGY_ADC_LOWER1_BP
				   + index * HYDROLOGY_ADC_LOWER_LEN,
			   &lower, HYDROLOGY_ADC_LOWER_LEN);
	Read_Flash_Segment(HYDROLOGY_ADC_WARNNING1_1_BP
				   + index * HYDROLOGY_ADC_WARNNING_LEN,
			   &warnning1, HYDROLOGY_ADC_WARNNING_LEN);
	Read_Flash_Segment(HYDROLOGY_ADC_WARNNING2_1_BP
				   + index * HYDROLOGY_ADC_WARNNING_LEN,
			   &warnning2, HYDROLOGY_ADC_WARNNING_LEN);

	convert_array_bcd2dec(range, HYDROLOGY_ADC_RANGE_LEN);
	convert_array_bcd2dec(coefficient, HYDROLOGY_ADC_COEFFICIENT_LEN);
	convert_array_bcd2dec(offset, HYDROLOGY_ADC_OFFSET_LEN);
	convert_array_bcd2dec(base, HYDROLOGY_ADC_BASE_LEN);
	convert_array_bcd2dec(&upper, HYDROLOGY_ADC_UPPER_LEN);
	convert_array_bcd2dec(&lower, HYDROLOGY_ADC_LOWER_LEN);
	convert_array_bcd2dec(&warnning1, HYDROLOGY_ADC_WARNNING_LEN);
	convert_array_bcd2dec(&warnning2, HYDROLOGY_ADC_WARNNING_LEN);

	Range	    = range[ 0 ] * 100 + range[ 1 ];
	Coefficient = coefficient[ 0 ] + (( float )coefficient[ 1 ]) / 100;
	Offset	    = offset[ 0 ] + (( float )offset[ 1 ]) / 100;
	//   Base = (double)((((base[0] * 100) + base[1] * 100) + base[2] * 100)
	//   + base[3]) +
	//                                         (double)((double)base[4] +
	//                                         ((double)base[5]/100))/100;
	Base = ( float )base[ 0 ];
	Base = Base * 100 + ( float )base[ 1 ];
	Base = Base * 100 + ( float )base[ 2 ];
	Base += (( float )base[ 3 ] / 100);
	//   Base = Base + Base1 + Base2;
	if (index == 4 || index == 10 || index == 11) {
		tmp = (( float )v / (4096.0)) * 2 * Range * Coefficient + Offset
		      + Base;  //电源电压、内部电压、内部温度，目前index=4时是电源电压
			       // lshb 200520
	}
	else {
		if (v < 620)
			v = 620;  //防止出现负值，4mA以下都取4mA（0.5V）
		tmp = (( float )(v - 620) / (2483.0)) * Range * Coefficient
		      + Offset + Base;	//外部4-20mA模拟量输入(v-620.0)
					/// (3103.0-620.0)  lshb 200520
	}
	return tmp;
}
/*
float ConvertAnalog(int v,int range)
{
  float tmp;
  int x = 3;
  tmp = v / (4096.0) * range * x;
  //tmp = (v -4096.0) / (4096.0) * range;
  return tmp;
}
*/

void ADC_Element(char* value, int index, int adc_i) {
	// int range[5] = {1,20,100,5000,4000}; //模拟量范围
	// int range[16] = {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};
	float floatvalue = 0;

	// floatvalue = ConvertAnalog(A[index+1],range[index]);
	floatvalue = ConvertAnalog(A[ index - 1 ], adc_i); /*++++++++++++++++*/
	memcpy(value, ( char* )(&floatvalue), 4);
}

char value[ 4 ] = { 0, 0, 0, 0 };

void HydrologyUpdateElementTable() /*++++++++++++++++++++++++++*/
{
	int  i	   = 0;
	char user  = 0;
	char rs485 = 0;

	/*加延时,备份*/
	// Hydrology_ReadStoreInfo(HYDROLOGY_USER_ElEMENT_COUNT,&user,HYDROLOGY_USER_ElEMENT_COUNT_LEN);
	Read_Flash_Segment(HYDROLOGY_USER_ElEMENT_COUNT_BP, &user,
			   HYDROLOGY_USER_ElEMENT_COUNT_LEN);
	// Hydrology_ReadStoreInfo(HYDROLOGY_RS485_REGISTER_COUNT,&rs485,HYDROLOGY_RS485_REGISTER_COUNT_LEN);
	Read_Flash_Segment(HYDROLOGY_RS485_REGISTER_COUNT_BP, &rs485,
			   HYDROLOGY_RS485_REGISTER_COUNT_LEN);
	System_Delayus(100);
	UserElementCount   = ( int )user;
	RS485RegisterCount = ( int )rs485;
	TraceInt4(UserElementCount, 1);
	TraceInt4(RS485RegisterCount, 1);
	for (i = 0; i < UserElementCount; i++) {

		//     Hydrology_ReadStoreInfo(HYDROLOGY_ELEMENT1_ID+i*HYDROLOGY_ELEMENT_ID_LEN,&Element_table[i].ID,HYDROLOGY_ELEMENT_ID_LEN);
		//     Hydrology_ReadStoreInfo(HYDROLOGY_ELEMENT1_TYPE+i*HYDROLOGY_ELEMENT_TYPE_LEN,&Element_table[i].type,HYDROLOGY_ELEMENT_TYPE_LEN);
		//     Hydrology_ReadStoreInfo(HYDROLOGY_ELEMENT1_MODE+i*HYDROLOGY_ELEMENT_MODE_LEN,&Element_table[i].Mode,HYDROLOGY_ELEMENT_MODE_LEN);
		//     Hydrology_ReadStoreInfo(HYDROLOGY_ELEMENT1_CHANNEL+i*HYDROLOGY_ELEMENT_CHANNEL_LEN,&Element_table[i].Channel,HYDROLOGY_ELEMENT_CHANNEL_LEN);

		Read_Flash_Segment(
			HYDROLOGY_ELEMENT1_ID_BP + i * HYDROLOGY_ELEMENT_ID_LEN,
			&Element_table[ i ].ID, HYDROLOGY_ELEMENT_ID_LEN);
		Read_Flash_Segment(HYDROLOGY_ELEMENT1_TYPE_BP
					   + i * HYDROLOGY_ELEMENT_TYPE_LEN,
				   &Element_table[ i ].type,
				   HYDROLOGY_ELEMENT_TYPE_LEN);
		Read_Flash_Segment(HYDROLOGY_ELEMENT1_MODE_BP
					   + i * HYDROLOGY_ELEMENT_MODE_LEN,
				   &Element_table[ i ].Mode,
				   HYDROLOGY_ELEMENT_MODE_LEN);
		Read_Flash_Segment(HYDROLOGY_ELEMENT1_CHANNEL_BP
					   + i * HYDROLOGY_ELEMENT_CHANNEL_LEN,
				   &Element_table[ i ].Channel,
				   HYDROLOGY_ELEMENT_CHANNEL_LEN);
		// Hydrology_ReadStoreInfo(HYDROLOGY_SWITCH1,temp_value,HYDROLOGY_SWITCH_LEN);
		getElementDd(
			Element_table[ i ].ID, &Element_table[ i ].D,
			&Element_table[ i ]
				 .d);  // D,d??????????????????????????????????��?
	}
	Element_table[ i ].ID	   = NULL;
	Element_table[ i ].type	   = NULL;
	Element_table[ i ].D	   = NULL;
	Element_table[ i ].d	   = NULL;
	Element_table[ i ].Mode	   = NULL;
	Element_table[ i ].Channel = NULL;
}
char s_isr_count_flag = 0;
char s_picture_flag   = 0;
void HydrologyDataPacketInit() {
	char packet_len = 0;
	int  i		= 0;
	packet_len += HYDROLOGY_DATA_SEND_FLAG_LEN;
	packet_len += HYDROLOGY_DATA_TIME_LEN;
	while (Element_table[ i ].ID != 0) {
		mallocElement(Element_table[ i ].ID, Element_table[ i ].D,
				Element_table[ i ].d, &inputPara[ i ]);
		packet_len += inputPara[ i ].num;
		i++;
	}
	DataPacketLen = packet_len;
	if(IsDebug){
		Hydrology_WriteStoreInfo(HYDROLOGY_DATA_PACKET_LEN, &packet_len,
					HYDROLOGY_DATA_PACKET_LEN_LEN);
	}
	//  Hydrology_ReadStoreInfo(HYDROLOGY_ISR_COUNT_FLAG,&s_isr_count_flag,HYDROLOGY_ISR_COUNT_FLAG_LEN);
	Read_Flash_Segment(HYDROLOGY_ISR_COUNT_FLAG_BP, &s_isr_count_flag,
			   HYDROLOGY_ISR_COUNT_FLAG_LEN);

	Hydrology_ReadStoreInfo(PICTURE_FLAG, &s_picture_flag,
				PICTURE_FLAG_LEN);  //++
	/*配置完后更新下时间，以防配置时间不准*/
	char newtime[ 6 ] = { 0 };
	Ec200Poweron();
	System_Delayms(50);
	rtc_time_t rtc_time = Ec200GetNetworktime();
	Ec200Poweroff();
	if (rtc_time.year != 0) {
		_RTC_SetTime(rtc_time.sec, rtc_time.min, rtc_time.hour,
			     rtc_time.date, rtc_time.month, 1, rtc_time.year,
			     0);  //修改RTC的系统时间
		printf("update time through network success: %d/%d/%d "
		       "%d:%d:%d\r\n",
		       rtc_time.year, rtc_time.month, rtc_time.date,
		       rtc_time.hour, rtc_time.min, rtc_time.sec);
	}
}

int HydrologySample(char* _saveTime) {

	int	     i = 0, j = 0;
	int	     adc_i = 0, isr_i = 0;
	int	     interval = 0;
	int	     a = 0, b = 0, c = 0;
	volatile int rs485_i		 = 0;
	long	     isr_count		 = 0;
	char	     io_i		 = 0;
	char	     isr_count_temp[ 5 ] = { 0 };
	char	     sampleinterval[ 2 ];
	char	     _temp_sampertime[ 6 ] = { 0 };
	static char  sampletime[ 6 ]	   = { 0 };
	/*
	 Hydrology_ReadStoreInfo(HYDROLOGY_SAMPLE_INTERVAL,sampleinterval,HYDROLOGY_SAMPLE_INTERVAL_LEN);
	 ////ly sampleinterval[0] = _BCDtoHEX(sampleinterval[0]);
	  sampleinterval[1] = _BCDtoHEX(sampleinterval[1]);
	  interval = sampleinterval[1]+sampleinterval[0]*100;*/

	// ly
	// 根据配置报文的存储时间间隔来判断当前是否到采样时间
	Utility_Strncpy(sampletime, _saveTime, 6);
	int ret = 0;
	ret	= Utility_Is_A_CheckTime(
		    sampletime);  //是否到采样时间
				  // int tmp = sampletime[4]%(interval/60);
	if (!ret && IsDebug == 0)
	// if(tmp!= 0 )
	{
		TraceMsg("Not Sample Time!", 1);
		return -1;
	}

	TraceMsg(" Start Sample:   ", 0);
	UART1_Open_9600(UART3_U485_TYPE);

	while (Element_table[ j ].ID != 0) {
		if (Element_table[ j ].Mode == ADC) {
			Sampler_Open();	 //打开24V电源
			ADC_Sample();
			Sampler_Close();  //关闭24V电源
			break;
		}
		j++;
	}

	while (Element_table[ i ].ID != 0) {
		memset(value, 0, sizeof(value));
		switch (Element_table[ i ].Mode) {
		case ADC: {
			// ADC_Sample();
			int index = 1;
			index	  = _BCDtoHEX(
				    Element_table[ i ].Channel);  // 0x10->10
			ADC_Element(value, index, adc_i);
			adc_i++;
			break;
		}
		case ISR_COUNT: {
			isr_i = _BCDtoHEX(Element_table[ i ].Channel);
			Hydrology_ReadStoreInfo(
				HYDROLOGY_ISR_COUNT1
					+ (isr_i - 1) * HYDROLOGY_ISR_COUNT_LEN,
				isr_count_temp, HYDROLOGY_ISR_COUNT_LEN);
			isr_count = (isr_count_temp[ 4 ] * 0x100000000)
				    + (isr_count_temp[ 3 ] * 0x1000000)
				    + (isr_count_temp[ 2 ] * 0x10000)
				    + (isr_count_temp[ 1 ] * 0x100)
				    + (isr_count_temp[ 0 ]);
			memcpy(value, ( char* )(&isr_count), 4);
			// isr_i++;
			break;
		}
		case IO_STATUS: {
			io_i = _BCDtoHEX(Element_table[ i ].Channel);
			Hydrology_ReadIO_STATUS(value, io_i);
			// io_i++;
			break;
		}
		case RS485: {
			// rs485_i = (int)Element_table[i].Channel;
			// //封装到下面的函数里了
			Hydrology_ReadRS485(value, rs485_i);
			rs485_i++;
			break;
		}
		}

		switch (Element_table[ i ].type) {
		case ANALOG: {
			convertSampleTimetoHydrology(g_rtc_nowTime,
						     _temp_sampertime);
			Hydrology_SetObservationTime(Element_table[ i ].ID,
						     _temp_sampertime, i);
			Hydrology_WriteStoreInfo(
				HYDROLOGY_ANALOG1 + a * HYDROLOGY_ANALOG_LEN,
				value, HYDROLOGY_ANALOG_LEN);
			a++;
			break;
		}
		case PULSE: {
			convertSampleTimetoHydrology(g_rtc_nowTime,
						     _temp_sampertime);
			Hydrology_SetObservationTime(Element_table[ i ].ID,
						     _temp_sampertime, i);
			Hydrology_WriteStoreInfo(
				HYDROLOGY_PULSE1 + b * HYDROLOGY_PULSE_LEN,
				value, HYDROLOGY_PULSE_LEN);
			b++;
			break;
		}
		case SWITCH: {
			convertSampleTimetoHydrology(g_rtc_nowTime,
						     _temp_sampertime);
			Hydrology_SetObservationTime(Element_table[ i ].ID,
						     _temp_sampertime, i);
			Hydrology_WriteStoreInfo(
				HYDROLOGY_SWITCH1 + c * HYDROLOGY_SWITCH_LEN,
				value, HYDROLOGY_SWITCH_LEN);
			c++;
			break;
		}
		}
		i++;
	}
	// UART2_Open(UART2_BT_TYPE);  // zh  bt_spp mode
	UART3_Open(UART3_CONSOLE_TYPE);
	TraceMsg("Sample Done!  ", 0);
	return 0;
}

int HydrologyOnline() {
	if (time_10min >= 1)
		// hydrologyProcessSend(LinkMaintenance);

		return 0;
}

int HydrologyOffline() {
	GPRS_Close_TCP_Link();
	GPRS_Close_GSM();

	return 0;
}

int HydrologySaveData(char* _saveTime, char funcode)  // char *_saveTime
{
	int   i = 0, acount = 0, pocunt = 0;
	float floatvalue = 0;
	long  intvalue1	 = 0;
	// int intvalue2 = 0;
	int  type	       = 0;
	int  cnt	       = 0;
	int  _effect_count     = 0;
	char switch_value[ 4 ] = { 0 };
	Hydrology_ReadDataPacketCount(
		&_effect_count);  //读取flash里还有多少数据包未发送
	type = hydrologyJudgeType(funcode);
	// char storeinterval;
	static char storetime[ 6 ] = { 0, 0, 0, 0, 0, 0 };
	/*
	Hydrology_ReadStoreInfo(HYDROLOGY_WATERLEVEL_STORE_INTERVAL,&storeinterval,HYDROLOGY_WATERLEVEL_STORE_INTERVAL_LEN);
	//ly storeinterval = _BCDtoHEX(storeinterval);
      */
	Utility_Strncpy(storetime, _saveTime, 6);
	// ly
	// 根据配置报文的存储时间间隔判断当前是否到达存储时间
	int ret = 0;
	ret = Utility_Is_A_SaveTime(storetime);	 //判断是否到达存储时间

	// int tmp = storetime[4]%(storeinterval);

	if (!ret && IsDebug == 0)
	//  if(tmp!= 0&&IsDebug==0)
	{
		TraceMsg("Not Save Time!", 1);
		return -1;
	}

	TraceMsg("Start Store:   ", 0);
	if (type == 1) {
		while (Element_table[ i ].ID != 0) {
			switch (Element_table[ i ].type) {
			case ANALOG: {
				Hydrology_ReadAnalog(&floatvalue, acount++);
				mallocElement(
					Element_table[ i ].ID,
					Element_table[ i ].D,
					Element_table[ i ].d,
					&inputPara
						[ i ]);	 // ly
							 // 根据配置报文的存储时间间隔判断当前是否到达存储时间
				converToHexElement(( double )floatvalue,
						   Element_table[ i ].D,
						   Element_table[ i ].d,
						   inputPara[ i ].value);
				break;
			}
			case PULSE: {
				Hydrology_ReadPulse(&intvalue1, pocunt++);
				mallocElement(Element_table[ i ].ID,
					      Element_table[ i ].D,
					      Element_table[ i ].d,
					      &inputPara[ i ]);
				converToHexElement(( double )intvalue1,
						   Element_table[ i ].D,
						   Element_table[ i ].d,
						   inputPara[ i ].value);
				break;
			}
			case SWITCH: {
				// Hydrology_ReadSwitch(&intvalue2);
				Hydrology_ReadSwitch(switch_value);
				mallocElement(Element_table[ i ].ID,
					      Element_table[ i ].D,
					      Element_table[ i ].d,
					      &inputPara[ i ]);
				// converToHexElement((double)intvalue2,Element_table[i].D,Element_table[i].d,inputPara[i].value);
				// memcpy(inputPara[i].value,switch_value,4);
				inputPara[ i ].value[ 0 ] = switch_value[ 3 ];
				inputPara[ i ].value[ 1 ] = switch_value[ 2 ];
				inputPara[ i ].value[ 2 ] = switch_value[ 1 ];
				inputPara[ i ].value[ 3 ] = switch_value[ 0 ];

				break;
			}
			case STORE: {
				inputPara[ i ].guide[ 0 ] =
					Element_table[ i ].ID;
				inputPara[ i ].guide[ 1 ] =
					Element_table[ i ].ID;
				inputPara[ i ].value =
					( char* )malloc(SinglePacketSize);
				if (NULL != inputPara[ i ].value) {
					inputPara[ i ].num = SinglePacketSize;
					// Hydrology_ReadRom(RomElementBeginAddr,inputPara[i].value,SinglePacketSendCount++);
				}
				break;
			}
			default:
				break;
			}
			i++;
			cnt++;
		}
	}

#if 0
   g_HydrologyDataPacket.send_flag = 0x00;
   Hydrology_ReadObservationTime(Element_table[0].ID,g_HydrologyDataPacket.observationtime,0);
  for(i = 0;i < cnt;i++)
  {
    memcpy((g_HydrologyDataPacket.element)[i].guide, inputPara[i].guide,2);
    (g_HydrologyDataPacket.element)[i].value = (char*) malloc(inputPara[i].num);
    if(NULL == (g_HydrologyDataPacket.element)[i].value)
       return -1;
    memcpy((g_HydrologyDataPacket.element)[i].value,inputPara[i].value, inputPara[i].num);
    (g_HydrologyDataPacket.element)[i].num = inputPara[i].num; 
  }
#else
	char _data[ HYDROLOGY_DATA_ITEM_LEN ] = {
		0
	};  //将要存储在flash里的数据条
	char observationtime[ 5 ];
	int  len   = 0;
	_data[ 0 ] = 0x00;  // 发送标记位初始化0x00
	Hydrology_ReadObservationTime(Element_table[ 0 ].ID, observationtime,
				      0);  // or save_time
	memcpy(&_data[ 1 ], observationtime, HYDROLOGY_OBSERVATION_TIME_LEN);
	len += 6;
	for (i = 0; i < cnt; i++) {
		memcpy(&_data[ len ], inputPara[ i ].value, inputPara[ i ].num);
		len += inputPara[ i ].num;
		if (inputPara[ i ].value != NULL) {
			free(inputPara[ i ].value);  //+++
			inputPara[ i ].value = NULL;
		}
	}
	/* 还没确定是否写入成功就先维护effect_count? */
	// ++_effect_count;  //待发送有效数据包数量增1
	// Hydrology_SetDataPacketCount(_effect_count);
#endif
	if (Store_WriteDataItemAuto(_data) < 0) {
		return -1;
	}
	TraceMsg("Save Data Success", 1);
	return 0;
}
int HydrologyInstantWaterLevel(char* _saveTime)	 //发送函数
{

	static char endtime[ 6 ] = { 0, 0, 0, 0, 0, 0 };
	static char time[ 6 ]	 = { 0 };
	Utility_Strncpy(endtime, _saveTime, 6);
	int ret	      = 0;
	int send_flag = 0;

	ret = Utility_Is_A_ReportTime(endtime);
	if (!ret) {
		TraceMsg(" Not Send Time!", 1);
		TraceMsg(" Time is:   ", 1);
		convertSendTimetoHydrology(endtime, time);
		TraceHexMsg(time, 6);
		return -1;
	          }
	/*
	 int tmp = endtime[4]%(timerinterval*5);
	 if(tmp!= 0 && !IsDebug)
	 {
	//if((endtime[4]-'0')%((timerinterval-'0')*5)!= 0 )
	 TraceMsg(" Time is:   ",0);
	 TraceHexMsg(endtime,5);
	   return -1;
	 }
     */
	int _effect_count = 0;
	Hydrology_ReadDataPacketCount(
		&_effect_count);  //读取flash还有多少数据包未发送
	TraceInt4(_effect_count, 1);

	int _startIdx = 0;
	int _endIdx   = 0;

	char _send[ 200 ] = { 0 };
	int  _ret	  = 0;
	int  _seek_num	  = 0;
	int  sendlen	  = 0;

	_ret = FlowCheckSampleData(&_startIdx, &_endIdx);  //校验并读取startidx
							   // endidx两个索引值
	if (_ret != 0) {
		return -1;
	}
	while (_effect_count != 0) {
		TraceMsg("read data in :", 0);
		TraceInt4(_startIdx, 1);
		TraceInt4(_effect_count, 1);
		if (_seek_num
		    > HYDROLOGY_DATA_MAX_IDX)  //寻找的数据条数已经超过最大值就退出，防止死循环
		{
			TraceMsg("seek num out of range", 1);
			// hydrologHEXfree();
			System_Delayms(2000);
			System_Reset();
		}

		_ret = Store_ReadDataItem(_startIdx, _send,
					  0);  //读取数据，ret为读出的数据长度

		if (_ret < 0) {
			TraceMsg("can't read data ! very bad .", 1);
			return -1;  //无法读取数据
				    //就直接退了.
		}
		else if (_ret == 1) {
			TraceMsg("It's sended data", 1);
			if (_startIdx
			    >= HYDROLOGY_DATA_MAX_IDX) {  // startidx超过最大，则置为最小，例如0-2000的循环
				_startIdx = HYDROLOGY_DATA_MIN_IDX;
			}
			else {
				++_startIdx;
			}
			++_seek_num;			   //下一数据
			Hydrology_SetStartIdx(_startIdx);  //更新_startIdx.
			TraceInt4(_startIdx, 1);
			TraceInt4(_endIdx, 1);
			// hydrologyExitSend();
		}
		else  // ret>1
		{
			sendlen = _ret;
			if (IsDebug == 1)  //测试报
			{
				hydrologyProcessSend(_send, Test);
				// hydrologyProcessSend(_send,TimerReport);
				// IsDebug = 0;
			}
			else  //正常运行发送定时报
			{
				send_flag = hydrologyProcessSend(
					_send,
					TimerReport);  // send_flag发送成功标志
			}
			if (send_flag == TRUE) {
				Store_MarkDataItemSended(
					_startIdx);  //发送完，需设置_startIdx对应的数据条的发送标记位
				--_effect_count;
				Hydrology_SetDataPacketCount(
					_effect_count);	 //更新_effect_count
				if (_startIdx >= HYDROLOGY_DATA_MAX_IDX) {
					_startIdx = HYDROLOGY_DATA_MIN_IDX;
				}
				else {
					++_startIdx;
				}
				++_seek_num;
				Hydrology_SetStartIdx(
					_startIdx);  //更新_startIdx.
						     //发送索引

				// TraceInt4(_startIdx, 1);
				// TraceInt4(_endIdx, 1);
				//  hydrologyExitSend();
			}
			else {	//有一组报文发送失败，则终止本轮发送，留到下一个发送时间点发送
				TraceMsg("network condition is poor,leave it "
					 "for the next time to send !",
					 1);
				break;
			}
		}
	}

	// TryToSyncTime();
	TraceMsg("Report done", 1);
	// System_Delayms(2000);
	/**************************************************************************************/
	// hydrologyProcessSend(TimerReport);

	if (!IsDebug) {
		printf("waiting msg..........\r\n");
		System_Delayms(5000);
		// Ec200Poweron();
		// JudgeServerDataArrived();
		// Hydrology_ProcessGPRSReceieve();
		// Ec200Poweroff();
		// JudgeServerDataArrived();
		// Hydrology_ProcessGPRSReceieve();
		// JudgeServerDataArrived();
		// Hydrology_ProcessGPRSReceieve();
		// JudgeServerDataArrived();
		Hydrology_ProcessGPRSReceieve();
	}
	Ec200CloseLink();
	Ec200Poweroff();

	time_10min = 0;

	endtime[ 0 ] = 0;
	endtime[ 1 ] = 0;
	endtime[ 2 ] = 0;
	endtime[ 3 ] = 0;
	endtime[ 4 ] = 0;
	endtime[ 5 ] = 0;

	return 0;
}

int HydrologyVoltage() {
	//    char _temp_voltage[4];
	//
	//    _temp_voltage[0] = A[0] >> 8;
	//    _temp_voltage[1] = A[0] & 0x00FF;
	//
	//    Store_SetHydrologyVoltage(_temp_voltage);
	//
	return 0;
}

/*检查时间*/
int Hydrology_TimeCheck() {
	RTC_ReadTimeBytes5(g_rtc_nowTime);  //读取RTC的时间存在g_rtc_nowtime
	// char _temp_str[18];

	int _time_error = 1;
	for (int i = 0; i < 11; ++i) {
		if (RTC_IsBadTime(g_rtc_nowTime, 1) == 0) {
			_time_error = 0;
			return 0;
		}
		TraceMsg("Time is bad once : ", 1);
		printf("%d/%d/%d %d:%d \r\n", g_rtc_nowTime[ 0 ],
		       g_rtc_nowTime[ 1 ], g_rtc_nowTime[ 2 ],
		       g_rtc_nowTime[ 3 ], g_rtc_nowTime[ 4 ]);
		RTC_ReadTimeBytes5(g_rtc_nowTime);
	}
	if (_time_error > 0) {	//依旧是错误时间
		TraceMsg("Device time is bad !", 1);
		TraceMsg("trying to sync time through network.... ", 1);
		char newtime[ 6 ] = { 0 };
		System_Delayms(50);
		rtc_time_t rtc_time = Ec200GetNetworktime();
		if (rtc_time.year != 0) {
			_RTC_SetTime(rtc_time.sec, rtc_time.min, rtc_time.hour,
				     rtc_time.date, rtc_time.month, 1,
				     rtc_time.year, 0);	 //修改RTC的系统时间
			printf("update time through network success: %d/%d/%d "
			       "%d:%d:%d\r\n",
			       rtc_time.year, rtc_time.month, rtc_time.date,
			       rtc_time.hour, rtc_time.min, rtc_time.sec);
			return 0;
		}
	}
	if (RTC_IsBadTime(g_rtc_nowTime, 0) < 0) {  //依旧是错误时间?
		TraceMsg("rtc time format illegal, rebooting..\r\n", 1);
		System_Reset();
		return -2;
	}
	else
		printf("rtc time format correct, keep running....\r\n");
	return 0;
}

int HydrologyTask() {
	char rtc_nowTime[ 6 ];
	int  sampleDone, storeDone, reportDone;
	TimerB_Clear();
	WatchDog_Clear();
	Hydrology_ProcessUARTReceieve();

	Ec200Poweron();
	Hydrology_TimeCheck();
	Ec200Poweroff();
	/*
	若打开main_gotoSleep():

		1. 处理下行报文
		2. 校时
		3. 休眠一分钟
		4.
	醒来看看是否到达配置报文里的时间间隔
			到达间隔：
	干活完返回1 没到间隔：
	直接返回1


	*/
	// if (!IsDebug) {
	// 	if (time_1min)
	// 		time_1min = 0;
	// 	else
	// 		// Main_GotoSleep();
	// 		return -1;
	// }

	RTC_ReadTimeBytes5(g_rtc_nowTime);
	RTC_ReadTimeBytes6(rtc_nowTime);

	sampleDone = HydrologySample(rtc_nowTime);		   //采样
	storeDone  = HydrologySaveData(rtc_nowTime, TimerReport);  //存储
	reportDone = HydrologyInstantWaterLevel(rtc_nowTime);	   //发送
	/*发送图片报*/
	char pictureinterval = 0;
	char _send[ 2 ]	     = { 0 };
	if (s_picture_flag == 0x01) {
		Hydrology_ReadStoreInfo(PICTURE_INTERVAL, &pictureinterval,
					PICTURE_INTERVAL_LEN);
		pictureinterval = _BCDtoHEX(pictureinterval);
		//判断时间间隔
		// int tmp = rtc_nowTime[3]%pictureinterval;
		if (rtc_nowTime[ 3 ] % (pictureinterval)
		    && rtc_nowTime[ 4 ] == 0) {
			hydrologyProcessSend(_send, Picture);
		}
	}

	if (sampleDone == 0 || storeDone == 0 || reportDone == 0) {
		return 0;
	}
	else {
		return -1;
	}
}
