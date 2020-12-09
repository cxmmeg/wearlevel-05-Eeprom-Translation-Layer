#include "lowpower.h"
#include "common.h"
#include "main.h"
#include "msp430common.h"
#include "rtc.h"

int	   g_sleep_interval_type = INTERVAL_MIN;
int	   g_sleep_interval	 = 5;
extern int IsDebug;

/* 板子上电，RTC_INT引脚拉低,让继电器合上 */
void BoardPowerOn() {
	P2DIR |= BIT0;
	P2OUT &= ~BIT0;
}
void BoardPowerOff() {
	P2DIR |= BIT0;
	P2OUT |= BIT0;
}

void CalcNextWakeupTime(char* nextHour, char* nextMin) {
	char interval	  = g_sleep_interval;
	char intervalType = g_sleep_interval_type;
	char currHour	  = _BCDtoHEX(_RTC_Read_OneByte(RegAddr_Hour));
	char currMin	  = _BCDtoHEX(_RTC_Read_OneByte(RegAddr_Min));
	if (currHour < 0 || currHour >= 24) {
		printf("currHour error : %d:%d", currHour, currMin);
		return;
	}

	if (currMin < 0 || currMin >= 60) {
		printf("currMin error : %d:%d", currHour, currMin);
		return;
	}

	if (intervalType == INTERVAL_HOUR) {
		if (interval < 0 || interval >= 24) {
			printf("interval error : %d \n", interval);
			return;
		}
		char carry = currMin >= 59 ? 1 : 0;
		*nextHour = (((currHour + carry) / interval + 1) * interval - 1)
			    % 24;
		*nextMin = 59;
	}
	else {
		if (interval < 0 || interval >= 60) {
			printf("interval error : %d \n", interval);
			return;
		}
		char carry = (currMin + 1) % interval == 0 ? 1 : 0;
		*nextMin   = (((currMin + carry) / interval + 1) * interval - 1)
			   % 60;
		*nextHour =
			(currHour
			 + ((((currMin + carry) / interval + 1) * interval - 1)
			    / 60))
			% 24;
	}
}

void SleepIfWakeupAcceidently() {

	char rtc_nowTime[ 6 ];
	char date, control;
	_RTC_ReadTime(&rtc_nowTime[ 5 ], &rtc_nowTime[ 4 ], &rtc_nowTime[ 3 ],
		      &rtc_nowTime[ 2 ], &rtc_nowTime[ 1 ], &date,
		      &rtc_nowTime[ 0 ], &control);
	char curr_hour = rtc_nowTime[ 3 ];
	char curr_min  = rtc_nowTime[ 4 ];

	printf("curr time : %d:%d \r\n", curr_hour, curr_min);

	/* 如果是正常唤醒时间，直接返回，继续往下走 */
	if (g_sleep_interval_type == INTERVAL_MIN
	    && (curr_min + 1) % g_sleep_interval == 0)
		return;
	if (g_sleep_interval_type == INTERVAL_HOUR
	    && (curr_hour + 1) % g_sleep_interval == 0 && curr_min == 59)
		return;

	char wakeup_hour, wakeup_min;
	CalcNextWakeupTime(&wakeup_hour, &wakeup_min);

	/* 如果是当前时间是异常唤醒，计算下距离唤醒时间还有多久，如果超过3分钟就继续休眠
	 */
	if (curr_hour <= wakeup_hour && curr_min < wakeup_min && !IsDebug
	    && (wakeup_hour - curr_hour) * 60 + wakeup_min - curr_min >= 3) {
		printf("accident wakeup, keep sleeping...\r\n");
		gotoSleepLPM3();
	}
}
void SetSleepInterval(int interval, int interval_type) {
	g_sleep_interval      = interval;
	g_sleep_interval_type = interval_type;
}

void gotoSleepLPM3() {

}