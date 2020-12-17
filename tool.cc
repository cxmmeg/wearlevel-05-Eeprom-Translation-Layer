#include "tool.h"
#include "common.h"
#include "rtc.h"
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

void Tool::SetBit(char* data, int pos) {
	data[ pos / 8 ] |= 1 << (pos % 8);
}
void Tool::UnSetBit(char* data, int pos) {
	data[ pos / 8 ] &= ~(1 << (pos % 8));
}

bool Tool::IsBitSet(char* data, int pos) {
	if (data[ pos / 8 ] & (1 << (pos % 8)))
		return true;
	else
		return false;
}
bool Tool::IsBitUnSet(char* data, int pos) {
	if (Tool::IsBitSet(data, pos))
		return false;
	return true;
}

void Tool::SetBit(vector< char >& data, int pos) {
	data[ pos / 8 ] |= 1 << (pos % 8);
}
void Tool::UnSetBit(vector< char >& data, int pos) {
	data[ pos / 8 ] &= ~(1 << (pos % 8));
}

bool Tool::IsBitSet(const vector< char >& data, int pos) {
	if (data[ pos / 8 ] & (1 << (pos % 8)))
		return true;
	else
		return false;
}
bool Tool::IsBitUnSet(const vector< char >& data, int pos) {
	if (Tool::IsBitSet(data, pos))
		return false;
	return true;
}
static int CalcCharSelBitCnt(char data) {
	int cnt = 0;
	while (data > 0) {
		cnt++;
		data &= (data - 1);
	}
	return cnt;
}

static int GetCharSelBitCnt(char data) {
	static vector< int > dict;
	if (dict.size() == 0) {
		for (int i = 0; i <= 0xFF; i++)
			dict.push_back(CalcCharSelBitCnt(i));
	}

	return dict[ data ];
}

int Tool::CountSelBitCnt(const vector< char >& bitmap) {
	int cnt = 0;
	int len = bitmap.size();
	for (int i = 0; i < len; i++) {
		cnt += GetCharSelBitCnt(bitmap[ i ]);
	}
	return cnt;
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

/*++++++++++++++++++++Test++++++++++++++++++++++++*/
void BitOperationTest() {
	char test_cast[] = { 0x0F, 0x0F };

	for (int pos = 0; pos < 16; pos++) {
		if (Tool::IsBitSet(test_cast, pos))
			LOG_INFO("bit%d is set\r\n", pos);

		if (Tool::IsBitUnSet(test_cast, pos))
			LOG_INFO("bit%d is unset\r\n", pos);
	}
}

void GetRandomNumTest() {
	for (int i = 0; i < 100; i++) {
		LOG_INFO("round %d : %d \r\n", i, Tool::GetRandomNum(100));
		Delay(1);
	}
}

/*------------------Test------------------------------*/