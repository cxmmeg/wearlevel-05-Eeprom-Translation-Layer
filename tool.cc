#include "tool.h"
#include "common.h"
#include "rtc.h"
#include "tool.h"
#include <assert.h>
#include <ctime>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void Tool::SwapMemory(void* mem1, void* mem2, unsigned int length) {
	char* buffer = ( char* )malloc(length + 1);
	assert(buffer);

	memcpy(buffer, ( char* )mem1, length);
	memcpy(mem1, mem2, length);
	memcpy(( char* )mem2, buffer, length);

	free(buffer);
	buffer = NULL;
}
int Tool::GetRandomNum(int max) {
	Time rtc_time;
	rtc_time.GetRtcTime();

	struct tm local_time;
	local_time.tm_sec  = rtc_time.sec;
	local_time.tm_min  = rtc_time.min;
	local_time.tm_hour = rtc_time.hour;
	local_time.tm_mday = rtc_time.date;
	local_time.tm_mon  = rtc_time.month;
	local_time.tm_year = rtc_time.year;

	time_t time_stamp = mktime(&local_time);

	srand(( unsigned int )time_stamp);
	return rand() % max;
}